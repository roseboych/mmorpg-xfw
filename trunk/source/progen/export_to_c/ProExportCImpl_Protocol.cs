using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using progen.pro_meta;

namespace progen.export_to_c
{
    public partial class ProExportCImpl : IProExport
    {
        protected void GenerateProtocols()
        {
            //generate a file include all protocol defined, that can be used by application
            GenProIncludesH();
            GenProIncludesCPP();

            //auto generate all pro region
            foreach (ProRegion pr in ProMetaHelper.Instance().ProRegions)
            {
                if (pr.HaveProMeta)
                {
                    GenProRegionId(pr);
                    GenProRegionH(pr);
                    GenProRegionCPP(pr);
                }
            }

            GenProFactory();
        }

        protected void GenProIncludesH()
        {
            try
            {
                MakeOutputFile(path_root_, "Pro_all.h");

                GenHFileHeaderB("Pro_all");

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);
                cur_stream_.WriteString(ExportCConst.IncludeFilesOfProtocol, 0);
                cur_stream_.WriteEmptyLine();

                if (!ProMetaHelper.Instance().HaveProtocols)
                    return;

                foreach (ProRegion pr in ProMetaHelper.Instance().ProRegions)
                {
                    if( !pr.HaveProMeta)
                        continue;

                    string ffname = String.Format("#include <prolib/gen/Pro_{0}_pid.h>", pr.RegionName);
                    cur_stream_.WriteString(ffname, 0);
                    ffname = String.Format("#include <prolib/gen/Pro_{0}.h>", pr.RegionName);
                    cur_stream_.WriteString(ffname, 0);
                }

                cur_stream_.WriteEmptyLine();
            }
            finally
            {
                GenHFileHeaderE("Pro_all");
                cur_stream_.FinishGenerate();
            }
        }

