#pragma once

#include"LogManager.h"
#include"ISession.h"
#include<map>
#include"AbstractState.h"
#include"ProtocolManager.h"

class _BASICMANAGER
{
private:
	std::map<int, sentence*> msgmap;
	STATUS myNo;

public:
	virtual ~_BASICMANAGER();
	_BASICMANAGER(int no);
	void ClearMap();
	int MapSize();
	void addMsg(int a, sentence* b);
	void getMsg(sentence* out, int in);
	int getNo();
	virtual void Process(ISession* is,int* managerNo,char* data, int* datasize) abstract;
};