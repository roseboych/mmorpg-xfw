using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace progen
{
    public class GenerateStream
    {
        private StreamWriter f_stream_ = null;

        public GenerateStream( string path, string file, string commons)
        {
            FileUtil.MakeDir( ref path);
            file =FileUtil.GetFile(path, file);

            f_stream_ = new StreamWriter( file, false, Encoding.UTF8);

            WriteString( commons, 0);
        }

        public void FinishGenerate()
        {
            if( f_stream_ != null)
            {
                f_stream_.Flush();
                f_stream_.Close();
                f_stream_ = null;
            }
        }

        public void WriteString( string str, int tabnum)
        {
            for (int ind = 0; ind < tabnum; ++ind)
                f_stream_.Write("\t");

            f_stream_.Write(str);
            f_stream_.WriteLine();
        }

        public void WriteComment( string str, int tabnum)
        {
            for (int ind = 0; ind < tabnum; ++ind)
                f_stream_.Write("\t");

            f_stream_.Write("//");
            f_stream_.Write(str);
            f_stream_.WriteLine();
        }

        public void WriteCommentMultilines(string str)
        {
            f_stream_.Write("/**");
            f_stream_.WriteLine();
            f_stream_.Write(str);
            f_stream_.WriteLine();
            f_stream_.Write("**/");
            f_stream_.WriteLine();
        }

        public void WriteClassDesc( string name, string desc)
        {
            f_stream_.Write("/**");
            f_stream_.WriteLine();
            f_stream_.Write(String.Format("* @class {0}", name));
            f_stream_.WriteLine();
            f_stream_.Write(String.Format("* @brief"));
            f_stream_.WriteLine();
            f_stream_.Write(desc);
            f_stream_.WriteLine();
            f_stream_.Write("**/");
            f_stream_.WriteLine();
        }

        public void WriteEmptyLine()
        {
            f_stream_.WriteLine();
        }
    }

}
