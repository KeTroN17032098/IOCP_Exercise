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
        { }

        public 
    }
}
