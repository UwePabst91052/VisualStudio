using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
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
    /// Interaktionslogik für Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        private string[] buildNumbers = null;
        private static string folderRootLocal = @"D:\PostMerge\workspace\";
        private static string folderRootRemote = @"\\194.138.158.199\PostMerge\workspace\";
        private static string ptrFolderLocalPinky3 = @"D:\git\WinAC_Plus\Test\PTR\Projects\Postmerge_Pinky3";
        private static string ptrFolderRemotePinky3 = @"\\194.138.158.199\git\WinAC_Plus\Test\PTR\Projects\Postmerge_Pinky3";
        private static string ptrFolderLocalPinky5 = @"D:\git\WinAC_Plus\Test\PTR\Projects\Postmerge_Pinky5";
        private static string ptrFolderRemotePinky5 = @"\\194.138.158.199\git\WinAC_Plus\Test\PTR\Projects\Postmerge_Pinky5";
        private string jenkinsAgent = "Pinky_3";
        private string folderRoot = folderRootRemote;
        private string ptrFolderLocal = ptrFolderLocalPinky3;
        private string ptrFolderRemote = ptrFolderRemotePinky3;
        private string selectedFolder = "";
        private int numberTestFiles = 100;
        string[] ptrProjectFiles;
        private FileSystemWatcher watcher = new FileSystemWatcher();
        public string JenkinsAgent { set => jenkinsAgent = value; }
        public string SelectedFolder { get => selectedFolder; }
        public int NumberTestFiles { get => numberTestFiles; }

        public Window1()
        {
            InitializeComponent();
        }

        private void Window_ContentRendered(object sender, EventArgs e)
        {
            lbPostmergeTests.Items.Add("TargetSetup");
            lbPostmergeTests.Items.Add("TestSuite");

            if (jenkinsAgent == "Pinky_3")
            {
                ptrFolderLocal = ptrFolderLocalPinky3;
                ptrFolderRemote = ptrFolderRemotePinky3;
            }
            else
            {
                ptrFolderLocal = ptrFolderLocalPinky5;
                ptrFolderRemote = ptrFolderRemotePinky5;
            }

            if (rbLocal.IsChecked == true)
            {
                FillPtrProjectsList(ptrFolderLocal);
            }

            if (rbRemote.IsChecked == true)
            {
                FillPtrProjectsList(ptrFolderRemote);
            }

            watcher.Created += OnCreated;
        }

        private void FillPtrProjectsList(string ptrFolder)
        {
            try
            {
                ptrProjectFiles = Directory.GetFiles(ptrFolder, "*.proj.xml");
            }
            catch (IOException ex)
            {
                MessageBox.Show(ex.Message + "\nSwitch to local settings.");
                return;
            }
            lbPtrProjects.Items.Clear();
            foreach (string file in ptrProjectFiles)
            {
                string[] sub = file.Split('\\');
                lbPtrProjects.Items.Add(sub[sub.Length - 1]);
            }
        }

        private void LbPtrProjects_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count == 0)
            {
                labelNumberFiles.Content = "";
                return;
            }

            string fullPath = ptrProjectFiles[(sender as ListBox).Items.IndexOf(e.AddedItems[0])];
            XmlReader xmlReader = XmlReader.Create(fullPath);
            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.Load(xmlReader);
            xmlReader.Close();
            XmlNodeList testFiles = xmlDocument.GetElementsByTagName("Testitem");
            numberTestFiles = testFiles.Count;
            labelNumberFiles.Content = numberTestFiles.ToString() + " test files";
        }

        private void OnCreated(object sender, FileSystemEventArgs e)
        {
            Dispatcher.Invoke(() => labelSelectedFolder.Content = e.FullPath);
            selectedFolder = e.FullPath;
            if (selectedFolder.Contains("logs"))
            {
                Dispatcher.Invoke(() => Close());
            }
        }

        private void LbBuildNumbers_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            selectedFolder = folderRoot + (string)e.AddedItems[0] + @"\logs";
            labelSelectedFolder.Content = selectedFolder;
        }

        private void LbPostmergeTests_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count == 0)
            {
                lbBuildNumbers.Items.Clear();
                return;
            }
            string testType = (string)e.AddedItems[0];
            if (testType != "")
            {
                buildNumbers = Directory.GetDirectories(folderRoot);
            }

            if ((bool)cbWaitFolder.IsChecked)
            {
                ActivateFolderWatch(testType);
            }

            selectedFolder = testType;
            lbBuildNumbers.Items.Clear();
            if (null != buildNumbers)
            {
                for (int i = buildNumbers.Length - 1; i >= 0; i--)
                {
                    int buildNumber;
                    string item = buildNumbers[i];
                    string[] sub = item.Split('\\');
                    if (int.TryParse(sub[sub.Length - 1], out buildNumber))
                    {
                        lbBuildNumbers.Items.Add(sub[sub.Length - 1] + "\\" + selectedFolder);
                    }
                }
            }
            else
            {
                MessageBox.Show("No type selected");
            }
        }

        private void CbWaitFolder_Checked(object sender, RoutedEventArgs e)
        {
            string testType = "";
            if (lbPostmergeTests.SelectedIndex >= 0)
            {
                testType = lbPostmergeTests.SelectedItem as string;
            }
            else return;

            ActivateFolderWatch(testType);
        }

        private void ActivateFolderWatch(string testType)
        {
            string logFolder = folderRoot + testType;
            watcher.Path = logFolder;
            watcher.EnableRaisingEvents = true;
            watcher.IncludeSubdirectories = true;
        }

        private void GroupLogfileLocation_Checked(object sender, RoutedEventArgs e)
        {
            if ((sender as RadioButton).Name == "rbLocal")
            {
                FillPtrProjectsList(ptrFolderLocal);
                folderRoot = folderRootLocal;
            }

            if ((sender as RadioButton).Name == "rbRemote")
            {
                FillPtrProjectsList(ptrFolderRemote);
                folderRoot = folderRootRemote;
            }

            lbBuildNumbers.Items.Clear();
        }

    }
}
