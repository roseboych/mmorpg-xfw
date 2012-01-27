using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;

namespace progen
{
    public class XmlUtil
    {
        public const string DATETIME_FORMAT = "{0:yyyy-MM-dd}";

        private string xmlfile_ ="";

        private XmlDocument doc_ = null;
        private XmlElement root_ = null;
        private XmlNamespaceManager nsmgr_ = null;

        public XmlUtil( string xmlfile)
        {
            this.xmlfile_ =xmlfile;

            if (!FileUtil.IsFileExist(xmlfile))
                throw new FileNotFoundException(xmlfile);

            doc_ = new XmlDocument();
            doc_.Load(this.xmlfile_);

            root_ = doc_.DocumentElement;

            nsmgr_ = new XmlNamespaceManager(doc_.NameTable);
        }

        #region property method

        public static string GetAttrStrVal( XmlElement e, string attr, string defval)
        {
            string v = e.GetAttribute(attr);
            if (v == null || v == "")
                return defval;
            else
                return v;
        }

        public static bool GetAttrBoolValByYesNo( XmlElement e, string attr, bool defval)
        {
            string v = GetAttrStrVal(e, attr, defval?"yes":"no");
            v = v.ToLower();
            try
            {
                if (v == "yes")
                    return true;
                else
                    return false;
            }
            catch (Exception)
            {
                return defval;
            }
        }

        public static int GetAttrIntVal(XmlElement e, string attr, int defval)
        {
            string v =GetAttrStrVal(e, attr, defval.ToString());
            try
            {
                return int.Parse(v);
            }
            catch(Exception){
                return defval;
            }
        }

        public static float GetAttrFloatVal(XmlElement e, string attr, float defval)
        {
            string v = GetAttrStrVal(e, attr, defval.ToString());
            try
            {
                return float.Parse(v);
            }
            catch (Exception)
            {
                return defval;
            }
        }

        public static decimal GetAttrDecimalVal(XmlElement e, string attr, decimal defval)
        {
            string v = GetAttrStrVal(e, attr, defval.ToString());
            try
            {
                return decimal.Parse(v);
            }
            catch (Exception)
            {
                return defval;
            }
        }

        public static DateTime GetAttrDateTimeVal(XmlElement e, string attr, DateTime defval)
        {
            string v = GetAttrStrVal(e, attr, String.Format( DATETIME_FORMAT, defval));
            try
            {
                return DateTime.Parse(v);
            }
            catch (Exception)
            {
                return defval;
            }
        }

        #endregion

        #region element method

        public static XmlElement GetFirstChildren(XmlElement parent, string tag)
        {
            XmlNodeList nl = null;
            if (tag == null || tag == "")
                nl = parent.ChildNodes;
            else
                nl = parent.GetElementsByTagName(tag);

            for( int ind =0; ind < nl.Count; ++ind)
            {
                if (nl[ind].NodeType == XmlNodeType.Element)
                    return (XmlElement)nl[ind];
            }

            return null;
        }

        public static List<XmlElement> GetChildrenElement(XmlElement parent, string tag)
        {
            List<XmlElement> ret = new List<XmlElement>();

            XmlNodeList nl = null;
            if( tag == null || tag == "")
                nl =parent.ChildNodes;
            else
                nl = parent.GetElementsByTagName(tag);

            for (int ind = 0; ind < nl.Count; ++ind)
            {
                if (nl[ind].NodeType == XmlNodeType.Element)
                {
                    ret.Add( (XmlElement)nl[ind]);
                }
            }

            return ret;
        }

        public static string GetCDATAContent(XmlElement e)
        {
            XmlCDataSection ds = null;

            XmlNodeList nl =e.ChildNodes;
            for( int ind =0; ind < nl.Count; ++ind)
            {
                if( nl[ind].NodeType == XmlNodeType.CDATA)
                {
                    ds = (XmlCDataSection)nl[ind];
                    break;
                }
            }

            if (ds != null)
                return ds.Value;
            else
                return "";
        }

        public static string GetElementText(XmlElement e)
        {
            XmlNode cur =null;
            XmlNodeList nl =e.ChildNodes;
            for( int ind =0; ind < nl.Count; ++ind)
            {
                if( nl[ind].NodeType == XmlNodeType.Text)
                {
                    cur = nl[ind];
                    break;
                }
            }

            if (cur != null)
                return cur.Value;
            else
                return "";
        }

        #endregion

        #region property

        public bool IsValid
        {
            get { return doc_ != null; }
        }

        public string XmlFile
        {
            get { return this.xmlfile_; }
        }

        public XmlElement Root
        {
            get { return root_; }
        }

        #endregion

    }
}
