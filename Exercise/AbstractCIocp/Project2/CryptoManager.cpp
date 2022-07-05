#include"CryptoManager.h"

CryptoManager* CryptoManager::instance = nullptr;

void CryptoManager::Encrypt(char* data, int indata, char* result, int* outdata)
{
	DWORD i;
	int key = k3;
	if (!data || indata <= 0 || !result)
	{
		return;
	}
	*outdata = indata;
	for (i = 0; i < indata; i++)
	{
		result[i] = data[i] ^ key >> 8;
		key = (result[i] + key) * k1 + k2;
	}
}

CryptoManager::CryptoManager()
{
	k1 = std::rand()%90000+10000;
	k2 = std::rand() % 90000 + 10000;
	k3 = std::rand() % 90000 + 10000;
	LogManager::LogPrint("CryptoManager Created\nk1 : %d\nk2 : %d\nk3 : %d\n", k1, k2, k3);
}

CryptoManager::~CryptoManager()
{
	LogManager::LogPrint("CryptoManager Deleted");
}

void CryptoManager::Decrypt(char* data, int indata, char* result, int* outdata)
{
	DWORD i;
	char prev;
	int key = k3;
	if (!data || indata <= 0 || !result)
	{
		return;
	}
	*outdata = indata;
	for (i = 0; i < indata; i++)
	{
		prev = data[i];
		result[i] = data[i] ^ key >> 8;	
		key = (prev + key) * k1 + k2;
	}
}

void CryptoManager::CreateInstance()
{
	if (instance == nullptr)
	{
		instance = new CryptoManager();
	}
}

void CryptoManager::ClearInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

CryptoManager* CryptoManager::GetInstance()
{
	return instance;
}