        protected void GenProIncludesCPP()
        {
            try
            {
                MakeOutputFile(path_root_, "Pro_all.cpp");

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);
                cur_stream_.WriteString(ExportCConst.IncludeFilesOfProtocol, 0);
                cur_stream_.WriteEmptyLine();

                if (!ProMetaHelper.Instance().HaveProtocols)
                    return;

                foreach (ProRegion pr in ProMetaHelper.Instance().ProRegions)
                {
                    if (!pr.HaveProMeta)
                        continue;

                    string ffname = String.Format("#include <prolib/gen/Pro_{0}.cpp>", pr.RegionName);
                    cur_stream_.WriteString(ffname, 0);
                }

                cur_stream_.WriteEmptyLine();
            }
            finally
            {
                cur_stream_.FinishGenerate();
            }
        }

        protected void GenProFactory()
        {
            try
            {
                MakeOutputFile(path_root_, "ProtocolFactory.cpp");

                cur_stream_.WriteString(
@"
#include <vector>
#include <prolib/ProtocolFactory.h>
#include <prolib/SerializeFactoryContainer.h>
#include <prolib/gen/Pro_all.h>
", 0);
                cur_stream_.WriteEmptyLine();

                cur_stream_.WriteString("BEGIN_PRO_FACTORY()", 0);
                cur_stream_.WriteEmptyLine();

                if (!ProMetaHelper.Instance().HaveProtocols)
                    return;

                foreach (ProRegion pr in ProMetaHelper.Instance().ProRegions)
                {
                    if (!pr.HaveProMeta)
                        continue;

                    cur_stream_.WriteComment(String.Format("Pro_{0}.h", pr.RegionName), 1);

                    foreach (ProMeta pm in pr.ProMetas)
                        cur_stream_.WriteString(String.Format("PRO_MAPPING( {0}, {1})", pm.ProID, pm.Name), 1);
                }

                cur_stream_.WriteEmptyLine();
            }
            finally
            {
                cur_stream_.WriteString("END_PRO_FACTORY()", 0);

                cur_stream_.FinishGenerate();
            }
        }

        protected void GenProRegionId( ProRegion pr)
        {
            string ffname = String.Format("Pro_{0}_pid", pr.RegionName);
            try
            {
                MakeOutputFile(path_root_, ffname + ".h");

                GenHFileHeaderB(ffname);

                cur_stream_.WriteEmptyLine();

                string rn =pr.RegionName;
                cur_stream_.WriteString(String.Format("enum TPRO_{0}", rn.ToUpper()), 0);
                cur_stream_.WriteString("{", 0);

                foreach (ProMeta pm in pr.ProMetas)
                {
                    if( !AppUtil.IsEmptyString( pm.Desc))
                        cur_stream_.WriteComment(pm.Desc, 1);

                    cur_stream_.WriteString(String.Format("{0} = {1}, ", pm.ProID, pm.ProIDValue), 1);
                }

            }
            finally
            {
                cur_stream_.WriteString("};", 0);

                GenHFileHeaderE(ffname);
                cur_stream_.FinishGenerate();
            }
        }

        protected void GenProRegionH( ProRegion pr)
        {
            string ffname = String.Format("Pro_{0}", pr.RegionName);
            try
            {
                MakeOutputFile(path_root_, ffname+".h");

                GenHFileHeaderB(ffname);

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);
                cur_stream_.WriteString(ExportCConst.IncludeFilesOfProtocol, 0);
                cur_stream_.WriteEmptyLine();

                //defined under one namespace
                cur_stream_.WriteString("PROTOCOL_NAMESPACE_BEGIN", 0);
                cur_stream_.WriteEmptyLine();

                foreach( ProMeta pm in pr.ProMetas)
                {
                    OutputHOfProtocol(pm);
                }

                cur_stream_.WriteString("PROTOCOL_NAMESPACE_END", 0);
            }
            finally
            {
                GenHFileHeaderE(ffname);
                cur_stream_.FinishGenerate();
            }
        }

        protected void GenProRegionCPP(ProRegion pr)
        {
            string ffname = String.Format("Pro_{0}", pr.RegionName);
            try
            {
                MakeOutputFile(path_root_, ffname + ".cpp");

                GenHFileHeaderB(ffname);

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);
                cur_stream_.WriteString(ExportCConst.IncludeFilesOfProtocol, 0);
                cur_stream_.WriteString(String.Format("#include <prolib/gen/Pro_{0}_pid.h>", pr.RegionName), 0);
                cur_stream_.WriteEmptyLine();

                //defined under one namespace
                cur_stream_.WriteString("PROTOCOL_NAMESPACE_BEGIN", 0);
                cur_stream_.WriteEmptyLine();

                foreach (ProMeta pm in pr.ProMetas)
                {
                    OutputCPPOfProtocol(pm);
                }

                cur_stream_.WriteString("PROTOCOL_NAMESPACE_END", 0);
            }
            finally
            {
                GenHFileHeaderE(ffname);
                cur_stream_.FinishGenerate();
            }
        }

        protected void OutputHOfProtocol( ProMeta pm)
        {
            if (!AppUtil.IsEmptyString(pm.Desc))
                cur_stream_.WriteClassDesc(pm.Name, pm.Desc);

            // class header
            cur_stream_.WriteString(String.Format("class {0} : public AppProtocol<{0}>", pm.Name), 0);
            cur_stream_.WriteString("{", 0);
            cur_stream_.WriteString(String.Format("typedef AppProtocol<{0}> inherit;", pm.Name), 1);
            cur_stream_.WriteEmptyLine();
            cur_stream_.WriteString("public:", 0);
            cur_stream_.WriteString(String.Format("{0}();", pm.Name), 1);
            cur_stream_.WriteEmptyLine();

            // defined local struct
            if( pm.DefinedLocalStruct)
            {
                cur_stream_.WriteString("public:", 0);
                cur_stream_.WriteEmptyLine();

                foreach( LocalStruct ls in pm.LocalStructs)
                {
                    OutputLocalStruct(ls);
                    cur_stream_.WriteEmptyLine();
                }
            }

            //-------------------------protected function-----------------------------
            cur_stream_.WriteString("protected:", 0);
            //BEGIN_LOAD_PRO

            //BEGIN_SAVE_PRO

            //BEGIN_CLONE_PRO


            //-------------------------defined members------------------------------------
            cur_stream_.WriteString("public:", 0);
            cur_stream_.WriteEmptyLine();

            foreach( ProMetaMember p in pm.Members)
            {
                if (!AppUtil.IsEmptyString(p.Desc))
                    cur_stream_.WriteComment(p.Desc, 1);
                cur_stream_.WriteString(String.Format("{0}    {1};", DataType2CType( p), p.Name), 1);
            }

            //------------------------------------------------------------------------
            cur_stream_.WriteString("};", 0);
            cur_stream_.WriteEmptyLine();
        }

        protected void OutputLocalStruct( LocalStruct ls)
        {
            if (!AppUtil.IsEmptyString(ls.Desc))
                cur_stream_.WriteComment(ls.Desc, 1);

            cur_stream_.WriteString(String.Format("typedef struct {0}", ls.Name), 1);
            cur_stream_.WriteString("{", 1);

            foreach(StructMember sm in ls.Members)
            {
                if (!AppUtil.IsEmptyString(sm.Desc))
                    cur_stream_.WriteComment(sm.Desc, 2);

                cur_stream_.WriteString(String.Format("{0}    {1};", DataType2CType( sm), sm.Name), 2);
            }

            cur_stream_.WriteString("} "+ls.Name+";", 1);
        }

        protected void OutputCPPOfProtocol(ProMeta pm)
        {
            cur_stream_.WriteString(String.Format("//-----------------------------{0}-------------------------------//", pm.Name), 0);
            cur_stream_.WriteEmptyLine();

            cur_stream_.WriteString(String.Format("{0}::{0}():inherit( {1})", pm.Name, pm.ProID), 0);
            cur_stream_.WriteString("{}", 0);

            cur_stream_.WriteEmptyLine();
        }

    }
}