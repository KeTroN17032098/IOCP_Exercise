#include"BasicManager.h"

_BASICMANAGER::~_BASICMANAGER()
{
	std::map<int, sentence*>::iterator iter;
	for (iter = msgmap.begin(); iter != msgmap.end(); iter++)
	{
		sentence* tmp = (*iter).second;
		delete tmp;
	}
	msgmap.clear();
	LogManager::LogPrint("BasicManager Deleted");
}

_BASICMANAGER::_BASICMANAGER(int no)
{
	myNo = (STATUS)no;
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

void _BASICMANAGER::addMsg(int a, sentence* b)
{
	sentence* tmp = new sentence[200];
	wcscpy(tmp, b);
	msgmap.insert(std::make_pair(a,tmp));
	LogManager::LogPrint("Added %d for msg",a);
	LogManager::LogPrint(b);
}

void _BASICMANAGER::getMsg(sentence* out, int in)
{
	wcscpy(out, msgmap.find(in)->second);
}

int _BASICMANAGER::getNo()
{
	return (int)myNo;
}
