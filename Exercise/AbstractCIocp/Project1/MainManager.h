#pragma once
#include"ISessionManager.h"
#include"LoginManager.h"
//������ ä���ִ� ���� �Ŵ��� Ŭ��������
class MainManager : public _VIRTUALIOCP
{
private:
	ISessionManager* sm;
	LoginManager* lm;
	ISocket* listen_isock;

	static bool printlogs;
	static DWORD WINAPI WorkerThread(LPVOID arg);
	static MainManager* instance;

	MainManager();
	~MainManager();
public:
	static MainManager* getinstance()
	{
		if (instance == nullptr)instance = new MainManager();
		return instance;
	}
	static void setlogprint() { printlogs = true; }
	static void printlog(char* msg);
	static void printlog(int a, char* fmt, ...);
	static void clearinstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	bool packitproc(ISession* ptr);

	// _VIRTUALIOCP��(��) ���� ��ӵ�
	virtual void accept(SOCKET clientsock,HANDLE hcp) override;
	virtual void recv(void* session, DWORD cbTransferred) override;
	virtual void send(void* session, DWORD cbTransferred) override;
	virtual void disconnect(int retval,void* session) override;

	virtual void init(int portno) override;
	virtual void end() override;
	virtual void run() override;
};