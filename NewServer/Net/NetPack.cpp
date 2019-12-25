#include "NetPack.h"
#include <sstream>

CNetPackPool::CNetPackPool()
{
    memset(mMsgNum,0,MAX_MSG_ID);
    memset(mMsgPopNum,0,MAX_MSG_ID);
}

CNetPackPool::~CNetPackPool()
{
    ShowState();
    Show("Destroy NetPackPool OK!");
}

void CNetPackPool::ShowState()
{
    Show("Show NetMessage Number");
    for (int i = 0; i < MAX_MSG_ID;i++)
    {
        if(mMsgNum[i].Get()>0 || mMsgPopNum[i].Get()>0)
        {
            std::stringstream ss;
            ss << "(MsgId,Push,Pop)=(" << i << ","<<mMsgNum[i].Get()<<","<<mMsgPopNum[i].Get()<<")";
            Show(ss.str().c_str());
        }
    }
    Show("Show NetMessage Different Number");
    for (int i = 0; i < MAX_MSG_ID;i++)
    {
        if(mMsgNum[i].Get() != mMsgPopNum[i].Get())
        {
            std::stringstream ss;
            ss << "(MsgId,Push,Pop)=(" << i << ","<<mMsgNum[i].Get()<<","<<mMsgPopNum[i].Get()<<")";
            Show(ss.str().c_str());
        }
    }
}

NetPack* CNetPackPool::Pop(int type,unsigned short Id)
{
    assert(Id >= 0 && Id < 65536);
    NetPack* pack = new NetPack(type);//后续用内存池
    if(!pack)
    {
        Show("NetPack not Enough!");
        return NULL;
    }
    pack->Id = Id;
    *(int *)&pack->Data[pack->Type] = 0xDeadBeef;
    //if(pack->Id >= 0 && pack->Id < MAX_MSG_ID)
        //mMsgPopNum[pack->Id].Inc();
    return pack;
}

void CNetPackPool::Push(NetPack* pack)
{
    assert(pack->Id >= 0 && pack->Id < MAX_MSG_ID);
    //if(pack->Id >= 0 && pack->Id < MAX_MSG_ID)
        //mMsgNum[pack->Id].Inc();
    delete pack;//后续用内存池
    Show("Destory Netpack success!");
}

NetPack* NewMsgId(unsigned short Id, int type)
{
    return CNetPackPool::Instance()->Pop(type, Id);
}

NetPack *NewMsgSize(int size,unsigned short Id)
{
    NetPack *pack = NULL;
    if(size <= TINY)
    {
        pack = CNetPackPool::Instance()->Pop(TINY, Id);
    }
    else if(size <= NORMAL)
    {
        pack = CNetPackPool::Instance()->Pop(NORMAL, Id);
    }
    else if(size <= BIG)
    {
        pack = CNetPackPool::Instance()->Pop(BIG, Id);
    }
    if(pack)
        pack->Size = size;
    return pack;
}

void  FreeMsg(NetPack* p)
{
    assert(p->Ref == 0);
    CNetPackPool::Instance()->Push(p);
}

NetPack* ParseRecv(char* pBuf,unsigned int iLen)
{
    NetPack *pack = NULL;
    unsigned short Id, Size;
    if(!pBuf || iLen < 4)
        return NULL;
    Id = *(unsigned short*)pBuf;
    Size = *(unsigned short*)(pBuf + WordSize);
    if(Size > BIG)
        return NULL;
    if((Size + WordSize + WordSize) > iLen)
        return NULL;
    pack = NewMsgSize(Size,Id);
    if(!pack)
        return NULL;
    memcpy(pack->Data,(pBuf+2*WordSize),pack->Size);
    return pack;
}

bool  CheckPackMem(NetPack* p)
{
    short type = p->Type;
    bool ret = true;
    switch(type)
    {
        case TINY:
            if(*(int*)(p->Data+TINY) != 0xDeadBeef)
                ret = false;
            break;
        case NORMAL:
            if(*(int*)(p->Data+NORMAL) != 0xDeadBeef)
                ret = false;
            break;
        case BIG:
            if(*(int*)(p->Data+BIG) != 0xDeadBeef)
                ret = false;
            break;
        default:
            ret = false;
            break;
    }
    if(!ret)
    {
        assert(false);
        Show("check package memory error!!!");
    }
    return ret;
}