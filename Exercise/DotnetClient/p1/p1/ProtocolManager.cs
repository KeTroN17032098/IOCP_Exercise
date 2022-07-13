using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace p1
{
    using PROTOCOL = UInt32;
    using PROTOCOL_MAIN = Byte;
    using PROTOCOL_SUB = Byte;
    using PROTOCOL_DETAIL = UInt16;


    class ProtocolManager
    {
        private static ProtocolManager instance = null;

        private int mainsize = sizeof(PROTOCOL_MAIN) * 8;
        private int subsize = sizeof(PROTOCOL_SUB) * 8;
        private int detailsize = sizeof(PROTOCOL_DETAIL) * 8;

        public static void CreateInstance()
        {
            if (instance == null) instance = new ProtocolManager();
        }
        public static void ClearInstance()
        {
            if (instance != null) instance = null;
        }
        public static ProtocolManager GetInstance()
        {
            return instance;
        }

        public void AddMain(ref PROTOCOL p, in PROTOCOL_MAIN m)
        {
            PROTOCOL tmp = Convert.ToUInt32(1 << (subsize + detailsize));
            p = p % tmp;
            p += tmp * m;
        }

        public PROTOCOL_MAIN GetMain(in PROTOCOL p)
        {
            PROTOCOL tmp = Convert.ToUInt32(1 << (subsize + detailsize));
            return Convert.ToByte(p / tmp);
        }

        public void AddSub(ref PROTOCOL p, in PROTOCOL_SUB s)
        {
            PROTOCOL tmp = Convert.ToUInt32(1 << subsize) - 1;
            tmp = tmp << detailsize;
            p = p & (~tmp);
            tmp = Convert.ToUInt32(s << detailsize);
            p += tmp;
        }
        public PROTOCOL_SUB GetSub(in PROTOCOL p)
        {
            PROTOCOL tmp = p >> detailsize;
            tmp = tmp % Convert.ToUInt32(1 << subsize);
            return Convert.ToByte(tmp);
        }
        public void AddDetail(ref PROTOCOL p, in PROTOCOL_DETAIL d)
        {
            p = (p >> detailsize) << detailsize;
            p += d;
        }
        public PROTOCOL_DETAIL GetDETAIL(in PROTOCOL p)
        {
            PROTOCOL tmp = p % Convert.ToUInt32(1 << detailsize);
            return Convert.ToUInt16(tmp);
        }

        public void PrintBit(in PROTOCOL p)
        {
            Console.WriteLine(p);
            Console.WriteLine(Convert.ToString(p, 2));
        }
    }
}
