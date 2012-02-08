using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class EnumItem : IMetaLoader
    {
        #region property

        private string name_ = "";
        private int value_ = ProMetaConst.INVALIDE_VALUE;
        private bool value_set_ = false;
        private bool ishex_ = false;
        private string value_hex_ = "";
        private string desc_ = "";

        public string Name
        {
            get { return this.name_; }
        }

        public string Desc
        {
            get { return this.desc_; }
        }

        public bool HasSetValue
        {
            get { return this.value_set_; }
        }

        public int Value
        {
            get { return this.value_; }
        }

        public bool IsHexValue
        {
            get { return this.ishex_; }
        }

        public string HexValue
        {
            get { return this.value_hex_; }
        }

        #endregion

        #region IMetaLoader implementation

        public virtual void LoadFromXml(XmlElement e)
        {
            name_ = XmlUtil.GetAttrStrVal(e, "name", "");
            if (name_ == "")
                throw new exception.AnalysisException(String.Format("global enum item node must defined a name property"));

            desc_ = XmlUtil.GetAttrStrVal(e, "desc", "");

            if( XmlUtil.GetAttrStrVal(e, "value", "") != "")
            {
                value_set_ = true;
                ishex_ = XmlUtil.GetAttrBoolValByYesNo(e, "hex", false);
                if (ishex_)
                    value_hex_ = XmlUtil.GetAttrStrVal(e, "value", "");
                else
                    value_ = XmlUtil.GetAttrIntVal(e, "value", ProMetaConst.INVALIDE_VALUE);
            }
        }

        #endregion
    }

    public class EnumStruct : IMetaLoader
    {
        #region property

        private string name_ = "";
        private string desc_ = "";
        private List<EnumItem> items_ = new List<EnumItem>();
        private ProRegion owner_ = null;

        public string Name
        {
            get { return this.name_; }
        }

        public string Desc
        {
            get { return this.desc_; }
        }

        public List<EnumItem> Items
        {
            get { return this.items_; }
        }

        public ProRegion Owner
        {
            get { return this.owner_; }
        }

        #endregion

        public EnumStruct(string name, ProRegion o)
        {
            this.name_ =name;
            this.owner_ =o;
        }

        #region analyse function

        public EnumItem GetEnumItemByName( string name)
        {
            foreach( EnumItem ei in items_)
            {
                if( ei.Name == name)
                    return ei;
            }

            return null;
        }

        public void AddEnumItem( EnumItem ei)
        {
            if (GetEnumItemByName(ei.Name) != null)
                throw new exception.AnalysisException(String.Format("enum[{0}] redefined a item name[{1}]", name_, ei.Name));

            items_.Add(ei);
        }

        #endregion

        #region IMetaLoader implementation

        public virtual void LoadFromXml(XmlElement e)
        {
            desc_ = XmlUtil.GetAttrStrVal(e, "desc", "");

            List<XmlElement> ms = XmlUtil.GetChildrenElement(e, "item");
            foreach (XmlElement e1 in ms)
            {
                EnumItem ei = new EnumItem();
                ei.LoadFromXml(e1);

                this.AddEnumItem(ei);
            }
        }

        #endregion
    }
}
