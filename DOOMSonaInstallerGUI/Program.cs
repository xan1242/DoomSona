using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DOOMSonaInstallerGUI
{
    internal static class Program
    {
        private static DialogResult ShowNoArgError()
        {
            return MessageBox.Show("No arguments were passed to the installer nor was a game path found in Reloaded-II mod loader.\n\n" +
                "This is usually because you launched it manually or something went wrong.\n\n" +
                "You can relaunch the installer by passing the path to the game directory as the \"--path\" argument and trying again.\n\n" +
                "You can also pass \"--help\" to show more.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private static DialogResult ShowNoModVerError()
        {
            return MessageBox.Show("Can't read the mod version from ModConfig.json.\n\n" +
                "Check if this installer is in the Reloaded-II mod directory and try again.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private static DialogResult ShowGameRunningError()
        {
            return MessageBox.Show("An application with the name \"P5R\" is running (assuming this is the game).\n\n" +
                "Please close it before launching this installer.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private static DialogResult ShowHelpDialog()
        {
            return MessageBox.Show("--help = Show this help\n\n" +
                "--path PathToGame = Define the destination/game path\n\n" +
                "--launchGame = Launches Persona 5 Royale through Reloaded-II after finishing the installation\n\n" +
                "--disableGameRunningCheck = Disables checks if the game is already running or not\n\n" +
                "--cleanup = Immediately starts the cleanup procedure (removes v1.0.0 old files)\n\n" +
                "--autoProceed = Automatically closes when cleanup finishes.", 
                "Commandline Help", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
        }

        private static Mutex mutex = new Mutex(true, "{E4F9E88C-C48D-4901-AF12-E017BB65FC43}");

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            if (mutex.WaitOne(TimeSpan.Zero, true))
            {
                if (InstallerLogic.IsCmdFlagPresent("--help"))
                {
                    ShowHelpDialog();
                    return;
                }

                if (InstallerLogic.IsProcessRunning("P5R") && !InstallerLogic.IsCmdFlagPresent("--disableGameRunningCheck"))
                {
                    ShowGameRunningError();
                    return;
                }

                if (InstallerLogic.IsCmdFlagPresent("--cleanup"))
                {
                    string GamePath = InstallerLogic.GetGamePath();
                    if (GamePath == null)
                    {
                        ShowNoArgError();
                        return;
                    }

                    InstallerLogic.bGotoUninstall = true;
                    InstallerLogic.bUninstallMode = true;
                }
                else
                {
                    if (InstallerLogic.GetModVersion() == null)
                    {
                        ShowNoModVerError();
                        return;
                    }
                }

                InstallerLogic.DOOMArgs = InstallerLogic.ReadFileAndFlatten(Path.Combine("DOOMSona", "args.txt"));
                InstallerLogic.DOOMOverrideArgs = InstallerLogic.ReadFileAndFlatten(Path.Combine("DOOMSona", "override-args.txt"));

                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Form1());
                mutex.ReleaseMutex();
            }
            else
            {
                MessageBox.Show("Another instance of the installer is already running.\n\nPlease close the other instance or terminate manually it if it's not visible.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
