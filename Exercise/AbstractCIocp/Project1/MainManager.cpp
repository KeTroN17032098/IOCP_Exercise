#include"MainManager.h"

MainManager* MainManager::instance = nullptr;
bool MainManager::printlogs = false;

DWORD __stdcall MainManager::WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;
	DWORD myH = GetCurrentThreadId();

	while (1)
	{
		DWORD cbTransferred;
		SOCKET client_sock;
		ExOverlapped* ptrk;
		LPOVERLAPPED ptra;
		bool packitproc = false;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(PULONG_PTR)&client_sock, &ptra, INFINITE);
		printlog(0, "ThreadID[%d] - getcompletionstat", myH);
		ptrk = (ExOverlapped*)ptra;
		IO_TYPE iotype = ptrk->type;
		if (cbTransferred == -27 && iotype == acceptIO)//accept일 경우
		{
			printlog(0,"ThreadID[%d] -  Accept Process",myH);
			instance->accept(client_sock,hcp);
			printlog(0,"ThreadID[%d] - Accept Completed",myH);
			continue;
		}
		ISession* ptr = (ISession*)ptrk->clientLP;
		printlog(0, "ThreadID[%d] - Transferred Byte : %d", myH, cbTransferred);
		if (retval == 0 || cbTransferred == 0)
		{
			printlog(0, "ThreadID[%d] - Disconnect Process", myH);
			instance->disconnect(retval,ptr);
			printlog(0, "ThreadID[%d] - Disconnect Success", myH);
			continue;
		}
		//완료된 패킷 처리
		if (iotype == recvIO)
		{
			printlog(0, "ThreadID[%d] - Recv Process", myH);
			instance->recv(ptr, cbTransferred);
			printlog(0, "ThreadID[%d] - Recv Process Complete", myH);
		}
		else if (iotype == sendIO)
		{
			printlog(0, "ThreadID[%d] - Send Process", myH);
			instance->send(ptr, cbTransferred);
			printlog(0, "ThreadID[%d] - Send Process Complete", myH);
		}

	}
}

MainManager::MainManager()
{
	sm = nullptr;
	lm = nullptr;
	listen_isock = nullptr;
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)err_quit("wsastartup");
	printlog("wsa startup");
}

MainManager::~MainManager()
{
	sm = nullptr;
	lm = nullptr;
	listen_isock = nullptr;
	WSACleanup();
	printlog("wsa closed");
}

void MainManager::printlog(char* msg)
{
	if (printlogs == false)return;

	std::cout << msg << std::endl;
}

void MainManager::printlog(int a, char* fmt, ...)
{
	if (printlogs == false)return;
	char Dst[600];

	va_list arg;
	va_start(arg, fmt);

	vsprintf(Dst, fmt, arg);
	std::cout << Dst << std::endl;

	va_end(arg);
}

bool MainManager::packitproc(ISession* ptr)
{
	STATUS s;
	int pno,psize;
	char data[1024];
	char buf[1024];
	ptr->unpackPackitB(&s, &pno, &psize, data);
	if (ptr->checkpno(pno) == false)err_quit("과거패킷");
	if (s == START&&ptr->getStat()==START)//lobby msg 전송
	{
		char dsadfs[300] = "1번으로 로그인\n2번으로 회원가입\n";
		psize=ptr->packPackitLOBBY(data, WELCOMEMSG,strlen(dsadfs),dsadfs);
		ptr->packPackitB(LOBBY, psize, data);
		ptr->changeStat(LOBBY);
		printlog("WELCOMEMSG PACKED");
	}
	else if (s == LOBBY&&ptr->getStat()==LOBBY)//로비 선택지 선택됨
	{
		LOBBY_PROTOCOL p;
		int sel;
		ptr->unpackpackitLOBBY(data, &p, &sel, buf);
		ptr->PrintAddress();
		printlog(0, "PROTOCOL : %d || Selected : %d", p,sel);
		if (p == SLECTION)
		{
			ZeroMemory(data, sizeof(data));
			if (sel == 1)
			{
				psize=lm->packPackit(data, LGIN, LOGIN_MSG, -1, nullptr, nullptr, "아이디와 비밀번호를 입력하세요.\n", NOERR);
				ptr->packPackitB(LGSI, psize, data);
				ptr->changeStat(LGSI);
				printlog("LOGINMSG PACKED");
			}
			else if (sel == 2)
			{
				psize = lm->packPackit(data,SIGNIN , SIGNIN_MSG, -1, nullptr, nullptr, "아이디와 비밀번호를 입력하세요.\n", NOERR);
				ptr->packPackitB(LGSI, psize, data);
				ptr->changeStat(LGSI);
				printlog("SIGNINMSG PACKED");

			}
		}
	}
	else if (s==LGSI&&ptr->getStat()==LGSI)//로그인 매니저로 판단
	{
		LORS l;
		int p,nun;
		char Id[20] = "";
		char pw[20]="";
		char msg[300]="";
		LGIN_SGIN_ERR le;
		lm->unpackPackit(data, &l, &p, &nun, Id, pw, nullptr, nullptr);
		if (l == LGIN)
		{

			le = lm->tryLogin(Id, pw);
			if (le == NOERR)//로그인 성공 메시지
			{
				psize = lm->packPackit(data, l, LOGIN_SUCCESS, 1, nullptr, nullptr, "로그인 성공", NOERR);
				ptr->packPackitB(LGSI, psize, data);
			}
			else//로그인 실패
			{
				lm->geterrmsg(msg, le);
				psize = lm->packPackit(data, l, LOGIN_ERROR, 1, nullptr, nullptr, msg, le);
				ptr->packPackitB(LGSI, psize, data);
			}
		}
		else if(l==SIGNIN)
		{
			le = lm->trySignin(Id, pw);
			if (le == NOERR)//로비로 귀환
			{
				psize = lm->packPackit(data, l, SIGNIN_SUCCESS, 1, nullptr, nullptr, "회원가입 성공", NOERR);
				ptr->packPackitB(LGSI, psize, data);
				ptr->changeStat(LOBBY);
			}
			else//회원가입 실패
			{	
				lm->geterrmsg(msg, le);
				psize = lm->packPackit(data, l, SIGNIN_ERROR, 1, nullptr, nullptr,msg, le);
				ptr->packPackitB(LGSI, psize, data);
			}
		}
	}
	else//disconnected
	{
		sm->deleteSession(ptr);
		printlog("cracked");
		return false;
	}
	//패킷 처리후 recv버퍼 초기화 필요
	ptr->clearRecvbuf();
	return true;
}

