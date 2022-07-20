#include"LoginManager.h"

LoginManager* LoginManager::instance = nullptr;

LoginManager::LoginManager(): _BASICMANAGER((int)STATUS::LOGIN)
{
	join_member.clear();
	loggedin.clear();
	_BASICMANAGER::addMsg((int)LOGIN_PROTOCOL::LOGIN_MSG, L"�α����� �Ͻñ����� ���̵�� ��й�ȣ�� �Է��ϼ���.");
	_BASICMANAGER::addMsg((int)SIGNIN_PROTOCOL::SIGNIN_MSG, L"ȸ�������� �Ͻñ����� ���̵�� ��й�ȣ�� �Է��ϼ���.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::MULTIPLEID, L"���̵� �ߺ��Դϴ�.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::LENGTHLIMIT, L"���̵�� ��й�ȣ�� 20���̳��� �Է����ּ���.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::SMALLCHAR, L"�ʹ� ª���ϴ�.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::ALREADYLGIN, L"�̹� �α��εǾ� �ֽ��ϴ�.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::NOMATCH, L"���̵� Ȥ�� ��й�ȣ �����Դϴ�.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::UNKNOWNERR, L"�� �� ���� �����Դϴ�.");
	_BASICMANAGER::addMsg((int)LOGINMANAGER_ERR::NOERR, L"������ �����ϴ�.");
	_BASICMANAGER::addMsg((int)LOGIN_PROTOCOL::LOGIN_SUCCESS, L"�α��ο� �����Ͽ����ϴ�.");
	_BASICMANAGER::addMsg((int)SIGNIN_PROTOCOL::SIGNIN_SUCCESS, L"ȸ�����Կ� �����Ͽ����ϴ�.");
	_BASICMANAGER::addMsg((int)SELECTION::SELECTION_MSG, L"�α����ϰų� ȸ�������� �ϼž� �մϴ�.");
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

int LoginManager::trySignin(sentence* id, sentence* pw)
{
	if (wcslen(id) == 0 || wcslen(pw) == 0)return (int)LOGINMANAGER_ERR::SMALLCHAR;
	if (wcslen(id) > 20 ||wcslen(pw) > 20)return (int)LOGINMANAGER_ERR::LENGTHLIMIT;
	for (int i = 0; i < join_member.size(); i++)
	{
		if (wcscmp(join_member[i]->ID, id) == 0)return (int)LOGINMANAGER_ERR::MULTIPLEID;
	}

	int uuid = DBManager::getinstance()->getNewUUID(id, pw);
	if (uuid == -1)return (int)LOGINMANAGER_ERR::UNKNOWNERR;

	LoginInfo* tmp = new LoginInfo();
	ZeroMemory(tmp, sizeof(LoginInfo));
	wcscpy(tmp->ID, id);
	wcscpy(tmp->PW, pw);
	tmp->uuid = uuid;

	join_member.push_back(tmp);

	return tmp->uuid;
}

int LoginManager::tryLogin(sentence* id,sentence* pw)
{
	LoginInfo* tmp = nullptr;
	std::deque<LoginInfo*>::iterator iter;
	for (iter = join_member.begin(); iter != join_member.end(); iter++)
	{
		if (wcscmp(id, (*iter)->ID) == 0)
		{
			if (wcscmp(pw, (*iter)->PW) == 0)
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



int LoginManager::packPackit(char* Dest, int l, int p, int num, sentence* id, sentence* pw, sentence* msg, int e, public_key_class* pub)
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
		int msgsize = wcslen(msg) * sizeof(sentence);
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
		int msgsize = wcslen(id)*sizeof(sentence);
		memcpy(Dest + size, &msgsize, sizeof(int));
		size += sizeof(int);
		memcpy(Dest + size, id, msgsize);
		size += msgsize;
		msgsize = wcslen(pw) * sizeof(sentence);
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

void LoginManager::unpackPackit(char* Data, int* l, int* p, int* num, sentence* id, sentence* pw, sentence* msg, int* e,public_key_class* pub)
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
	sentence id[20] = L"", pw[20] = L"", msg[500] = L"";
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
			if (e > 0)//�α��� ����
			{
				is->setUUID(e);
				_BASICMANAGER::getMsg(msg, (int)LOGIN_PROTOCOL::LOGIN_SUCCESS);
				int k = packPackit(data, (int)LORS::LOGIN, (int)LOGIN_PROTOCOL::LOGIN_SUCCESS, e, nullptr, nullptr, msg, 0, nullptr);
				*datasize = k;
				*managerNo =(int) STATUS::LOBBY;
			}
			else//�α��� ����
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
			if (e > 0)//ȸ������ ����
			{
				is->setUUID(e);
				_BASICMANAGER::getMsg(msg, (int)SIGNIN_PROTOCOL::SIGNIN_SUCCESS);
				int k = packPackit(data, (int)LORS::SIGNIN, (int)SIGNIN_PROTOCOL::SIGNIN_SUCCESS, e, nullptr, nullptr, msg, 0, nullptr);
				*datasize = k;
				*managerNo = (int)STATUS::LOBBY;
			}
			else//�α��� ����
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
