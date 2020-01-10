#include "Prec.h"
//#include "gflags/gflags_win/gflags/gflags.h"
#include <signal.h>
#include "./Common/Common.h"
#include "./Common/MemoryPool.h"

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
	
}

const int MinDeltaTickCount = 1000 / 15;
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
	RealWorldTime = GTimer->GetNowTimeStamp();
	DWORD LastTickTime = TimeGetTime();
	int Moredelta = 0;
	while (!GProMgr->IsExit())
	{
		NowTickCount = TimeGetTime();
		RealWorldTime = GTimer->GetNowTimeStamp();

		DWORD DeltaTickCount = NowTickCount - LastTickTime;
		if((int)DeltaTickCount > MinDeltaTickCount - Moredelta)
		{
			GProMgr->MainLoop();
			GProMgr->SetExit(IsExit);

			LastTickTime = NowTickCount;
			Moredelta += (DeltaTickCount - MinDeltaTickCount);
			if(Moredelta > 2*MinDeltaTickCount)
				Moredelta = 2 * MinDeltaTickCount;
			else if(Moredelta < 0)
				Moredelta = 0;
		}
		else if((int)DeltaTickCount < MinDeltaTickCount)
		{
			SLEEP(MinDeltaTickCount - DeltaTickCount);
		}
	}
	GProMgr->Realase();
	return 0;
}