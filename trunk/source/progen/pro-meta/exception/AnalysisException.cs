using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace progen.pro_meta.exception
{
    public class AnalysisException : SystemException
    {
        public AnalysisException(string msg, Exception e)
            : base(msg, e)
        {
        }

        public AnalysisException( string msg)
            : base( msg)
        {

        }
    }
}
