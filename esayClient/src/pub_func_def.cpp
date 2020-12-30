#include "pub_func_dec.h"

void cin_cmd(EasyTcpClient *client)
{
    int cmd;
    while (true)
    {
        std::cin >> cmd;
        if (cmd == -1)
        {
            break;
        }
        switch (cmd)
        {
        case LOG_IN:
        {
            login request;
            strcpy(request.user_name, "pengjiang");
            strcpy(request.passwd, "123456");

            int ret = client->sendData(&request);
        }
        break;
        case LOG_OUT:
        {
            logOut request;
            strcpy(request.user_name, "pengjiang");

            int ret = client->sendData(&request);
        }
        break;
        default:
            std::cout << "error cmd!" << std::endl;
            break;
        }
    }
}

void sendMsg(EasyTcpClient *clients[], int begin, int end, int tIndex, std::atomic_int &sendCount)
{
    std::cout << "thread<" << tIndex << "> start." << std::endl;
    for (int i = begin; i < end; ++i)
    {
        clients[i] = new EasyTcpClient;
        clients[i]->Connect((char *)"127.0.0.1", 8888);
    }
    
    std::cout << "clients<begin=" << begin << ">,end<=" << end << "> connect success!" << std::endl;
    std::chrono::milliseconds tMilli(4000);
    std::this_thread::sleep_for(tMilli);
    login request;
    strcpy(request.user_name, "pengjiang");
    strcpy(request.passwd, "123456");
    bool runFlag = true;
    while (true)
    {
        for (int i = begin; i < end; ++i)
        {
            if (clients[i]->isRun())
            {
                clients[i]->sendData(&request);
                sendCount++;
                clients[i]->onRun();
            }
        }
    }
    for (int i = begin; i < end; ++i)
    {
        clients[i]->Close();
        delete clients[i];
    }
}