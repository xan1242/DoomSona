using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DOOMSonaInstallerGUI
{
    public partial class Form1 : Form
    {
        public delegate void PageAdvancer();
        public delegate void FinalButtonFlowNotifier(bool nextBtnState);
        List<UserControl> pagesList = new List<UserControl>();
        private int currentPageIndex = 0;

        private PageOneControl page1;
        private PageTwoControl page2;
        private PageThreeControl page3;
        //private PageFourControl page4;

        private void InitializePagesList()
        {
            page1 = new PageOneControl();
            page2 = new PageTwoControl();
            page3 = new PageThreeControl();
            //page4 = new PageFourControl();

            //page1.pageAdv += AdvancePage;
            page3.btnNotifier += SetFinalButtonFlow;
            //page3.pageAdv += AdvancePage;

            pagesList.Add(page1);
            pagesList.Add(page2);
            pagesList.Add(page3);
            //pagesList.Add(page4);
        }

        private DialogResult ShowCancelDialog()
        {
            if (InstallerLogic.bInstallCompleted)
            {
                return DialogResult.Yes;
            }

            return MessageBox.Show("Are you sure you want to cancel the installation?", "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
        }

        private DialogResult ShowRldRunningError()
        {
            return MessageBox.Show("An application with the name \"Reloaded-II\" is found to be running.\n\nPlease close the application first before proceeding.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private DialogResult ShowRld32RunningError()
        {
            return MessageBox.Show("An application with the name \"Reloaded-II32\" is found to be running.\n\nPlease close the application first before proceeding.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        public Form1()
        {
            InitializeComponent();
            InitializePagesList();
        }

        private void AdvancePage()
        {
            if (currentPageIndex == 0)
            {
                if (InstallerLogic.IsProcessRunning("Reloaded-II"))
                {
                    ShowRldRunningError();
                    return;
                }

                if (InstallerLogic.IsProcessRunning("Reloaded-II32"))
                {
                    ShowRld32RunningError();
                    return;
                }
            }

            if (currentPageIndex < pagesList.Count - 1)
            {
                EnableAllFlowButtons();
                currentPageIndex++;
                ShowCurrentPage();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ShowCurrentPage();
            SetBtnNextTextForFirstPage();
        }

        private void SetBtnNextTextForFirstPage()
        {
            string gameModVersion = InstallerLogic.GetGameVersion();
            if (gameModVersion != null)
            {
                if (gameModVersion.Length < 1)
                {
                    btnNext.Text = "Next";
                    return;
                }

                string modVersion = InstallerLogic.GetModVersion();
                if (modVersion != gameModVersion)
                    btnNext.Text = "Update";
                else
                    btnNext.Text = "Reinstall";
                return;
            }

            btnNext.Text = "Next";
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            if (currentPageIndex == pagesList.Count - 1)
            {
                Application.Exit();
            }

            AdvancePage();
        }

        private void btnBack_Click(object sender, EventArgs e)
        {
            if (currentPageIndex > 0)
            {
                currentPageIndex--;
                ShowCurrentPage();
            }
        }

        private void DisableAllFlowButtons()
        {
            btnBack.Enabled = false;
            btnNext.Enabled = false;
        }

        private void EnableAllFlowButtons()
        {
            btnBack.Enabled = true;
            btnNext.Enabled = true;
        }

        private void SetFinalButtonFlow(bool nextEnabled)
        {
            btnBack.Enabled = false;
            btnBack.Visible = false;
            btnCancel.Enabled = false;
            btnCancel.Visible = false;

            btnNext.Enabled = nextEnabled;
            btnNext.Visible = true;

            btnNext.Text = "Finish";
        }

        private void ShowCurrentPage()
        {
            // Clear panel and show the current page
            ClearPanelControls(panel1);
            UserControl currentPage = pagesList[currentPageIndex];
            panel1.Controls.Add(currentPage);
            currentPage.Visible = true;

            switch(currentPageIndex)
            {
                case 2:
                    SetFinalButtonFlow(false);
                    break;
                case 1:
                    btnNext.Text = "Install";
                    break;
                case 0:
                    SetBtnNextTextForFirstPage();
                    btnBack.Enabled = false;
                    break;
                default:
                    btnNext.Text = "Next";
                    break;
            }
        }

        private void ClearPanelControls(Panel panel)
        {
            panel.Controls.Clear();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            if (ShowCancelDialog() == DialogResult.Yes)
            {
                Application.Exit();
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                DialogResult result = ShowCancelDialog();
                if (result == DialogResult.No)
                {
                    e.Cancel = true;
                }
            }
        }
    }
}
