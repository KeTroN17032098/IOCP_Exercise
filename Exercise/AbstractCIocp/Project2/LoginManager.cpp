#include"LoginManager.h"

LoginManager* LoginManager::instance = nullptr;

LoginManager::LoginManager(): _BASICMANAGER(STATUS::LGSI)
{
	join_member.clear();
	loggedin.clear();
	_BASICMANAGER::addMsg(LOGIN_MSG, "로그인을 하시기위해 아이디와 비밀번호를 입력하세요.");
	_BASICMANAGER::addMsg(SIGNIN_MSG, "회원가입을 하시기위해 아이디와 비밀번호를 입력하세요.");
	_BASICMANAGER::addMsg(MULTIPLEID, "아이디 중복입니다.");
	_BASICMANAGER::addMsg(LENGTHLIMIT, "아이디와 비밀번호는 20자이내로 입력해주세요.");
	_BASICMANAGER::addMsg(SMALLCHAR, "너무 짧습니다.");
	_BASICMANAGER::addMsg(ALREADYLGIN, "이미 로그인되어 있습니다.");
	_BASICMANAGER::addMsg(NOMATCH, "아이디 혹은 비밀번호 오류입니다.");
	_BASICMANAGER::addMsg(UNKNOWNERR, "알 수 없는 오류입니다.");
	_BASICMANAGER::addMsg(NOERR, "에러가 없습니다.");
	_BASICMANAGER::addMsg(LOGIN_SUCCESS, "로그인에 성공하였습니다.");
	_BASICMANAGER::addMsg(SIGNIN_SUCCESS, "회원가입에 성공하였습니다.");
	LogManager::LogPrint("LoginManager Created");
	getJoinm();
	LogManager::LogPrint("JOINMEBER GET COMPLETED");

}

LoginManager::~LoginManager()
{
	join_member.clear();
	loggedin.clear();
	LogManager::LogPrint("LoginManager Deleted");
}

void LoginManager::CreateInstance()
{
	if(instance==nullptr)instance = new LoginManager();
}

LoginManager* LoginManager::GetInstance()
{
	return instance;
}

void LoginManager::ClearInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void LoginManager::getJoinm()
{
	int a = 0;
	LoginInfo** tmp = DBManager::getinstance()->getJoinedMember(&a);
	for (int i = 0; i < a; i++)
	{
		join_member.push_back(tmp[i]);
	}
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

	return tmp->uuid;
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
					return tmp->uuid;
				}
			}
			break;
		}
	}

	return NOMATCH;
}

void LoginManager::LogOut(int uuid)
{
	std::deque<int>::iterator iter;
	for (iter = loggedin.begin(); iter != loggedin.end(); iter++)
	{
		if ((*iter) == uuid)
		{
			break;
		}
	}
	if (iter != loggedin.end())loggedin.erase(iter);
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

void LoginManager::insideProcess(ISession* is, int* managerNo, char* data, int* datasize)
{
	int l = 0, p = 0, num = 0, e = 0;
	char id[20] = "", pw[20] = "", msg[500] = "";
	unpackPackit(data, &l, &p, &num, id, pw, msg, &e);
	LogManager::LogPrint("LoginManager Data\nLGSI - %d\nLGSIP - %d",l,p);
	if (l == LGIN && p == LOGIN_TRY)
	{
		*managerNo = getNo();
		e = tryLogin(id, pw);
		if (e > 0)//uuid가 리턴됨
		{
			is->setUUID(e);
			getMsg(msg, LOGIN_SUCCESS);
			int k = packPackit(data, LGIN, LOGIN_SUCCESS, e, nullptr, nullptr, msg, NOERR);
			*datasize = k;
		}
		else
		{
			getMsg(msg, e);
			int k = packPackit(data, LGIN, LOGIN_ERROR, -1, nullptr, nullptr, msg, e);
			*datasize = k;
		}
	}
	else if (l == SIGNIN && p == SIGNIN_TRY)
	{
		*managerNo = getNo();
		e = trySignin(id, pw);
		if (e > 0)//uuid가 리턴됨
		{
			is->setUUID(e);
			getMsg(msg, SIGNIN_SUCCESS);
			int k = packPackit(data, SIGNIN, SIGNIN_SUCCESS, e, nullptr, nullptr, msg, NOERR);
			*datasize = k;
		}
		else
		{
			getMsg(msg, e);
			int k = packPackit(data, SIGNIN, SIGNIN_ERROR, -1, nullptr, nullptr, msg, e);
			*datasize = k;
		}
	}

}

void LoginManager::outsideProcess(ISession* is, int* managerNo, char* data, int* datasize)
{
	if (*managerNo == LOBBY)
	{
		int sel = 0;
		int s = 0;
		memcpy(&sel, data, sizeof(int));
		LogManager::LogPrint("LoginManager Data\nTransferred From  - %d\nLGSI - %d\n",*managerNo,sel );

		if (sel == 1)
		{
			char msg[100]="";
			getMsg(msg, LOGIN_MSG);
			*managerNo = getNo();
			s = packPackit(data, LGIN, LOGIN_MSG, -1, nullptr, nullptr, msg, NOERR);
			*datasize = s;
		}
		else if (sel == 2)
		{
			*managerNo = getNo();
			char msg[100] = "";
			getMsg(msg, SIGNIN_MSG);
			*managerNo = getNo();
			s = packPackit(data, SIGNIN, SIGNIN_MSG, -1, nullptr, nullptr, msg, NOERR);
			*datasize = s;
		}
	}
	
}
