using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.IO;
using System.Xml;
using System.Windows.Threading;
using System.ComponentModel;
using System.Diagnostics;

namespace DisplayIntTestStatistics
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public struct TargetResults
    {
        public TextBox tbTarget;
        public TextBox tbNumber;
        public TextBox tbSum;
        public ListBox lbFiles;
    }
    public partial class MainWindow : Window
    {
        private TargetResults[] targetResults = new TargetResults[3];
        private PtrResultContent ptrResultContent = null;
        private BackgroundWorker collectThread = new BackgroundWorker();
        private int numberFilesScanned = 0;
        private FileSystemWatcher watcher = new FileSystemWatcher();
        private bool isObservationOn = false;
        private int maxNumberFiles = 0;
        private string jenkinsAgent = "Pinky_3";

        public MainWindow()
        {
            InitializeComponent();
        }

        private void MainWindow_ContentRendered(object sender, EventArgs e)
        {
            TargetResults results = new TargetResults
            {
                tbTarget = tbTarget1,
                tbNumber = tbNumber1,
                tbSum = tbSum1,
                lbFiles = lbFiles1
            };
            targetResults[0] = results;
            results = new TargetResults
            {
                tbTarget = tbTarget2,
                tbNumber = tbNumber2,
                tbSum = tbSum2,
                lbFiles = lbFiles2
            };
            targetResults[1] = results;
            results = new TargetResults
            {
                tbTarget = tbTarget3,
                tbNumber = tbNumber3,
                tbSum = tbSum3,
                lbFiles = lbFiles3
            };
            targetResults[2] = results;
            collectThread.WorkerReportsProgress = true;
            collectThread.DoWork += workerDoWork;
            collectThread.ProgressChanged += workerProgressChanged;
            collectThread.RunWorkerCompleted += workerRunWorkerCompleted;
            watcher.Created += ObserveLogFolder;
            ResetTargetResults();
        }

        private void OnBtnStartClicked(object sender, RoutedEventArgs e)
        {
            string resultPath = tbTestResultsPath.Text;
            string[] resultFiles = Directory.GetFiles(resultPath, "*.result");
            maxNumberFiles = resultFiles.Length;
            pbCollectData.Maximum = maxNumberFiles;
            pbCollectData.Value = 0;
            collectThread.RunWorkerAsync(resultFiles);
        }

        private void OnBtnObserveClicked(object sender, RoutedEventArgs e)
        {
            if (isObservationOn)
            {
                isObservationOn = false;
                btnObserve.Content = "Observe On";
                watcher.EnableRaisingEvents = false;
                tbObserve.Text = "Off";
            }
            else
            {
                isObservationOn = true;
                btnObserve.Content = "Observe Off";
                string resultPath = tbTestResultsPath.Text;
                watcher.Path = tbTestResultsPath.Text;
                watcher.EnableRaisingEvents = true;
                tbObserve.Text = "On";
                numberFilesScanned = 0;
                string[] resultFiles = Directory.GetFiles(tbTestResultsPath.Text, "*.result");
                numberFilesScanned += resultFiles.Length;
                pbCollectData.Maximum = maxNumberFiles;
                collectThread.RunWorkerCompleted += workerRunObserverCompleted;
                collectThread.RunWorkerAsync(resultFiles);
            }
        }

        private void ResetTargetResults()
        {
            foreach (TargetResults results in targetResults)
            {
                results.tbTarget.Text = "0";
                results.tbNumber.Text = "0";
                results.tbSum.Text = "0";
                results.lbFiles.Items.Clear();
            }
        }

        private void workerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            UpdateStatistics();
        }

        private void workerRunObserverCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            ResetTargetResults();
            int numberFailedTests = UpdateStatistics();
            pbCollectData.Maximum = maxNumberFiles + numberFailedTests;
            if (numberFilesScanned > 0)
            {
                string statusText = string.Format("{0} files from {1} scanned",
                    numberFilesScanned, maxNumberFiles + numberFailedTests);
                pbCollectData.Value = numberFilesScanned;
                UpdateStatus(statusText);
            }
        }
        private int UpdateStatistics()
        {
            int index = 0;
            int numberFailedTests = 0;
            foreach (string target in ptrResultContent.TargetList)
            {
                numberFailedTests += ptrResultContent.FillStatistic(target,
                    targetResults[index].tbTarget, targetResults[index].tbNumber,
                    targetResults[index].tbSum, targetResults[index].lbFiles);
                index++;
            }
            return numberFailedTests;
        }

        private void UpdateStatus(string text)
        {
            labelStatus.Content = text;
        }
        private void workerProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            pbCollectData.Value = e.ProgressPercentage;
            string statusText = string.Format("{0} files from {1} scanned", 
                e.ProgressPercentage, maxNumberFiles);
            UpdateStatus(statusText);
        }

        private void workerDoWork(object sender, DoWorkEventArgs e)
        {
            CollectThread(sender, (string[])e.Argument);
        }

        private void CollectThread(object sender, string[] resultXmlFiles)
        {
            ptrResultContent = new PtrResultContent(targetResults);
            ptrResultContent.CollectResultContent(sender, resultXmlFiles);
            string statusText = string.Format("{0} files were scanned", resultXmlFiles.Length);
            Dispatcher.Invoke(() => UpdateStatus(statusText));
        }

        private void ObserveLogFolder(object sender, FileSystemEventArgs e)
        {
            if (!e.FullPath.Contains(".result")) return;
            ptrResultContent.CollectResultContent(e.FullPath);
            Dispatcher.Invoke(() => ResetTargetResults());
            int numberFailedTests = Dispatcher.Invoke(() => UpdateStatistics());
            string statusText = string.Format("{0} files from {1} scanned", ++numberFilesScanned, maxNumberFiles);
            Dispatcher.Invoke(() => pbCollectData.Maximum = maxNumberFiles + numberFailedTests);
            Dispatcher.Invoke(() => pbCollectData.Value = numberFilesScanned);
            Dispatcher.Invoke(() => UpdateStatus(statusText));
        }

        private void OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            foreach (string testSuite in e.AddedItems)
            {
                string[] sub = testSuite.Split('.');
                string search = sub.Last();
                string target = "";
                bool remote = tbTestResultsPath.Text.Contains("194.138.");
                switch ((sender as ListBox).Name)
                {
                    case "lbFiles1":
                        target = tbTarget1.Text;
                        break;
                    case "lbFiles2":
                        target = tbTarget2.Text;
                        break;
                    case "lbFiles3":
                        target = tbTarget3.Text;
                        break;
                    default:
                        break;
                }
                string Url = ptrResultContent.SearchForUrl(target, search, remote, jenkinsAgent);
                try
                {
                    Process.Start(Url);
                }
                catch (Win32Exception noBrowser)
                {
                    MessageBox.Show(noBrowser.Message);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void BtnSelectFolder_Click(object sender, RoutedEventArgs e)
        {
            Window1 selectPoUp = new Window1();
            selectPoUp.ShowDialog();
            if (selectPoUp.SelectedFolder != "")
            {
                tbTestResultsPath.Text = selectPoUp.SelectedFolder;
                watcher.Path = tbTestResultsPath.Text;
                maxNumberFiles = selectPoUp.NumberTestFiles;
                pbCollectData.Maximum = maxNumberFiles;
                pbCollectData.Value = 0;
            }
            jenkinsAgent = selectPoUp.JenkinsAgent;
        }

        private void BtnUnittests_Click(object sender, RoutedEventArgs e)
        {
            WindowUnittests windowUnittests = new WindowUnittests();
            windowUnittests.ResultPath = tbTestResultsPath.Text + @"\Unittests";
            windowUnittests.ShowDialog();
        }
    }

    internal class PtrResultContent
    {
        private XmlDocument xmlDocument = new XmlDocument();
        private Dictionary<string, List<PtrResultDetail>> resultTargets = new Dictionary<string, List<PtrResultDetail>>(3);
        private Statistics statistics = new Statistics();
        private TargetResults[] targetResults = null;

        private class Statistics
        {
            public int Passed { get; set; }
            public int Failed { get; set; }
        }

        public PtrResultContent(TargetResults[] results)
        {
            targetResults = results;
        }

        public Dictionary<string, List<PtrResultDetail>>.KeyCollection TargetList
        {
            get
            {
                return resultTargets.Keys;
            }
        }

        public void CollectResultContent(string resultXmlFile)
        {
            while (true)
            {
                try
                {
                    XmlReader reader = XmlReader.Create(resultXmlFile);
                    xmlDocument.Load(reader);
                    reader.Close();
                    ExtractResultDetails();
                    break;
                }
                catch (IOException ex)
                {
                    Console.WriteLine(ex.Message);
                    Thread.Sleep(100);
                }
            }
        }

        public void CollectResultContent(object sender, string[] resultXmlFiles)
        {
            int counter = 0;
            foreach (string filePath in resultXmlFiles)
            {
                XmlReader reader = XmlReader.Create(filePath);
                xmlDocument.Load(reader);
                reader.Close();
                ExtractResultDetails();
                if (null != sender)
                {
                    counter++;
                    (sender as BackgroundWorker).ReportProgress(counter);
                }
                //Thread.Sleep(100);
           }
        }
        private void ExtractResultDetails()
        {
            PtrResultDetail detail = new PtrResultDetail();
            XmlNode root = xmlDocument.DocumentElement;
            detail.TestName = root.Attributes["Name"].Value;
            XmlNodeList allKeys = xmlDocument.GetElementsByTagName("Key");
            foreach (XmlNode xmlNode in allKeys)
            {
                switch (xmlNode.InnerText)
                {
                    case "Result":
                        detail.Result = xmlNode.NextSibling.InnerText;
                        break;
                    case "LocalTargetName":
                        detail.Target = xmlNode.NextSibling.InnerText;
                        break;
                    case "Folder":
                        detail.Folder = xmlNode.NextSibling.InnerText;
                        break;
                    case "LogFilePath":
                        detail.LogFile = xmlNode.NextSibling.InnerText;
                        break;
                    default:
                        break;
                }
            }
            if (resultTargets.TryGetValue(detail.Target, out List<PtrResultDetail> value))
            {
                value.Add(detail);
            }
            else
            {
                List<PtrResultDetail> item = new List<PtrResultDetail>();
                item.Add(detail);
                resultTargets.Add(detail.Target, item);
            }
        }

        public int FillStatistic(string target, TextBox tbTarget, TextBox tbNumber, TextBox tbSum, ListBox lbFiles)
        {
            int numberFailed = 0;
            int sumFiles = 0;
            tbTarget.Text = target;
            if (resultTargets.TryGetValue(target, out List<PtrResultDetail> value))
            {
                foreach (var detail in value)
                {
                    string result = detail.Result;
                    AddToListBox(lbFiles, detail);
                    sumFiles++;
                }
                numberFailed = lbFiles.Items.Count;
                tbNumber.Text = numberFailed.ToString();
                tbSum.Text = sumFiles.ToString();
            }
            return numberFailed;
        }

        private void AddToListBox(ListBox lbFiles, PtrResultDetail detail)
        {
            string testSuite = AssembleTestSuite(detail);
            if (lbFiles.Items.Contains(testSuite))
            {   
                // remove repeated passed tests or skip repeated failed tests
                if (detail.Result == "Passed")
                {
                    lbFiles.Items.Remove(testSuite);
                }
            }
            else
            {
                if (detail.Result != "Passed")
                {
                    lbFiles.Items.Add(testSuite);
                }
            }
        }

        private string AssembleTestSuite(PtrResultDetail detail)
        {
            string testSuite = "";
            string[] subs = detail.Folder.Split('\\');
            for (int i = 3; i < subs.Length; i++)
            {
                testSuite += subs[i] + ".";
            }
            testSuite += detail.TestName;
            return testSuite;
        }

        internal string SearchForUrl(string target, string search, bool remote, string jenkinsAgent)
        {
            string logFileUrl = "";
            if (resultTargets.TryGetValue(target, out List<PtrResultDetail> value))
            {
                string logFilePath = value.Find(x => x.TestName.Equals(search)).LogFile;
                string[] sub = logFilePath.Split('\\', '.');
                if (remote)
                {
                    if (jenkinsAgent == "Pinky_3")
                    {
                        sub[0] = "file://194.138.158.199";
                    }
                    else
                    {
                        sub[0] = "file://194.138.159.1";
                    }
                }
                else
                {
                    sub[0] = "file:D:";
                }
                sub[sub.Length - 1] = ".html";
                logFileUrl = sub[0];
                for (int i = 1; i < sub.Length - 1; i++)
                {
                    logFileUrl += "/" + sub[i];
                }
                logFileUrl += sub[sub.Length - 1];
            }
            return logFileUrl;
        }
    }

    internal class PtrResultDetail
    {
        public string TestName { get; set; }
        public string Result { get; set; }
        public string Folder { get; set; }
        public string Target { get; set; }
        public string LogFile { get; set; }

        public PtrResultDetail()
        {
        }
    }
}
