namespace DOOMSonaInstallerGUI
{
    partial class UninstallPageOneControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.cbASICleanup = new System.Windows.Forms.CheckBox();
            this.labelDescription = new System.Windows.Forms.Label();
            this.labelTitle = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.cbASICleanup);
            this.panel1.Controls.Add(this.labelDescription);
            this.panel1.Controls.Add(this.labelTitle);
            this.panel1.Location = new System.Drawing.Point(16, 16);
            this.panel1.Margin = new System.Windows.Forms.Padding(16);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(574, 359);
            this.panel1.TabIndex = 0;
            // 
            // cbASICleanup
            // 
            this.cbASICleanup.AutoSize = true;
            this.cbASICleanup.Location = new System.Drawing.Point(4, 340);
            this.cbASICleanup.Name = "cbASICleanup";
            this.cbASICleanup.Size = new System.Drawing.Size(206, 17);
            this.cbASICleanup.TabIndex = 4;
            this.cbASICleanup.Text = "Re&move ASI Loader and Bootstrapper";
            this.cbASICleanup.UseVisualStyleBackColor = true;
            this.cbASICleanup.CheckedChanged += new System.EventHandler(this.cbASICleanup_CheckedChanged);
            // 
            // labelDescription
            // 
            this.labelDescription.AutoSize = true;
            this.labelDescription.Location = new System.Drawing.Point(6, 48);
            this.labelDescription.Margin = new System.Windows.Forms.Padding(3, 16, 3, 0);
            this.labelDescription.Name = "labelDescription";
            this.labelDescription.Size = new System.Drawing.Size(150, 13);
            this.labelDescription.TabIndex = 99;
            this.labelDescription.Text = "Uninstallation Description Text";
            // 
            // labelTitle
            // 
            this.labelTitle.AutoSize = true;
            this.labelTitle.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTitle.Location = new System.Drawing.Point(3, 0);
            this.labelTitle.Name = "labelTitle";
            this.labelTitle.Size = new System.Drawing.Size(217, 32);
            this.labelTitle.TabIndex = 99;
            this.labelTitle.Text = "Uninstall / Cleanup";
            // 
            // UninstallPageOneControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "UninstallPageOneControl";
            this.Size = new System.Drawing.Size(606, 391);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label labelTitle;
        private System.Windows.Forms.Label labelDescription;
        private System.Windows.Forms.CheckBox cbASICleanup;
    }
}
