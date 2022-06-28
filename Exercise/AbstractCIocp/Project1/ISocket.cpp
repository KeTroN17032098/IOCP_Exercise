#include"ISocket.h"

ISocket::ISocket(SOCKET s)
{
	int size = sizeof(SOCKADDR_IN);
	this->s = s;
	getpeername(s, (SOCKADDR*)&addr, &size);	
	ex1 = init_exover(recvIO, this);
	ex2 = init_exover(sendIO, this);
	wsabuf = new WSABUF;
}

ISocket::~ISocket()
{
	PrintAddress();
	std::cout << "Socket Delete Progress" << std::endl;
	ZeroMemory(wsabuf, sizeof(WSABUF));
	delete wsabuf;
	clear_exover(ex1);
	clear_exover(ex2);
	closesocket(s);
	std::cout << "Socket Deleted" << std::endl;
}

void ISocket::Bind(int portno)
{
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(portno);
	int retval;
	retval = bind(s, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (retval == SOCKET_ERROR) err_quit("bind()");	
}

void ISocket::Listen()
{
	int retval;
	retval = listen(s, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}

SOCKET ISocket::Accept()
{
	int addrlen;
	addrlen = sizeof(SOCKADDR_IN);
	SOCKET client_sock = accept(s, (SOCKADDR*)&addr, &addrlen);	
	if (client_sock == INVALID_SOCKET)
	{
		err_display("accept()");
		return INVALID_SOCKET;
	}
	return client_sock;
}

bool ISocket::send(_BUFFER* b)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;

	ZeroMemory(&this->ex2->overlapped, sizeof(WSAOVERLAPPED));
	this->wsabuf->buf = b->getbuf();
	this->wsabuf->len = b->getleft();

	retval = WSASend(this->s, this->wsabuf, 1, &sendbytes, flags, &this->ex2->overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING) 
		{
			err_display("WSASend()");
			return false;
		}
	}
	return true;
}

bool ISocket::recv(_BUFFER* b)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;

	ZeroMemory(&this->ex1->overlapped, sizeof(WSAOVERLAPPED));
	this->wsabuf->buf = b->getbuf();
	this->wsabuf->len = b->getleft();

	retval = WSARecv(this->s, this->wsabuf, 1, &sendbytes, &flags, &this->ex1->overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			err_display("WSAREcv()");
			return false;
		}
	}
	return true;
}

bool ISocket::transPackit(_BUFFER* b,DWORD cbTransferred, IO_TYPE iotype)
{
	if (iotype == recvIO)
	{
		if (b->getcompleted() < sizeof(int))//size 다 못받아왔음
		{
			if (b->getcompleted() + cbTransferred >= sizeof(int))//이번에 다 받았다면
			{
				int size = 0;
				memcpy(&size, b->getresult(), sizeof(int));
				b->setleft(size);
				b->fcompleted(cbTransferred);
			}
			else
			{
				b->fcompleted(cbTransferred);
				b->setleft(0);
			}
		}
		else//본문 받아오기
		{
			b->fcompleted(cbTransferred);
		}
		if (b->is_completed() == true)//이번에 패킷을 다 받아왔다면
		{
			return true;
		}
	}
	else if (iotype == sendIO)
	{
		b->fcompleted(cbTransferred);
		if (b->is_completed() == true)//이번에 패킷을 다 보냈다면
		{
			return true;
		}
	}
	return false;
}
