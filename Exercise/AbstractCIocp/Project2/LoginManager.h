#pragma once
#include"BasicManager.h"
#include"DBManager.h"
#include<map>
#include<vector>
#include<string>

class LoginManager : public _BASICMANAGER
{
private:
	enum LORS
	{
		LGIN = 76,
		SIGNIN,
	};

	enum LOGIN_PROTOCOL
	{
		LOGIN_MSG = 5005,
		LOGIN_TRY,
		LOGIN_SUCCESS,
		LOGIN_ERROR
	};

	enum SIGNIN_PROTOCOL
	{
		SIGNIN_MSG = 436,
		SIGNIN_TRY,
		SIGNIN_SUCCESS,
		SIGNIN_ERROR
	};

	enum LGIN_SGIN_ERR
	{
		MULTIPLEID = -472,// ȸ������ ���̵��ߺ�
		LENGTHLIMIT,// �ִ� ���� �ʰ�
		SMALLCHAR,//����� ����
		ALREADYLGIN,//�̹� �α��� ��
		NOMATCH,//���̵� Ȥ�� ��й�ȣ ����
		UNKNOWNERR,//�˼����� ����
		NOERR//���� ����
	};

	static LoginManager* instance;

	std::deque<LoginInfo*> join_member;
	std::deque<int> loggedin;


	LoginManager();
	virtual ~LoginManager();

public:
	static void CreateInstance();
	static LoginManager* GetInstance();
	static void ClearInstance();

	void getJoinm();

	int trySignin(char* id, char* pw);//ȸ������ �õ�
	int tryLogin(char* id, char* pw);//�α��� �õ�
	
	void LogOut(int uuid);

	int packPackit(char* Dest,int l, int p, int num, char* id, char* pw, char* msg, int e);
	void unpackPackit(char* Data, int* l, int* p, int* num, char* id, char* pw, char* msg, int* e);

	// _BASICMANAGER��(��) ���� ��ӵ�
	virtual void insideProcess(ISession* is, int* managerNo, char* data, int* datasize) override;
	virtual void outsideProcess(ISession* is, int* managerNo, char* data, int* datasize) override;
};