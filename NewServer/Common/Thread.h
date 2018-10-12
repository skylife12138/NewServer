#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef _WIN32
#include <Windows.h>
#include <process.h>

#include <psapi.h>
#include <cstddef>  
#include <dbghelp.h>
#else
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <netinet/in.h>
#include <execinfo.h>
#endif // _WIN32

class BaseThread
{
public:
	virtual void Stop() = 0;
	virtual void DoThread() = 0;
};

class ThreadHandler
{
public:
	ThreadHandler(BaseThread* pThread, bool NeedWaitFor);
	~ThreadHandler();

	void         Stop();
	bool         Start();
	bool         Kill(unsigned int ExitCode);
	bool         WaitFor(unsigned int WaitTime = 0xffffffff);
	void         Release();
	unsigned int GetThreadId() { return _ThreadId; }
	BaseThread*  GetThread() { return _pThread; }
	bool         IsStop() {return _IsStop;}

#ifdef _WIN32
	void*        GetWinHandle() { return _WinHandle; }
#endif // _WIN32
private:
#ifdef _WIN32
	static unsigned int _stdcall _StaticThreadFunc(void *arg);
#else
	static unsigned int _StaticThreadFunc(void  *arg);
#endif
private:
	bool _IsStop;
	bool _NeedWaitfor;
	BaseThread* _pThread;
#ifdef _WIN32
	unsigned int _ThreadId;
	void* _WinHandle;
#else
	pthread_t _ThreadId;
#endif // _WIN32
};

bool CreateThreadHandler(BaseThread* pThread, bool NeedWaitFor, ThreadHandler*& pThreadHandler);

#endif
