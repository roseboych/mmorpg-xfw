using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class StructMember : IMetaLoader
    {
        #region property

        private string name_ = "";
        private string type_ = ProMetaConst.DATATYPE_NOTSUPPORT;
        private MacroDefine macrolen_ =null;
        private int ilen_ = ProMetaConst.INVALIDE_VALUE;
        private string desc_ = "";
        //是否是stl数组
        private bool islist_ = false;
        //是否是基本类型静态数组
        private bool isbasearray_ = false;
        private string array_ref_ = null;

        public string Name
        {
            get { return this.name_; }
        }

        public string Type
        {
            get { return this.type_; }
        }

        public MacroDefine MacroLen
        {
            get { return this.macrolen_; }
        }

        public int ILen
        {
            get { return this.ilen_; }
        }

        public string Desc
        {
            get { return this.desc_; }
        }

        public bool IsList
        {
            get { return this.islist_; }
        }

        public bool IsBaseArray
        {
            get { return this.isbasearray_; }
        }

        public string ArrayRef
        {
            get { return this.array_ref_; }
        }

        #endregion

        #region IMetaLoader implementation

        public virtual void LoadFromXml(XmlElement e)
        {
            name_ =XmlUtil.GetAttrStrVal(e, "name", "");
            if (name_ == "")
                throw new exception.AnalysisException(String.Format("member node must defined a name property"));

            islist_ = XmlUtil.GetAttrBoolValByYesNo(e, "islist", false);

            isbasearray_ = XmlUtil.GetAttrBoolValByYesNo(e, "isarray", false);
            if( isbasearray_)
            {
                array_ref_ = XmlUtil.GetAttrStrVal(e, "array-ref", "");
                if (array_ref_ == "")
                    throw new exception.AnalysisException(String.Format("array-ref[{0}] of the member node defined not defined", array_ref_));
            }

            type_ = XmlUtil.GetAttrStrVal(e, "type", ProMetaConst.DATATYPE_NOTSUPPORT);
            if (!ProMetaConst.IsValidDataType(type_))
                throw new exception.AnalysisException(String.Format("type property[{0}] of the member node defined not supported by system", type_));

            //basic type array or static string , need to define macrolen or ilen
            if( type_ == ProMetaConst.DATATYPE_STATICSTRING || isbasearray_)
            {
                string mstr = XmlUtil.GetAttrStrVal(e, "macrolen", "");
                if (mstr != "")
                {
                    macrolen_ =ProMetaHelper.Instance().GetMacroDefineByName(mstr);
                    if( macrolen_ == null)
                        throw new exception.AnalysisException(String.Format("macrolen property[{0}] of the member node defined not defined", mstr));
                }

                ilen_ = XmlUtil.GetAttrIntVal(e, "ilen", ProMetaConst.INVALIDE_VALUE);

                if( ilen_ == ProMetaConst.INVALIDE_VALUE && macrolen_ == null)
                    throw new exception.AnalysisException(String.Format("the member[{0}] of the static string type must define macrolen or ilen property", name_));
            }

            desc_ = XmlUtil.GetAttrStrVal(e, "desc", "");
        }

        #endregion
    }
}
