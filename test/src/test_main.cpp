#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std::chrono;
using namespace std;

void test_chrono() {
    typedef std::chrono::duration<int> seconds_type;
    typedef std::chrono::duration<int, std::milli> milliseconds_type;
    typedef std::chrono::duration<int, std::ratio<60 * 60>> hours_type;

    hours_type h_oneday(24);                // 24h
    seconds_type s_oneday(60 * 60 * 24);    // 86400s
    milliseconds_type ms_oneday(s_oneday);  // 86400000ms

    seconds_type s_onehour(60 * 60);  // 3600s
                                      // hours_type h_onehour (s_onehour); //
                                      // NOT VALID (type truncates), use:
    hours_type h_onehour(std::chrono::duration_cast<hours_type>(s_onehour));
    milliseconds_type ms_onehour(
        s_onehour);  // 3600000ms (ok, no type truncation)

    std::cout << ms_onehour.count() << "ms in 1h" << std::endl;
}

class Printer{
public:
	static Printer* getInstance(){ return pPrinter;}
	void PrintText(string text){
		cout << "打印内容:" << text << endl;
		cout << "已打印次数:" << mTimes << endl;
		cout << "--------------" << endl;
		mTimes++;
	}
	
//默认构造  和 拷贝构造 私有化
private:
	Printer(){ mTimes = 0; }
	Printer(const Printer&){}
private:
	static Printer* pPrinter;
	int mTimes;
};

Printer* Printer::pPrinter = new Printer;//静态对象指针的类外实现

void test(){
	Printer* printer = Printer::getInstance();
	printer->PrintText("离职报告!");
	printer->PrintText("入职合同!");
	printer->PrintText("提交代码!");
}

int main() {
    test();
    // test_chrono();
    return 0;
}