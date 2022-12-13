/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997 - 2003 John Robbins
----------------------------------------------------------------------*/
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
using System.Text ;
using Microsoft.VisualStudio.VCProject ;
using Microsoft.VisualStudio.VCProjectEngine ;
using System.IO ;

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
    
    /// <summary>
    /// The interval, in minutes, for autosaving to kick in.
    /// </summary>
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
    
    /// <summary>
    /// Determines if the background SuperSaver autosave trims
    /// whitespace.
    /// </summary>
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
    /// Determines if background saves will do read only and virgin
    /// files.
    /// </summary>
    private static bool m_SaveNewAndReadOnly = false ;
    
    public static bool SaveNewAndReadOnly
    {
        get
        {
            if ( false == m_ReadRegistry )
            {
                ReadRegistryValues ( ) ;
            }
            return ( m_SaveNewAndReadOnly ) ;
        }
        set
        {
            m_SaveNewAndReadOnly = value ;
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
    
    /// <summary>
    /// Called by the Options dialog to force settings to be written
    /// to the registry.
    /// </summary>
    public static void WorkAroundLackOfShutdown ( )
    {
        WriteRegistryValues ( ) ;
    }
    
    // Sets the background timer interval.
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
        // Do my trim save.
        SaveAll ( SaveNewAndReadOnly ) ;
        
/*
        // If I'm already saving, just skip out.
        if ( m_iIsSaving > 0 )
        {
            return ;
        }

        // This is ugly.  However, since I'm forced to bring each text
        // window to the foreground in order to strip whitespace off the
        // end of lines, there's a slim chance you could be in the
        // middle of typing and accidentally type into the wrong window.
        // By boosting this thread, I can avoid problems.
        ThreadPriority OldPriority = ThreadPriority.Normal ;
        try
        {
            if ( true == m_TrimWhiteSpace )
            {
                OldPriority =
                        System.Threading.Thread.CurrentThread.Priority ;
                System.Threading.Thread.CurrentThread.Priority =
                                                ThreadPriority.Highest ;
            }
            SaveAll ( ! SaveNewAndReadOnly ) ;
        }
        finally
        {
            if ( true == m_TrimWhiteSpace )
            {
                System.Threading.Thread.CurrentThread.Priority =
                                                           OldPriority ;
            }
        }
*/
    }
    
   
    //public static void SaveAll ( bool SkipVirginAndReadOnly )
    public static void SaveAll ( bool SaveReadOnlyAndVirgin )
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
        // 4.  In one attempt, I used the DTE.Find property to replace
        //     all extranious spaces on all the solution's text files
        //     every time I did a SuperSave.SuperSaverSaveAll.  While I
        //     wanted to be a little more efficient, this was the best
        //     I could think to do.   At you'd have the extranious
        //     spaces stripped (as god intended files to be saved!!) and
        //     not lose any auto indenting spaces applied by smart
        //     indenting.  In order to ensure everything got saved,
        //     which you can't guarentee programatically, I simply
        //     executed the real File.SaveAll command.
        // 5.  While Step 4 was a good approach and did work, it had a
        //     few bumps.  The first was that it also caught "virgin
        //     files" (those created with File | New File), which a real
        //     background autosave should not touch.  A background
        //     autosave should only save files that have already been
        //     saved.  (See how Visual SlickEdit does a background save
        //     to see how it's done).
        //     As there was no way for me to interrupt File.SaveAll,
        //     what you'd see eventually was a slew of File Save As
        //     dialogs popped up.  After about 20, things would get
        //     unstable with the AddIn as there were 20+ threads
        //     blocked.
        // 6.  To work around the problems of step 4, I went back to
        //     grinding through each file and special casing the
        //     "virgin files" on a background save.  My idea was to
        //     use the DTE.Find object with the
        //     vsFindTarget.vsFindTargetCurrentDocument so I could do
        //     the whitespace stripping.  That meant I needed to force
        //     each text window to the foreground so the DTE.Find object
        //     would work on it.  That seemed like a reasonable idea
        //     until I found two major problems:
        //     A.  For a WinForm code document, activating the document
        //         activated only the *DESIGN* mode.  There's no way to
        //         activate the *CODE* mode.  I consider this a bug in
        //         the architecture.  Consequently, to find the actual
        //         active window, I grovelled through the Window menu
        //         as there's no way to properly get the real active
        //         window.
        //     B.  The DTE.SuppressUI does not work like you'd expect.
        //         I was assuming that it would actually stop the UI
        //         updating as the name implies.  Since I was moving
        //         windows around to force the DTE.Find object to
        //         strip spaces, I didn't want the UI to jump around.
        //         However, setting DTE.SuppressUI does not seem to
        //         have any affect and the windows still do the ugly
        //         dance.
        // 7.  Alas, even after all of that pain and struggle, it STILL
        //     doesn't work.
        //     A.  Activating a text window, causes a couple of really
        //         odd problems.
        //         a.  When VS.NET is minimized, it becomes
        //             un-minimized.
        //         b.  It causes a heck of a lot of taskbar flashing.
        //     B.  By calling the Find object on another thread, that
        //         exposes a bug where the Find object starts popping
        //         the "text not found"/"x items replaced" dialog when
        //         the SaveCurrent function runs!  If you have the
        //         dialog turned off, you'll hear a beep every time you
        //         use SaveCurrent.
        // 8.  I've had to fall back to a somewhat broken background
        //     autosave.  If any read only files or "virgin files" are
        //     open, I can't call File.SaveAll.  Bleech.
        

        // If I'm already saving, just skip out.
        if ( m_iIsSaving > 0 )
        {
            return ;
        }
        
        // Increment my flag so I don't re-enter the save code.
        System.Threading.Interlocked.Increment ( ref m_iIsSaving ) ;
        
        try
        {
            bool ReadOnly = false ;
            bool Virgin   = false ;
            bool ReadOnlyNeedSave = false ;
            bool DirtyDocs = false ;
            
            DetermineDocROVSStates ( ref ReadOnly         ,
                                     ref Virgin           ,
                                     ref ReadOnlyNeedSave ,
                                     ref DirtyDocs         ) ;
            
            if ( ( false == ReadOnly          ) &&
                 ( false == Virgin            ) &&
                 ( true == m_TrimWhiteSpace   )    )
            {
                SafeFindObject SafeFind =
                    new SafeFindObject ( m_ApplicationObject ,
                                vsFindTarget.vsFindTargetOpenDocuments);
                                    
                // Let it rip.
                SafeFind.Execute ( ) ;
            }
            // Only bother if there are files that need saving.
            if ( true == DirtyDocs )
            {
                bool DoSave = true ;
                
                // If the user does not want background saves and there
                // are read only and virgin files open, I can't save.
                if ( ( false == SaveReadOnlyAndVirgin  ) &&
                     ( ( true == ReadOnlyNeedSave ) ||
                       ( true == Virgin           )   )     )
                {
                    DoSave = false ;
                }
                if ( true == DoSave )
                {
                    // Force all files to be saved.  By the way, if you
                    // pass null as the command args (param 2), you'll
                    // get an exception, so always pass an empty string
                    // as the parameters.
                    m_ApplicationObject.ExecuteCommand  ( "File.SaveAll" ,
                                                          ""              );
                }
            }
        }
        finally
        {
            // Decrement the block flag as I am done.
            System.Threading.Interlocked.Decrement ( ref m_iIsSaving );
        }
        
        #region Original Attempt (does not work due to bugs in VS.NET)
/*
        // If a solution isn't open, there's not much to do.
        if ( null == ApplicationObject.Solution )
        {
            return ;
        }
        
        // If I'm already saving, just skip out.
        if ( m_iIsSaving > 0 )
        {
            return ;
        }
        
        // Increment my flag so I don't re-enter the save code.
        System.Threading.Interlocked.Increment ( ref m_iIsSaving ) ;

        try
        {
            // Is this not the biggest and ugliest hack you can imagine?
            // There's no way to get the actual active *DOCUMENT*
            // window.  If a WinForm *CODE* window is currently active,
            // say Foo.cs, the active document is reported as Foo.cs.
            // However, if you call the Activate() method on the
            // document, the *DESIGN* window becomes the active window.
            // I looked all over the automation object model and tried
            // a million things to see how I could ensure the last
            // active editing/design window could be restored, and I
            // couldn't find a way.
            // Keep in mind this bug work around was precipitated by
            // the bug in TextDocument.ReplacePattern.  Nothing like
            // bugs on top of bugs....
            // What I finally figured out is that I can look at the
            // Window menu, to find the active edit area window.  It
            // can't get much uglier than that simply to figure out
            // which window is active in the edit area.
            // Obviously, this is hardcoded to English.
            
            // Suppress the UI as there'll be a lot of window flashing
            // since I'm forced to bring the window to the foreground
            // in order to properly handle the search and replace.
            m_ApplicationObject.SuppressUI = true ;
            
            // The actual window with focus.
            Window ActiveWin = m_ApplicationObject.ActiveWindow  ;
            
            // Holds the active edit area window.
            String ActiveEditCaption = null ;
            CommandBar WinCmdBar =
                           m_ApplicationObject.
                                CommandBars[ResConstant.WindowMenuName];
            foreach ( CommandBarControl Ctrl in WinCmdBar.Controls )
            {
                if ( ResConstant.ActiveWindowMenu ==
                                      Ctrl.Caption.Substring ( 0 ,
                                 ResConstant.ActiveWindowMenu.Length ) )
                {
                    ActiveEditCaption = Ctrl.Caption.Substring ( 3 ) ;
                    break ;
                }
            }
            
            // First, loop through all the open documents.
            foreach ( Document CurrDoc in m_ApplicationObject.Documents)
            {
                // Is this a virgin file?  If I'm not supposed to touch
                // them (only with background auto saves), don't bother.
                bool IsVirginFile =
                           ( "~" == CurrDoc.Name.Substring ( 0 , 1 ) ) ;
                if ( ( true == IsVirginFile          ) &&
                     ( true == SkipVirginAndReadOnly )    )
                {
                    continue ;
                }
                
                // If the file is read only and it's a background save,
                // I'll skip it.
                if ( ( true == IsFileReadOnly ( CurrDoc.FullName )) &&
                     ( true == SkipVirginAndReadOnly              )   )
                {
                    continue ;
                }
                    
                // First look to see if it's a TextDocument.
                TextDocument TxtDoc = (TextDocument)
                                      CurrDoc.Object ( "TextDocument" );
                if ( null != TxtDoc )
                {
                    // If the document is read only, and the
                    // document is dirty, I'll save it.
                    if ( ( false == CurrDoc.ReadOnly ) &&
                         ( false == CurrDoc.Saved    )    )
                    {
                        if ( true == m_TrimWhiteSpace )
                        {
                            // Grrr.  I can't call CurrDoc.Activate()
                            // here as that will immediately activate
                            // the *DESIGN* view, and not the text view.
                            // I have to loop through the windows and
                            // see if I can find the *CODE" view.
                            // Activating the *DESIGN* view will not
                            // work with the workaround for
                            // TextDocument.ReplacePattern.
                            // This is so sad.
                            Window TextWin = null ;
                            foreach ( Window Win in
                                            m_ApplicationObject.Windows)
                            {
                                if ( Win.Caption == CurrDoc.Name )
                                {
                                    TextWin = Win ;
                                    break ;
                                }
                            }
                            if ( null != TextWin )
                            {
                                // Make this doc the active document so
                                // I can use the SafeFindObject on it.
                                // If ReplacePattern wasn't buggy, I'd
                                // do it here.
                                TextWin.Activate ( ) ;
                                
                                SafeFindObject SafeFind = new
                                    SafeFindObject
                                        ( m_ApplicationObject ,
                                            vsFindTarget.
                                           vsFindTargetCurrentDocument);
                                    
                                // Let it rip.
                                SafeFind.Execute ( ) ;
                            }
                        }
                        if ( ( true  == IsVirginFile        ) &&
                             ( false == SkipVirginAndReadOnly )    )
                        {
                            // Do the Save As Dialog.
                            DoSaveAsForVirgin ( CurrDoc ) ;
                        }
                        else
                        {
                            // If this happens to be a read only
                            // file and the user presses cancel
                            // to the Save dialog, Document.Save
                            // throws an exception.  THIS IS A
                            // PERFECT EXAMPLE OF MISUSING
                            // EXCEPTIONS!
                            try
                            {
                                // Save the document.
                                CurrDoc.Save ( CurrDoc.FullName ) ;
                            }
                            catch ( COMException )
                            {
                            }
                        }
                    }
                }
                else
                {
                    try
                    {
                        // If it needs saving, do so.
                        CurrDoc.Save ( CurrDoc.FullName ) ;
                    }
                    catch ( COMException )
                    {
                    }
                }
            }
            
            // Reset the active edit control.
            foreach ( Window CurrWin in m_ApplicationObject.Windows )
            {
                if ( ActiveEditCaption == CurrWin.Caption )
                {
                    CurrWin.Activate ( ) ;
                    break ;
                }
            }
            
            // Even though I just set the active edit control, the focus
            // could be elsewhere so I need to ensure I set it back
            // there.
            ActiveWin.Activate ( ) ;
            
            
            EnvDTE._Solution Sol = ApplicationObject.Solution ;

            // Loop through all the projects and attempt to save them.
            Projects Projs = Sol.Projects ;
            foreach ( Project CurrProj in Projs )
            {
                // Since the project system is a mess, the best I can do
                // is whip through and simply try to save them.  If
                // the macinations fail, there's not much I can do.
                
                // Some projects don't properly record their state
                // in the Saved property (think Setup projects).
                if ( ( false == CurrProj.Saved   ) ||
                     ( true  == CurrProj.IsDirty )   )
                {
                    // If I'm ignoring virgin files, this function was
                    // called as part of the background saving.  If the
                    // project is read only and I attempt to save it,
                    // the user will be presented a dialog asking if
                    // they want to overwrite, etc.  When it comes to
                    // background saving, I'll skip read only projects
                    // that have been changed.
                    if ( ( false ==
                              IsFileReadOnly ( CurrProj.FullName ) ) ||
                         ( false == SkipVirginAndReadOnly               )   )
                    {
                        // Try to see if it's a VC project.
                        String LangStr = null ;
                        try
                        {
                            LangStr = CurrProj.CodeModel.Language ;
                        }
                        catch
                        {
                            LangStr = null ;
                        }
                        
                        if ( LangStr ==
                            CodeModelLanguageConstants.vsCMLanguageVC )
                        {
                            VCProject VCPrj =(VCProject)CurrProj.Object;
                            VCPrj.Save ( ) ;
                        }
                        else
                        {
                            // I'll try a couple of different saves
                            // here. Most projects support the Save so
                            // that'll take care of most.  If the project
                            // doesn't support the Save and SaveAs
                            // methods, I'll call the IDE function to do
                            // the final save.
                            try
                            {
                                CurrProj.Save ( CurrProj.FullName ) ;
                            }
                            catch ( System.NotImplementedException )
                            {
                                try
                                {
                                    CurrProj.SaveAs (CurrProj.FullName);
                                }
                                catch ( System.NotImplementedException)
                                {
                                }
                            }
                        }
                    }
                }
            }
            
            // I've take care of files and projects, now do the solution
            // itself.

            // If the solution is dirty, save it.
            if ( true == Sol.IsDirty )
            {
                // If it's
                if ( ( false ==  IsFileReadOnly ( Sol.FullName ) ) ||
                     ( false == SkipVirginAndReadOnly            )   )
                {
                    // PSYCH!  There's no direct Save method for the
                    // solution object!
                    // I'll try to save as onto itself.
                    Sol.SaveAs ( Sol.FullName )  ;
                }
            }
        }
        finally
        {
            // Let the UI continue.
            m_ApplicationObject.SuppressUI = false ;
            
            // Decrement the block flag as I am done.
            System.Threading.Interlocked.Decrement ( ref m_iIsSaving );
        }
*/
        #endregion
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
                SafeFindObject SafeFind = new
                            SafeFindObject ( m_ApplicationObject ,
                                             vsFindTarget.
                                           vsFindTargetCurrentDocument);
                                
                SafeFind.Execute ( ) ;
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
    
    
    /// <summary>
    /// Determines open file states.
    /// </summary>
    /// <param name="ReadOnly">
    /// Returns true if there are read only files open.
    /// </param>
    /// <param name="Virgin">
    /// Returns true if there are virgin files open.
    /// </param>
    /// <param name="ReadOnlyNeedSave">
    /// Returns true if there are read only that need saving files open.
    /// </param>
    /// <param name="DirtyDocs">
    /// Returns true if there are unsaved files open.
    /// </param>
    private static void DetermineDocROVSStates ( ref bool ReadOnly         ,
                                                 ref bool Virgin           ,
                                                 ref bool ReadOnlyNeedSave ,
                                                 ref bool DirtyDocs         )
    {
        foreach ( Document CurrDoc in m_ApplicationObject.Documents )
        {
            // While one would think that the ReadOnly property on a
            // document would tell you if the document is read only.
            // WRONG!  It's broken.
            if ( true == IsFileReadOnly ( CurrDoc.FullName ) )
            {
                ReadOnly = true ;
                if ( false == CurrDoc.Saved )
                {
                    ReadOnlyNeedSave = true ;
                }
            }
            else if ( "~" == CurrDoc.Name.Substring ( 0 , 1 ) )
            {
                Virgin = true ;
            }
            if ( false == CurrDoc.Saved )
            {
                DirtyDocs = true ;
            }
        }
    }
    
/*
    // A helper function to return TRUE if any read only or virgin
    // documents are open.
    private static Boolean ReadOnlyOrVirginDocsOpen ( )
    {
        Boolean Ret = false ;
        foreach ( Document CurrDoc in m_ApplicationObject.Documents )
        {
            // While one would think that the ReadOnly property on a
            // document would tell you if the document is read only.
            // WRONG!  It's broken.
            if ( true == IsFileReadOnly( CurrDoc.FullName ) )
            {
                Ret = true ;
                break ;
            }
            else if ( "~" == CurrDoc.Name.Substring ( 0 , 1 ) )
            {
                Ret = true ;
                break ;
            }
        }
        return ( Ret ) ;
    }
*/

    private static bool IsFileReadOnly ( String FileName )
    {
        FileAttributes fa = File.GetAttributes ( FileName ) ;
        return ( FileAttributes.ReadOnly ==
                                    ( fa & FileAttributes.ReadOnly ) ) ;
    }

/*
    /// <summary>
    /// Saves "virgin files" with a Save As Dialog.
    /// </summary>
    /// <remarks>
    /// A "virgin file" is one created by File|New File and has not been
    /// saved yet.
    /// </remarks>
    /// <param name="CurrDoc">
    /// The current document with focus.
    /// </param>
    private static void DoSaveAsForVirgin ( Document CurrDoc )
    {
        // Step one is to get the document caption as that'll form
        // the first part of the filename.
        String DocName = CurrDoc.ActiveWindow.Caption ;
        
        // Next get the extension off the tail of the name.
        Int32 DotPos = CurrDoc.Name.LastIndexOf ( "." ) ;
        String Ext = CurrDoc.Name.Substring ( DotPos ,
                                              CurrDoc.Name.Length -
                                                             DotPos ) ;
                                              
        // Get the solution so I can start the dialog in the solution
        // dir.
        String SolName = m_ApplicationObject.Solution.FileName ;
        Int32 SlashPos = SolName.LastIndexOf ( "\\" ) ;
        String SolDir = SolName.Substring ( 0 , SlashPos ) ;
                                                      

        // Do the file dialog.
        SaveFileDialog Dlg = new SaveFileDialog ( ) ;
        
        // Set up the basic options.
        Dlg.AddExtension = true ;
        Dlg.CheckFileExists = false ;
        Dlg.CheckPathExists = true ;
        Dlg.FileName = DocName + Ext ;
        Dlg.OverwritePrompt = true ;
        Dlg.RestoreDirectory = true ;
        Dlg.ValidateNames = true ;
        Dlg.InitialDirectory = SolDir ;
        
        Dlg.Title = ResConstant.VirginDialogTitle ;
        
        // Build the filter.
        String ExtName = Ext.Substring ( 1 ).ToLower ( ) ;
        StringBuilder TypeFilter = new StringBuilder ( ) ;

        TypeFilter.AppendFormat ( ResConstant.VirginSaveFormat ,
                                  ExtName                       ) ;

        Dlg.Filter = TypeFilter.ToString ( ) ;
        Dlg.FilterIndex = 2 ;
        
        if ( DialogResult.OK == Dlg.ShowDialog ( ) )
        {
            CurrDoc.Save ( Dlg.FileName ) ;
        }
    }
*/
        
    #region Command and Toolbar Setup
    
    /// <summary>
    /// Takes care of doing the initial command setup.
    /// </summary>
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
    
    /// <summary>
    /// Removes old commands and commandbars for this Add-In.
    /// </summary>
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
    
    #region Registry String Constants
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
    /// The persistant SaveNewAndReadOnly registry value.
    /// </summary>
    private const String SaveNewAndReadOnlyVal = "SaveNewAndReadOnly" ;
    #endregion

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
            
            int Val = (int)Key.GetValue ( TrimSpaceOnAutoSave , 1 ) ;
            
            if ( Val == 1 )
            {
                m_TrimWhiteSpace = true ;
            }
            else
            {
                m_TrimWhiteSpace = false ;
            }
            Val = (int)Key.GetValue ( SaveNewAndReadOnlyVal , 0 ) ;
            if ( Val == 1 )
            {
                m_SaveNewAndReadOnly = true ;
            }
            else
            {
                m_SaveNewAndReadOnly = false ;
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

            if ( true == SaveNewAndReadOnly )
            {
                Val = 1 ;
            }
            else
            {
                Val = 0 ;
            }
            Key.SetValue ( SaveNewAndReadOnlyVal , Val ) ;
        }
        catch ( System.Exception )
        {
        }
    }
}   // End of TrimAndSave class.
}   // End of SuperSaver namespace.
