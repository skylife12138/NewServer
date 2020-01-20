#include "Prec.h"
//#include "gflags/gflags_win/gflags/gflags.h"
#include <signal.h>
#include "./Common/Common.h"
#include "./Common/MemoryPool.h"
#include "./Logic/CJsonLog.h"
#if USE_TCMALLOC
#include <gperftools/malloc_extension.h>
#endif

int IsExit = true;
void EndFun(int n)
{
	if (n == SIGINT || n == SIGTERM)
		IsExit = false;
	else
		Error("Error: Unknown Signal %d",n);
}

static void TestPrint(GlobalTimer* gtimer, GTimerList* t1, void* udata, int size)
{
	Show("testprint right!!!");
}

void ShowTCMolloc()
{
#if USE_TCMALLOC
	MallocExtension *ext = MallocExtension::instance();
	if(!ext)
		return;
	char buf[1024 * 4];
	ext->GetStats(buf,sizeof(buf));
	Show("buf");
#endif
}

void TestJsonLog()
{
	CJsonLog JsonLog;
	JsonLog["name"] = "skylife";
	JsonLog.WriteJsonLog(EJLT_First);
}

const int MinDeltaTickCount = 1000 / 15;
int main(int argc,char** argv)
{
	//gflags::SetUsageMessage("GameServer");
	//gflags::ParseCommandLineFlags(&argc, &argv, false);
	signal(SIGINT, EndFun);
	signal(SIGTERM, EndFun);

	GProMgr = new GProjectMgr;
	if (!GProMgr->Init())
	{
		Error("Project Init Error!");
		return 0;
	}
	//定时器测试
	ShowTCMolloc();
	DWORD LastTickTime = TimeGetTime();
	int Moredelta = 0;
	while (!GProMgr->IsExit())
	{
		NowTickCount = TimeGetTime();
		RealWorldTime = GTimer->GetNowTimeStamp();
		GTimer->GetSystemTime(NowSystemTime,RealWorldTime);

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