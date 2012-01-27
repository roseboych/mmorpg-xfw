using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace progen.pro_meta
{
    public class ProMetaConst
    {
        public const string XML_SUBDIR = "xml-define";
        public const string XML_ANALYSEORDER = "analyse-order";

        public const int INVALIDE_VALUE = -1;

        //data type
        public const string DATATYPE_NOTSUPPORT = "invalide";
        public const string DATATYPE_INT8 = "int8";
        public const string DATATYPE_INT32 = "int32";
        public const string DATATYPE_INT64 = "int64";
        public const string DATATYPE_UINT8 = "uint8";
        public const string DATATYPE_UINT32 = "uint32";
        public const string DATATYPE_UINT64 = "uint64";
        public const string DATATYPE_ULONG32 = "ulong32";
        public const string DATATYPE_ULONG64 = "ulong64";
        public const string DATATYPE_BOOL = "bool";
        public const string DATATYPE_CHAR8 = "char8";
        public const string DATATYPE_UCHAR8 = "uchar8";
        public const string DATATYPE_FLOAT32 = "float32";
        public const string DATATYPE_TIMESTAMP = "timestamp";
        public const string DATATYPE_STATICSTRING   = "staticstring";
        public const string DATATYPE_LOCALSTRUCT = "local-struct";
        public const string DATATYPE_GLOBALSTRUCT = "global-struct";

        public static Hashtable datatypes_ = new Hashtable();

        static ProMetaConst()
        {
            //regist support data types
            datatypes_.Add(DATATYPE_INT8, DATATYPE_INT8);
            datatypes_.Add(DATATYPE_INT32, DATATYPE_INT32);
            datatypes_.Add(DATATYPE_INT64, DATATYPE_INT64);
            datatypes_.Add(DATATYPE_UINT8, DATATYPE_UINT8);
            datatypes_.Add(DATATYPE_UINT32, DATATYPE_UINT32);
            datatypes_.Add(DATATYPE_UINT64, DATATYPE_UINT64);
            datatypes_.Add(DATATYPE_ULONG32, DATATYPE_ULONG32);
            datatypes_.Add(DATATYPE_ULONG64, DATATYPE_ULONG64);
            datatypes_.Add(DATATYPE_BOOL, DATATYPE_BOOL);
            datatypes_.Add(DATATYPE_CHAR8, DATATYPE_CHAR8);
            datatypes_.Add(DATATYPE_UCHAR8, DATATYPE_UCHAR8);
            datatypes_.Add(DATATYPE_FLOAT32, DATATYPE_FLOAT32);
            datatypes_.Add(DATATYPE_TIMESTAMP, DATATYPE_TIMESTAMP);
            datatypes_.Add(DATATYPE_STATICSTRING, DATATYPE_STATICSTRING);
            datatypes_.Add(DATATYPE_LOCALSTRUCT, DATATYPE_LOCALSTRUCT);
            datatypes_.Add(DATATYPE_GLOBALSTRUCT, DATATYPE_GLOBALSTRUCT);

        }

        #region common function

        public static bool IsValidDataType( string dt)
        {
            return datatypes_.ContainsKey(dt);
        }

        #endregion

    }
}
