#include"AbstractIOCP.h"

DWORD __stdcall _VIRTUALIOCP::WorkerThread(LPVOID arg)
{
	int retval;
	_VIRTUALIOCP* vi = (_VIRTUALIOCP*)arg;
	DWORD myH = GetCurrentThreadId();
	HANDLE hcp = vi->getHCP();

	while (1)
	{
		DWORD cbTransferred;
		SOCKET client_sock;
		ExOverlapped* ptrk;
		LPOVERLAPPED ptra;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(PULONG_PTR)&client_sock, &ptra, INFINITE);
		ptrk = (ExOverlapped*)ptra;
		IO_TYPE iotype = ptrk->type;
		if (cbTransferred == -27 && iotype == acceptIO)//accept老 版快
		{
			vi->accept(client_sock, hcp);
			continue;
		}
		void* ptr = ptrk->clientLP;
		if (retval == 0 || cbTransferred == 0)
		{
			vi->disconnect(retval, ptr);
			continue;
		}
		//肯丰等 菩哦 贸府
		if (iotype == recvIO)
		{
			vi->recv(ptr, cbTransferred);
		}
		else if (iotype == sendIO)
		{
			vi->send(ptr, cbTransferred);
		}
	}
}
