#pragma once

#include"BasicThing.h"
#include"LogManager.h"

class CryptoManager
{
private:
	static CryptoManager* instance;
	int	k1, k2, k3;


	CryptoManager();
	~CryptoManager();
public:
	void Encrypt(char* data, int indata, char* result, int* outdata);
	void Decrypt(char* data, int indata, char* result, int* outdata);
	
	static void CreateInstance();
	static void ClearInstance();
	static CryptoManager* GetInstance();
};