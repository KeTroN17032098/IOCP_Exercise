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
	MULTIPLEID =472,// 회원가입 아이디중복
	LENGTHLIMIT,// 최대 길이 초과
	SMALLCHAR,//공백란 있음
	ALREADYLGIN,//이미 로그인 됨
	NOMATCH,//아이디 혹은 비밀번호 오류
	UNKNOWNERR,//알수없는 오류
	NOERR//오류 없음
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

	LGIN_SGIN_ERR trySignin(char* id, char* pw);//회원가입 시도
	LGIN_SGIN_ERR tryLogin(char* id, char* pw);//로그인 시도

	void geterrmsg(char* msg,LGIN_SGIN_ERR e);//에러 메시지 반환

	void getJoinMember();//db에서 가져오는 함수
	int createMember(char* id, char* pw);//db에 아이디 비번 전송

	int packPackit(char* Dest,LORS l,int p,int num,char* id,char *pw,char* msg,LGIN_SGIN_ERR e);
	void unpackPackit(char* Data,LORS* l, int* p,int* num,char* id,char* pw,char* msg,LGIN_SGIN_ERR* e);
};