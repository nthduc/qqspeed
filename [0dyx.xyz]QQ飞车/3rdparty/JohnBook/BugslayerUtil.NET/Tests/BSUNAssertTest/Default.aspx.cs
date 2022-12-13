/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;
using System.Text ;
using System.Diagnostics ;
using System.IO ;
using Wintellect ;


namespace Assert
{
	/// <summary>
	/// Summary description for WebForm1.
	/// </summary>
	public class WebForm1 : Page
	{
        protected System.Web.UI.WebControls.Button ButtonClearASPAssertTest;
        protected System.Web.UI.WebControls.Button ButtonDoDebugAssert;
        protected System.Web.UI.WebControls.Button ButtonDoTraceWriteLine;
        protected System.Web.UI.WebControls.CheckBox ShowInDbgCheckBox;
        protected System.Web.UI.WebControls.CheckBox ShowWithODSCheckBox;
        protected System.Web.UI.WebControls.CheckBox ShowInFileCheckBox;
        protected System.Web.UI.WebControls.CheckBox ShowInEvtLogCheckBox;
        protected System.Web.UI.WebControls.CheckBox StartDbgCheckBox;
        protected System.Web.UI.WebControls.Button ButtonAnotherAssert;
        protected System.Web.UI.WebControls.Button ButtonDoMoreTraceWriteLine;
        protected BugslayerUtil.NET.AssertControl AssertDisplay;
        protected System.Web.UI.WebControls.Button ButtonDoASPTraceWrite;

		public WebForm1()
		{
			Page.Init += new System.EventHandler(Page_Init);
		}

		private void Page_Load(object sender, System.EventArgs e)
		{
			// Put user code to initialize the page here
		}

		private void Page_Init(object sender, EventArgs e)
		{
			//
			// CODEGEN: This call is required by the ASP.NET Web Form Designer.
			//
			InitializeComponent();
			
			ASPTraceListener Listen = 
			            (ASPTraceListener)Application.Get ( "ASPTraceListener" ) ;			
			
			ShowInDbgCheckBox.Checked = Listen.ShowDebugLog ;
			ShowWithODSCheckBox.Checked = Listen.ShowOutputDebugString ;
			if ( null != Listen.Writer )
			{
			    ShowInFileCheckBox.Checked = true ;
			}
			if ( null != Listen.EventSource )
			{
			    ShowInEvtLogCheckBox.Checked = true ;
			}
			
			StartDbgCheckBox.Checked = Listen.LaunchDebuggerOnAssert ;
			
		}
		
		private void ClearAssertControl ( )
		{
            AssertDisplay.Text = "" ;
		}
		
        public void ButtonClearASPAssertTest_Click ( object         sender ,
                                                     System.EventArgs e     )
        {
            // Update the listener flags.
            UpdateListenerFlags ( ) ;            
            ClearAssertControl ( ) ;
        }                                                
		
        public void ButtonDoDebugAssert_Click ( object         sender ,
                                                System.EventArgs e     )
        {
            Foo ( ) ;
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
            // Update the listener flags.
            UpdateListenerFlags ( ) ;
            Debug.Assert ( false , "I'm a Debug.Assert" ) ;
        }
        
        private void ButtonAnotherAssert_Click(object sender, System.EventArgs e)
        {
            UpdateListenerFlags ( ) ;
            Debug.Assert ( 1 == 2 , "1 == 2" , "I need to learn basic math!" ) ;
            //Another one to see two assertions in a post.
            Debug.Assert ( 2 == 1 , "2 == 1" , "It's not transitive either!" ) ;            
        }

		#region Web Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{    
            this.ButtonClearASPAssertTest.Click += new System.EventHandler(this.ButtonClearASPAssertTest_Click);
            this.ButtonDoDebugAssert.Click += new System.EventHandler(this.ButtonDoDebugAssert_Click);
            this.ButtonDoTraceWriteLine.Click += new System.EventHandler(this.ButtonDoTraceWriteLine_Click);
            this.ButtonDoASPTraceWrite.Click += new System.EventHandler(this.ButtonDoASPTraceWrite_Click);
            this.ButtonAnotherAssert.Click += new System.EventHandler(this.ButtonAnotherAssert_Click);
            this.ButtonDoMoreTraceWriteLine.Click += new System.EventHandler(this.ButtonDoMoreTraceWriteLine_Click);
            this.Load += new System.EventHandler(this.Page_Load);

        }
		#endregion

        private void ButtonDoTraceWriteLine_Click(object sender, System.EventArgs e)
        {
            // Clear the assertion.
            ClearAssertControl ( ) ;
            // Update the listener flags.
            UpdateListenerFlags ( ) ;
            System.Diagnostics.Trace.WriteLine ( "This is from System.Diagnostics.Trace.WriteLine" ) ;
        }
        
        private void ButtonDoMoreTraceWriteLine_Click(object sender, System.EventArgs e)
        {
            ClearAssertControl ( ) ;
            // Update the listener flags.
            UpdateListenerFlags ( ) ;
            System.Diagnostics.Trace.Write ( "A message" , "Some category" ) ;
            System.Diagnostics.Trace.Write ( this ) ;
            System.Diagnostics.Trace.WriteLine ( "String" , "Category" ) ;
            System.Diagnostics.Trace.WriteLine ( this ) ;
        }

        private void ButtonDoASPTraceWrite_Click(object sender, System.EventArgs e)
        {
            // Clear the assertion.
            ClearAssertControl ( ) ;
            // Update the listener flags.
            UpdateListenerFlags ( ) ;
            Trace.Write ( "This is from the Page objects Trace.Write" ) ;
        }
        
        private void UpdateListenerFlags ( )
        {
            ASPTraceListener Listen = 
                (ASPTraceListener)Application.Get ( "ASPTraceListener" ) ;			
                
            if ( true == ShowInEvtLogCheckBox.Checked )
            {
                Listen.EventSource = "Application" ;
            }
            else
            {
                Listen.EventSource = null ;
            }
            
            Listen.ShowDebugLog = ShowInDbgCheckBox.Checked ;

            Listen.ShowOutputDebugString  = ShowWithODSCheckBox.Checked ;

            if ( true == ShowInFileCheckBox.Checked )
            {
                if ( null == Listen.Writer )
                {
                    Listen.Writer = new StreamWriter ( "c:\\log.txt" ) ;
                }
            }
            else
            {
                Listen.Writer = null ;
            }
            Listen.LaunchDebuggerOnAssert = StartDbgCheckBox.Checked ;
            
        }

	}
}
