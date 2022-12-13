/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997 - 2003 John Robbins
----------------------------------------------------------------------*/
using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EnvDTE;


namespace Wintellect
{
/// <summary>
/// Summary description for OptionPropPageBase.
/// </summary>
public class OptionPropPageBase :
	                 System.Windows.Forms.UserControl ,
                     IDTToolsOptionsPage
{
	/// <summary>
	/// Required designer variable.
	/// </summary>
	private System.ComponentModel.Container components = null ;

	public OptionPropPageBase ( )
	{
		// This call is required by the Windows.Forms Form Designer.
		InitializeComponent();
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	protected override void Dispose( bool disposing )
	{
		if( disposing )
		{
			if ( components != null )
			{
				components.Dispose ( ) ;
			}
		}
		base.Dispose ( disposing ) ;
	}

	#region Component Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	private void InitializeComponent()
	{
		components = new System.ComponentModel.Container();
	}
	#endregion
	
    public virtual void OnAfterCreated ( DTE DTEObject )
    {
        // To ensure this option property page looks right, I need to
        // set all the fonts to what the user chose as the Dialog and
        // Tool Windows font.  I'll use the late-binding stuff to get
        // the values out of the DTE properties.
        Properties Props = DTEObject.get_Properties ( "FontsAndColors",
                                          "Dialogs and Tool Windows" );

        String FntName = (String)Props.Item ( "FontFamily" ).Value ;
    
        Object ObjTemp = Props.Item ( "FontSize" ).Value ;
        Int32  FntSize = Convert.ToInt32 ( ObjTemp ) ;
    
        // Create the font.
        Font DlgFont = new Font ( FntName            ,
                                  FntSize            ,
                                  GraphicsUnit.Point  ) ;
                    
        // Set the font on the dialog.
        this.Font = DlgFont ;
    
        // Loop through all the controls on the dialog and set their
        // fonts as well.  Some controls will pick the above up, but
        // not all so that's why I need to do this manually.
        foreach ( Control Ctl in this.Controls )
        {
            Ctl.Font = DlgFont ;
        }
    }
    
    public virtual void GetProperties ( ref object PropertiesObject )
    {
        PropertiesObject = null;
    }

    public virtual void OnOK ( )
    {
    }

    public virtual void OnCancel ( )
    {
    }
    public virtual void OnHelp ( )
    {
    }
    
}
}
