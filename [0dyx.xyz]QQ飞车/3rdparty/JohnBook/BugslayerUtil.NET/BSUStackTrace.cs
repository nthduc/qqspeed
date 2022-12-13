/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.Diagnostics ;
using System.Reflection ;
using System.Text ;
using System.Threading ;

namespace Wintellect
{
/// <summary>
/// A stack trace class that actually works to get the source and line
/// information when you call ToString.  The <see cref="StackTrace "/>
/// class in the VS.NET V1 bits never seems to include the source and
/// line even though the documentation says it does.
/// If you don't care about getting the source and line with a stack
/// trace, use the StackTrace class directly.
/// </summary>
public class BugslayerStackTrace : StackTrace
{
	/*//////////////////////////////////////////////////////////////////
	// Constructors.  Note that all of the ctors here (except the ones
	// initialized from exceptions) tell the base class to skip the ctor
	// for this class in it's stack trace.
	//////////////////////////////////////////////////////////////////*/
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerStackTrace"/> class
    /// from the current location, in a caller's frame.
    /// </summary>
	public BugslayerStackTrace ( ) : base ( true )
	{
	    m_sLineEnd = DefaultLineEnd ;
	    m_sSourceIndentString = null ;
	    FunctionIndent = DefaultFunctionIndent ;
	}

    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerStackTrace"/> class.
    /// </summary>
    /// <param name="e">
    /// The exception object from which to construct the stack trace.
    /// </param>
    public BugslayerStackTrace ( Exception e ) : base ( e , true )
    {
        m_sLineEnd = DefaultLineEnd ;
        m_sSourceIndentString = null ;
        FunctionIndent = DefaultFunctionIndent ;
    }
        
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerStackTrace"/> class from the current
    /// location, in a caller's frame, optionally skipping the given
    /// number of frames.
    /// </summary>
    /// <param name="skipFrames">
    /// The number of frames up the stack from which to start the trace.
    /// </param>
    public BugslayerStackTrace ( int skipFrames )
            : base ( skipFrames + 1 , true )
    {
        m_sLineEnd = DefaultLineEnd ;
        m_sSourceIndentString = null ;
        FunctionIndent = DefaultFunctionIndent ;
    }
    
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerStackTrace"/> class using the provided
    /// exception object, optionally skipping the given number of
    /// frames.
    /// </summary>
    /// <param name="e">
    /// The exception object from which to construct the stack trace.
    /// </param>
    /// <param name="skipFrames">
    /// The number of frames up the stack from which to start the trace.
    /// </param>
    public BugslayerStackTrace ( Exception e , int skipFrames)
            : base ( e , skipFrames , true )
    {
        m_sLineEnd = DefaultLineEnd ;
        m_sSourceIndentString = null ;
        FunctionIndent = DefaultFunctionIndent ;
    }
    
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerStackTrace"/> class for another thread,
    /// optionally capturing source information.
    /// </summary>
    /// <param name="targetThread">
    /// The thread whose stack trace is requested.
    /// </param>
    public BugslayerStackTrace ( Thread targetThread  )
            : base ( targetThread , true )
    {
        m_sLineEnd = DefaultLineEnd ;
        m_sSourceIndentString = null ;
        FunctionIndent = DefaultFunctionIndent ;
    }

    
    /*/////////////////////////////////////////////////////////////////
    // Overloaded methods.
    /////////////////////////////////////////////////////////////////*/
    
    /// <summary>
    /// Builds a readable representation of the stack trace.
    /// </summary>
    /// <remarks>
    /// This includes, method name, source and source line.
    /// </remarks>
    /// <returns>
    /// A readable representation of the stack trace.
    /// </returns>
    public override string ToString ( )
    {
        // New up the StringBuilder to hold all the stuff.
        StringBuilder StrBld = new StringBuilder ( ) ;

        // First thing on is a line feed.
        StrBld.Append ( DefaultLineEnd ) ;
            
        // Loop'em and do'em!  You can't use foreach here as StackTrace
        // is not derived from IEnumerable.
        for ( int i = 0 ; i < FrameCount ; i++ )
        {
            StackFrame StkFrame = GetFrame ( i ) ;
            if ( null != StkFrame )
            {
                BuildFrameInfo ( StrBld , StkFrame ) ;
            }
        }
        return ( StrBld.ToString ( ) ) ;
    }

