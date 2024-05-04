namespace DOOMSonaInstallerGUI
{
    partial class PageOneControl
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
            this.btnConfig = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.labelGamePathTitle = new System.Windows.Forms.Label();
            this.labelInstalledModVersion = new System.Windows.Forms.Label();
            this.labelGamePath = new System.Windows.Forms.Label();
            this.labelModVersionTitle = new System.Windows.Forms.Label();
            this.labelModVersion = new System.Windows.Forms.Label();
            this.btnUninstall = new System.Windows.Forms.Button();
            this.labelTitle = new System.Windows.Forms.Label();
            this.labelDescription = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.btnConfig);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Controls.Add(this.btnUninstall);
            this.panel1.Controls.Add(this.labelTitle);
            this.panel1.Controls.Add(this.labelDescription);
            this.panel1.Location = new System.Drawing.Point(16, 16);
            this.panel1.Margin = new System.Windows.Forms.Padding(16);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(574, 359);
            this.panel1.TabIndex = 1;
            // 
            // btnConfig
            // 
            this.btnConfig.Location = new System.Drawing.Point(4, 304);
            this.btnConfig.Name = "btnConfig";
            this.btnConfig.Size = new System.Drawing.Size(118, 23);
            this.btnConfig.TabIndex = 4;
            this.btnConfig.Text = "C&onfigure DOOM";
            this.btnConfig.UseVisualStyleBackColor = true;
            this.btnConfig.Click += new System.EventHandler(this.btnConfig_Click);
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.labelGamePathTitle);
            this.panel2.Controls.Add(this.labelInstalledModVersion);
            this.panel2.Controls.Add(this.labelGamePath);
            this.panel2.Controls.Add(this.labelModVersionTitle);
            this.panel2.Controls.Add(this.labelModVersion);
            this.panel2.Location = new System.Drawing.Point(197, 288);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(374, 71);
            this.panel2.TabIndex = 7;
            // 
            // labelGamePathTitle
            // 
            this.labelGamePathTitle.AutoSize = true;
            this.labelGamePathTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelGamePathTitle.Location = new System.Drawing.Point(3, 42);
            this.labelGamePathTitle.Name = "labelGamePathTitle";
            this.labelGamePathTitle.Size = new System.Drawing.Size(73, 13);
            this.labelGamePathTitle.TabIndex = 99;
            this.labelGamePathTitle.Text = "Game Path:";
            // 
            // labelInstalledModVersion
            // 
            this.labelInstalledModVersion.AutoSize = true;
            this.labelInstalledModVersion.Location = new System.Drawing.Point(3, 29);
            this.labelInstalledModVersion.Name = "labelInstalledModVersion";
            this.labelInstalledModVersion.Size = new System.Drawing.Size(176, 13);
            this.labelInstalledModVersion.TabIndex = 99;
            this.labelInstalledModVersion.Text = "Installed mod version text goes here";
            // 
            // labelGamePath
            // 
            this.labelGamePath.AutoSize = true;
            this.labelGamePath.Location = new System.Drawing.Point(3, 55);
            this.labelGamePath.Name = "labelGamePath";
            this.labelGamePath.Size = new System.Drawing.Size(339, 13);
            this.labelGamePath.TabIndex = 99;
            this.labelGamePath.Text = "Game Path Unknown !!! Pass the path as an argument to this app first!";
            // 
            // labelModVersionTitle
            // 
            this.labelModVersionTitle.AutoSize = true;
            this.labelModVersionTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelModVersionTitle.Location = new System.Drawing.Point(3, 0);
            this.labelModVersionTitle.Name = "labelModVersionTitle";
            this.labelModVersionTitle.Size = new System.Drawing.Size(81, 13);
            this.labelModVersionTitle.TabIndex = 99;
            this.labelModVersionTitle.Text = "Mod Version:";
            // 
            // labelModVersion
            // 
            this.labelModVersion.AutoSize = true;
            this.labelModVersion.Location = new System.Drawing.Point(3, 16);
            this.labelModVersion.Name = "labelModVersion";
            this.labelModVersion.Size = new System.Drawing.Size(135, 13);
            this.labelModVersion.TabIndex = 99;
            this.labelModVersion.Text = "Mod version text goes here";
            // 
            // btnUninstall
            // 
            this.btnUninstall.Location = new System.Drawing.Point(4, 333);
            this.btnUninstall.Name = "btnUninstall";
            this.btnUninstall.Size = new System.Drawing.Size(118, 23);
            this.btnUninstall.TabIndex = 5;
            this.btnUninstall.Text = "&Uninstall / Cleanup";
            this.btnUninstall.UseVisualStyleBackColor = true;
            this.btnUninstall.Click += new System.EventHandler(this.btnUninstall_Click);
            // 
            // labelTitle
            // 
            this.labelTitle.AutoSize = true;
            this.labelTitle.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTitle.Location = new System.Drawing.Point(3, 0);
            this.labelTitle.Name = "labelTitle";
            this.labelTitle.Size = new System.Drawing.Size(284, 32);
            this.labelTitle.TabIndex = 99;
            this.labelTitle.Text = "Welcome to DOOMSona!";
            // 
            // labelDescription
            // 
            this.labelDescription.AutoSize = true;
            this.labelDescription.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelDescription.Location = new System.Drawing.Point(3, 48);
            this.labelDescription.Margin = new System.Windows.Forms.Padding(3, 16, 3, 0);
            this.labelDescription.Name = "labelDescription";
            this.labelDescription.Size = new System.Drawing.Size(137, 13);
            this.labelDescription.TabIndex = 99;
            this.labelDescription.Text = "Installation Description Text";
            this.labelDescription.UseMnemonic = false;
            // 
            // PageOneControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "PageOneControl";
            this.Size = new System.Drawing.Size(606, 391);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label labelDescription;
        private System.Windows.Forms.Label labelTitle;
        private System.Windows.Forms.Button btnUninstall;
        private System.Windows.Forms.Label labelModVersion;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label labelModVersionTitle;
        private System.Windows.Forms.Label labelGamePath;
        private System.Windows.Forms.Label labelInstalledModVersion;
        private System.Windows.Forms.Label labelGamePathTitle;
        private System.Windows.Forms.Button btnConfig;
    }
}
