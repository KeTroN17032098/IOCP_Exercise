#include"LoginManager.h"

LoginManager* LoginManager::instance = nullptr;

LoginManager::LoginManager()
{
	join_member.clear();
	loggedin.clear();
	err_msg.clear();

	err_msg.insert(std::make_pair(MULTIPLEID,"아이디 중복"));
	err_msg.insert(std::make_pair(LENGTHLIMIT, "길이 초과"));
	err_msg.insert(std::make_pair(SMALLCHAR, "공백란 존재"));
	err_msg.insert(std::make_pair(ALREADYLGIN, "이미 로그인됨"));
	err_msg.insert(std::make_pair(NOMATCH, "잘못된 아이디 혹은 비번입니다."));
	err_msg.insert(std::make_pair(UNKNOWNERR, "알 수 없는 오류"));
	err_msg.insert(std::make_pair(NOERR, "에러 없음"));
}

LoginManager::~LoginManager()
{
	join_member.clear();
	loggedin.clear();
	err_msg.clear();
}

LGIN_SGIN_ERR LoginManager::trySignin(char* id, char* pw)
{
	if (strlen(id) == 0 || strlen(pw) == 0)return SMALLCHAR;
	if (strlen(id) > 20 || strlen(pw) > 20)return LENGTHLIMIT;
	for (int i = 0; i < join_member.size(); i++)
	{
		if (strcmp(join_member[i]->ID, id) == 0)return MULTIPLEID;
	}

	int uuid = createMember(id, pw);
	if (uuid == -1)return UNKNOWNERR;

	LoginInfo* tmp = init_li();
	strcpy(tmp->ID, id);
	strcpy(tmp->PW, pw);
	tmp->uuid = uuid;

	join_member.push_back(tmp);

	return NOERR;
}

LGIN_SGIN_ERR LoginManager::tryLogin(char* id, char* pw)
{
	LoginInfo* tmp = nullptr;
	std::deque<LoginInfo*>::iterator iter;
	for (iter = join_member.begin(); iter != join_member.end(); iter++)
	{
		if (strcmp(id, (*iter)->ID) == 0)
		{
			if (strcmp(pw, (*iter)->PW) == 0)
			{
				tmp = (*iter);
				if (std::find(loggedin.begin(), loggedin.end(), tmp->uuid) != loggedin.end())
				{
					return ALREADYLGIN;
				}
				else
				{
					loggedin.push_back(tmp->uuid);
					return NOERR;
				}
			}
			break;
		}
	}

	return NOMATCH;
}

void LoginManager::geterrmsg(char* msg, LGIN_SGIN_ERR e)
{
	strcpy(msg, err_msg.find(e)->second.c_str());
}

void LoginManager::getJoinMember()
{
	MYSQL mysql;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	mysql_init(&mysql);

	if (!mysql_real_connect(&mysql, HOST_IP, USER, PASSWORD, DATABASE, 3306, NULL, 0))
	{
		std::cout << "MYSQL CONNECT ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}

	if (mysql_query(&mysql, "set names 'euckr';"))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
	if (mysql_query(&mysql, "call joined_member();"))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
	sql_result = mysql_store_result(&mysql);

	int k = 0;
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		LoginInfo* tmp = init_li();
		strcpy(tmp->ID, sql_row[1]);
		strcpy(tmp->PW, sql_row[2]);
		char* stop;
		int uuid = (int)std::strtoll(sql_row[0],&stop,10);
		tmp->uuid = uuid;

		join_member.push_back(tmp);

		k++;
	}
	mysql_free_result(sql_result);

	mysql_close(&mysql);

	std::cout << "JOINED MEMBER LIST NUMBER : "<<k << std::endl;
	for (int i = 0; i < join_member.size(); i++)
	{
		std::cout << i+1 << "-[" << join_member[i]->uuid << "/" << join_member[i]->ID << "/" << join_member[i]->PW << "]" << std::endl;
	}
}

int LoginManager::createMember(char* id, char* pw)
{
	int result = -1;
	char* stop;
	MYSQL mysql;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	mysql_init(&mysql);

	if (!mysql_real_connect(&mysql, HOST_IP, USER, PASSWORD, DATABASE, 3306, NULL, 0))
	{
		std::cout << "MYSQL CONNECT ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}

	if (mysql_query(&mysql, "set names 'euckr';"))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
	std::string quey = "call add_member('";
	quey.append(id);
	quey.append("','");
	quey.append(pw);
	quey.append("');");
	if (mysql_query(&mysql,quey.c_str()))
	{
		std::cout << "MYSQL QUERY ERROR : " << mysql_error(&mysql) << std::endl;
		exit(-1);
	}
	sql_result = mysql_store_result(&mysql);


	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		result= (int)std::strtoll(sql_row[0], &stop, 10);
	}
	mysql_free_result(sql_result);

	mysql_close(&mysql);
}

