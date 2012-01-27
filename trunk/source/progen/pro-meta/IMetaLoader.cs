using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using progen.pro_meta.exception;

namespace progen.pro_meta
{
    interface IMetaLoader
    {
        void LoadFromXml(XmlElement e);
    }
}
