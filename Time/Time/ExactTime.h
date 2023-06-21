#include <ctime>
#include <string>

//testtime
namespace tt {

	//
	// \brief: 精确时间类
	//
	class ExactTime : public std::tm
	{
	public:
		//! 默认构造函数
		ExactTime()
			: tm_millisec(0), tm_microsec(0)
		{
		}

		//! 构造函数（重载）
		ExactTime(const std::tm& _tm, int _tm_millisec, int _tm_microsec)
			: std::tm(_tm), tm_millisec(_tm_millisec), tm_microsec(_tm_microsec)
		{
		}

		//! 转换为字符串（此字符串含有':'，不能用于文件名）
		std::string toString() const;

		//! 转换为字符串，可用于文件名
		std::string toStringForFilename() const;

		//减法
		int operator-(const ExactTime& t1);

		//! 获取当前时间
		static ExactTime currentTime();

		int tm_millisec; // 毫秒
		int tm_microsec; // 微秒
	};



}
