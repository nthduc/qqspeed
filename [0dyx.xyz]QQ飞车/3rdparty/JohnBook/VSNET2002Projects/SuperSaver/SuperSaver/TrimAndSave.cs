using System ;
using EnvDTE ;
using Microsoft.Office.Core ;
using Extensibility ;
using System.Runtime.InteropServices ;
using System.Windows.Forms ;
using System.Timers ;
using System.Threading ;
using System.Diagnostics ;
using Microsoft.Win32 ;

namespace SuperSaver
{

/// <summary>
/// The class that does the actual work for SuperSaver.
/// </summary>
public class TrimAndSave
{
    /// <summary>
    /// The application object.  This has to be set in the Connect
    /// class before you use any methods in this class.
    /// </summary>
    private static _DTE m_ApplicationObject = null ;
    
    static public _DTE ApplicationObject
    {
        get
        {
            return ( m_ApplicationObject ) ;
        }
    }
    
    private static AddIn m_AddInInstance = null ;
    
    public static void Initialize ( _DTE AppObj , AddIn AddInst )
    {
        m_ApplicationObject = AppObj ;
        m_AddInInstance = AddInst ;
    }

    static bool m_ReadRegistry = false ;
	
    /// <summary>
    /// The interval for autosave to kick in.
    /// </summary>
    private static int m_AutoSaveInterval = 10 ;
    
    public static int AutoSaveInterval
    {
        get
        {
            if ( false == m_ReadRegistry )
            {
                ReadRegistryValues ( ) ;
            }
            return ( m_AutoSaveInterval ) ;
        }
        set
        {
            m_AutoSaveInterval = value ;
            // Don't forget to update the time interval!!
            SetTimerInterval ( ) ;
        }
    }
	
    /// <summary>
    /// Determines if AutoSave trims whitespace.
    /// </summary>
    private static bool m_TrimWhiteSpace = true ;
    
    public static bool TrimWhiteSpace
    {
        get
        {
            if ( false == m_ReadRegistry )
            {
                ReadRegistryValues ( ) ;
            }
            return ( m_TrimWhiteSpace ) ;
        }
        set
        {
            m_TrimWhiteSpace = value ;
        }
    }
	
    /// <summary>
    /// The autosave timer.
    /// </summary>
    private static System.Timers.Timer m_AutoSaveTimer =
                                          new System.Timers.Timer ( ) ;
                                        
    /// <summary>
    /// If greater than zero, I'm in the middle of a save so I will not
    /// do another re-entrant save.
    /// </summary>
    private static int m_iIsSaving = 0 ;

    /// <summary>
    /// The boring constructor.
    /// </summary>
    public TrimAndSave ( )
    {
    }
	
    /// <summary>
    /// Called when the AddIn starts.  Cranks up the timer.
    /// </summary>
    public static void StartUp ( )
    {
        // Read the registry values to get life going.
        ReadRegistryValues ( ) ;
    
        // Crank up the timer.
        m_AutoSaveTimer.Elapsed += new ElapsedEventHandler ( OnTimer ) ;

        m_AutoSaveTimer.AutoReset = true ;
        
        // Set the time.
        SetTimerInterval ( ) ;
    }
	
    /// <summary>
    /// Called when the AddIn us unloading.  Stops the timer.
    /// </summary>
    public static void ShutDown ( )
    {
        // Stop the timer.
        m_AutoSaveTimer.Stop ( ) ;
        
        // Save the current values to the registry.
        WriteRegistryValues ( ) ;
    }
    
    public static void WorkAroundLackOfShutdown ( )
    {
        WriteRegistryValues ( ) ;
    }
    
    private static void SetTimerInterval ( )
    {
        if ( 0 == m_AutoSaveInterval )
        {
            m_AutoSaveTimer.Enabled = false ;
        }
        else
        {
            m_AutoSaveTimer.Interval =
                        ( 1000 * 60 ) * m_AutoSaveInterval ;
            m_AutoSaveTimer.Enabled = true ;
        }
    }

    /// <summary>
    /// Takes care of saving the files on the background thread.
    /// </summary>
    /// <param name="source">
    /// Usual event param.
    /// </param>
    /// <param name="e">
    /// Usual event param.
    /// </param>
    private static void OnTimer ( Object source , ElapsedEventArgs e )
    {
        // Save 'em.
        if ( true == m_TrimWhiteSpace )
        {
            // Do my trim save.
            SaveAll ( ) ;
        }
        else
        {
            // Do the normal saveall.
            m_ApplicationObject.ExecuteCommand ( "File.SaveAll" ,
                                                  ""              ) ;
        }
    }
    
