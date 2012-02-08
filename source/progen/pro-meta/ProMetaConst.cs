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
        public const string DATATYPE_INT16 = "int16";
        public const string DATATYPE_INT32 = "int32";
        public const string DATATYPE_INT64 = "int64";
        public const string DATATYPE_UINT32 = "uint32";
        public const string DATATYPE_FLOAT32 = "float32";
        public const string DATATYPE_TIMESTAMP = "timestamp";
        public const string DATATYPE_STATICSTRING   = "staticstring";
        public const string DATATYPE_LOCALSTRUCT = "local-struct";
        public const string DATATYPE_GLOBALSTRUCT = "global-struct";
        public const string DATATYPE_SERIALIZEOBJ = "serializeobj";

        public static Hashtable datatypes_ = new Hashtable();

        static ProMetaConst()
        {
            //regist support data types
            datatypes_.Add(DATATYPE_INT8, DATATYPE_INT8);
            datatypes_.Add(DATATYPE_INT16, DATATYPE_INT16);
            datatypes_.Add(DATATYPE_INT32, DATATYPE_INT32);
            datatypes_.Add(DATATYPE_INT64, DATATYPE_INT64);
            datatypes_.Add(DATATYPE_UINT32, DATATYPE_UINT32);
            datatypes_.Add(DATATYPE_FLOAT32, DATATYPE_FLOAT32);
            datatypes_.Add(DATATYPE_TIMESTAMP, DATATYPE_TIMESTAMP);
            datatypes_.Add(DATATYPE_STATICSTRING, DATATYPE_STATICSTRING);
            datatypes_.Add(DATATYPE_LOCALSTRUCT, DATATYPE_LOCALSTRUCT);
            datatypes_.Add(DATATYPE_GLOBALSTRUCT, DATATYPE_GLOBALSTRUCT);
            datatypes_.Add(DATATYPE_SERIALIZEOBJ, DATATYPE_SERIALIZEOBJ);
        }

        #region common function

        public static bool IsValidDataType( string dt)
        {
            return datatypes_.ContainsKey(dt);
        }

        #endregion

    }
}
