/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
// Needed for StringBuilder.
using System.Text ;
// Needed for the TextWriter class.
using System.IO ;
// Needed for importing OutputDebugString.
using System.Runtime.InteropServices ;
// Needed to look up all the managed code info...
using System.Reflection ;
// Needed for the ArrayList.
using System.Collections ;
// Needed for all the web stuff.
using System.Web ;
using System.Web.UI ;
// The diagnostic class.
using System.Diagnostics ;
using BugslayerUtil.NET ;


namespace Wintellect
{
/// <summary>
/// The <see cref="System.Diagnostics.TraceListener"/> derived class
/// that redirects the Debug and Trace objects methods to the
/// appropriate ASP.NET trace and AssertPage.AssertionOutput method.
/// </summary>
public class ASPTraceListener : TraceListener
{

    #region Standard TraceListener Methods
    /// <summary>
    /// The usual constructor.
    /// </summary>
    /// <remarks>
    /// </remarks>
    public ASPTraceListener()
    {
    }

    ///////////////////////////////////////////////////////////////////
    ///////////////// START OF TraceListener METHODS //////////////////
    ///////////////////////////////////////////////////////////////////

    /// <summary>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Fail"/>
    /// </summary>
    /// <remarks>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Fail"/>
    /// </remarks>
    /// <param name="Message">
    /// The message to display.
    /// </param>
    public override void Fail ( String Message )
    {
        Fail ( Message , null ) ;
    }

    /// <summary>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Fail"/>
    /// </summary>
    /// <remarks>
    /// </remarks>
    /// <param name="Message">
    /// The message to display.
    /// </param>
    /// <param name="DetailMessage">
    /// The detailed message to display.
    /// </param>
    public override void Fail ( String Message       ,
                                String DetailMessage  )
    {
        // For reasons beyond me, it's nearly impossible to
        // consistently be able to get the number of items on the
        // stack up to the Debug.Assert.  Sometimes it's 4 other
        // times it's 5.  Unfortunately, the only way I can see
        // to handle this is to manually figure it out.  Bummer.
        StackTrace StkSheez = new StackTrace ( ) ;

        int i = 0 ;
        for ( ; i < StkSheez.FrameCount ; i++ )
        {
            MethodBase Meth = StkSheez.GetFrame(i).GetMethod ( ) ;

            // If nothing is returned, get out now.
            if ( null != Meth )
            {
                if ( "Debug" == Meth.ReflectedType.Name )
                {
                    i++ ;
                    break ;
                }
            }
        }
        BugslayerStackTrace Stk = new BugslayerStackTrace ( i ) ;
        HandleOutput ( Message , DetailMessage , Stk ) ;
    }

    /// <summary>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </summary>
    /// <remarks>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </remarks>
    /// <param name="Obj">
    /// An Object whose fully qualified class name you want to write.
    /// </param>
    /// <param name="Message">
    /// A message to write.
    /// </param>
    new public virtual void Write ( Object Obj , String Message )
    {
        StringBuilder Str = new StringBuilder ( Message ) ;
        Str.Append ( ": " ) ;
        Str.Append ( Obj.GetType().ToString ( ) ) ;
        HandleOutput ( Str.ToString ( ) , null , null ) ;
    }

    /// <summary>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </summary>
    /// <remarks>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </remarks>
    /// <param name="Message">
    /// A message to write.
    /// </param>
    /// <param name="Category">
    /// A category name used to organize the output.
    /// </param>
    new public virtual void Write ( String Message , String Category )
    {
        StringBuilder Str = new StringBuilder ( Category ) ;
        Str.Append ( ": " ) ;
        Str.Append ( Message ) ;
        HandleOutput ( Str.ToString ( ) , null , null ) ;
    }

    /// <summary>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </summary>
    /// <remarks>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </remarks>
    /// <param name="Message">
    /// A message to write.
    /// </param>
    public override void Write ( String Message )
    {
        HandleOutput ( Message , null , null ) ;
    }

    /// <summary>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </summary>
    /// <remarks>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Write"/>
    /// </remarks>
    /// <param name="Obj">
    /// An  whose fully qualified class name
    /// you want to write.
    /// </param>
    new public virtual void Write ( Object Obj )
    {
        HandleOutput ( Obj.ToString ( ) , null , null ) ;
    }

    /// <summary>
    /// Overrides
    /// <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </summary>
    /// <remarks>
    /// Overrides
    /// <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </remarks>
    /// <param name="Message">
    /// A message to write.
    /// </param>
    /// <param name="Category">
    /// A category name used to organize the output.
    /// </param>
    new public virtual void WriteLine ( String Message  ,
                                        String Category  )
    {
        StringBuilder Str = new StringBuilder ( Category ) ;
        Str.Append ( ": " ) ;
        Str.Append ( Message ) ;
        HandleOutput ( Str.ToString ( ) , null , null ) ;
    }

