#include "dataprocessor.h"
#include <windows.h>  // Windows API头文件
#include <iostream>
#include <vector>
#include <memory>     // 智能指针头文件
#include <string>

/**
 * @brief DLL加载和管理类
 * @note 使用RAII机制管理DLL生命周期
 */
class DllLoader {
    HMODULE hModule;            // DLL模块句柄
    CreateProcessorFunc createFunc;  // 创建函数指针
    DestroyProcessorFunc destroyFunc; // 销毁函数指针

public:
    /**
     * @brief 构造函数加载DLL并获取函数地址
     * @param dllPath DLL文件路径
     * @throw std::runtime_error 如果加载失败
     */
    explicit DllLoader(const wchar_t* dllPath) {
        // 加载DLL文件
        hModule = LoadLibraryW(dllPath);
        if (!hModule) {
            throw std::runtime_error("DLL加载失败，错误代码: " + std::to_string(GetLastError()));
        }

        // 获取创建函数地址
        createFunc = reinterpret_cast<CreateProcessorFunc>(
            GetProcAddress(hModule, "createDataProcessor"));

        // 获取销毁函数地址
        destroyFunc = reinterpret_cast<DestroyProcessorFunc>(
            GetProcAddress(hModule, "destroyDataProcessor"));

        // 检查是否成功获取两个函数
        if (!createFunc || !destroyFunc) {
            FreeLibrary(hModule);  // 释放已加载的DLL
            throw std::runtime_error("获取DLL函数地址失败");
        }
    }

    // 禁止拷贝构造和赋值
    DllLoader(const DllLoader&) = delete;
    DllLoader& operator=(const DllLoader&) = delete;

    /**
     * @brief 析构函数自动释放DLL
     */
    ~DllLoader() {
        if (hModule) {
            FreeLibrary(hModule);  // 释放DLL
        }
    }

    /**
     * @brief 创建处理器对象
     * @return 包装在unique_ptr中的处理器对象
     * @note 使用自定义删除器确保通过DLL函数销毁对象
     */
    std::unique_ptr<DataProcessor, std::function<void(DataProcessor*)>> createProcessor() {
        return std::unique_ptr<DataProcessor, std::function<void(DataProcessor*)>>(
            createFunc(),  // 调用DLL创建函数
            [this](DataProcessor* p) { destroyFunc(p); }  // 自定义删除器
        );
    }
};

int main() {
    try {
        std::cout << "===== 开始数据处理 =====" << std::endl;

        // 1. 加载DLL
        DllLoader loader(L"dataprocessor.dll");

        // 2. 创建处理器对象（自动管理生命周期）
        auto processor = loader.createProcessor();

        // 3. 准备测试数据
        std::vector<int> data{ 1, 2, 3, 4, 5 };
        std::cout << "原始数据: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 4. 处理数据（数据将通过引用传递到DLL中处理）
        processor->processData(data);

        // 5. 显示处理结果
        processor->printResults();

        // 6. 显示被修改后的数据
        std::cout << "处理后的数据: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "【错误】" << e.what() << std::endl;
        return 1;
    }

    std::cout << "===== 程序正常结束 =====" << std::endl;
    return 0;
}