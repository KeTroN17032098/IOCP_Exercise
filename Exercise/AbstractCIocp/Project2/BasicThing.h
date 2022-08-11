#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<deque>
#include <winsock2.h>
#include <ws2tcpip.h>
#include"MemPool.h"

enum class STATUS
{
	LOGIN=1,
	LOBBY,
	DISCONNECT
};
enum class ENCRYPTED
{
	Y=1,
	N
};

typedef wchar_t sentence;

enum class IO_TYPE
{
	recvIO=1,
	sendIO,
	acceptIO,

};

class _BUFFER : public MemPool<_BUFFER,256>
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
	void recvclear();
	~_BUFFER();
};

struct LoginInfo
{
	sentence ID[20];
	sentence PW[20];
	int uuid;
};

struct ExOverlapped
{
	OVERLAPPED overlapped;
	IO_TYPE type;
	void* clientLP;
};

