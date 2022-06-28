#include"IPackit.h"

IPackit::IPackit(SOCKET s) : ISocket(s)
{
	sendbuf.clear();
	recvbuf = new _BUFFER(recvIO, 1024, 0);
	pno = 1;
}

IPackit::~IPackit()
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

int IPackit::packPackitB(STATUS s, int psize, char* data)
{
	_BUFFER* tmp = new _BUFFER(sendIO, 1024, psize);
	int size = sizeof(int);
	memcpy(tmp->getresult() + size, &s, sizeof(STATUS));
	size += sizeof(STATUS);
	memcpy(tmp->getresult() + size, &this->pno, sizeof(int));
	size += sizeof(int);
	memcpy(tmp->getresult() + size, data, psize);
	size += psize;
	memcpy(tmp->getresult(), &size, sizeof(int));
	this->sendbuf.push_back(tmp);
	PackitCheck(tmp->getresult(), tmp->getleft());
	pno++;
	return size;
}

void IPackit::unpackPackitB(STATUS* s,int* pno, int* psize, char* data)
{
	int packitsize = recvbuf->getcompleted();
	PackitCheck(recvbuf->getresult(), packitsize);
	int size = sizeof(int);
	memcpy(s, recvbuf->getresult() + size, sizeof(STATUS));
	size += sizeof(STATUS);
	memcpy(pno, recvbuf->getresult() + size, sizeof(int));
	size += sizeof(int);
	memcpy(data, recvbuf->getresult() + size, packitsize-size);
	*psize = packitsize - size;
}

bool IPackit::recvprogress(DWORD cbTransferred)
{
	return this->transPackit(recvbuf, cbTransferred, recvIO);
}

bool IPackit::sendprogress(DWORD cbTransferred)
{
	bool tmp= this->transPackit(sendbuf.front(), cbTransferred, sendIO);
	if (tmp == true)sendbuf.pop_front();
	return tmp;
}

bool IPackit::startrecv(){


	return this->recv(this->recvbuf);
}

bool IPackit::startsend()
{
	return this->send(this->sendbuf.front());
}

int IPackit::packPackitLOBBY(char* Dst, LOBBY_PROTOCOL p, int sel, char* msg)
{
	int size = 0;
	size += addDataatEndwithoutSize(Dst + size, &p,sizeof(LOBBY_PROTOCOL));
	size += addDataatEndwithoutSize(Dst + size, &sel,sizeof(int));
	size += addDataatEnd(Dst + size, msg, strlen(msg)+4);
	return size;
}

void IPackit::unpackpackitLOBBY(char* data, LOBBY_PROTOCOL* p, int* sel, char* msg)
{
	int tmp=0;
	int size = 0;
	size += getOnlyData(data+size, p,sizeof(LOBBY_PROTOCOL));
	size += getOnlyData(data+size, sel,sizeof(int));
	size += getDatawithSize(data+size, msg,&tmp);
}
