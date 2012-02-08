using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;
using progen.pro_meta.exception;

namespace progen.pro_meta
{
    public class ProMetaHelper
    {
        private static ProMetaHelper instance_ = new ProMetaHelper();

        public static ProMetaHelper Instance()
        {
            return instance_;
        }

        #region property

        private List<GlobalStruct> global_struct_ = new List<GlobalStruct>();
        private List<MacroDefine> macro_define_ = new List<MacroDefine>();
        private List<EnumStruct> global_enum_ = new List<EnumStruct>();

        private List<ProRegion> pro_region_ = new List<ProRegion>();

        public List<ProRegion> ProRegions
        {
            get { return pro_region_; }
        }

        public bool HaveProtocols
        {
            get { return pro_region_.Count > 0; }
        }

        public List<GlobalStruct> GlobalStruct
        {
            get { return this.global_struct_; }
        }

        public bool HaveGlobalStruct
        {
            get { return global_struct_.Count > 0; }
        }

        public List<EnumStruct> GlobalEnum
        {
            get { return this.global_enum_; }
        }

        public bool HaveGlobalEnum
        {
            get { return global_enum_.Count > 0; }
        }

        public List<MacroDefine> MacroDefines
        {
            get { return this.macro_define_; }
        }

        public bool HaveMacroDefines
        {
            get { return macro_define_.Count > 0; }
        }

        #endregion

        #region interface function

        public ProRegion GetProRegionByName( string name)
        {
            for( int ind =0; ind < pro_region_.Count; ++ind)
            {
                if (pro_region_[ind].RegionName == name)
                    return pro_region_[ind];
            }

            return null;
        }

        public void AddProRegion( ProRegion pr)
        {
            if( GetProRegionByName( pr.RegionName) != null)
                throw new AnalysisException(String.Format("redefine {0} region name", pr.RegionName));

            pro_region_.Add(pr);
        }

        public GlobalStruct GetGlobalStructByName( string name)
        {
            for (int ind = 0; ind < global_struct_.Count; ++ind)
            {
                if (global_struct_[ind].Name == name)
                    return global_struct_[ind];
            }

            return null;
        }

        public void AddGlobalStruct( GlobalStruct gs)
        {
            if (GetGlobalStructByName(gs.Name) != null)
                throw new AnalysisException(String.Format("redefine {0} global struct", gs.Name));

            global_struct_.Add(gs);
        }

        public MacroDefine GetMacroDefineByName( string name)
        {
            for (int ind = 0; ind < macro_define_.Count; ++ind)
            {
                if (macro_define_[ind].Name == name)
                    return macro_define_[ind];
            }

            return null;
        }

        public void AddMacroDefine( MacroDefine md)
        {
            if( GetMacroDefineByName( md.Name) != null)
                throw new AnalysisException(String.Format("redefine {0} macro define", md.Name));

            macro_define_.Add(md);
        }

        public void AddGlobalEnum(EnumStruct es)
        {
            if (GetGlobalStructByName(es.Name) != null)
                throw new AnalysisException(String.Format("redefine {0} global enum", es.Name));

            global_enum_.Add(es);
        }

        public EnumStruct GetGlobalEnumByName(string name)
        {
            for (int ind = 0; ind < global_enum_.Count; ++ind)
            {
                if (global_enum_[ind].Name == name)
                    return global_enum_[ind];
            }

            return null;
        }

        #endregion

        #region load pro meta defined

        private ProMetaHelper()
        {
        }

        public void InitMeta()
        {
            FileUtil.GetCurPath();

            string xmlpath = FileUtil.GetPath(FileUtil.GetCurPath(), ProMetaConst.XML_SUBDIR);

            List<string> buildorder = new List<string>();

            {
                string ord = FileUtil.GetFile(xmlpath, ProMetaConst.XML_ANALYSEORDER);
                if (FileUtil.IsFileExist(ord))
                {
                    XmlUtil xu = new XmlUtil(ord);
                    List<XmlElement> mls =XmlUtil.GetChildrenElement(xu.Root, "xml");
                    foreach( XmlElement e1 in mls)
                    {
                        buildorder.Add(XmlUtil.GetElementText(e1));
                    }
                }
            }

            List<ProRegion> pros = new List<ProRegion>();

            {
                List<string> allxml = FileUtil.ListFilesOfDir(xmlpath, "*.xml");
                foreach (string xml in allxml)
                {
                    string p = FileUtil.GetFile(xmlpath, xml);

                    try
                    {
                        XmlUtil xu = new XmlUtil(p);

                        ProRegion pr = new ProRegion(xu);
                        pr.RegionName = FileUtil.GetFileNameWithoutExtension(p);
                        pros.Add(pr);
                    }
                    catch (Exception se)
                    {
                        LoggerHelper.GenError(String.Format("{0} analyse failed............(X)", xml));
                        throw se;
                    }
                }
            }

            //sort
            for( int ind =buildorder.Count - 1; ind >= 0; --ind)
            {
                foreach( ProRegion r in pros)
                {
                    if( r.RegionName == buildorder[ind])
                    {
                        pros.Remove(r);
                        pros.Insert(0, r);
                        break;
                    }
                }
            }

            //load
            foreach( ProRegion r in pros)
            {
                try
                {
                    r.AnalyseProRegion();
                    this.AddProRegion(r);
                }
                catch (Exception se)
                {
                    LoggerHelper.GenError(String.Format("{0} analyse failed............(X)", r.Xml));
                    throw se;
                }
            }
        }

        #endregion
    }
}
