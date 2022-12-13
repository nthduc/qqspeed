using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ExceptMonTest
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Button BtnOK;
        private System.Windows.Forms.Button BtnDoOne;
        private System.Windows.Forms.Button BtnFinallyCity;
        private System.Windows.Forms.Button BtnNoCatch;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null)
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
            this.BtnOK = new System.Windows.Forms.Button();
            this.BtnDoOne = new System.Windows.Forms.Button();
            this.BtnFinallyCity = new System.Windows.Forms.Button();
            this.BtnNoCatch = new System.Windows.Forms.Button();
            this.SuspendLayout();
            //
            // BtnOK
            //
            this.BtnOK.Location = new System.Drawing.Point(16, 176);
            this.BtnOK.Name = "BtnOK";
            this.BtnOK.Size = new System.Drawing.Size(264, 48);
            this.BtnOK.TabIndex = 0;
            this.BtnOK.Text = "&Ok";
            this.BtnOK.Click += new System.EventHandler(this.BtnOK_Click);
            //
            // BtnDoOne
            //
            this.BtnDoOne.Location = new System.Drawing.Point(16, 8);
            this.BtnDoOne.Name = "BtnDoOne";
            this.BtnDoOne.Size = new System.Drawing.Size(264, 48);
            this.BtnDoOne.TabIndex = 1;
            this.BtnDoOne.Text = "Normal City";
            this.BtnDoOne.Click += new System.EventHandler(this.BtnDoOne_Click);
            //
            // BtnFinallyCity
            //
            this.BtnFinallyCity.Location = new System.Drawing.Point(16, 64);
            this.BtnFinallyCity.Name = "BtnFinallyCity";
            this.BtnFinallyCity.Size = new System.Drawing.Size(264, 48);
            this.BtnFinallyCity.TabIndex = 2;
            this.BtnFinallyCity.Text = "Finally City";
            this.BtnFinallyCity.Click += new System.EventHandler(this.BtnFinallyCity_Click);
            //
            // BtnNoCatch
            //
            this.BtnNoCatch.Location = new System.Drawing.Point(16, 120);
            this.BtnNoCatch.Name = "BtnNoCatch";
            this.BtnNoCatch.Size = new System.Drawing.Size(264, 48);
            this.BtnNoCatch.TabIndex = 3;
            this.BtnNoCatch.Text = "No Catch City!";
            this.BtnNoCatch.Click += new System.EventHandler(this.BtnNoCatch_Click);
            //
            // Form1
            //
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(292, 231);
            this.Controls.Add(this.BtnNoCatch);
            this.Controls.Add(this.BtnFinallyCity);
            this.Controls.Add(this.BtnDoOne);
            this.Controls.Add(this.BtnOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Exception Monitor Tests";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.Run(new Form1());
		}
		
		private bool bDoMsgBox = false ;

        private void BtnOK_Click(object sender, System.EventArgs e)
        {
            Close ( ) ;
        }

        private void BtnDoOne_Click(object sender, System.EventArgs e)
        {
            try
            {
                Foo ( ) ;
            }
            catch ( ArgumentException exp )
            {
                exp = exp ;
            }
        }

        private void BtnFinallyCity_Click(object sender, System.EventArgs e)
        {
            try
            {
                try
                {
                    Fee ( ) ;
                }
                finally
                {
                    if ( true == bDoMsgBox )
                    {
                        MessageBox.Show ( "Whoops!" ) ;
                    }
                }
            }
            catch ( ArgumentNullException exp )
            {
                exp = exp ;
            }
        }

        private void BtnNoCatch_Click(object sender, System.EventArgs e)
        {
            Do ( ) ;
        }

        private void Form1_Load(object sender, System.EventArgs e)
        {
            CenterToScreen ( ) ;
        }

        private void Foo ( )
        {
            Bar ( ) ;
        }

        private void Bar ( )
        {
            Baz ( ) ;
        }

        private void Baz ( )
        {
            Bing ( ) ;
        }

        private void Bing ( )
        {
            Bang ( ) ;
        }

        private void Bang ( )
        {
            Boom ( ) ;
        }

        private void Boom ( )
        {
            throw new ArgumentException ( "Yikes!" ) ;
        }
        
        private void Fee ( )
        {
            try
            {
                Fi ( ) ;
            }
            finally
            {
                if ( true == bDoMsgBox )
                {
                    MessageBox.Show ( "Whoops!" ) ;
                }
            }
        }

        private void Fi ( )
        {
            try
            {
                Fo ( ) ;
            }
            finally
            {
                if ( true == bDoMsgBox )
                {
                    MessageBox.Show ( "Whoops!" ) ;
                }
            }
        }

        private void Fo ( )
        {
            try
            {
                Fum ( ) ;
            }
            finally
            {
                if ( true == bDoMsgBox )
                {
                    MessageBox.Show ( "Whoops!" ) ;
                }
            }
        }
        
        private void Fum ( )
        {
            try
            {
                throw new ArgumentNullException ( "Stop that!" ) ;
            }
            finally
            {
                if ( true == bDoMsgBox )
                {
                    MessageBox.Show ( "Whoops!" ) ;
                }
            }

        }


        private void Do ( )
        {
            Re ( ) ;
        }

        private void Re ( )
        {
            Mi ( ) ;
        }

        private void Mi ( )
        {
            Fa ( ) ;
        }

        private void Fa ( )
        {
            So ( ) ;
        }

        private void So ( )
        {
            La ( ) ;
        }

        private void La ( )
        {
            Ti ( ) ;
        }

        private void Ti ( )
        {
            Homer ( ) ;
        }

        private void Homer ( )
        {
            int i = 0 ;
            int y = 10 ;

            int z = y / i ;
        }
        
	}
}
