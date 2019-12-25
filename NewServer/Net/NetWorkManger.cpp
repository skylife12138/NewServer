#include "NetWorkManger.h"
#include "NetPack.h"
#include <sstream>
#include "./protobuffer/player.pb.h"


const int iRecvTimeOut = 5000;
bool NewWorkMgr::NetWorkInit()
{
    pCtx = zmq_ctx_new();
    if(!pCtx)
    {
		cout << "Net New Error!" << endl;
		return 0;
	}
    pSock = zmq_socket(pCtx,ZMQ_DEALER);
	if (!pSock)
	{
		cout << "Socket Init Error!" << endl;
		zmq_ctx_destroy(pCtx);
		return false;
	}
	if (zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRecvTimeOut, sizeof(iRecvTimeOut)) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return false;
	}
    std::stringstream ss;
    ss << "tcp://*:" << BindPort;
    if (zmq_bind(pSock, ss.str().c_str()) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return false;
	}
	cout << "bind at: " << ss.str().c_str() << endl;
    return true;
}

void NewWorkMgr::HandleNetMsg()
{
    char szMsg[1024] = { 0 };
    errno = 0;
	if (zmq_recv(pSock, szMsg, sizeof(szMsg), 0) < 0)
	{
        //cout << "error = " << zmq_strerror(errno) << endl;
		return;
	}
	NetPack* aPack = ParseRecv(szMsg, 1024);
	if(!aPack)
	{
		cout << "ParseRecv Error!!!" << endl;
		return;
	}

	TestProto TestP;
	TestP.ParseFromArray(aPack->Data, aPack->Size);
	cout << "TestProto is id =" << TestP.id() << " isman = " << TestP.isman() << endl;
	return;
}