#include "ObjectFactory.h"
#include "MemoryPool.h"

#include "../Net/NetPack.h"

struct GenerateObjectPoolAggregation
{
    ObjectPool<NetPack> _PoolNetPack;
};

ObjectFactory::ObjectFactory()
{
    int PlayerNum = 10;

    int TotalSize = 0;
    TotalSize += (sizeof(NetPack) + BIG + sizeof(int)+sizeof(int)) * (PlayerNum * 5);
    //...

    _impl = (GenerateObjectPoolAggregation *)operator new(sizeof(GenerateObjectPoolAggregation) + TotalSize);
    new (_impl) GenerateObjectPoolAggregation;

    int UseSize = 0;
    UseSize +=  _impl->_PoolNetPack.Init(BIG+sizeof(int),PlayerNum*5,"NetPack",(char*)_impl+sizeof(GenerateObjectPoolAggregation)+UseSize,false);
    //...

    assert(UseSize == TotalSize);
    cout << "ObjectPool Size = " << TotalSize << endl;
}

ObjectFactory::~ObjectFactory()
{
    _impl->_PoolNetPack.UnInit();
    //...
    _impl->~GenerateObjectPoolAggregation();
    operator delete(_impl);
}

void ObjectFactory::ShowState()
{
    int TotalMem = 0;
    TotalMem = _impl->_PoolNetPack.ShowState();
    //...

    cout << "All Memory Size = " << TotalMem << endl;
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
