#pragma once

#include"BasicThing.h"
#include<string>
#include<fstream>
#include<ctime>
#include"CCS.h"
#include <codecvt>

class LogManager
{
private:
	static LogManager* instance;
	LogManager(bool a);
	~LogManager();

	std::string filePath;
	bool ak;
	CCS* key;

public:
	static void CreateInstance(bool a);
	static void ClearInstance();
	static LogManager* GetInstance();

	void printlog(char* fmt, ...);

	std::string getTodayDate();
	std::string getNow();

	static void LogPrint(char* fmt, ...);
	static void LogPrint(sentence* msg);

};