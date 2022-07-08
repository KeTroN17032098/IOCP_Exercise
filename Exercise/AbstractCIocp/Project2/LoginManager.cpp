#include"LoginManager.h"

LoginManager* LoginManager::instance = nullptr;

LoginManager::LoginManager(): _BASICMANAGER((int)STATUS::LOGIN)
{
	join_member.clear();
	loggedin.clear();
	_BASICMANAGER::addMsg((int)LOGIN_PROTOCOL::LOGIN_MSG, "로그인을 하시기위해 아이디와 비밀번호를 입력하세요.");
	_BASICMANAGER::addMsg((int)SIGNIN_PROTOCOL::SIGNIN_MSG, "회원가입을 하시기위해 아이디와 비밀번호를 입력하세요.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::MULTIPLEID, "아이디 중복입니다.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::LENGTHLIMIT, "아이디와 비밀번호는 20자이내로 입력해주세요.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::SMALLCHAR, "너무 짧습니다.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::ALREADYLGIN, "이미 로그인되어 있습니다.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::NOMATCH, "아이디 혹은 비밀번호 오류입니다.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::UNKNOWNERR, "알 수 없는 오류입니다.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::NOERR, "에러가 없습니다.");
	_BASICMANAGER::addMsg((int)LOGIN_PROTOCOL::LOGIN_SUCCESS, "로그인에 성공하였습니다.");
	_BASICMANAGER::addMsg((int)SIGNIN_PROTOCOL::SIGNIN_SUCCESS, "회원가입에 성공하였습니다.");
	_BASICMANAGER::addMsg((int)SELECTION::SELECTION_MSG, "로그인하거나 회원가입을 하셔야 합니다.");
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
	if (strlen(id) == 0 || strlen(pw) == 0)return (int)LOGINMANAGER_ERR::SMALLCHAR;
	if (strlen(id) > 20 || strlen(pw) > 20)return (int)LOGINMANAGER_ERR::LENGTHLIMIT;
	for (int i = 0; i < join_member.size(); i++)
	{
		if (strcmp(join_member[i]->ID, id) == 0)return (int)LOGINMANAGER_ERR::MULTIPLEID;
	}

	int uuid = DBManager::getinstance()->getNewUUID(id, pw);
	if (uuid == -1)return (int)LOGINMANAGER_ERR::UNKNOWNERR;

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
					return (int)LOGINMANAGER_ERR::ALREADYLGIN;
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

	return (int)LOGINMANAGER_ERR::NOMATCH;
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



int LoginManager::packPackit(char* Dest, int l, int p, int num, char* id, char* pw, char* msg, int e, public_key_class* pub)
{
	int size = 0;
	PROTOCOL tmp = 0;
	ProtocolManager::GetInstance()->AddMainPart(&tmp, l);
	ProtocolManager::GetInstance()->AddSubPart(&tmp, p);
	PROTOCOL_DETAIL tmpd = 0;
	if (l == (int)LORS::SELCTION)
	{
		if (p == (int)SELECTION::REQ_SELECTIONMSG)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::MSG|(int)LOGINMANAGER_DETAIL::PUBLICKEY);
		}
		else if (p == (int)SELECTION::SELECTION_MSG)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::MSG|(int)LOGINMANAGER_DETAIL::PUBLICKEY);
		}
		else if (p == (int)SELECTION::SELECTION_SET)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::NUMBER);
		}
	}
	else if (l == (int)LORS::LOGIN)
	{
		if (p == (int)LOGIN_PROTOCOL::LOGIN_MSG)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::MSG);
		}
		else if (p == (int)LOGIN_PROTOCOL::LOGIN_TRY)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::IDPW);
		}
		else if (p == (int)LOGIN_PROTOCOL::LOGIN_SUCCESS)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::NUMBER|(int)LOGINMANAGER_DETAIL::MSG);
		}
		else if (p == (int)LOGIN_PROTOCOL::LOGIN_ERROR)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::ERRCODE | (int)LOGINMANAGER_DETAIL::MSG);
		}
	}
	else if (l == (int)LORS::SIGNIN)
	{
		if (p == (int)SIGNIN_PROTOCOL::SIGNIN_MSG)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::MSG);
		}
		else if (p == (int)SIGNIN_PROTOCOL::SIGNIN_TRY)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::IDPW);
		}
		else if (p == (int)SIGNIN_PROTOCOL::SIGNIN_SUCCESS)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::NUMBER | (int)LOGINMANAGER_DETAIL::MSG);
		}
		else if (p == (int)SIGNIN_PROTOCOL::SIGNIN_ERROR)
		{
			tmpd = ((int)LOGINMANAGER_DETAIL::ERRCODE | (int)LOGINMANAGER_DETAIL::MSG);
		}
	}
	ProtocolManager::GetInstance()->AddDetailPart(&tmp, tmpd);
	memcpy(Dest + size, &tmp, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	if (tmpd & (int)LOGINMANAGER_DETAIL::NUMBER)
	{
		memcpy(Dest + size, &num, sizeof(int));
		size += sizeof(int);
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::MSG)
	{
		int msgsize = strlen(msg);
		memcpy(Dest + size, &msgsize, sizeof(int));
		size += sizeof(int);
		memcpy(Dest + size, msg, msgsize);
		size += msgsize;
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::ERRCODE)
	{
		memcpy(Dest + size, &e, sizeof(int));
		size += sizeof(int);
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::IDPW)
	{
		int msgsize = strlen(id);
		memcpy(Dest + size, &msgsize, sizeof(int));
		size += sizeof(int);
		memcpy(Dest + size, id, msgsize);
		size += msgsize;
		msgsize = strlen(pw);
		memcpy(Dest + size, &msgsize, sizeof(int));
		size += sizeof(int);
		memcpy(Dest + size, pw, msgsize);
		size += msgsize;
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::PUBLICKEY)
	{
		int keysize = sizeof(public_key_class);
		memcpy(Dest + size, pub, keysize);
		size += keysize;
	}
	return size;
}

