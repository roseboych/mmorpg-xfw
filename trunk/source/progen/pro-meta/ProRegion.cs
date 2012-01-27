using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace progen.pro_meta
{
    public class ProRegion : IMetaLoader
    {
        #region property

        private string region_name_ = "";
        private int proid_begin_ = ProMetaConst.INVALIDE_VALUE;
        private int proid_end_ = ProMetaConst.INVALIDE_VALUE;
        private int cur_proid_ = ProMetaConst.INVALIDE_VALUE;

        private List<ProMeta> pros_ = new List<ProMeta>();

        private XmlUtil xml_ = null;

        public string RegionName
        {
            get { return this.region_name_; }
            set { this.region_name_ = value; }
        }

        public bool HaveProMeta
        {
            get { return this.pros_.Count > 0; }
        }

        public List<ProMeta> ProMetas
        {
            get { return this.pros_; }
        }

        public string Xml
        {
            get { return xml_.XmlFile; }
        }

        #endregion

        public ProRegion( XmlUtil xml)
        {
            this.xml_ =xml;
        }

        #region interface  function

        public ProMeta GetProMetaByName( string name)
        {
            foreach( ProMeta pro in pros_)
            {
                if( pro.Name == name)
                    return pro;
            }

            return null;
        }

        public ProMeta GetProMetaByProID(string id)
        {
            foreach (ProMeta pro in pros_)
            {
                if (pro.ProID == id)
                    return pro;
            }

            return null;
        }

        #endregion

        #region analysis function

        public void AnalyseProRegion()
        {
            LoadFromXml(xml_.Root);
        }

        protected int GetNextProId()
        {
            if (cur_proid_ == ProMetaConst.INVALIDE_VALUE)
                cur_proid_ = proid_begin_;
            else
            {
                ++cur_proid_;
                if (cur_proid_ > proid_end_)
                {
                    cur_proid_ = proid_end_;

                    return ProMetaConst.INVALIDE_VALUE;
                }
            }

            return cur_proid_;
        }

        protected void AnalyseGlobalStruct( XmlElement e)
        {
            string name =XmlUtil.GetAttrStrVal(e, "name", "");
            if (name == "")
                throw new exception.AnalysisException(String.Format("pro-region/global-struct/struct node must define a name"));
            GlobalStruct gs = new GlobalStruct( name, this);

            gs.LoadFromXml(e);

            ProMetaHelper.Instance().AddGlobalStruct(gs);
        }

        protected void AnalyseGlobalEnum( XmlElement e)
        {
            string name = XmlUtil.GetAttrStrVal(e, "name", "");
            if (name == "")
                throw new exception.AnalysisException(String.Format("pro-region/global-enum/enum node must define a name"));
            EnumStruct es = new EnumStruct(name, this);

            es.LoadFromXml(e);

            ProMetaHelper.Instance().AddGlobalEnum(es);
        }

        protected void AnalyseMacroDefine(XmlElement e)
        {
            string name = XmlUtil.GetAttrStrVal(e, "name", "");
            if (name == "")
                throw new exception.AnalysisException(String.Format("pro-region/macro-define/macro node must define a name"));
            MacroDefine md = new MacroDefine(name, this);

            md.LoadFromXml(e);

            ProMetaHelper.Instance().AddMacroDefine(md);
        }

        protected void AnalyseProDefine( XmlElement e)
        {
            ProMeta pro = new ProMeta(this, GetNextProId());
            pro.LoadFromXml(e);

            if (GetProMetaByName(pro.Name) != null || GetProMetaByProID(pro.ProID) != null)
                throw new exception.AnalysisException(String.Format("pro-region/pro-meta/pro node redefine name[{0}] or proid[{1}]", pro.Name, pro.ProID));

            pros_.Add(pro);
        }

        #endregion

        #region IMetaLoader implementation

        public virtual void LoadFromXml( XmlElement e)
        {
            //meta
            XmlElement meta =XmlUtil.GetFirstChildren(e, "meta");

            if( meta == null)
                throw new exception.AnalysisException(String.Format("messing pro-region/meta node"));

            string strproid =XmlUtil.GetAttrStrVal(meta, "proid", "");
            int[] proids = AppUtil.StrToIntArr(strproid, ",", ProMetaConst.INVALIDE_VALUE);
            if (proids.Length != 2 || proids[0] == ProMetaConst.INVALIDE_VALUE || proids[1] == ProMetaConst.INVALIDE_VALUE)
                throw new exception.AnalysisException(String.Format("pro-region/meta[proid:{0}] format error", strproid));

            proid_begin_ = proids[0];
            proid_end_ = proids[1];

            //global-struct
            XmlElement gstr = XmlUtil.GetFirstChildren(e, "global-struct");
            if (gstr != null)
            {
                List<XmlElement> procss =XmlUtil.GetChildrenElement( gstr, "struct");
                foreach (XmlElement e1 in procss)
                {
                    AnalyseGlobalStruct(e1);
                }
            }

            //global-enum
            XmlElement genum = XmlUtil.GetFirstChildren(e, "global-enum");
            if (genum != null)
            {
                List<XmlElement> procss = XmlUtil.GetChildrenElement(genum, "enum");
                foreach (XmlElement e1 in procss)
                {
                    AnalyseGlobalEnum(e1);
                }
            }

            //macro-define
            XmlElement mdef = XmlUtil.GetFirstChildren(e, "macro-define");
            if (mdef != null)
            {
                List<XmlElement> procss = XmlUtil.GetChildrenElement(mdef, "macro");
                foreach (XmlElement e1 in procss)
                {
                    AnalyseMacroDefine(e1);
                }
            }

            //pro-meta
            XmlElement pros = XmlUtil.GetFirstChildren(e, "pro-meta");
            if( pros != null)
            {
                List<XmlElement> pross =XmlUtil.GetChildrenElement(pros, "pro");
                foreach( XmlElement e1 in pross)
                {
                    AnalyseProDefine(e1);
                }
            }
        }

        #endregion
    }
}