    /*/////////////////////////////////////////////////////////////////
    // Private methods.
    /////////////////////////////////////////////////////////////////*/
    /// <summary>
    /// Takes care of the scut work to convert a frame into a string
    /// and to plop it into a string builder.
    /// </summary>
    /// <param name="StrBld">
    /// The StringBuilder to append the results to.
    /// </param>
    /// <param name="StkFrame">
    /// The stack frame to convert.
    /// </param>
    private void BuildFrameInfo ( StringBuilder StrBld   ,
                                  StackFrame    StkFrame  )
    {
        // Get the method from all the cool reflection stuff.
        MethodBase Meth = StkFrame.GetMethod ( ) ;

        // If nothing is returned, get out now.
        if ( null == Meth )
        {
            return ;
        }
        
        // Grab the method.
        String StrMethName = Meth.ReflectedType.Name ;
        
        // Slap in the function indent if one is there.
        if ( null != FunctionIndent )
        {
            StrBld.Append ( FunctionIndent ) ;
        }
        
        // Get the class type and name on there.
        StrBld.Append ( StrMethName ) ;
        StrBld.Append ( "." ) ;
        StrBld.Append ( Meth.Name ) ;
        StrBld.Append ( "(" ) ;
        
        // Slap the parameters on, including all param names.
        ParameterInfo[] Params = Meth.GetParameters ( ) ;
        for ( int i = 0 ; i < Params.Length ; i++ )
        {
            ParameterInfo CurrParam = Params[ i ] ;
            StrBld.Append ( CurrParam.ParameterType.Name ) ;
            StrBld.Append ( " " ) ;
            StrBld.Append ( CurrParam.Name ) ;
            if ( i != ( Params.Length - 1 ) )
            {
                StrBld.Append ( ", " ) ;
            }
        }

        // Close the param list.
        StrBld.Append ( ")" ) ;
        
        // Get the source and line on only if there is one.
        if ( null != StkFrame.GetFileName ( ) )
        {
            // Am I supposed to indent the source?  If so, I need to put
            // a line break on the end followed by the indent.
            if ( null != SourceIndentString )
            {
                StrBld.Append ( LineEnd ) ;
                StrBld.Append ( SourceIndentString ) ;
            }
            else
            {
                // Just add a space.
                StrBld.Append ( ' ' ) ;
            }
        
            // Get the file and line of the problem on here.
            StrBld.Append ( StkFrame.GetFileName ( ) ) ;
            StrBld.Append ( "(" ) ;
            StrBld.Append ( StkFrame.GetFileLineNumber().ToString());
            StrBld.Append ( ")" ) ;
        }
        // Always stick a line feed on.
        StrBld.Append ( LineEnd ) ;
    }
    
    /*/////////////////////////////////////////////////////////////////
    // Properties.
    /////////////////////////////////////////////////////////////////*/

    /// <summary>
    /// The private string for the line ending characters.
    /// </summary>
    private string m_sLineEnd ;
    	
    /// <summary>
    /// Sets the characters to use for the end of the line.
    /// </summary>
    public string LineEnd
    {
        get
        {
            return ( m_sLineEnd ) ;
        }
        set
        {
            m_sLineEnd = value ;
        }
    }

    /// <summary>
    /// The private string for holding the indent characters.
    /// </summary>
    private string m_sSourceIndentString ;
    
    /// <summary>
    /// Holds the string to use for indenting sourse code.  If this is
    /// null, the default, the source and line is place on the same
    /// line as the function name.
    /// </summary>
    public string SourceIndentString
    {
        get
        {
            return ( m_sSourceIndentString ) ;
        }
        set
        {
            m_sSourceIndentString = value ;
        }
    }
    
    /// <summary>
    /// The private string for holding the indent to put on the front
    /// of the function.
    /// </summary>
    private string m_sFunctionIndent ;
    
    /// <summary>
    /// The string to put at the beginning of all functions.  This is
    /// \t\t\t to mimic the default StackTrace.ToString().
    /// </summary>
    public string FunctionIndent
    {
        get
        {
            return ( m_sFunctionIndent ) ;
        }
        set
        {
            m_sFunctionIndent = value ;
        }
    }
    
    /*/////////////////////////////////////////////////////////////////
    // Constants.
    /////////////////////////////////////////////////////////////////*/
    /// <summary>
    /// The default line ending, "\r\n".
    /// </summary>
    public const string DefaultLineEnd = "\r\n" ;
    /// <summary>
    /// The default function indent, "\t".
    /// </summary>
    public const string DefaultFunctionIndent = "\t" ;
    
    
}   // End of BugslayerStackTrace class.

}   // End of Wintellect namespace.
