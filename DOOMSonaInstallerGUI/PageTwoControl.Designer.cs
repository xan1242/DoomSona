namespace DOOMSonaInstallerGUI
{
    partial class PageTwoControl
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
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.linkReference = new System.Windows.Forms.LinkLabel();
            this.btnRefresh = new System.Windows.Forms.Button();
            this.btPRefresh = new System.Windows.Forms.Button();
            this.btnPRemove = new System.Windows.Forms.Button();
            this.btnPAdd = new System.Windows.Forms.Button();
            this.lblPBoxTitle = new System.Windows.Forms.Label();
            this.lbPWads = new System.Windows.Forms.ListBox();
            this.lblBoxTitle = new System.Windows.Forms.Label();
            this.btnRemove = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.lbWads = new System.Windows.Forms.ListBox();
            this.lblOverrideArgs = new System.Windows.Forms.Label();
            this.tbOverrideArgs = new System.Windows.Forms.TextBox();
            this.lblArgs = new System.Windows.Forms.Label();
            this.tbCmdArgs = new System.Windows.Forms.TextBox();
            this.labelDescription = new System.Windows.Forms.Label();
            this.labelTitle = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
            this.toolTipArgs = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipOverrideArgs = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipWADListLabel = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipPWADListLabel = new System.Windows.Forms.ToolTip(this.components);
            this.toolTipLink1 = new System.Windows.Forms.ToolTip(this.components);
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.linkReference);
            this.panel1.Controls.Add(this.btnRefresh);
            this.panel1.Controls.Add(this.btPRefresh);
            this.panel1.Controls.Add(this.btnPRemove);
            this.panel1.Controls.Add(this.btnPAdd);
            this.panel1.Controls.Add(this.lblPBoxTitle);
            this.panel1.Controls.Add(this.lbPWads);
            this.panel1.Controls.Add(this.lblBoxTitle);
            this.panel1.Controls.Add(this.btnRemove);
            this.panel1.Controls.Add(this.btnAdd);
            this.panel1.Controls.Add(this.lbWads);
            this.panel1.Controls.Add(this.lblOverrideArgs);
            this.panel1.Controls.Add(this.tbOverrideArgs);
            this.panel1.Controls.Add(this.lblArgs);
            this.panel1.Controls.Add(this.tbCmdArgs);
            this.panel1.Controls.Add(this.labelDescription);
            this.panel1.Controls.Add(this.labelTitle);
            this.panel1.Location = new System.Drawing.Point(16, 16);
            this.panel1.Margin = new System.Windows.Forms.Padding(16);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(574, 359);
            this.panel1.TabIndex = 0;
            // 
            // linkReference
            // 
            this.linkReference.AutoSize = true;
            this.linkReference.Location = new System.Drawing.Point(6, 131);
            this.linkReference.Name = "linkReference";
            this.linkReference.Size = new System.Drawing.Size(243, 13);
            this.linkReference.TabIndex = 12;
            this.linkReference.TabStop = true;
            this.linkReference.Text = "Chocolate Doom &Wiki: Command line arguments...";
            this.linkReference.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkReference_LinkClicked);
            // 
            // btnRefresh
            // 
            this.btnRefresh.Location = new System.Drawing.Point(7, 333);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(80, 23);
            this.btnRefresh.TabIndex = 6;
            this.btnRefresh.Text = "Refre&sh";
            this.btnRefresh.UseVisualStyleBackColor = true;
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // btPRefresh
            // 
            this.btPRefresh.Location = new System.Drawing.Point(293, 333);
            this.btPRefresh.Name = "btPRefresh";
            this.btPRefresh.Size = new System.Drawing.Size(80, 23);
            this.btPRefresh.TabIndex = 10;
            this.btPRefresh.Text = "Refres&h";
            this.btPRefresh.UseVisualStyleBackColor = true;
            this.btPRefresh.Click += new System.EventHandler(this.btPRefresh_Click);
            // 
            // btnPRemove
            // 
            this.btnPRemove.Location = new System.Drawing.Point(379, 333);
            this.btnPRemove.Name = "btnPRemove";
            this.btnPRemove.Size = new System.Drawing.Size(93, 23);
            this.btnPRemove.TabIndex = 9;
            this.btnPRemove.Text = "Rem&ove PWAD";
            this.btnPRemove.UseVisualStyleBackColor = true;
            this.btnPRemove.Click += new System.EventHandler(this.btnPRemove_Click);
            // 
            // btnPAdd
            // 
            this.btnPAdd.Location = new System.Drawing.Point(478, 333);
            this.btnPAdd.Name = "btnPAdd";
            this.btnPAdd.Size = new System.Drawing.Size(93, 23);
            this.btnPAdd.TabIndex = 8;
            this.btnPAdd.Text = "A&dd PWAD...";
            this.btnPAdd.UseVisualStyleBackColor = true;
            this.btnPAdd.Click += new System.EventHandler(this.btnPAdd_Click);
            // 
            // lblPBoxTitle
            // 
            this.lblPBoxTitle.AutoSize = true;
            this.lblPBoxTitle.Location = new System.Drawing.Point(290, 157);
            this.lblPBoxTitle.Name = "lblPBoxTitle";
            this.lblPBoxTitle.Size = new System.Drawing.Size(77, 13);
            this.lblPBoxTitle.TabIndex = 110;
            this.lblPBoxTitle.Text = "PWAD List (?):";
            // 
            // lbPWads
            // 
            this.lbPWads.FormattingEnabled = true;
            this.lbPWads.Location = new System.Drawing.Point(291, 173);
            this.lbPWads.Name = "lbPWads";
            this.lbPWads.Size = new System.Drawing.Size(280, 160);
            this.lbPWads.TabIndex = 11;
            this.lbPWads.MouseClick += new System.Windows.Forms.MouseEventHandler(this.lbPWads_MouseClick);
            // 
            // lblBoxTitle
            // 
            this.lblBoxTitle.AutoSize = true;
            this.lblBoxTitle.Location = new System.Drawing.Point(6, 157);
            this.lblBoxTitle.Name = "lblBoxTitle";
            this.lblBoxTitle.Size = new System.Drawing.Size(73, 13);
            this.lblBoxTitle.TabIndex = 108;
            this.lblBoxTitle.Text = "IWAD List (?):";
            // 
            // btnRemove
            // 
            this.btnRemove.Location = new System.Drawing.Point(93, 333);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(93, 23);
            this.btnRemove.TabIndex = 5;
            this.btnRemove.Text = "&Remove IWAD";
            this.btnRemove.UseVisualStyleBackColor = true;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(192, 333);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(93, 23);
            this.btnAdd.TabIndex = 4;
            this.btnAdd.Text = "&Add IWAD...";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // lbWads
            // 
            this.lbWads.FormattingEnabled = true;
            this.lbWads.Location = new System.Drawing.Point(3, 173);
            this.lbWads.Name = "lbWads";
            this.lbWads.Size = new System.Drawing.Size(282, 160);
            this.lbWads.TabIndex = 7;
            this.lbWads.MouseClick += new System.Windows.Forms.MouseEventHandler(this.lbWads_MouseClick);
            // 
            // lblOverrideArgs
            // 
            this.lblOverrideArgs.AutoSize = true;
            this.lblOverrideArgs.Location = new System.Drawing.Point(32, 106);
            this.lblOverrideArgs.Name = "lblOverrideArgs";
            this.lblOverrideArgs.Size = new System.Drawing.Size(117, 13);
            this.lblOverrideArgs.TabIndex = 103;
            this.lblOverrideArgs.Text = "Override arguments (?):";
            // 
            // tbOverrideArgs
            // 
            this.tbOverrideArgs.Location = new System.Drawing.Point(152, 103);
            this.tbOverrideArgs.Name = "tbOverrideArgs";
            this.tbOverrideArgs.Size = new System.Drawing.Size(419, 20);
            this.tbOverrideArgs.TabIndex = 13;
            this.tbOverrideArgs.TextChanged += new System.EventHandler(this.tbOverrideArgs_TextChanged);
            // 
            // lblArgs
            // 
            this.lblArgs.AutoSize = true;
            this.lblArgs.Location = new System.Drawing.Point(6, 80);
            this.lblArgs.Name = "lblArgs";
            this.lblArgs.Size = new System.Drawing.Size(143, 13);
            this.lblArgs.TabIndex = 101;
            this.lblArgs.Text = "Command-line arguments (?):";
            // 
            // tbCmdArgs
            // 
            this.tbCmdArgs.Location = new System.Drawing.Point(152, 77);
            this.tbCmdArgs.Margin = new System.Windows.Forms.Padding(3, 16, 3, 3);
            this.tbCmdArgs.Name = "tbCmdArgs";
            this.tbCmdArgs.Size = new System.Drawing.Size(419, 20);
            this.tbCmdArgs.TabIndex = 14;
            this.tbCmdArgs.TextChanged += new System.EventHandler(this.tbCmdArgs_TextChanged);
            // 
            // labelDescription
            // 
            this.labelDescription.AutoSize = true;
            this.labelDescription.Location = new System.Drawing.Point(6, 48);
            this.labelDescription.Margin = new System.Windows.Forms.Padding(3, 16, 3, 0);
            this.labelDescription.Name = "labelDescription";
            this.labelDescription.Size = new System.Drawing.Size(410, 13);
            this.labelDescription.TabIndex = 99;
            this.labelDescription.Text = "DOOM WAD Description aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
            // 
            // labelTitle
            // 
            this.labelTitle.AutoSize = true;
            this.labelTitle.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTitle.Location = new System.Drawing.Point(3, 0);
            this.labelTitle.Name = "labelTitle";
            this.labelTitle.Size = new System.Drawing.Size(358, 32);
            this.labelTitle.TabIndex = 99;
            this.labelTitle.Text = "DOOM WAD && Parameter Setup";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "Doom IWAD|*.wad|All files|*.*";
            // 
            // openFileDialog2
            // 
            this.openFileDialog2.Filter = "Doom PWAD|*.wad|All files|*.*";
            // 
            // PageTwoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "PageTwoControl";
            this.Size = new System.Drawing.Size(606, 391);
            this.VisibleChanged += new System.EventHandler(this.PageTwoControl_VisibleChanged);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label labelTitle;
        private System.Windows.Forms.Label labelDescription;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog2;
        private System.Windows.Forms.TextBox tbCmdArgs;
        private System.Windows.Forms.Label lblArgs;
        private System.Windows.Forms.Label lblOverrideArgs;
        private System.Windows.Forms.TextBox tbOverrideArgs;
        private System.Windows.Forms.ToolTip toolTipArgs;
        private System.Windows.Forms.ToolTip toolTipOverrideArgs;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.ListBox lbWads;
        private System.Windows.Forms.Label lblBoxTitle;
        private System.Windows.Forms.Button btnRemove;
        private System.Windows.Forms.ToolTip toolTipWADListLabel;
        private System.Windows.Forms.Label lblPBoxTitle;
        private System.Windows.Forms.ListBox lbPWads;
        private System.Windows.Forms.ToolTip toolTipPWADListLabel;
        private System.Windows.Forms.Button btnPRemove;
        private System.Windows.Forms.Button btnPAdd;
        private System.Windows.Forms.Button btnRefresh;
        private System.Windows.Forms.Button btPRefresh;
        private System.Windows.Forms.LinkLabel linkReference;
        private System.Windows.Forms.ToolTip toolTipLink1;
    }
}
