#include"IPacket.h"

IPacket::IPacket(SOCKET s) : ISocket(s)
{
	sendbuf.clear();
	recvbuf = new _BUFFER(recvIO, 1024, 0);
	recvpno = 1;
	sendpno = 1;
}

IPacket::~IPacket()
{
	delete recvbuf;
	std::deque<_BUFFER*>::iterator iter;
	for (iter = sendbuf.begin(); iter != sendbuf.end(); iter++)
	{
		_BUFFER* tmp = *iter;
		delete tmp;
	}
	sendbuf.clear();
}

bool IPacket::recvprogress(DWORD cbTransferred)
{
	_BUFFER* b = recvbuf;
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
	return false;
}

bool IPacket::sendprogress(DWORD cbTransferred)
{
	if (sendbuf.size() < 1)return false;
	_BUFFER* b = sendbuf.front();
	b->fcompleted(cbTransferred);
	if (b->is_completed() == true)//이번에 패킷을 다 보냈다면
	{
		sendbuf.pop_front();
		delete b;
		return true;
	}
	return false;
}

bool IPacket::tryrecv()
{
	if (recvbuf->getcompleted() > 0)return true;
	return this->recv(recvbuf->getbuf(), recvbuf->getleft());
}

bool IPacket::trysend()
{
	if (sendbuf.size() != 1)
	{
		return true;
	}
	else
	{
		return this->send(sendbuf.front()->getbuf(), sendbuf.front()->getleft());
	}
}

int IPacket::Pack(int status, int datasize, char* Data)
{
	_BUFFER* tmp = new _BUFFER(sendIO, datasize + 12, datasize);
	int size = sizeof(int);
	memcpy(tmp->getresult() + size, &status, sizeof(int));
	size += sizeof(int);
	memcpy(tmp->getresult() + size, &sendpno, sizeof(int));
	size += sizeof(int);
	memcpy(tmp->getresult() + size, Data, datasize);
	size += datasize;
	memcpy(tmp->getresult(), &size, sizeof(int));
	sendpno++;
	sendbuf.push_back(tmp);
	return size;
}

void IPacket::UnPack(int* status, int* recvpno, int* datasize, char* Data)
{
	int packitsize = recvbuf->getcompleted();
	int size = sizeof(int);
	memcpy(status, recvbuf->getresult() + size, sizeof(int));
	size += sizeof(STATUS);
	memcpy(recvpno, recvbuf->getresult() + size, sizeof(int));
	size += sizeof(int);
	memcpy(Data, recvbuf->getresult() + size, packitsize - size);
	*datasize = packitsize - size;
}
