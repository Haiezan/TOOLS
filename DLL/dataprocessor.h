#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <vector>
#include <functional>
#include <memory>

/**
 * @brief ���ݴ������ӿ���
 * @note �������DLL������ʹ����ȫ��ͬ�Ķ���
 */
class DataProcessor {
public:
    // �������ݴ��麯��
    virtual void processData(std::vector<int>& data) = 0;

    // ��ӡ������麯��
    virtual void printResults() const = 0;

    // ����������ȷ����ȷ�ͷ���������Դ
    virtual ~DataProcessor() = default;
};

// ���崴������������ĺ���ָ������
using CreateProcessorFunc = DataProcessor * (*)();

// �������ٴ���������ĺ���ָ������
using DestroyProcessorFunc = void(*)(DataProcessor*);

#endif // DATA_PROCESSOR_H