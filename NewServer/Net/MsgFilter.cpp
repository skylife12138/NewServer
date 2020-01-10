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
        cout << "Recv Msg Not Regisit id = " << p->Id << endl;
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
    cout << "DealMsg id = " << p->Id << endl;
}

void CMsgFilter::CMsgFilter::UnDealMsg(NetPack* p,Cplayer* aPly)
{
    //发送错误消息
    cout << "UnDealMsg id = " << p->Id << endl;
}

