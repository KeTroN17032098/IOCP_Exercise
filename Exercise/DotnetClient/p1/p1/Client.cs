using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Threading;

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
        private Client(string ip,int port)
        {
            tc = new TcpClient(ip, port);
            stream = tc.GetStream();
        }

        public void Recv(MyBuffer b)
        {
            int nbytes = 0;
            byte[] packetsize = new byte[4];
            while (nbytes == 4)
            {
                nbytes += stream.Read(packetsize, nbytes, 4-nbytes);
            }
            int psize = BitConverter.ToInt32(packetsize, 0);
            stream.Read(b.GetBuf, 4, psize - 4);
            Buffer.BlockCopy(packetsize, 0, b.GetBuf, 0, 4);
        }

        public void Send(MyBuffer b)
        {
            stream.Write(b.GetBuf, b.GetCompleted, b.GetLeft);
        }


        public static void RecvThread(object obj)
        {
            MyBuffer recvbuf = obj as MyBuffer;
            while (true)
            {
                instance.Recv(recvbuf);
                Thread.Sleep(10);
            }
        }


        ~Client()
        {
            stream.Close();
            tc.Close();
        }
    }
}
