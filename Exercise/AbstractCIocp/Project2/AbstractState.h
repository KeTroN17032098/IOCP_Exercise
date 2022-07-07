#pragma once

class _STATE
{
public:
	virtual void Recv(void* session,char* data,int* datasize) abstract;
	virtual void Send(void* session, char* data, int* datasize) abstract;
};
