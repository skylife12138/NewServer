#include "../Prec.h"
#include "CPlayer.h"
#include "../Net/IoThread.h"

void MultiSendMsg(std::vector<Cplayer*> PlyVec,NetPack* aPack)
{
    int Count = PlyVec.size();
    if(Count == 0)
    {
        FreeMsg(aPack);
        return;
    }
    aPack->Ref += Count;
    for (auto iter = PlyVec.begin(); iter != PlyVec.end();++iter)
    {
        Cplayer *aPly = *iter;
        aPly->SendMsg(aPack,true);
    }
}

bool ParsePBMessage(NetPack* aPack,::google::protobuf::MessageLite& msg)
{
    if(!msg.ParseFromArray(aPack->Data,aPack->Size))
        return false;
    return true;
}

void SendPBMessage(Cplayer* aPly,unsigned short id,::google::protobuf::MessageLite& msg)
{
    int size = msg.ByteSizeLong();
    NetPack *aPack = NewMsgSize(size,id);
    if(aPack)
    {
        uint8_t *begin = (uint8_t *)aPack->Data;
        uint8_t *end = msg.SerializeWithCachedSizesToArray(begin);
        if(end-begin!=size)
        {
            FreeMsg(aPack);
            return;
        }
        aPack->Size = msg.GetCachedSize();
        aPly->SendMsg(aPack);
    }
}

void MultiSendPBMessage(std::vector<Cplayer*> PlyVec,unsigned short id,::google::protobuf::MessageLite& msg)
{
    int size = msg.ByteSizeLong();
    NetPack *aPack = NewMsgSize(size,id);
    if(aPack)
    {
        uint8_t *begin = (uint8_t *)aPack->Data;
        uint8_t *end = msg.SerializeWithCachedSizesToArray(begin);
        if(end-begin!=size)
        {
            FreeMsg(aPack);
            return;
        }
        aPack->Size = msg.GetCachedSize();
        MultiSendMsg(PlyVec,aPack);
    }
}

Cplayer::Cplayer(CClient* client)
{
    PlyerId = 0;
    mClient = client;
}

static int CalCount = 0;
void Cplayer::DoAction()
{
    NetPack *aPack = NewMsgId(MSG_HeartBeat,NORMAL);
    if(!aPack)
        return;
    aPack->Push<int>(1);
    SendMsg(aPack);
    CalCount++;
}

static unsigned short FormatMsg(char* MsgBuf,NetPack* aPack)
{
	char* temp = MsgBuf;
	int iLen = 0;
	*(unsigned short *)temp = aPack->Size+sizeof(unsigned short);
	iLen += sizeof(unsigned short);
	temp += sizeof(unsigned short);

	*(unsigned short *)temp = aPack->Id;
	iLen += sizeof(unsigned short);
	temp += sizeof(unsigned short);

	memcpy(temp,aPack->Data,aPack->Size);
	return aPack->Size + 2*sizeof(unsigned short);
}

void Cplayer::SendMsg(NetPack *aPack, bool bMultiSend /*= false*/)
{
    if(!aPack || !mClient || !mClient->GetTcpConnection())
        return;   //Error
    if(!bMultiSend)
        aPack->Ref++;
    int size = FormatMsg(mClient->GetTcpConnection()->current_message.data,aPack);
    IoThread::Instance()->SendMsg(mClient->GetTcpConnection(),size);
    aPack->Ref--;
    if(aPack->Ref == 0)
        FreeMsg(aPack);
}

