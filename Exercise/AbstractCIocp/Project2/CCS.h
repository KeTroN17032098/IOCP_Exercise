#pragma once
#include<Windows.h>

class CCS
{
private:
	CRITICAL_SECTION cs;
public:
	CCS()
	{
		InitializeCriticalSection(&cs);
	}
	~CCS()
	{
		DeleteCriticalSection(&cs);
	}
	void Enter()
	{
		EnterCriticalSection(&cs);
	}
	void Leave()
	{
		LeaveCriticalSection(&cs);
	}
};

class LockForSession
{
private:
	CCS* ccs;
public:
	LockForSession(CCS* ccs)
	{
		this->ccs = ccs;
		this->ccs->Enter();
	}
	~LockForSession()
	{
		this->ccs->Leave();
		this->ccs = nullptr;
	}
};
