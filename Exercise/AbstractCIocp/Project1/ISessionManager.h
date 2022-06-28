#pragma once
#include"ISession.h"

class ISessionManager
{
private:
	std::deque<ISession*> sessions;

	static ISessionManager* instance;
	ISessionManager();
	~ISessionManager();
public:
	static ISessionManager* getinstance()
	{
		if (instance == nullptr)instance = new ISessionManager();
		return instance;
	}
	static void clearinstance()
	{
		if (instance != nullptr) 
		{
			delete instance;
			instance = nullptr;
		}
	}

	ISession* findbysocket(SOCKET s);
	ISession* findbyuuid(int i);

	void addSession(SOCKET s);
	void deleteSession(SOCKET s);
	void deleteSession(ISession* s)
	{
		std::deque<ISession*>::iterator iter;
		for (iter = sessions.begin(); iter != sessions.end(); iter++)
		{
			if (s == *iter)
			{
				sessions.erase(iter);
				std::cout << "Erased From Session Que" << std::endl;
				delete s;
				std::cout << "Deleted Session" << std::endl;
				s = nullptr;
				return;
			}
		}
	}
};