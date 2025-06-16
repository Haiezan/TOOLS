#include "dataprocessor.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

/**
 * @brief 数据处理具体实现类
 * @note 继承自抽象接口，实现所有纯虚函数
 */
class DataProcessorImpl : public DataProcessor {
    int sum = 0;    // 存储计算结果：总和
    int max = 0;    // 存储计算结果：最大值
    int min = 0;    // 存储计算结果：最小值

public:
    /**
     * @brief 处理数据的具体实现
     * @param data 输入输出参数，既读取原始数据也返回处理后的数据
     */
    void processData(std::vector<int>& data) override {
        // 计算总和
        sum = std::accumulate(data.begin(), data.end(), 0);

        // 查找最大值
        max = *std::max_element(data.begin(), data.end());

        // 查找最小值
        min = *std::min_element(data.begin(), data.end());

        // 修改原始数据（演示数据可以双向传递）
        for (auto& num : data) {
            num *= 2;  // 将所有数据乘以2
        }
    }

    /**
     * @brief 打印计算结果
     */
    void printResults() const override {
        std::cout << "===== 处理结果 =====\n";
        std::cout << "总和: " << sum << "\n";
        std::cout << "最大值: " << max << "\n";
        std::cout << "最小值: " << min << std::endl;
    }
};

// 导出C风格接口函数（避免名称修饰问题）
extern "C" {
    /**
     * @brief 创建处理器实例
     * @return 新创建的DataProcessor对象指针
     */
    __declspec(dllexport) DataProcessor* createDataProcessor() {
        return new DataProcessorImpl();  // 在堆上创建实现类实例
    }

    /**
     * @brief 销毁处理器实例
     * @param p 要销毁的DataProcessor对象指针
     */
    __declspec(dllexport) void destroyDataProcessor(DataProcessor* p) {
        delete p;  // 调用派生类的析构函数
    }
}