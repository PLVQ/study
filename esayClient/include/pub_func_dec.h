#ifndef _PUB_FUNC_DEC_
#define _PUB_FUNC_DEC_
#include "easy_tcp_client.h"

// 输入命令
void cin_cmd(EasyTcpClient *client);
// 连接服务器,发送消息
void sendMsg(EasyTcpClient *clients[], int begin, int end, int tIndex);

#endif