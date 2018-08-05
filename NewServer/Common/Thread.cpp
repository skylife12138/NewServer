#include "Thread.h"

ThreadHandler::ThreadHandler(BaseThread* pThread,bool NeedWaitFor)
{
	_IsStop = true;
    _NeedWaitfor = NeedWaitFor;
	_pThread = pThread;
	_ThreadId = 0;
#ifdef WIN32
	_WinHandle = INVALID_HANDLE_VALUE;
#endif
}

ThreadHandler::~ThreadHandler()
{
#ifdef WIN32
	if (_WinHandle != INVALID_HANDLE_VALUE)
#endif // WIN32
	{
		unsigned int _ErrorCode = 0;
		Kill(_ErrorCode);
	}
}

void ThreadHandler::Stop()
{

}

bool ThreadHandler::Kill(unsigned int ExitCode)
{
#ifdef WIN32
	if (_WinHandle == INVALID_HANDLE_VALUE)
		return false;
	if (TerminateThread(_WinHandle, ExitCode))
	{
		CloseHandle(_WinHandle);
		_WinHandle = INVALID_HANDLE_VALUE;
		return true;
	}
	return false;
#else
	pthread_cancel(_ThreadId);
	return false;
#endif // WIN32
}

bool ThreadHandler::WaitFor(unsigned int WaitTime)
{

}

void ThreadHandler::Release()
{

}