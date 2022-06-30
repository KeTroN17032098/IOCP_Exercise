#pragma once

#define DEBUG_LOG
#include"MainManager.h"

int main()
{
	MainManager::CreateInstance(true, "127.0.0.1", "Last", "1234", "simpleaccount", 3306);
	MainManager::GetInstance()->run(9000);
	MainManager::GetInstance()->ClearInstance();
	return 0;
}