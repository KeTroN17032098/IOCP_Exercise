#include"VariousStates.h"

void LoginState::Recv(void* session)
{
	int mno = 0,pn=0;
	ISession* ptr = (ISession*)session;
	char data1[8192]="",data[8192]="";
	int datasize = 0;
	ptr->UnPack(&mno, &pn, &datasize, data);
	if (ptr->checkpno(pn) == false)return;
	if (ptr->getPublicKey()->exponent != 0)
	{
		//클라에서 Encrypt Key를 아직 안 받아옴 = 아직 서버의 공개키를 전달하지 않음 = 아직 암호화 안됨
		LoginManager::GetInstance()->Process(ptr, &mno,data,&datasize);
		memcpy(data1, data, datasize);
		Crypt::GetInstance()->Encrypt(data1,data,&datasize,ptr->getPublicKey());
		ptr->Pack(mno, datasize, data1);
	}
	else
	{
		Crypt::GetInstance()->Decrypt(data, data1, &datasize);
		LoginManager::GetInstance()->Process(ptr, &mno, data1, &datasize);
		Crypt::GetInstance()->Encrypt(data1, data, &datasize, ptr->getPublicKey());
		ptr->Pack(mno, datasize, data);
		if (mno != (int)STATUS::LOGIN)a = true;
	}
	ptr->trysend();
}

void LoginState::Send(void* session)
{
	ISession* ptr = (ISession*)session;
	if (a == true)
	{
		ptr->setState(STATUS::LOBBY);
	}
	ptr->tryrecv();
}

void LobbyState::Recv(void* session)
{
	int mno = 0, pn = 0;
	ISession* ptr = (ISession*)session;
	char data1[8192] = "", data[8192] = "";
	int datasize = 0;
	ptr->UnPack(&mno, &pn, &datasize, data);
	if (ptr->checkpno(pn) == false)return;
	Crypt::GetInstance()->Decrypt(data, data1, &datasize);
	LobbyManager::GetInstance()->Process(ptr, &mno, data1, &datasize);
	Crypt::GetInstance()->Encrypt(data1, data, &datasize, ptr->getPublicKey());
	ptr->Pack(mno, datasize, data);
	ptr->trysend();
}

void LobbyState::Send(void* session)
{
	ISession* ptr = (ISession*)session;
	ptr->tryrecv();
}
