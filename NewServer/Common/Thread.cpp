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
#ifdef WIN32
unsigned int _stdcall ThreadHandler::_StaticThreadFunc(void *arg)
{
	ThreadHandler* pThreadCtr = (ThreadHandler*)arg;
	pThreadCtr->_IsStop = false;
	pThreadCtr->_pThread->DoThread();
	if (!pThreadCtr->_NeedWaitfor)
	{
		CloseHandle(pThreadCtr->_WinHandle);
		pThreadCtr->_WinHandle = INVALID_HANDLE_VALUE;
		pThreadCtr->_IsStop = true;
	}
	return 0;
}
#else
unsigned int ThreadHandler::_StaticThreadFunc(void  *arg)
{
	ThreadHandler* pThreadCtr = (ThreadHandler*)arg;
	pThreadCtr->_IsStop = false;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

	sigset_t new_set, old_set;
	sigemptyset(&new_set);
	sigemptyset(&old_set);
	sigaddset(&new_set, SIGHUP);
	sigaddset(&new_set, SIGINT);
	sigaddset(&new_set, SIGQUIT);
	sigaddset(&new_set, SIGTERM);
	sigaddset(&new_set, SIGUSR1);
	sigaddset(&new_set, SIGUSR2);
	sigaddset(&new_set, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &new_set, &old_set);

	if (false == pThreadCtrl->m_bNeedWaitfor)
	{
		pthread_detach(pthread_self());
	}
	pThreadCtr->_pThread->DoThread();
	return 0;
}
#endif


void ThreadHandler::Stop()
{
	if (_pThread != NULL)
		_pThread->Stop();
}

bool ThreadHandler::Start()
{
#ifdef WIN32
	_WinHandle = (HANDLE)_beginthreadex(0, 0, _StaticThreadFunc, this, 0, &_ThreadId);
	if (!_WinHandle)
		return false;
#else
	if (pthread_create(&_ThreadId, NULL, (void* (*)(void*))_StaticThreadFunc, this))
		return false;
#endif // WIN32
	return true;
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
	return true;
#endif // WIN32
}

bool ThreadHandler::WaitFor(unsigned int WaitTime)
{
	if (!_NeedWaitfor)
		return false;
#ifdef WIN32
	if (_WinHandle == INVALID_HANDLE_VALUE)
		return false;
	DWORD ret = WaitForSingleObject(_WinHandle, WaitTime);
	CloseHandle(_WinHandle);
	_WinHandle = INVALID_HANDLE_VALUE;
	_IsStop = true;
	if (ret == WAIT_OBJECT_0)
		return true;
#else
	pthread_join(_ThreadId, NULL);
	return true;
#endif
	return false;
}

void ThreadHandler::Release()
{
	delete this;
}


bool CreateThreadHandler(BaseThread* pThread, bool NeedWaitFor, ThreadHandler*& pThreadHandler)
{
	ThreadHandler* pThreadCtrl = new ThreadHandler(pThread,NeedWaitFor);
	pThreadHandler = pThreadCtrl;
	if (!pThreadCtrl)
		return false;
	if (!pThreadCtrl->Start())
	{
		delete pThreadCtrl;
		pThreadHandler = NULL;
		return false;
	}
	return false;
}