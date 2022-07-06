#include"MainManager.h"

MainManager* MainManager::instance = nullptr;

MainManager::MainManager(bool a, char* dbip, char* dbuser, char* dbpw, char* dbname, int dbport) : _VIRTUALIOCP()
{
	int retval;
	LogManager::CreateInstance(a);
	ISessionManager::createinstance();
	DBManager::CreateInstance(dbip, dbuser, dbpw, dbname, dbport);
	LoginManager::CreateInstance();
	LobbyManager::CreateInstance();
	Crypt::CreateInstance();
	ListenSock = nullptr;

}

MainManager::~MainManager()
{

	ISessionManager::clearinstance();
	DBManager::clearInstance();
	LoginManager::ClearInstance();
	LobbyManager::ClearInstance();
	LogManager::ClearInstance();
	Crypt::ClearInstance();
}

void MainManager::CreateInstance(bool a, char* dbip, char* dbuser, char* dbpw, char* dbname, int dbport)
{
	if (instance == nullptr)
	{
		instance = new MainManager(a,dbip,dbuser,dbpw,dbname,dbport);
	}
}

void MainManager::ClearInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

MainManager* MainManager::GetInstance()
{
	return instance;
}


void MainManager::accept(SOCKET clientsock, HANDLE hcp)
{
	int retval;
	CreateIoCompletionPort((HANDLE)clientsock, hcp, clientsock, 0);
	LogManager::LogPrint("Connected Socket with IOCP");
	ISessionManager::getinstance()->addSession(clientsock);
	LogManager::LogPrint("Made Session by Socket");
	ISession* tmp = ISessionManager::getinstance()->findbysocket(clientsock);
	if (tmp->tryrecv() == false)
	{
		ISessionManager::getinstance()->deleteSession(clientsock);
	}
}

void MainManager::recv(void* session, DWORD cbTransferred)
{
	ISession* ptr = (ISession*)session;
	if (ptr->recvprogress(cbTransferred) == true)//府矫宏 场巢 -> 菩哦 贸府 ->傈价
	{
		char data[1024] = "";
		char data1[8192] = "";
		int a, b, c;
		LogManager::LogPrint("PackitProc Started");
		ptr->UnPack(&a, &b, &c, data1);
		ptr->clearRecvbuf();
		LogManager::LogPrint("Transfer to Manager : %d", a);
		if (ptr->checkpno(b) == false)return;
		if (a != START)
		{
			Crypt::GetInstance()->Decrypt(data1, data, &c);
		}
		else
		{
			memcpy(data, data1, c);
		}

		if (a == START)
		{
			LobbyManager::GetInstance()->outsideProcess(ptr,&a, data, &c);
		}
		else if (a == LOBBY)
		{
			LobbyManager::GetInstance()->insideProcess(ptr,&a, data, &c);
		}
		else if (a == LGSI)
		{
			LoginManager::GetInstance()->insideProcess(ptr,&a, data, &c);
		}
		else if (a == DISCONNECTED)
		{
			LogManager::LogPrint("REQUEST : DISCONNECTED");
		}
		else return;
		LogManager::LogPrint("Last Manager : %d", a);
		LogManager::LogPrint("Data Total Size : %d", c);
		int asd;
		if (a != DISCONNECTED) 
		{
			Crypt::GetInstance()->Encrypt(data, data1, &c,ptr->getPublicKey());
			asd = ptr->Pack(a, c, data1);
		}
		else asd = ptr->Pack(a, c, data);
		LogManager::LogPrint("Packet Total Size : %d",asd);

		ptr->changeStat((STATUS)a);
		ptr->trysend();
	}
	else//府矫宏 代 凳 -> 荐脚 拌加
	{
		ptr->tryrecv();
	}
}

void MainManager::send(void* session, DWORD cbTransferred)
{
	ISession* ptr = (ISession*)session;
	ptr->sendprogress(cbTransferred);
	if (ptr->is_sendleft() == true)ptr->trysend();
	else ptr->tryrecv();
}

void MainManager::disconnect(int retval, void* session)
{
	ISession* ptr = (ISession*)session;
	if (retval == 0)
	{
		DWORD temp1, temp2;
		ptr->getoverlapped(&temp1, &temp2);
		LogManager::LogPrint("WSAGetOverlappedResult()");
	}
	ISessionManager::getinstance()->deleteSession(ptr);
}

void MainManager::ListenSocketSet(int portno)
{//府胶醋 家南 檬扁拳
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//TCP,IPV4肺 家南 积己
	if (sock == INVALID_SOCKET)
	{
		LogManager::LogPrint("Socket Error - MainManger");
		exit(-1);
	}
	ListenSock = new ISocket(sock);
	ListenSock->Bind(portno);
}

void MainManager::ListenSocketEnd()
{//府胶醋 家南 秦力
	delete ListenSock;
	ListenSock = nullptr;
}

void MainManager::run(int portno)
{
	init();
	ListenSocketSet(portno);
	ListenSock->Listen();
	LogManager::LogPrint("Listen Started");
	while (1)
	{
		SOCKET cs = ListenSock->Accept();
		if (cs == INVALID_SOCKET)break;
		SOCKADDR_IN das;
		int sda = sizeof(das);
		getpeername(cs, (SOCKADDR*)&das, &sda);
		LogManager::LogPrint( "Socket Accepted : %s:%d", inet_ntoa(das.sin_addr), ntohs(das.sin_port));
		PostAccept(cs);
	}
	end();
	ListenSocketEnd();
}
