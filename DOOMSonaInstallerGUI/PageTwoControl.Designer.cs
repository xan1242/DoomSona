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
            this.panel1 = new System.Windows.Forms.Panel();
            this.labelTitle = new System.Windows.Forms.Label();
            this.labelDescription = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxDoom2WAD = new System.Windows.Forms.TextBox();
            this.btnDoom2WAD = new System.Windows.Forms.Button();
            this.btnDoom1WAD = new System.Windows.Forms.Button();
            this.textBoxDoom1WAD = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.textBoxDoom1WAD);
            this.panel1.Controls.Add(this.btnDoom1WAD);
            this.panel1.Controls.Add(this.btnDoom2WAD);
            this.panel1.Controls.Add(this.textBoxDoom2WAD);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.labelDescription);
            this.panel1.Controls.Add(this.labelTitle);
            this.panel1.Location = new System.Drawing.Point(16, 16);
            this.panel1.Margin = new System.Windows.Forms.Padding(16);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(574, 359);
            this.panel1.TabIndex = 0;
            // 
            // labelTitle
            // 
            this.labelTitle.AutoSize = true;
            this.labelTitle.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTitle.Location = new System.Drawing.Point(3, 0);
            this.labelTitle.Name = "labelTitle";
            this.labelTitle.Size = new System.Drawing.Size(218, 32);
            this.labelTitle.TabIndex = 0;
            this.labelTitle.Text = "DOOM WAD Setup";
            // 
            // labelDescription
            // 
            this.labelDescription.AutoSize = true;
            this.labelDescription.Location = new System.Drawing.Point(6, 48);
            this.labelDescription.Margin = new System.Windows.Forms.Padding(3, 16, 3, 0);
            this.labelDescription.Name = "labelDescription";
            this.labelDescription.Size = new System.Drawing.Size(125, 13);
            this.labelDescription.TabIndex = 1;
            this.labelDescription.Text = "DOOM WAD Description";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 339);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "DOOM 2 WAD:";
            // 
            // textBoxDoom2WAD
            // 
            this.textBoxDoom2WAD.Location = new System.Drawing.Point(87, 336);
            this.textBoxDoom2WAD.Name = "textBoxDoom2WAD";
            this.textBoxDoom2WAD.Size = new System.Drawing.Size(403, 20);
            this.textBoxDoom2WAD.TabIndex = 3;
            // 
            // btnDoom2WAD
            // 
            this.btnDoom2WAD.Location = new System.Drawing.Point(496, 333);
            this.btnDoom2WAD.Name = "btnDoom2WAD";
            this.btnDoom2WAD.Size = new System.Drawing.Size(75, 23);
            this.btnDoom2WAD.TabIndex = 4;
            this.btnDoom2WAD.Text = "Browse...";
            this.btnDoom2WAD.UseVisualStyleBackColor = true;
            this.btnDoom2WAD.Click += new System.EventHandler(this.btnDoom2WAD_Click);
            // 
            // btnDoom1WAD
            // 
            this.btnDoom1WAD.Location = new System.Drawing.Point(496, 304);
            this.btnDoom1WAD.Name = "btnDoom1WAD";
            this.btnDoom1WAD.Size = new System.Drawing.Size(75, 23);
            this.btnDoom1WAD.TabIndex = 5;
            this.btnDoom1WAD.Text = "Browse...";
            this.btnDoom1WAD.UseVisualStyleBackColor = true;
            this.btnDoom1WAD.Click += new System.EventHandler(this.btnDoom1WAD_Click);
            // 
            // textBoxDoom1WAD
            // 
            this.textBoxDoom1WAD.Location = new System.Drawing.Point(87, 307);
            this.textBoxDoom1WAD.Name = "textBoxDoom1WAD";
            this.textBoxDoom1WAD.Size = new System.Drawing.Size(403, 20);
            this.textBoxDoom1WAD.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 310);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(81, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "DOOM 1 WAD:";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "Doom IWAD|*.wad|All files|*.*";
            // 
            // openFileDialog2
            // 
            this.openFileDialog2.FileName = "openFileDialog2";
            this.openFileDialog2.Filter = "Doom IWAD|*.wad|All files|*.*";
            // 
            // PageTwoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "PageTwoControl";
            this.Size = new System.Drawing.Size(606, 391);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label labelTitle;
        private System.Windows.Forms.Label labelDescription;
        private System.Windows.Forms.Button btnDoom2WAD;
        private System.Windows.Forms.TextBox textBoxDoom2WAD;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxDoom1WAD;
        private System.Windows.Forms.Button btnDoom1WAD;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog2;
    }
}
