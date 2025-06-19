#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <vector>
#include <memory>
#include <functional>

/**
 * @brief �������ݽṹ��
 * @param numbers ���������������
 * @param multiplier ���ݳ������ӣ�Ĭ��ֵΪ1��
 */
struct ProcessInput {
    std::vector<int> numbers;  // ��Ҫ��������ݼ���
    int multiplier = 1;        // ���ݷŴ�ϵ����Ĭ��Ϊ1
};

/**
 * @brief ������ݽṹ��
 * @param sum �����ܺ�
 * @param max �������ֵ
 * @param min ������Сֵ
 * @param processed_data ���������ݸ���
 */
struct ProcessOutput {
    int sum = 0;                // �洢�����ܺͽ��
    int max = 0;                // �洢���ֵ���
    int min = 0;                // �洢��Сֵ���
    std::vector<int> processed_data; // �洢���������ݸ���
};

/**
 * @brief ���ݴ������ӿ���
 * @note ���о��崦��������ʵ����Щ���麯��
 */
class DataProcessor {
public:
    /**
     * @brief �������ݵĴ��麯��
     * @param input �����������ݵĽṹ��
     * @param output ���ڽ��մ������Ľṹ��
     */
    virtual void process(const ProcessInput& input, ProcessOutput& output) = 0;

    /**
     * @brief ��ʾ�������Ĵ��麯��
     * @param output �����������Ľṹ��
     */
    virtual void displayResults(const ProcessOutput& output) const = 0;

    /**
     * @brief ����������ȷ����������ȷ�ͷ���Դ
     */
    virtual ~DataProcessor() = default;
};

// ������������������C�ӿڱ��������������⣩
extern "C" {
    /**
     * @brief ����������ʵ���ĺ���ָ������
     * @return �´�����DataProcessor����ָ��
     */
    typedef DataProcessor* (*CreateProcessorFunc)();

    /**
     * @brief ���ٴ�����ʵ���ĺ���ָ������
     * @param p Ҫ���ٵ�DataProcessor����ָ��
     */
    typedef void (*DestroyProcessorFunc)(DataProcessor*);
}

#endif // DATA_PROCESSOR_H