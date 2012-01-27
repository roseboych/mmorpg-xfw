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
        protected void GenerateEnumStruct()
        {
            try
            {
                MakeOutputFile(path_root_, "pro_enum_def.h");

                GenHFileHeaderB("pro_enum_def");

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);
                cur_stream_.WriteString(String.Format("#include <prolib/gen/pro_macro_def.h>"), 0);

                if (!ProMetaHelper.Instance().HaveGlobalEnum)
                    return;

                List<EnumStruct> mds = ProMetaHelper.Instance().GlobalEnum;
                foreach (EnumStruct md in mds)
                {
                    //output one enum struct
                    OutputOneEnumStruct(md);

                    cur_stream_.WriteEmptyLine();
                }

            }
            finally
            {
                GenHFileHeaderE("pro_enum_def");
                cur_stream_.FinishGenerate();
            }
        }

        protected void OutputOneEnumStruct( EnumStruct es)
        {
            if (!AppUtil.IsEmptyString(es.Desc))
                cur_stream_.WriteComment(es.Desc, 0);

            //enum defined
            cur_stream_.WriteString(String.Format("enum {0}", es.Name), 0);
            cur_stream_.WriteString("{", 0);

            foreach( EnumItem it in es.Items)
            {
                if (!AppUtil.IsEmptyString(it.Desc))
                    cur_stream_.WriteComment(it.Desc, 1);

                if( it.HasSetValue)
                    cur_stream_.WriteString(String.Format("{0} ={1},", it.Name, it.Value), 1);
                else
                    cur_stream_.WriteString(String.Format("{0},", it.Name), 1);
            }

            cur_stream_.WriteString("};", 0);
        }
    }
}