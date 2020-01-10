#ifndef _CCLIENT_H_
#define _CCLIENT_H_

#include <queue>
#include <unordered_map>
#include "../Logic/SingleTon.h"
#include "../Common/Portable.h"
#include "../Logic/CPlayer.h"

const int MAX_IP_LEN = 16;

class TcpConnection;
class NetPack;
class Cplayer;

class CClient
{
public:
    CClient();
    ~CClient();
    void Init();
    Cplayer *GetPlayer() { return aPlayer; }
    void SetSerial(QWORD Ser) { Serial = Ser; }
    void SetIp(char *ip);

    void SetConnection(TcpConnection *con) { ConnData = con; }
    TcpConnection *GetTcpConnection() { return ConnData; }
    void AddRecvMsg(char* buff,uint16_t size);
    NetPack *GetAndPopRecvMsg();

    void DisPatch();
    void ParseMsg();

private:
    Cplayer *aPlayer;
    TcpConnection *ConnData;
    std::queue<NetPack *> RecvMsgVec;//后续用无锁队列,因为接收和解析在不同线程，所以使用队列

    QWORD Serial;
    char Ip[MAX_IP_LEN];
};

class ClientMgr:public SingleTon<ClientMgr>
{
public:
    ClientMgr() { Init(100); }
    virtual ~ClientMgr();
    void Init(int MaxPlayer);
    CClient *GetClient(int ClientIndex);
    CClient *GetClientByFd(QWORD sock);
    void AddClientFdMap(QWORD sock, CClient *p);
    void DelClientFdMap(QWORD sock);

    CClient *PopAClient();

    void ParseAllClient();
public:
    static QWORD GlobalSerial;
private:
    LockMeta *C_Lock;
	
	
    int StaticPlayerNum;
    CClient **StaticMalloc;
    std::queue<CClient *> ClientList;
    std::unordered_map<QWORD, CClient *> ClientFdMap;
};








#endif