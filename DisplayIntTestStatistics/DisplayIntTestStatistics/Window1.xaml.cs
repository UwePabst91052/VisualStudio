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
        private static string folderRootLocal = @"D:\Jenkins_Home\workspace\PCStation\";
        private static string folderRootRemote = @"\\md2fmd5c\Jenkins_Home\workspace\PCStation\";
        private static string ptrFolderLocal = @"D:\IntegrationTestExecution\PTRProjects\Projects\PCStationIntegrationTest";
        private static string ptrFolderRemote = @"\\md2fmd5c\IntegrationTestExecution\PTRProjects\Projects\PCStationIntegrationTest";
        private string folderRoot = folderRootRemote;
        private string selectedFolder = "";
        private int numberTestFiles = 100;
        string[] ptrProjectFiles;
        private FileSystemWatcher watcher = new FileSystemWatcher();
        public string SelectedFolder { get => selectedFolder; }
        public int NumberTestFiles { get => numberTestFiles; }

        public Window1()
        {
            InitializeComponent();
        }

        private void Window_ContentRendered(object sender, EventArgs e)
        {
            lbLivetests.Items.Add("RT_DB_Job_Test1_Live");
            lbLivetests.Items.Add("RT_Merge_Job3_Tests2_Live");
            lbLivetests.Items.Add("RT_ST_Job_Test1_Live");

            lbAcceptance.Items.Add("RT_DB_Job_Test2_Acceptance");
            lbAcceptance.Items.Add("RT_Merge_Job4_Tests3_Merge");
            lbAcceptance.Items.Add("RT_ST_Job_Test2_Acceptance");

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

        private void LbTestType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count == 0)
            {
                lbBuildNumbers.Items.Clear();
                return;
            }
            string testType = (string)e.AddedItems[0];
            if (testType != "")
            {
                selectedFolder = folderRoot + testType;
                buildNumbers = Directory.GetDirectories(selectedFolder);
            }

            if ((bool)cbWaitFolder.IsChecked)
            {
                ActivateFolderWatch(testType);
            }

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
                        lbBuildNumbers.Items.Add(sub[sub.Length - 2] + "\\" + sub[sub.Length - 1]);
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
            if (lbLivetests.SelectedIndex >= 0)
            {
                testType = lbLivetests.SelectedItem as string;
            }
            else if (lbAcceptance.SelectedIndex >= 0)
            {
                testType = lbAcceptance.SelectedItem as string;
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
