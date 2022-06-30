#pragma once

#include"LogManager.h"
#include"ISession.h"
#include<map>
#include<string>

class _BASICMANAGER
{
private:
	std::map<int, std::string> msgmap;
	int myNo;

public:
	virtual ~_BASICMANAGER();
	_BASICMANAGER(int no);
	void ClearMap();
	int MapSize();
	void addMsg(int a, char* b);
	void getMsg(char* out, int in);
	int getNo();
	virtual void insideProcess(ISession* is,int* managerNo,char* data, int* datasize) abstract;
	virtual void outsideProcess(ISession* is,int* managerNo, char* data, int* datasize) abstract;
};