#ifndef _NETWORKMANGER_H_
#define _NETWORKMANGER_H_
#include "../libzmq/include/zmq.h"
#include "../Prec.h"

class NewWorkMgr
{
public:
    NewWorkMgr():BindPort(5547),pCtx(NULL),pSock(NULL){}
    bool NetWorkInit();
    void HandleNetMsg();

private:
    int BindPort;
    void* pCtx;
    void* pSock;
};



#endif