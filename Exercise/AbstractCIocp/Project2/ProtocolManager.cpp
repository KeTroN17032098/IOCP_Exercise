#include"ProtocolManager.h"

ProtocolManager* ProtocolManager::instance = nullptr;

ProtocolManager::ProtocolManager()
{
	mainsize = sizeof(PROTOCOL_MAIN) * 8;
	subsize = sizeof(PROTOCOL_SUB) * 8;
	detailsize = sizeof(PROTOCOL_DETAIL) * 8;
}

void ProtocolManager::CreateInstance()
{
	if (instance == nullptr)instance = new ProtocolManager();
}

void ProtocolManager::ClearInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

ProtocolManager* ProtocolManager::GetInstance()
{
	return instance;
}

void ProtocolManager::AddMainPart(PROTOCOL* target, PROTOCOL_MAIN main)
{
	PROTOCOL tmp = static_cast<PROTOCOL>(main);
	tmp = tmp << subsize + detailsize;
	*target = (*target) % (1 << subsize + detailsize);
	*target = (*target) | (main << subsize + detailsize);
}
void ProtocolManager::AddSubPart(PROTOCOL* target, PROTOCOL_SUB sub)
{
	PROTOCOL tmp = static_cast<PROTOCOL>(sub);
	PROTOCOL_SUB tmps = 0;
	~tmps;
	*target &= ~(tmps << detailsize);
	tmp = tmp << detailsize;
	*target |= tmp;
}

void ProtocolManager::AddDetailPart(PROTOCOL* target, PROTOCOL_DETAIL detail)
{
	PROTOCOL tmp = static_cast<PROTOCOL>(detail);
	*target = ((*target) >> detailsize) << detailsize;
	*target |= tmp;
}

PROTOCOL_MAIN ProtocolManager::GetMainPart(PROTOCOL target)
{
	PROTOCOL tmp = target >> detailsize + subsize;
	return static_cast<PROTOCOL_MAIN>(tmp);
}

PROTOCOL_SUB ProtocolManager::GetSubPart(PROTOCOL target)
{
	PROTOCOL tmp = target / (1 << detailsize);
	tmp %= (1 << subsize);
	return static_cast<PROTOCOL_SUB>(tmp);
}

PROTOCOL_DETAIL ProtocolManager::GetDetailPart(PROTOCOL target)
{
	PROTOCOL tmp = target % (1 << detailsize);
	return static_cast<PROTOCOL_DETAIL>(tmp);
}
