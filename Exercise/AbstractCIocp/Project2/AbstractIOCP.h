#pragma once

#include"BasicThing.h"
#include"LogManager.h"

class _VIRTUALIOCP // °¡»ó IOCP
{
public:
	virtual void accept(SOCKET clientsock, HANDLE hcp) abstract;
	virtual void recv(void* session, DWORD cbTransferred) abstract;
	virtual void send(void* session, DWORD cbTransferred) abstract;
	virtual void disconnect(int retval, void* session) abstract;

	void init();
	void end();
	void PostAccept(SOCKET s);


	static DWORD WINAPI WorkerThread(LPVOID arg);

	_VIRTUALIOCP();
	virtual ~_VIRTUALIOCP();
private:
	HANDLE hcp;
	HANDLE* threads;
	int count;
	ExOverlapped accept_overlapped;
};