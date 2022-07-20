using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace p1
{
    class MyInfo
    {
        public int uuid
        {
            set; get;
        }

        private static MyInfo instance=null;
        public static MyInfo GetInstance => instance;
        public static void CreateInstance()
        {
            if (instance == null) instance = new MyInfo();
        }
        public static void ClearInstance()
        {
            if (instance != null) instance = null;
        }

        private MyInfo()
        {
            uuid = -1;
        }


    }
}
