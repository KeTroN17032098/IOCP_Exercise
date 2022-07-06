#include"client.h"


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
	�Էµ� �����͸� ���ϴ�
	��ȯ�� : size
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
	���� �տ� ������ ������ ���� int�� �־��� �� �����Ͱ� �־����ϴ�.
	��ȯ�� : size+sizeof(int)
	*/
	int dszie = sizeof(int);
	memcpy(dst, &size, sizeof(int));
	memcpy(dst + dszie, data, size);
	dszie += size;
	return dszie;
}

int packPackitB(char* Dest,STATUS s,int pno, int psize, char* data)//��ŷ�� �����Ϳ� ����� �ٿ��� �۽� ť�� �ѱ�� �Լ�
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
void unpackPackitB(char* From,int packitsize,STATUS* s, int* pno, int* psize, char* data)//����� �����ؼ� ��ŷ�� �����͸� ����
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
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags)//input ������ ��κ��̳� buf ��� �����͸� ������ data�� �޾ƿ�
{
	int received;//�޾ƿ� ������ ����Ʈ ��
	char* ptr = buf;//�޾ƿ� ������ ����� �޸� ������ ������
	int left = len;//���� �� �޸�

	while (left > 0) {//�޸𸮰� ������� ���� �ݺ�
		received = recv(s, ptr, left, flags);//recieved ��ŭ �����͸� �޾ƿ´�
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;//���Ͽ����� ��� ���ϰ����� ���� ������ ����ϰ� ����
		else if (received == 0)//�ƹ��͵� �޾ƿ��� �ʾҴٸ� �ݺ�����
			break;
		left -= received;//�޾ƿ� ��ŭ ���� �� �޸� �������� ����
		ptr += received;//���� �����Ͱ� ���۵Ǵ� �޸� �����ͷ� �����ǰ� �Ѵ�
	}

	return (len - left);//������ �޾ƿ� ���� �� ���� ����
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
	std::cout << "============================\n��Ŷ �⺻ ���\n��ü ������ : " << a << "\n�⺻ �������� : " << b << "\n��Ŷ �ѹ� : " << c << std::endl;
	if (b == LOBBY)
	{
		int d, e, f;
		memcpy(&d, packit + pt, sizeof(int));
		pt += sizeof(int);
		std::cout << "�κ� ��Ŷ\n�κ� �������� : " << d << std::endl;
		char msg[500]="";
		if (d == SLECTION) 
		{
			memcpy(&f, packit + pt, sizeof(int));
			pt += sizeof(int);
			std::cout << "���� : " << f << std::endl;
		}
		if (d == WELCOMEMSG) 
		{
			memcpy(&e, packit + pt, sizeof(int));
			pt += sizeof(int);
			memcpy(msg, packit + 20, a-20);
			pt += e;
			std::cout << "�޽��� ũ�� : " << e << "\n�޽��� ���� :" << msg << std::endl;
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
			std::cout << "�α��� ��Ŷ\n�α���/ȸ������ ���� : " << d << "\n�������� : " << e << std::endl;
			if (e == LOGIN_SUCCESS)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(����) : " << f << std::endl;
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
				std::cout << "���̵� ũ�� : " << g << "\n���̵� : " << id << "\n��й�ȣ ũ�� : " << h << "\n��й�ȣ : " << pw << std::endl;
			}
			if(e!=LOGIN_TRY)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "�޽��� ũ�� : " << g << "\n�޽��� : " << msg << std::endl;
			}
			if (e == LOGIN_ERROR)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "���� �ڵ� : " << h << std::endl;
			}
		}
		else
		{
			std::cout << "ȸ������ ��Ŷ\n�α���/ȸ������ ���� : " << d << "\n�������� : " << e << std::endl;
			if (e == SIGNIN_SUCCESS)
			{
				memcpy(&f, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "UUID(����) : " << f << std::endl;
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
				std::cout << "���̵� ũ�� : " << g << "\n���̵� : " << id << "\n��й�ȣ ũ�� : " << h << "\n��й�ȣ : " << pw << std::endl;
			}
			if (e != SIGNIN_TRY)
			{
				memcpy(&g, packit + pt, sizeof(int));
				pt += sizeof(int);
				memcpy(msg, packit + pt, g);
				pt += g;
				std::cout << "�޽��� ũ�� : " << g << "\n�޽��� : " << msg << std::endl;
			}
			if (e ==SIGNIN_ERROR)
			{
				memcpy(&h, packit + pt, sizeof(int));
				pt += sizeof(int);
				std::cout << "���� �ڵ� : " << h << std::endl;
			}
		}
	}
	std::cout << "��Ŷ �м� �Ϸ�\n===================================" << std::endl;
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
	int retval;//���� ���� ������ ����
	bool logable = true;


	// ���� �ʱ�ȭ,dll �� �޸𸮿� �ø��� ���� ȣ��
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//���� ���� �� ������ ���ϵ��� ���� �� ����
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//TCP,IPV4�� ���� ����
	if (sock == INVALID_SOCKET) err_quit("socket()");//���� ���� ������ ����

	// connect()
	SOCKADDR_IN serveraddr;//���� �ּҸ� ������ ����ü
	ZeroMemory(&serveraddr, sizeof(serveraddr));//����ü ���� ����ش�
	serveraddr.sin_family = AF_INET;//����ü�� IP���� ����
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);//����ü�� ���� ip ����
	serveraddr.sin_port = htons(SERVERPORT);//����ü�� ���� ��Ʈ ��ȣ ����
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));//������ �� ���� �ڵ�,���� �ּ� ����ü ������, ���� �ּ� ����ü �뷮�� �־ ����
	if (retval == SOCKET_ERROR) err_quit("connect()");//������ ����
	
	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];//������ ����
	char data[BUFSIZE];
	char data1[BUFSIZE*8];

	int len;//���� ������ �迭 ����

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
	// ������ ������ ���
	while (1) 
	{//Ư�� ��Ȳ ���� ��� ���ư�
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
					std::cout << "���̵� :";
					std::cin >> id;
					std::cout << "��й�ȣ :";
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
					std::cout << "���̵� :";
					std::cin >> id;
					std::cout << "��й�ȣ :";
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

	// ���� ����
	WSACleanup();
	return 0;
}