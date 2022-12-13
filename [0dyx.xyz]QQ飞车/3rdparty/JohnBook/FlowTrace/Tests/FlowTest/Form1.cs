using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices ;
using System.Threading ;

namespace FlowTest
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class FlowTestDlg : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Button BtnThreadOne;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FlowTestDlg()
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
            this.BtnThreadOne = new System.Windows.Forms.Button();
            this.SuspendLayout();
            //
            // BtnThreadOne
            //
            this.BtnThreadOne.Location = new System.Drawing.Point(25, 8);
            this.BtnThreadOne.Name = "BtnThreadOne";
            this.BtnThreadOne.Size = new System.Drawing.Size(160, 23);
            this.BtnThreadOne.TabIndex = 0;
            this.BtnThreadOne.Text = "Crank up a thread";
            this.BtnThreadOne.Click += new System.EventHandler(this.BtnThreadOne_Click);
            //
            // FlowTestDlg
            //
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(210, 127);
            this.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                          this.BtnThreadOne});
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "FlowTestDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FlowTrace Test Program";
            this.ResumeLayout(false);

        }
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.Run(new FlowTestDlg());
		}

        private void BtnThreadOne_Click(object sender, System.EventArgs e)
        {
            ThreadOne c = new ThreadOne ( ) ;
            Thread t = new Thread ( new ThreadStart ( c.EntryPoint ) ) ;
            t.IsBackground = true ;
            t.Name = "Thread One" ;
            t.Start ( ) ;
        }
	}
	
	class ThreadOne
	{
	    public void EntryPoint ( )
	    {
	        Do ( ) ;
	    }
	    
        void Do ( )
        {
            Re ( ) ;
        }
        
        void Re ( )
        {
            Mi ( ) ;
        }
        
        void Mi ( )
        {
            Fa ( ) ;
        }
        
        void Fa ( )
        {
            So ( ) ;
        }
        
        void So ( )
        {
            La ( ) ;
        }
        
        void La ( )
        {
            Ti ( ) ;
        }
        
        void Ti ( )
        {
            Doh ( ) ;
        }
        
        void Doh ( )
        {
            MessageBeep ( -1 ) ;
        }
        
        [DllImport("user32.dll")]
        private static extern Int32 MessageBeep ( Int32 uType ) ;
	}
}
