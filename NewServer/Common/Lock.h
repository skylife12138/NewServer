#ifndef _LOCK_H_
#define _LOCK_H_

#include "Prec.h"
#ifdef WIN32
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
	void operator =(CriticalLock &lock) {} //½ûÖ¹¿½±´
public:
#ifdef WIN32
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
#ifdef WIN32
	CRITICAL_SECTION m_lock;
#else
	pthread_mutex_t m_lock;
#endif
};

class FakeLock:public LockMeta
{
public:
	FakeLock() {};
	~FakeLock() {};
	void Lock() {}
	void UnLock() {}
	void Release()
	{
		if (this)
			delete this;
	}
};

LockMeta* CreateThreadLock()
{
	return new CriticalLock();
}

LockMeta* CreateFakeLock()
{
	return new FakeLock();
}

#endif
