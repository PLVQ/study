#include "easy_tcp_client.hpp"

void cin_cmd(EasyTcpClient *client) {
    int cmd;
    while (true) {
        std::cin >> cmd;
        if (cmd == -1) {
            break;
        }
        switch (cmd) {
            case LOG_IN: {
                login request;
                strcpy(request.user_name, "pengjiang");
                strcpy(request.passwd, "123456");

                int ret = client->sendData(&request);
            } break;
            case LOG_OUT: {
                logOut request;
                strcpy(request.user_name, "pengjiang");

                int ret = client->sendData(&request);
            } break;
            default:
                std::cout << "error cmd!" << std::endl;
                break;
        }
    }
}

void sendMsg() {
    
}