    public static void SaveAll ( )
    {
        // My original idea for accomplishing the proper stripping and
        // saving was to:
        // 1.  Run through the list of open documents.
        // 2.  Determine which documents had text (i.e., code) in them
        // 3.  For those that are text, use the
        //     TextDocument.ReplacePatten to do the actual replace.
        // 4.  Save any changed documents.
        // 5.  Loop through all open projects and save any that needed
        //     saving.
        // 6.  Finally, save the solution.
        //
        // Alas, the best laid plans of mice and men oft go aglay.
        //
        // 1.  The TextDocument.ReplacePattern method has a whopper
        //     bug where it will not propertly do sub expression
        //     replacement.
        //     My search expression : "{[^ \t]+}{[ \t]#$}" and
        //     replacement "\1" would cause all lines ending with
        //     extra white space to be replaced with the string "\1"
        //     which is not a good thing to have happen in source
        //     code.
        // 2.  After much gnashing of teeth, I stumbled onto the
        //     DTE.Find object.  Interestingly, the documentation for
        //     DTE.Find tells you you'd be much better off with
        //     using TextDocument.ReplacePattern (we know better than
        //     that!).  The good news is that DTE.Find does work.
        //     However, since Find either works on all solution
        //     documents or just the current active one, that's a small
        //     problem as documents can be changed and not have an
        //     active window (ala my C# doc comment macro).
        //     I had to choose all solution documents, instead of
        //     looping through them.
        // 3.  Finally, I'd also found the project object system in
        //     VS.NET to be quite a mess.  Some projects, like Setup
        //     projects, are undocumented and don't support any form
        //     of Save or SaveAs method.  Also, with the conflicting
        //     project objects, the undocumented ones, and future ones
        //     I haven't seen, there was no clean way to loop through
        //     and be able to save projects.
        //
        // In the end, I am using the DTE.Find property to replace all
        // extranious spaces on all the solution's text files every
        // time I do a SuperSave.SuperSaverSaveAll.  While I wanted to
        // be a little more efficient, this was the best I could do.
        // At least now you'll have the extranious spaces stripped (as
        // god intended files to be saved!!) and not loose any auto
        // indenting spaces applied by smart indenting.
        // In order to ensure everything got saved, which you can't
        // guarentee programatically, I simply execute the real
        // File.SaveAll command.
        
        // If I'm already saving, just skip out.
        if ( m_iIsSaving > 0 )
        {
            return ;
        }
        
        // Increment my flag so I don't re-enter the save code.
        System.Threading.Interlocked.Increment ( ref m_iIsSaving ) ;
        
        try
        {
            // If there are any read only documents open, I can't
            // do the global search and replace as that can change
            // read only documents.
            if ( false == ReadOnlyDocsOpen ( ) )
            {
                SafeFindObject SafeFind =
                    new SafeFindObject ( m_ApplicationObject ) ;
                                    
                // Do it to all docs.
                SafeFind.SetupFind ( vsFindTarget.vsFindTargetSolution);
                           
                // Let it rip.
                try
                {
                    SafeFind.Execute ( ) ;
                }
                finally
                {
                    // No matter what, get the user's settings back in
                    // place.
                    SafeFind.RestoreFind ( ) ;
                }
            }
            // Force all files to be saved.  By the way, if you pass
            // null as the command args (param 2), you'll get an
            // exception, so always pass an empty string as the
            // parameters.
            m_ApplicationObject.ExecuteCommand  ( "File.SaveAll" , "" ) ;
        }
        finally
        {
            // Decrement the block flag as I am done.
            System.Threading.Interlocked.Decrement ( ref m_iIsSaving );
        }
    }
    
