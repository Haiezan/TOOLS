#include <Windows.h>
#include <iostream>
#include "ExactTime.h"

int main()
{
    tt::ExactTime exact_time = tt::ExactTime::currentTime();
    std::cout << "当前时间：" << exact_time.toString() << std::endl;
    Sleep(10);

    tt::ExactTime exact_time2 = tt::ExactTime::currentTime();
    std::cout << "当前时间：" << exact_time2.toString() << std::endl;
    std::cout << exact_time2 - exact_time;

    return 0;
}
