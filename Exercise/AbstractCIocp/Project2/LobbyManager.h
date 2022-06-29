#pragma once

#include"BasicManager.h"
#include"LoginManager.h"



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

	// _BASICMANAGER��(��) ���� ��ӵ�
	virtual void insideProcess(int* managerNo, char* data, int* datasize) override;
	virtual void outsideProcess(int* managerNo, char* data, int* datasize) override;
};