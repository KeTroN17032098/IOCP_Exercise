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

void GiveChar(char* Dst, char* fmt, ...)//원하는 문자열 포맷으로 만들기
{
	va_list arg;
	va_start(arg, fmt);

	vsprintf(Dst, fmt, arg);


	va_end(arg);
}

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


int addDataatEnd(char* dst, void* data, int size)
{
	/*
	먼저 앞에 사이즈 정보를 담은 int가 넣어진 후 데이터가 넣어집니다.
	반환값 : size+sizeof(int)
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
	입력된 데이터만 들어갑니다
	반환값 : size
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
	std::cout << "============================\n패킷 기본 헤더\n전체 사이즈 : " << a << "\n기본 프로토콜 : " << b << "\n패킷 넘버 : " << c << std::endl;
	if (b == LOBBY)
	{
		int d, e, f;
		memcpy(&d, packit + pt, sizeof(int));
		pt += sizeof(int);
		std::cout << "로비 패킷\n로비 프로토콜 : " << d << std::endl;
		char msg[500] = "";
		memcpy(&f, packit + pt, sizeof(int));
		pt += sizeof(int);
		std::cout << "선택 : " << f << std::endl;
		memcpy(&e, packit + pt, sizeof(int));
		pt += sizeof(int);
		memcpy(msg, packit + pt, e);
		pt += e;
		std::cout << "메시지 크기 : " << e << "\n메시지 내용 :" << msg << std::endl;



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
			std::cout << "로그인 패킷\n로그인/회원가입 여부 : " << d << "\n프로토콜 : " << e << std::endl;
			if (e == 5007)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(예정) : " << f << std::endl;
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
				std::cout << "아이디 크기 : " << g << "\n아이디 : " << id << "\n비밀번호 크기 : " << h << "\n비밀번호 : " << pw << std::endl;
			}
			if (e !=5006)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "메시지 크기 : " << g << "\n메시지 : " << msg << std::endl;
			}
			if (e == 5008)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "에러 코드 : " << h << std::endl;
			}
		}
		else
		{
			std::cout << "회원가입 패킷\n로그인/회원가입 여부 : " << d << "\n프로토콜 : " << e << std::endl;
			if (e ==438)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(예정) : " << f << std::endl;
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
				std::cout << "아이디 크기 : " << g << "\n아이디 : " << id << "\n비밀번호 크기 : " << h << "\n비밀번호 : " << pw << std::endl;
			}
			if (e !=437)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "메시지 크기 : " << g << "\n메시지 : " << msg << std::endl;
			}
			if (e ==439)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "에러 코드 : " << h << std::endl;
			}
		}
	}
	std::cout << "패킷 분석 완료\n===================================" << std::endl;
}