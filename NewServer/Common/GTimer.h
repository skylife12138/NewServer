#ifndef _GTIMER_H_
#define _GTIMER_H_
#include<map>
#include<set>
#include"./Logic/SingleTon.h"
#include "./Common/Portable.h"

const int Max_DelayTimeNum = 10000;
struct ST
{
	int Year;
	int Month;
	int Date;
	int WeekDay;
	int Hour;
	int Minute;
	int Second;
};

class DelayTimeBase;
class GTimeMgr:public SingleTon<GTimeMgr>
{
public:
	GTimeMgr();
	~GTimeMgr();
	void TimeTick();

	DWORD GetNowTimeStamp();
	DWORD MakeTime(ST& TimeData);
	DWORD GetTimeStampFromStr(const char* TimeStr);
	void  GetSystemTime(ST& TimeData, DWORD TimeStamp=0);

	int  GetDelayUuid();
	void FreeDelayUuid(int Uuid);
	bool CreateDelayTimer(int WaitTime, DelayTimeBase* DelayTime);
	void AddDelayTimer(int TimeUuid, DelayTimeBase* DelayTime);
	void DelDelayTimer(int TimeUuid);
private:
	int NowUuidNum;
	std::set<int> FreeUuid;
	std::set<int> NeedDel;
	std::map<int, DelayTimeBase*> DelayDataMap;
};

class DelayTimeBase
{
public:
	DelayTimeBase() :WaitScond(0),TriggerSecond(0) {}
	void Init(DWORD time);
	void clear();
	void ReHook();
	DWORD GetTriggerSecond() { return TriggerSecond; }

	virtual void Invoke() = 0;
private:
	DWORD WaitScond; //等待时间
	DWORD TriggerSecond;//触发时刻
};

class TestDelayTime:public DelayTimeBase
{
public:
	virtual void Invoke();
};

#endif