    /// <summary>
    /// Overrides
    /// <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </summary>
    /// <remarks>
    /// Overrides
    ///  <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </remarks>
    /// <param name="Obj">
    /// An  whose fully qualified class name
    /// you want to write.
    /// </param>
    /// <param name="Message">
    /// A message to write.
    /// </param>
    new public virtual void WriteLine ( Object Obj     ,
                                        String Message  )
    {
        StringBuilder Str = new StringBuilder ( Message ) ;
        Str.Append ( ": " ) ;
        Str.Append ( Obj.GetType().ToString ( ) ) ;
        HandleOutput ( Str.ToString ( ) , null , null ) ;
    }

    /// <summary>
    /// Overrides
    /// <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </summary>
    /// <remarks>
    /// Overrides
    /// <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </remarks>
    /// <param name="Message">
    /// A message to write.
    /// </param>
    public override void WriteLine ( String Message )
    {
        HandleOutput ( Message , null , null ) ;
    }

    /// <summary>
    /// Overrides
    /// <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </summary>
    /// <remarks>
    /// Overrides
    /// <see cref="System.Diagnostics.TraceListener.WriteLine"/>
    /// </remarks>
    /// <param name="Obj">
    /// An  whose fully qualified class name
    /// you want to write.
    /// </param>
    new public virtual void WriteLine ( Object Obj )
    {
        HandleOutput ( Obj.ToString ( ) , null , null ) ;
    }

    ////////////////////////////////////////////////////////////////
    ///////////////// END OF TraceListener METHODS /////////////////
    ////////////////////////////////////////////////////////////////
    #endregion

    ///////////////////////////////////////////////////////////////////
    /////////////////////// START OF PROPERTIES ///////////////////////
    ///////////////////////////////////////////////////////////////////
    #region ASPTraceListener Properties
    
    /// <summary>
    /// Private instance set by
    /// <see cref="Wintellect.ASPTraceListener.ShowDebugLog"/>
    /// </summary>
    private bool m_ShowDebugLog = true ;
    /// <summary>
    ///  Show assertion output in any attached debuggers.  This is
    ///  true by default.
    /// </summary>
    public bool ShowDebugLog
    {
        get
        {
            return ( m_ShowDebugLog ) ;
        }
        set
        {
            m_ShowDebugLog = value ;
        }
    }

    /// <summary>
    /// Private instance set by
    /// <see cref="Wintellect.ASPTraceListener.ShowOutputDebugString"/>
    /// </summary>
    private bool m_ShowOutputDebugString = false ;
    /// <summary>
    /// Show assertion statements through the operating system
    /// OutputDebugString call.  This is off by default.
    /// </summary>
    public bool ShowOutputDebugString
    {
        get
        {
            return ( m_ShowOutputDebugString ) ;
        }
        set
        {
            m_ShowOutputDebugString = value ;
        }
    }

    /// <summary>
    /// Private instance set by
    /// <see cref="Wintellect.ASPTraceListener.EventSource"/>
    /// </summary>
    private String m_EventSource = null ;
    /// <summary>
    /// Sets the name of the event source for writing assertions to
    /// the event log and turns on output to the event log.
    /// </summary>
    public String EventSource
    {
        get
        {
            return ( m_EventSource ) ;
        }
        set
        {
            // If I've already got an event trace open, it doesn't
            // matter.  The magic of garbage collection at work!
            m_EventSource = value ;

            // If the new setting is null, kill the existing log.
            if ( null == m_EventSource )
            {
                if ( null != m_EvtLog )
                {
                    m_EvtLog = null ;
                }
            }
            else
            {
                // Does the event source exist?
                if ( false ==
                    EventLog.SourceExists ( m_EventSource ) )
                {
                    EventLog.CreateEventSource ( m_EventSource ,
                        m_EventSource  ) ;
                }
                m_EvtLog = new EventLog ( ) ;
                m_EvtLog.Source = m_EventSource  ;
            }
        }
    }

    /// <summary>
    /// The internal EventLog I keep around to do the
    /// event log writing.
    /// </summary>
    private EventLog m_EvtLog = null ;

