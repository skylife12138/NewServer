#ifndef _DYNAMICPOOL_H_
#define _DYNAMICPOOL_H_
#include<deque>
#include "Portable.h"
#include "Lock.h"
#include "Common.h"
#include<iostream>

class LockMeta;
class FakeLock:public LockMeta
{
public:
	FakeLock() {};
	~FakeLock() {};
	void Lock() {}
	void UnLock() {}
	void Release()
	{
		if (this)
			delete this;
	}
};


template<class T>
class ShellT:public T
{
public:
	ShellT():UseCount(0){};
	~ShellT() {};
	int UseCount; //��¼���õĴ���
};

template<class T>
class DynamicPool
{
public:
	explicit DynamicPool();
	~DynamicPool();

	bool Init(int InitSize,int StepSize,bool IsSingleThread=false,int MaxSize=0);//MaxSize=0��ʾû���������
	T* FetchObj();
	void ReleaseObj(T* pObj);
private:
	bool _AllocT(DWORD ASize);
private:
	DWORD _MaxAllocSize;
	DWORD _EveryStepSize;
	DWORD _NowAllocSize;

	LockMeta* _AllLock;
	LockMeta* _FreeLock;

	std::deque<ShellT<T>*> _AllMollocList;
	std::deque<ShellT<T>*> _FreeMollocList;
};

//����Ϊ���Դ���

class TestObj
{
public:
	TestObj() {}
	~TestObj() {}

	void Init() { std::cout << "test dynamicpool" << std::endl; }
};

class TestDyPool
{
public:
	TestDyPool();
	~TestDyPool() {}
	TestObj* Fetch();
private:
	DynamicPool<TestObj> ObjPool;
};

#endif
