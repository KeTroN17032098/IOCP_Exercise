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
	static void createinstance()
	{
		instance = new ISessionManager();
	}
	static ISessionManager* getinstance()
	{
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
				delete s;
				s = nullptr;
				return;
			}
		}
	}
};