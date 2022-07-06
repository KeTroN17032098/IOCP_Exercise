#include"client.h"


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
Crypt* Crypt::instance = nullptr;

Crypt::Crypt()
{
	rsa_gen_keys(&pub, &priv);
}

Crypt::~Crypt()
{
}

void Crypt::CreateInstance()
{
	if (instance == nullptr)instance = new Crypt();
}

void Crypt::ClearInstance()
{
	if (instance != nullptr)delete instance;
}

Crypt* Crypt::GetInstance()
{
	return instance;
}

void Crypt::Encrypt(char* data, char* encrypted, int* size)
{
	encrypted = (char*)rsa_encrypt(data, *size, &pub);
	*size = 8 * (*size);
}

void Crypt::Encrypt(char* data, char* encrypted, int* size, public_key_class* key)
{
	encrypted = (char*)rsa_encrypt(data, *size, key);
	*size = 8 * (*size);
}

void Crypt::Decrypt(char* Encrypted, char* data, int* size)
{
	char* buf = rsa_decrypt((long long*)Encrypted, *size, &priv);
	memcpy(data, buf, (*size) / 8);
	*size = (*size) / 8;
}



int addDataatEndwithoutSize(char* dst, void* data, int size)
{
	/*
	입력된 데이터만 들어갑니다
	반환값 : size
	*/
	memcpy(dst, data, size);

	return size;
}

int getOnlyData(char* from, void* dst, int size)
{
	memcpy(dst, from, size);
	return size;
}


