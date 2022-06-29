#include"BasicManager.h"

_BASICMANAGER::~_BASICMANAGER()
{
	msgmap.clear();
	LogManager::LogPrint("BasicManager Deleted");
}

_BASICMANAGER::_BASICMANAGER(int no)
{
	myNo = no;
	msgmap.clear();
	LogManager::LogPrint("BasicManager Created");
}

void _BASICMANAGER::ClearMap()
{
	msgmap.clear();
	LogManager::LogPrint("Map Cleared");
}

int _BASICMANAGER::MapSize()
{
	return msgmap.size();
}

void _BASICMANAGER::addMsg(int a, char* b)
{
	std::string tmp = b;
	msgmap.insert(std::make_pair(a,b));
	LogManager::LogPrint("Added %d - %s",a,b);
}

void _BASICMANAGER::getMsg(char* out, int in)
{
	strcpy(out, msgmap.find(in)->second.c_str());
}

int _BASICMANAGER::getNo()
{
	return myNo;
}
