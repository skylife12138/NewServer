#include "Prec.h"
#include "gflags/gflags.h"
#include <signal.h>
#include "DynamicPool.h"
#include "zmq.h"

int IsExit = true;
void EndFun(int n)
{
	if (n == SIGINT || n == SIGTERM)
		IsExit = false;
	else
		cout << "Error: Unknown Signal "<<n << endl;
}

static void TestPrint(GlobalTimer* gtimer, GTimerList* t1, void* udata, int size)
{
	cout << "testprint right!!!" << endl;
}

void test()
{
	int* udata = GTimer->InitTimer<int>(10, TestPrint, NULL);
	TestDyPool DPool;
	int count = 10000;
	while (count)
	{
		cout << count;
		TestObj* obj = DPool.Fetch();
		if (obj)
		{
			obj->Init();
			cout << sizeof(obj) << endl;
			DPool.Relase(obj);
			count--;
		}

	}
}

const int iRecvTimeOut = 5000; //zmq接收超时时间(毫秒)
const char *pAddr = "tcp://*:5547";
int main(int argc,char** argv)
{
	gflags::SetUsageMessage("GameServer");
	gflags::ParseCommandLineFlags(&argc, &argv, false);
	signal(SIGINT, EndFun);
	signal(SIGTERM, EndFun);

	GProMgr = new GProjectMgr;
	if (!GProMgr)
	{
		cout << "Project Init Error!" << endl;
		return 0;
	}

	void *pCtx = zmq_ctx_new();
	if (!pCtx)
	{
		cout << "Net New Error!" << endl;
		return 0;
	}
	void *pSock = zmq_socket(pCtx,ZMQ_DEALER);
	if (!pSock)
	{
		cout << "Socket Init Error!" << endl;
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	if (zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRecvTimeOut, sizeof(iRecvTimeOut)) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	if (zmq_bind(pSock, pAddr) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	cout << "bind at: " << pAddr << endl;

	//测试代码开始
	//test();
	//测试代码结束
	DWORD NowSecond = GTimer->GetNowTimeStamp();
	while (!GProMgr->IsExit())
	{
		NowSecond = GTimer->GetNowTimeStamp();
		GProMgr->MainLoop();
		GProMgr->SetExit(IsExit);

		int Uuid = GenUuid();
		cout << Uuid << endl;
		char szMsg[1024] = { 0 };
		errno = 0;
		if (zmq_recv(pSock, szMsg, sizeof(szMsg), 0) < 0)
		{
			cout << "error = " << zmq_strerror(errno) << endl;
			continue;
		}
		cout << "Recveived message: " << szMsg << endl;
	}
	return 0;
}