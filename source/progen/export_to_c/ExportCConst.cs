using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace progen.export_to_c
{
    public class ExportCConst
    {
        public const string FileHeadInfo =
@"/**
* protocol module
*
* auto generate file, don't change it.
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2012
*/";

        public const string IncludeFiles1 =
@"
#include <prolib/core_type.h>

#include <string>
#include <list>
#include <algorithm>
";

        public const string IncludeFilesOfProtocol =
@"
#include <prolib/BasicProtocol.h>
#include <prolib/ProtocolFactory.h>
#include <prolib/SingletonT.h>

#include <prolib/gen/pro_macro_def.h>
#include <prolib/gen/pro_enum_def.h>
#include <prolib/gen/pro_struct_def.h>
";

    }
}
