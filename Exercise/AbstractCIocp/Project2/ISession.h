#pragma once
#include"IPacket.h"

class ISession : public IPacket
{
private:
	STATUS stat;
	int uuid;
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

	bool is_me_u(int  uuid) { return (uuid == this->uuid); }
};