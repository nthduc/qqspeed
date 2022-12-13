using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Runtime.InteropServices;

namespace SimpleToolWindow
{
	/// <summary>
	/// Summary description for ScratchPadControl.
	/// </summary>
	public class ScratchPadControl : System.Windows.Forms.UserControl
	{
        private System.Windows.Forms.TextBox ScratchTextBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ScratchPadControl()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitForm call

		}

		/// <summary>
		/// Clean up any resources being used.
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

		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.ScratchTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // ScratchTextBox
            // 
            this.ScratchTextBox.AcceptsReturn = true;
            this.ScratchTextBox.AcceptsTab = true;
            this.ScratchTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
                | System.Windows.Forms.AnchorStyles.Left) 
                | System.Windows.Forms.AnchorStyles.Right)));
            this.ScratchTextBox.Location = new System.Drawing.Point(0, 0);
            this.ScratchTextBox.Multiline = true;
            this.ScratchTextBox.Name = "ScratchTextBox";
            this.ScratchTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.ScratchTextBox.Size = new System.Drawing.Size(272, 200);
            this.ScratchTextBox.TabIndex = 1;
            this.ScratchTextBox.Text = "Hello.  Type anything you want in here!";
            // 
            // ScratchPadControl
            // 
            this.Controls.Add(this.ScratchTextBox);
            this.Name = "ScratchPadControl";
            this.Size = new System.Drawing.Size(272, 200);
            this.ResumeLayout(false);

        }
		#endregion
	}
}
