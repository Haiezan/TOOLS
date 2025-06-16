#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <vector>
#include <functional>
#include <memory>

/**
 * @brief 数据处理抽象接口类
 * @note 主程序和DLL都必须使用完全相同的定义
 */
class DataProcessor {
public:
    // 处理数据纯虚函数
    virtual void processData(std::vector<int>& data) = 0;

    // 打印结果纯虚函数
    virtual void printResults() const = 0;

    // 虚析构函数确保正确释放派生类资源
    virtual ~DataProcessor() = default;
};

// 定义创建处理器对象的函数指针类型
using CreateProcessorFunc = DataProcessor * (*)();

// 定义销毁处理器对象的函数指针类型
using DestroyProcessorFunc = void(*)(DataProcessor*);

#endif // DATA_PROCESSOR_H