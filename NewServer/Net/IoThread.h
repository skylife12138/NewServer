
/*
      此IO线程用于网络通讯，NET_LIB_EVENT为使用Libevent，否则使用自定义网络收发
*/

#ifndef _IOTHREAD_H_
#define _IOTHREAD_H_
#include <vector>
#include <set>
#include "NetPack.h"
#include "CClient.h"

#include "../Common/Thread.h"
#include "../Common/Common.h"

#include "../libevent/include/event2/buffer.h"
#include "../libevent/include/event2/bufferevent.h"
#include "../libevent/include/event2/listener.h"
#include "../libevent/include/event2/util.h"
#include "../libevent/include/event2/event.h"
#include "../libevent/include/event2/thread.h"

#define NET_LIB_EVENT

struct Net_Message
{
	uint16_t size;
	char data[MAX_MESSAGE_LEN];
};

struct TcpConnection {
	TcpConnection(){}
	~TcpConnection(){};
	QWORD sock_id;
	evutil_socket_t sock_fd;
	struct bufferevent* bufev;
	bool is_waiting_header;
	Net_Message current_message;
};

class IoThread : public BaseThread,public SingleTon<IoThread>
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

	void SendMsg(TcpConnection* Conn,int size);

#ifdef _WIN32
	HANDLE GetHandle();
	HANDLE I_CompletionPort;
#endif

	ThreadHandler* I_pThreadHandler;
public:
	struct event_base *base;
	struct evconnlistener *listener;
	struct event *singal_event;

private:
	bool I_IsStop;
};



#endif
