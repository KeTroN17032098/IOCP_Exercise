#include"CryptManager.h"

Crypt* Crypt::instance = nullptr;

Crypt::Crypt()
{
	rsa_gen_keys(&pub, &priv);
	LogManager::LogPrint("RSA KEY Generated %lld,%lld || %lld,%lld",pub.exponent,pub.modulus,priv.exponent,priv.modulus);
}

Crypt::~Crypt()
{
	LogManager::LogPrint("Crypt Manager Deleted");
}

void Crypt::CreateInstance()
{
	if (instance == nullptr)instance = new Crypt();
}

void Crypt::ClearInstance()
{
	if (instance!= nullptr)delete instance;
}

Crypt* Crypt::GetInstance()
{
	return instance;
}

void Crypt::Encrypt(char* data, char* encrypted, int* size)
{
	encrypted = (char*)rsa_encrypt(data, *size, &pub);
	*size = 8 * (*size);
}

void Crypt::Encrypt(char* data, char* encrypted, int* size, public_key_class* key)
{
	long long* tmp=rsa_encrypt(data, *size, key);
	LogManager::LogPrint("Encrypted");
	*size = 8 * (*size);
	LogManager::LogPrint("Encrypt Size : %d", *size);
	memcpy(encrypted, "Das", 3);
	LogManager::LogPrint("Encrypt Data Copied");
}

void Crypt::Decrypt(char* Encrypted, char* data, int* size)
{
	char* buf = rsa_decrypt((long long*)Encrypted, *size, &priv);
	memcpy(data, buf, (*size) / 8);
	*size = (*size) / 8;
}
