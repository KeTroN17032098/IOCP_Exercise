using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace p1
{

    class LoginManager
    {
        private enum LORS
        {
            SELCTION = 76,
            LOGIN,
            SIGNIN
        };
        private enum SELECTION
        {
            REQ_SELECTIONMSG = 32,
            SELECTION_MSG,
            SELECTION_SET
        };

        private enum LOGIN_PROTOCOL
        {
            LOGIN_MSG = 25,
            LOGIN_TRY,
            LOGIN_SUCCESS,
            LOGIN_ERROR
        };

        private enum SIGNIN_PROTOCOL
        {
            SIGNIN_MSG = 42,
            SIGNIN_TRY,
            SIGNIN_SUCCESS,
            SIGNIN_ERROR
        };

        private enum LOGINMANAGER_ERR
        {
            MULTIPLEID = -472,// 회원가입 아이디중복
            LENGTHLIMIT,// 최대 길이 초과
            SMALLCHAR,//공백란 있음
            ALREADYLGIN,//이미 로그인 됨
            NOMATCH,//아이디 혹은 비밀번호 오류
            UNKNOWNERR,//알수없는 오류
            NOERR//오류 없음
        };

        private enum LOGINMANAGER_DETAIL
        {
            NUMBER = 1,
            MSG = 1 << 1,
            ERRCODE = 1 << 2,
            IDPW = 1 << 3,
            PUBLICKEY = 1 << 4
        };

        public LoginManager()
        {

        }


    }
}
