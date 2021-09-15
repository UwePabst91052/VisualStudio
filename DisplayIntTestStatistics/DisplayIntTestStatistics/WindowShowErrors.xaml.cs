using System;
using System.Collections.Generic;
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
    /// Interaktionslogik für WindowShowErrors.xaml
    /// </summary>
    public partial class WindowShowErrors : Window
    {
        private string target;
        private string fileName;
        private XmlDocument document;

        public WindowShowErrors()
        {
            InitializeComponent();
        }

        public WindowShowErrors(string target, string fileName, XmlDocument document)
        {
            this.target = target;
            this.fileName = fileName;
            this.document = document;

            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            ShowResultInTable();
        }

        private void ShowResultInTable()
        {
            cellTarget.Blocks.Add(new Paragraph(new Run(target)));
            cellFilename.Blocks.Add(new Paragraph(new Run(fileName)));
            Table tableXmlOutput = fd.Blocks.ElementAt(0) as Table;
            tableXmlOutput.RowGroups.Add(DisplayXmlOutput());
        }

        private TableRowGroup DisplayXmlOutput()
        {
            TableRowGroup rowGroup = new TableRowGroup();
            XmlNodeList testCases = document.GetElementsByTagName("TestCase");
            foreach (XmlNode testCase in testCases)
            {
                TableRow tableRow = new TableRow();

                TableCell tableCell = new TableCell();
                Section sectionInfo = new Section();
                if (fileName.Contains("Confirm"))
                {
                    // in confirmation output get also test suite name
                    Paragraph paragraphSuite = new Paragraph(new Run(testCase.ParentNode.Attributes["name"].Value));
                    Paragraph paragraphCase = new Paragraph(new Run(testCase.Attributes["name"].Value));
                    paragraphCase.TextIndent = 30.0;
                    sectionInfo.Blocks.Add(paragraphSuite);
                    sectionInfo.Blocks.Add(paragraphCase);
                }
                else
                {
                    // in main output only 
                    Paragraph paragraphCase = new Paragraph(new Run(testCase.Attributes["name"].Value));
                    sectionInfo.Blocks.Add(paragraphCase);
                }
                tableCell.Blocks.Add(sectionInfo);
                tableRow.Cells.Add(tableCell);

                tableCell = new TableCell();
                sectionInfo = new Section();
                foreach (XmlNode info in testCase.ChildNodes)
                {
                    if (info.InnerText.Contains("failed"))
                    {
                        sectionInfo.Blocks.Add(new Paragraph(new Run(info.InnerText)));
                    }
                }
                tableCell.Blocks.Add(sectionInfo);
                tableRow.Cells.Add(tableCell);

                rowGroup.Rows.Add(tableRow);
            }

            return rowGroup;
        }
    }
}
