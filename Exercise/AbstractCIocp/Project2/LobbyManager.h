#pragma once

#include"BasicManager.h"
#include"LoginManager.h"
#include"CryptManager.h"



class LobbyManager :public _BASICMANAGER
{
private:
	static LobbyManager* instance;
	LobbyManager();
	~LobbyManager();

	enum LOBBY_PROTOCOL
	{
		WELCOMEMSG = 145,
		SLECTION
	};

public:
	static void CreateInstance();
	static void ClearInstance();
	static LobbyManager* GetInstance();

	int PackData(char* data,int p, int sel, char* msg);
	void UnPackData(char* From,int* p, int* sel, char* msg);

	// _BASICMANAGER을(를) 통해 상속됨
	virtual void insideProcess(ISession* is, int* managerNo, char* data, int* datasize) override;
	virtual void outsideProcess(ISession* is, int* managerNo, char* data, int* datasize) override;
};