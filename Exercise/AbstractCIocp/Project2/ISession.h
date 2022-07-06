#pragma once
#include"IPacket.h"
#include"rsa.h"
#include"LogManager.h"

class ISession : public IPacket
{
private:
	STATUS stat;
	int uuid;
	public_key_class clientkey;
public:
	ISession(SOCKET s) :IPacket(s)
	{
		stat = START;
		uuid = -1;
	}

	~ISession() {}

	void changeStat(STATUS s) { stat = s; }
	void setUUID(int a) { uuid = a; }
	STATUS getStat() { return stat; }
	int getUUID() { return uuid; }

	void setPublicKey(long long m, long long e)
	{
		clientkey.exponent = e;
		clientkey.modulus = m;
		LogManager::LogPrint("Client Key : %lld %lld", e, m);
	}

	public_key_class* getPublicKey() { return &clientkey; }

	bool is_me_u(int  uuid) { return (uuid == this->uuid); }
};