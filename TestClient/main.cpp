#include "libzmq/include/zmq.h"
#include <iostream>
#include <sstream>
#include <string>
#include "./protobuffer/player.pb.h"

unsigned short FormatMsg(char* MsgBuf,unsigned short Id,const ::google::protobuf::MessageLite& msg)
{
	int size = msg.ByteSizeLong();
	char* temp = MsgBuf;
	int iLen = 0;
	*(unsigned short *)temp = Id;
	iLen += sizeof(unsigned short);
	temp += sizeof(unsigned short);

	*(unsigned short *)temp = size;
	iLen += sizeof(unsigned short);
	temp += sizeof(unsigned short);

	memcpy(temp,msg.SerializeAsString().c_str(),size);
	return size + 2*sizeof(unsigned short);
}

int main(int argc, char * argv[])
{
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
	TestP.set_id(10);
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

	return 0;
}