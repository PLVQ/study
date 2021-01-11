#include "multi_thread.h"
#include "alloctor.h"

// void threadTask(int begin, int end, char* str[])
// {
//     std::cout << "begin:" << begin << " end:" << end << std::endl;
//     for(int i = begin; i < end; i++)
//     {
//         str[i] = new char[64];
//     }
// }

// void multiThread()
// {
//     std::thread t[4];
//     const int count = 10000;
//     int avg = count/4;
//     char* str[count];
//     for (int i = 0; i < 4; ++i)
//     {
//         int begin = i*avg;
//         int end = (i+1)*avg;
//         t[i] = std::thread(threadTask, begin, end, str);
//         t[i].detach();
//     }
// }
// void threadTask(int begin, int end, char* str[]);

// void multiThread();
int main()
{
    // char * str1 = new char[128];
    // delete[] str1;

    // char* str2 = new char[32];
    // delete[] str2;

    // char* c = new char;
    // delete c;

    // char* str3[12];

    // for(int i = 0; i<12; i++)
    // {
    //     str3[i] = new char[64];
    // }

    // for(int i = 0; i<12; i++)
    // {
    //     delete[] str3[i];
    // }
    multiThread();
    getchar();
    return 0;
}