#include"LogManager.h"

LogManager* LogManager::instance = nullptr;

LogManager::LogManager(bool a)
{
	ak = a;
	if (a == true) 
	{
		filePath.append(getTodayDate());
		filePath.append("_log.txt");
		std::cout << filePath << std::endl;
		printlog("LOG MANAGER STARTED");
	}
}

LogManager::~LogManager()
{
	printlog("LOG MANAGER END");
	if(ak==true)filePath.clear();
}

void LogManager::CreateInstance(bool a)
{
	if (instance == nullptr)
	{
		instance = new LogManager(a);
	}
}

void LogManager::ClearInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

LogManager* LogManager::GetInstance()
{
	return instance;
}

void LogManager::printlog(char* fmt, ...)
{
	CRITICAL_SECTION crit;
	InitializeCriticalSection(&crit);

	EnterCriticalSection(&crit);
	if (ak == true)
	{
		std::ofstream ofile;
		ofile.open(filePath, std::ios_base::out | std::ios_base::app);
		if (ofile.is_open())
		{
			char tmp[1024] = "";
			va_list vl;
			va_start(vl, fmt);
			vsprintf_s(tmp, fmt, vl);
			va_end(vl);

			std::string asd = getNow();
			asd.append(" - ");
			asd.append(tmp);
			asd.append("\n");
			ofile.write(asd.c_str(), asd.size());
			ofile.close();
		}
	}
	LeaveCriticalSection(&crit);
}

std::string LogManager::getTodayDate()
{
	if (ak == true) 
	{
		time_t timer;
		struct tm* t;
		timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
		t = localtime(&timer); // �������� ���� ����ü�� �ֱ�
		std::string tmp = "";
		tmp.append(std::to_string(t->tm_year+1900));
		tmp.append(std::to_string(t->tm_mon + 1));
		tmp.append(std::to_string(t->tm_mday));
		return tmp;
	}
	return NULL;
}

std::string LogManager::getNow()
{
	if (ak == true) 
	{
		time_t timer;
		struct tm* t;
		timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
		t = localtime(&timer); // �������� ���� ����ü�� �ֱ�
		std::string tmp = "";
		tmp.append(std::to_string(t->tm_year+1900));
		tmp.append(std::to_string(t->tm_mon + 1));
		tmp.append(std::to_string(t->tm_mday));
		tmp.append("/");
		tmp.append(std::to_string(t->tm_hour));
		tmp.append(":");
		tmp.append(std::to_string(t->tm_min));
		tmp.append(":");
		tmp.append(std::to_string(t->tm_sec));
		return tmp;
	}
	return NULL;
}

void LogManager::LogPrint(char* fmt, ...)
{
	CRITICAL_SECTION crit;
	InitializeCriticalSection(&crit);
	EnterCriticalSection(&crit);
	if (instance->ak == true)
	{
		std::ofstream ofile;
		ofile.open(instance->filePath, std::ios_base::out | std::ios_base::app);
		if (ofile.is_open())
		{
			char tmp[1024] = "";
			va_list vl;
			va_start(vl, fmt);
			vsprintf_s(tmp, fmt, vl);
			va_end(vl);

			std::string asd = instance->getNow();
			asd.append(" - ");
			asd.append(tmp);
			asd.append("\n");
			ofile.write(asd.c_str(), asd.size());
			ofile.close();
		}
	}
	LeaveCriticalSection(&crit);
}
