using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace progen
{
    public class LoggerHelper
    {
        private static LoggerHelper instance_ = new LoggerHelper();

        private LoggerHelper(){}

        private bool gen_error_ = false;

        #region log function

        protected void InitLog()
        {

        }

        protected void LogInfo( string msg)
        {

        }

        #endregion

        #region interface function

        protected static LoggerHelper Instance()
        {
            if (instance_ == null)
                instance_ = new LoggerHelper();

            return instance_;
        }

        public static void Initialize()
        {
            Instance().InitLog();
        }

        public static void StartGenerate()
        {
            Instance().gen_error_ = false;

            Instance().LogInfo("start generate protocol .........................");
        }

        public static void EndGenerate()
        {
            Instance().LogInfo( String.Format( "end generate protocol ........[{0}]................", 
                Instance().gen_error_?"success":"failed"));
        }

        public static void Log( string msg)
        {
            Instance().LogInfo(msg);
        }

        public static void GenError( string msg)
        {
            Instance().gen_error_ = true;

            Instance().LogInfo(msg);
        }

        #endregion
    }
}
