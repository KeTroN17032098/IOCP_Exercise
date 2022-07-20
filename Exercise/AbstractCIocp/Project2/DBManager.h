#pragma once
#pragma comment(lib,"libmysql.lib")
#include"LogManager.h"
#include<string>
#include<iostream>
#include<mysql.h>

class DBManager
{
private:
	static DBManager* instance;

	MYSQL mysql;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	std::string dbConnectInfo[4];
	int portno;

	DBManager(char* ip,char* user,char* pw,char* db,int port);
	~DBManager();
public:
	static DBManager* getinstance()
	{
		return instance;
	}
	static void CreateInstance(char* ip, char* user, char* pw, char* db,int port)
	{
		if (instance == nullptr)instance = new DBManager(ip, user, pw, db,port);
	}
	static void clearInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
			LogManager::LogPrint("DBManger Deleted");

		}
	}
	LoginInfo** getJoinedMember(int* count);
	int getNewUUID(sentence* id, sentence* pw);
};