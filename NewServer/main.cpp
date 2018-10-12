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

void test()
{
	TestDelayTime* t = GTimer->CreateDelayTimer<TestDelayTime>(10);
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

int main(int argc,char** argv)
{
	gflags::SetUsageMessage("GameServer");
	gflags::ParseCommandLineFlags(&argc, &argv, false);
	signal(SIGINT, EndFun);
	signal(SIGTERM, EndFun);

    GProMgr = new GProjectMgr;
    bool Ret = GProMgr->Init();
	if (!Ret)
	{
		cout << "Project Init Error!" << endl;
		return 1;
	}
	//测试代码开始
	//test();
	int a, b, c;
	zmq_version(&a, &b, &c);
	std::cout << "zmq_version:" << a << "_" << b << "_" << c << std::endl;
	//测试代码结束
	DWORD NowSecond = GTimer->GetNowTimeStamp();
	while (!GProMgr->IsExit())
	{
		NowSecond = GTimer->GetNowTimeStamp();
		GProMgr->MainLoop();
		GProMgr->SetExit(IsExit);
	}
	system("pause");
	return 0;
}