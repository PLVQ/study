#include "pub_func_dec.h"

void oneThread()
{
    const int cCount = 1000;
    EasyTcpClient *clients[cCount];
    cellTimeStamp tTime;
    for (int i = 0; i < cCount; ++i)
    {
        clients[i] = new EasyTcpClient;
        if (!clients[i]->Connect((char *)"127.0.0.1", 8888))
        {
            return;
        }
    }
    std::cout << "connect consume time:" << tTime.getElapsedSecondInMilliSec() << std::endl;
    std::chrono::milliseconds tMilli(1000);
    std::this_thread::sleep_for(tMilli);
    login request;
    strcpy(request.user_name, "pengjiang");
    strcpy(request.passwd, "123456");

    while (true)
    {
        for (int i = 0; i < cCount; ++i)
        {
            clients[i]->sendData(&request);
            // int ret = clients[i]->onRun();
        }
    }
    for (int i = 0; i < cCount; ++i)
    {
        clients[i]->Close();
    }
}

void multiThread()
{
    const int cCount = 1000;
    EasyTcpClient *clients[cCount];
    int tCount = 4;
    cellTimeStamp tTime;
    std::thread *nThread[tCount];
    for (int i = 0; i < 4; ++i)
    {
        int avg = cCount / tCount;
        int begin = i * avg;
        int end = (i + 1) * avg;
        nThread[i] = new std::thread([&, begin, end, i]() {
            sendMsg(clients, begin, end, i + 1);
        });
        nThread[i]->detach();
    }
    while (true)
    {
    }
    for (int i = 0; i < 4; ++i)
    {
        delete nThread[i];
    }
}

int main()
{
    // oneThread();
    multiThread();
    getchar();
    return 0;
}