void LoginManager::unpackPackit(char* Data, int* l, int* p, int* num, char* id, char* pw, char* msg, int* e,public_key_class* pub)
{
	int size = 0;
	PROTOCOL tmp = 0;
	memcpy(&tmp, Data, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	*l = ProtocolManager::GetInstance()->GetMainPart(tmp);
	*p= ProtocolManager::GetInstance()->GetSubPart(tmp);
	PROTOCOL_DETAIL tmpd = ProtocolManager::GetInstance()->GetDetailPart(tmp);
	if (tmpd & (int)LOGINMANAGER_DETAIL::NUMBER)
	{
		memcpy(&num, Data+size,sizeof(int));
		size += sizeof(int);
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::MSG)
	{
		int msgsize = 0;
		memcpy(&msgsize, Data + size, sizeof(int));
		size += sizeof(int);
		memcpy(msg,Data+size,msgsize);
		size += msgsize;
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::ERRCODE)
	{
		memcpy(e,Data+size,sizeof(int));
		size += sizeof(int);
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::IDPW)
	{
		int msgsize = 0;
		memcpy(&msgsize,Data + size,sizeof(int));
		size += sizeof(int);
		memcpy(id,Data+size,msgsize);
		size += msgsize;
		msgsize = 0;
		memcpy(&msgsize, Data + size, sizeof(int));
		size += sizeof(int);
		memcpy(pw, Data + size, msgsize);
		size += msgsize;
	}
	if (tmpd & (int)LOGINMANAGER_DETAIL::PUBLICKEY)
	{
		int keysize = sizeof(public_key_class);
		memcpy(pub,Data+size, keysize);
		size += keysize;
	}
}

void LoginManager::Process(ISession* is, int* managerNo, char* data, int* datasize)
{
	int l = 0, p = 0, num = 0, e = 0;
	char id[20] = "", pw[20] = "", msg[500] = "";
	public_key_class pub;
	unpackPackit(data, &l, &p, &num, id, pw, msg, &e,&pub);
	LogManager::LogPrint("LoginManager Data\nLGSI - %d\nLGSIP - %d",l,p);
	
	if (l == (int)LORS::SELCTION)
	{
		if (p == (int)SELECTION::REQ_SELECTIONMSG)
		{
			is->setPublicKey(pub.modulus, pub.exponent);
			_BASICMANAGER::getMsg(msg, (int)SELECTION::SELECTION_MSG);
			*datasize=packPackit(data, l, (int)SELECTION::SELECTION_MSG, 0, nullptr, nullptr, msg, 0, is->getPublicKey());
			*managerNo = _BASICMANAGER::getNo();
		}
		else if (p == (int)SELECTION::SELECTION_SET)
		{
			if (num == 1)
			{
				_BASICMANAGER::getMsg(msg, (int)LOGIN_PROTOCOL::LOGIN_MSG);
				*datasize = packPackit(data, (int)LORS::LOGIN, (int)LOGIN_PROTOCOL::LOGIN_MSG, 0, nullptr, nullptr, msg, 0,nullptr);
				*managerNo = _BASICMANAGER::getNo();
			}
			else if (num == 2)
			{
				_BASICMANAGER::getMsg(msg, (int)SIGNIN_PROTOCOL::SIGNIN_MSG);
				*datasize = packPackit(data, (int)LORS::SIGNIN, (int)LOGIN_PROTOCOL::LOGIN_MSG, 0, nullptr, nullptr, msg, 0, nullptr);
				*managerNo = _BASICMANAGER::getNo();
			}
		}
	}
	else if (l == (int)LORS::LOGIN)
	{
		if (p == (int)LOGIN_PROTOCOL::LOGIN_TRY)
		{
			e = tryLogin(id, pw);
			if (e > 0)//로그인 성공
			{
				is->setUUID(e);
				_BASICMANAGER::getMsg(msg, (int)LOGIN_PROTOCOL::LOGIN_SUCCESS);
				int k = packPackit(data, (int)LORS::LOGIN, (int)LOGIN_PROTOCOL::LOGIN_SUCCESS, e, nullptr, nullptr, msg, 0, nullptr);
				*datasize = k;
				*managerNo =(int) STATUS::LOBBY;
			}
			else//로그인 실패
			{
				_BASICMANAGER::getMsg(msg, e);
				int k = packPackit(data, (int)LORS::LOGIN, (int)LOGIN_PROTOCOL::LOGIN_ERROR, -1, nullptr, nullptr, msg, e, nullptr);
				*datasize = k;
				*managerNo = _BASICMANAGER::getNo();
			}
		}
		else
		{
			_BASICMANAGER::getMsg(msg, (int)LOGINMANAGER_ERR::UNKNOWNERR);
			int k = packPackit(data, (int)LORS::LOGIN, (int)LOGIN_PROTOCOL::LOGIN_ERROR, -1, nullptr, nullptr, msg, (int)LOGINMANAGER_ERR::UNKNOWNERR, nullptr);
			*datasize = k;
			*managerNo = _BASICMANAGER::getNo();
		}
	}
	else if (l == (int)LORS::SIGNIN)
	{
		if (p == (int)SIGNIN_PROTOCOL::SIGNIN_TRY)
		{
			e = trySignin(id, pw);
			if (e > 0)//회원가입 성공
			{
				is->setUUID(e);
				_BASICMANAGER::getMsg(msg, (int)SIGNIN_PROTOCOL::SIGNIN_SUCCESS);
				int k = packPackit(data, (int)LORS::SIGNIN, (int)SIGNIN_PROTOCOL::SIGNIN_SUCCESS, e, nullptr, nullptr, msg, 0, nullptr);
				*datasize = k;
				*managerNo = (int)STATUS::LOBBY;
			}
			else//로그인 실패
			{
				_BASICMANAGER::getMsg(msg, e);
				int k = packPackit(data, (int)LORS::SIGNIN, (int)SIGNIN_PROTOCOL::SIGNIN_ERROR, -1, nullptr, nullptr, msg, e, nullptr);
				*datasize = k;
				*managerNo = _BASICMANAGER::getNo();
			}
		}
		else
		{
			_BASICMANAGER::getMsg(msg, (int)LOGINMANAGER_ERR::UNKNOWNERR);
			int k = packPackit(data, (int)LORS::SIGNIN, (int)SIGNIN_PROTOCOL::SIGNIN_ERROR, -1, nullptr, nullptr, msg, (int)LOGINMANAGER_ERR::UNKNOWNERR, nullptr);
			*datasize = k;
			*managerNo = _BASICMANAGER::getNo();
		}
	}
}
