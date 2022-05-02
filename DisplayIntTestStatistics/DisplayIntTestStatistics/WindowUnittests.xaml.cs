using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml;

namespace DisplayIntTestStatistics
{
    /// <summary>
    /// Interaktionslogik für WindowUnittests.xaml
    /// </summary>
    public partial class WindowUnittests : Window
    {
        private UnittestResults unittestResults = new UnittestResults();
        private string resultPath;
        public WindowUnittests()
        {
            InitializeComponent();
        }

        public string ResultPath { set => resultPath = value; }

        private void BtnStartScan_Click(object sender, RoutedEventArgs e)
        {
            unittestResults.CollectUnittestResults(resultPath);
            tbNumber.Text = unittestResults.NumberFailed.ToString();
            tbSum.Text = unittestResults.NumberTestfiles.ToString();
            foreach (FailedUnittest unittest in unittestResults.FailedLogs)
            {
                AddToListBox(unittest.LogFilePath);
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            tbResultPath.Text = resultPath;
        }

        private void AddToListBox(string logfile)
        {
            string[] sub = logfile.Split('\\');
            string target = "";
            if (sub[0] == "live" || sub.Length == 1)
            {
                target = sub[sub.Length - 1].Split('_', '.')[3];
            }
            else
            {
                target = sub[0];
            }
            int index = logfile.IndexOf("Unittest");
            index = logfile.IndexOf('\\', index);
            string entry = logfile.Substring(++index);
            lbFailedTests.Items.Add(entry);
        }

        private void LbFailedTests_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string selected = (e.AddedItems[0] as string);
            string[] sub = selected.Split('\\');
            string target = "";
            string fileName = "";
            if (sub[0] == "live" || sub.Length == 1)
            {
                target = sub[sub.Length - 1].Split('_', '.')[3];
            }
            else
            {
                target = sub[0];
            }
            if (sub.Length == 1)
            {
                fileName = sub[0];
            }
            else
            {
                fileName = sub[1] + "\\" + sub[2];
            }
            XmlDocument document = null;
            foreach (FailedUnittest unittest in unittestResults.FailedLogs)
            {
                if (unittest.LogFilePath.Contains(selected))
                {
                    document = unittest.XmlDocument;
                    break;
                }
            }
            DisplayFailedLog(target, fileName, document);
        }

        private void DisplayFailedLog(string target, string fileName, XmlDocument document)
        {
            WindowShowErrors dialog = new WindowShowErrors(target, fileName, document);
            dialog.Title += fileName;
            dialog.ShowDialog();
        }
    }

    internal class UnittestResults
    {
        private List<FailedUnittest> failedLogs = new List<FailedUnittest>();
        private XmlDocument xmlDocument = new XmlDocument();

        public int NumberFailed { get; set; }
        public int NumberTestfiles { get; set; }
        internal List<FailedUnittest> FailedLogs { get => failedLogs; }

        public void CollectUnittestResults(string rootFolder)
        {
            if (!Directory.Exists(rootFolder)) return;
            // scan target files
            string[] logfiles = Directory.GetFiles(rootFolder, "*_out.xml", SearchOption.AllDirectories);
            NumberTestfiles += logfiles.Length;
            NumberFailed += ScanLivetestOutput(logfiles, "Info");
            NumberFailed += ScanLivetestOutput(logfiles, "Error");

            // scan testsudio output files
            logfiles = Directory.GetFiles(rootFolder, "x_*_tests*.xml", SearchOption.AllDirectories);
            NumberTestfiles += logfiles.Length;
            NumberFailed += ScanLivetestOutput(logfiles, "Error");
            NumberFailed += ScanLivetestOutput(logfiles, "FatalError");
        }

        private int ScanLivetestOutput(string[] logfiles, string tagName)
        {
            int countFailedTests = 0;
            int numberFailed = 0;
            foreach (string logfile in logfiles)
            {
                XmlReader xmlReader = XmlReader.Create(logfile);
                xmlDocument = new XmlDocument();
                xmlDocument.Load(xmlReader);
                xmlReader.Close();
                XmlNodeList nodesInfo = xmlDocument.GetElementsByTagName(tagName);
                foreach (XmlNode xmlNode in nodesInfo)
                {
                    foreach (XmlNode child in xmlNode.ChildNodes)
                    {
                        if (child.GetType() == typeof(XmlCDataSection))
                        {
                            string text = (child as XmlCDataSection).InnerText;
                            if (text.Contains("failed"))
                            {
                                numberFailed++;
                            }
                        }
                    }
                }
                if (numberFailed > 0)
                {
                    RemovePassedTestCases();
                    failedLogs.Add(new FailedUnittest(xmlDocument, logfile));
                    countFailedTests++;
                    numberFailed = 0;
                }
            }
            return countFailedTests;
        }

        private void RemovePassedTestCases()
        {
            List<XmlNode> forDeletion = new List<XmlNode>();
            XmlNodeList nodesTestCase = xmlDocument.GetElementsByTagName("TestCase");
            foreach (XmlNode testCase in nodesTestCase)
            {
                if (!testCase.InnerText.Contains("failed"))
                {
                    forDeletion.Add(testCase);
                }
            }
            foreach (XmlNode testCase in forDeletion)
            {
                testCase.ParentNode.RemoveChild(testCase);
            }
        }
    }

    internal  class FailedUnittest
    {
        public FailedUnittest(XmlDocument xmlDocument, string logFilePath)
        {
            XmlDocument = xmlDocument;
            LogFilePath = logFilePath;
        }

        public XmlDocument XmlDocument { get; set; }
        public string LogFilePath { get; set; }
    }
}
