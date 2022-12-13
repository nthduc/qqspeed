using System ;
using EnvDTE ;
using System.Runtime.InteropServices ;

namespace SuperSaver
{
	public class SafeFindObject
	{
	    // All the prior settings off the find object.
	    vsFindAction            Action ;
	    Boolean                 Backwards ;
	    String                  FilesOfType ;
	    String                  FindWhat ;
	    Boolean                 KeepModifiedDocumentsOpen ;
	    Boolean                 MatchCase ;
	    Boolean                 MatchInHiddenText ;
	    Boolean                 MatchWholeWord ;
	    vsFindPatternSyntax     PatternSyntax ;
	    String                  ReplaceWith ;
	    vsFindResultsLocation   ResultsLocation ;
	    String                  SearchPath ;
	    Boolean                 SearchSubfolders ;
	    vsFindTarget            Target ;
	    
	    Find FindObj ;
	    
		public SafeFindObject ( _DTE AppObj )
		{
		    FindObj = AppObj.Find ;
		    
		    // Save off all the current options.
            Action =                     FindObj.Action ;
            Backwards =                  FindObj.Backwards ;
            FilesOfType =                FindObj.FilesOfType ;
            FindWhat =                   FindObj.FindWhat ;
            KeepModifiedDocumentsOpen =  FindObj.KeepModifiedDocumentsOpen ;
            MatchCase =                  FindObj.MatchCase ;
            MatchInHiddenText =          FindObj.MatchInHiddenText ;
            MatchWholeWord =             FindObj.MatchWholeWord ;
            PatternSyntax =              FindObj.PatternSyntax ;
            ReplaceWith =                FindObj.ReplaceWith ;
            ResultsLocation =            FindObj.ResultsLocation ;
            SearchPath =                 FindObj.SearchPath ;
            SearchSubfolders =           FindObj.SearchSubfolders ;
            Target =                     FindObj.Target ;
		}
		
		public void SetupFind ( vsFindTarget Target )
		{
            FindObj.Action = vsFindAction.vsFindActionReplaceAll ;
            FindObj.FindWhat = "{[^ \t]+}{[ \t]#$}" ;
            FindObj.ReplaceWith = "\\1" ;
            FindObj.MatchCase = false ;
            FindObj.MatchWholeWord = false ;
            FindObj.MatchInHiddenText = false ;
            FindObj.PatternSyntax =
                vsFindPatternSyntax.vsFindPatternSyntaxRegExpr ;
            FindObj.ResultsLocation =
                vsFindResultsLocation.vsFindResultsNone ;
            FindObj.Action = vsFindAction.vsFindActionReplaceAll ;
            
            FindObj.Target = Target ;
		}
		
		public void RestoreFind ( )
		{
            FindObj.Action =                     Action ;
            FindObj.Backwards =                  Backwards ;
            FindObj.FilesOfType =                FilesOfType ;
            FindObj.FindWhat =                   FindWhat ;
            FindObj.KeepModifiedDocumentsOpen =  KeepModifiedDocumentsOpen ;
            FindObj.MatchCase =                  MatchCase ;
            FindObj.MatchInHiddenText =          MatchInHiddenText ;
            FindObj.MatchWholeWord =             MatchWholeWord ;
            FindObj.PatternSyntax =              PatternSyntax ;
            FindObj.ReplaceWith =                ReplaceWith ;
            FindObj.ResultsLocation =            ResultsLocation ;
            FindObj.SearchPath =                 SearchPath ;
            FindObj.SearchSubfolders =           SearchSubfolders ;
            FindObj.Target =                     Target ;
        }
        
        public void Execute ( )
        {
            // OK, this is a little bad.  Having been burned so bad on
            // getting something this simple to work, I'm gun shy.
            // I figure it's easier to just do the Find.Execute instead of
            // messing around looking for text documents that may or may
            // not be open, I'll just catch the exception.
            try
            {
                FindObj.Execute ( ) ;
            }
            catch ( COMException /*eX*/ )
            {
                // I really wish there was a cleaner way of determining
                // what exactly the exception was.  In all cases, the
                // the HRESULT is 0x80004005 (E_FAIL).  The two times
                // I've seen this fail is when there are no files open
                // or the active document is a WinForm.  While I could
                // look at the messages to determine the cases, that's
                // hard coding the error messages so it's ugly.  
                // Basically, I have to punt here.  If it's a 
                // COMException, I'll just eat it.
            }
        
        }
	}
}
