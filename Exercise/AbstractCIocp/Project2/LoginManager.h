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
	MULTIPLEID = 472,// 회원가입 아이디중복
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

	int trySignin(char* id, char* pw);//회원가입 시도
	int tryLogin(char* id, char* pw);//로그인 시도
	void geterrmsg(char* msg, int e);//에러 메시지 반환

	void packetprocess(char* data, int* datasize);//위 함수 들로 결과물만 나오도록

	int packPackit(char* Dest,int l, int p, int num, char* id, char* pw, char* msg, int e);
	void unpackPackit(char* Data, int* l, int* p, int* num, char* id, char* pw, char* msg, int* e);
};