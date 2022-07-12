using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace p1
{
    class MyBuffer
    {
        private byte[] buf;
        private int totalsize;
        private int completed;
        private int left;

        public MyBuffer(int size)
        {
            buf = new byte[size];
            totalsize = size;
            completed = 0;
            left = size;
        }

        public int GetStart => completed;
        public byte[] GetBuf => buf;

        public int GetTSize => totalsize;
        public int GetLeft => left;
        public int GetCompleted => completed;

        public void ByteTransfer(int Transferred)
        {
            left -= Transferred;
            completed += Transferred;
        }
    }
}
