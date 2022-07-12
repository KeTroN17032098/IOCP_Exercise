#pragma once
#include"ISocket.h"
#include"ProtocolManager.h"
#include"CCS.h"

class IPacket : public ISocket
{
	std::deque<_BUFFER*> sendbuf;
	_BUFFER* recvbuf;
	int recvpno;
	int sendpno;
	CCS ccs;
public:
	IPacket(SOCKET s);
	virtual ~IPacket();

	bool recvprogress(DWORD cbTransferred);
	bool sendprogress(DWORD cbTransferred);
	bool tryrecv();
	bool trysend();

	int Pack(int status,int datasize,char* Data);
	void UnPack(int* status,int* recvpno,int* datasize,char* Data);

	bool checkpno(int a)
	{
		if (a < recvpno)
		{
			return false;
		}
		recvpno = a;
		return true;
	}

	bool is_sendleft()
	{
		return (sendbuf.size() > 0);
	}

	void clearRecvbuf()
	{
		recvbuf->recvclear();
	}
};