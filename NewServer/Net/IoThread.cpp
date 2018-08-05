#include "IoThread.h"

IoThread::IoThread()
{
	I_pThreadHandler = NULL;
	I_IsStop = false;
}

IoThread::~IoThread()
{

}

bool IoThread::Init()
{
#ifdef WIN32
	//windows用完成端口
	I_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (I_CompletionPort==NULL)
		return false;
	if (!Start())
		return false;
	return true;
#endif
}

void IoThread::UnInit()
{
	if (!I_IsStop)
		Stop();
}

UINT32 IoThread::GetThreadId()
{
	if (I_pThreadHandler)
		return I_pThreadHandler->GetThreadId();
	return 0;
}

bool IoThread::Start()
{
	CreateThreadHandler(this, true, I_pThreadHandler);
	if (!I_pThreadHandler)
		return false;
	return true;
}

void IoThread::Stop()
{
	I_IsStop = true;
	if (I_pThreadHandler)
	{
		I_pThreadHandler->WaitFor(3000);
		I_pThreadHandler->Release();
		I_pThreadHandler = NULL;
	}
}

#ifdef WIN32
HANDLE IoThread::GetHandle()
{
	return I_CompletionPort;
}
#endif

void IoThread::DoThread()
{
	
}
