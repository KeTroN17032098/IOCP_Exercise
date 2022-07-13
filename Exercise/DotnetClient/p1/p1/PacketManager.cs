using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace p1
{
    enum STATUS
    {
        LOGIN = 1,
        LOBBY,
        DISCONNECT
    };
    enum ENCRYPTED
    {
        Y = 1,
        N
    };
    enum  LOBBY_PROTOCOL
    {
        REQ_LOBBYDATA=29,
		LOBBYDATA
    };
    enum  LOBBYDATA_PROTOCOL
    {
        PROFILE=54,

	};
    enum  LOBBYMANAGER_DETAIL
    {
        MSG=1,
	};
	enum  LORS
	{
		SELCTION=76,
		LOGIN,
		SIGNIN
	};

	enum  SELECTION
	{
		REQ_SELECTIONMSG=32,
		SELECTION_MSG,
		SELECTION_SET
	};

	enum  LOGIN_PROTOCOL
	{
		LOGIN_MSG = 25,
		LOGIN_TRY,
		LOGIN_SUCCESS,
		LOGIN_ERROR
	};

	enum  SIGNIN_PROTOCOL
	{
		SIGNIN_MSG = 42,
		SIGNIN_TRY,
		SIGNIN_SUCCESS,
		SIGNIN_ERROR
	};

	enum  LOGINMANAGER_ERR
	{
		MULTIPLEID = -472,// 회원가입 아이디중복
		LENGTHLIMIT,// 최대 길이 초과
		SMALLCHAR,//공백란 있음
		ALREADYLGIN,//이미 로그인 됨
		NOMATCH,//아이디 혹은 비밀번호 오류
		UNKNOWNERR,//알수없는 오류
		NOERR//오류 없음
	};

	enum  LOGINMANAGER_DETAIL
	{
		NUMBER=1,
		MSG=1<<1,
		ERRCODE=1<<2,
		IDPW=1<<3,
		PUBLICKEY=1<<4
	};
	class PacketManager
    {
		
    }
}