int getDatawithSize(char* from, void* dst, int* size)
{
	int tmp = 0;
	memcpy(size, from+tmp, sizeof(int));
	tmp += sizeof(int);
	memcpy(dst, from + tmp, *size+4);
	tmp += *size;
	return tmp;
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

int packPackitB(char* Dest,STATUS s,int pno, int psize, char* data)//패킹된 데이터에 헤더를 붙여서 송신 큐에 넘기는 함수
{
	int size = sizeof(int);
	memcpy(Dest + size, &s, sizeof(STATUS));
	size += sizeof(STATUS);
	memcpy(Dest + size, &pno, sizeof(int));
	size += sizeof(int);
	memcpy(Dest + size, data, psize);
	size += psize;
	memcpy(Dest, &size, sizeof(int));
	
	return size;
}
void unpackPackitB(char* From,int packitsize,STATUS* s, int* pno, int* psize, char* data)//헤더를 참조해서 패킹된 데이터를 언팩
{
	int size = sizeof(int);
	memcpy(s, From + size, sizeof(STATUS));
	size += sizeof(STATUS);
	memcpy(pno, From + size, sizeof(int));
	size += sizeof(int);
	memcpy(data,From + size, packitsize - size);
	*psize = packitsize - size;
}
int packPackit(char* Dest, LORS l, int p, int num, char* id, char* pw, char* msg, LGIN_SGIN_ERR e)
{
	int size = 0;
	size += addDataatEndwithoutSize(Dest + size, &l, sizeof(LORS));
	if (l == LGIN)
	{
		LOGIN_PROTOCOL lp = (LOGIN_PROTOCOL)p;
		size += addDataatEndwithoutSize(Dest + size, &lp, sizeof(int));
		if (lp == LOGIN_MSG)
		{
			size += addDataatEnd(Dest + size, msg, strlen(msg));
		}
		else if (lp == LOGIN_TRY)
		{
			size += addDataatEnd(Dest + size, id, strlen(id));
			size += addDataatEnd(Dest + size, pw, strlen(pw));
		}
		else if (lp == LOGIN_SUCCESS)
		{
			size += addDataatEndwithoutSize(Dest + size, &num, sizeof(int));
			size += addDataatEnd(Dest + size, msg, strlen(msg));
		}
		else
		{
			size += addDataatEnd(Dest + size, msg, strlen(msg));			
			size += addDataatEndwithoutSize(Dest + size, &e, sizeof(LGIN_SGIN_ERR));
		}
	}
	else
	{
		SIGNIN_PROTOCOL sp = (SIGNIN_PROTOCOL)p;
		size += addDataatEndwithoutSize(Dest + size, &sp, sizeof(int));

		if (sp == SIGNIN_MSG)
		{
			size += addDataatEnd(Dest + size, msg, strlen(msg));
		}
		else if (sp == SIGNIN_TRY)
		{
			size += addDataatEnd(Dest + size, id, strlen(id));
			size += addDataatEnd(Dest + size, pw, strlen(pw));
		}
		else if (sp == SIGNIN_SUCCESS)
		{
			size += addDataatEndwithoutSize(Dest + size, &num, sizeof(int));
			size += addDataatEnd(Dest + size, msg, strlen(msg));
		}
		else
		{
			size += addDataatEnd(Dest + size, msg, strlen(msg));
			size += addDataatEndwithoutSize(Dest + size, &e, sizeof(LGIN_SGIN_ERR));

		}
	}
	return size;
}
void unpackPackit(char* Data, LORS* l, int* p, int* num, char* id, char* pw, char* msg, LGIN_SGIN_ERR* e)
{
	int size = 0;
	int tmp=0;
	size += getOnlyData(Data + size, l, sizeof(LORS));
	size += getOnlyData(Data + size, p, sizeof(int));
	if (*l == LGIN)
	{
		LOGIN_PROTOCOL lp = (LOGIN_PROTOCOL)*p;

		if (lp == LOGIN_MSG)
		{
			size += getDatawithSize(Data + size, msg, &tmp);
		}
		else if (lp == LOGIN_TRY)
		{
			size += getDatawithSize(Data + size, id, &tmp);
			size += getDatawithSize(Data + size, pw, &tmp);
		}
		else if (lp == LOGIN_SUCCESS)
		{
			size += getOnlyData(Data + size, num, sizeof(int));
			size += getDatawithSize(Data + size, msg, &tmp);
		}
		else
		{
			size += getDatawithSize(Data + size, msg, &tmp);
			size += getOnlyData(Data + size, e, sizeof(LGIN_SGIN_ERR));
		}
	}
	else
	{
		SIGNIN_PROTOCOL sp = (SIGNIN_PROTOCOL)*p;

		if (sp == SIGNIN_MSG)
		{
			size += getDatawithSize(Data + size, msg, &tmp);
		}
		else if (sp == SIGNIN_TRY)
		{
			size += getDatawithSize(Data + size, id, &tmp);
			size += getDatawithSize(Data + size, pw, &tmp);
		}
		else if (sp == SIGNIN_SUCCESS)
		{
			size += getOnlyData(Data + size, num, sizeof(int));
			size += getDatawithSize(Data + size, msg, &tmp);
		}
		else
		{
			size += getDatawithSize(Data + size, msg, &tmp);
			size += getOnlyData(Data + size, e, sizeof(LGIN_SGIN_ERR));

		}
	}
}
int packPackitLOBBY(char* Dst, LOBBY_PROTOCOL p, int sel, char* msg)
{
	int size = 0;
	size += addDataatEndwithoutSize(Dst + size, &p, sizeof(LOBBY_PROTOCOL));
	if (p == SLECTION)size += addDataatEndwithoutSize(Dst + size, &sel, sizeof(int));
	if (p == WELCOMEMSG)size += addDataatEnd(Dst + size, msg, strlen(msg));
	return size;
}

void unpackpackitLOBBY(char* data, LOBBY_PROTOCOL* p, int* sel, char* msg)
{
	int size = 0;
	memcpy(p, data + size, sizeof(int));
	size += sizeof(int);
	if (*p == SLECTION)
	{
		memcpy(sel, data + size, sizeof(int));
		size += sizeof(int);
	}
	if (*p == WELCOMEMSG)
	{
		int msgsize = 0;
		memcpy(&msgsize, data + size, sizeof(int));
		size += sizeof(int);
		memcpy(msg, data + size, msgsize);
		size += msgsize;
	}
}
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)//input 변수가 대부분이나 buf 경우 포인터를 제공해 data를 받아옴
{
	int received;//받아온 데이터 바이트 수
	char* ptr = buf;//받아올 데이터 저장될 메모리 공간의 포인터
	int left = len;//남은 빈 메모리

	while (left > 0) {//메모리가 비어있을 동안 반복
		received = recv(s, ptr, left, flags);//recieved 만큼 데이터를 받아온다
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;//소켓에러의 경우 리턴값으로 소켓 에러를 출력하고 종료
		else if (received == 0)//아무것도 받아오지 않았다면 반복종료
			break;
		left -= received;//받아온 만큼 남은 빈 메모리 공간에서 뺀다
		ptr += received;//다음 데이터가 시작되는 메모리 포인터로 지정되게 한다
	}

	return (len - left);//데이터 받아온 양의 총 합을 리턴
}

