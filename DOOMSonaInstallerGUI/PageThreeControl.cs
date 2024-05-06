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
using static DOOMSonaInstallerGUI.Form1;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace DOOMSonaInstallerGUI
{
    public partial class PageThreeControl : UserControl
    {
        private BackgroundWorker installerWorker;
        public event FinalButtonFlowNotifier btnNotifier;
        private bool bLogFileReady = false;

        private DialogResult ShowDoneDialog()
        {
            return MessageBox.Show("DOOMSona v1.0.0 was removed. Press OK to continue.", "Information", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
        }

        public PageThreeControl()
        {
            InitializeComponent();
            InitializeInstallerWorker();

            //if (InstallerLogic.bUninstallMode)
            //{
                labelTitle.Text = "Cleaning up old files...";
                labelDescription.Text = "Cleaning up old files, please wait...";
            //}
            //else
            //{
            //    labelTitle.Text = "Installing...";
            //    labelDescription.Text = "Installing, please wait...";
            //}

            try
            {
                if (File.Exists("DOOMSonaInstallerGUI.log"))
                    File.Delete("DOOMSonaInstallerGUI.log");
                bLogFileReady = true;
            }
            catch(Exception ex)
            {
                LogMessage("WARNING: Failed to open the log file, exception: " + ex);
            }

        }

        private void InitializeInstallerWorker()
        {
            installerWorker = new BackgroundWorker();
            installerWorker.WorkerReportsProgress = true;

            installerWorker.DoWork += InstallerWorker_DoWork;
            installerWorker.ProgressChanged += InstallerWorker_ProgressChanged;
            installerWorker.RunWorkerCompleted += InstallerWorker_RunWorkerCompleted;
        }

        private void InstallerWorker_DoUninstallWork(object sender, DoWorkEventArgs e, BackgroundWorker worker)
        {
            LogMessage("Performing cleanup...");
            int clnResult = InstallerLogic.PerformCleanup(InstallerLogic.GetGamePath());
            if (clnResult != 0)
            {
                LogMessage("Cleanup failed. PerformCleanup result: " + clnResult);
            }

            float pct = (1.0f / 4.0f) * 100.0f;
            worker.ReportProgress(Convert.ToInt32(pct), $"Cleaning... {pct}%");

            if (InstallerLogic.bCleanupASILoader)
            {
                int clnAsiResult = InstallerLogic.PerformASILoaderCleanup(InstallerLogic.GetGamePath());
                if (clnAsiResult != 0)
                {
                    LogMessage("ASI Loader cleanup failed. PerformASILoaderCleanup result: " + clnResult);
                }

                pct = (2.0f / 4.0f) * 100.0f;
                worker.ReportProgress(Convert.ToInt32(pct), $"Cleaning... {pct}%");

                int clnBootResult = InstallerLogic.PerformASIBootstrapperCleanup(InstallerLogic.GetGamePath());
                if (clnBootResult != 0)
                {
                    LogMessage("ASI Boostrap cleanup failed. PerformASIBootstrapperCleanup result: " + clnResult);
                }

                pct = (3.0f / 4.0f) * 100.0f;
                worker.ReportProgress(Convert.ToInt32(pct), $"Cleaning... {pct}%");
            }

            pct = (4.0f / 4.0f) * 100.0f;
            worker.ReportProgress(Convert.ToInt32(pct), $"Cleaning... {pct}%");

            LogMessage("Cleanup completed.");
        }

        private void InstallerWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            //if (InstallerLogic.bUninstallMode)
                LogMessage("Starting cleanup...");
            //else
            //    LogMessage("Starting installation...");

            LogMessage("Destination: " + InstallerLogic.GetGamePath());

            //if (InstallerLogic.bUninstallMode)
            //{
                InstallerWorker_DoUninstallWork(sender, e, worker);
                return;
            //}
            // else
            // {
            //     if (InstallerLogic.GetGameVersion() != null)
            //     {
            //         InstallerLogic.bCleanupASILoader = true;
            //         InstallerWorker_DoUninstallWork(sender, e, worker);
            //     }
            // 
            //     LogMessage("Extracting DOOMSona ZIP...");
            //     int installResult = InstallerLogic.PerformInstall("DOOMSona.zip", InstallerLogic.GetGamePath(), ".");
            //     if (installResult != 0)
            //     {
            //         e.Result = installResult;
            //         e.Cancel = true;
            //         LogMessage("Installation failed. PerformInstall result: " + installResult);
            //         return;
            //     }
            // 
            //     float pct = (1.0f / 3.0f) * 100.0f;
            //     worker.ReportProgress(Convert.ToInt32(pct), $"Installing... {pct}%");
            // 
            // 
            //     LogMessage("Tagging installation with mod version...");
            //     int tagResult = InstallerLogic.TagInstallationVersion(InstallerLogic.GetGamePath());
            //     if (tagResult != 0)
            //     {
            //         e.Result = tagResult;
            //         e.Cancel = true;
            //         LogMessage("Installation failed. TagInstallationVersion result: " + tagResult);
            //         return;
            //     }
            // 
            //     pct = (2.0f / 3.0f) * 100.0f;
            //     worker.ReportProgress(Convert.ToInt32(pct), $"Installing... {pct}%");
            // 
            //     LogMessage("Installing Reloaded-II Bootstrapper ASI & setting DontInject flag...");
            //     int rldResult = InstallerLogic.Reloaded_InstallBootstrap(InstallerLogic.GetGamePath());
            //     if (rldResult != 0)
            //     {
            //         e.Result = rldResult;
            //         e.Cancel = true;
            //         LogMessage("Installation failed. Reloaded_InstallBootstrap result: " + rldResult);
            //         return;
            //     }
            // 
            //     pct = (3.0f / 3.0f) * 100.0f;
            //     worker.ReportProgress(Convert.ToInt32(pct), $"Installing... {pct}%");
            // 
            //     LogMessage("Installation completed.");
            // }
        }

        private void InstallerWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            // Update UI with progress value
            progressBar1.Value = e.ProgressPercentage;
            labelDescription.Text = e.UserState.ToString();
            LogMessage(e.UserState.ToString()); // Log progress message
        }

        private void InstallerWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if ((e.Error != null) || (e.Cancelled))
            {
                //labelDescription.Text = "";
                //if (InstallerLogic.bUninstallMode)
                //{
                    labelTitle.Text = "Cleanup Failed";
                    LogMessage("Cleanup failed.");
                //}
                //else
                //{
                //    labelTitle.Text = "Installation Failed";
                //    LogMessage("Installation failed.");
                //}
            }
            else
            {
                //if (InstallerLogic.bUninstallMode)
                //{
                    labelDescription.Text = "";
                    labelTitle.Text = "Cleanup Complete";
               // }
                //else
               // {
                //    labelDescription.Text = "";
                //    labelTitle.Text = "Installation Complete";
                //}
            }

            InstallerLogic.bInstallCompleted = true;

            ShowDoneDialog();

            if (InstallerLogic.IsCmdFlagPresent("--autoProceed"))
            {
                InstallerLogic.DoExit();
            }

            btnNotifier?.Invoke(true);
        }

        private void LogMessage(string message)
        {
            if (textBoxLog.InvokeRequired)
            {
                textBoxLog.Invoke(new Action<string>(LogMessage), new object[] { message });
            }
            else
            {
                textBoxLog.AppendText($"{DateTime.Now}: {message}{Environment.NewLine}");
                if (bLogFileReady)
                    File.AppendAllText("DOOMSonaInstallerGUI.log", $"{DateTime.Now}: {message}{Environment.NewLine}");
            }
        }

        private void PageThreeControl_Load(object sender, EventArgs e)
        {
            InstallerLogic.RegisterLogger(LogMessage);
            if (!installerWorker.IsBusy)
            {
                installerWorker.RunWorkerAsync();
            }

            // int installResult = InstallerLogic.PerformInstall("DOOMSona.zip", InstallerLogic.GetGamePath());
            // if (installResult != 0)
            // {
            //     return;
            // }
            // 
            // labelDescription.Text = "Tagging installation with mod version...";
            // 
            // int tagResult = InstallerLogic.TagInstallationVersion(InstallerLogic.GetGamePath());
            // if (tagResult != 0)
            // {
            //     return;
            // }
            // 
            // labelDescription.Text = "Installing Reloaded-II Bootstrapper ASI & setting DontInject flag...";
            // InstallerLogic.Reloaded_InstallBootstrap(InstallerLogic.GetGamePath());
        }
    }
}
