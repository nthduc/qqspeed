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
	/// VB.NET has in InputBox, but C# does not.
	/// </summary>
	public class InputBox : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Label lblLabel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.TextBox txtText;
		private System.ComponentModel.Container components = null;

        /// <summary>
        /// Usual ctor.
        /// </summary>
		protected InputBox ( )
		{
			InitializeComponent();
		}
		
        /// <summary>
        /// Creates an input box.
        /// </summary>
        /// <param name="TitleText">
        /// The title of the dialog.
        /// </param>
        /// <param name="LabelText">
        /// The label text for inside the box.
        /// </param>
        /// <param name="TextBoxText">
        /// The value for the edit control.
        /// </param>
		public InputBox ( String TitleText   ,
		                  String LabelText   ,
		                  String TextBoxText  )
		{
		    InitializeComponent ( ) ;
		    this.Text = TitleText ;
		    lblLabel.Text = LabelText ;
		    txtText.Text = TextBoxText ;
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
		
        /// <summary>
        /// The value of the edit control text box.
        /// </summary>
		public String InputText
		{
		    get
		    {
		        return ( txtText.Text ) ;
		    }
		}
		
		#region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
		private void InitializeComponent()
		{
            this.lblLabel = new System.Windows.Forms.Label();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.txtText = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            //
            // lblLabel
            //
            this.lblLabel.Location = new System.Drawing.Point(16, 16);
            this.lblLabel.Name = "lblLabel";
            this.lblLabel.Size = new System.Drawing.Size(288, 40);
            this.lblLabel.TabIndex = 0;
            //
            // btnOK
            //
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(312, 8);
            this.btnOK.Name = "btnOK";
            this.btnOK.TabIndex = 2;
            this.btnOK.Text = "&OK";
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            //
            // btnCancel
            //
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(312, 32);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "&Cancel";
            //
            // txtText
            //
            this.txtText.Location = new System.Drawing.Point(8, 72);
            this.txtText.Name = "txtText";
            this.txtText.Size = new System.Drawing.Size(376, 20);
            this.txtText.TabIndex = 1;
            this.txtText.Text = "";
            //
            // InputBox
            //
            this.AcceptButton = this.btnOK;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(394, 101);
            this.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                          this.txtText,
                                                                          this.btnCancel,
                                                                          this.btnOK,
                                                                          this.lblLabel});
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "InputBox";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "InputBox";
            this.Load += new System.EventHandler(this.InputBox_Load);
            this.ResumeLayout(false);

        }
		#endregion

        /// <summary>
        /// Handles the OK button click.
        /// </summary>
        private void btnOK_Click(object sender, System.EventArgs e)
        {
            this.DialogResult = DialogResult.OK ;
            Close ( ) ;
        }

        /// <summary>
        /// Forces focus to the edit box.
        /// </summary>
        private void InputBox_Load(object sender, System.EventArgs e)
        {
            txtText.Focus ( ) ;
        }

	}
}
