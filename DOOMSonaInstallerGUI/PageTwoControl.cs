using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DOOMSonaInstallerGUI
{
    public partial class PageTwoControl : UserControl
    {
        public PageTwoControl()
        {
            InitializeComponent();
            labelDescription.Text = "Here you can optionally configure your WAD files for the full versions DOOM 1 and DOOM 2.\nYou may obtain these from within the game files of the full version copies.\n\n" +
                "DOOMSona already comes with:\n\n" +
                "- Shareware episode of DOOM\n" +
                "- Freedoom Phase 1\n" +
                "- Freedoom Phase 2\n" +
                "- Chex® Quest\n" +
                "\n" +
                "So even if you do not have the full DOOM WADs, you'll still be able to play the pre-included WADs.";

            textBoxDoom1WAD.Text = InstallerLogic.GetDoom1WADPath();
            textBoxDoom2WAD.Text = InstallerLogic.GetDoom2WADPath();
        }

        private void btnDoom1WAD_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                textBoxDoom1WAD.Text = openFileDialog1.FileName;
                InstallerLogic.SetDoom1WADPath(textBoxDoom1WAD.Text);
            }
        }

        private void btnDoom2WAD_Click(object sender, EventArgs e)
        {
            if (openFileDialog2.ShowDialog() == DialogResult.OK)
            {
                textBoxDoom2WAD.Text = openFileDialog2.FileName;
                InstallerLogic.SetDoom2WADPath(textBoxDoom2WAD.Text);
            }
        }
    }
}