    public static void SaveCurrent ( )
    {
        // This is not how I want this command to work.  However, given
        // the messy state of life when it comes to:
        // 1. The problems with TextDocument.ReplacePattern (as
        //    described above).
        // 2. The problems that some open items might not implement a
        //    documented save.
        // There's no clean way I can see to do the command like I
        // would want.
        //
        // Get the DTE.SelectedItems collection (which contains the
        //  selected items in the solution)
        // foreach item in the selected items
        //    if it's a document and not saved
        //      get the text document
        //      call ReplacePattern
        //    call Save on the item
        
        // Reality dictates the best I can do is look to see if the
        // active document is a text file.  If it is, then I can use
        // DTE.Find as that only works on documents visible in the
        // IDE.
        if ( m_iIsSaving > 0 )
        {
            return ;
        }
        
        // Increment my flag so I don't re-enter the save code.
        System.Threading.Interlocked.Increment ( ref m_iIsSaving ) ;
        
        try
        {
            Document ActDoc = m_ApplicationObject.ActiveDocument ;
            TextDocument TxtDoc = null ;
            
            if  ( null != ActDoc )
            {
                TxtDoc = (TextDocument)
                                    ActDoc.Object ( "TextDocument" ) ;
            }
            
            if ( null != TxtDoc )
            {
                SafeFindObject SafeFind =
                            new SafeFindObject ( m_ApplicationObject ) ;
                                
                // Set to search only the current document.
                SafeFind.
                    SetupFind(vsFindTarget.vsFindTargetCurrentDocument);
                       
                // Let it rip.
                try
                {
                    SafeFind.Execute ( ) ;
                }
                finally
                {
                    // No matter what, get the user's settings back in
                    // place.
                    SafeFind.RestoreFind ( ) ;
                }
            }

            // Do the regular command.  It's the best I can do....
            m_ApplicationObject.
                    ExecuteCommand  ( "File.SaveSelectedItems" , "" ) ;
        }
        finally
        {
            // Decrement the block flag as I am done.
            System.Threading.Interlocked.Decrement ( ref m_iIsSaving );
        }
    }

    // A helper function to return TRUE if any read only documents
    // are open.
    private static Boolean ReadOnlyDocsOpen ( )
    {
        Boolean Ret = false ;
        foreach ( Document CurrDoc in m_ApplicationObject.Documents )
        {
            if ( true == CurrDoc.ReadOnly )
            {
                Ret = true ;
                break ;
            }
        }
        return ( Ret ) ;
    }

    
        
    #region Command and Toolbar Setup
    
    static public void DoInitialCommandSetup ( )
    {
        try
        {
            // Get the commands collection.
            Commands Cmds = m_ApplicationObject.Commands ;
            // Get the command bars collection.
            _CommandBars CommandBars = m_ApplicationObject.CommandBars;

            // To make my development life easier, I'm going to assume
            // that the commands might already have been added to this
            // instance of VS.  Therefore, before I add them, I'll
            // attempt to delete each one.  This way, I it will be much
            // easier to continue to add commands from this AddIn as I
            // develop it.
            RemoveCommandsAndCommandBars ( ) ;

            // Create the command bar.
            CommandBar SuperSaverCmdBar = (CommandBar)
                Cmds.AddCommandBar ( ResConstant.CommandBarName      ,
                            vsCommandBarType.vsCommandBarTypeToolbar ,
                            null                                     ,
                            1                                        );

            // Allocate an empty context GUID array.
            object []ContextGUIDS = new object[] { } ;

            // Now I can add my commands.  Safely.  If anything
            // throws here, it's something really serious!

            int DisFlags =
                (int)vsCommandStatus.vsCommandStatusSupported +
                (int)vsCommandStatus.vsCommandStatusEnabled ;

            Command Cmd = null ;

            // Add the SuperSaverSave command.
            Cmd = Cmds.AddNamedCommand (
                            m_AddInInstance                         ,
                            ResConstant.SuperSaverSave              ,
                            ResConstant.SuperSaverSaveButtonText    ,
                            ResConstant.SuperSaverSaveToolTip       ,
                            false                                   ,
                            ResConstant.SuperSaverSaveBitmapID      ,
                            ref ContextGUIDS                        ,
                            DisFlags                                 );

            Cmd.AddControl ( SuperSaverCmdBar , 1 ) ;

            // Add the SuperSaverSaveAll command.
            Cmd = Cmds.AddNamedCommand (
                            m_AddInInstance                         ,
                            ResConstant.SuperSaverSaveAll           ,
                            ResConstant.SuperSaverSaveAllButtonText ,
                            ResConstant.SuperSaverSaveAllToolTip    ,
                            false                                   ,
                            ResConstant.SuperSaverSaveAllBitmapID   ,
                            ref ContextGUIDS                        ,
                            DisFlags                                 );

            Cmd.AddControl ( SuperSaverCmdBar , 2 ) ;

            // VS.NET adds the buttons in the dumbest style, icon
            // and text.  Here I'll run through and fix their mistake
            // to force the buttons to be icons.  Why that's not the
            // default is beyond me.
            foreach ( CommandBarControl ctl in
                                            SuperSaverCmdBar.Controls )
            {
                if ( ctl is CommandBarButton )
                {
                    CommandBarButton btn = (CommandBarButton)ctl ;
                    btn.Style = MsoButtonStyle.msoButtonIcon ;
                }
            }
            
            // The default location for new toolbars \1
            // msoBarFloating, which defaults to 0,0 on the screen.
            // This will at least get it mounted in the docking area.
            SuperSaverCmdBar.Position = MsoBarPosition.msoBarTop ;
        }
        catch ( System.Exception eEx )
        {
            MessageBox.Show ( eEx.Message + "\r\n" +
                              eEx.StackTrace.ToString ( )        ,
                              "ExceptBion in DoInitialCommandSetup"  );
        }
    }
    
