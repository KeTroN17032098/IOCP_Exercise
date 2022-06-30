#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SERVERIP   "127.0.0.1"//루프백 주소
#define SERVERPORT 9000//서버 포트 지정
#define BUFSIZE    1024//버퍼 사이즈
#define loggable 1

enum STATUS
{
	START = 1,
	LOBBY,
	LGSI,
	DISCONNECTED
};
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
enum LOBBY_PROTOCOL
{
	WELCOMEMSG = 145,
	SLECTION
};

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg);
// 소켓 함수 오류 출력
void err_display(char* msg);

// 소켓 함수 오류 출력
void err_display(int errcode);

int addDataatEnd(char* dst, void* data, int size);


int addDataatEndwithoutSize(char* dst, void* data, int size);


int getOnlyData(char* from, void* dst, int size);


int getDatawithSize(char* from, void* dst, int* size);

int packPackitLOBBY(char* Dst, LOBBY_PROTOCOL p, int sel, char* msg);
void unpackpackitLOBBY(char* data, LOBBY_PROTOCOL* p, int* sel, char* msg);
int packPackitB(char* Dest,STATUS s,int pno, int psize, char* data);//패킹된 데이터에 헤더를 붙여서 송신 큐에 넘기는 함수
void unpackPackitB(char* From, int packitsize, STATUS* s, int* pno, int* psize, char* data);//헤더를 참조해서 패킹된 데이터를 언팩
int packPackit(char* Dest, LORS l, int p, int num, char* id, char* pw, char* msg, LGIN_SGIN_ERR e);
void unpackPackit(char* Data, LORS* l, int* p, int* num, char* id, char* pw, char* msg, LGIN_SGIN_ERR* e);
int recvn(SOCKET s, char* buf, int len, int flags);

void logPrint(bool logable, char* msg);

void PackitCheck(char* packit, int size);