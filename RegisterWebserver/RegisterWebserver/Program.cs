using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Security;

namespace RegisterWebserver
{
    class Program
    {

        static void Main(string[] args)
        {
            bool isRegistered = true;

            if (args[0] == "verify")
            {
                using (Process pcsystemControl = new Process())
                {
                    pcsystemControl.StartInfo.FileName = @"powershell";
                    pcsystemControl.StartInfo.Arguments = "C:\\Users\\pu3846\\Documents\\Playground\\VerifyWebserver.ps1 -Computername " + args[1];
                    pcsystemControl.StartInfo.UseShellExecute = false;
                    pcsystemControl.StartInfo.RedirectStandardOutput = true;

                    pcsystemControl.Start();

                    string message = pcsystemControl.StandardOutput.ReadToEnd();
                    Console.WriteLine(message);

                    string[] seperators = new string[] { "\r\n" };
                    string[] lineArray = message.Split(seperators, StringSplitOptions.None);

                    foreach (string line in lineArray)
                    {
                        if (line.Contains("Warning"))
                        {
                            if (line.Contains("AnalyseSimaticApplication"))
                            {
                                isRegistered = false;
                            }
                        }
                    }

                    pcsystemControl.WaitForExit();
                    Console.WriteLine("Exit Code = " + pcsystemControl.ExitCode.ToString());
                }

                if (isRegistered)
                {
                    Console.WriteLine("Webserver is already registered");
                }
                else
                {
                    Console.WriteLine("Webserver is not registered");
                }
            }
            else if (args[0] == "register")
            {
                using (Process pcsystemControl = new Process())
                {
                    pcsystemControl.StartInfo.FileName = @"powershell";
                    pcsystemControl.StartInfo.Arguments = "C:\\Users\\pu3846\\Documents\\Playground\\RegisterWebserver.ps1 -Computername " + args[1];
                    pcsystemControl.StartInfo.UseShellExecute = false;
                    pcsystemControl.StartInfo.RedirectStandardOutput = true;

                    pcsystemControl.Start();

                    string message = pcsystemControl.StandardOutput.ReadToEnd();
                    Console.WriteLine(message);
                }
            }
        }
    }
}
