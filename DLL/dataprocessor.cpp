#include "dataprocessor.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

/**
 * @brief ���ݴ������ʵ����
 * @note �̳��Գ���ӿڣ�ʵ�����д��麯��
 */
class DataProcessorImpl : public DataProcessor {
    int sum = 0;    // �洢���������ܺ�
    int max = 0;    // �洢�����������ֵ
    int min = 0;    // �洢����������Сֵ

public:
    /**
     * @brief �������ݵľ���ʵ��
     * @param data ��������������ȶ�ȡԭʼ����Ҳ���ش���������
     */
    void processData(std::vector<int>& data) override {
        // �����ܺ�
        sum = std::accumulate(data.begin(), data.end(), 0);

        // �������ֵ
        max = *std::max_element(data.begin(), data.end());

        // ������Сֵ
        min = *std::min_element(data.begin(), data.end());

        // �޸�ԭʼ���ݣ���ʾ���ݿ���˫�򴫵ݣ�
        for (auto& num : data) {
            num *= 2;  // ���������ݳ���2
        }
    }

    /**
     * @brief ��ӡ������
     */
    void printResults() const override {
        std::cout << "===== ������ =====\n";
        std::cout << "�ܺ�: " << sum << "\n";
        std::cout << "���ֵ: " << max << "\n";
        std::cout << "��Сֵ: " << min << std::endl;
    }
};

// ����C���ӿں��������������������⣩
extern "C" {
    /**
     * @brief ����������ʵ��
     * @return �´�����DataProcessor����ָ��
     */
    __declspec(dllexport) DataProcessor* createDataProcessor() {
        return new DataProcessorImpl();  // �ڶ��ϴ���ʵ����ʵ��
    }

    /**
     * @brief ���ٴ�����ʵ��
     * @param p Ҫ���ٵ�DataProcessor����ָ��
     */
    __declspec(dllexport) void destroyDataProcessor(DataProcessor* p) {
        delete p;  // �������������������
    }
}