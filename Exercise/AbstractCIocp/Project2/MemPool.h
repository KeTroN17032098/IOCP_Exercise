#pragma once

#include<Windows.h>
#include<iostream>
#include<fstream>

template <class T, int ALLOC_BLOCK_SIZE = 50>
class MemPool
{
private:
	static UCHAR* mStartPointer;
	static BYTE* mUsingBlock;
	static VOID AllocBlock()
	{
		mStartPointer = new UCHAR[(sizeof(T)) * ALLOC_BLOCK_SIZE];
		mUsingBlock = new BYTE[ALLOC_BLOCK_SIZE];
		for (int i = 0; i < ALLOC_BLOCK_SIZE; i++)
		{
			mUsingBlock[i] = 0;
		}
	}
	static int FindEmptyBlock()
	{
		for (int i = 0; i < ALLOC_BLOCK_SIZE; i++)
		{
			if (mUsingBlock[i]!=0)
			{
				return i;
			}
		}
		return -1;
	}
protected:
public:
	virtual ~MemPool() {}
	static VOID* operator new(std::size_t allocLength)
	{
		printf("=============================\n");
		if (!mStartPointer)AllocBlock();
		int a = FindEmptyBlock();
		if (a == -1)return NULL;
		printf("%d\n", a);
		UCHAR* returnP = mStartPointer + sizeof(T) * a;
		mUsingBlock[a] = 1;
		printf("=============================\n");
		return returnP;
	}
	static VOID operator delete(VOID* deletepointer)
	{
		printf("=============================\n");
		int a = static_cast<int>(reinterpret_cast<UCHAR*>(deletepointer) - mStartPointer) / sizeof(T);
		printf("%d\n", a);
		mUsingBlock[a] = 0;
		printf("=============================\n");
	}

	static VOID printBlock()
	{
		std::ofstream fout("memorycheck.txt", std::ios_base::out | std::ios_base::app);
		if (fout.is_open()==false)
		{
			return;
		}
		for (int i = 0; i < ALLOC_BLOCK_SIZE; i++)
		{
			if (!mStartPointer)AllocBlock();
			fout << i << " : " << static_cast<int>(mUsingBlock[i]) << "\n";
		}
		fout.close();
	}
};

template<class T, int ALLOC_BLOCK_SIZE>
UCHAR* MemPool<T, ALLOC_BLOCK_SIZE>::mStartPointer;
template<class T, int ALLOC_BLOCK_SIZE>
BYTE* MemPool<T, ALLOC_BLOCK_SIZE>::mUsingBlock;