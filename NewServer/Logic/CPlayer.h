#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#include <vector>
#include "../Net/NetPack.h"
#include "Define.h"
#include "../Net/CClient.h"
#include "../protobuffer/player.pb.h"

class CClient;

class Cplayer
{
public:
    explicit Cplayer(CClient *client);
    ~Cplayer(){}
    template <EMsgId MsgId> void RecvMsg(NetPack* p);

    void DoAction();
    void SendMsg(NetPack *aPack, bool bMultiSend = false);

private:
    int PlyerId;
    CClient *mClient;
private:
    Cplayer(Cplayer &p);
    Cplayer &operator=(const Cplayer* p);
};


void MultiSendMsg(std::vector<Cplayer*> PlyVec,NetPack* aPack);

bool ParsePBMessage(NetPack* aPack,::google::protobuf::MessageLite& msg);
void SendPBMessage(Cplayer* aPly,unsigned short id,::google::protobuf::MessageLite& msg);
void MultiSendPBMessage(std::vector<Cplayer*> PlyVec,unsigned short id,::google::protobuf::MessageLite& msg);

#endif