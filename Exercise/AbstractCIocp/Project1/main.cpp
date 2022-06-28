#include"MainManager.h"

int main()
{
	MainManager::setlogprint();
	MainManager::getinstance()->init(9000);
	MainManager::getinstance()->run();
	MainManager::getinstance()->end();
	MainManager::getinstance()->clearinstance();
	return 0;
}