int LoginManager::packPackit(char* Dest, LORS l, int p, int num, char* id, char* pw,char* msg, LGIN_SGIN_ERR e)
{
	int size = 0;
	size += addDataatEndwithoutSize(Dest + size, &l,sizeof(LORS));
	if (l == LGIN)
	{
		LOGIN_PROTOCOL lp = (LOGIN_PROTOCOL)p;
		size += addDataatEndwithoutSize(Dest + size, &lp,sizeof(int));
		if (lp == LOGIN_MSG)
		{
			size += addDataatEnd(Dest + size, msg,strlen(msg)+4);
		}
		else if (lp == LOGIN_TRY)
		{
			size += addDataatEnd(Dest + size, id, strlen(id));
			size += addDataatEnd(Dest + size, pw, strlen(pw));
		}
		else if (lp == LOGIN_SUCCESS)
		{
			size += addDataatEndwithoutSize(Dest + size, &num,sizeof(int));
			size += addDataatEnd(Dest + size, msg, strlen(msg)+4);
		}
		else
		{
			std::cout << msg << std::endl;
			size += addDataatEnd(Dest + size, msg, strlen(msg)+4);
			size += addDataatEndwithoutSize(Dest + size, &e, sizeof(LGIN_SGIN_ERR));
		}
	}
	else
	{
		SIGNIN_PROTOCOL sp = (SIGNIN_PROTOCOL)p;
		size += addDataatEndwithoutSize(Dest + size, &sp, sizeof(int));

		if (sp == SIGNIN_MSG)
		{
			size += addDataatEnd(Dest + size, msg, strlen(msg)+4);
		}
		else if (sp == SIGNIN_TRY)
		{
			size += addDataatEnd(Dest + size, id, strlen(id));
			size += addDataatEnd(Dest + size, pw, strlen(pw));
		}
		else if (sp == SIGNIN_SUCCESS)
		{
			size += addDataatEndwithoutSize(Dest + size, &num, sizeof(int));
			size += addDataatEnd(Dest + size, msg, strlen(msg)+4);
		}
		else
		{
			size += addDataatEnd(Dest + size, msg, strlen(msg)+4);
			size += addDataatEndwithoutSize(Dest + size, &e, sizeof(LGIN_SGIN_ERR));

		}
	}
	return size;
}

void LoginManager::unpackPackit(char* Data, LORS* l, int* p, int* num, char* id, char* pw,char* msg, LGIN_SGIN_ERR* e)
{
	int size = 0;
	int tmp=0;
	size += getOnlyData(Data + size, l,sizeof(LORS));
	size += getOnlyData(Data + size, p,sizeof(int));
	if (*l == LGIN)
	{
		LOGIN_PROTOCOL lp = (LOGIN_PROTOCOL)*p;

		if (lp == LOGIN_MSG)
		{
			size += getDatawithSize(Data + size, msg,&tmp);
		}
		else if (lp == LOGIN_TRY)
		{
			size += getDatawithSize(Data + size, id, &tmp);
			size += getDatawithSize(Data + size, pw, &tmp);
		}
		else if (lp == LOGIN_SUCCESS)
		{
			size += getOnlyData(Data + size, num, sizeof(int));
			size += getDatawithSize(Data + size,msg, &tmp);
		}
		else
		{
			size += getDatawithSize(Data + size, msg, &tmp);
			size += getOnlyData(Data + size, e, sizeof(LGIN_SGIN_ERR));

		}
	}
	else
	{
		SIGNIN_PROTOCOL sp = (SIGNIN_PROTOCOL)*p;

		if (sp == SIGNIN_MSG)
		{
			size += getDatawithSize(Data + size, msg, &tmp);
		}
		else if (sp == SIGNIN_TRY)
		{
			size += getDatawithSize(Data + size, id, &tmp);
			size += getDatawithSize(Data + size, pw, &tmp);
		}
		else if (sp == SIGNIN_SUCCESS)
		{
			size += getOnlyData(Data + size, num, sizeof(int));
			size += getDatawithSize(Data + size, msg, &tmp);
		}
		else
		{
			size += getDatawithSize(Data + size, msg, &tmp);
			size += getOnlyData(Data + size, e, sizeof(LGIN_SGIN_ERR));
		}
	}
}
