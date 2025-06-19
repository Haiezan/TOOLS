#include "dataprocessor.h"
#include <numeric>  // 包含数值算法头文件
#include <algorithm> // 包含STL算法头文件
#include <iostream>  // 包含输入输出头文件

/**
 * @brief 数据处理具体实现类
 * @note 继承自DataProcessor接口，实现所有纯虚函数
 */
class DataProcessorImpl : public DataProcessor {
public:
    /**
     * @brief 具体的数据处理实现
     * @param input 输入数据
     * @param output 输出结果
     */
    void process(const ProcessInput& input, ProcessOutput& output) override {
        // 验证输入数据是否为空
        if (input.numbers.empty()) {
            throw std::invalid_argument("输入数据不能为空");
        }

        // 计算数据总和（使用STL accumulate算法）
        output.sum = std::accumulate(input.numbers.begin(),
            input.numbers.end(), 0);

        // 查找最大值（使用STL max_element算法）
        output.max = *std::max_element(input.numbers.begin(),
            input.numbers.end());

        // 查找最小值（使用STL min_element算法）
        output.min = *std::min_element(input.numbers.begin(),
            input.numbers.end());

        // 处理数据：每个元素乘以乘数因子
        output.processed_data.reserve(input.numbers.size()); // 预分配空间
        for (int num : input.numbers) {
            output.processed_data.push_back(num * input.multiplier);
        }
    }

    /**
     * @brief 显示处理结果的具体实现
     * @param output 包含处理结果的结构体
     */
    void displayResults(const ProcessOutput& output) const override {
        std::cout << "======= 数据处理结果 =======\n";
        std::cout << "■ 数据总和: " << output.sum << "\n";
        std::cout << "■ 最大值: " << output.max << "\n";
        std::cout << "■ 最小值: " << output.min << "\n";

        std::cout << "■ 处理后数据: ";
        for (int num : output.processed_data) {
            std::cout << num << " ";
        }
        std::cout << "\n===========================\n";
    }
};

// 导出C风格接口函数（避免名称修饰问题）
extern "C" {
    /**
     * @brief 创建处理器实例的导出函数
     * @return 新创建的DataProcessorImpl对象指针
     * @note 使用__declspec(dllexport)确保函数被导出
     */
    __declspec(dllexport) DataProcessor* createDataProcessor() {
        return new DataProcessorImpl(); // 在堆上创建实现类实例
    }

    /**
     * @brief 销毁处理器实例的导出函数
     * @param p 要销毁的DataProcessor对象指针
     */
    __declspec(dllexport) void destroyDataProcessor(DataProcessor* p) {
        delete p; // 调用虚析构函数确保正确释放资源
    }
}