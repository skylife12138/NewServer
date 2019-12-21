#include "libzmq/include/zmq.h"
#include <iostream>

int main(int argc, char * argv[])
{
	void * pCtx = NULL;
	void * pSock = NULL;
	//ʹ��tcpЭ�����ͨ�ţ���Ҫ���ӵ�Ŀ�����IP��ַΪ192.168.1.2
	//ͨ��ʹ�õ�����˿� Ϊ7766 
	const char * pAddr = "tcp://127.0.0.1:5547";

	//����context 
	if ((pCtx = zmq_ctx_new()) == NULL)
	{
		return 0;
	}
	//����socket 
	if ((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
	{
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	int iSndTimeout = 5000;// millsecond
						   //���ý��ճ�ʱ 
	if (zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	//����Ŀ��IP192.168.1.2���˿�7766 
	if (zmq_connect(pSock, pAddr) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	//ѭ��������Ϣ 
	while (1)
	{
		static int i = 0;
		char szMsg[1024] = { 0 };
		snprintf(szMsg, sizeof(szMsg), "hello world : %3d", i++);
		printf("Enter to send...\n");
		if (zmq_send(pSock, szMsg, sizeof(szMsg), 0) < 0)
		{
			fprintf(stderr, "send message faild\n");
			continue;
		}
		printf("send message : [%s] succeed\n", szMsg);
		getchar();
	}

	return 0;
}