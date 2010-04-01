using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Diagnostics;

namespace opcspot_mgr
{
    public class Utilities
    {
        /// <summary>
        /// Determines whether the specified name is a valid name.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="spaceEnabled">if set space enabled to <c>true</c>.</param>
        /// <returns>
        /// 	<c>true</c> if the specified name is a valid name; otherwise, return <c>false</c>.
        /// </returns>
        public static bool IsValidName(string name, bool spaceEnabled)
        {
            Regex regex = null;

            if (spaceEnabled)
            {
                regex = new Regex(@"^[a-zA-Z_][a-zA-Z0-9_ ]*$");
            }
            else
            {
                regex = new Regex(@"^[a-zA-Z_][a-zA-Z0-9_]*$");
            }

            return regex.IsMatch(name);
        }

        /// <summary>
        /// Get the main path of the current process.
        /// </summary>
        /// <returns>The main path</returns>
        public static string GetMainPath()
        {
            return Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName);
        }
    }
}
