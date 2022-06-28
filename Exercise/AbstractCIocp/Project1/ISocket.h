#pragma once
#include"basicthing.h"
//단순한 수신/송신만 담당 하도록 
class ISocket
{
private:
	SOCKET s;
	SOCKADDR_IN addr;
	ExOverlapped* ex1;
	ExOverlapped* ex2;
	LPWSABUF wsabuf;
public:
	ISocket(SOCKET s);
	virtual ~ISocket();

	void Bind(int portno);
	void Listen();
	SOCKET Accept();

	bool send(_BUFFER* b);
	bool recv(_BUFFER* b);

	bool transPackit(_BUFFER* b, DWORD cbTransferred, IO_TYPE iotype);

	void PrintAddress()
	{
		std::cout << "This Socket's Address is " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;
	}

	void getoverlapped(DWORD* d1, DWORD* d2)
	{
		WSAGetOverlappedResult(s, &ex1->overlapped, d1, FALSE, d2);
	}

	bool is_me(SOCKET socketnum) { return (socketnum == s); }
};