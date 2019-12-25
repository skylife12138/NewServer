#ifndef _NETPACK_H_
#define _NETPACK_H_
#include "../Common/common.h"
#include "../Common/Portable.h"

template<typename A>
struct base_type {typedef A type;};
template<typename A>
struct base_type<A*> {typedef A type;};
template<typename A>
struct base_type<A&> {typedef A type;};

#pragma warning(disable:4200)
#pragma pack(push,1)//把原来对齐方式设置压栈，设新的对齐方式设置为1个字节对齐

struct NetPack
{
    volatile unsigned short Ref;
    unsigned short Id;
    unsigned short Size;
    unsigned short Pos;
    const unsigned short Type;
    char Data[0];

    explicit NetPack(unsigned short tp)
    :Ref(0)
    ,Id(0)
    ,Size(0)
    ,Pos(0)
    ,Type(tp)
    {
    }

    template<typename T>
    NetPack& operator<<(T t)
    {
        memcpy(Data + Size, &t, sizeof(typename base_type<T>::type));
        Size += (unsigned short)(sizeof(typename base_type<T>::type));
        assert(Size <= Type);
        return *this;
    }

    template<typename T>
    T& Pop()
    {
        assert(Pos <= Size-sizeof(typename base_type<T>::type));
        T *Ret = (T *)(Data + Pos);
        Pos += sizeof(typename base_type<T>::type);
        return *Ret;
    }

    template<typename T>
    NetPack& Push(T t)
    {
        memcpy(Data + Size, &t, sizeof(typename base_type<T>::type));
        Size += (unsigned short)(sizeof(typename base_type<T>::type));
        assert(Size <= Type);
        return *this;
    }

    DWORD PopBuff(char* Buff,DWORD Len)
    {
        assert(Size >= Pos);
        unsigned short CopySize = std::min((unsigned short)(Size - Pos), (unsigned short)(Len));
        memcpy(Buff, Data + Pos, CopySize);
        Pos += CopySize;
        return CopySize;
    }

    DWORD PushBuff(const char* Buff,DWORD Len)
    {
        assert(Size <= Type);
        unsigned short CopySize = std::min((unsigned short)(Type - Size), (unsigned short)(Len));
        memcpy(Data + Pos,Buff,CopySize);
        Size += CopySize;
        return CopySize;
    }

    bool CanPop(DWORD len)
    {
        return Pos + len <= Size;
    }

    bool CanPush(DWORD len)
    {
        return Size + len <= Type;
    }

    char* CurrentPopBuff() 
    {
        return Data + Pos;
    }

    void AddPopPos(WORD len) 
    { 
        assert(Pos+len<=Size);
        Pos += len;
    }

    template<typename T>
    T* CurrentPopPtr()
    {
        assert(Pos + sizeof(T) <= Size);
        T *Ret = (T *)(Data + Pos);
        return Ret;
    }

    template<typename T>
    T* CurrentPushPtr()
    {
        assert(Size + sizeof(T) <= Type);
        T *Ret = (T *)(Data + Size);
        return Ret;
    }

    //以下函数可以导出给脚本使用
    char PopInt8()
    {
        assert(Pos <= (Size-sizeof(char)));
        char Ret = *(char *)(Data + Pos);
        Pos += sizeof(char);
        return Ret;
    }

    short PopInt16()
    {
        assert(Pos <= (Size-sizeof(short)));
        short Ret = *(short *)(Data + Pos);
        Pos += sizeof(short);
        return Ret;
    }

    int PopInt32()
    {
        assert(Pos <= (Size-sizeof(int)));
        int Ret = *(int *)(Data + Pos);
        Pos += sizeof(int);
        return Ret;
    }

    float PopFloat()
    {
        assert(Pos <= (Size-sizeof(float)));
        float Ret = *(float *)(Data + Pos);
        Pos += sizeof(float);
        return Ret;
    }

    NetPack& PushInt8(char s)
    {
        memcpy(Data+Size,&s,sizeof(char));
        Size += (unsigned short)sizeof(char);
        return *this;
    }

    NetPack& PushInt16(short s)
    {
        memcpy(Data+Size,&s,sizeof(short));
        Size += (unsigned short)sizeof(short);
        return *this;
    }

    NetPack& PushInt32(int s)
    {
        memcpy(Data+Size,&s,sizeof(int));
        Size += (unsigned short)sizeof(int);
        return *this;
    }

    NetPack& PushFloat(float s)
    {
        memcpy(Data+Size,&s,sizeof(float));
        Size += (unsigned short)sizeof(float);
        return *this;
    }
};
#pragma pack(pop)//恢复原来对齐状态

extern NetPack* NewMsgId(unsigned short Id, int type = TINY);
extern NetPack *NewMsgSize(int size,unsigned short Id);
extern void     FreeMsg(NetPack* p);
extern NetPack *ParseRecv(char* pBuf,unsigned int iLen);

extern bool     CheckPackMem(NetPack* p);

class CNetPackPool:public SingleTon<CNetPackPool>
{
public:
    CNetPackPool();
    ~CNetPackPool();
    NetPack *Pop(int type,unsigned short Id);
    void Push(NetPack* pack);
    void ShowState();
private:
    static const int MAX_MSG_ID = 65536;
    AtomicNumber mMsgNum[MAX_MSG_ID];
    AtomicNumber mMsgPopNum[MAX_MSG_ID];
};

#endif