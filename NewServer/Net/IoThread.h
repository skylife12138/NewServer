#ifndef _IOTHREAD_H_
#define _IOTHREAD_H_
#include "../Common/Thread.h"

#include <vector>
#include <set>

class IoThread:public BaseThread
{
public:
	IoThread();
    virtual ~IoThread();

	virtual void DoThread();
	virtual void Stop();

	bool Start();
	bool Init();
	void UnInit();

	UINT32 GetThreadId();

#ifdef _WIN32
	HANDLE GetHandle();
	HANDLE I_CompletionPort;
#endif

	ThreadHandler* I_pThreadHandler;
	bool I_IsStop;

private:

};



#endif
