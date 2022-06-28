#include"LoginManager.h"

LoginManager* LoginManager::instance = nullptr;

LoginManager::LoginManager()
{
	join_member.clear();
	loggedin.clear();
	err_msg.clear();
	err_msg.insert(std::make_pair(MULTIPLEID, "아이디 중복"));
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

void LoginManager::CreateInstance()
{
	instance = new LoginManager();
}

LoginManager* LoginManager::GetInstance()
{
	return instance;
}

void LoginManager::ClearInstance()
{
	delete instance;
}

int LoginManager::trySignin(char* id, char* pw)
{
	if (strlen(id) == 0 || strlen(pw) == 0)return SMALLCHAR;
	if (strlen(id) > 20 || strlen(pw) > 20)return LENGTHLIMIT;
	for (int i = 0; i < join_member.size(); i++)
	{
		if (strcmp(join_member[i]->ID, id) == 0)return MULTIPLEID;
	}

	int uuid = DBManager::getinstance()->getNewUUID(id, pw);
	if (uuid == -1)return UNKNOWNERR;

	LoginInfo* tmp = new LoginInfo();
	ZeroMemory(tmp, sizeof(LoginInfo));
	strcpy(tmp->ID, id);
	strcpy(tmp->PW, pw);
	tmp->uuid = uuid;

	join_member.push_back(tmp);

	return NOERR;
}

int LoginManager::tryLogin(char* id, char* pw)
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

void LoginManager::geterrmsg(char* msg, int e)
{
	strcpy(msg, err_msg.find((LGIN_SGIN_ERR)e)->second.c_str());
}

int LoginManager::packPackit(char* Dest, int l, int p, int num, char* id, char* pw, char* msg, int e)
{
	int size = 0;
	memcpy(Dest + size, &l, sizeof(int));
	size += sizeof(int);
	if (l == LGIN)
	{
		memcpy(Dest + size, &p, sizeof(int));
		size += sizeof(int);
		if (p == LOGIN_SUCCESS)
		{
			memcpy(Dest + size, &num, sizeof(int));
			size += sizeof(int);
		}
		if (p == LOGIN_TRY)
		{
			int k = strlen(id);
			memcpy(Dest + size, &k, sizeof(int));
			size += sizeof(int);
			memcpy(Dest + size, id, k);
			size += k;
			k = strlen(pw);
			memcpy(Dest + size, &k, sizeof(int));
			size += sizeof(int);
			memcpy(Dest + size, pw, k);
			size += k;
		}
		if (p != LOGIN_TRY)
		{
			int k = strlen(msg);
			memcpy(Dest + size, &k, sizeof(int));
			size += sizeof(int);
			memcpy(Dest + size, msg, k);
			size += k;

		}
		if (p == LOGIN_ERROR)
		{
			memcpy(Dest + size, &e, sizeof(int));
			size += sizeof(int);
		}
	}
	else if (l==SIGNIN)
	{
		memcpy(Dest + size, &p, sizeof(int));
		size += sizeof(int);
		if (p == SIGNIN_SUCCESS)
		{
			memcpy(Dest + size, &num, sizeof(int));
			size += sizeof(int);
		}
		if (p == SIGNIN_TRY)
		{
			int k = strlen(id);
			memcpy(Dest + size, &k, sizeof(int));
			size += sizeof(int);
			memcpy(Dest + size, id, k);
			size += k;
			k = strlen(pw);
			memcpy(Dest + size, &k, sizeof(int));
			size += sizeof(int);
			memcpy(Dest + size, pw, k);
			size += k;
		}
		if (p != SIGNIN_TRY)
		{
			int k = strlen(msg);
			memcpy(Dest + size, &k, sizeof(int));
			size += sizeof(int);
			memcpy(Dest + size, msg, k);
			size += k;
		}
		if (p == SIGNIN_ERROR)
		{
			memcpy(Dest + size, &e, sizeof(int));
			size += sizeof(int);
		}
	}

	return size;
}

void LoginManager::unpackPackit(char* Data, int* l, int* p, int* num, char* id, char* pw, char* msg, int* e)
{
	int size = 0;
	memcpy(l,Data+size, sizeof(int));
	size += sizeof(int);
	if (*l == LGIN)
	{
		memcpy(p,Data+size, sizeof(int));
		size += sizeof(int);
		if (*p == LOGIN_SUCCESS)
		{
			memcpy(num,Data+size, sizeof(int));
			size += sizeof(int);
		}
		if (*p == LOGIN_TRY)
		{
			int k = 0;
			memcpy(&k,Data+size, sizeof(int));
			size += sizeof(int);
			memcpy(id,Data+size, k);
			size += k;
			k = 0;
			memcpy(&k, Data + size, sizeof(int));
			size += sizeof(int);
			memcpy(pw, Data + size, k);
			size += k;
		}
		if (*p != LOGIN_TRY)
		{
			int k = 0;
			memcpy(&k, Data + size, sizeof(int));
			size += sizeof(int);
			memcpy(msg, Data + size, k);
			size += k;

		}
		if (*p == LOGIN_ERROR)
		{
			memcpy(e,Data+size,sizeof(int));
			size += sizeof(int);
		}
	}
	else if (*l == SIGNIN)
	{
		memcpy(p, Data + size, sizeof(int));
		size += sizeof(int);
		if (*p == SIGNIN_SUCCESS)
		{
			memcpy(num, Data + size, sizeof(int));
			size += sizeof(int);
		}
		if (*p == SIGNIN_TRY)
		{
			int k = 0;
			memcpy(&k, Data + size, sizeof(int));
			size += sizeof(int);
			memcpy(id, Data + size, k);
			size += k;
			k = 0;
			memcpy(&k, Data + size, sizeof(int));
			size += sizeof(int);
			memcpy(pw, Data + size, k);
			size += k;
		}
		if (*p != SIGNIN_TRY)
		{
			int k = 0;
			memcpy(&k, Data + size, sizeof(int));
			size += sizeof(int);
			memcpy(msg, Data + size, k);
			size += k;
		}
		if (*p == SIGNIN_ERROR)
		{
			memcpy(e, Data + size, sizeof(int));
			size += sizeof(int);
		}
	}

}
