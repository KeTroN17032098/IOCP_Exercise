#pragma once
#include"AbstractState.h"
#include"LoginManager.h"
#include"LobbyManager.h"

class LoginState : public _STATE
{
private:
	bool a;
public:
	// _STATE을(를) 통해 상속됨
	virtual void Recv(void* session) override;
	virtual void Send(void* session) override;
};

class LobbyState : public _STATE 
{
public:
	// _STATE을(를) 통해 상속됨
	virtual void Recv(void* session) override;

	virtual void Send(void* session) override;

};