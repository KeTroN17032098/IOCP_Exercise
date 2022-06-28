#pragma once
#pragma comment(lib,"libmysql.lib")
#include"basicthing.h"
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

	DBManager(char* ip,char* user,char* pw,char* db,int port);
	~DBManager();
public:
	static DBManager* getinstance()
	{
		return instance;
	}
	static void CreateInstance(char* ip, char* user, char* pw, char* db,int port)
	{
		instance = new DBManager(ip, user, pw, db,port);
	}
	void clearInstance()
	{
		delete instance;
	}
	LoginInfo* getJoinedMember(int* count);
	int getNewUUID(char* id, char* pw);
};