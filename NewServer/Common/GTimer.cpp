#include "GTimer.h"
#include "Common.h"
#include <iostream>
#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif // _WIN32


#define sizeofa(array)                    \
    (sizeof(array) / sizeof(array[0]))


#define time_after_eq(unknow,know)      \
    ((int)(unknow)-(int)(know)>=0) 


void TM_SetDela(GTimerList *t1, int dt)
{
	t1->Delta = dt;
}

int  TM_GetDela(GTimerList *t1)
{
	return t1->Delta;
}

void TM_SetTimerFunc(GTimerList *t1, GTimerFunc func)
{
	t1->Func = func;
}

DWORD GetNowTime()
{
	DWORD NowSeond = (DWORD)time(NULL);
	return NowSeond;
}

DWORD GlobalTimer::GetTimeStampFromStr(const char* TimeStr)
{
	tm TempData;
#ifdef _WIN32
	sscanf_s(TimeStr,"%4d-%2d-%2d %2d:%2d:%2d",
#else
	sscanf(TimeStr, "%4d-%2d-%2d %2d:%2d:%2d",
#endif // _WIN32
		&TempData.tm_year,&TempData.tm_mon, &TempData.tm_mday, &TempData.tm_hour, &TempData.tm_min, &TempData.tm_sec);
	TempData.tm_isdst = 0;
	return (DWORD)mktime(&TempData);
}

void GlobalTimer::GetSystemTime(ST& TimeData,DWORD TimeStamp)
{
	tm TempData;
    time_t NowRealTime = TimeStamp? TimeStamp:time(NULL);
#ifdef WIN32
	localtime_s(&TempData,&NowRealTime);
#else
	localtime_r(&NowRealTime,&TempData);
#endif
	
	TimeData.Year = TempData.tm_year+1900;
	TimeData.Month = TempData.tm_mon+1;
	TimeData.Date = TempData.tm_mday;
	TimeData.WeekDay = TempData.tm_wday?TempData.tm_wday:7;
	TimeData.Hour = TempData.tm_hour;
	TimeData.Minute = TempData.tm_min;
	TimeData.Second = TempData.tm_sec;
}

DWORD GlobalTimer::MakeTime(ST& TimeData)
{
	tm TempData;
	TempData.tm_year = TimeData.Year-1900;
	TempData.tm_mon = TimeData.Month-1;
	TempData.tm_mday = TimeData.Date;
	TempData.tm_hour = TimeData.Hour ;
	TempData.tm_min = TimeData.Minute;
	TempData.tm_sec = TimeData.Second;
	TempData.tm_isdst = 0;
	return (DWORD)mktime(&TempData);
}

DWORD GlobalTimer::GetNowTimeStamp()
{
   return GetNowTime();
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
	Show("TestDelayTime");
	ReHook();
}

GTimerList *TM_GetTimer(void* udata)
{
	return (GTimerList *)udata - 1;
}

GlobalTimer::~GlobalTimer()
{
	for (int i = 0; i < (int)sizeofa(mRotary); i++)
	{
		while (!list_empty(&mRotary[i]))
		{
			GTimerList *t1 = list_first_entry(&mRotary[i], GTimerList, lst);
			DestoryTimer(t1);
		}
	}
	while (!list_empty(&mErase))
	{
		GTimerList *t1 = list_first_entry(&mErase, GTimerList, lst);
		DestoryTimer(t1);
	}
}

GlobalTimer::GlobalTimer(int gtick, void*(*alloc)(int), void(*dealloc)(void*, int))
{
	mAlloc = alloc;
	mDealloc = dealloc;
	mGTick = gtick;
	mSize = 0;
	INIT_LIST_HEAD(&mErase);
	for (int i = 0; i < (int)sizeofa(mRotary); i++)
	{
		INIT_LIST_HEAD(&mRotary[i]);
	}
}

void* GlobalTimer::InitTimer(int dt, GTimerFunc func, int size, GDeleteFunc delFunc)
{
	GTimerList *t1 = (GTimerList *)mAlloc(size + sizeof(GTimerList));
	INIT_LIST_HEAD(&t1->lst);
	t1->Expire = mGTick + dt;
	t1->Delta = dt;
	t1->Func = func;
	t1->delFunc = delFunc;
	t1->udata = (void *)(t1 + 1);
	t1->size = size;
	AddTimerImpl(t1);
	mSize++;
	return t1->udata;
}

void GlobalTimer::AddTimer(GTimerList *t1)
{
	t1->Expire += t1->Delta;
	list_del(&t1->lst);
	AddTimerImpl(t1);
}

void GlobalTimer::DelTimer(GTimerList *t1)
{
	list_move(&t1->lst, &mErase);
}

void GlobalTimer::SetTimer(GTimerList *t1, int expire)
{
	t1->Expire = mGTick + expire;
	list_del(&t1->lst);
	AddTimerImpl(t1);
}

int GlobalTimer::GetExpire(GTimerList *t1)
{
	return t1->Expire - mGTick;
}

void GlobalTimer::DestoryTimer(GTimerList *t1)
{
	if (t1->delFunc)
	{
		t1->delFunc(t1->udata, t1->size);
	}
	list_del(&t1->lst);
	mDealloc(t1, sizeof(GTimerList) + t1->size);
	--mSize;
}

void GlobalTimer::AddTimerImpl(GTimerList *t1)
{
	int expire = t1->Expire;
	int dt = t1->Delta;
	list_head *entry;
	if (dt < TM_SIZE)
	{
		int i = expire & TM_MASK;
		entry = &mRotary[i];
	}
	else if (dt < TM_SIZE*TM_SIZE)
	{
		int i = (expire >> TM_BITS) & TM_MASK;
		entry = &mRotary[i + TM_SIZE];
	}
	else if (dt < TM_SIZE*TM_SIZE*TM_SIZE)
	{
		int i = (expire >> (TM_BITS * 2)) & TM_MASK;
		entry = &mRotary[i + TM_SIZE * 2];
	}
	else
	{
		int i = (expire >> (TM_BITS * 3)) & TM_MASK;
		entry = &mRotary[i + TM_SIZE * 3];
	}
	list_add_tail(&t1->lst, entry);
}

void GlobalTimer::Tick(int gtick)
{
	while (!list_empty(&mErase))
	{
		GTimerList *t1 = list_first_entry(&mErase, GTimerList, lst);
		DestoryTimer(t1);
	}
	INIT_LIST_HEAD(&mErase);
	while (time_after_eq(gtick, mGTick))
	{
		OneTick();
	}
}

#define INDEX(n) ((mGTick>>(TM_BITS*n)&TM_MASK))
void GlobalTimer::OneTick()
{
	int index = mGTick & TM_MASK;
	if (index == 0 && Cascade(TM_SIZE, INDEX(1)) && Cascade(TM_SIZE * 2, INDEX(2)))
	{
		Cascade(TM_SIZE * 3, INDEX(3));
	}
	++mGTick;
	list_head entry;
	list_head *head = &entry;
	list_replace_init(&mRotary[index], head);
	while (!list_empty(head))
	{
		GTimerList *t1 = list_first_entry(head, GTimerList, lst);
		if (t1->Func)
			t1->Func(this, t1, t1->udata, t1->size);
		if (head->next == &t1->lst)
			DestoryTimer(t1);
	}
}

bool GlobalTimer::Cascade(int base, int index)
{
	list_head entry;
	list_head *head = &entry;
	list_replace_init(&mRotary[index + base], head);
	list_head *pos;
	list_head *n;
	list_for_each_safe(pos, n, head)
	{
		GTimerList *t1 = list_entry(pos, GTimerList, lst);
		AddTimerImpl(t1);
	}
	return index = 0;
}

