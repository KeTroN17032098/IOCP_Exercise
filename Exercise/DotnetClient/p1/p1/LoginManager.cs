using System;

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

        private int PackData(ref byte[] data,in LORS l,in Byte p,in int num,in string id,in string pw,in string msg,in LOGINMANAGER_ERR e,in Int64 modulus,in Int64 exponent)
        {
            int size = 0;
            UInt32 protocol = 0;
            ProtocolManager.GetInstance().AddMain(ref protocol, (byte)l);
            ProtocolManager.GetInstance().AddSub(ref protocol,p);
            Byte tmpd = 0;
            if (l == LORS.SELCTION)
            {
                if (p == (int)SELECTION.REQ_SELECTIONMSG)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.MSG | (int)LOGINMANAGER_DETAIL.PUBLICKEY);
                }
                else if (p == (int)SELECTION.SELECTION_MSG)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.MSG | (int)LOGINMANAGER_DETAIL.PUBLICKEY);
                }
                else if (p == (int)SELECTION.SELECTION_SET)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.NUMBER);
                }
            }
            else if (l == LORS.LOGIN)
            {
                if (p == (Byte)LOGIN_PROTOCOL.LOGIN_MSG)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.MSG);
                }
                else if (p == (int)LOGIN_PROTOCOL.LOGIN_TRY)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.IDPW);
                }
                else if (p == (int)LOGIN_PROTOCOL.LOGIN_SUCCESS)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.NUMBER | (int)LOGINMANAGER_DETAIL.MSG);
                }
                else if (p == (int)LOGIN_PROTOCOL.LOGIN_ERROR)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.ERRCODE | (int)LOGINMANAGER_DETAIL.MSG);
                }
            }
            else if (l == LORS.SIGNIN)
            {
                if (p == (int)SIGNIN_PROTOCOL.SIGNIN_MSG)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.MSG);
                }
                else if (p == (int)SIGNIN_PROTOCOL.SIGNIN_TRY)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.IDPW);
                }
                else if (p == (int)SIGNIN_PROTOCOL.SIGNIN_SUCCESS)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.NUMBER | (int)LOGINMANAGER_DETAIL.MSG);
                }
                else if (p == (int)SIGNIN_PROTOCOL.SIGNIN_ERROR)
                {
                    tmpd = ((int)LOGINMANAGER_DETAIL.ERRCODE | (int)LOGINMANAGER_DETAIL.MSG);
                }
            }
            ProtocolManager.GetInstance().AddDetail(ref protocol, tmpd);
            Buffer.BlockCopy(BitConverter.GetBytes(protocol), 0, data, 0, sizeof(UInt32));
            size += sizeof(UInt32);
            if ((tmpd & (byte)LOGINMANAGER_DETAIL.NUMBER)!=0)
            {
                Buffer.BlockCopy(BitConverter.GetBytes(num), 0, data, size, sizeof(int));
                size += sizeof(int);
            }
            if ((tmpd & (int)LOGINMANAGER_DETAIL.MSG)!=0)
            {
                int msgsize = msg.Length;
                memcpy(Dest + size, &msgsize, sizeof(int));
                size += sizeof(int);
                memcpy(Dest + size, msg, msgsize);
                size += msgsize;
            }
            if ((tmpd & (int)LOGINMANAGER_DETAIL.ERRCODE)!=0)
            {
                memcpy(Dest + size, &e, sizeof(int));
                size += sizeof(int);
            }
            if ((tmpd & (int)LOGINMANAGER_DETAIL.IDPW)!=0)
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
            if ((tmpd & (int)LOGINMANAGER_DETAIL.PUBLICKEY) != 0)
            {
                int keysize = sizeof(public_key_class);
                memcpy(Dest + size, pub, keysize);
                size += keysize;
            }
            return size;
        }

    }
}
