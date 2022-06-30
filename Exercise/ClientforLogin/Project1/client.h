#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SERVERIP   "127.0.0.1"//������ �ּ�
#define SERVERPORT 9000//���� ��Ʈ ����
#define BUFSIZE    1024//���� ������
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
	MULTIPLEID = -472,// ȸ������ ���̵��ߺ�
	LENGTHLIMIT,// �ִ� ���� �ʰ�
	SMALLCHAR,//����� ����
	ALREADYLGIN,//�̹� �α��� ��
	NOMATCH,//���̵� Ȥ�� ��й�ȣ ����
	UNKNOWNERR,//�˼����� ����
	NOERR//���� ����
};
enum LOBBY_PROTOCOL
{
	WELCOMEMSG = 145,
	SLECTION
};

// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg);
// ���� �Լ� ���� ���
void err_display(char* msg);

// ���� �Լ� ���� ���
void err_display(int errcode);

int addDataatEnd(char* dst, void* data, int size);


int addDataatEndwithoutSize(char* dst, void* data, int size);


int getOnlyData(char* from, void* dst, int size);


int getDatawithSize(char* from, void* dst, int* size);

int packPackitLOBBY(char* Dst, LOBBY_PROTOCOL p, int sel, char* msg);
void unpackpackitLOBBY(char* data, LOBBY_PROTOCOL* p, int* sel, char* msg);
int packPackitB(char* Dest,STATUS s,int pno, int psize, char* data);//��ŷ�� �����Ϳ� ����� �ٿ��� �۽� ť�� �ѱ�� �Լ�
void unpackPackitB(char* From, int packitsize, STATUS* s, int* pno, int* psize, char* data);//����� �����ؼ� ��ŷ�� �����͸� ����
int packPackit(char* Dest, LORS l, int p, int num, char* id, char* pw, char* msg, LGIN_SGIN_ERR e);
void unpackPackit(char* Data, LORS* l, int* p, int* num, char* id, char* pw, char* msg, LGIN_SGIN_ERR* e);
int recvn(SOCKET s, char* buf, int len, int flags);

void logPrint(bool logable, char* msg);

void PackitCheck(char* packit, int size);