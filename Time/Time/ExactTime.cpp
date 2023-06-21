#include "ExactTime.h"
#include <chrono>

using namespace std::chrono;

namespace tt {

	std::string ExactTime::toString() const
	{
		char temp[27]{ 0 };
		snprintf(temp,
			27,
			"%04d-%02d-%02d %02d:%02d:%02d.%03d%03d",
			tm_year + 1900,
			tm_mon + 1,
			tm_mday,
			tm_hour,
			tm_min,
			tm_sec,
			tm_millisec,
			tm_microsec);
		return std::string(temp);
	}

	std::string ExactTime::toStringForFilename() const
	{
		char temp[23]{ 0 };
		snprintf(temp,
			23,
			"%04d%02d%02d_%02d%02d%02d.%03d%03d",
			tm_year + 1900,
			tm_mon + 1,
			tm_mday,
			tm_hour,
			tm_min,
			tm_sec,
			tm_millisec,
			tm_microsec);
		return std::string(temp);
	}

	int ExactTime::operator-(const ExactTime& t1)
	{
		return (this->tm_millisec - t1.tm_millisec) * 1000 + this->tm_microsec - t1.tm_microsec;
	}

	ExactTime ExactTime::currentTime()
	{
		auto duration_since_epoch = system_clock::now().time_since_epoch(); // 从1970-01-01 00:00:00到当前时间点的时长
		auto microseconds_since_epoch = duration_cast<microseconds>(duration_since_epoch).count(); // 将时长转换为微秒数
		time_t seconds_since_epoch = static_cast<time_t>(microseconds_since_epoch / 1000000); // 将时长转换为秒数

#if defined _MSC_VER && _MSC_VER >= 1400
		ExactTime exact_time;
		localtime_s(&exact_time, &seconds_since_epoch);
		exact_time.tm_microsec = static_cast<int>(microseconds_since_epoch % 1000);
		exact_time.tm_millisec = static_cast<int>(microseconds_since_epoch / 1000 % 1000);
#elif defined __GNUC__
		ExactTime exact_time;
		localtime_r(&seconds_since_epoch, &exact_time);
		exact_time.tm_microsec = static_cast<int>(microseconds_since_epoch % 1000);
		exact_time.tm_millisec = static_cast<int>(microseconds_since_epoch / 1000 % 1000);
#else
#error "Unknown compiler"
#endif

		return exact_time;
	}

}
