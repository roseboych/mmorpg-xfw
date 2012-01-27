using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace progen
{
    public class ExportConfig
    {
        private static ExportConfig instance_ = new ExportConfig();

        private const string ini_file_ = "./progen.ini";

        private bool export_2_c_ = false;
        private string export_2_c_path_ = "";

        private bool export_2_java_ = false;
        private string export_2_java_path_ = "";

        #region .ini property

        public bool Export2C
        {
            get { return this.export_2_c_; }
        }

        public string Export2CPath
        {
            get { return this.export_2_c_path_; }
        }

        public bool Export2Java
        {
            get { return this.export_2_java_; }
        }

        public string Export2JavaPath
        {
            get { return this.export_2_java_path_; }
        }

        #endregion

        public static ExportConfig Instance()
        {
            return instance_;
        }

        private ExportConfig()
        {
            InitConfig();
        }

        private void InitConfig()
        {
            export_2_c_ = ReadYesNo("c", "generate", false);
            export_2_c_path_ = ReadString("c", "export_path", "./temp/c/");

            export_2_java_ = ReadYesNo("java", "generate", false);
            export_2_java_path_ = ReadString("java", "export_path", "./temp/java/");
        }

        public string ReadString(string Section, string Ident, string Default)
        {
            Byte[] Buffer = new Byte[65535];
            int bufLen = GetPrivateProfileString( Section, Ident, Default, Buffer, Buffer.GetUpperBound(0), ini_file_);
            string s = Encoding.GetEncoding(0).GetString(Buffer);
            s = s.Substring(0, bufLen);
            return s.Trim();
        }

        public int ReadInteger(string Section, string Ident, int Default)
        {
            string intStr = ReadString( Section, Ident, Convert.ToString(Default));

            try
            {
                return Convert.ToInt32(intStr);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public bool ReadYesNo(string Section, string Ident, bool Default)
        {
            try
            {
                string v = ReadString(Section, Ident, Default?"yes":"no");
                v =v.ToLower();
                if (v == "yes")
                    return true;
                else
                    return false;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, byte[] retVal, int size, string filePath);
    }
}