    static private void RemoveCommandsAndCommandBars ( )
    {
        // Get the commands collection.
        Commands Cmds = m_ApplicationObject.Commands ;
        
        try
        {
            // Sometimes VB is better.  I had the C# funnel turned on
            // and the docs didn't say anything about passing -1 as
            // the ID param.  Sheez.  I went nuts trying to figure out
            // how to get a command without looping through the entire
            // complete list!
            Command Cmd =
                Cmds.Item ( ResConstant.SuperSaverSaveAllFullCommand ,
                            -1                                       );
                            
            if ( null != Cmd )
            {
                Cmd.Delete ( ) ;
            }
        }
        catch
        {
        }
        
        try
        {
            Command Cmd =
                Cmds.Item ( ResConstant.SuperSaverSaveFullCommand ,
                -1                                                 );
                            
            if ( null != Cmd )
            {
                Cmd.Delete ( ) ;
            }
        }
        catch
        {
        }
        try
        {
            // Kill my command bar
            _CommandBars CommandBars = m_ApplicationObject.CommandBars;
            CommandBar CmdBar =
                (CommandBar)CommandBars[ResConstant.CommandBarName] ;

            Cmds.RemoveCommandBar ( CmdBar ) ;
        }
        catch
        {
        }
    }
    
    #endregion
    
    /// <summary>
    /// The registry key where I store options.
    /// </summary>
    private const String SuperSaverOptionsRegKey =
                        "\\AddIns\\SuperSaver\\Settings" ;
    
    /// <summary>
    /// The persistant autosave time registry value.
    /// </summary>
    private const String AutoSaveTimeInterval = "AutoSaveInterval" ;

    /// <summary>
    /// The persistant trim space registry value.
    /// </summary>
    private const String TrimSpaceOnAutoSave = "TrimSpaceOnAutoSave" ;

    /// <summary>
    /// Reads all registry values and initialized the class variables.
    /// </summary>
    private static void ReadRegistryValues ( )
    {
        try
        {
            String RegVal = m_ApplicationObject.RegistryRoot +
                              SuperSaverOptionsRegKey ;
                                
            // Get the autosave time from the registry.
            RegistryKey Key =
                          Registry.CurrentUser.CreateSubKey ( RegVal ) ;
                        
            m_AutoSaveInterval =
                    (int)Key.GetValue ( AutoSaveTimeInterval , 10 ) ;
            
            int Val = (int)Key.GetValue ( TrimSpaceOnAutoSave , 1 );
            
            if ( Val == 1 )
            {
                m_TrimWhiteSpace = true ;
            }
            else
            {
                m_TrimWhiteSpace = false ;
            }
            
            m_ReadRegistry = true ;
        }
        catch ( System.Exception eEx )
        {
            MessageBox.Show ( eEx.Message + "\r\n" +
                              eEx.StackTrace.ToString ( ) +
                              " Type : " + eEx.ToString ( ) ,
                              "Exception in ReadRegistryValues" ) ;
        
        }
    }
    
    /// <summary>
    /// Saves the class variables to the registry.
    /// </summary>
    private static void WriteRegistryValues ( )
    {
        try
        {
            String RegVal = m_ApplicationObject.RegistryRoot +
                                SuperSaverOptionsRegKey ;
        
            RegistryKey Key =
                          Registry.CurrentUser.CreateSubKey ( RegVal ) ;
                        
            Key.SetValue ( AutoSaveTimeInterval , m_AutoSaveInterval ) ;
            
            int Val = 0 ;
            if ( true == m_TrimWhiteSpace )
            {
                Val = 1 ;
            }
            Key.SetValue ( TrimSpaceOnAutoSave , Val ) ;
        }
        catch ( System.Exception )
        {
        }
    }
}   // End of TrimAndSave class.
}   // End of SuperSaver namespace.
