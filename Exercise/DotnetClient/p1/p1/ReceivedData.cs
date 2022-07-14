using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace p1
{
    class ReceivedData
    {
        private byte[] data;
        private STATUS s;
        private ENCRYPTED isEncrypted;

        public ReceivedData(byte[] data,STATUS s,ENCRYPTED e)
        {
            this.data = new byte[data.Length];
            Buffer.BlockCopy(data, 0, this.data, 0, data.Length);
            this.s = s;
            this.isEncrypted = e;
        }

        public byte[] GetData => data;
        public STATUS GetSTATUS => s;
        public ENCRYPTED GetENCRYPTED => isEncrypted;
    }
}
