using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class GlobalStruct : IMetaLoader
    {
        private ProRegion owner_ =null;
        private string name_ = "";
        private string desc_ = "";
        private List<StructMember> members_ = new List<StructMember>();

        public GlobalStruct( string name, ProRegion o)
        {
            this.owner_ =o;
            this.name_ =name;
        }

        #region property

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

        public ProRegion Owner
        {
            get { return this.owner_; }
        }

        #endregion

        #region Analyse function

        protected void AddMember( StructMember md)
        {
            if (GetMemberByName(md.Name) != null)
                throw new exception.AnalysisException(String.Format("redefine global-struct/struct/member node name[{0}]", md.Name));

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

        public virtual void LoadFromXml( XmlElement e)
        {
            desc_ = XmlUtil.GetAttrStrVal(e, "desc", "");

            List<XmlElement> ms =XmlUtil.GetChildrenElement(e, "member");
            foreach( XmlElement e1 in ms)
            {
                StructMember md = new StructMember();
                md.LoadFromXml(e1);

                this.AddMember(md);
            }
        }

        #endregion
    }
}
