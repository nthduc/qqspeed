/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System;
using System.Diagnostics ;

namespace Wintellect
{

/// <summary>
/// The ASP.NET <see cref="System.Web.HttpApplication"/> class you
/// derive your applications from.  This class automatically hooks up
/// the <see cref="Wintellect.ASPTraceListener"/> class so any
/// Debug.Assert or Trace.Write* will show up with the  rest of your
/// ASP.NET tracing code.  If you drop a the
/// <see cref="BugslayerUtil.NET.AssertControl"/> on your page, output
/// will go there as well.
/// </summary>
public class AssertHttpApplication : System.Web.HttpApplication
{

    /// <summary>
    /// The constructor that does all the work!
    /// </summary>
    /// <remarks>
    /// Simply calls
    /// <see cref="Wintellect.AssertHttpApplication.SetupTraceListener"/>
    /// and
    /// <see cref="Wintellect.AssertHttpApplication.SetupDebugListener"/>
    /// </remarks>
    public AssertHttpApplication ( )
    {
        // Call the functions that do the actual work.  I want the
        // code in other functions so that I can use the
        // appropriate conditional compilation directive on them.
        SetupTraceListener ( ) ;
        SetupDebugListener ( ) ;
    }

    /// <summary>
    /// Initializes the class.
    /// </summary>
    /// <remarks>
    /// The Init method is called AFTER the constructor. Unfortunately,
    /// the Application object is not available in the constructor so I
    /// must override the <see cref="System.Web.HttpApplication.Init"/>
    /// method to add the <see cref="ASPTraceListener"/> class to the
    /// application state bag.
    /// <para>
    /// If you use the Init method in your derivations, you MUST call
    /// the base method!!
    /// </para>
    /// </remarks>
    public override void Init ( )
    {
        // Call the base implementation.
        base.Init ( ) ;

        // Plop the app assert into the application state bag so
        // that the pages on this machine can get it.
        Application.Add ( "ASPTraceListener" , m_Listener ) ;
    }

#if ( TRACE || DEBUG )
    /// <summary>
    /// The private ASPTraceListener class.  This only exists if TRACE
    /// or DEBUG is defined.
    /// </summary>
    private ASPTraceListener m_Listener = new ASPTraceListener ( ) ;

    /// <summary>
    /// Returns the <see cref="Wintellect.ASPTraceListener"/> so you
    /// can set output properties.
    /// </summary>
    public ASPTraceListener Listener
    {
        get
        {
            return ( m_Listener ) ;
        }
    }
#endif

    /// <summary>
    /// Removes all object from Trace.Listeners
    /// </summary>
    /// <remarks>
    /// Takes care of removing all TraceListeners from
    /// <see cref="System.Diagnostics.Trace"/> and adding the
    /// <see cref="Wintellect.ASPTraceListener"/> as the only one.
    /// </remarks>
    [Conditional ( "TRACE" )]
    protected void SetupTraceListener ( )
    {
        // Loop through and kill all the trace listeners in both the
        // Trace and Debug objects and add my own listener.
        for ( int i = 0 ; i < Trace.Listeners.Count ; i++ )
        {
            Trace.Listeners.RemoveAt ( 0 ) ;
        }
        Trace.Listeners.Add ( m_Listener ) ;
    }

    /// <summary>
    /// Removes all Debug.Listeners
    /// </summary>
    /// <remarks>
    /// Takes care of removing all TraceListeners from
    /// <see cref="System.Diagnostics.Debug"/> and adding the
    /// <see cref="Wintellect.ASPTraceListener"/> as the only one.
    /// </remarks>
    [Conditional ( "DEBUG" )]
    protected void SetupDebugListener ( )
    {
        // Loop through and kill all the trace listeners in both the
        // Trace and Debug objects and add my own listener.
        for ( int i = 0 ; i < Debug.Listeners.Count ; i++ )
        {
            Debug.Listeners.RemoveAt ( 0 ) ;
        }

        Debug.Listeners.Add ( m_Listener ) ;
    }

}   // End of AssertHttpApplication class.

}   // End of Wintellect namespace.
