#include "ObjectFactory.h"
#include "MemoryPool.h"

#include "../Net/NetPack.h"
#include "Log.h"

struct GenerateObjectPoolAggregation
{
    ObjectPool<SLogMsgData> _PoolLogMsg;
    ObjectPool<NetPack> _PoolNetPack;
    
};

ObjectFactory::ObjectFactory()
{
    int PlayerNum = 10;

    int TotalSize = 0;
    TotalSize += (sizeof(SLogMsgData)) * (PlayerNum * 2);
    TotalSize += (sizeof(NetPack) + BIG + sizeof(int)+sizeof(int)) * (PlayerNum * 5);
    
    //...

    _impl = (GenerateObjectPoolAggregation *)operator new(sizeof(GenerateObjectPoolAggregation) + TotalSize);
    new (_impl) GenerateObjectPoolAggregation;

    int UseSize = 0;
    UseSize +=  _impl->_PoolLogMsg.Init(0,PlayerNum*2,"LogMsg",(char*)_impl+sizeof(GenerateObjectPoolAggregation)+UseSize,false);
    UseSize +=  _impl->_PoolNetPack.Init(BIG+sizeof(int),PlayerNum*5,"NetPack",(char*)_impl+sizeof(GenerateObjectPoolAggregation)+UseSize,false);
    //...

    assert(UseSize == TotalSize);
    Show("ObjectPool Size = %d", TotalSize);
}

ObjectFactory::~ObjectFactory()
{
    _impl->_PoolNetPack.UnInit();
    //...
    _impl->_PoolLogMsg.UnInit();//必须在最后析构
    
    _impl->~GenerateObjectPoolAggregation();
    operator delete(_impl);
}

void ObjectFactory::ShowState()
{
    int TotalMem = 0;
    TotalMem = _impl->_PoolLogMsg.ShowState();
    TotalMem = _impl->_PoolNetPack.ShowState();
    //...

    Show("All Memory Size = %d", TotalMem);
}

template <>NetPack *ObjectFactory::Create<NetPack,int>(int p)
{
    return _impl->_PoolNetPack.Create(p);
}

template <>void ObjectFactory::Destory<NetPack>(NetPack* obj)
{
    _impl->_PoolNetPack.Destory(obj);
}

template <>bool ObjectFactory::NotEnough<NetPack>()
{
    return _impl->_PoolNetPack.NotEnough();
}

template <>SLogMsgData *ObjectFactory::Create<SLogMsgData>()
{
    return _impl->_PoolLogMsg.Create();
}

template <>void ObjectFactory::Destory<SLogMsgData>(SLogMsgData* obj)
{
    _impl->_PoolLogMsg.Destory(obj);
}

template <>bool ObjectFactory::NotEnough<SLogMsgData>()
{
    return _impl->_PoolLogMsg.NotEnough();
}
