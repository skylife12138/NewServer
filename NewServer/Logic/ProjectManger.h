#ifndef PROJECTMANGER_H_
#define PROJECTMANGER_H_
#include "../Common/Portable.h"

class GProjectMgr
{
public:
  GProjectMgr() { Init(); }
  ~GProjectMgr();
  bool Init();
  void Realase();
  void MainLoop();

  void SetExit(bool isexit) { _proexit = isexit; }
  bool IsExit();

public:

private:
  bool _proexit;
  DWORD _LastTickTime;
};

extern GProjectMgr* GProMgr;
extern class GlobalTimer* GTimer;
extern class NewWorkMgr* NetMgr;
#endif