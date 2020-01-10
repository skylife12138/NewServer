#include "libzmq/include/zmq.h"
#include <iostream>
#include <sstream>
#include <string>
#include "./protobuffer/player.pb.h"
using namespace std;

#include "../libevent/include/event2/buffer.h"
#include "../libevent/include/event2/bufferevent.h"
#include "../libevent/include/event2/listener.h"
#include "../libevent/include/event2/util.h"
#include "../libevent/include/event2/event.h"

#define NET_LIB_EVENT

static void Conn_Writecb(struct bufferevent*, void *);
static void Conn_Readcb(struct bufferevent*, void *);
static void Conn_Eventcb(struct bufferevent*, short, void *);


unsigned short FormatMsg(char* MsgBuf,unsigned short Id,const ::google::protobuf::MessageLite& msg)
{
	int size = msg.ByteSizeLong();
	char* temp = MsgBuf;
	int iLen = 0;
	*(unsigned short *)temp = size+sizeof(unsigned short);
	iLen += sizeof(unsigned short);
	temp += sizeof(unsigned short);

	*(unsigned short *)temp = Id;
	iLen += sizeof(unsigned short);
	temp += sizeof(unsigned short);

	memcpy(temp,msg.SerializeAsString().c_str(),size);
	return size + 2*sizeof(unsigned short);
}

int main(int argc, char * argv[])
{
#ifndef NET_LIB_EVENT
	void * pCtx = NULL;
	void * pSock = NULL;

	const char * pAddr = "tcp://127.0.0.1:5547";

	
	if ((pCtx = zmq_ctx_new()) == NULL)
	{
		return 0;
	}
	
	if ((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
	{
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	int iSndTimeout = 5000;
	if (zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	if (zmq_connect(pSock, pAddr) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	//

	char szMsg[1024] = {0};

	TestProto TestP;
	TestP.set_id(0);
	TestP.set_isman(true);

	int size = FormatMsg(szMsg, 1, TestP);

	while (1)
	{
		printf("Enter to send...\n");
		if (zmq_send(pSock, szMsg, size, 0) < 0)
		{
			fprintf(stderr, "send message faild\n");
			continue;
		}
		printf("send message : [%s] succeed\n", szMsg);
		getchar();
	}
#else
    struct event_base *base;

	struct sockaddr_in sin;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201,&wsa_data);
#endif
	base = event_base_new();
	if(!base)
	{
		cout << "Could not initialize libevent!" << endl;
		return 0;
	}
	const char *MethodName = event_base_get_method(base);
	if(MethodName)
		cout << "Method Name is " << MethodName << endl;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
	sin.sin_port = htons(5547); /* unspecified port */

	struct bufferevent *bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	if(!bev)
	{
		cout << "Error constructing bufferevent!" << endl;
		return 0;
	}
	bufferevent_setcb(bev,Conn_Readcb,Conn_Writecb,Conn_Eventcb,NULL);
	bufferevent_enable(bev,EV_READ | EV_PERSIST);

	if(bufferevent_socket_connect(bev,(struct sockaddr *)&sin,sizeof(sin))<0)
	{
		cout << "connect error" << endl;
		bufferevent_free(bev);
		return 0;
	}

	event_base_dispatch(base);
	//event_base_free(base);
#endif

	return 0;
}

void Conn_Writecb(struct bufferevent* bev, void *arg)
{
	return;
}

void Conn_Readcb(struct bufferevent* bev, void *arg)
{
	return;
}

void Conn_Eventcb(struct bufferevent* bev, short events, void *user_data)
{
	if(events & BEV_EVENT_EOF)
		cout << "Connection closed!!!" << endl;
	else if(events & BEV_EVENT_ERROR)
	{
		cout << "Error Occur" <<EVUTIL_SOCKET_ERROR() << " "<<evutil_socket_error_to_string(5) << endl;
	}
	else if(events & BEV_EVENT_CONNECTED)
	{
		int index = 100;
		while(index>0)
		{
			char szMsg[1024] = {0};

			TestProto TestP;
			TestP.set_id(1);
			TestP.set_isman(true);

			int size = FormatMsg(szMsg, 1, TestP);
			cout << "sendmsg: " << szMsg << endl;
			//bufferevent_write(bev,szMsg,size);
			index--;
		}
		
	}

}