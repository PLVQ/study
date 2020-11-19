class dataHeader
{
public:
    int dataLen;
    int cmd;
};
enum CMD {
    LOG_IN,
    LOG_OUT
};
class login : public dataHeader
{
public:
    login()
    {
        dataLen = sizeof(login);
        cmd = LOG_IN;
    }
    char user_name[32];
    char passwd[32];
};

struct response : public dataHeader
{
public:
    response()
    {
        dataLen = sizeof(response);
        cmd = LOG_IN;
        ret = 0;
    }
    int ret;
};