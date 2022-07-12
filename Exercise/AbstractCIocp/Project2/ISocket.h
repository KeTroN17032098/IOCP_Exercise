#pragma once
#include"basicthing.h"


class ISocket
{
private:
	SOCKET s;
	SOCKADDR_IN addr;
	ExOverlapped ex1;
	ExOverlapped ex2;

public:
	ISocket(SOCKET s);
	virtual ~ISocket();

	void Bind(int portno);
	void Listen();
	SOCKET Accept();

	bool send(char* buf,int size);
	bool recv(char* buf, int size);

	void PrintAddress()
	{
		std::cout << "This Socket's Address is " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;
	}

	void getoverlapped(DWORD* d1, DWORD* d2)
	{
		WSAGetOverlappedResult(s, &ex1.overlapped, d1, FALSE, d2);
	}

	bool is_me(SOCKET socketnum) { return (socketnum == s); }
};