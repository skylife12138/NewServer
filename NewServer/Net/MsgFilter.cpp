#include "MsgFilter.h"

#define AddTemplateHandler(MsgId,Time) AddHandler(MsgId,&Cplayer::RecvMsg<MsgId>,Time)

CMsgFilter::CMsgFilter()
{
    AddTemplateHandler(MSG_HeartBeat, 0);
}

void CMsgFilter::AddHandler(WORD MsgId, MsgHandler h, DWORD Time)
{
    assert(MsgId < MSG_MAX);
    SMsgTimeAndHandler mData;
    mData.LastRecvTime = 0;
    mData.MinTimeSpace = Time;
    mData.mHandler = h;
    mHandlerData.emplace(MsgId,mData);
}

void CMsgFilter::DealMsg(NetPack *p,Cplayer* aPly)
{
    if(!aPly)
        return;
    assert(p->Id < MSG_MAX);
    if(p->Id >= MSG_MAX)
    {
        delete aPly;
        return;
    }
    auto iter = mHandlerData.find(p->Id);
    if(iter == mHandlerData.end())
    {
        Error("Recv Msg Not Regisit id = %d", p->Id);
        delete aPly;
        return;
    }
    if(iter->second.LastRecvTime + iter->second.MinTimeSpace > GTimer->GetNowTimeStamp())
    {
        UnDealMsg(p,aPly);
        delete aPly;
        return;
    }
    iter->second.LastRecvTime = GTimer->GetNowTimeStamp();
    (aPly->*(iter->second.mHandler))(p);
    Show("DealMsg id = %d", p->Id);
}

void CMsgFilter::CMsgFilter::UnDealMsg(NetPack* p,Cplayer* aPly)
{
    //发送错误消息
    Error("UnDealMsg id = %d",p->Id);
}

