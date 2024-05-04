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
                "Welcome to DOOMSona! Please follow the instructions carefully:\n\n" +
                "- Make sure that Reloaded-II is closed while installing DOOMSona.\n\n" +
                "   - In the background, we will be setting a flag that disables R2's loader injection so that it uses\n" +
                "     ThirteenAG's Ultimate ASI Loader to ensure maximum compatibility\n\n" +
                "- Once the installation finishes, you can open Reloaded-II and launch Persona 5 Royal.\n\n" +
                "You only need to go through this procedure once.\n\n" +
                "For uninstallation, you can relaunch this utility via Reloaded-II's \"Configure Mod\" option.";

            labelModVersion.Text = "Mod version: ";
            string modVersion = InstallerLogic.GetModVersion();
            if (modVersion == null)
            {
                labelModVersion.Text += "None";
                btnUninstall.Enabled = false;
            }
            else
                labelModVersion.Text += modVersion;


            labelInstalledModVersion.Text = "Installed version: ";

            string GameVersion = InstallerLogic.GetGameVersion();
            if (GameVersion == null)
            {
                labelInstalledModVersion.Text += "None";
            }
            else
            {
                labelInstalledModVersion.Text += GameVersion;
            }

            string GamePath = InstallerLogic.GetGamePath();
            if (GamePath != null)
            {
                labelGamePath.Text = GamePath;
            }
            else
            {
                btnConfig.Enabled = false;
                btnUninstall.Enabled = false;
            }
        }

        private void btnConfig_Click(object sender, EventArgs e)
        {
            string GamePath = InstallerLogic.GetGamePath();
            if (GamePath == null)
                return;

            string pathChocoDoomConfig = Path.Combine(GamePath, "DOOMSona", "chocolate-doom-setup.exe");
            if (!File.Exists(pathChocoDoomConfig))
                return;

            // Specify the new working directory
            string newWorkingDir = Path.Combine(GamePath, "DOOMSona");

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
    }
}