void logPrint(bool logable, char* msg)
{
	if (logable == true)
	{
		std::cout << msg << std::endl;
	}
}
void PackitCheck(char* packit, int size)
{
#if loggable 
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
		char msg[500]="";
		if (d == SLECTION) 
		{
			memcpy(&f, packit + pt, sizeof(int));
			pt += sizeof(int);
			std::cout << "선택 : " << f << std::endl;
		}
		if (d == WELCOMEMSG) 
		{
			memcpy(&e, packit + pt, sizeof(int));
			pt += sizeof(int);
			memcpy(msg, packit + 20, a-20);
			pt += e;
			std::cout << "메시지 크기 : " << e << "\n메시지 내용 :" << msg << std::endl;
		}



	}
	else if (b == LGSI)
	{
		int d, e, f, g, h;
		char id[20] = "";
		char pw[20] = "";
		char msg[500]="";
		memcpy(&d, packit + pt, sizeof(int));
		pt += sizeof(int);
		memcpy(&e, packit + pt, sizeof(int));
		pt += sizeof(int);
		if (d==LGIN)
		{
			std::cout << "로그인 패킷\n로그인/회원가입 여부 : " << d << "\n프로토콜 : " << e << std::endl;
			if (e == LOGIN_SUCCESS)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(예정) : " << f << std::endl;
			}
			if (e==LOGIN_TRY)
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
			if(e!=LOGIN_TRY)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "메시지 크기 : " << g << "\n메시지 : " << msg << std::endl;
			}
			if (e == LOGIN_ERROR)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "에러 코드 : " << h << std::endl;
			}
		}
		else
		{
			std::cout << "회원가입 패킷\n로그인/회원가입 여부 : " << d << "\n프로토콜 : " << e << std::endl;
			if (e == SIGNIN_SUCCESS)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(예정) : " << f << std::endl;
			}
			if (e == SIGNIN_TRY)
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
			if (e != SIGNIN_TRY)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "메시지 크기 : " << g << "\n메시지 : " << msg << std::endl;
			}
			if (e ==SIGNIN_ERROR)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "에러 코드 : " << h << std::endl;
			}
		}
	}
	std::cout << "패킷 분석 완료\n===================================" << std::endl;
