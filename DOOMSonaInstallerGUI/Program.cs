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
            return MessageBox.Show("No arguments were passed to the installer nor was a game path found in Reloaded-II mod loader.\n\nThis is usually because you launched it manually or something went wrong.\n\nYou can relaunch the installer by passing the path to the game directory as the first argument and trying again.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private static DialogResult ShowNoModVerError()
        {
            return MessageBox.Show("Can't read the mod version from ModConfig.json.\n\nCheck if this installer is in the Reloaded-II mod directory and try again.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private static DialogResult ShowMissingFilesError()
        {
            return MessageBox.Show("DOOMSona.zip file does not exist in this installer's working directory.\n\nPlease check its existence or if it has been removed and try again after making sure that all files are in place.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                string GamePath = InstallerLogic.GetGamePath();
                if (GamePath == null)
                {
                    ShowNoArgError();
                    return;
                }

                if (InstallerLogic.GetModVersion() == null)
                {
                    ShowNoModVerError();
                    return;
                }

                if (!File.Exists("DOOMSona.zip"))
                {
                    ShowMissingFilesError();
                    return;
                }

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
