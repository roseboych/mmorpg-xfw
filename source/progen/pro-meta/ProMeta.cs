using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class ProMeta : IMetaLoader
    {
        #region property

        private string name_ = "";
        private string pro_iid_ = "";
        private int pro_iid_v_ = ProMetaConst.INVALIDE_VALUE;
        private bool onlyserver_ = false;
        private string desc_ = "";
        private string construct_block_c_ = "";
        private string construct_block_java_ = "";
        private string destruct_block_c_ = "";
        private string destruct_block_java_ = "";
        private string custom_function_c_ = "";
        private string custom_function_java_ = "";

        private List<LocalStruct> local_structs_ = new List<LocalStruct>();
        private List<ProMetaMember> members_ = new List<ProMetaMember>();

        private ProRegion owner_ = null;

        public string Name
        {
            get { return this.name_; }
        }

        public string ProID
        {
            get { return this.pro_iid_; }
        }

        public int ProIDValue
        {
            get { return this.pro_iid_v_; }
        }

        public bool OnlyServer
        {
            get { return this.onlyserver_; }
        }

        public string Desc
        {
            get { return this.desc_; }
        }

        public string ConstructBlockC
        {
            get { return this.construct_block_c_; }
        }

        public string ConstructBlockJava
        {
            get { return this.construct_block_java_; }
        }

        public string DestructBlockC
        {
            get { return this.destruct_block_c_; }
        }

        public string DestructBlockJava
        {
            get { return this.destruct_block_java_; }
        }

        public string CustomFunctionC
        {
            get { return this.custom_function_c_; }
        }

        public string CustomFunctionJava
        {
            get { return this.custom_function_java_; }
        }

        public bool DefinedLocalStruct
        {
            get { return this.local_structs_.Count > 0; }
        }

        public List<LocalStruct> LocalStructs
        {
            get { return this.local_structs_; }
        }

        public List<ProMetaMember> Members
        {
            get { return this.members_; }
        }

        public ProRegion Owner
        {
            get { return this.owner_; }
        }

        #endregion

        public ProMeta( ProRegion o, int proidv)
        {
            this.pro_iid_v_ =proidv;
            this.owner_ = o;
        }

        #region analyse function

        public LocalStruct GetLocalStructByName(string name)
        {
            for (int ind = 0; ind < local_structs_.Count; ++ind)
            {
                if (local_structs_[ind].Name == name)
                    return local_structs_[ind];
            }

            return null;
        }

        public void AddLocalStruct(LocalStruct ls)
        {
            if (GetLocalStructByName(ls.Name) != null)
                throw new exception.AnalysisException(String.Format("redefine {0} local struct on the protocol[{1}]", ls.Name, name_));

            local_structs_.Add(ls);
        }

        protected void AddMember(ProMetaMember md)
        {
            if (GetMemberByName(md.Name) != null)
                throw new exception.AnalysisException(String.Format("redefine pro-meta/pro/member[{0}] node name[{1}]", name_, md.Name));

            members_.Add(md);
        }

        protected ProMetaMember GetMemberByName(string name)
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
            name_ = XmlUtil.GetAttrStrVal(e, "name", "");
            if (name_ == "")
                throw new exception.AnalysisException(String.Format("pro-region/pro-meta/pro node must define name property"));

            pro_iid_ = XmlUtil.GetAttrStrVal(e, "id", "");
            if( pro_iid_ == "")
                throw new exception.AnalysisException(String.Format("pro-region/pro-meta/pro node must define id property"));

            onlyserver_ = XmlUtil.GetAttrBoolValByYesNo(e, "onlyserver", false);
            XmlElement desc = XmlUtil.GetFirstChildren(e, "desc");
            if( desc != null)
                desc_ = XmlUtil.GetCDATAContent(desc);

            XmlElement cb =XmlUtil.GetFirstChildren(e, "construct-block");
            if( cb != null)
            {
                XmlElement cbc = XmlUtil.GetFirstChildren(cb, "c");
                if (cbc != null)
                    construct_block_c_ = XmlUtil.GetCDATAContent(cbc);

                XmlElement cbj = XmlUtil.GetFirstChildren(cb, "java");
                if (cbj != null)
                    construct_block_java_ = XmlUtil.GetCDATAContent(cbj);
            }

            XmlElement db = XmlUtil.GetFirstChildren(e, "destruct-block");
            if (db != null)
            {
                XmlElement cbc = XmlUtil.GetFirstChildren(db, "c");
                if (cbc != null)
                    destruct_block_c_ = XmlUtil.GetCDATAContent(cbc);

                XmlElement cbj = XmlUtil.GetFirstChildren(db, "java");
                if (cbj != null)
                    destruct_block_java_ = XmlUtil.GetCDATAContent(cbj);
            }

            XmlElement cf = XmlUtil.GetFirstChildren(e, "custom-function");
            if (cf != null)
            {
                XmlElement cbc = XmlUtil.GetFirstChildren(cf, "c");
                if (cbc != null)
                    custom_function_c_ = XmlUtil.GetCDATAContent(cbc);

                XmlElement cbj = XmlUtil.GetFirstChildren(cf, "java");
                if (cbj != null)
                    custom_function_java_ = XmlUtil.GetCDATAContent(cbj);
            }

            //local-struct
            List<XmlElement> lss = XmlUtil.GetChildrenElement(e, "local-struct");
            foreach( XmlElement e1 in lss)
            {
                string name = XmlUtil.GetAttrStrVal(e1, "name", "");
                if (name == "")
                    throw new exception.AnalysisException(String.Format("pro-meta/pro/local-struct node must define a name"));
                LocalStruct ls = new LocalStruct(name, this);

                ls.LoadFromXml(e1);

                this.AddLocalStruct(ls);
            }

            //members
            XmlElement ms = XmlUtil.GetFirstChildren(e, "members");
            if( ms != null)
            {
                List<XmlElement> mm = XmlUtil.GetChildrenElement(ms, "member");
                foreach (XmlElement e1 in mm)
                {
                    ProMetaMember md = new ProMetaMember( this);
                    md.LoadFromXml(e1);

                    this.AddMember(md);
                }
            }
        }

        #endregion
    }
}
