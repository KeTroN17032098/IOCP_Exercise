#pragma once

class _STATE
{
public:
	virtual void Recv(void* session) abstract;
	virtual void Send(void* session) abstract;
};
