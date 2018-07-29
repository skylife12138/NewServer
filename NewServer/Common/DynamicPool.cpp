#include "DynamicPool.h"

template<class T>
DynamicPool<T>::DynamicPool()
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
DynamicPool<T>::~DynamicPool()
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
bool DynamicPool<T>::Init(int InitSize, int StepSize, bool IsSingleThread, int MaxSize)
{
	if (MaxSize && InitSize > MaxSize)
		return false;
	_MaxAllocSize = MaxSize;
	_EveryStepSize = StepSize;

	if (!IsSingleThread)
	{
		_AllLock = CreateThreadLock();
		_FreeLock = CreateThreadLock();
	}
	else
	{
		_AllLock = new FakeLock;
		_FreeLock = new FakeLock;
	}
	if (!_AllLock || _FreeLock)
		return false;
	return _AllocT(InitSize);
}

template<class T>
T* DynamicPool<T>::FetchObj()
{
	ShellT<T>* pShellT = NULL;
	_FreeLock->Lock();
	if (_FreeMollocList.empty())
	{
		_FreeLock->UnLock();
		if (!_AllocT(_EveryStepSize))
			return NULL;
		_FreeLock->Lock();
	}
	pShellT = _FreeMollocList.front();
	if (!pShellT)
	{
		_FreeLock->UnLock();
		return NULL;
	}
	if (pShellT->UseCount)
	{
		_FreeLock->UnLock();
		return NULL;
	}
	pShellT->UseCount++;

	_FreeMollocList.pop_front();
	_FreeLock->UnLock();
	return pShellT;
}

template<class T>
void DynamicPool<T>::ReleaseObj(T* pObj)
{
	if (!pObj)
		return;
	_FreeLock->Lock();
	ShellT* pShellT = static_cast<ShellT*>(pObj);
	if (!pShellT)
	{
		_FreeLock->UnLock();
		return;
	}
	if (pShellT->UseCount != 1)
	{
		_FreeLock->UnLock();
		return;
	}
	pShellT->UseCount--;
	_FreeMollocList->push_back(pShellT);
	_FreeLock->UnLock();
}

template<class T>
bool DynamicPool<T>::_AllocT(DWORD ASize)
{
	if (ASize == 0)
		return false;
	if (_MaxAllocSize && _NowAllocSize + ASize > _MaxAllocSize)
		return false;
	_FreeLock->Lock();
	if (!_FreeMollocList.empty())
	{
		_FreeLock->UnLock();
		return true;
	}
	DWORD NewSize = ASize;
	if (_MaxAllocSize)
	{
		if (_NowAllocSize + NewSize >= _MaxAllocSize)
			NewSize = _MaxAllocSize - _NowAllocSize;
	}
	ShellT<T>* pShellT = new ShellT<T>[NewSize];
	if (!pShellT)
	{
		_FreeLock->UnLock();
		return false;
	}
	_NowAllocSize += NewSize;
	for (DWORD i = 0; i < NewSize; i++)
		_FreeMollocList.push_back(&pShellT[i]);
	_FreeLock->UnLock();

	_AllLock->Lock();
	_AllMollocList.push_back(pShellT);
	_AllLock->UnLock();

	return true;
}

//以下为测试代码

TestDyPool::TestDyPool()
{
	ObjPool.Init(100, 10,true);
}

TestObj* TestDyPool::Fetch()
{
   return ObjPool.FetchObj();
}