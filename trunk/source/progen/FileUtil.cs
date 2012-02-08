using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace progen
{
    public class FileUtil
    {
        private FileUtil()
        {}

        public static bool MakeDir( ref string path)
        {
            bool babs =Path.IsPathRooted(path);
            if (!babs)
                path = Path.Combine(GetCurPath(), path);

            if (Directory.Exists(path))
                return true;

            if( File.Exists( path))
                throw new SystemException( String.Format("{0} is a file, not a path", path));

            try
            {
                Directory.CreateDirectory(path);
                return true;
            }
            catch(Exception se)
            {
                throw se;
            }
        }

        public static bool DelDir( string path)
        {
            bool babs = Path.IsPathRooted(path);
            if (!babs)
                path = Path.Combine(GetCurPath(), path);

            if (!Directory.Exists(path))
                return true;

            if (File.Exists(path))
                throw new SystemException(String.Format("dir that want to be deleted , {0} is a file, not a path", path));

            try
            {
                Directory.Delete(path, true);
                return true;
            }
            catch (Exception se)
            {
                throw se;
            }
        }

        public static string GetCurPath()
        {
            return Directory.GetCurrentDirectory();
        }

        public static string GetPath( string parent, string path)
        {
            string p =Path.Combine(parent, path);
            if (!Directory.Exists(p))
                throw new SystemException(String.Format("{0} path not exist", p));

            return p;
        }

        public static string GetFile( string path, string file)
        {
            string p = Path.Combine(path, file);
            return p;
        }

        public static bool IsPathExist( string path)
        {
            return Directory.Exists(path);
        }

        public static bool IsFileExist( string f)
        {
            return File.Exists(f);
        }

        public static string GetFileNameWithoutExtension(string f)
        {
            return Path.GetFileNameWithoutExtension(f);
        }

        public static bool IsFileExist( string path, string file)
        {
            if (!IsPathExist(path))
                return false;

            string p =Path.Combine(path, file);

            return IsFileExist(p);
        }

        public static List<string> ListFilesOfDir( string dir, string ext)
        {
            List<string> ret = new List<string>();

            if( !IsPathExist( dir))
                return ret;

            string[] files =Directory.GetFiles(dir, ext, SearchOption.TopDirectoryOnly);
            for (int ind = 0; ind < files.Length; ++ind)
            {
                string fname =Path.GetFileName(files[ind]);
                if( fname == "")
                    continue;
                ret.Add(fname);
            }

            return ret;
        }
    }
}
