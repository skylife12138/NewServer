#include "Prec.h"
//#include "gflags/gflags_win/gflags/gflags.h"
#include <signal.h>
#include "./Common/DynamicPool.h"
#include "../Common/GTimer.h"

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

const int iRecvTimeOut = 5000; //zmq
const char *pAddr = "tcp://*:5547";
int main(int argc,char** argv)
{
	//gflags::SetUsageMessage("GameServer");
	//gflags::ParseCommandLineFlags(&argc, &argv, false);
	signal(SIGINT, EndFun);
	signal(SIGTERM, EndFun);

	GProMgr = new GProjectMgr;
	if (!GProMgr)
	{
		cout << "Project Init Error!" << endl;
		return 0;
	}
	//定时器测试
	//test();

	DWORD NowSecond = GTimer->GetNowTimeStamp();
	while (!GProMgr->IsExit())
	{
		NowSecond = GTimer->GetNowTimeStamp();
		GProMgr->MainLoop();
		GProMgr->SetExit(IsExit);

		int Uuid = GenUuid();
	}
	return 0;
}