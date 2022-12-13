namespace SuperSaver
{
using System ;
using System.Collections ;
using Microsoft.Office.Core ;
using Extensibility ;
using System.Runtime.InteropServices ;
using EnvDTE ;
using System.Windows.Forms ;
using System.Diagnostics ;

/// <summary>
///   The object for implementing an Add-in.
/// </summary>
/// <seealso class='IDTExtensibility2' />
[GuidAttribute ( "6249C0C5-CA0D-40D7-861F-DBD717FEDCF5" ) ,
                  ProgId("SuperSaver"                    )  ]
public class Connect : Object , IDTExtensibility2 , IDTCommandTarget
{
    private _DTE  m_ApplicationObject = null ;
    private AddIn m_AddInInstance = null ;
    private Command m_FileSaveCmd = null ;
    
    /// <summary>
    /// Ye ol' standard constructor.
    /// </summary>
    public Connect ( )
    {
    }

    /// <summary>
    /// Implements the OnConnection method of the IDTExtensibility2
    /// interface. Receives notification that the Add-in is being
    /// loaded.
    /// </summary>
    /// <param term='application'>
    /// Root object of the host application.
    /// </param>
    /// <param term='ConnectMode'>
    /// Describes how the Add-in is being loaded.
    /// </param>
    /// <param term='AddInInst'>
    /// Object representing this Add-in.
    /// </param>
    /// <param name="custom">
    /// Custom data.
    /// </param>
    /// <seealso class='IDTExtensibility2' />
    public void OnConnection ( object                        application,
                               Extensibility.ext_ConnectMode ConnectMode,
                               object                        AddInInst  ,
                               ref System.Array              custom     )
    {
        m_ApplicationObject = (_DTE)application;
        m_AddInInstance = (AddIn)AddInInst;

        // I've got to get this as early as possible as the query
        // request can come in while I'm in the middle of adding
        // commands.
        if ( null == m_FileSaveCmd )
        {
            // Get the file save command I'll need in the Query
            // method.
            Commands Cmds = m_ApplicationObject.Commands ;
            m_FileSaveCmd = Cmds.Item ( "File.SaveSelectedItems" ,
                                        -1                        );
            Debug.Assert ( null != m_FileSaveCmd   ,
                           "null != m_FileSaveCmd"  ) ;
                               
        }
        
        if ( ConnectMode == ext_ConnectMode.ext_cm_UISetup )
        {
            TrimAndSave.Initialize ( m_ApplicationObject ,
                                     m_AddInInstance      ) ;

            // Do the one time command and button setup.
            TrimAndSave.DoInitialCommandSetup ( ) ;
        }
        if ( ( ext_ConnectMode.ext_cm_AfterStartup == ConnectMode ) ||
             ( ext_ConnectMode.ext_cm_Startup      == ConnectMode )   )
        {
            if ( null == TrimAndSave.ApplicationObject )
            {
                TrimAndSave.Initialize ( m_ApplicationObject ,
                                         m_AddInInstance      ) ;
            }
            if ( ext_ConnectMode.ext_cm_Startup == ConnectMode )
            {
                TrimAndSave.StartUp ( ) ;
            }
        }
    }

    /// <summary>
    /// Implements the OnDisconnection method of the
    /// IDTExtensibility2 interface.  Receives notification that
    /// the Add-in is being unloaded.
    /// </summary>
    /// <param term='DisConnectMode'>
    /// Describes how the Add-in is being unloaded.
    /// </param>
    /// <param term='CustomData'>
    /// Array of parameters that are host application specific.
    /// </param>
    /// <seealso class='IDTExtensibility2' />
    public void OnDisconnection ( ext_DisconnectMode DisConnectMode ,
                                  ref System.Array   CustomData      )
    {
        TrimAndSave.ShutDown ( ) ;
    }

    /// <summary>
    /// Implements the OnAddInsUpdate method of the
    /// IDTExtensibility2 interface.  Receives notification that the
    /// collection of Add-ins has changed.
    /// </summary>
    /// <param term='CustomData'>
    /// Array of parameters that are host application specific.
    /// </param>
    /// <seealso class='IDTExtensibility2' />
    public void OnAddInsUpdate ( ref System.Array CustomData )
    {
    }

    /// <summary>
    /// Implements the OnStartupComplete method of the
    /// IDTExtensibility2 interface.  Receives notification that the
    /// host application has completed loading.
    /// </summary>
    /// <param term='CustomData'>
    /// Array of parameters that are host application specific.
    /// </param>
    /// <seealso class='IDTExtensibility2' />
    public void OnStartupComplete ( ref System.Array CustomData )
    {
    }

    /// <summary>
    /// Implements the OnBeginShutdown method of the
    /// IDTExtensibility2 interface.  Receives notification that the
    /// host application is being unloaded.
    /// </summary>
    /// <param term='CustomData'>
    /// Array of parameters that are host application specific.
    /// </param>
    /// <seealso class='IDTExtensibility2' />
    public void OnBeginShutdown ( ref System.Array CustomData )
    {
        TrimAndSave.ShutDown ( ) ;
    }

    /// <summary>
    /// Implements the QueryStatus method of the IDTCommandTarget
    /// interface.  This is called when the command's
    /// availability is updated
    /// </summary>
    /// <param term='CommandName'>
    ///	The name of the command to determine state for.
    /// </param>
    /// <param term='NeededText'>
    ///	Text that is needed for the command.
    /// </param>
    /// <param term='Status'>
    /// The state of the command in the user interface.
    /// </param>
    /// <param term='CommandText'>
    /// Text requested by the NeededText parameter.
    /// </param>
    /// <seealso class='Exec' />
    public void QueryStatus (
                      string                     CommandName      ,
                      EnvDTE.vsCommandStatusTextWanted NeededText ,
                      ref EnvDTE.vsCommandStatus Status           ,
                      ref object                 CommandText       )
    {
        if ( EnvDTE.vsCommandStatusTextWanted.
             vsCommandStatusTextWantedNone == NeededText )
        {
            // The SaveAll command is availible all the time.
            if (ResConstant.SuperSaverSaveAllFullCommand==CommandName)
            {
                Status = (vsCommandStatus)
                    vsCommandStatus.vsCommandStatusSupported |
                    vsCommandStatus.vsCommandStatusEnabled ;
            }
            else if ( ResConstant.SuperSaverSaveFullCommand ==
                                                        CommandName )
            {
                // I wrestled all over the place to figure out if it
                // was safe to save the current file.  In the end, I
                // found that it was best if I simply asked if the
                // default save command is availible.  If it is,
                // then my save command will be.  Cheezy, but
                // effective.
                if ( true == m_FileSaveCmd.IsAvailable )
                {
                    Status = (vsCommandStatus)
                        vsCommandStatus.vsCommandStatusSupported |
                        vsCommandStatus.vsCommandStatusEnabled ;
                }
                else
                {
                    Status = vsCommandStatus.vsCommandStatusUnsupported;
                }
            }
        }
    }
    
    /// <summary>
    /// Implements the Exec method of the IDTCommandTarget
    /// interface.  This is called when the command is invoked.
    /// </summary>
    /// <param term='CommandName'>
    ///	The name of the command to execute.
    /// </param>
    /// <param term='ExecuteOption'>
    /// Describes how the command should be run.
    /// </param>
    /// <param term='VarIn'>
    /// Parameters passed from the caller to the command handler.
    /// </param>
    /// <param term='VarOut'>
    ///	Parameters passed from the command handler to the caller.
    /// </param>
    /// <param term='Handled'>
    ///	Informs the caller if the command was handled or not.
    /// </param>
    /// <seealso class='Exec' />
    public void Exec ( string                     CommandName   ,
                       EnvDTE.vsCommandExecOption ExecuteOption ,
                       ref object                 VarIn         ,
                       ref object                 VarOut        ,
                       ref bool                   Handled        )
    {
        // Since the IDE does such a fine job of eating all exceptions,
        // and never letting you know they happened, I need to ensure
        // I watch for them myself.
        try
        {
            if ( EnvDTE.vsCommandExecOption.
                vsCommandExecOptionDoDefault == ExecuteOption )
            {
                if ( ResConstant.SuperSaverSaveAllFullCommand ==
                                                            CommandName)
                {
                    TrimAndSave.SaveAll ( ) ;
                    Handled = true ;
                }
                else if ( ResConstant.SuperSaverSaveFullCommand ==
                                                            CommandName)
                {
                    TrimAndSave.SaveCurrent ( ) ;
                    Handled = true ;
                }
                else
                {
                    Handled = false ;
                }
            }
            else
            {
                Handled = false ;
            }
        }
        catch ( System.Exception eEx )
        {
            MessageBox.Show ( eEx.Message + "\r\n" +
                                eEx.StackTrace.ToString ( ) ,
                              "Exception in Exec"            ) ;
        }
    }
}   // End of Connect class
}   // End of SuperSaver namespace.