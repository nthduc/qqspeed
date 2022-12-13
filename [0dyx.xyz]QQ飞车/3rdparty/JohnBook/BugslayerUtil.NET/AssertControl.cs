/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System;
// Following three are needed by all custom controls.
using System.Web.UI;
using System.Web.UI.WebControls;
using System.ComponentModel;
// Needed for StringBuilder.
using System.Text ;
// Needed for the color.
using System.Drawing ;
using Wintellect ;

namespace BugslayerUtil.NET
{

/// <summary>
/// Drop the assertion control on any pages you want to see assertion
/// output.  As long as you derive your application from
/// <see cref="AssertHttpApplication"/> everything will get hooked up.
/// This control will use the ForeColor and FontSize properties.
/// </summary>
[DefaultProperty("Text"),
ToolboxData("<{0}:AssertControl runat=server></{0}:AssertControl>")]
public class AssertControl : System.Web.UI.WebControls.WebControl
{
    /// <summary>
    /// Private instance set by 
    /// <see cref="BugslayerUtil.NET.AssertControl.Text"/>
    /// </summary>
	private string m_text = null ;

    /// <summary>
    /// Gets and sets the text to display.
    /// </summary>
	[ Bindable ( true )         ,
		Category ( "Appearance" ) ,
		DefaultValue ( "" )        ]
	public string Text
	{
		get
		{
			return m_text;
		}

		set
		{
			m_text = value;
		}
	}
	
    /// <summary>
    /// Adds an assertion to the assertion control.
    /// </summary>
    /// <remarks>
    /// The <see cref="ASPTraceListener"/> class will call this method
    /// to add any assertion information generated on the server side.
    /// </remarks>
    /// <param name="Message">
    /// The main assertion message.
    /// </param>
    /// <param name="DetailMessage">
    /// The detailed message passed to the assertion.
    /// </param>
    /// <param name="Stk">
    /// The stack trace.
    /// </param>
    public void AddAssertion ( String              Message       ,
                               String              DetailMessage ,
                               BugslayerStackTrace Stk            )
    {
        // Build up the HTML to output.
        StringBuilder Str = new StringBuilder ( ) ;
        
        // Put the horizontal row on it.
        Str.Append ( "<HR width=\"100%\" SIZE=1>" ) ;
        
        // Make sure to put some space between assertions.
        if ( null != m_text )
        {
            Str.Append ( "<br><br>" ) ;
        }
        
        Str.Append ( Message ) ;
        Str.Append ( "<br>" ) ;
		
        if ( null != DetailMessage )
        {
            Str.Append ( "<br>" ) ;
            Str.Append ( DetailMessage ) ;
            Str.Append ( "<br><br>" ) ;
        }
        
        // Set up the options for formatting that this control needs.
        Stk.LineEnd = "<br>" ;
        Stk.SourceIndentString = "&nbsp&nbsp&nbsp&nbsp&nbsp" ;

        Str.Append ( Stk.ToString ( ) ) ;

        Str.Append ( "<HR width=\"100%\" SIZE=1>" ) ;
        
        StringBuilder FinalStr = new StringBuilder ( m_text ) ;
        FinalStr.Append ( Str.ToString ( ) ) ;
        
        m_text = FinalStr.ToString ( ) ;
            
    }
	
    /// <summary>
    /// Render this control to the output parameter specified.
    /// </summary>
    /// <remarks>
    /// 	<see cref="System.Web.UI.WebControls.WebControl.Render"/>
    /// </remarks>
    /// <param name="output">
    /// The HTML writer to write out to
    /// </param>
	protected override void Render(HtmlTextWriter output)
	{
        if ( null != m_text )
        {
            StringBuilder Str = new StringBuilder ( ) ;
    
            Str.Append ( "<span style=\"" ) ;
            
            if ( true == ForeColor.IsEmpty )
            {
                ForeColor = Color.Red ;
            }
            Str.Append ( " color:" ) ;
            Str.Append ( ForeColor.Name.ToString ( ) ) ;
            
            if ( false == Font.Size.IsEmpty )
            {
                Str.Append ( ";font-size:" ) ;
                Str.Append ( Font.Size.ToString ( ) ) ;
            }
            
            Str.Append ( "\">" ) ;
            
            Str.Append ( m_text ) ;
            
            Str.Append ( "</span>" ) ;
            
			output.Write ( Str.ToString ( ) ) ;
		}
	}
}   // End of AssertControl class.


}   // End of BugslayerUtil.NET namespace.
