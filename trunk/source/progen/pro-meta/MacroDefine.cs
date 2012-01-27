using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class MacroDefine : IMetaLoader
    {
        #region property

        private string name_ ="";
        private string value_ = "";
        private string type_ = ProMetaConst.DATATYPE_NOTSUPPORT;
        private string desc_ = "";

        private ProRegion owner_ =null;

        public string Name
        {
            get { return this.name_; }
        }

        public string Value
        {
            get { return this.value_; }
        }

        public string Type
        {
            get { return this.type_; }
        }

        public string Desc
        {
            get { return this.desc_; }
        }

        public ProRegion Owner
        {
            get { return this.owner_; }
        }

        #endregion

        public MacroDefine( string name, ProRegion o)
        {
            this.name_ =name;
            this.owner_ =o;
        }

        #region IMetaLoader implementation

        public virtual void LoadFromXml(XmlElement e)
        {
            value_ = XmlUtil.GetAttrStrVal(e, "value", "");
            if (value_ == "")
                throw new exception.AnalysisException(String.Format("macro-define/macro[{0}] node must define value property", name_));

            desc_ = XmlUtil.GetAttrStrVal(e, "desc", "");
            
            type_ = XmlUtil.GetAttrStrVal(e, "type", ProMetaConst.DATATYPE_INT32);
            if (type_ != ProMetaConst.DATATYPE_INT32)
                throw new exception.AnalysisException(String.Format("macro-define/macro[{0}] node only support int32 date type now", name_));
        }

        #endregion
    }
}
