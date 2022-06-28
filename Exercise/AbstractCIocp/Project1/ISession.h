#pragma once
#include"IPackit.h"

class ISession : public IPackit
{
private:
	STATUS stat;
	int uuid;
public:
	ISession(SOCKET s):IPackit(s)
	{
		stat = START;
		uuid = -1;
	}

	~ISession(){}

	void changeStat(STATUS s) { stat = s; }
	void setUUID(int a) { uuid = a; }
	STATUS getStat() { return stat; }

	bool is_me_u(int  uuid) { return (uuid == this->uuid); }
};