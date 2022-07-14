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

	class PacketManager
    {
        private int recvno, sendno;
        private static PacketManager instance;
        public static void CreateInstance()
        {
            if (instance == null) instance = new PacketManager();
        }
        public static void ClearInstance()
        {
            if (instance != null) instance = null;
        }
        public static PacketManager GetInstance => instance;

        private PacketManager()
        {
            recvno = 1;
            sendno = 1;
            ProtocolManager.CreateInstance();
        }

        public int PackPacket(in STATUS s,in ENCRYPTED e,in byte[] data,in int datasize,out byte[] packet)
        {
            byte[] result = new byte[datasize + 2 * sizeof(Int32) + sizeof(UInt32)];
            int size = sizeof(Int32);
            UInt32 tmp = 0;
            ProtocolManager.GetInstance().AddMain(ref tmp, (byte)s);
            ProtocolManager.GetInstance().AddDetail(ref tmp, (byte)e);
            Buffer.BlockCopy(BitConverter.GetBytes(tmp), 0, result, size, sizeof(UInt32));
            size += sizeof(UInt32);
            Buffer.BlockCopy(BitConverter.GetBytes(sendno), 0, result, size, sizeof(Int32));
            sendno++;
            size += sizeof(Int32);
            Buffer.BlockCopy(data, 0, result, size, datasize);
            size += datasize;
            Buffer.BlockCopy(BitConverter.GetBytes(size), 0, result, 0, sizeof(int));
            packet = result;
            return size;
        }

        public bool UnpackPacket(in byte[] Packet,in int PacketSize,out STATUS s,out ENCRYPTED isEncrypted,out int DataSize,out byte[] Data)
        {
            byte[] result = new byte[PacketSize - (2 * sizeof(Int32) + sizeof(UInt32))];
            int size = sizeof(Int32);
            UInt32 tmp = BitConverter.ToUInt32(Packet, size);
            s = (STATUS)ProtocolManager.GetInstance().GetMain(tmp);
            isEncrypted = (ENCRYPTED)ProtocolManager.GetInstance().GetDETAIL(tmp);
            size += sizeof(UInt32);
            int pno = BitConverter.ToInt32(Packet, size);
            if (pno < recvno)
            {
                Data = null;
                DataSize = 0;
                return false;
            }
            else recvno = pno;
            size += sizeof(Int32);
            Buffer.BlockCopy(Packet, size, result, 0, PacketSize - size);
            Data = result;
            DataSize = PacketSize - size;
            return true;

        }
    }
}
