using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace p1
{
    class ThreadManager
    {
        private Queue<byte[]> SendQueue;
        private byte[] RecvResult;
        private EventWaitHandle ewh1, ewh2;
        private ThreadManager(string ip,int port)
        {
            Client.CreateInstace(ip, port);
            ewh1 = new EventWaitHandle(false, EventResetMode.AutoReset);
            ewh2 = new EventWaitHandle(false, EventResetMode.AutoReset);

        }
        ~ThreadManager()
        {
            Client.ClearInstance();
        }
        private static ThreadManager instance;
        public static void CreateInstance(string ip,int port) => instance = new ThreadManager(ip,port);

        public static void ClearInstance() => instance = null;

        public static ThreadManager GetInstance => instance;

        public void Send(STATUS s,ENCRYPTED e,int datasize,byte[] data)
        {
            uint tmp = 0;
            ProtocolManager.GetInstance().AddMain(ref tmp, (byte)s);
            ProtocolManager.GetInstance().AddDetail(ref tmp, (byte)e);

        }

        private static void RecvThread(Object obj)
        {
            instance.ewh1.WaitOne();
        }
        private static void SendThread(Object obj)
        {
            instance.ewh2.WaitOne();
        }
    }
}
