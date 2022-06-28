#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<deque>
#include <winsock2.h>
#include <ws2tcpip.h>
 //전역함수 없이, 가상IOCP는 쓰레드함수를 가져서 this를 넣어서 구동되도록
enum STATUS
{
	START=1,
	LOBBY,
	LGSI,
	DISCONNECTED
};


enum IO_TYPE
{
	recvIO = 1,
	sendIO,
	acceptIO
};

class _BUFFER
{
private:
	char* buf;
	int left;
	int completed;
	int buftsize;
public:
	_BUFFER(int size);
	_BUFFER(IO_TYPE iotype, int size, int psize);
	char* getbuf();
	void setleft(int a);
	void fcompleted(int a);
	bool is_completed();
	char* getresult();
	int getleft();
	int getcompleted();
	~_BUFFER();

};

class _VIRTUALIOCP // 가상 IOCP
{
public:
	virtual void accept(SOCKET clientsock,HANDLE hcp) abstract;
	virtual void recv(void* session,DWORD cbTransferred) abstract;
	virtual void send(void* session, DWORD cbTransferred) abstract;
	virtual void disconnect(int retval,void* session) abstract;
	virtual void init(int portno) abstract;
	virtual void end() abstract;
	virtual void run() abstract;
};

struct LoginInfo
{
	char ID[20];
	char PW[20];
	int uuid;
};

LoginInfo* init_li();

void DeleteLI(LoginInfo* l);

struct ExOverlapped
{
	OVERLAPPED overlapped;
	IO_TYPE type;
	void* clientLP;
};
ExOverlapped* init_exover(IO_TYPE type, void* clientLp);
void clear_exover(ExOverlapped* eo);
void GiveChar(char* Dst, char* fmt, ...);

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg);
// 소켓 함수 오류 출력
void err_display(char* msg);

// 소켓 함수 오류 출력
void err_display(int errcode);


int addDataatEnd(char* dst, void* data, int size);


int addDataatEndwithoutSize(char* dst, void* data,int size);


int getOnlyData(char* from, void* dst,int size);


int getDatawithSize(char* from, void* dst,int* size);

void PackitCheck(char* packit, int size);