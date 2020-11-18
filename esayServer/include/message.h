struct dataHeader
{
    int dataLen;
    int cmd;
};

struct login
{
    char user_name[32];
    char passwd[32];
};

struct  logout
{
    char user_name[32];
};

struct response
{
    int ret;
};

enum CMD {
    LOG_IN,
    LOG_OUT
};