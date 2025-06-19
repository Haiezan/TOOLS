#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <vector>
#include <memory>
#include <functional>

/**
 * @brief 输入数据结构体
 * @param numbers 待处理的整数数组
 * @param multiplier 数据乘数因子（默认值为1）
 */
struct ProcessInput {
    std::vector<int> numbers;  // 需要处理的数据集合
    int multiplier = 1;        // 数据放大系数，默认为1
};

/**
 * @brief 输出数据结构体
 * @param sum 数据总和
 * @param max 数据最大值
 * @param min 数据最小值
 * @param processed_data 处理后的数据副本
 */
struct ProcessOutput {
    int sum = 0;                // 存储计算总和结果
    int max = 0;                // 存储最大值结果
    int min = 0;                // 存储最小值结果
    std::vector<int> processed_data; // 存储处理后的数据副本
};

/**
 * @brief 数据处理抽象接口类
 * @note 所有具体处理器必须实现这些纯虚函数
 */
class DataProcessor {
public:
    /**
     * @brief 处理数据的纯虚函数
     * @param input 包含输入数据的结构体
     * @param output 用于接收处理结果的结构体
     */
    virtual void process(const ProcessInput& input, ProcessOutput& output) = 0;

    /**
     * @brief 显示处理结果的纯虚函数
     * @param output 包含处理结果的结构体
     */
    virtual void displayResults(const ProcessOutput& output) const = 0;

    /**
     * @brief 虚析构函数确保派生类正确释放资源
     */
    virtual ~DataProcessor() = default;
};

// 导出函数类型声明（C接口避免名称修饰问题）
extern "C" {
    /**
     * @brief 创建处理器实例的函数指针类型
     * @return 新创建的DataProcessor对象指针
     */
    typedef DataProcessor* (*CreateProcessorFunc)();

    /**
     * @brief 销毁处理器实例的函数指针类型
     * @param p 要销毁的DataProcessor对象指针
     */
    typedef void (*DestroyProcessorFunc)(DataProcessor*);
}

#endif // DATA_PROCESSOR_H