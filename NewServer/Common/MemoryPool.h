#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_
#include<deque>
#include "Portable.h"
#include "Lock.h"
#include "Common.h"
#include<iostream>



class LockMeta;
class FakerLock:public LockMeta
{
public:
	FakerLock() {};
	~FakerLock() {};
	void Lock() {}
	void UnLock() {}
	void Release()
	{
		if (this)
			delete this;
	}
};

class MemoryPool
{
public:
	int Init(int Size,int Num,char* Name,void* mem=NULL, bool IsSingleThread=true,int MaxSize=0);//MaxSize=0
	void UnInit();
	void* Alloc();
	void Release(void* node);

	int ShowState();
	bool NotEnough();

private:
	char _PoolName[32];
	bool _NotEnough;
	DWORD _UseNum;
	DWORD _MaxUseNum;
	DWORD _AllocStaticNum;
	DWORD _AllocExtNum;

	DWORD _EachSize;
	DWORD _MaxAllocSize;

	LockMeta* _Lock;

	void* _AllMollocList;
	void* _FreeMollocList;
	void *_ExtMollocList;
};

struct ShellCount
{
public:
	ShellCount() : UseCount(0){};
	int UseCount; //引用计数
};

template<class T>
class ShellT:public ShellCount, public T //必须先继承ShellCount保证UseCount变量在ShellT的内存头部
{
public:
	ShellT(){ };
	template <class P> ShellT(P p) : T(p){};
	template <class P1, class P2> ShellT(P1 p1, P2 p2) : T(p1, p2){};
	~ShellT(){};
public: 
	
};

template<class T>
class ObjectPool
{
public:
	int Init(int ExtSize, int Num, char *Name, void *mem = NULL, bool IsSingleThread = true, int MaxSize = 0);
	void UnInit()
	{
		_MemPool.UnInit();
	}
	bool NotEnough() { return _MemPool.NotEnough(); }
	void Destory(T *Obj);

	T* Create()
	{
		_Lock->Lock();
		ShellT<T> *node = (ShellT<T>*)_MemPool.Alloc();
		new (node) ShellT<T>();
		node->UseCount++;
		_Lock->UnLock();
		return (T *)node;
	}
	template<class P>
	T* Create(P p)
	{
		_Lock->Lock();
		ShellT<T> *node = (ShellT<T>*)_MemPool.Alloc();
		new (node) ShellT<T>(p);
		node->UseCount++;
		_Lock->UnLock();
		return (T *)node;
	}
	template<class P1,class P2>
	T* Create(P1 p1,P2 p2)
	{
		_Lock->Lock();
		ShellT<T> *node = (ShellT<T>*)_MemPool.Alloc();
		new (node) ShellT<T>(p1,p2);
		node->UseCount++;
		_Lock->UnLock();
		return (T *)node;
	}
	int ShowState()
	{
		return _MemPool.ShowState();
	}
private:
	MemoryPool _MemPool;
	LockMeta* _Lock;
};

template<class T>
int ObjectPool<T>::Init(int ExtSize, int Num, char *Name, void *mem, bool IsSingleThread, int MaxSize)
{
	if (!IsSingleThread) 
		_Lock = CreateThreadLock();
	else
		_Lock = new FakerLock;
	if (!_Lock)
	{
		assert(false);
		return 0;
	}
	return _MemPool.Init(sizeof(ShellT<T>)+ExtSize,Num,Name,mem,true,MaxSize);
}

template<class T>
void ObjectPool<T>::Destory(T* Obj)
{
	_Lock->Lock();
	ShellT<T>* pShellT = static_cast<ShellT<T>*>(Obj);
	if(!pShellT || pShellT->UseCount!=1)
	{
		_Lock->UnLock();
		return;
	}
	pShellT->UseCount--;
	Obj->~T();
	_MemPool.Release(Obj);
	_Lock->UnLock();
}


#endif
