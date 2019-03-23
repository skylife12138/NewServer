#ifndef _GTIMER_H_
#define _GTIMER_H_
#include<map>
#include<set>
#include<stddef.h>
#include<assert.h>
#include "List.h"
#include "SingleTon.h"
#include "Portable.h"

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

struct GTimerList;
class GlobalTimer;

typedef void(*GTimerFunc)(GlobalTimer *gtimer, GTimerList *t1, void *udata, int size);
typedef void(*GDeleteFunc)(void *udata, int size);

extern DWORD GetNowTime();

struct GTimerList
{
	struct list_head lst;
	int Expire;
	int Delta;
	GTimerFunc Func;
	int size;
	void *udata;
	GDeleteFunc delFunc;
};

class DelayTimeObject
{
public:
	DelayTimeObject() :WaitScond(0),TriggerSecond(0) {}
	void Init(DWORD time);
	void clear();
	void ReHook();
	DWORD GetTriggerSecond() { return TriggerSecond; }

	virtual void Invoke() = 0;
private:
	DWORD WaitScond; //等待时间
	DWORD TriggerSecond;//触发时刻
};

class GlobalTimer
{
	GlobalTimer(const GlobalTimer &);
	GlobalTimer &operator=(const GlobalTimer &);
public:
	~GlobalTimer();
	explicit GlobalTimer(int gtick, void*(*alloc)(int), void(*dealloc)(void*, int));
	template<class T> inline T* InitTimer(int dt, GTimerFunc func, GDeleteFunc delFunc)
	{
		void *udata = InitTimer(dt, func, sizeof(T), delFunc);
		return (T *)udata;
	}
	void *InitTimer(int dt, GTimerFunc func, int size, GDeleteFunc delFunc);
	void AddTimer(GTimerList *t1);
	void Tick(int gtick);
	int Size() { return mSize; }
	void DelTimer(GTimerList *t1);
	void SetTimer(GTimerList *t1, int expire);
	int GetExpire(GTimerList *t1);

	DWORD MakeTime(ST& TimeData);
	DWORD GetTimeStampFromStr(const char* TimeStr);
	void  GetSystemTime(ST& TimeData, DWORD TimeStamp = 0);
	DWORD GetNowTimeStamp();
private:
	void DestoryTimer(GTimerList *t1);
	void OneTick();
	bool Cascade(int base, int index);
	void AddTimerImpl(GTimerList *t1);

	static const int TM_BITS = 8;
	static const int TM_SIZE = 1 << TM_BITS;
	static const int TM_MASK = TM_SIZE - 1;

	struct list_head mRotary[TM_SIZE * 4];
	int mGTick;
	int mSize;
	struct list_head mErase;
	void *(*mAlloc)(int);
	void(*mDealloc)(void *, int);
};

void TM_SetDela(GTimerList *t1, int dt);
int  TM_GetDela(GTimerList *t1);
void TM_SetTimerFunc(GTimerList *t1, GTimerFunc func);
GTimerList *TM_GetTimer(void* udata);

template<class T>
inline T* GT_UserData(void* udata, int size)
{
	assert(size >= sizeof(T));
	if (size >= sizeof(T))
		return (T *)udata;
	return 0;
}

class TestDelayTime:public DelayTimeObject
{
public:
	virtual void Invoke();
};

#endif
