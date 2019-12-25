#ifndef _LOCK_H_
#define _LOCK_H_

#include "../Prec.h"
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <pthread.h>
#endif

class LockMeta
{
public:
	virtual ~LockMeta() {}
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
	virtual void Release() = 0;
};

class CriticalLock:public LockMeta
{
public:
	void operator =(CriticalLock &lock) {} //禁止拷贝
public:
#ifdef _WIN32
	CriticalLock()
	{
		InitializeCriticalSection(&m_lock);
	}
	~CriticalLock()
	{
		DeleteCriticalSection(&m_lock);
	}
	void Lock()
	{
		EnterCriticalSection(&m_lock);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_lock);
	}
#else
	CriticalLock()
	{
		pthread_mutex_init(&m_lock,NULL);
	}
	~CriticalLock()
	{
		pthread_mutex_destroy(&m_lock);
	}
	void Lock()
	{
		pthread_mutex_lock(&m_lock);
	}
	void UnLock()
	{
		pthread_mutex_unlock(&m_lock);
	}
#endif
	void Release()
	{
		if (this)
			delete this;
	}
private:
#ifdef _WIN32
	CRITICAL_SECTION m_lock;
#else
	pthread_mutex_t m_lock;
#endif
};

#ifdef _WIN32
class AtomicNumber
{
public:
	AtomicNumber() { mValue = 0; }
	long Get() { return mValue; }
	void Set(int n) { InterlockedExchange(&mValue, n); }
	void Inc() { InterlockedIncrement(&mValue); }
	void Dec() { InterlockedDecrement(&mValue); }
	void Add(int n) { InterlockedExchangeAdd(&mValue, n); }
	void Sub(int n) { InterlockedExchangeAdd(&mValue, -n); }
private:
	volatile long mValue;
};
#else
class AtomicNumber
{
public:
	AtomicNumber() { mValue = 0; }
	long Get() { return mValue; }
	void Set(int n) { __sync_lock_test_and_set(&mValue, n); }
	void Inc() { __sync_add_and_fetch(&mValue,1); }
	void Dec() { __sync_add_and_fetch(&mValue,1); }
	void Add(int n) { __sync_add_and_fetch(&mValue, n); }
	void Sub(int n) { __sync_add_and_fetch(&mValue, -n); }
private:
	volatile long mValue;
};
#endif

#endif
