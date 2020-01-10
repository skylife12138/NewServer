#include "IoThread.h"
#include "NetWorkManger.h"
#include <sstream>
#include "MsgFilter.h"
#include "./protobuffer/player.pb.h"


static void Conn_Readcb(struct bufferevent* bev, void *arg)
{
	TcpConnection* Conn = static_cast<TcpConnection *>(arg);
	assert(Conn);
	struct evbuffer* inbuf = bufferevent_get_input(bev);
	CClient* Client = ClientMgr::Instance()->GetClientByFd(Conn->sock_id);
	if(!Client || !inbuf || !Conn)
		return;
	
	do
	{
		size_t data_sz = evbuffer_get_length(inbuf);

		if(Conn->is_waiting_header)
		{
			if(data_sz < WordSize)
				return;
			size_t read_sz = bufferevent_read(bev,&Conn->current_message,WordSize);
			assert(read_sz == WordSize);
			if(Conn->current_message.size < 0)
				return;
			Conn->is_waiting_header = false;
			assert(Conn->current_message.data);
		}
		else
		{
			if(data_sz < Conn->current_message.size || MAX_MESSAGE_LEN < Conn->current_message.size)
				return;
			size_t read_sz = bufferevent_read(bev,&(Conn->current_message.data),Conn->current_message.size);
			assert(read_sz == Conn->current_message.size);

			Client->AddRecvMsg(Conn->current_message.data, Conn->current_message.size);

			Conn->is_waiting_header = true;
			memset(Conn->current_message.data,0,MAX_MESSAGE_LEN);
		}
	} while (true);
}

static void Conn_Eventcb(struct bufferevent* bev, short events, void *user_data)
{
	if(events & BEV_EVENT_EOF)
		cout << "Connection closed!!!" << endl;
	else if(events & BEV_EVENT_ERROR)
		cout << "Error Occur" << strerror(errno) << endl;
	else if(events & BEV_EVENT_CONNECTED)
		cout << "CClient Connected Finish!" << endl;

}

static void Signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
	struct timeval delay = {2, 0};

	//event_base_loopexit(base,&delay);
}

static void Listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socketlen, void *user_data)
{
	IoThread *Io_Thread = static_cast<IoThread *>(user_data);
	assert(Io_Thread);
	struct bufferevent *bev;
	bev = bufferevent_socket_new(Io_Thread->base,fd,BEV_OPT_CLOSE_ON_FREE);
	if(!bev)
	{
		cout << "Error constructing bufferevent!" << endl;
		return;
	}
	
	TcpConnection *Conn = new TcpConnection();
	CClient* Client= ClientMgr::Instance()->PopAClient();
	assert(Conn);
	assert(Client);
	if(!Conn || !Client)
		return;
	Conn->sock_id = ClientMgr::GlobalSerial++;
	Conn->sock_fd = fd;
	Conn->bufev = bev;
	Conn->is_waiting_header = true;
	memset(Conn->current_message.data,0,MAX_MESSAGE_LEN);

	bufferevent_setcb(bev,Conn_Readcb,NULL,Conn_Eventcb,Conn);
	bufferevent_enable(bev,EV_READ | EV_PERSIST);

	Client->SetConnection(Conn);
	Client->SetSerial(Conn->sock_id);
	Client->SetIp(inet_ntoa(((struct sockaddr_in* )sa)->sin_addr));

	ClientMgr::Instance()->AddClientFdMap(Conn->sock_id,Client);
}

IoThread::IoThread()
{
	I_pThreadHandler = NULL;
	I_IsStop = false;
	base = NULL;
	listener = NULL;
	singal_event = NULL;
}

IoThread::~IoThread()
{
	I_pThreadHandler = NULL;
	I_IsStop = false;
	if(listener)
		evconnlistener_free(listener);
	if(singal_event)
		event_free(singal_event);
	if(base)
		event_base_free(base);	
}

bool IoThread::Init()
{
#ifdef _WIN32
	//windows使用iocp
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

#ifdef _WIN32
HANDLE IoThread::GetHandle()
{
	return I_CompletionPort;
}
#endif

void IoThread::SendMsg(TcpConnection* Conn,int size)
{
	if(!Conn)
		return;
	bufferevent_write(Conn->bufev,Conn->current_message.data,size);//唤醒event_base
}

void IoThread::DoThread()
{
#ifdef NET_LIB_EVENT
	
	struct sockaddr_in sin;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201,&wsa_data);
	evthread_use_windows_threads();
#else
	event_use_pthreads();
#endif

	base = event_base_new();
	if(!base)
	{
		cout << "Could not initialize libevent!" << endl;
		return;
	}
	const char *MethodName = event_base_get_method(base);
	if(MethodName)
		cout << "Method Name is " << MethodName << endl;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(5547);

	listener = evconnlistener_new_bind(base,Listener_cb,this,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,(struct sockaddr*)&sin,sizeof(sin));
	if(!listener)
	{
		cout << "Could not create listener!" << endl;
		return;
	}

	singal_event = evsignal_new(base,SIGINT,Signal_cb,(void *)base);
	struct timeval tv = {10, 0};
	if(!singal_event || event_add(singal_event,&tv) < 0)
	{
		cout << "Could not create/add a signal event!" << endl;
		return;
	}
	cout << "IoThread Id = " << GetThreadId() << endl;
	event_base_dispatch(base);

#endif
}