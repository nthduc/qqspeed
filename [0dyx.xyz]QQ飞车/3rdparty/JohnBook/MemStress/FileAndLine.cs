/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace MemStress
{
    /// <summary>
    /// The dialog to get a file and line to fail on.
    /// </summary>
	public class FileAndLineDlg : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtFileBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox chkFailAll;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.NumericUpDown upLineNum;

		private System.ComponentModel.Container components = null;

        /// <summary>
        /// Usual ctor.
        /// </summary>
		public FileAndLineDlg()
		{
			InitializeComponent();
		}
		
		private String m_FileName = "" ;
		
        /// <summary>
        /// The filename specified.
        /// </summary>
		public String FileName
		{
		    get
		    {
		        return ( m_FileName ) ;
		    }
		    set
		    {
		        m_FileName = value ;
		    }
		}
		
        /// <summary>
        /// The line specified.
        /// </summary>
		private int m_Line = 0 ;

        /// <summary>
        /// The line specified.
        /// </summary>
		public int Line
		{
		    get
		    {
		        return ( m_Line ) ;
		    }
		    set
		    {
		        m_Line = value ;
		    }
		}

        /// <summary>
        /// The dispose pattern.
        /// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
		private void InitializeComponent()
		{
            this.label1 = new System.Windows.Forms.Label();
            this.txtFileBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.upLineNum = new System.Windows.Forms.NumericUpDown();
            this.chkFailAll = new System.Windows.Forms.CheckBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.upLineNum)).BeginInit();
            this.SuspendLayout();
            //
            // label1
            //
            this.label1.Location = new System.Drawing.Point(8, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(208, 23);
            this.label1.TabIndex = 0;
            this.label1.Text = "Enter the file for failing allocations:";
            //
            // txtFileBox
            //
            this.txtFileBox.Location = new System.Drawing.Point(8, 32);
            this.txtFileBox.Name = "txtFileBox";
            this.txtFileBox.Size = new System.Drawing.Size(208, 20);
            this.txtFileBox.TabIndex = 1;
            this.txtFileBox.Text = "";
            //
            // label2
            //
            this.label2.Location = new System.Drawing.Point(8, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(128, 24);
            this.label2.TabIndex = 2;
            this.label2.Text = "Enter the line number for failing allocations:";
            //
            // upLineNum
            //
            this.upLineNum.Location = new System.Drawing.Point(152, 64);
            this.upLineNum.Maximum = new System.Decimal(new int[] {
                                                                      1000000,
                                                                      0,
                                                                      0,
                                                                      0});
            this.upLineNum.Minimum = new System.Decimal(new int[] {
                                                                      1,
                                                                      0,
                                                                      0,
                                                                      0});
            this.upLineNum.Name = "upLineNum";
            this.upLineNum.Size = new System.Drawing.Size(64, 20);
            this.upLineNum.TabIndex = 3;
            this.upLineNum.Value = new System.Decimal(new int[] {
                                                                    1,
                                                                    0,
                                                                    0,
                                                                    0});
            //
            // chkFailAll
            //
            this.chkFailAll.Location = new System.Drawing.Point(8, 96);
            this.chkFailAll.Name = "chkFailAll";
            this.chkFailAll.Size = new System.Drawing.Size(208, 24);
            this.chkFailAll.TabIndex = 4;
            this.chkFailAll.Text = "Fail all allocations in this file";
            this.chkFailAll.CheckedChanged += new System.EventHandler(this.chkFailAll_CheckedChanged);
            //
            // btnOK
            //
            this.btnOK.Location = new System.Drawing.Point(28, 128);
            this.btnOK.Name = "btnOK";
            this.btnOK.TabIndex = 5;
            this.btnOK.Text = "&Ok";
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            //
            // btnCancel
            //
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(124, 128);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.TabIndex = 6;
            this.btnCancel.Text = "&Cancel";
            //
            // FileAndLineDlg
            //
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(226, 159);
            this.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                          this.btnCancel,
                                                                          this.btnOK,
                                                                          this.chkFailAll,
                                                                          this.upLineNum,
                                                                          this.label2,
                                                                          this.txtFileBox,
                                                                          this.label1});
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FileAndLineDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "FileAndLine";
            this.Load += new System.EventHandler(this.FileAndLineDlg_Load);
            ((System.ComponentModel.ISupportInitialize)(this.upLineNum)).EndInit();
            this.ResumeLayout(false);

        }
		#endregion

        /// <summary>
        /// Handles enabling and disabling the updown.
        /// </summary>
        private void chkFailAll_CheckedChanged(object sender, System.EventArgs e)
        {
            if ( true == chkFailAll.Checked )
            {
                upLineNum.Enabled = false ;
            }
            else
            {
                upLineNum.Enabled = true ;
            }
        }

        /// <summary>
        /// Handles the OK button.
        /// </summary>
        private void btnOK_Click(object sender, System.EventArgs e)
        {
            if ( ( 0 == txtFileBox.Text.Length ) ||
                 ( 0 == upLineNum.Value        )   )
            {
                MessageBox.Show ( this ,  "Invalid data" , "MemStress") ;
            }
            else
            {
                m_FileName = txtFileBox.Text ;
                if ( true == chkFailAll.Checked )
                {
                    m_Line = -1 ;
                }
                else
                {
                    m_Line = (int)upLineNum.Value ;
                }
                this.DialogResult = DialogResult.OK ;
                Close ( ) ;
            }
        }

        /// <summary>
        /// Initializes the updown state.
        /// </summary>
        private void FileAndLineDlg_Load(object sender, System.EventArgs e)
        {
            if ( -1 == m_Line )
            {
                upLineNum.Enabled = false ;
            }
        }

	}
}
