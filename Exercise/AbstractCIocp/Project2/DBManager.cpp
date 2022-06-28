#include"DBManager.h"

DBManager::DBManager(char* ip, char* user, char* pw, char* db,int port)
{

	mysql_init(&mysql);

	if (!mysql_real_connect(&mysql, ip,user,pw,db,port, NULL, 0))
	{
		std::cout << "MYSQL CONNECT ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}

	if (mysql_query(&mysql, "set names 'euckr';"))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
}

DBManager::~DBManager()
{
	mysql_close(&mysql);
}

LoginInfo* DBManager::getJoinedMember(int* count)
{
	if (mysql_query(&mysql, "call joined_member();"))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
	sql_result = mysql_store_result(&mysql);
	int k = sql_result->row_count;

	LoginInfo* tmp = new LoginInfo[k];
	for (int i = 0; i < k; i++)
	{
		ZeroMemory(&tmp[i], sizeof(LoginInfo));
	}
	int c = 0;
	char* stop;
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		strcpy(tmp[c].ID, sql_row[1]);
		strcpy(tmp[c].PW, sql_row[2]);
		tmp[c].uuid= (int)std::strtoll(sql_row[0], &stop, 10);
		c++;
	}
	mysql_free_result(sql_result);
	*count = k;
	return tmp;
}

int DBManager::getNewUUID(char* id, char* pw)
{
	int result = -1;
	char* stop;
	std::string quey = "call add_member('";
	quey.append(id);
	quey.append("','");
	quey.append(pw);
	quey.append("');");
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
	mysql_free_result(sql_result);
	return result;
}

