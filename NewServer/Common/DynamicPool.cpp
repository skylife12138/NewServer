#include "DynamicPool.h"

template<class T>
DynamicPool::DynamicPool()
{
	 _MaxAllocSize =0;
	 _EveryStepSize=0;
	 _NowAllocSize =0;

	 _AllLock = NULL;
	 _FreeLock = NULL;

	 _AllMollocList.clear();
	 _FreeMollocList.clear();
}

template<class T>
DynamicPool::~DynamicPool()
{
	for(auto iter = _FreeMollocList.begin();iter != _FreeMollocList.end();++iter)
		*iter = NULL;
	_FreeMollocList.clear();
	for(auto iter = _AllMollocList.begin();iter != _AllMollocList.end();++iter)
	{
		delete [](*iter);
		*iter = NULL;
	}
	_AllMollocList.clear();
	if (_AllLock)
	{
		_AllLock->Release();
		_AllLock = NULL;
	}
	if (_FreeLock)
	{
		_FreeLock->Release();
		_FreeLock = NULL;
	}
}

template<class T>
bool DynamicPool::Init(int InitSize, int StepSize, bool IsSingleThread, int MaxSize)
{

}

template<class T>
T* DynamicPool::FetchObj()
{

}

template<class T>
void DynamicPool::ReleaseObj(T* pObj)
{

}

template<class T>
bool DynamicPool::_AllocT(DWORD ASize)
{

}