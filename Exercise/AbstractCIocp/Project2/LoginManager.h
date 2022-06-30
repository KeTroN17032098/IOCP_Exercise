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
		MULTIPLEID = -472,// 회원가입 아이디중복
		LENGTHLIMIT,// 최대 길이 초과
		SMALLCHAR,//공백란 있음
		ALREADYLGIN,//이미 로그인 됨
		NOMATCH,//아이디 혹은 비밀번호 오류
		UNKNOWNERR,//알수없는 오류
		NOERR//오류 없음
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

	int trySignin(char* id, char* pw);//회원가입 시도
	int tryLogin(char* id, char* pw);//로그인 시도
	
	void LogOut(int uuid);

	int packPackit(char* Dest,int l, int p, int num, char* id, char* pw, char* msg, int e);
	void unpackPackit(char* Data, int* l, int* p, int* num, char* id, char* pw, char* msg, int* e);

	// _BASICMANAGER을(를) 통해 상속됨
	virtual void insideProcess(ISession* is, int* managerNo, char* data, int* datasize) override;
	virtual void outsideProcess(ISession* is, int* managerNo, char* data, int* datasize) override;
};