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

                cur_stream_.WriteString("USE_PROTOCOL_NAMESPACE", 0);

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

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);
                cur_stream_.WriteString(ExportCConst.IncludeFilesOfProtocol, 0);
                cur_stream_.WriteString(String.Format("#include <prolib/gen/Pro_{0}_pid.h>", pr.RegionName), 0);
                cur_stream_.WriteString(String.Format("#include <prolib/gen/Pro_{0}.h>", pr.RegionName), 0);
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
            cur_stream_.WriteString(String.Format("~{0}();", pm.Name), 1);
            cur_stream_.WriteEmptyLine();

            // defined local struct
            if( pm.DefinedLocalStruct)
            {
                cur_stream_.WriteString("public:", 0);

                foreach( LocalStruct ls in pm.LocalStructs)
                {
                    OutputLocalStruct(ls);
                    cur_stream_.WriteEmptyLine();
                }
            }

            //-------------------------protected function-----------------------------
            cur_stream_.WriteString("protected:", 0);

            //BEGIN_LOAD_PRO
            cur_stream_.WriteString(String.Format("BEGIN_LOAD_PRO( pdata, totlen, ext)"), 1);
            OutputLoadLogic(pm);
            cur_stream_.WriteString(String.Format("END_LOAD_PRO()"), 1);

            cur_stream_.WriteEmptyLine();

            //BEGIN_SAVE_PRO
            cur_stream_.WriteString(String.Format("BEGIN_SAVE_PRO( pdata, buflen, len, ext)"), 1);
            OutputSaveLogic(pm);
            cur_stream_.WriteString(String.Format("END_SAVE_PRO()"), 1);

            cur_stream_.WriteEmptyLine();

            //BEGIN_CLONE_PRO
            cur_stream_.WriteString(String.Format("BEGIN_CLONE_PRO( {0}, proo)", pm.Name), 1);
            OutputCloneLogic(pm);
            cur_stream_.WriteString(String.Format("END_CLONE_PRO()"), 1);

            cur_stream_.WriteEmptyLine();

            //-------------------------defined members------------------------------------
            cur_stream_.WriteString("public:", 0);

            foreach( ProMetaMember p in pm.Members)
            {
                if (!AppUtil.IsEmptyString(p.Desc))
                    cur_stream_.WriteComment(p.Desc, 1);

                if (p.IsBaseArray)
                {
                    string sname = "";
                    if (p.MacroLen != null)
                        sname = String.Format("{0}[{1}]", p.Name, p.MacroLen.Name);
                    else
                        sname = String.Format("{0}[{1}]", p.Name, p.ILen);
                    
                    cur_stream_.WriteString(String.Format("{0}    {1};", DataType2CType( p), sname), 1);
                }
                else
                    cur_stream_.WriteString(String.Format("{0}    {1};", DataType2CType( p), p.Name), 1);
            }

            cur_stream_.WriteEmptyLine();

            //---------------------------------custom function-----------------------------------------
            if( !AppUtil.IsEmptyString(pm.CustomFunctionC))
            {
                cur_stream_.WriteString("public:", 0);
                cur_stream_.WriteString(pm.CustomFunctionC, 1);

                cur_stream_.WriteEmptyLine();
            }

            //------------------------------------------------------------------------
            cur_stream_.WriteString("};", 0);
            cur_stream_.WriteEmptyLine();
        }

        protected void OutputLoadLogic( ProMeta pm)
        {
            foreach( ProMetaMember p in pm.Members)
            {
                if( p.Type == ProMetaConst.DATATYPE_LOCALSTRUCT)
                {
                    if( p.IsList)
                    {
                        cur_stream_.WriteString(String.Format("LOAD_LIST_PRO_BEGIN( pdata, totlen, {0} )", p.RefLocalStruct.Name), 2);

                        foreach( StructMember sm in p.RefLocalStruct.Members)
                        {
                            OutputLoadLogicItem( true, sm, 3, null);
                        }

                        cur_stream_.WriteString(String.Format("LOAD_LIST_PRO_END( {0} )", p.Name), 2);
                    }
                    else
                    {
                        foreach (StructMember sm in p.RefLocalStruct.Members)
                        {
                            OutputLoadLogicItem( false, sm, 2, p.Name);
                        }
                    }
                }
                else if( p.Type == ProMetaConst.DATATYPE_GLOBALSTRUCT)
                {
                    if( p.IsList)
                    {
                        cur_stream_.WriteString(String.Format("LOAD_LIST_PRO_BEGIN( pdata, totlen, {0} )", p.RefGlobalStruct.Name), 2);

                        foreach (StructMember sm in p.RefGlobalStruct.Members)
                        {
                            OutputLoadLogicItem(true, sm, 3, null);
                        }

                        cur_stream_.WriteString(String.Format("LOAD_LIST_PRO_END( {0} )", p.Name), 2);
                    }
                    else
                    {
                        foreach (StructMember sm in p.RefGlobalStruct.Members)
                        {
                            OutputLoadLogicItem(false, sm, 2, p.Name);
                        }
                    }
                }
                else if( p.Type == ProMetaConst.DATATYPE_SERIALIZEOBJ)
                {
                    if( p.IsList)
                    {
                        cur_stream_.WriteString(String.Format("LOAD_SERIALIZEOBJ_LIST_BEGIN( pdata, totlen)"), 2);
                        cur_stream_.WriteString(String.Format("LOAD_OBJECT_PRO( pdata, totlen, _item)"), 3);
                        cur_stream_.WriteString(String.Format("LOAD_SERIALIZEOBJ_LIST_END( {0} )", p.Name), 2);
                    }
                    else
                    {
                        cur_stream_.WriteString(String.Format("LOAD_OBJECT_PRO( pdata, totlen, {0} )", p.Name), 2);
                    }
                }
                else
                {
                    OutputLoadLogicItem(false, p, 2, null);
                }
            }
        }

        private void OutputLoadLogicItem( bool islist, StructMember p, int tab, string structref)
        {
            if( islist)
            {
                if( p.IsBaseArray)
                {
                    string ctype = DataType2CType(p);
                    cur_stream_.WriteString(String.Format("LOAD_ARRAY_PRO_LS( pdata, totlen, sizeof({0}), {1}, {2} )", 
                        ctype, p.Name, p.ArrayRef), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT8)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT8_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT16)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT16_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT32)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT32_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_UINT32)
                {
                    cur_stream_.WriteString(String.Format("LOAD_UINT32_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT64)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT64_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_FLOAT32)
                {
                    cur_stream_.WriteString(String.Format("LOAD_FLOAT32_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_TIMESTAMP)
                {
                    cur_stream_.WriteString(String.Format("LOAD_TIMESTAMP_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_STATICSTRING)
                {
                    cur_stream_.WriteString(String.Format("LOAD_STR_PRO_LS( pdata, totlen, {0} )", p.Name), tab);
                }
            }
            else
            {
                string strname = "";
                if (structref != null)
                    strname = String.Format("{0}.{1}", structref, p.Name);
                else
                    strname = p.Name;

                if (p.IsBaseArray)
                {
                    string strname2 = "";
                    if (structref != null)
                        strname2 = String.Format("{0}.{1}", structref, p.ArrayRef);
                    else
                        strname2 = p.ArrayRef;

                    string ctype = DataType2CType(p);
                    cur_stream_.WriteString(String.Format("LOAD_ARRAY_PRO( pdata, totlen, sizeof({0}), {1}, {2} )",
                        ctype, strname, strname2), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT8)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT8_PRO( pdata, totlen, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT16)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT16_PRO( pdata, totlen, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT32)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT32_PRO( pdata, totlen, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_UINT32)
                {
                    cur_stream_.WriteString(String.Format("LOAD_UINT32_PRO( pdata, totlen, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT64)
                {
                    cur_stream_.WriteString(String.Format("LOAD_INT64_PRO( pdata, totlen, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_FLOAT32)
                {
                    cur_stream_.WriteString(String.Format("LOAD_FLOAT32_PRO( pdata, totlen, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_TIMESTAMP)
                {
                    cur_stream_.WriteString(String.Format("LOAD_TIMESTAMP_PRO( pdata, totlen, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_STATICSTRING)
                {
                    cur_stream_.WriteString(String.Format("LOAD_STR_PRO( pdata, totlen, {0} )", strname), tab);
                }
            }

        }

        protected void OutputSaveLogic( ProMeta pm)
        {
            foreach (ProMetaMember p in pm.Members)
            {
                if (p.Type == ProMetaConst.DATATYPE_LOCALSTRUCT)
                {
                    if (p.IsList)
                    {
                        cur_stream_.WriteString(String.Format("SAVE_LIST_PRO_BEGIN( pdata, buflen, len, {0}, {1} )", p.Name, p.RefLocalStruct.Name), 2);

                        foreach (StructMember sm in p.RefLocalStruct.Members)
                        {
                            OutputSaveLogicItem(true, sm, 3, null);
                        }

                        cur_stream_.WriteString(String.Format("SAVE_LIST_PRO_END()"), 2);
                    }
                    else
                    {
                        foreach (StructMember sm in p.RefLocalStruct.Members)
                        {
                            OutputSaveLogicItem(false, sm, 2, p.Name);
                        }
                    }
                }
                else if (p.Type == ProMetaConst.DATATYPE_GLOBALSTRUCT)
                {
                    if (p.IsList)
                    {
                        cur_stream_.WriteString(String.Format("SAVE_LIST_PRO_BEGIN( pdata, buflen, len, {0}, {1} )", p.Name, p.RefGlobalStruct.Name), 2);

                        foreach (StructMember sm in p.RefGlobalStruct.Members)
                        {
                            OutputSaveLogicItem(true, sm, 3, null);
                        }

                        cur_stream_.WriteString(String.Format("SAVE_LIST_PRO_END()"), 2);
                    }
                    else
                    {
                        foreach (StructMember sm in p.RefGlobalStruct.Members)
                        {
                            OutputSaveLogicItem(false, sm, 2, p.Name);
                        }
                    }
                }
                else if (p.Type == ProMetaConst.DATATYPE_SERIALIZEOBJ)
                {
                    if (p.IsList)
                    {
                        cur_stream_.WriteString(String.Format("SAVE_SERIALIZEOBJ_LIST_BEGIN( pdata, buflen, len, {0} )", p.Name), 2);
                        cur_stream_.WriteString(String.Format("SAVE_OBJECT_PRO( pdata, buflen, len, _item)"), 3);
                        cur_stream_.WriteString(String.Format("SAVE_SERIALIZEOBJ_LIST_END()"), 2);
                    }
                    else
                    {
                        cur_stream_.WriteString(String.Format("SAVE_OBJECT_PRO( pdata, buflen, len, {0} )", p.Name), 2);
                    }
                }
                else
                {
                    OutputSaveLogicItem(false, p, 2, null);
                }
            }
        }

        private void OutputSaveLogicItem(bool islist, StructMember p, int tab, string structref)
        {
            if (islist)
            {
                if (p.IsBaseArray)
                {
                    string ctype = DataType2CType(p);
                    cur_stream_.WriteString(String.Format("SAVE_ARRAY_PRO_LS( pdata, buflen, len, sizeof({0}), {1}, {2} )",
                        ctype, p.Name, p.ArrayRef), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT8)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT8_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT16)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT16_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT32)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT32_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_UINT32)
                {
                    cur_stream_.WriteString(String.Format("SAVE_UINT32_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT64)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT64_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_FLOAT32)
                {
                    cur_stream_.WriteString(String.Format("SAVE_FLOAT32_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_TIMESTAMP)
                {
                    cur_stream_.WriteString(String.Format("SAVE_TIMESTAMP_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_STATICSTRING)
                {
                    cur_stream_.WriteString(String.Format("SAVE_STR_PRO_LS( pdata, buflen, len, {0} )", p.Name), tab);
                }
            }
            else
            {
                string strname = "";
                if (structref != null)
                    strname = String.Format("{0}.{1}", structref, p.Name);
                else
                    strname = p.Name;

                if (p.IsBaseArray)
                {
                    string strname2 = "";
                    if (structref != null)
                        strname2 = String.Format("{0}.{1}", structref, p.ArrayRef);
                    else
                        strname2 = p.ArrayRef;

                    string ctype = DataType2CType(p);
                    cur_stream_.WriteString(String.Format("SAVE_ARRAY_PRO( pdata, buflen, len, sizeof({0}), {1}, {2} )",
                        ctype, strname, strname2), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT8)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT8_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT16)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT16_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT32)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT32_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_UINT32)
                {
                    cur_stream_.WriteString(String.Format("SAVE_UINT32_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_INT64)
                {
                    cur_stream_.WriteString(String.Format("SAVE_INT64_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_FLOAT32)
                {
                    cur_stream_.WriteString(String.Format("SAVE_FLOAT32_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_TIMESTAMP)
                {
                    cur_stream_.WriteString(String.Format("SAVE_TIMESTAMP_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
                else if (p.Type == ProMetaConst.DATATYPE_STATICSTRING)
                {
                    cur_stream_.WriteString(String.Format("SAVE_STR_PRO( pdata, buflen, len, {0} )", strname), tab);
                }
            }

        }

        protected void OutputCloneLogic( ProMeta pm)
        {
            foreach (ProMetaMember p in pm.Members)
            {
                if (p.Type == ProMetaConst.DATATYPE_LOCALSTRUCT)
                {
                    if( p.IsList)
                        cur_stream_.WriteString(String.Format("CLONE_LIST_PRO( proo, {0} )", p.Name), 2);
                    else
                        cur_stream_.WriteString(String.Format("CLONE_VAR_PRO( proo, {0} )", p.Name), 2);
                }
                else if (p.Type == ProMetaConst.DATATYPE_GLOBALSTRUCT)
                {
                    if (p.IsList)
                        cur_stream_.WriteString(String.Format("CLONE_LIST_PRO( proo, {0} )", p.Name), 2);
                    else
                        cur_stream_.WriteString(String.Format("CLONE_VAR_PRO( proo, {0} )", p.Name), 2);
                }
                else if (p.Type == ProMetaConst.DATATYPE_SERIALIZEOBJ)
                {
                    if (p.IsList)
                    {
                        cur_stream_.WriteString(String.Format("CLONE_SERIALIZEOBJ_LIST_PRO( proo, {0} )", p.Name), 2);
                    }
                    else
                    {
                        cur_stream_.WriteString(String.Format("CLONE_OBJECT_PRO( proo, {0} )", p.Name), 2);
                    }
                }
                else
                {

                    if (p.IsBaseArray)
                    {
                        string ctype = DataType2CType(p);
                        cur_stream_.WriteString(String.Format("CLONE_ARRAY_PRO( proo, sizeof({0}), {1}, {2})", ctype, p.Name, p.ArrayRef), 2);
                    }
                    else
                        cur_stream_.WriteString(String.Format("CLONE_VAR_PRO( proo, {0} )", p.Name), 2);
                }
            }
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

                if (sm.IsBaseArray)
                {
                    string sname = "";
                    if( sm.MacroLen != null)
                        sname =String.Format("{0}[{1}]", sm.Name, sm.MacroLen.Name);
                    else
                        sname = String.Format("{0}[{1}]", sm.Name, sm.ILen);
                    cur_stream_.WriteString(String.Format("{0}    {1};", DataType2CType(sm), sname), 2);
                }
                else
                    cur_stream_.WriteString(String.Format("{0}    {1};", DataType2CType(sm), sm.Name), 2);
            }

            cur_stream_.WriteString("} "+ls.Name+";", 1);
        }

        protected void OutputCPPOfProtocol(ProMeta pm)
        {
            cur_stream_.WriteString(String.Format("//-----------------------------{0}-------------------------------//", pm.Name), 0);

            cur_stream_.WriteString(String.Format("{0}::{0}():inherit( {1})", pm.Name, pm.ProID), 0);
            if( AppUtil.IsEmptyString( pm.ConstructBlockC))
                cur_stream_.WriteString("{}", 0);
            else
            {
                cur_stream_.WriteString("{", 0);
                cur_stream_.WriteString(pm.ConstructBlockC, 1);
                cur_stream_.WriteString("}", 0);
            }

            cur_stream_.WriteEmptyLine();

            cur_stream_.WriteString(String.Format("{0}::~{0}()", pm.Name), 0);
            if (AppUtil.IsEmptyString(pm.DestructBlockC))
                cur_stream_.WriteString("{}", 0);
            else
            {
                cur_stream_.WriteString("{", 0);
                cur_stream_.WriteString(pm.DestructBlockC, 1);
                cur_stream_.WriteString("}", 0);
            }

            cur_stream_.WriteEmptyLine();
        }

    }
}