    /// <summary>
    /// Private instance set by
    /// <see cref="Wintellect.ASPTraceListener.Writer"/>
    /// </summary>
    private TextWriter m_Writer = null ;
    /// <summary>
    /// The TextStream for file/pipe/whatever output.
    /// </summary>
    /// <remarks>
    /// Setting this property will direct assertions to the file.
    /// Keep in mind that you might want to ensure that this writer
    /// is synchronized because this one is used across all threads.
    /// </remarks>
    public TextWriter Writer
    {
        get
        {
            return ( m_Writer ) ;
        }
        set
        {
            m_Writer = value ;
        }
    }

    /// <summary>
    /// The declaration for OutputDebugString.
    /// </summary>
    /// <remarks>
    /// An interop call to KERNEL32!OutputDebugStringA
    /// </remarks>
    /// <param name="s">
    /// The string to display.
    /// </param>
    [DllImport("kernel32.dll")]
    private static extern void OutputDebugStringA ( String s ) ;

    /// <summary>
    /// Private instance set by
    /// <see cref="Wintellect.ASPTraceListener.LaunchDebuggerOnAssert"/>
    /// </summary>
    private bool m_LaunchDebuggerOnAssert = true ;
    /// <summary>
    /// If true, and an interactive user is logged in, a debugger
    /// will be launched whenever an assertion is triggered.
    /// The default is false.
    /// </summary>
    public bool LaunchDebuggerOnAssert
    {
        get
        {
            return ( m_LaunchDebuggerOnAssert ) ;
        }
        set
        {
            m_LaunchDebuggerOnAssert = value ;
        }
    }


    ////////////////////////////////////////////////////////////////
    ////////////////////// END OF PROPERTIES ///////////////////////
    ////////////////////////////////////////////////////////////////
    #endregion

    ///////////////////////////////////////////////////////////////////
    /////////////////// START OF WORKER FUNCTIONS//////////////////////
    ///////////////////////////////////////////////////////////////////
    /// <summary>
    /// Private assertion title message.
    /// </summary>
    private const String AssertionMsg = "ASSERTION FAILURE!\r\n" ;
    /// <summary>
    /// Private hard coded carraige return line feed string.
    /// </summary>
    private const String CrLf = "\r\n" ;
    /// <summary>
    /// The private assertion string boarder.
    /// </summary>
    private const String Border =
        "----------------------------------------\r\n" ;

    /// <summary>
    /// Output the assertion or trace message.
    /// </summary>
    /// <remarks>
    /// Takes care of all the output for the trace or assertion.
    /// </remarks>
    /// <param name="Message">
    /// The message to display.
    /// </param>
    /// <param name="DetailMessage">
    /// The detailed message to display.
    /// </param>
    /// <param name="Stk">
    /// The  value
    /// containing stack walk information for the assertion.  If this is
    /// not null, this function is called from an assertion.  Trace
    /// output sets this to null.
    /// </param>
    protected void HandleOutput ( String              Message       ,
                                  String              DetailMessage ,
                                  BugslayerStackTrace Stk            )
    {
        // Create the StringBuilder to help me build the text
        // string for the output here.
        StringBuilder StrOut = new StringBuilder ( ) ;

        // If the StackArray is not null, it's an assertion.
        if ( null != Stk )
        {
            StrOut.Append ( Border ) ;
            StrOut.Append ( AssertionMsg ) ;
            StrOut.Append ( Border ) ;
        }

        // Pop on the message.
        StrOut.Append ( Message ) ;
        StrOut.Append ( CrLf ) ;

        // Poke on the detail message if it's there.
        if ( null != DetailMessage )
        {
            StrOut.Append ( DetailMessage ) ;
            StrOut.Append ( CrLf ) ;
        }

        // If an assertion, show the stack below a border.
        if ( null != Stk )
        {
            StrOut.Append ( Border ) ;
        }

        // Go through and poke on all the stack information
        // if it's present.
        if ( null != Stk )
        {
            Stk.SourceIndentString = "      " ;
            Stk.FunctionIndent = "   " ;
            StrOut.Append ( Stk.ToString ( ) ) ;
        }

        // Since I use the string multiple places, get it once here.
        String FinalString = StrOut.ToString ( ) ;

        if ( ( true == m_ShowDebugLog         ) &&
             ( true == Debugger.IsLogging ( ) )    )
        {
            Debugger.Log ( 0 , null , FinalString ) ;
        }
        if ( true == m_ShowOutputDebugString )
        {
            OutputDebugStringA ( FinalString ) ;
        }
        if ( null != m_EvtLog )
        {
            m_EvtLog.WriteEntry ( FinalString ,
                System.Diagnostics.EventLogEntryType.Error ) ;
        }
        if ( null != m_Writer )
        {
            m_Writer.WriteLine ( FinalString ) ;
            // Add a CRLF just in case.
            m_Writer.WriteLine ( "" ) ;
            m_Writer.Flush ( ) ;
        }

        // Always do the page level output!
        if ( null != Stk )
        {
            // Do the warning output to the current TraceContext.
            HttpContext.Current.Trace.Warn ( FinalString ) ;
            
            // Hunt down the AssertionControl on the page.
            
            // First, make sure the handler is a page!
            if ( HttpContext.Current.Handler is System.Web.UI.Page )
            {
                System.Web.UI.Page CurrPage =
                    (System.Web.UI.Page)HttpContext.Current.Handler ;
                    
                // Take the easy way out if there are no
                // controls (which I doubt!)
                if ( true == CurrPage.HasControls( ) )
                {
                    // Hunt down the control.
                    AssertControl AssertCtl = null ;
                    FindAssertControl ( CurrPage.Controls ,
                                        out AssertCtl      ) ;
                                        
                    // If there was one, add the happy assertion!
                    if ( null != AssertCtl )
                    {
                        AssertCtl.AddAssertion ( Message       ,
                                                 DetailMessage ,
                                                 Stk            ) ;
                    }
                }
            }

            // Finally, launch the debugger if I'm supposed to.
            if ( true == m_LaunchDebuggerOnAssert )
            {
                // If a debugger is already attached, I can just use
                // Debugger.Break on it.  It doesn't matter where the
                // debugger is running, as long as it's running on this
                // process.
                if ( true == Debugger.IsAttached )
                {
                    Debugger.Break ( ) ;
                }
                else
                {
                    // With the changes to the security model for the
                    // RTM release of .NET, the ASPNET account that
                    // ASPNET_WP.EXE uses is set to User instead of
                    // running as the System account.  In order to
                    // allow Debugger.Launch to work, you need to add
                    // ASPNET to the Debugger Users group.  While this
                    // is safe for development systems, you may want
                    // to be careful on production systems.
                    bool bRet = IsRequestFromLocalMachine ( ) ;
                    if ( true == bRet )
                    {
                        Debugger.Launch ( ) ;
                    }
                }
            }
        }
        else
        {
            // The TraceContext is accessable right off the
            // HttpContext.
            HttpContext.Current.Trace.Write ( FinalString ) ;
        }
    }

