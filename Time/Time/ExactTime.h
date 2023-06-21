#include <ctime>
#include <string>

//testtime
namespace tt {

	//
	// \brief: ��ȷʱ����
	//
	class ExactTime : public std::tm
	{
	public:
		//! Ĭ�Ϲ��캯��
		ExactTime()
			: tm_millisec(0), tm_microsec(0)
		{
		}

		//! ���캯�������أ�
		ExactTime(const std::tm& _tm, int _tm_millisec, int _tm_microsec)
			: std::tm(_tm), tm_millisec(_tm_millisec), tm_microsec(_tm_microsec)
		{
		}

		//! ת��Ϊ�ַ��������ַ�������':'�����������ļ�����
		std::string toString() const;

		//! ת��Ϊ�ַ������������ļ���
		std::string toStringForFilename() const;

		//����
		int operator-(const ExactTime& t1);

		//! ��ȡ��ǰʱ��
		static ExactTime currentTime();

		int tm_millisec; // ����
		int tm_microsec; // ΢��
	};



}
