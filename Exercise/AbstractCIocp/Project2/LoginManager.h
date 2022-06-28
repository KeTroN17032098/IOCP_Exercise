#pragma once
#include"DBManager.h"
#include<map>
#include<vector>
#include<string>
enum LORS
{
	LGIN = 76,
	SIGNIN
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
	MULTIPLEID = 472,// ȸ������ ���̵��ߺ�
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
	static LoginManager* instance;

	std::deque<LoginInfo*> join_member;
	std::deque<int> loggedin;
	std::map<LGIN_SGIN_ERR, std::string> err_msg;


	LoginManager();
	~LoginManager();

public:
	static void CreateInstance();
	static LoginManager* GetInstance();
	static void ClearInstance();

	int trySignin(char* id, char* pw);//ȸ������ �õ�
	int tryLogin(char* id, char* pw);//�α��� �õ�
	void geterrmsg(char* msg, int e);//���� �޽��� ��ȯ

	void packetprocess(char* data, int* datasize);//�� �Լ� ��� ������� ��������

	int packPackit(char* Dest,int l, int p, int num, char* id, char* pw, char* msg, int e);
	void unpackPackit(char* Data, int* l, int* p, int* num, char* id, char* pw, char* msg, int* e);
};