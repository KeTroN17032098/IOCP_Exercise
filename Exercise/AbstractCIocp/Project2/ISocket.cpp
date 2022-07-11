#include"ISocket.h"

ISocket::ISocket(SOCKET s)
{
	int size = sizeof(SOCKADDR_IN);
	this->s = s;
	getpeername(s, (SOCKADDR*)&addr, &size);
	ZeroMemory(&ex1, sizeof(ExOverlapped));
	ZeroMemory(&ex2, sizeof(ExOverlapped));
	ex1.clientLP = this;
	ex1.type = IO_TYPE::recvIO;
	ex2.clientLP = this;
	ex2.type = IO_TYPE::sendIO;
}

ISocket::~ISocket()
{
	ZeroMemory(&ex1, sizeof(ExOverlapped));
	ZeroMemory(&ex2, sizeof(ExOverlapped));
	closesocket(s);
}

void ISocket::Bind(int portno)
{
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(portno);
	int retval;
	retval = bind(s, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (retval == SOCKET_ERROR) exit(-1);
}

void ISocket::Listen()
{
	int retval;
	retval = listen(s, SOMAXCONN);
	if (retval == SOCKET_ERROR) exit(-1);
}

SOCKET ISocket::Accept()
{
	int addrlen;
	addrlen = sizeof(SOCKADDR_IN);
	SOCKET client_sock = accept(s, (SOCKADDR*)&addr, &addrlen);
	if (client_sock == INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}
	return client_sock;
}

bool ISocket::send(char* buf, int size)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;
	WSABUF wsabuf;


	ZeroMemory(&this->ex2.overlapped, sizeof(WSAOVERLAPPED));
	ZeroMemory(&wsabuf, sizeof(WSABUF));

	wsabuf.buf = buf;
	wsabuf.len = size;

	retval = WSASend(this->s, &wsabuf, 1, &sendbytes, flags, &this->ex2.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}

bool ISocket::recv(char* buf, int size)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;
	WSABUF wsabuf;

	ZeroMemory(&this->ex1.overlapped, sizeof(WSAOVERLAPPED));
	ZeroMemory(&wsabuf, sizeof(WSABUF));
	wsabuf.buf = buf;
	wsabuf.len = size;

	retval = WSARecv(this->s, &wsabuf, 1, &sendbytes, &flags, &this->ex1.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
