#include "Prec.h"
#include "gflags/gflags.h"
#include <signal.h>

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
	TestDelayTime* Test = new TestDelayTime();
	GTimeMgr::Instance()->CreateDelayTimer(10, Test);
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
	test();
	DWORD NowSecond = GTimeMgr::Instance()->GetNowTimeStamp();
    while(!GProMgr->IsExit())
    {
	  NowSecond = GTimeMgr::Instance()->GetNowTimeStamp();
      GProMgr->MainLoop();
	  GProMgr->SetExit(IsExit);
    }
    system("pause");
    return 0;
}