#pragma once

#include"LogManager.h"
#include"ISession.h"
#include<map>
#include<string>
#include"AbstractState.h"
#include"ProtocolManager.h"

class _BASICMANAGER
{
private:
	std::map<int, std::string> msgmap;
	STATUS myNo;

public:
	virtual ~_BASICMANAGER();
	_BASICMANAGER(int no);
	void ClearMap();
	int MapSize();
	void addMsg(int a, char* b);
	void getMsg(char* out, int in);
	int getNo();
	virtual void Process(ISession* is,int* managerNo,char* data, int* datasize) abstract;
};