using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ausprobieren
{
    class Program
    {

        static void Main(string[] args)
        {
            Ausprobieren ausprobieren = new Ausprobieren();
            Console.WriteLine(ausprobieren.Teste());
        }
    }

    class Ausprobieren
    {
        public string myText = "Copyright SIEMENS c 2010 - 2023";

        public string Teste()
        {
            Console.WriteLine(myText);
            char [] byteArray = myText.ToCharArray();
            byteArray[18] = '@';
            string output = new string(byteArray);
            return output;
        }
    }
}
