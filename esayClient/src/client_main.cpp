#include "pub_func_dec.h"

void oneThread()
{
    const int cCount = 1;
    EasyTcpClient *clients[cCount];
    for (int i = 0; i < cCount; ++i)
    {
        clients[i] = new EasyTcpClient;
        if (!clients[i]->Connect((char *)"127.0.0.1", 8888))
        {
            return;
        }
    }
    login request;
    strcpy(request.user_name, "pengjiang");
    strcpy(request.passwd, "123456");

    cellTimeStamp tTime;
    while (true)
    {
        for (int i = 0; i < cCount; ++i)
        {
            if (tTime.getElapsedSecond() >= 1.0)
            {
                clients[i]->sendData(&request);
                tTime.update();
            }
            int ret = clients[i]->onRun();
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
    std::atomic_int sendCount = {0};
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
            sendMsg(clients, begin, end, i + 1, sendCount);
        });
        nThread[i]->detach();
    }
    while (true)
    {
        auto t = tTime.getElapsedSecond();
		if (t >= 1.0)
		{
			printf("thread<%d>,clients<%d>,time<%lf>,send<%d>\n",tCount, cCount,t,(int)(sendCount/ t));
			sendCount = 0;
			tTime.update();
		}
		Sleep(1);
    }
    for (int i = 0; i < 4; ++i)
    {
        delete nThread[i];
    }
}

int main()
{
    oneThread();
    // multiThread();
    getchar();
    return 0;
}