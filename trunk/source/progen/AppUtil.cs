using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;
using System.IO;
using System.Reflection;
using System.ComponentModel;

namespace progen
{
    public class AppUtil
    {

        #region common util

        public static string EnumToString(object o)
        {
            if (o == null)
                return "null point object";
            Type t = o.GetType();
            string s = o.ToString();
            FieldInfo ei = t.GetField(s);

            DescriptionAttribute[] EnumAttributes = (DescriptionAttribute[])ei.GetCustomAttributes(typeof(DescriptionAttribute), false);
            if (EnumAttributes.Length > 0)
            {
                return EnumAttributes[0].Description;
            }

            return "unknown enum type";
        }

        public static int[] StrToIntArr(string src, string delimit, int errval)
        {
            string[] vk = src.Split(new string[] { delimit }, StringSplitOptions.None);
            int[] ret =new int[ vk.Length];
            for (int ind = 0; ind < vk.Length; ++ind)
            {
                try{
                    ret[ind] =int.Parse(vk[ind]);
                }
                catch(Exception){
                    ret[ind] = errval;
                }
            }

            return ret;
        }

        public static bool IsEmptyString( string str)
        {
            return str == null || str == "";
        }

        #endregion


        #region crypto util

        private static readonly byte[] s_IV = { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 };

        private static readonly byte[] s_defaultKey = { 0x01, 0x09, 0x07, 0x09, 0x00, 0x09, 0x01, 0x06 };

        public static byte[] RC2Encrypt(byte[] data, byte[] key)
        {
            MemoryStream result = new MemoryStream();
            using (RC2CryptoServiceProvider rc2 = new RC2CryptoServiceProvider())
            {
                using (ICryptoTransform encryptor = rc2.CreateEncryptor(key, s_IV))
                {
                    using (CryptoStream encryptStream = new CryptoStream(result, encryptor, CryptoStreamMode.Write))
                    {
                        encryptStream.Write(data, 0, data.Length);
                        encryptStream.FlushFinalBlock();
                    }
                }
            }
            return result.ToArray();
        }

        public static byte[] RC2Decrypt(byte[] data, byte[] key)
        {
            byte[] result = new byte[data.Length];
            using (RC2CryptoServiceProvider rc2 = new RC2CryptoServiceProvider())
            {
                using (ICryptoTransform decryptor = rc2.CreateDecryptor(key, s_IV))
                {
                    MemoryStream stream = new MemoryStream(data);
                    using (CryptoStream decryptStream = new CryptoStream(stream, decryptor, CryptoStreamMode.Read))
                    {
                        decryptStream.Read(result, 0, result.Length);
                    }
                }
            }
            return result;
        }

        public static string FastEncrypt(byte[] b)
        {
            byte[] enc = RC2Encrypt(b, s_defaultKey);

            return Convert.ToBase64String(enc);
        }

        public static byte[] FastDecrypt(string s)
        {
            byte[] b = Convert.FromBase64String(s);
            return RC2Decrypt(b, s_defaultKey);
        }

        public static string FastEncryptString(string s)
        {
            return FastEncrypt(Encoding.UTF8.GetBytes(s));
        }

        public static string FastDecryptString(string s)
        {
            byte[] b = FastDecrypt(s);
            return Encoding.UTF8.GetString(b).TrimEnd('\0');
        }

        public static string MD5(string s)
        {
            if (s == null)
                s = "";

            byte[] tmpByte = Encoding.UTF8.GetBytes(s);

            MD5 md5 = new MD5CryptoServiceProvider();
            tmpByte = md5.ComputeHash(tmpByte);
            md5.Clear();

            s = "";
            for (int i = 0; i < tmpByte.Length; i++)
                s += tmpByte[i].ToString("x2");

            return s;
        }

        #endregion

        #region IO util

        public const int BUFFER_SIZE = 4096;

        public static void CopyStream(Stream source, Stream dest)
        {
            byte[] buffer = new byte[BUFFER_SIZE];
            int sourceBytes =0;

            do
            {
                sourceBytes = source.Read(buffer, 0, buffer.Length);
                dest.Write(buffer, 0, sourceBytes);
            } 
            while (sourceBytes > 0);
        }

        public static string GetSplitContent(string content, string pre, string aft)
        {
            if (content == null)
                return "";
            int pos1 = 0;
            if (pre != null)
            {
                pos1 = content.IndexOf(pre, StringComparison.OrdinalIgnoreCase);
                if (pos1 != -1)
                    pos1 += pre.Length;
            }

            int pos2 = 0;
            if (aft != null)
                pos2 = content.IndexOf(aft, StringComparison.OrdinalIgnoreCase);

            if (pre == null)
            {
                if (aft == null)
                    return content;
                else if (pos2 == -1)
                    return "";
                else
                    return content.Substring(0, pos2);
            }
            else if (pos1 != -1)
            {
                if (aft == null)
                    return content.Substring(pos1);
                else if (pos2 > pos1)
                    return content.Substring(pos1, pos2 - pos1);
            }

            return "";
        }

        /*
         * get the string before string specified by sep
         */
        public static string GetStringBefore(string s, string sep)
        {
            int i = s.IndexOf(sep);
            if (i != -1)
            {
                return s.Substring(0, i);
            }
            else
            {
                return "";
            }
        }

        #endregion
    }
}
