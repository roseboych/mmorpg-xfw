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
        protected void GenerateMacroDefine()
        {
            try
            {
                MakeOutputFile(path_root_, "pro_macro_def.h");

                GenHFileHeaderB("pro_macro_def");

                cur_stream_.WriteString(ExportCConst.IncludeFiles1, 0);

                if (!ProMetaHelper.Instance().HaveMacroDefines)
                    return;

                List<MacroDefine> mds = ProMetaHelper.Instance().MacroDefines;
                foreach (MacroDefine md in mds)
                {
                    if (!AppUtil.IsEmptyString(md.Desc))
                        cur_stream_.WriteComment(md.Desc, 0);

                    if (md.Type == ProMetaConst.DATATYPE_INT32)
                        cur_stream_.WriteString(String.Format("#define {0}  {1}", md.Name, md.Value), 0);

                    cur_stream_.WriteEmptyLine();
                }

            }
            finally
            {
                GenHFileHeaderE("pro_macro_def");
                cur_stream_.FinishGenerate();
            }
        }


    }
}