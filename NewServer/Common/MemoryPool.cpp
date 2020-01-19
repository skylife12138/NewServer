#include "MemoryPool.h"

int MemoryPool::Init(int Size,int Num,char* Name, void* MemArry,bool IsSingleThread, int MaxSize)
{
	if (MaxSize && Size*Num > MaxSize)
	{
		assert(false);
		//ERROR
		return 0;
	}
	_EachSize = Size;
	_MaxAllocSize = MaxSize;
	_AllocStaticNum = Num;
	_AllocExtNum = 0;
	_UseNum = 0;
	_MaxUseNum = 0;
	_NotEnough = false;

	strncpy(_PoolName,Name,sizeof(_PoolName));
	_PoolName[sizeof(_PoolName) - 1] = '\0';
	_AllMollocList = NULL;
	_FreeMollocList = NULL;
	_ExtMollocList = NULL;

	if (!IsSingleThread) 
		_Lock = CreateThreadLock();
	else
		_Lock = new FakerLock;
	if (!_Lock)
	{
		assert(false);
		return 0;
	}
	if(!MemArry)
	{
		_AllMollocList = operator new(Size * Num);
		MemArry = _AllMollocList;
	}
	for(int i=0;i<Num;i++)
	{
		void **p = (void **)((char *)MemArry + i * Size);
		*p = _FreeMollocList;
		_FreeMollocList = p;
	}
	return Size * Num;
}

void MemoryPool::UnInit()
{
	cout << "delete memorypool " << _PoolName << endl;
	int num = 0;
	void *pos = _FreeMollocList;
	while(pos)
	{
		++num;
		pos = *(void **)pos;
	}
	if(num != _AllocStaticNum + _AllocExtNum)
	{
		assert(false);
		//Error
	}
	if(_AllMollocList)
	{
		operator delete(_AllMollocList);
		_AllMollocList = NULL;
	}
	pos = _ExtMollocList;
	while(pos)
	{
		void *next = *(void **)pos;
		operator delete(pos);
		pos = next;
	}
	if (_Lock)
	{
		_Lock->Release();
		_Lock = NULL;
	}
}

void* MemoryPool::Alloc()
{
	_Lock->Lock();
	void *node = NULL;
	if(_FreeMollocList)
	{
		node = _FreeMollocList;
		_FreeMollocList = *(void **)_FreeMollocList;
	}
	else
	{
		if(strcmp(_PoolName,"LogMsg"))//防止写日志发生死循环
			Show("MemoryPool %s Not Enouth!",_PoolName);

		node = operator new(_EachSize);
		*(void **)node = _ExtMollocList;
		_ExtMollocList = node;
		++_AllocExtNum;
		_NotEnough = true;
		node = (char *)node + sizeof(void*);
	}
	++_UseNum;
	if(_MaxUseNum < _UseNum)
		_MaxUseNum = _UseNum;
	_Lock->UnLock();
	return node;
}

void MemoryPool::Release(void* node)
{
	if (!node)
		return;
	_Lock->Lock();
	--_UseNum;
	*(void **)node = _FreeMollocList;
	_FreeMollocList = node;
	_Lock->UnLock();
}

int MemoryPool::ShowState()
{
	int TotalMem = (_AllocStaticNum + _AllocExtNum) * _EachSize;
	Show("MemoryPool %s , UseNum = %d , TotalMem = %d",_PoolName, _UseNum,TotalMem);
	return TotalMem;
}

bool MemoryPool::NotEnough()
{
	bool NotEnough = _NotEnough;
	_NotEnough = false;
	return NotEnough;
}