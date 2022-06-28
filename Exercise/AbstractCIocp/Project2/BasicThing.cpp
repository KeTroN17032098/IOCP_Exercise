#include"BasicThing.h"

_BUFFER::_BUFFER(int size)
{
	buf = new char[size];
	buftsize = size;
	left = 0;
	completed = 0;
}

_BUFFER::_BUFFER(IO_TYPE iotype, int size, int psize)
{
	buf = new char[size];
	buftsize = size;
	completed = 0;
	if (iotype == sendIO)
	{
		left = psize;
	}
	else if (iotype == recvIO)
	{
		left = size;
	}
}

char* _BUFFER::getbuf()
{
	if (completed >= buftsize)return nullptr;
	return buf + completed;
}

void _BUFFER::setleft(int a)
{
	left = a;
}

void _BUFFER::fcompleted(int a)
{
	left -= a;
	completed += a;
}

bool _BUFFER::is_completed()
{
	if (completed > sizeof(int) && left == 0)return true;
	else return false;
}

char* _BUFFER::getresult()
{
	return buf;
}

int _BUFFER::getleft()
{
	return left;
}

int _BUFFER::getcompleted()
{
	return completed;
}

void _BUFFER::recvclear()
{
	ZeroMemory(buf, this->buftsize);
	left = buftsize;
	completed = 0;
}

_BUFFER::~_BUFFER()
{
	delete[] buf;
}

