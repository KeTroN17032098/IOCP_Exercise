#include"basicthing.h"

_BUFFER::_BUFFER(int size)
{
	buf = new char[size];
	left = 0;
	completed = 0;
	buftsize = size;
}

_BUFFER::_BUFFER(IO_TYPE iotype, int size, int psize)
{
	buf = new char[size];
	completed = 0;
	buftsize = size;
	if (iotype == sendIO)
	{
		left = psize + 12;
	}
	else if (iotype == recvIO)
	{
		left = size;
	}
}

char* _BUFFER::getbuf() {
	if (completed >= buftsize)return nullptr;
	return buf + completed;
}

void _BUFFER::setleft(int a) { left = a; }

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

char* _BUFFER::getresult() { return buf; }

int _BUFFER::getleft() { return left; }

int _BUFFER::getcompleted() { return completed; }

_BUFFER::~_BUFFER()
{
	delete[] buf;
}

LoginInfo* init_li()
{
	LoginInfo* tmp = new LoginInfo;
	ZeroMemory(tmp, sizeof(LoginInfo));
	tmp->uuid = -1;
	return tmp;
}

void DeleteLI(LoginInfo* l)
{
	ZeroMemory(l, sizeof(LoginInfo));
	delete l;
	l = nullptr;
}

void GiveChar(char* Dst, char* fmt, ...)//���ϴ� ���ڿ� �������� �����
{
	va_list arg;
	va_start(arg, fmt);

	vsprintf(Dst, fmt, arg);


	va_end(arg);
}

// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ���� �Լ� ���� ���
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[����] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


int addDataatEnd(char* dst, void* data, int size)
{
	/*
	���� �տ� ������ ������ ���� int�� �־��� �� �����Ͱ� �־����ϴ�.
	��ȯ�� : size+sizeof(int)
	*/
	int dszie = sizeof(int);
	memcpy(dst, &size, sizeof(int));
	memcpy(dst + dszie, data, size);
	dszie += size;
	return dszie;
}


int addDataatEndwithoutSize(char* dst, void* data, int size)
{
	/*
	�Էµ� �����͸� ���ϴ�
	��ȯ�� : size
	*/
	memcpy(dst , data,size );

	return size;
}

int getOnlyData(char* from, void* dst,int size)
{
	memcpy(dst, from, size);
	return size;
}


int getDatawithSize(char* from, void* dst,int* size)
{
	memcpy(size, from, sizeof(int));
	memcpy(dst, from + sizeof(int), *size);
	return *size + sizeof(int);
}
ExOverlapped* init_exover(IO_TYPE type, void* clientLp)
{
	ExOverlapped* tmp = new ExOverlapped;
	ZeroMemory(&tmp->overlapped, sizeof(WSAOVERLAPPED));
	tmp->type = type;
	tmp->clientLP = clientLp;
	return tmp;
}

void clear_exover(ExOverlapped* eo)
{
	ZeroMemory(&eo->overlapped, sizeof(WSAOVERLAPPED));
	eo->clientLP = nullptr;
	delete eo;
	eo = nullptr;
}

void PackitCheck(char* packit, int size)
{
	int a, b, c;

	int pt = 0;

	memcpy(&a, packit, sizeof(int));
	pt += sizeof(int);
	memcpy(&b, packit + pt, sizeof(int));
	pt += sizeof(int);
	memcpy(&c, packit + pt, sizeof(int));
	pt += sizeof(int);
	std::cout << "============================\n��Ŷ �⺻ ���\n��ü ������ : " << a << "\n�⺻ �������� : " << b << "\n��Ŷ �ѹ� : " << c << std::endl;
	if (b == LOBBY)
	{
		int d, e, f;
		memcpy(&d, packit + pt, sizeof(int));
		pt += sizeof(int);
		std::cout << "�κ� ��Ŷ\n�κ� �������� : " << d << std::endl;
		char msg[500] = "";
		memcpy(&f, packit + pt, sizeof(int));
		pt += sizeof(int);
		std::cout << "���� : " << f << std::endl;
		memcpy(&e, packit + pt, sizeof(int));
		pt += sizeof(int);
		memcpy(msg, packit + pt, e);
		pt += e;
		std::cout << "�޽��� ũ�� : " << e << "\n�޽��� ���� :" << msg << std::endl;



	}
	else if (b == LGSI)
	{
		int d, e, f, g, h;
		char id[20] = "";
		char pw[20] = "";
		char msg[500] = "";
		memcpy(&d, packit + pt, sizeof(int));
		pt += sizeof(int);
		memcpy(&e, packit + pt, sizeof(int));
		pt += sizeof(int);
		if (d == 76)
		{
			std::cout << "�α��� ��Ŷ\n�α���/ȸ������ ���� : " << d << "\n�������� : " << e << std::endl;
			if (e == 5007)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(����) : " << f << std::endl;
			}
			if (e == 5006)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(id, packit + pt, g);
				pt += g;
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(pw, packit + pt, h);
				pt += h;
				std::cout << "���̵� ũ�� : " << g << "\n���̵� : " << id << "\n��й�ȣ ũ�� : " << h << "\n��й�ȣ : " << pw << std::endl;
			}
			if (e !=5006)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "�޽��� ũ�� : " << g << "\n�޽��� : " << msg << std::endl;
			}
			if (e == 5008)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "���� �ڵ� : " << h << std::endl;
			}
		}
		else
		{
			std::cout << "ȸ������ ��Ŷ\n�α���/ȸ������ ���� : " << d << "\n�������� : " << e << std::endl;
			if (e ==438)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(����) : " << f << std::endl;
			}
			if (e ==437)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(id, packit + pt, g);
				pt += g;
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(pw, packit + pt, h);
				pt += h;
				std::cout << "���̵� ũ�� : " << g << "\n���̵� : " << id << "\n��й�ȣ ũ�� : " << h << "\n��й�ȣ : " << pw << std::endl;
			}
			if (e !=437)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "�޽��� ũ�� : " << g << "\n�޽��� : " << msg << std::endl;
			}
			if (e ==439)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "���� �ڵ� : " << h << std::endl;
			}
		}
	}
	std::cout << "��Ŷ �м� �Ϸ�\n===================================" << std::endl;
}