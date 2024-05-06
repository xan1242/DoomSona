using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static DOOMSonaInstallerGUI.Form1;

namespace DOOMSonaInstallerGUI
{
    public partial class PageOneControl : UserControl
    {
        public PageOneControl()
        {
            InitializeComponent();
            labelDescription.Text =
                "On this page, you can:\n\n" +
                "- Configure Chocolate DOOM by pressing \"Configure DOOM\"\n" +
                "- Open the DOOMSona root directory by pressing \"DOOM Root Dir...\"\n\n" +
                "On the next page, you can manage the installed WADs in your game as well as the command-line parameters.";

            labelModVersion.Text = "Mod version: ";
            string modVersion = InstallerLogic.GetModVersion();
            if (modVersion == null)
            {
                labelModVersion.Text += "None";
                btnOpenDir.Enabled = false;
            }
            else
                labelModVersion.Text += modVersion;

            string GamePath = InstallerLogic.GetGamePath();
            if (GamePath != null)
            {
                labelGamePath.Text = GamePath;
            }
            else
            {
                btnConfig.Enabled = false;
                btnOpenDir.Enabled = false;
            }

            toolTipRootDir.SetToolTip(btnOpenDir, "Opens the DOOMSona root directory in your file browser.");
            toolTipConfig.SetToolTip(btnConfig, "Opens the Chocolate DOOM configurator.");
        }

        private void btnConfig_Click(object sender, EventArgs e)
        {
            if (!Directory.Exists("DOOMSona"))
                return;

            string pathChocoDoomConfig = Path.Combine("DOOMSona", "chocolate-doom-setup.exe");
            if (!File.Exists(pathChocoDoomConfig))
                return;

            // Specify the new working directory
            string newWorkingDir = "DOOMSona";

            // Create a ProcessStartInfo object
            ProcessStartInfo psi = new ProcessStartInfo
            {
                FileName = pathChocoDoomConfig,
                WorkingDirectory = newWorkingDir,
                UseShellExecute = false,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                CreateNoWindow = false
            };

            // Start the process
            Process process = new Process { StartInfo = psi };
            process.Start();
        }

        private void btnOpenDir_Click(object sender, EventArgs e)
        {
            if (Directory.Exists("DOOMSona"))
                Process.Start("DOOMSona");
        }
    }
}
