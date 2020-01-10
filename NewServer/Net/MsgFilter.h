#ifndef _MSGFILTER_H_
#define _MSGFILTER_H_
#include "NetPack.h"
#include <iostream>
#include "../Logic/Define.h"
#include "../Logic/CPlayer.h"

typedef void (Cplayer::*MsgHandler)(NetPack *p);

struct SMsgTimeAndHandler
{
    DWORD LastRecvTime;
    DWORD MinTimeSpace;
    MsgHandler mHandler;
};

class CMsgFilter : public SingleTon<CMsgFilter>
{
public:
    CMsgFilter();
    void AddHandler(WORD MsgId, MsgHandler h, DWORD Time);

    void DealMsg(NetPack *p,Cplayer* aPly);
    void UnDealMsg(NetPack* p,Cplayer* aPly);
private:
    std::map<WORD, SMsgTimeAndHandler> mHandlerData;
};

#endif