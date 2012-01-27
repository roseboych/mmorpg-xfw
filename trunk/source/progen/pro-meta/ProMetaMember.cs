using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class ProMetaMember : StructMember
    {
        private LocalStruct ref_local_struct_ = null;
        private GlobalStruct ref_global_struct_ = null;

        private ProMeta owner_ = null;

        public ProMetaMember( ProMeta o)
        {
            this.owner_ =o;
        }

        public LocalStruct RefLocalStruct
        {
            get { return this.ref_local_struct_; }
        }

        public GlobalStruct RefGlobalStruct
        {
            get { return this.ref_global_struct_; }
        }

        public ProMeta Owner
        {
            get { return this.owner_; }
        }

        #region IMetaLoader implementation

        public override void LoadFromXml(XmlElement e)
        {
            base.LoadFromXml(e);

            if( Type == ProMetaConst.DATATYPE_LOCALSTRUCT)
            {
                string lstr = XmlUtil.GetAttrStrVal(e, "struct-name", "");
                if (lstr == "")
                    throw new exception.AnalysisException(String.Format("pro-meta/pro[{0}]/members/member type=local-struct must define struct-name property", owner_.Name));

                ref_local_struct_ =owner_.GetLocalStructByName(lstr);
                if( ref_local_struct_ == null)
                    throw new exception.AnalysisException(String.Format("pro-meta/pro[{0}]/members/member type=local-struct define struct-name[{1}] not exist", owner_.Name, lstr));
            }

            if (Type == ProMetaConst.DATATYPE_GLOBALSTRUCT)
            {
                string lstr = XmlUtil.GetAttrStrVal(e, "struct-name", "");
                if (lstr == "")
                    throw new exception.AnalysisException(String.Format("pro-meta/pro[{0}]/members/member type=global-struct must define struct-name property", owner_.Name));

                ref_global_struct_ = ProMetaHelper.Instance().GetGlobalStructByName( lstr);
                if (ref_global_struct_ == null)
                    throw new exception.AnalysisException(String.Format("pro-meta/pro[{0}]/members/member type=global-struct define struct-name[{1}] not exist", owner_.Name, lstr));
            }
        }

        #endregion
    }
}
