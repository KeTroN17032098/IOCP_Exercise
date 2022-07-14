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
        private Queue<ReceivedData> RecvResult;
        private EventWaitHandle ewh2;
        bool DisconnectSelected;
        private ThreadManager(string ip,int port)
        {
            PacketManager.CreateInstance();
            Client.CreateInstace(ip, port);
            ewh2 = new EventWaitHandle(false, EventResetMode.ManualReset);
            DisconnectSelected = false;
            Thread t1 = new Thread(RecvThread);
            Thread t2 = new Thread(SendThread);
            t1.Start();
            t2.Start();

        }
        ~ThreadManager()
        {
            PacketManager.ClearInstance();
            Client.ClearInstance();
        }
        private static ThreadManager instance;
        public static void CreateInstance(string ip,int port) => instance = new ThreadManager(ip,port);

        public static void ClearInstance() => instance = null;

        public static ThreadManager GetInstance => instance;

        public void Send(STATUS s,ENCRYPTED e,int datasize,byte[] data)
        {
            int packetsize = PacketManager.GetInstance.PackPacket(s, e, data, datasize, packet: out byte[] tmp);
            SendQueue.Enqueue(tmp);
            ewh2.Set();
        }

        public ReceivedData GetReceivedData()
        {
            if (RecvResult.Count != 0) return RecvResult.Dequeue();
            else return null;
        }

        private static void RecvThread(Object obj)
        {
            while (true)
            {
                int tmpsize = Client.GetInstance.RecvPacket(vs:out byte[] tmp);
                if(PacketManager.GetInstance.UnpackPacket(tmp, tmpsize, out STATUS s, out ENCRYPTED e, out int datasize, out byte[] data)==false)
                {
                    continue;
                }
                if(s==STATUS.DISCONNECT)
                {
                    instance.DisconnectSelected = true;
                    instance.ewh2.Set();
                    break;
                }
                ReceivedData rd = new ReceivedData(data, s, e);
                instance.RecvResult.Enqueue(rd);
            }
            Console.WriteLine("RecvThread Closed");

        }
        private static void SendThread(Object obj)
        {
            while (true)
            {
                instance.ewh2.WaitOne();
                if (instance.DisconnectSelected) break;
                byte[] tmp = instance.SendQueue.Dequeue();
                if(Client.GetInstance.SendPacket(tmp, tmp.Length)==false)
                {
                    break;
                }
                instance.ewh2.Reset();
            }
            Console.WriteLine("SendThread Closed");
        }
    }
}
