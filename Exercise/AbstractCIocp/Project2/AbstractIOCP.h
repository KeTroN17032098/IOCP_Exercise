#pragma once
#include"BasicThing.h"

class _VIRTUALIOCP // °¡»ó IOCP
{
public:
	virtual void accept(SOCKET clientsock, HANDLE hcp) abstract;
	virtual void recv(void* session, DWORD cbTransferred) abstract;
	virtual void send(void* session, DWORD cbTransferred) abstract;
	virtual void disconnect(int retval, void* session) abstract;
	virtual void init(int portno) abstract;
	virtual void end() abstract;
	virtual void run() abstract;
	virtual HANDLE getHCP() abstract;
	static DWORD WINAPI WorkerThread(LPVOID arg);
};