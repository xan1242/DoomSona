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
    public partial class UninstallPageOneControl : UserControl
    {
        public UninstallPageOneControl()
        {
            InitializeComponent();

            labelDescription.Text = "This will attempt to clean up any files written to the Persona 5 Royal directory except for the ASI loader\n" +
                "and the Reloaded bootstrapper.\n\n" +
                "If you aren't using the ASI loader for other mods (e.g. Zolika's ZMenu) then you may also check the box\n" +
                "\"Remove ASI Loader and Bootstrapper\" to clean up the ASI loader and revert to using Reloaded as the injector.";
        }

        private void cbASICleanup_CheckedChanged(object sender, EventArgs e)
        {
            InstallerLogic.bCleanupASILoader = cbASICleanup.Checked;
        }
    }
}
