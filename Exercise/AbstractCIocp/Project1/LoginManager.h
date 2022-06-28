#pragma once
#pragma comment(lib,"libmysql.lib")
#include"basicthing.h"
#include<map>
#include<vector>
#include<string>
#include<iostream>
#include<mysql.h>

#define HOST_IP "127.0.0.1"
#define USER "root"
#define PASSWORD  "1234"
#define DATABASE "simpleaccount"

enum LORS
{
	LGIN=76,
	SIGNIN
};

enum LOGIN_PROTOCOL
{
	LOGIN_MSG=5005,
	LOGIN_TRY,
	LOGIN_SUCCESS,
	LOGIN_ERROR
};

enum SIGNIN_PROTOCOL
{
	SIGNIN_MSG=436,
	SIGNIN_TRY,
	SIGNIN_SUCCESS,
	SIGNIN_ERROR
};

enum LGIN_SGIN_ERR
{
	MULTIPLEID =472,// ȸ������ ���̵��ߺ�
	LENGTHLIMIT,// �ִ� ���� �ʰ�
	SMALLCHAR,//����� ����
	ALREADYLGIN,//�̹� �α��� ��
	NOMATCH,//���̵� Ȥ�� ��й�ȣ ����
	UNKNOWNERR,//�˼����� ����
	NOERR//���� ����
};

class LoginManager
{
private:
	std::deque<LoginInfo*> join_member;
	std::deque<int> loggedin;
	std::map<LGIN_SGIN_ERR, std::string> err_msg;

	static LoginManager* instance;
	LoginManager();
	~LoginManager();
public:
	static LoginManager* getinstance()
	{
		if (instance == nullptr)instance = new LoginManager();
		return instance;
	}
	static void clearinstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	LGIN_SGIN_ERR trySignin(char* id, char* pw);//ȸ������ �õ�
	LGIN_SGIN_ERR tryLogin(char* id, char* pw);//�α��� �õ�

	void geterrmsg(char* msg,LGIN_SGIN_ERR e);//���� �޽��� ��ȯ

	void getJoinMember();//db���� �������� �Լ�
	int createMember(char* id, char* pw);//db�� ���̵� ��� ����

	int packPackit(char* Dest,LORS l,int p,int num,char* id,char *pw,char* msg,LGIN_SGIN_ERR e);
	void unpackPackit(char* Data,LORS* l, int* p,int* num,char* id,char* pw,char* msg,LGIN_SGIN_ERR* e);
};