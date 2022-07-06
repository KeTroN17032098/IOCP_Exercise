#include"LobbyManager.h"

LobbyManager* LobbyManager::instance = nullptr;

LobbyManager::LobbyManager() : _BASICMANAGER(LOBBY)
{
	addMsg(WELCOMEMSG, "환영합니다.\n로그인은 1번.\n회원가입은 2번.");
}

LobbyManager::~LobbyManager()
{
	ClearMap();
}

void LobbyManager::CreateInstance()
{
	if (instance == nullptr)instance = new LobbyManager();
}

void LobbyManager::ClearInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

LobbyManager* LobbyManager::GetInstance()
{
	return instance;
}

int LobbyManager::PackData(char* data, int p, int sel, char* msg)
{
	int size = 0;
	memcpy(data + size, &p, sizeof(int));
	size += sizeof(int);
	if (p ==SLECTION)
	{
		memcpy(data + size, &sel, sizeof(int));
		size += sizeof(int);
	}
	if (p == WELCOMEMSG)
	{
		int msgsize = strlen(msg);
		memcpy(data + size, &msgsize, sizeof(int));
		size += sizeof(int);
		memcpy(data + size, msg,msgsize);
		size += msgsize;
	}
	return size;
}

void LobbyManager::UnPackData(char* From, int* p, int* sel, char* msg)
{
	int size = 0;
	memcpy(p, From + size, sizeof(int));
	size += sizeof(int);
	if (*p == SLECTION)
	{
		memcpy(sel, From + size, sizeof(int));
		size += sizeof(int);
	}
	if (*p == WELCOMEMSG)
	{
		int msgsize = 0;
		memcpy(&msgsize,From+size,sizeof(int));
		size += sizeof(int);
		memcpy(msg,From+size,msgsize);
		size += msgsize;
	}
}

void LobbyManager::insideProcess(ISession* is, int* managerNo, char* data, int* datasize)
{
	int a, b;
	UnPackData(data, &a, &b, nullptr);
	LogManager::LogPrint("LOBBYManager Data\nLBProtocol  - %d", a);
	if (a == SLECTION)
	{
		*managerNo = getNo();
		memcpy(data, &b, sizeof(int));
		LogManager::LogPrint("Process move to LoginManager");
		LoginManager::GetInstance()->outsideProcess(is,managerNo, data, datasize);
		memcpy(&b,data, sizeof(int));
		LogManager::LogPrint("JTC : %d",b);

	}
}

void LobbyManager::outsideProcess(ISession* is, int* managerNo, char* data, int* datasize)
{
	if (*managerNo == START)
	{
		LogManager::LogPrint("LOBBYManager Data\nTransferred From  - %d",*managerNo);

		public_key_class tmp;
		memcpy(&tmp, data, sizeof(public_key_class));
		is->setPublicKey(tmp.modulus, tmp.exponent);

		char msg[100] = "";
		getMsg(msg, WELCOMEMSG);
		int p = PackData(data, WELCOMEMSG, -1, msg);
		memcpy(data + p, Crypt::GetInstance()->getPublicKey(), sizeof(public_key_class));
		memcpy(msg, data + 8, 42);
		public_key_class jk;
		memcpy(&jk, data + p, sizeof(public_key_class));
		LogManager::LogPrint("Send MSG : %s", msg);
		LogManager::LogPrint("Send Key : %lld %lld", jk.exponent, jk.modulus);
		*datasize = p+sizeof(public_key_class);
		*managerNo = getNo();
	}
}
