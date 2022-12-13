/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.IO ;
using System.Diagnostics ;

namespace Wintellect
{
/// <summary>
/// It's sad, but the default
/// <seealso cref="TextWriterTraceListener"/> does not write out
/// the full stack trace on assertions.  Why, I'll never know.
/// Use this class as a drop in replacement as it will do the
/// stack trace you expect.
/// </summary>
public class BugslayerTextWriterTraceListener :
	            TextWriterTraceListener
{
    // I want to override a single method, but noooo, I have to
    // mimic every base class ctor.  Sheez.
    
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerTextWriterTraceListener"/> class with
    /// <see cref="TextWriter "/> as the output recipient.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    public BugslayerTextWriterTraceListener ( ) : base ( ) { }
    
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerTextWriterTraceListener"/> class using the
    /// stream as the recipient of the debugging and tracing output.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    /// <param name="stream">
    /// A that represents the stream the
    ///  writes to.
    /// </param>
    public BugslayerTextWriterTraceListener ( Stream stream )
            : base ( stream ) { }

    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerTextWriterTraceListener"/> class using the
    /// file as the recipient of the debugging and tracing output.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    /// <param name="fileName">
    /// The name of the file the
    ///  writes to.
    /// </param>
    public BugslayerTextWriterTraceListener( string fileName )
            : base ( fileName ) { }
            
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerTextWriterTraceListener"/> class using the
    /// writer as the recipient of the debugging and tracing output.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    /// <param name="writer">
    /// A  that receives the output from the
    /// </param>
    public BugslayerTextWriterTraceListener ( TextWriter writer )
            : base ( writer ) { }
            
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerTextWriterTraceListener"/> class with the
    /// specified name, using the stream as the recipient of the
    /// debugging and tracing output.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    /// <param name="stream">
    /// A  that represents the stream the
    ///  writes to.
    /// </param>
    /// <param name="name">
    /// The name of the new instance.
    /// </param>
    public BugslayerTextWriterTraceListener ( Stream stream ,
                                              string name    )
            : base ( stream , name ) { }
            
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerTextWriterTraceListener"/> class with the
    /// specified name, using the file as the recipient of the
    /// debugging and tracing output.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    /// <param name="fileName">
    /// The name of the file the
    ///  writes to.
    /// </param>
    /// <param name="name">
    /// The name of the new instance.
    /// </param>
    public BugslayerTextWriterTraceListener ( string fileName ,
                                              string name      )
            : base ( fileName , name ) { }
    
    /// <summary>
    /// Initializes a new instance of the
    /// <see cref="BugslayerTextWriterTraceListener"/> class with the
    /// specified writer, using the file as the recipient of the
    /// debugging and tracing output.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    /// <param name="writer">
    /// A  that receives the output from the
    /// .
    /// </param>
    /// <param name="name">
    /// The name of the new instance.
    /// </param>
    public BugslayerTextWriterTraceListener ( TextWriter writer ,
                                              string     name    )
            : base ( writer , name ) { }

    /// <summary>
    /// Overrides the <see cref="TextWriterTraceListener "/> so that
    /// the stack trace is written to the text file.  An assertion
    /// without a stack trace is pretty worthless.  While there are
    /// two Fail methods, the 
    /// <see cref="System.Diagnostics.TextWriterTraceListener"/>.Fail
    /// version simply calls this version.
    /// </summary>
    /// <remarks>
    /// See <see cref="TextWriterTraceListener"/>
    /// </remarks>
    /// <param name="message">
    /// A message to emit.
    /// </param>
    /// <param name="detailMessage">
    /// A detailed message to emit.
    /// </param>
    public override void Fail ( string message       ,
                                string detailMessage  )
    {
        Writer.WriteLine ( "---- DEBUG ASSERTION FAILED ----" ) ;
        Writer.WriteLine ( "---- Assert Short Message ----" ) ;
        if ( null != message )
        {
            Writer.WriteLine ( message ) ;
        }
        Writer.WriteLine ( "---- Assert Long Message ----" ) ;
        if ( null != detailMessage )
        {
            Writer.WriteLine ( detailMessage ) ;
        }
        
        // There's four levels of stack between here and the user's
        // code.
        BugslayerStackTrace bst = new BugslayerStackTrace ( 4 ) ;
        Writer.WriteLine ( bst.ToString ( ) ) ;
    }
}   // End of BugslayerTextWriterTraceListener class


}   // End of Wintellect namespace.
