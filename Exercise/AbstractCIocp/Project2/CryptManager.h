#pragma once

#include"rsa.h"
#include"LogManager.h"

class Crypt
{
private:
	static Crypt* instance;

	public_key_class pub;
	private_key_class priv;

	Crypt();
	~Crypt();
public:
	static void CreateInstance();
	static void ClearInstance();
	static Crypt* GetInstance();

	public_key_class* getPublicKey() { return &pub; }

	void Encrypt(char* data, char* encrypted, int* size);//자신의 공개키로 암호화
	void Encrypt(char* data, char* encrypted, int* size,public_key_class* key);// 입력된 공개키로 암호화
	void Decrypt(char* Encrypted, char* data, int* size);//자신의 개인키로 복호화
};