#else 
return;
#endif
}
void logPrint(bool logable, int data)
{
	if (logable == true)
	{
		std::cout << data << std::endl;
	}
}
int main()
{
	int retval;//리턴 값을 저장할 변수
	bool logable = true;


	// 윈속 초기화,dll 을 메모리에 올리기 위해 호출
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//버전 설정 후 성공이 리턴되지 않을 시 종료
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//TCP,IPV4로 소켓 생성
	if (sock == INVALID_SOCKET) err_quit("socket()");//소켓 생성 오류시 종료

	// connect()
	SOCKADDR_IN serveraddr;//서버 주소를 저장할 구조체
	ZeroMemory(&serveraddr, sizeof(serveraddr));//구조체 안을 비워준다
	serveraddr.sin_family = AF_INET;//구조체에 IP버전 저장
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);//구조체에 서버 ip 저장
	serveraddr.sin_port = htons(SERVERPORT);//구조체에 서버 포트 번호 저장
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));//연결이 될 소켓 핸들,서버 주소 구조체 포인터, 서버 주소 구조체 용량을 넣어서 연결
	if (retval == SOCKET_ERROR) err_quit("connect()");//에러시 종료
	
	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];//데이터 버퍼
	char data[BUFSIZE];
	char data1[BUFSIZE*8];

	int len;//보낼 버퍼의 배열 길이

	STATUS mystat = START;

	int pno,psize;

	pno = 1;

	public_key_class serverkey;

	Crypt::CreateInstance();


	ZeroMemory(data, sizeof(data));
	memcpy(data, Crypt::GetInstance()->getPublicKey(), sizeof(public_key_class));
	psize = sizeof(public_key_class);
	retval = packPackitB(buf, mystat, pno, psize, data);

	retval = send(sock, buf, retval, 0);
	if (retval == SOCKET_ERROR) 
	{
		err_quit("send");
	}
	logPrint(logable, retval);
	logPrint(logable, "START MSG SEND");
	// 서버와 데이터 통신
	while (1) 
	{//특수 상황 제외 계속 돌아감
		ZeroMemory(buf, sizeof(buf));
		len = 0;
		logPrint(logable, "RECV1");
		retval = recv(sock, buf,sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_quit("recv1");
		}
		memcpy(&len, buf, sizeof(int));
		logPrint(logable, retval);

		retval = recv(sock, buf + sizeof(int), len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_quit("recv2");
		}
		logPrint(logable, retval);

		ZeroMemory(data, sizeof(data));
		unpackPackitB(buf, len, &mystat, &pno, &psize, data);
		char msg[500] = "";
		ZeroMemory(msg, sizeof(msg));
		logPrint(logable, "UNPACKED BASIC");
		if (mystat == LOBBY)
		{
			LOBBY_PROTOCOL lp;
			int sel;
			unpackpackitLOBBY(data, &lp, &sel, msg);
			memcpy(&serverkey, data + psize - sizeof(public_key_class), sizeof(public_key_class));

			std::cout << msg << std::endl;
			std::cin >> sel;
			psize = packPackitLOBBY(data, SLECTION, sel, "");
			Crypt::GetInstance()->Encrypt(data, data1, &psize);
			len = packPackitB(buf, LOBBY, pno + 1, psize, data1);
		}
		else if (mystat == LGSI)
		{
			LORS mylrs;
			int lgsip;
			char id[20] = "";
			char pw[20] = "";
			LGIN_SGIN_ERR le;
			int uuif;
			unpackPackit(data, &mylrs, &lgsip, &uuif, id, pw, msg, &le);
			if (mylrs == LGIN)
			{
				logPrint(logable, "LOGIN STATUS");
				LOGIN_PROTOCOL lp = (LOGIN_PROTOCOL)lgsip;
				printf("%s\n", msg);
				if (lp == LOGIN_MSG||lp==LOGIN_ERROR)
				{
					logPrint(logable, "GET LOGININFO");
					std::cout << "아이디 :";
					std::cin >> id;
					std::cout << "비밀번호 :";
					std::cin >> pw;
					psize = packPackit(data, LGIN, LOGIN_TRY, 1, id, pw, nullptr, NOERR);
					Crypt::GetInstance()->Encrypt(data, data1, &psize);
					len = packPackitB(buf, LGSI, pno + 1, psize, data1);
				}
				else
				{
					len = packPackitB(buf, DISCONNECTED, pno + 1, 0, nullptr);
				}
			}
			else
			{
				logPrint(logable, "SIGNIN STATUS");

				SIGNIN_PROTOCOL sp = (SIGNIN_PROTOCOL)lgsip;
				std::cout << msg << std::endl;
				if (sp == SIGNIN_MSG || sp == SIGNIN_ERROR)
				{
					std::cout << "아이디 :";
					std::cin >> id;
					std::cout << "비밀번호 :";
					std::cin >> pw;
					psize = packPackit(data, SIGNIN, SIGNIN_TRY, 1, id, pw, nullptr, NOERR);
					Crypt::GetInstance()->Encrypt(data, data1, &psize);
					len = packPackitB(buf, LGSI, pno + 1, psize, data1);
				}
				else
				{
					len = packPackitB(buf, DISCONNECTED, pno + 1, 0, nullptr);

				}
			}
		}
		else if (mystat == DISCONNECTED)
		{
			break;
		}
		PackitCheck(buf, len);

		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_quit("send cycle");
		}
		logPrint(logable, "Send CycleCompleted");
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}