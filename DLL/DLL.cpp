#include "dataprocessor.h"
#include <windows.h>  // Windows API头文件
#include <iostream>   // 标准输入输出
#include <memory>     // 智能指针头文件
#include <string>

/**
 * @brief DLL加载器类
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
    explicit DllLoader(const wchar_t* dllPath) : hModule(nullptr) {
        // 加载DLL文件
        hModule = LoadLibraryW(dllPath);
        if (!hModule) {
            // 获取错误代码并转换为字符串
            DWORD errorCode = GetLastError();
            throw std::runtime_error("DLL加载失败，错误代码: " + std::to_string(errorCode));
        }

        // 获取创建函数地址
        createFunc = reinterpret_cast<CreateProcessorFunc>(
            GetProcAddress(hModule, "createDataProcessor"));
        if (!createFunc) {
            FreeLibrary(hModule);
            throw std::runtime_error("获取createDataProcessor函数失败");
        }

        // 获取销毁函数地址
        destroyFunc = reinterpret_cast<DestroyProcessorFunc>(
            GetProcAddress(hModule, "destroyDataProcessor"));
        if (!destroyFunc) {
            FreeLibrary(hModule);
            throw std::runtime_error("获取destroyDataProcessor函数失败");
        }
    }

    /**
     * @brief 析构函数自动释放DLL
     */
    ~DllLoader() {
        if (hModule) {
            FreeLibrary(hModule);  // 释放DLL资源
            hModule = nullptr;     // 重置句柄
        }
    }

    /**
     * @brief 创建处理器对象
     * @return 包装在unique_ptr中的处理器对象
     * @note 使用自定义删除器确保通过DLL函数销毁对象
     */
    std::unique_ptr<DataProcessor, std::function<void(DataProcessor*)>> createProcessor() {
        // 使用std::function包装删除器以支持lambda捕获
        return std::unique_ptr<DataProcessor, std::function<void(DataProcessor*)>>(
            createFunc(),  // 调用DLL创建函数
            [this](DataProcessor* p) { destroyFunc(p); }  // 自定义删除器
        );
    }

    // 禁止拷贝构造和赋值
    DllLoader(const DllLoader&) = delete;
    DllLoader& operator=(const DllLoader&) = delete;
};

/**
 * @brief 主程序入口
 * @return 程序执行状态码
 */
int main() {
    try {
        std::cout << "===== 开始数据处理 =====" << std::endl;

        // 1. 加载DLL（使用宽字符路径支持中文）
        DllLoader loader(L"dataprocessor.dll");

        // 2. 创建处理器实例（自动管理生命周期）
        auto processor = loader.createProcessor();

        // 3. 准备输入数据
        ProcessInput input;
        input.numbers = { 10, 20, 30, 40, 50 };  // 测试数据
        input.multiplier = 3;                   // 设置乘数因子

        // 4. 处理数据
        ProcessOutput output;
        processor->process(input, output);

        // 5. 显示处理结果
        processor->displayResults(output);

        std::cout << "===== 处理完成 =====" << std::endl;
    }
    catch (const std::exception& e) {
        // 捕获所有异常并打印错误信息
        std::cerr << "【系统错误】" << e.what() << std::endl;
        return EXIT_FAILURE;  // 返回非零错误码
    }

    return EXIT_SUCCESS;  // 正常退出
}