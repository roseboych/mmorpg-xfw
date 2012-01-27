using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using progen.pro_meta;

namespace progen
{
    public class Program
    {
        static void Main(string[] args)
        {
            LoggerHelper.Initialize();
            LoggerHelper.StartGenerate();

            try
            {
                ProMetaHelper.Instance().InitMeta();

                //generate protocol file
                if (ExportConfig.Instance().Export2C)
                {
                    export_to_c.ProExportCImpl exp = new export_to_c.ProExportCImpl();
                    exp.GenerateProLib();
                }

                if (ExportConfig.Instance().Export2Java)
                {
                    export_to_java.ProExportJavaImpl exp = new export_to_java.ProExportJavaImpl();
                    exp.GenerateProLib();
                }
            }
            catch(Exception se)
            {
                LoggerHelper.GenError(se.Message);
            }

            LoggerHelper.EndGenerate();
        }
    }
}
