#pragma once
#include"IPacket.h"
#include"rsa.h"
#include"LogManager.h"
#include"AbstractState.h"
#include<map>


class ISession : public IPacket
{
private:
	int uuid;
	public_key_class clientkey;
	_STATE* current_state;
	std::map<int, _STATE*> statelist;
public:
	ISession(SOCKET s) :IPacket(s)
	{
		uuid = -1;
		ZeroMemory(&clientkey, sizeof(public_key_class));
		statelist.clear();
	}

	~ISession() 
	{
		std::map<int, _STATE*>::iterator iter;
		for (iter = statelist.begin(); iter != statelist.end(); iter++)
		{
			_STATE* tmp = (*iter).second;
			delete tmp;
		}
		statelist.clear();
	}

	void setUUID(int a) { uuid = a; }
	int getUUID() { return uuid; }

	void addState(int status, _STATE* state) 
	{
		statelist.insert(std::make_pair(status, state));
	}

	_STATE* getState() { return current_state; }

	void setState(STATUS s) { current_state = statelist.find((int)s)->second; }

	void setPublicKey(long long m, long long e)
	{
		clientkey.exponent = e;
		clientkey.modulus = m;
		LogManager::LogPrint("Client Key : %lld %lld", e, m);
	}

	public_key_class* getPublicKey() { return &clientkey; }

	bool is_me_u(int  uuid) { return (uuid == this->uuid); }
};