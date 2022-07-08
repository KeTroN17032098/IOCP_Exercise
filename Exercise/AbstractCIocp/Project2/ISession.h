#pragma once
#include"IPacket.h"
#include"rsa.h"
#include"LogManager.h"
#include"VariousStates.h"


class ISession : public IPacket
{
private:
	int uuid;
	public_key_class clientkey;
	_STATE* current_state;
	LoginState loginstate;
	LobbyState lobbystate;
public:
	ISession(SOCKET s) :IPacket(s)
	{
		uuid = -1;
		ZeroMemory(&clientkey, sizeof(public_key_class));
	}

	~ISession() {}

	void setUUID(int a) { uuid = a; }
	int getUUID() { return uuid; }

	_STATE* getState() { return current_state; }
	void setState(_STATE* s) { current_state = s; }

	_STATE* getLoginState() { return &loginstate; }
	_STATE* getLobbyState() { return &lobbystate; }

	void setPublicKey(long long m, long long e)
	{
		clientkey.exponent = e;
		clientkey.modulus = m;
		LogManager::LogPrint("Client Key : %lld %lld", e, m);
	}

	public_key_class* getPublicKey() { return &clientkey; }

	bool is_me_u(int  uuid) { return (uuid == this->uuid); }
};