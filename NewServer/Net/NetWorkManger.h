#ifndef _NETWORKMANGER_H_
#define _NETWORKMANGER_H_
#include "../Prec.h"

const int MAX_MSG_LEN = 1024;

class NewWorkMgr
{
public:
    NewWorkMgr():BindPort(5548),pCtx(NULL),pSock(NULL){}
    bool NetWorkInit();
    void HandleNetMsg();

private:
    int BindPort;
    void* pCtx;
    void* pSock;
};


#endif