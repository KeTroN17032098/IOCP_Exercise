#include"VariousStates.h"

void LoginState::Recv(void* session, char* data, int* datasize)
{
	int mno = 0;
	ISession* ptr = (ISession*)session;
	char data1[8192]="";
	if (ptr->getPublicKey()->exponent != 0)
	{
		//클라에서 Encrypt Key를 아직 안 받아옴 = 아직 서버의 공개키를 전달하지 않음 = 아직 암호화 안됨
		LoginManager::GetInstance()->Process(ptr, &mno, data, datasize);
		memcpy(data1, data, *datasize);
		Crypt::GetInstance()->Encrypt(data1,data,datasize,ptr->getPublicKey());
	}
	else
	{

	}
}

void LoginState::Send(void* session, char* data, int* datasize)
{
}

void LobbyState::Recv(void* session, char* data, int* datasize)
{
}

void LobbyState::Send(void* session, char* data, int* datasize)
{
}
