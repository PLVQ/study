#include "multi_thread.h"

void threadTask(int begin, int end, char* str[])
{
    std::cout << "begin:" << begin << " end:" << end << std::endl;
    for(int i = begin; i < end; i++)
    {
        str[i] = new char[64];
    }
    for(int i = begin; i < end; i++)
    {
       delete[] str[i];
    }
    std::cout << "end" << std::endl;
}

void multiThread()
{
    std::thread t[4];
    const int count = 100000;
    int avg = count/4;
    char* str[count];
    for (int i = 0; i < 4; ++i)
    {
        int begin = i*avg;
        int end = (i+1)*avg;
        t[i] = std::thread(threadTask, begin, end, str);
        t[i].detach();
    }
}