namespace SimpleToolWindow
{
using System;
using Microsoft.Office.Core;
using Extensibility;
using System.Runtime.InteropServices;
using EnvDTE;
using System.Windows.Forms;
using VSNetToolHostShimLib ;
using System.Text ;

#region Read me for Add-in installation and setup information.
// When run, the Add-in wizard prepared the registry for the Add-in.
// At a later time, if the Add-in becomes unavailable for reasons such as:
//   1) You moved this project to a computer other than which is was originally created on.
//   2) You chose 'Yes' when presented with a message asking if you wish to remove the Add-in.
//   3) Registry corruption.
// you will need to re-register the Add-in by building the MyAddin21Setup project
// by right clicking the project in the Solution Explorer, then choosing install.
#endregion

/// <summary>
///   The object for implementing an Add-in.
/// </summary>
/// <seealso class='IDTExtensibility2' />
[GuidAttribute("10AA39B3-5B26-4656-BD96-B483FC501E3C"),
 ProgId("SimpleToolWindow")]
public class Connect : Object, Extensibility.IDTExtensibility2
{
	/// <summary>
	///		Implements the constructor for the Add-in object.
	///		Place your initialization code within this method.
	/// </summary>
	public Connect()
	{
	}

	/// <summary>
	///      Implements the OnConnection method of the IDTExtensibility2 interface.
	///      Receives notification that the Add-in is being loaded.
	/// </summary>
	/// <param term='application'>
	///      Root object of the host application.
	/// </param>
	/// <param term='connectMode'>
	///      Describes how the Add-in is being loaded.
	/// </param>
	/// <param term='addInInst'>
	///      Object representing this Add-in.
	/// </param>
	/// <seealso class='IDTExtensibility2' />
	public void OnConnection ( object           application ,
	                           ext_ConnectMode  connectMode ,
	                           object           addInInst   ,
	                           ref System.Array custom       )
	{
	
	    try
	    {
		    ApplicationObject = (_DTE)application;
		    AddInInstance = (AddIn)addInInst;
    		
		    // Your tool window must have a unique GUID.
		    String guid = "{E16579A4-5E96-4d84-8905-566988322B37}" ;
    		
    		// This'll contain the VSNetToolHostShim on output.
		    Object RefObj = null ;
    		
		    // Create the main tool window by loading the host shim.
		    TheToolWindow = ApplicationObject.Windows.
		        CreateToolWindow ( AddInInstance                       ,
   		                           "VSNetToolHostShim.VSNetToolWinShim",
			                       "Scratch Pad Window"                ,
			                       guid                                ,
			                       ref RefObj                         );
    			                    
            // Make the window visible.  You must do this before calling
            // the HostUserControl method or things won't get hooked
            // up right.
            TheToolWindow.Visible = true ;

            // Get the shim.  (This is a class level variable):
            // private VSNetToolHostShimLib.IVSNetToolWinShim ShimObj ;
            ShimObj = (VSNetToolHostShimLib.VSNetToolWinShimClass)
                       RefObj ;
            
            // Get this assembly so I can pass the location to the shim.
            System.Reflection.Assembly CurrAsm =
                   System.Reflection.Assembly.GetExecutingAssembly ( ) ;
            
            // Get the directory to this Add-In and append the name of
            // the resources DLL to the path so I can load the tab
            // button.
            StringBuilder StrSatDll = new StringBuilder ( ) ;
            
            String StrTemp = CurrAsm.Location.ToLower ( ) ;
            int iPos = StrTemp.IndexOf ( "simpletoolwindow.dll" ) ;
            StrSatDll.Append ( CurrAsm.Location.Substring ( 0 , iPos ));
            StrSatDll.Append ( "SimpleToolWindowResources.DLL" ) ;

            // Load the managed control into the ActiveX control and
            // have it load the bitmap.
            ShimObj.HostUserControl2 ( TheToolWindow                 ,
                                       CurrAsm.Location              ,
                                "SimpleToolWindow.ScratchPadControl" ,
                                       StrSatDll.ToString ( )        ,
                                       1                              );
        }
        catch ( System.Exception eEx )
        {
            MessageBox.Show ( eEx.Message + "\r\n" +
                                 eEx.StackTrace.ToString ( )  ,
                              "ExceptBion in OnConnection"     ) ;
        }
	}

	/// <summary>
	///     Implements the OnDisconnection method of the IDTExtensibility2 interface.
	///     Receives notification that the Add-in is being unloaded.
	/// </summary>
	/// <param term='disconnectMode'>
	///      Describes how the Add-in is being unloaded.
	/// </param>
	/// <param term='custom'>
	///      Array of parameters that are host application specific.
	/// </param>
	/// <seealso class='IDTExtensibility2' />
	public void OnDisconnection(Extensibility.ext_DisconnectMode disconnectMode, ref System.Array custom)
	{
	    if ( null != TheToolWindow )
	    {
	        TheToolWindow.Visible = false ;
	    }
	}

	/// <summary>
	///      Implements the OnAddInsUpdate method of the IDTExtensibility2 interface.
	///      Receives notification that the collection of Add-ins has changed.
	/// </summary>
	/// <param term='custom'>
	///      Array of parameters that are host application specific.
	/// </param>
	/// <seealso class='IDTExtensibility2' />
	public void OnAddInsUpdate(ref System.Array custom)
	{
	}

	/// <summary>
	///      Implements the OnStartupComplete method of the IDTExtensibility2 interface.
	///      Receives notification that the host application has completed loading.
	/// </summary>
	/// <param term='custom'>
	///      Array of parameters that are host application specific.
	/// </param>
	/// <seealso class='IDTExtensibility2' />
	public void OnStartupComplete(ref System.Array custom)
	{
	}

	/// <summary>
	///      Implements the OnBeginShutdown method of the IDTExtensibility2 interface.
	///      Receives notification that the host application is being unloaded.
	/// </summary>
	/// <param term='custom'>
	///      Array of parameters that are host application specific.
	/// </param>
	/// <seealso class='IDTExtensibility2' />
	public void OnBeginShutdown(ref System.Array custom)
	{
	}
	
	private _DTE ApplicationObject ;
	private AddIn AddInInstance ;
	private Window TheToolWindow ;
	private VSNetToolHostShimLib.IVSNetToolWinShim ShimObj ;
	
}
}