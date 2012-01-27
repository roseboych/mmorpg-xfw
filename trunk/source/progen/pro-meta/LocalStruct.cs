using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class LocalStruct : IMetaLoader
    {
        #region property

        private string name_ = "";
        private string desc_ = "";
        private List<StructMember> members_ = new List<StructMember>();

        private ProMeta owner_ = null;

        public string Name
        {
            get { return this.name_; }
        }

        public string Desc
        {
            get { return this.desc_; }
        }

        public List<StructMember> Members
        {
            get { return this.members_; }
        }

        public ProMeta Owner
        {
            get { return this.owner_; }
        }

        #endregion

        public LocalStruct( string name, ProMeta o)
        {
            this.name_ =name;
            this.owner_ =o;
        }

        #region Analyse function

        protected void AddMember(StructMember md)
        {
            if (GetMemberByName(md.Name) != null)
                throw new exception.AnalysisException(String.Format("redefine pro-meta/pro/local-struct/member node name[{0}]", md.Name));

            members_.Add(md);
        }

        protected StructMember GetMemberByName(string name)
        {
            for (int ind = 0; ind < members_.Count; ++ind)
            {
                if (members_[ind].Name == name)
                    return members_[ind];
            }

            return null;
        }

        #endregion

        #region IMetaLoader implementation

        public virtual void LoadFromXml(XmlElement e)
        {
            desc_ = XmlUtil.GetAttrStrVal(e, "desc", "");

            List<XmlElement> ms = XmlUtil.GetChildrenElement(e, "member");
            foreach (XmlElement e1 in ms)
            {
                StructMember md = new StructMember();
                md.LoadFromXml(e1);

                this.AddMember(md);
            }
        }

        #endregion
    }
}
