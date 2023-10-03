using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace CopyrightCorrection
{
    class Program
    {
        const string path1 = @"..\..\..\outTest.txt";
        const string path2 = @"..\..\..\out135.txt";
        const string path3 = @"..\..\..\out138.txt";

        static void Main(string[] args)
        {
            string outTest = File.ReadAllText(path1);
            string[] out135 = File.ReadAllLines(path2);
            string[] out138 = File.ReadAllLines(path3);

            string result = "";

            if (outTest.StartsWith("Information:"))
            {
                outTest = outTest.Substring(outTest.IndexOf("Copyright"));
            }

            result = correctCopyright(out135);
            Console.WriteLine("Vergleiche mit out135.txt");
            if (result == outTest)
            {
                Console.WriteLine("Dateien sind identisch\n");
            }
            else
            {
                Console.WriteLine("Dateien sind unterschiedlich");
            }

            result = correctCopyright(out138);
            Console.WriteLine("Vergleiche mit out138.txt");
            if (result == outTest)
            {
                Console.WriteLine("Dateien sind identisch");
            }
            else
            {
                Console.WriteLine("Dateien sind unterschiedlich");
            }
        }

        private static string correctCopyright(string[] lineArray)
        {
            string result = "";
            string pattern = " c ";
            pattern = pattern.Replace('c', (char)65533);

            foreach (string line in lineArray)
            {
                if (line.StartsWith("Copyright"))
                {
                    string newLine = line.Replace(" c ", pattern);
                    result += newLine;
                    result += "\r\n";
                }
                else
                {
                    if (!line.StartsWith("Information:"))
                    {
                        result += line;
                        result += "\r\n";
                    }
                }
            }
            return result;
        }
    }
}