void MainManager::accept(SOCKET clientsock,HANDLE hcp)
{
	int retval;
	CreateIoCompletionPort((HANDLE)clientsock, hcp, clientsock, 0);
	printlog("Connected Socket with IOCP");
	sm->addSession(clientsock);
	printlog("Made Session by Socket");
	ISession* tmp = sm->findbysocket(clientsock);
	if (tmp->startrecv() == false)
	{
		sm->deleteSession(clientsock);
	}

}

void MainManager::recv(void* session, DWORD cbTransferred)
{
	ISession* ptr = (ISession*)session;
	if (ptr->recvprogress(cbTransferred)==true)//리시브 끝남 -> 패킷 처리 ->전송
	{
		printlog("PackitProc Started");
		bool fads=packitproc(ptr);
		printlog("PackitProc Ended");
		if (fads==false) 
		{
			printlog("Session Completed");
			return;
		}
		ptr->startsend();
	}
	else//리시브 덜 됨 -> 수신 계속
	{
		ptr->startrecv();
	}
}

void MainManager::send(void* session, DWORD cbTransferred)
{
	ISession* ptr = (ISession*)session;
	ptr->sendprogress(cbTransferred);
	//만약 송신 큐에 남아있다면 계속 송신
	//송신 할 것이 없다면 수신 해야 함
	if (ptr->is_sendleft() == true)
	{
		ptr->startsend();
	}
	else
	{
		ptr->startrecv();
	}
}

void MainManager::disconnect(int retval,void* session)
{
	ISession* ptr = (ISession*)session;
	if (retval == 0)
	{
		DWORD temp1, temp2;
		ptr->getoverlapped(&temp1, &temp2);
		err_display("WSAGetOverlappedResult()");
	}
	sm->deleteSession(ptr);
}



void MainManager::init(int portno)
{
	int retval;

	SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == INVALID_SOCKET) err_quit("socket()");
	printlog("listen socket created");


	listen_isock = new ISocket(listen_socket);
	listen_isock->Bind(9000);
	printlog("listen isocket binded");

	sm = ISessionManager::getinstance();
	printlog("Session Manager created");

	lm = LoginManager::getinstance();
	printlog("LoginManager created");

	lm->getJoinMember();
	printlog("JoinMember get");
}

void MainManager::end()
{
	delete listen_isock;
	ISessionManager::clearinstance();
	LoginManager::clearinstance();
}

void MainManager::run()
{
	int retval;
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
	{
		printlog("IOCP FAILED");
		return;
	}
	printlog("IOCP Start");

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL)
		{
			printlog("IOCP Thread FAILED");
			return;
		}
		printlog(0, "Thread No.%d Created", i + 1);
		CloseHandle(hThread);
	}
	ExOverlapped* accept_overlapped = init_exover(acceptIO, this);
	DWORD recvbytes, flags;
	listen_isock->Listen();
	printlog("Listen Start");
	while (1)
	{
		SOCKET cs = listen_isock->Accept();
		if (cs == INVALID_SOCKET)break;
		SOCKADDR_IN das;	
		int sda = sizeof(das);
		getpeername(cs, (SOCKADDR*)&das, &sda);
		printlog(0, "Socket Accepted : %s:%d",inet_ntoa(das.sin_addr),ntohs(das.sin_port));
		PostQueuedCompletionStatus(hcp, -27, cs, &accept_overlapped->overlapped);
	}
}