    /// <summary>
    /// Determines if the request came from a local machine.
    /// </summary>
    /// <remarks>
    /// Checks if the IP address is 127.0.0.1 or the server variable
    /// LOCAL_ADDR matches the current machine.
    /// </remarks>
    /// <returns>
    /// Returns true if the request came from the local machine,
    /// false otherwise.
    /// </returns>
    private bool IsRequestFromLocalMachine ( )
    {
        // Get the request object.
        HttpRequest Req = HttpContext.Current.Request ;

        // Is the user sitting on the loopback node?
        bool bRet = Req.UserHostAddress.Equals ( "127.0.0.1" ) ;
        if ( false == bRet )
        {
            // Get the local IP address out of the server
            // variables.
            String LocalStr =
                Req.ServerVariables.Get ( "LOCAL_ADDR" ) ;
            // Compare the local IP with the IP address that
            // accompanied the request.
            bRet = Req.UserHostAddress.Equals ( LocalStr ) ;
        }
        return ( bRet ) ;

    }
    
    /// <summary>
    /// Finds any assertion controls on the page.
    /// </summary>
    /// <remarks>
    /// All assertion controls have the name "AssertControl" so this
    /// method simply loops through the page's control collection
    /// looking for them.  It also looks through children of children
    /// recursively.
    /// </remarks>
    /// <param name="CtlCol">
    /// The collection control to look through.
    /// </param>
    /// <param name="AssertCtrl">
    /// The output parameter that contains the assertion control found.
    /// </param>
    private void FindAssertControl ( ControlCollection CtlCol    ,
                                     out AssertControl AssertCtrl )
		                                    
    {
        // Loop through all the controls in the control array.
        foreach ( Control Ctl in CtlCol )
        {
            // Is this one the assertion control?
            if ( "AssertControl" == Ctl.GetType().Name )
            {
                // Yep!  Stop now.
                AssertCtrl = (AssertControl)Ctl ;
                return ;
            }
            else
            {
                // If this control has children do them too.
                if ( true == Ctl.HasControls ( ) )
                {
                    FindAssertControl ( Ctl.Controls ,
                                        out AssertCtrl ) ;
                    // If one of the children had the assertion,
                    // I can stop now.
                    if ( null != AssertCtrl )
                    {
                        return ;
                    }
                }
            }
        }
        // Didn't find it in this chain.
        AssertCtrl = null ;
        return  ;
    }
    
}   // End of ASPTraceListener class.

}   // End of Wintellect namespace.
