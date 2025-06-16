#include "dataprocessor.h"
#include <numeric>  // ������ֵ�㷨ͷ�ļ�
#include <algorithm> // ����STL�㷨ͷ�ļ�
#include <iostream>  // �����������ͷ�ļ�

/**
 * @brief ���ݴ������ʵ����
 * @note �̳���DataProcessor�ӿڣ�ʵ�����д��麯��
 */
class DataProcessorImpl : public DataProcessor {
public:
    /**
     * @brief ��������ݴ���ʵ��
     * @param input ��������
     * @param output ������
     */
    void process(const ProcessInput& input, ProcessOutput& output) override {
        // ��֤���������Ƿ�Ϊ��
        if (input.numbers.empty()) {
            throw std::invalid_argument("�������ݲ���Ϊ��");
        }

        // ���������ܺͣ�ʹ��STL accumulate�㷨��
        output.sum = std::accumulate(input.numbers.begin(),
            input.numbers.end(), 0);

        // �������ֵ��ʹ��STL max_element�㷨��
        output.max = *std::max_element(input.numbers.begin(),
            input.numbers.end());

        // ������Сֵ��ʹ��STL min_element�㷨��
        output.min = *std::min_element(input.numbers.begin(),
            input.numbers.end());

        // �������ݣ�ÿ��Ԫ�س��Գ�������
        output.processed_data.reserve(input.numbers.size()); // Ԥ����ռ�
        for (int num : input.numbers) {
            output.processed_data.push_back(num * input.multiplier);
        }
    }

    /**
     * @brief ��ʾ�������ľ���ʵ��
     * @param output �����������Ľṹ��
     */
    void displayResults(const ProcessOutput& output) const override {
        std::cout << "======= ���ݴ����� =======\n";
        std::cout << "�� �����ܺ�: " << output.sum << "\n";
        std::cout << "�� ���ֵ: " << output.max << "\n";
        std::cout << "�� ��Сֵ: " << output.min << "\n";

        std::cout << "�� ���������: ";
        for (int num : output.processed_data) {
            std::cout << num << " ";
        }
        std::cout << "\n===========================\n";
    }
};

// ����C���ӿں��������������������⣩
extern "C" {
    /**
     * @brief ����������ʵ���ĵ�������
     * @return �´�����DataProcessorImpl����ָ��
     * @note ʹ��__declspec(dllexport)ȷ������������
     */
    __declspec(dllexport) DataProcessor* createDataProcessor() {
        return new DataProcessorImpl(); // �ڶ��ϴ���ʵ����ʵ��
    }

    /**
     * @brief ���ٴ�����ʵ���ĵ�������
     * @param p Ҫ���ٵ�DataProcessor����ָ��
     */
    __declspec(dllexport) void destroyDataProcessor(DataProcessor* p) {
        delete p; // ��������������ȷ����ȷ�ͷ���Դ
    }
}