//need to make protocol standard class
#pragma once
#include<iostream>
#include<Windows.h>

typedef DWORD PROTOCOL;
typedef __int8 PROTOCOL_MAIN;
typedef __int8 PROTOCOL_SUB;
typedef __int16 PROTOCOL_DETAIL;
/*
	PROTOCOL - DWORD (UNSIGNED LONG - 32 bit)

	MAIN - 8 bit - at basic pack =>status					
	SUB -  8 bit - at basic pack => null					
	DETAIL - 16bit -at basic pack => enum class ENCRYPTED	
	*/

class ProtocolManager
{
private:
	static ProtocolManager* instance;

	int mainsize, subsize, detailsize;

	ProtocolManager();
	~ProtocolManager() {}
public:
	static void CreateInstance();
	static void ClearInstance();
	static ProtocolManager* GetInstance();

	void AddMainPart(PROTOCOL* target, PROTOCOL_MAIN main);
	void AddSubPart(PROTOCOL* target, PROTOCOL_SUB sub);
	void AddDetailPart(PROTOCOL* target, PROTOCOL_DETAIL detail);

	PROTOCOL_MAIN GetMainPart(PROTOCOL target);
	PROTOCOL_SUB GetSubPart(PROTOCOL target);
	PROTOCOL_DETAIL GetDetailPart(PROTOCOL target);
};