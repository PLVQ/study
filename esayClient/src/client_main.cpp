#include "pub_func_dec.h"

int main() {
    const int cCount = 100;
    EasyTcpClient *clients[cCount];
    cellTimeStamp tTime;
    for (int i = 0; i < cCount; ++i) {
        clients[i] = new EasyTcpClient;
        if (!clients[i]->initSocket()) {
            return 0;
        }
    }

    for (int i = 0; i < cCount; ++i) {
        if (!clients[i]->Connect((char *)"127.0.0.1", 8888)) {
            return 0;
        }
    }
    std::cout << "consume time:" << tTime.getElapsedSecondInMilliSec()
              << std::endl;
    login request;
    strcpy(request.user_name, "pengjiang");
    strcpy(request.passwd, "123456");

    while (true) {
        for (int i = 0; i < cCount; ++i) {
            clients[i]->sendData(&request);
            int ret = clients[i]->onRun();
        }
    }
    for (int i = 0; i < cCount; ++i) {
        clients[i]->Close();
    }
    getchar();
    return 0;
}