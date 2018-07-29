#include "GTimer.h"
#include <iostream>
#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif // WIN32

DWORD GetNowTime()
{
	//用c库函数精确到秒
	DWORD NowSeond = (DWORD)time(NULL);
	return NowSeond;
}

GTimeMgr::GTimeMgr()
{
	NowUuidNum=0;
	FreeUuid.clear();
	NeedDel.clear();
	DelayDataMap.clear();
}

GTimeMgr::~GTimeMgr()
{
	NowUuidNum = 0;
	FreeUuid.clear();
	NeedDel.clear();
	DelayDataMap.clear();
}

int  GTimeMgr::GetDelayUuid()
{
	int Uuid = 0;
	if (!FreeUuid.empty())
	{
		auto iter = FreeUuid.begin();
		Uuid = *iter;
		FreeUuid.erase(iter);
	}
	else
	{
		if (NowUuidNum >= Max_DelayTimeNum)
			return -1;
		Uuid = NowUuidNum;
		NowUuidNum++;
	}
	return Uuid;
}

void GTimeMgr::FreeDelayUuid(int Uuid)
{
	FreeUuid.insert(Uuid);
}

void GTimeMgr::AddDelayTimer(int TimeUuid, DelayTimeObject* DelayTime)
{
	if (!DelayTime)
		return;
	auto iter = DelayDataMap.find(TimeUuid);
	if (iter != DelayDataMap.end())
		return;
	DelayDataMap[TimeUuid] = DelayTime;
}

void GTimeMgr::DelDelayTimer(int TimeUuid)
{
	auto iter = DelayDataMap.find(TimeUuid);
	if (iter == DelayDataMap.end())
		return;
	delete iter->second;
	DelayDataMap.erase(iter);
	FreeDelayUuid(TimeUuid);
}

void GTimeMgr::TimeTick()
{
	for (auto iter = DelayDataMap.begin();iter != DelayDataMap.end();++iter)
	{
		DelayTimeObject* DelayTimer = iter->second;
		if (!DelayTimer || GetNowTimeStamp() < DelayTimer->GetTriggerSecond())
			continue;
		DelayTimer->Invoke();
		if (GetNowTimeStamp() >= DelayTimer->GetTriggerSecond())
		{
			DelayTimer->clear();
			NeedDel.insert(iter->first);
		}
	}
	for (auto iter = NeedDel.begin();iter != NeedDel.end();++iter)
	{
		DelDelayTimer(*iter);
	}
	NeedDel.clear();
}

DWORD GTimeMgr::GetNowTimeStamp()
{
	return GetNowTime();
}

DWORD GTimeMgr::GetTimeStampFromStr(const char* TimeStr)
{
	tm TempData;
#ifdef WIN32
	sscanf_s(TimeStr,"%4d-%2d-%2d %2d:%2d:%2d",
#else
	sscanf(TimeStr, "%4d-%2d-%2d %2d:%2d:%2d",
#endif // WIN32
		&TempData.tm_year,&TempData.tm_mon, &TempData.tm_mday, &TempData.tm_hour, &TempData.tm_min, &TempData.tm_sec);
	TempData.tm_isdst = 0;
	return (DWORD)mktime(&TempData);
}

void GTimeMgr::GetSystemTime(ST& TimeData,DWORD TimeStamp)
{
	tm TempData;
    time_t NowRealTime = TimeStamp? TimeStamp:time(NULL);
	localtime_s(&TempData,&NowRealTime);
	
	TimeData.Year = TempData.tm_year;
	TimeData.Month = TempData.tm_mon;
	TimeData.Date = TempData.tm_mday;
	TimeData.WeekDay = TempData.tm_wday;
	TimeData.Hour = TempData.tm_hour;
	TimeData.Minute = TempData.tm_min;
	TimeData.Second = TempData.tm_sec;
}

DWORD GTimeMgr::MakeTime(ST& TimeData)
{
	tm TempData;
	TempData.tm_year = TimeData.Year;
	TempData.tm_mon = TimeData.Month;
	TempData.tm_mday = TimeData.Date;
	TempData.tm_wday = TimeData.WeekDay;
	TempData.tm_hour = TimeData.Hour ;
	TempData.tm_min = TimeData.Minute;
	TempData.tm_sec = TimeData.Second;
	TempData.tm_isdst = 0;
	return (DWORD)mktime(&TempData);
}

void DelayTimeObject::Init(DWORD time)
{
	WaitScond = time;
	TriggerSecond = GetNowTime() + WaitScond;
}

void DelayTimeObject::clear()
{
	WaitScond = 0;
	TriggerSecond = 0;
}

void DelayTimeObject::ReHook()
{
	TriggerSecond = GetNowTime() + WaitScond;
}

void TestDelayTime::Invoke()
{
	std::cout << "TestDelayTime" << std::endl;
	ReHook();
}