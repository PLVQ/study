#include "alloctor.h"

int main()
{
    char * str1 = new char[128];
    delete[] str1;

    char* str2 = new char[32];
    delete[] str2;

    char* c = new char;
    delete c;

    char* str3[12];

    for(int i = 0; i<12; i++)
    {
        str3[i] = new char[64];
    }

    for(int i = 0; i<12; i++)
    {
        delete[] str3[i];
    }
    getchar();
    return 0;
}