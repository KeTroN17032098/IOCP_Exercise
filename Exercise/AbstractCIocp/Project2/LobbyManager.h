#pragma once

#include"BasicManager.h"
#include"ProtocolManager.h"
#include"CryptManager.h"



class LobbyManager :public _BASICMANAGER
{
private:
	static LobbyManager* instance;
	LobbyManager();
	~LobbyManager();

	enum class LOBBY_PROTOCOL
	{
		REQ_LOBBYDATA=29,
		LOBBYDATA
	};
	enum class LOBBYDATA_PROTOCOL
	{
		PROFILE=54,

	};
	enum class LOBBYMANAGER_DETAIL
	{
		MSG=1,
	};

public:
	static void CreateInstance();
	static void ClearInstance();
	static LobbyManager* GetInstance();

	int PackData(char* data,int m,int s, char* msg);
	void UnPackData(char* From, int* m, int* s, char* msg);

	// _BASICMANAGER을(를) 통해 상속됨
	virtual void Process(ISession* is, int* managerNo, char* data, int* datasize) override;
};