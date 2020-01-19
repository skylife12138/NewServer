#include "../Prec.h"
#include "CPlayer.h"
#include "../Net/NetPack.h"
#include "../protobuffer/player.pb.h"

template <> void Cplayer::RecvMsg<MSG_HeartBeat>(NetPack* p)
{
    TestProto TestP;
	TestP.ParseFromArray(p->Data, p->Size);
	Show("TestProto is id = %d,isman = %d", TestP.id(), TestP.isman());
}
