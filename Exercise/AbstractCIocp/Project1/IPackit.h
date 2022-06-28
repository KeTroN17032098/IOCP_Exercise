#pragma once
#include"ISocket.h"
//패킹 및 데이터 처리만
enum LOBBY_PROTOCOL
{
	WELCOMEMSG = 145,
	SLECTION
};

class IPackit :public ISocket
{
private:
	std::deque<_BUFFER*> sendbuf;
	_BUFFER* recvbuf;

	int pno;
public:
	IPackit(SOCKET s);
	virtual ~IPackit();

	int packPackitB(STATUS s,int psize, char* data);//패킹된 데이터에 헤더를 붙여서 송신 큐에 넘기는 함수
	void unpackPackitB(STATUS* s,int* pno,int* psize,char* data);//헤더를 참조해서 패킹된 데이터를 언팩

	void clearRecvbuf()
	{
		delete recvbuf;
		recvbuf = nullptr;
		recvbuf = new _BUFFER(recvIO, 1024, 0);
	}

	bool checkpno(int a)
	{
		if (a < pno)
		{
			return false;
		}
		pno = a;
		return true;
	}

	bool recvprogress(DWORD cbTransferred);
	bool sendprogress(DWORD cbTransferred);
	bool is_sendleft()
	{
		if (sendbuf.size() > 0)return true;
		else return false;
	}
	bool startrecv();
	bool startsend();

	int packPackitLOBBY(char* Dst, LOBBY_PROTOCOL p, int sel, char* msg);
	void unpackpackitLOBBY(char* data, LOBBY_PROTOCOL* p, int* sel, char* msg);
};
