#ifndef PROJECTMANGER_H_
#define PROJECTMANGER_H_

class GProjectMgr
{
public:
  GProjectMgr() { Init(); }
  ~GProjectMgr() {}
  bool Init();
  void MainLoop();

  bool IsExit();

private:
  bool _proexit;
};

extern GProjectMgr* GProMgr;

#endif