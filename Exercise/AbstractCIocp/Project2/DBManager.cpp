#include"DBManager.h"
DBManager* DBManager::instance = nullptr;

DBManager::DBManager(char* ip, char* user, char* pw, char* db,int port)
{
	LogManager::LogPrint("DBManager Created");

	dbConnectInfo[0] = ip;
	dbConnectInfo[1] = user;
	dbConnectInfo[2] = pw;
	dbConnectInfo[3] = db;
	portno = port;

	mysql_init(&mysql);

	if (!mysql_real_connect(&mysql,
		dbConnectInfo[0].c_str(),
		dbConnectInfo[1].c_str(),
		dbConnectInfo[2].c_str(),
		dbConnectInfo[3].c_str(),
		portno,
		NULL, 0))
	{
		std::cout << "MYSQL CONNECT ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}

	if (mysql_query(&mysql, "set names 'utf8';"))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}


}

DBManager::~DBManager()
{
	dbConnectInfo[0].clear();
	dbConnectInfo[1].clear();
	dbConnectInfo[2].clear();
	dbConnectInfo[3].clear();
	mysql_close(&mysql);
}

LoginInfo** DBManager::getJoinedMember(int* count)
{
	if (mysql_query(&mysql, "call joined_member();"))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
	LogManager::LogPrint("QUERY SENDED : call joined_member();");
	sql_result = mysql_store_result(&mysql);
	int k = sql_result->row_count;

	LoginInfo** tmp = new LoginInfo*[k];
	for (int i = 0; i < k; i++)
	{
		tmp[i] = new LoginInfo();
	}
	int c = 0;
	char* stop;
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		int idsize = MultiByteToWideChar(CP_ACP, 0, sql_row[1], -1, NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, sql_row[1], strlen(sql_row[1]) + 1, tmp[c]->ID, idsize);
		int pwsize = MultiByteToWideChar(CP_ACP, 0, sql_row[2], -1, NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, sql_row[2], strlen(sql_row[2]) + 1, tmp[c]->PW, pwsize);
		tmp[c]->uuid= (int)std::strtoll(sql_row[0], &stop, 10);
		c++;
		LogManager::LogPrint("ACCOUNT #%d : %s|%s|%d", c, tmp[c-1]->ID, tmp[c-1]->PW, tmp[c-1]->uuid);
	}

	while (mysql_more_results(&mysql))
		mysql_next_result(&mysql);

	mysql_free_result(sql_result);
	*count = k;

	return tmp;
}

int DBManager::getNewUUID(sentence* id, sentence* pw)
{

	int result = -1;
	char* stop;
	std::string quey = "call add_member('";
	std::wstring wid(id);
	std::string sid(wid.begin(), wid.end());
	quey.append(sid);
	quey.append("','");
	std::wstring wpw(pw);
	std::string spw(wpw.begin(), wpw.end());
	quey.append(spw);
	quey.append("');");
	LogManager::LogPrint("QUERY SENDED : %s",quey.c_str());

	if (mysql_query(&mysql, quey.c_str()))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
	sql_result = mysql_store_result(&mysql);
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		result = (int)std::strtoll(sql_row[0], &stop, 10);
	}
	while (mysql_more_results(&mysql))mysql_next_result(&mysql);
	mysql_free_result(sql_result);
	return result;
}

