#ifndef _DYNAMICPOOL_H_
#define _DYNAMICPOOL_H_
#include<queue>

template<class T>
class ShellT:public T
{
public:
	ShellT():UseCount(0){};
	~ShellT() {};
	int UseCount; //记录引用的次数
};

template<class T>
class DynamicPool
{
public:
	explicit DynamicPool();
	~DynamicPool();

	bool Init(int InitSize,int StepSize,bool IsSingleThread=false,int MaxSize=0);//MaxSize=0表示没有最大限制
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

	std::queue<ShellT*> _AllMollocList;
	std::queue<ShellT*> _FreeMollocList;
};

#endif
