#include"Prec.h"
GProjectMgr* GProMgr;

bool GProjectMgr::Init()
{
    cout << "init" << endl;
    _proexit = false;
	return true;
}

void GProjectMgr::MainLoop()
{
    cout << "mainLoop" << endl;
}

bool GProjectMgr::IsExit()
{
    return false;
}