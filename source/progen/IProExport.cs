using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace progen
{
    public interface IProExport
    {
        void GenerateProLib();

        void DeployProLib();
    }
}
