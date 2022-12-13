/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.IO ;
using System.Diagnostics ;
using System.Text ;

namespace Wintellect
{
/// <summary>
/// The event log trace listener class with real stack walking,
/// </summary>
/// <remark>
/// It's doubly sad, but the default
/// <seealso cref="EventLogTraceListener"/> does not write out
/// the full stack trace on assertions and it's sealed so you
/// can't derive from it.  I can't see a single reason
/// why it's sealed.
/// Use this class as a drop in replacement as it will do the
/// stack trace you expect.
/// </remark>
public class BugslayerEventLogTraceListener :
             TraceListener
{
    /// <summary>
    /// class without a trace listener
    /// </summary>
    /// <remarks>
    /// See <see cref="System.Diagnostics.EventLogTraceListener"/>.
    /// </remarks>
    public BugslayerEventLogTraceListener ( )
    {
        EventLog = null ;
        Name = null ;
    }
    
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="Wintellect.BugslayerEventLogTraceListener"/>
    /// class using the specified event log.
    /// </summary>
    /// <remarks>
    /// See <see cref="System.Diagnostics.EventLogTraceListener"/>.
    /// </remarks>
    /// <param name="eventLog">
    /// An that specifies the event log to
    /// write to.
    /// </param>
    public BugslayerEventLogTraceListener ( EventLog eventLog )
    {
        EventLog = eventLog ;
        Name = null ;
    }
    
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="Wintellect.BugslayerEventLogTraceListener"/>
    /// the specified source
    /// </summary>
    /// <remarks>
    /// See <see cref="System.Diagnostics.EventLogTraceListener"/>
    /// </remarks>
    /// <param name="source">
    /// The name of an existing event log source.
    /// </param>
    public BugslayerEventLogTraceListener ( string source )
    {
        EventLog = new EventLog ( ) ;
        EventLog.Source = source ;
        Name = null ;
    }
    
    /// <summary>
    /// Private instance set by
    /// <see cref="Wintellect.BugslayerEventLogTraceListener.EventLog"/>
    /// </summary>
    private EventLog m_EventLog ;
    
    // Wow, you can have properties names that are the type name...
    // Scary.
    /// <summary>
    /// Gets or sets the event log to write to.
    /// </summary>
    public EventLog EventLog
    {
        get
        {
            return ( m_EventLog ) ;
        }
        set
        {
            m_EventLog = value ;
        }
    }
    
    /// <summary>
    /// Private instance set by
    /// <see cref="Wintellect.BugslayerEventLogTraceListener.Name"/>
    /// </summary>
    private String m_sName ;
    /// <summary>
    /// Gets or sets the name of this
    /// <see cref="Wintellect.BugslayerEventLogTraceListener"/>.
    /// </summary>
    public override string Name
    {
        get
        {
            return ( m_sName ) ;
        }
        set
        {
            m_sName = value ;
        }
    }
    
    /// <summary>
    /// Wintellect.BugslayerEventLogTraceListener.
    /// </summary>
    /// <remarks>
    /// See <see cref="System.Diagnostics.EventLog.Close"/>
    /// </remarks>
    public override void Close ( )
    {
        if ( null != EventLog )
        {
            EventLog.Close ( ) ;
        }
    }
    
    /// <summary>
    /// Writes a message to the event log for this instance.
    /// </summary>
    /// <remarks>
    /// See <see cref="System.Diagnostics.EventLog"/>
    /// </remarks>
    /// <param name="message">
    /// A message to write.
    /// </param>
    public override void Write ( string message )
    {
        if ( null != EventLog )
        {
            EventLog.WriteEntry ( message                       ,
                                  EventLogEntryType.Information  ) ;
        }
    }
    
    /// <summary>
    /// Writes a message to the event log for this instance.
    /// </summary>
    /// <remarks>
    /// See <see cref="System.Diagnostics.EventLog"/>
    /// </remarks>
    /// <param name="message">
    /// The message to write.
    /// </param>
    public override void WriteLine ( string message )
    {
        Write ( message ) ;
    }
    
    /// <summary>
    /// Overrides the <see cref="TraceListener "/>.
    /// </summary>
    /// <remarks>
    /// Overrides the <see cref="TraceListener "/> so that
    /// the stack trace is written to the event log.  An assertion
    /// without a stack trace is pretty worthless.  While there are
    /// two Fail methods, the EventLogTraceListener one string
    /// parameter version simply calls this version.
    /// </remarks>
    /// <param name="Message">
    /// The message to display.
    /// </param>
    /// <param name="DetailMessage">
    /// The detailed message to display.
    /// </param>
    public override void Fail ( string Message       ,
                                string DetailMessage  )
    {
        if ( null == EventLog )
        {
            return ;
        }
        
        StringBuilder StrBld = new StringBuilder ( ) ;
        
        StrBld.Append ( "---- DEBUG ASSERTION FAILED ----\r\n" ) ;
        StrBld.Append ( "---- Assert Short Message ----\r\n" ) ;
        if ( null != Message )
        {
            StrBld.Append ( Message ) ;
            StrBld.Append ( "\r\n" ) ;
        }
        StrBld.Append  ( "---- Assert Long Message ----\r\n" ) ;
        if ( null != DetailMessage )
        {
            StrBld.Append ( DetailMessage ) ;
            StrBld.Append ( "\r\n" ) ;
        }
    
        // There's five levels of stack between here and the user's
        // code.
        BugslayerStackTrace bst = new BugslayerStackTrace ( 5 ) ;
        
        // Make the text prettier for the event log.
        bst.SourceIndentString = "     " ;
        bst.FunctionIndent = "  " ;
        
        StrBld.Append ( bst.ToString ( ) ) ;
        
        // Now write out the text to the event log.
        EventLog.WriteEntry ( StrBld.ToString ( )     ,
                              EventLogEntryType.Error  ) ;
    }
    
    /// <summary>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Fail"/>
    /// </summary>
    /// <remarks>
    /// Overrides <see cref="System.Diagnostics.TraceListener.Fail"/>
    /// </remarks>
    /// <param name="Message">
    /// The message to display.
    /// </param>
    public override void Fail ( string Message )
    {
        Fail ( Message , null ) ;
    }

    
}   // End of BugslayerEventLogTraceListener class

}   // End of Wintellect namespace.
