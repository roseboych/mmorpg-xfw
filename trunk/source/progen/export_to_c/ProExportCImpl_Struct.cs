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
        protected void GenerateGlobalStruct()
        {
            try
            {
                MakeOutputFile(path_root_, "pro_struct_def.h");

                GenHFileHeaderB("pro_struct_def");

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);
                cur_stream_.WriteString(String.Format("#include <prolib/StaticString.h>"), 0);
                cur_stream_.WriteString(String.Format("#include <prolib/gen/pro_macro_def.h>"), 0);
                cur_stream_.WriteString(String.Format("#include <prolib/gen/pro_enum_def.h>"), 0);
                cur_stream_.WriteEmptyLine();

                if (!ProMetaHelper.Instance().HaveGlobalStruct)
                    return;

                //defined under one namespace
                cur_stream_.WriteString("PROTOCOL_NAMESPACE_BEGIN", 0);
                cur_stream_.WriteEmptyLine();

                List<GlobalStruct> mds = ProMetaHelper.Instance().GlobalStruct;
                foreach (GlobalStruct md in mds)
                {
                    //output one enum struct
                    OutputOneGlobalStruct(md);

                    cur_stream_.WriteEmptyLine();
                }

                cur_stream_.WriteString("PROTOCOL_NAMESPACE_END", 0);
            }
            finally
            {
                GenHFileHeaderE("pro_struct_def");
                cur_stream_.FinishGenerate();
            }
        }

        protected void OutputOneGlobalStruct(GlobalStruct es)
        {
            if (!AppUtil.IsEmptyString(es.Desc))
                cur_stream_.WriteComment(es.Desc, 0);

            //enum defined
            cur_stream_.WriteString(String.Format("struct {0}", es.Name), 0);
            cur_stream_.WriteString("{", 0);

            foreach (StructMember it in es.Members)
            {
                if (!AppUtil.IsEmptyString(it.Desc))
                    cur_stream_.WriteComment(it.Desc, 1);

                if (it.IsBaseArray)
                {
                    string sname = "";
                    if ( it.MacroLen != null)
                        sname = String.Format("{0}[{1}]", it.Name, it.MacroLen.Name);
                    else
                        sname = String.Format("{0}[{1}]", it.Name, it.ILen);

                    cur_stream_.WriteString(String.Format("{0}    {1};", DataType2CType(it), sname), 1);
                }
                else
                    cur_stream_.WriteString(String.Format("{0} {1};", DataType2CType(it), it.Name), 1);
            }

            cur_stream_.WriteString("};", 0);
        }
    }
}
