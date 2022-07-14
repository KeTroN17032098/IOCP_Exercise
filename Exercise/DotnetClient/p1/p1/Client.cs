using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.IO;

namespace p1
{
 

    class Client
    {
        private static Client instance;
        public static Client GetInstance => instance;
        public static void CreateInstace(string ip, int port) => instance = new Client(ip, port);
        public static void ClearInstance() => instance = null;

        private TcpClient tc;
        private NetworkStream stream;

        private byte[] recvbuf,sendbuf;


        private Client(string ip,int port)
        {
            tc = new TcpClient(ip, port);
            stream = tc.GetStream();
            recvbuf = new byte[8192];
            sendbuf = new byte[8192];
        }

        public int RecvPacket(out byte[] vs)
        {
            /*
             NetworkStream으로 부터 패킷을 읽어온다. 소켓이 닫혔을 경우 0을, 에러발생시 -1을 리턴
             */
            System.Array.Clear(recvbuf, 0, 8192);
            byte[] sizearray = new byte[4];
            int size = 0;
            try 
            {
                int retval = stream.Read(sizearray, 0, 4);
                if(retval==0)
                {
                    Console.WriteLine("Socket Disconnected");
                    vs = null;
                    return 0;
                }
                size = BitConverter.ToInt32(sizearray, 0);
                retval = stream.Read(recvbuf, 4,size-4);
                if (retval == 0)
                {
                    Console.WriteLine("Socket Disconnected");
                    vs = null;
                    return 0;
                }
                Buffer.BlockCopy(sizearray, 0, recvbuf, 0, 4);
                byte[] tmp = new byte[size];
                Array.Copy(recvbuf,tmp, size);
                vs = tmp;
                return size;

            }
            catch(IOException e)
            {
                Console.WriteLine(e.Message);
                vs = null;
                return -1;
            }
        }

        public bool SendPacket(byte[] packet,int size)
        {
            System.Array.Clear(sendbuf, 0, 8192);
            Buffer.BlockCopy(packet, 0, sendbuf, 0, size);

            try { stream.Write(sendbuf, 0, size); }
            catch(IOException e)
            {
                Console.WriteLine(e.Message);
                return false;
            }
            return true;
        }

        ~Client()
        {
            stream.Close();
            tc.Close();
        }
    }
}
