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

namespace DOOMSonaInstallerGUI
{
    public partial class PageTwoControl : UserControl
    {
        const string pathIWADs = "DOOMSona";
        const string pathPWADs = "DOOMSona\\WADs";

        public PageTwoControl()
        {
            InitializeComponent();
            labelDescription.Text = "Here you can manage your WAD files for Chocolate DOOM and configure your command-line parameters.\n";

            toolTipArgs.SetToolTip(lblArgs, "Set the DOOM launch parameters here. These are added after whatever is set before by the GFD flow script.");
            toolTipOverrideArgs.SetToolTip(lblOverrideArgs, "Set the override DOOM launch parameters here. If set, this overrides anything that is set before by the GFD flow script and BEFORE the box above.");
            toolTipWADListLabel.SetToolTip(lblBoxTitle, "This is the list of IWADs that are currently installed in the DOOMSona directory.");
            toolTipPWADListLabel.SetToolTip(lblPBoxTitle, "This is the list of PWADs that are currently installed in the DOOMSona\\WADs directory.\nThese should be active in any IWAD you use.");
            toolTipLink1.SetToolTip(linkReference, "Opens the Chocolate Doom Wiki article for Command line arguments.\n\nhttps://www.chocolate-doom.org/wiki/index.php/Command_line_arguments");
        }

        private DialogResult ShowRemovalDialog()
        {
            return MessageBox.Show("Are you sure you want to remove the selected item?", "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
        }

        private DialogResult ShowExistsDialog()
        {
            return MessageBox.Show("There already exists a WAD with the same name.\n\nRemove the conflicting WAD first and then try again.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void GetWadFilePathsAndUpdateListBox(ListBox box, string directoryPath)
        {
            try
            {
                if (!Directory.Exists(directoryPath))
                    return;

                // Clear existing items in the ListBox
                box.Items.Clear();

                // Get all files with .WAD extension in the specified directory
                string[] files = Directory.GetFiles(directoryPath, "*.WAD");

                // Add full paths of the files to the ListBox
                foreach (string file in files)
                {
                    box.Items.Add(Path.GetFileName(file));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred during file listing: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnRemove_Click(object sender, EventArgs e)
        {
            try
            {
                if (lbWads.SelectedItems.Count <= 0) return;
                if (string.IsNullOrWhiteSpace(lbWads.Text)) return;
                if (ShowRemovalDialog() != DialogResult.Yes) return;
                

                string wadName = lbWads.Text;
                string wadPath = Path.Combine(pathIWADs, wadName);

                if (File.Exists(wadPath))
                    File.Delete(wadPath);

                GetWadFilePathsAndUpdateListBox(lbWads, pathIWADs);
            }
            catch(Exception ex)
            {
                MessageBox.Show($"An error occurred during file removal: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnPRemove_Click(object sender, EventArgs e)
        {
            try
            {
                if (lbPWads.SelectedItems.Count <= 0) return;
                if (string.IsNullOrWhiteSpace(lbPWads.Text)) return;
                if (ShowRemovalDialog() != DialogResult.Yes) return;
                

                string wadName = lbPWads.Text;
                string wadPath = Path.Combine(pathPWADs, wadName);

                if (File.Exists(wadPath))
                    File.Delete(wadPath);

                GetWadFilePathsAndUpdateListBox(lbPWads, pathPWADs);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred during file removal: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void PageTwoControl_VisibleChanged(object sender, EventArgs e)
        {
            if (((Control)sender).Visible)
            {
                tbCmdArgs.Text = InstallerLogic.DOOMArgs;
                tbOverrideArgs.Text = InstallerLogic.DOOMOverrideArgs;

                GetWadFilePathsAndUpdateListBox(lbWads, pathIWADs);
                GetWadFilePathsAndUpdateListBox(lbPWads, pathPWADs);
            }
        }

        private void btPRefresh_Click(object sender, EventArgs e)
        {
            GetWadFilePathsAndUpdateListBox(lbPWads, pathPWADs);
        }

        private void btnRefresh_Click(object sender, EventArgs e)
        {
            GetWadFilePathsAndUpdateListBox(lbWads, pathIWADs);
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            try
            {
                string path = openFileDialog1.FileName;
                string outPath = Path.Combine(pathIWADs, Path.GetFileName(path));

                if (File.Exists(outPath))
                {
                    ShowExistsDialog();
                    return;
                }

                File.Copy(path, outPath);

                GetWadFilePathsAndUpdateListBox(lbWads, pathIWADs);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred during IWAD adding: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnPAdd_Click(object sender, EventArgs e)
        {
            if (openFileDialog2.ShowDialog() != DialogResult.OK)
                return;

            try
            {
                string path = openFileDialog2.FileName;
                string outPath = Path.Combine(pathPWADs, Path.GetFileName(path));

                if (File.Exists(outPath))
                {
                    ShowExistsDialog();
                    return;
                }

                if (!Directory.Exists(pathPWADs))
                    Directory.CreateDirectory(pathPWADs);

                File.Copy(path, outPath);

                GetWadFilePathsAndUpdateListBox(lbPWads, pathPWADs);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred during PWAD adding: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void tbOverrideArgs_TextChanged(object sender, EventArgs e)
        {
            InstallerLogic.DOOMOverrideArgs = tbOverrideArgs.Text;
        }

        private void tbCmdArgs_TextChanged(object sender, EventArgs e)
        {
            InstallerLogic.DOOMArgs = tbCmdArgs.Text;
        }

        private void linkReference_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("https://www.chocolate-doom.org/wiki/index.php/Command_line_arguments");
        }

        private void lbWads_MouseClick(object sender, MouseEventArgs e)
        {
            if (lbWads.Items.Count > 0)
            {
                int index = lbWads.IndexFromPoint(e.Location);
                if (index == ListBox.NoMatches)
                {
                    lbWads.ClearSelected();
                }
            }
        }

        private void lbPWads_MouseClick(object sender, MouseEventArgs e)
        {
            if (lbPWads.Items.Count > 0)
            {
                int index = lbPWads.IndexFromPoint(e.Location);
                if (index == ListBox.NoMatches)
                {
                    lbPWads.ClearSelected();
                }
            }
        }
    }
}
