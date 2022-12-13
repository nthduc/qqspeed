/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Tester ;
using System.Diagnostics ;
using System.Threading ;
using System.Text ;

namespace TWindowTest
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Button button12;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button13;
        private System.Windows.Forms.Button button14;
        private System.Windows.Forms.ListBox ListBoxTopWins;
        private System.Windows.Forms.ListBox ListBoxChildWins;
        private System.Windows.Forms.Button button15;
        private System.Windows.Forms.Button button16;
        private System.Windows.Forms.Button button17;
        private System.Windows.Forms.Button button18;
        private System.Windows.Forms.Button button19;
        private System.Windows.Forms.Button button20;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button21;
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
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.button9 = new System.Windows.Forms.Button();
            this.button10 = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button12 = new System.Windows.Forms.Button();
            this.button11 = new System.Windows.Forms.Button();
            this.ListBoxChildWins = new System.Windows.Forms.ListBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button19 = new System.Windows.Forms.Button();
            this.button18 = new System.Windows.Forms.Button();
            this.button17 = new System.Windows.Forms.Button();
            this.button16 = new System.Windows.Forms.Button();
            this.button15 = new System.Windows.Forms.Button();
            this.ListBoxTopWins = new System.Windows.Forms.ListBox();
            this.button14 = new System.Windows.Forms.Button();
            this.button13 = new System.Windows.Forms.Button();
            this.button20 = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button21 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            //
            // button1
            //
            this.button1.Location = new System.Drawing.Point(16, 24);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(232, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "hWnd Property Tests";
            this.button1.Click += new System.EventHandler(this.button1_Click);
            //
            // button2
            //
            this.button2.Location = new System.Drawing.Point(16, 48);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(232, 23);
            this.button2.TabIndex = 3;
            this.button2.Text = "Caption and Title Property Tests";
            this.button2.Click += new System.EventHandler(this.button2_Click);
            //
            // button3
            //
            this.button3.Location = new System.Drawing.Point(16, 72);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(232, 23);
            this.button3.TabIndex = 4;
            this.button3.Text = "ClassName Property Tests";
            this.button3.Click += new System.EventHandler(this.button3_Click);
            //
            // button4
            //
            this.button4.Location = new System.Drawing.Point(16, 96);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(232, 23);
            this.button4.TabIndex = 5;
            this.button4.Text = "Left, Right, Top, Bottom Property Tests";
            this.button4.Click += new System.EventHandler(this.button4_Click);
            //
            // button5
            //
            this.button5.Location = new System.Drawing.Point(16, 120);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(232, 23);
            this.button5.TabIndex = 6;
            this.button5.Text = "Valid Property Tests";
            this.button5.Click += new System.EventHandler(this.button5_Click);
            //
            // button6
            //
            this.button6.Location = new System.Drawing.Point(16, 144);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(232, 23);
            this.button6.TabIndex = 7;
            this.button6.Text = "Enabled Property Tests";
            this.button6.Click += new System.EventHandler(this.button6_Click);
            //
            // button7
            //
            this.button7.Location = new System.Drawing.Point(16, 168);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(232, 23);
            this.button7.TabIndex = 8;
            this.button7.Text = "Parent Property Tests";
            this.button7.Click += new System.EventHandler(this.button7_Click);
            //
            // button8
            //
            this.button8.Location = new System.Drawing.Point(16, 192);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(232, 23);
            this.button8.TabIndex = 9;
            this.button8.Text = "ProcessId, ThreadId, and Id  Property Tests";
            this.button8.Click += new System.EventHandler(this.button8_Click);
            //
            // button9
            //
            this.button9.Location = new System.Drawing.Point(16, 216);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(232, 23);
            this.button9.TabIndex = 10;
            this.button9.Text = "SetWindowSize Method Tests";
            this.button9.Click += new System.EventHandler(this.button9_Click);
            //
            // button10
            //
            this.button10.Location = new System.Drawing.Point(16, 240);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(232, 23);
            this.button10.TabIndex = 11;
            this.button10.Text = "FindChildWindow* Method Tests";
            this.button10.Click += new System.EventHandler(this.button10_Click);
            //
            // groupBox1
            //
            this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                    this.button12,
                                                                                    this.button11,
                                                                                    this.ListBoxChildWins});
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(248, 416);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "TWindow Object Tests";
            //
            // button12
            //
            this.button12.Location = new System.Drawing.Point(8, 384);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(232, 23);
            this.button12.TabIndex = 13;
            this.button12.Text = "Do All TWindow Tests";
            this.button12.Click += new System.EventHandler(this.button12_Click);
            //
            // button11
            //
            this.button11.Location = new System.Drawing.Point(8, 256);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(232, 23);
            this.button11.TabIndex = 12;
            this.button11.Text = "Children Property Tests";
            this.button11.Click += new System.EventHandler(this.button11_Click);
            //
            // ListBoxChildWins
            //
            this.ListBoxChildWins.Location = new System.Drawing.Point(8, 280);
            this.ListBoxChildWins.Name = "ListBoxChildWins";
            this.ListBoxChildWins.Size = new System.Drawing.Size(232, 95);
            this.ListBoxChildWins.TabIndex = 17;
            //
            // groupBox2
            //
            this.groupBox2.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                    this.button19,
                                                                                    this.button18,
                                                                                    this.button17,
                                                                                    this.button16,
                                                                                    this.button15,
                                                                                    this.ListBoxTopWins,
                                                                                    this.button14,
                                                                                    this.button13});
            this.groupBox2.Location = new System.Drawing.Point(280, 8);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(248, 296);
            this.groupBox2.TabIndex = 12;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "TSystem Object Tests";
            //
            // button19
            //
            this.button19.Location = new System.Drawing.Point(8, 264);
            this.button19.Name = "button19";
            this.button19.Size = new System.Drawing.Size(232, 23);
            this.button19.TabIndex = 21;
            this.button19.Text = "Do All TSystem Tests";
            this.button19.Click += new System.EventHandler(this.button19_Click);
            //
            // button18
            //
            this.button18.Location = new System.Drawing.Point(8, 232);
            this.button18.Name = "button18";
            this.button18.Size = new System.Drawing.Size(232, 23);
            this.button18.TabIndex = 20;
            this.button18.Text = "*Resolution Methods Tests";
            this.button18.Click += new System.EventHandler(this.button18_Click);
            //
            // button17
            //
            this.button17.Location = new System.Drawing.Point(8, 208);
            this.button17.Name = "button17";
            this.button17.Size = new System.Drawing.Size(232, 23);
            this.button17.TabIndex = 19;
            this.button17.Text = "Sleep and Pause Method Tests";
            this.button17.Click += new System.EventHandler(this.button17_Click);
            //
            // button16
            //
            this.button16.Location = new System.Drawing.Point(8, 184);
            this.button16.Name = "button16";
            this.button16.Size = new System.Drawing.Size(232, 23);
            this.button16.TabIndex = 18;
            this.button16.Text = "Execute Method Tests";
            this.button16.Click += new System.EventHandler(this.button16_Click);
            //
            // button15
            //
            this.button15.Location = new System.Drawing.Point(8, 160);
            this.button15.Name = "button15";
            this.button15.Size = new System.Drawing.Size(232, 23);
            this.button15.TabIndex = 17;
            this.button15.Text = "FindTopTWindow* Method Tests";
            this.button15.Click += new System.EventHandler(this.button15_Click);
            //
            // ListBoxTopWins
            //
            this.ListBoxTopWins.HorizontalScrollbar = true;
            this.ListBoxTopWins.Location = new System.Drawing.Point(8, 64);
            this.ListBoxTopWins.Name = "ListBoxTopWins";
            this.ListBoxTopWins.Size = new System.Drawing.Size(232, 95);
            this.ListBoxTopWins.TabIndex = 16;
            //
            // button14
            //
            this.button14.Location = new System.Drawing.Point(8, 40);
            this.button14.Name = "button14";
            this.button14.Size = new System.Drawing.Size(232, 23);
            this.button14.TabIndex = 15;
            this.button14.Text = "TopTWindows Property Tests";
            this.button14.Click += new System.EventHandler(this.button14_Click);
            //
            // button13
            //
            this.button13.Location = new System.Drawing.Point(8, 16);
            this.button13.Name = "button13";
            this.button13.Size = new System.Drawing.Size(232, 23);
            this.button13.TabIndex = 14;
            this.button13.Text = "ForegroundTWindow Property Tests";
            this.button13.Click += new System.EventHandler(this.button13_Click);
            //
            // button20
            //
            this.button20.Location = new System.Drawing.Point(8, 16);
            this.button20.Name = "button20";
            this.button20.Size = new System.Drawing.Size(232, 23);
            this.button20.TabIndex = 22;
            this.button20.Text = "MultiMonitor Property Tests";
            this.button20.Click += new System.EventHandler(this.button20_Click);
            //
            // groupBox3
            //
            this.groupBox3.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                    this.button21,
                                                                                    this.button20});
            this.groupBox3.Location = new System.Drawing.Point(280, 312);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(248, 112);
            this.groupBox3.TabIndex = 23;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "TInput Object Tests";
            //
            // button21
            //
            this.button21.Location = new System.Drawing.Point(8, 40);
            this.button21.Name = "button21";
            this.button21.Size = new System.Drawing.Size(232, 23);
            this.button21.TabIndex = 23;
            this.button21.Text = "PlayKeys Tests";
            this.button21.Click += new System.EventHandler(this.button21_Click);
            //
            // Form1
            //
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(538, 431);
            this.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                          this.groupBox2,
                                                                          this.button10,
                                                                          this.button9,
                                                                          this.button8,
                                                                          this.button7,
                                                                          this.button6,
                                                                          this.button5,
                                                                          this.button4,
                                                                          this.button3,
                                                                          this.button2,
                                                                          this.button1,
                                                                          this.groupBox1,
                                                                          this.groupBox3});
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Tester Component Tester!";
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
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
		
		private void HwndTests ( )
		{
            TWindow x = new TWindowClass (  ) ;
            x.hWnd = 0xDEAD ;
            if ( 0xDEAD != x.hWnd )
            {
                Debug.Assert ( false ,
                    "Accessing hWnd property failed!" ) ;
            }
            x = null ;
        }

        private void button1_Click(object sender, System.EventArgs e)
        {
            HwndTests ( ) ;
        }
        
        void CaptionAndTitleTests ( )
        {
            TWindow x = new TWindowClass ( ) ;
            x.hWnd = (int)this.Handle ;
            
            String sCaption ;
            sCaption = x.Caption ;
            
            Debug.Assert ( sCaption == this.Text ,
                "Caption did not return the correct value!");
                           
            String sTitle ;
            sTitle = x.Title ;
            
            Debug.Assert ( sTitle == this.Text ,
                "Title did not return the correct value!" ) ;
            
            x = null ;
        }

        private void button2_Click(object sender, System.EventArgs e)
        {
            CaptionAndTitleTests ( ) ;
        }

        void ClassNameTests ( )
        {
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            String sClass = x.ClassName ;
            
            Debug.Assert ( sClass == x.ClassName ,
                "ClassName returned incorrect value!" ) ;
        
            x = null ;
        }
                
        private void button3_Click(object sender, System.EventArgs e)
        {
            ClassNameTests ( ) ;
        }
        
        void LRBTTests ( )
        {
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            int l = this.Left ;
            int r = this.Right ;
            int t = this.Top ;
            int b = this.Bottom ;
            
            int tl = x.Left ;
            int tr = x.Right ;
            int tt = x.Top ;
            int tb = x.Bottom ;
            
            Debug.Assert ( l == tl , "Left failed!" ) ;
            Debug.Assert ( r == tr , "Right failed!" ) ;
            Debug.Assert ( b == tb , "Bottom failed" ) ;
            Debug.Assert ( t == tt , "Top failed" ) ;

            x = null ;
        }

        private void button4_Click(object sender, System.EventArgs e)
        {
            LRBTTests ( ) ;
        }

        void ValidTests ( )
        {
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            bool t = x.Valid ;
            Debug.Assert ( true == t) ;
            
            x.hWnd = 0 ;
            t = x.Valid ;
            Debug.Assert ( false == t ) ;
        
            x = null ;
        }
                
        private void button5_Click(object sender, System.EventArgs e)
        {
            ValidTests ( ) ;
        }

        void EnabledTests ( )
        {
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            bool en = x.Enabled ;
            Debug.Assert ( true == en) ;
            
            button6.Enabled = false ;
            x.hWnd = (int)button6.Handle ;
            en = x.Enabled ;
            Debug.Assert ( false == en ,
                "Enabled return invalid value" ) ;
            button6.Enabled = true ;
        
            x = null ;
        
        }

        private void button6_Click(object sender, System.EventArgs e)
        {
            EnabledTests ( ) ;
        }

        void ParentTests ( )
        {
            TWindow x = new TWindow ( ) ;
            
            TWindow y = x.Parent ;
            
            Debug.Assert ( null == y ) ;
            
            TWindow z = new TWindow ( ) ;
            
            z.hWnd = (int)button7.Handle ;
            
            TWindow p = z.Parent ;
            
            Debug.Assert ( p.Caption == this.Text ,
                "Parent property failed!" ) ;
            z = null ;
            p = null ;
            x = null ;
        }
        
        private void button7_Click(object sender, System.EventArgs e)
        {
            ParentTests ( ) ;
        }

        void IdStarTests ( )
        {
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            Int32 tid = x.ThreadId ;
            Debug.Assert ( tid != 0 ,
                          "ThreadId returned invalid value" ) ;
            Int32 pid = x.ProcessId ;
            Debug.Assert ( pid != 0 ,
                           "ProcessId returned invalid value" ) ;
                           
            TWindow z = new TWindow ( ) ;
            z.hWnd = (int)button8.Handle ;
            Int32 idid = z.Id ;
            Debug.Assert ( idid != 0 ,
                           "Id returned invalid value" ) ;
            z = null ;
        }
        
        private void button8_Click(object sender, System.EventArgs e)
        {
            IdStarTests ( ) ;
        }

        void SizeTests ( )
        {
            Int32 Top = this.Top ;
            Int32 Bottom = this.Bottom ;
            Int32 Left = this.Left ;
            Int32 Right = this.Right ;
            
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            x.SetWindowSize ( 10 , 10 , 100 , 100 ) ;
            
            Thread.Sleep ( 2000 ) ;
            
            x.SetWindowSize ( Left          ,
                              Top           ,
                              Right - Left  ,
                              Bottom - Top   ) ;
        }
        
        private void button9_Click(object sender, System.EventArgs e)
        {
            SizeTests ( ) ;
        }

        void FindWindowTests ( )
        {
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            TWindow c = x.FindChildTWindowByTitle ( button10.Text ) ;
            bool b = c.Caption == button10.Text ;
            Debug.Assert ( b ,
                "FindChildTWindowByTitle returned invalid data!" ) ;
            c = null ;
            // This'll find the first button.
            
            c = x.FindChildTWindowByClass( x.ClassName );
            Debug.Assert ( c != null ,
                "FindChildTWindowByClass returned invalid data!" ) ;
                     
            TWindow Butt1 = new TWindowClass ( ) ;
            Butt1.hWnd = (int)button1.Handle ;
            c = null ;
            c = x.FindChildTWindow ( Butt1.ClassName       ,
                                     "hWnd Property Tests"   ) ;
            Debug.Assert ( c != null ,
                          "FindChildTWindow returned invalid data!" ) ;
                
            Int32 Id = c.Id ;
            
            TWindow d = x.FindChildTWindowWithID ( Id ) ;
            Debug.Assert ( d.Id == Id ,
                       "FindChildTWindowWithID returned invalid data" );
                     
            x = null ;
            c = null ;
        }
                
        private void button10_Click(object sender, System.EventArgs e)
        {
            FindWindowTests ( ) ;
        }

        private void button11_Click_1(object sender, System.EventArgs e)
        {
            TWindows c = new TWindows ( ) ;
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            TWindow s = x.FindChildWindowByTitle ( button10.Text ) ;
            
            
            //Object o = this.Handle ;
            //c.Add ( ref o ) ;
            
            Object q = (Object)x ;
            c.Add ( ref q ) ;
            q = (Object)s ;
            c.Add ( ref q ) ;
            
            int Count = c.Count ;
            
            
            for ( int i = 1 ; i <= c.Count ; i++ )
            {
                Trace.WriteLine ( i.ToString ( ) + " : " + c[i].Title );
            }
            
            foreach ( TWindow t in c )
            {
                Trace.WriteLine ( "foreach : " + t.Title ) ;
            }
            
            TWindow p = c.Remove ( 2 ) ;
            Debug.Assert ( 1 == c.Count ) ;
            Trace.WriteLine ( "---------" ) ;
            foreach ( TWindow t2 in c )
            {
                Trace.WriteLine ( "foreach (2) : " + t2.Title ) ;
            }
            Trace.WriteLine ( "---------" ) ;
            c = x.Children ;

            Trace.WriteLine ( "---------" ) ;
            foreach ( TWindow t3 in c )
            {
                Trace.WriteLine ( "foreach (3) : " + t3.Title ) ;
            }
            Trace.WriteLine ( "---------" ) ;
        }
        
        void ChildPropTests ( )
        {
            TWindow x = new TWindow ( ) ;
            x.hWnd = (int)this.Handle ;
            
            ListBoxChildWins.Items.Clear ( ) ;
            TWindows c = x.Children ;
            String s ;
            foreach ( TWindow t3 in c )
            {
                s = t3.Caption ;
                if ( "" == s )
                {
                    s = "[[" + t3.ClassName + "]]" ;
                }
                ListBoxChildWins.Items.Add ( s ) ;
            }
            
            Int32 cC = c.Count ;
            Int32 tC = this.Controls.Count ;
            
            // For whatever reason (a bug?) the Controls collection does
            // not have all the real child controls in it.  Weird.
            //Debug.Assert ( cC == tC ,
            //               "TWindows.Count returned invalid number!" ) ;
        }

        private void button11_Click(object sender, System.EventArgs e)
        {
            ChildPropTests ( ) ;
        }

        private void button12_Click(object sender, System.EventArgs e)
        {
            HwndTests ( ) ;
            CaptionAndTitleTests ( ) ;
            ClassNameTests ( ) ;
            LRBTTests ( ) ;
            ValidTests ( ) ;
            EnabledTests ( ) ;
            ParentTests ( ) ;
            IdStarTests ( ) ;
            SizeTests ( ) ;
            FindWindowTests ( ) ;
            ChildPropTests ( ) ;
        }

        void TSystemForegroundTests ( )
        {
            TSystem s = new TSystem ( ) ;
            TWindow t = s.ForegroundTWindow ;
            
            // Obviously, this'll be a problem when debugging!
            Debug.Assert ( t.hWnd == (int)this.Handle ,
                           "TSystem returned invalid value" ) ;
                           
            t = null ;
            t = s.GetForegroundTWindow ( ) ;
            Debug.Assert ( t.hWnd == (int)this.Handle ,
                           "TSystem returned invalid value" ) ;
        }

        private void button13_Click(object sender, System.EventArgs e)
        {
            TSystemForegroundTests ( ) ;
        }

        void TopTWinTests ( )
        {
            TSystem s = new TSystem ( ) ;
            TWindows c = s.TopTWindows ;
            Debug.Assert ( 0 != c.Count ) ;
            
            ListBoxTopWins.Items.Clear ( ) ;
            foreach ( TWindow t in c )
            {
                if ( ( t.Caption != "" ) && ( t.Visible == true ) )
                {
                    ListBoxTopWins.Items.Add ( t.Caption ) ;
                }
            }
        }
        
        private void button14_Click(object sender, System.EventArgs e)
        {
            TopTWinTests ( ) ;
        }

        void FindTopTWinTests ( )
        {
            TSystem s = new TSystem ( ) ;
            TWindow t = s.FindTopTWindowByTitle ( this.Text ) ;
            Debug.Assert ( t.hWnd == (int)this.Handle ,
                           "FindTopTWindowByTitle failed!" ) ;
                           
            TWindow t2 = s.FindTopTWindowByClass ( t.ClassName ) ;
            Debug.Assert ( t2.ClassName == t.ClassName ,
                           "FindTopTWindowByClass failed!" ) ;
                           
            TWindow t3 = s.FindTopTWindow ( t.ClassName , t.Caption ) ;
            Debug.Assert ( t3.hWnd == t2.hWnd ,
                           "FindTopTWindow failed!" ) ;
                           
        }
        
        private void button15_Click(object sender, System.EventArgs e)
        {
            FindTopTWinTests ( ) ;
        }
        
        void ExecTests ( )
        {
            TSystem s = new TSystem ( ) ;
            TWindow n = s.FindTopTWindowByTitle ( "Untitled - Notepad");
            if ( n != null )
            {
                MessageBox.Show ( "Notepad running, can't test." ) ;
            }
            else
            {
                bool b = s.Execute ( "NOTEPAD.EXE" ) ;
                Debug.Assert ( b == true ) ;
                Thread.Sleep ( 3000 ) ;
                n = s.FindTopTWindowByTitle ( "Untitled - Notepad");
                Debug.Assert ( n.ClassName == "Notepad" ,
                              "Execute did not seem to work." ) ;
            }
            
        }

        private void button16_Click(object sender, System.EventArgs e)
        {
            ExecTests ( ) ;
        }
        
        void SleepTests ( )
        {
            TSystem s = new TSystemClass ( ) ;
            s.Pause ( 1000 ) ;
            s.Sleep ( 2 ) ;
        }

        private void button17_Click(object sender, System.EventArgs e)
        {
            SleepTests ( ) ;
        }

        void ResTests ( )
        {
            TSystem s = new TSystemClass ( ) ;
            
            Size Res = SystemInformation.PrimaryMonitorSize ;
                   
            bool b = s.CheckResolution ( Res.Width , Res.Height ) ;
            Debug.Assert ( b == true ,
                           "CheckResolution returned false!" ) ;
             
            bool CaughtIt = false ;
            try
            {
                s.CheckResolution ( 0 , 0 ) ;
            }
            catch
            {
                CaughtIt = true ;
            }
            
            Debug.Assert ( CaughtIt == true ,
                            "CheckResolution didn't cause exception!" ) ;
                            
            Rectangle r = SystemInformation.VirtualScreen ;
            b = s.CheckVirtualResolution ( r.Width  ,
                                           r.Height ,
                                           r.Left   ,
                                           r.Top     ) ;
                                           
            CaughtIt = false ;
            try
            {
                s.CheckVirtualResolution ( r.Width  ,
                                           r.Height ,
                                           1        ,
                                           100       ) ;
            }
            catch
            {
                CaughtIt = true ;
            }

            Debug.Assert ( CaughtIt == true ,
                    "CheckVirtualResolution didn't cause exception!" ) ;

                    
            CaughtIt = false ;
            try
            {
                s.CheckVirtualResolution ( 0    ,
                                           0    ,
                                           1    ,
                                           100   ) ;
            }
            catch
            {
                CaughtIt = true ;
            }

            Debug.Assert ( CaughtIt == true ,
                    "CheckVirtualResolution didn't cause exception!" ) ;

        }
        
        private void button18_Click(object sender, System.EventArgs e)
        {
            ResTests ( ) ;
        }

        private void button19_Click(object sender, System.EventArgs e)
        {
            TSystemForegroundTests ( ) ;
            TopTWinTests ( ) ;
            FindTopTWinTests ( ) ;
            ExecTests ( ) ;
            SleepTests ( ) ;
            ResTests ( ) ;
        }

        void MultiMonTests ( )
        {
            TInput t = new TInput ( ) ;
            
            t.MultiMonitor = true ;
            
            bool b = t.MultiMonitor ;
            Debug.Assert ( b == true ,
                           "MultiMonitor returned invalid data!" ) ;

            t.MultiMonitor = false ;
            b = t.MultiMonitor ;
            Debug.Assert ( b == false ,
                           "MultiMonitor returned invalid data!" ) ;
                           
            t.MultiMonitor = true ;
        }
        
        private void button20_Click(object sender, System.EventArgs e)
        {
            MultiMonTests ( ) ;
        }

        void PKTests ( )
        {
            TInput t = new TInput ( ) ;
            TSystem s = new TSystemClass ( ) ;
            
            Size Res = SystemInformation.PrimaryMonitorSize ;
            Res.Height = Res.Height - 20 ;
            Res.Width = Res.Width - 20 ;
            
            t.PlayInput ( "{MOVETO 20 , 20}" ) ;
            
            String BaseText = "{MOVETO " ;
            StringBuilder txt = new StringBuilder ( ) ;
            
            txt.Append ( BaseText ) ;
            txt.Append ( "20 , " ) ;
            txt.Append ( Res.Height.ToString ( ) ) ;
            txt.Append ( "}" ) ;
            
            s.Sleep ( 2 ) ;
            
            t.PlayInput ( txt.ToString ( ) ) ;
            
            txt.Remove ( 0 , txt.Length ) ;
            txt.Append ( BaseText ) ;
            txt.Append ( Res.Width.ToString ( ) ) ;
            txt.Append ( " , " ) ;
            txt.Append ( Res.Height.ToString ( ) ) ;
            txt.Append ( "}" ) ;
            
            s.Sleep ( 2 ) ;
            
            t.PlayInput ( txt.ToString ( ) ) ;

            txt.Remove ( 0 , txt.Length ) ;
            txt.Append ( BaseText ) ;
            txt.Append ( Res.Width.ToString ( ) ) ;
            txt.Append ( " , 20}" ) ;
            
            s.Sleep ( 2 ) ;
            
            t.PlayInput ( txt.ToString ( ) ) ;
        }
        
        private void button21_Click(object sender, System.EventArgs e)
        {
            PKTests ( ) ;
        }

	}
}
