#include "CClient.h"
#include "MsgFilter.h"
#include "IoThread.h"


QWORD ClientMgr::GlobalSerial = 0;

CClient::CClient()
{   
    aPlayer = NULL;
    ConnData = NULL;
    Serial = 0;
    memset(Ip,0,sizeof(Ip));
}

CClient::~CClient()
{
    if(aPlayer)
        delete aPlayer;
    if(ConnData)
        delete ConnData;
}

void CClient::Init()
{
    aPlayer = new Cplayer(this);
}

void CClient::SetIp(char *ip)
{
    DWORD Len = strlen(ip) + 1;
    Len = Len > sizeof(Ip) ? sizeof(Ip)  : Len;
    strncpy(Ip,ip,Len);
    Ip[Len - 1] = '\0';
}

void CClient::AddRecvMsg(char* buff,uint16_t size)
{
    NetPack* aPack = ParseRecv(buff, size);
	if(!aPack)
	{
		cout << "ParseRecv Error!!!" << endl;
		return;
	}
    aPack->Ref++;
    RecvMsgVec.push(aPack);
}

NetPack* CClient::GetAndPopRecvMsg()
{
    if(RecvMsgVec.empty())
        return NULL;
    NetPack *aPack = RecvMsgVec.front();
    assert(aPack);
    RecvMsgVec.pop();
    return aPack;
}

void CClient::DisPatch()
{
    if(!aPlayer)
        return;
    ParseMsg();
    aPlayer->DoAction();
}

void CClient::ParseMsg()
{
    while(NetPack* aPack = GetAndPopRecvMsg())
    {
        if(!aPack)
        {
            continue;
        }
        WORD Id = aPack->Id;
        assert(aPlayer);
        if(aPlayer)
        {
            CMsgFilter::Instance()->DealMsg(aPack,aPlayer);
        }
        if((--aPack->Ref) == 0)
            FreeMsg(aPack);
    }
}

ClientMgr::~ClientMgr()
{

}

void ClientMgr::Init(int MaxPlayer)
{
    C_Lock = CreateThreadLock();
    StaticPlayerNum = MaxPlayer;
    StaticMalloc = new CClient *[StaticPlayerNum];
    for (int i = 0; i < StaticPlayerNum;i++)
    {
        StaticMalloc[i] = new CClient();
        ClientList.push(StaticMalloc[i]);
    }
    ClientFdMap.clear();
}

CClient *ClientMgr::GetClient(int ClientIndex)
{
    if(ClientIndex>=0 && ClientIndex < StaticPlayerNum)
        return StaticMalloc[ClientIndex];
    return NULL;
}

CClient *ClientMgr::GetClientByFd(QWORD sock)
{
    C_Lock->Lock();
    std::unordered_map<QWORD, CClient *>::iterator it = ClientFdMap.find(sock);
    std::unordered_map<QWORD, CClient *>::iterator end = ClientFdMap.end();
    C_Lock->UnLock();
    if(it == end)
        return NULL;
    return it->second;
}

void ClientMgr::AddClientFdMap(QWORD sock, CClient *p)
{
    C_Lock->Lock();
    ClientFdMap.insert(std::make_pair(sock,p));
    C_Lock->UnLock();
}

void ClientMgr::DelClientFdMap(QWORD sock)
{
    C_Lock->Lock();
    std::unordered_map<QWORD, CClient *>::iterator it = ClientFdMap.find(sock);
    std::unordered_map<QWORD, CClient *>::iterator end = ClientFdMap.end();
    C_Lock->UnLock();
    if(it == end)
        return;
    C_Lock->Lock();    
    ClientFdMap.erase(it);
    C_Lock->UnLock();
}

CClient *ClientMgr::PopAClient()
{
    if(ClientList.empty())
        return NULL;
    CClient *Ret = ClientList.front();
    ClientList.pop();
    assert(Ret);
    Ret->Init();
    return Ret;
}

void ClientMgr::ParseAllClient()
{
    C_Lock->Lock();
    //std::unordered_map<QWORD, CClient *> TempClientMap(ClientFdMap);
    C_Lock->UnLock();
    for(auto iter = ClientFdMap.begin();iter!=ClientFdMap.end();++iter)
    {
        CClient *p = iter->second;
        if( p )
            p->DisPatch();
    }
}