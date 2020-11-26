#include <iostream>
#include <vector>
#include <string>

class res
{
    public:
    std::string str;
};

int main()
{
    // std::string str;
    // std::cout << sizeof(str) << std::endl; 
    // std::string str1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    // std::cout << sizeof(str1) << std::endl; 
    // std::string str2 = "a";
    // std::cout << sizeof(str2) << std::endl; 
    res r;
    r.str = "hello";
    std::cout<<r.str<<std::endl;
    return 0;
}