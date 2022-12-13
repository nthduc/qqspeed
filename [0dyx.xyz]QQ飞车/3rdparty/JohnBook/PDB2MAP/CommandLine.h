/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _COMMANDLINE_H
#define _COMMANDLINE_H

/*//////////////////////////////////////////////////////////////////////
// Necessary Includes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Constants & Defines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////*/
// The options structure.
typedef struct tag_OPTIONS
{
    // Indicates that help should be shown because it was either
    // requested or there was a command line problem.
    BOOL    bShowHelp ;
    // Don't show the happy logo!
    BOOL    bSkipLogo ;
    // Verbose mode.  Note that this can only be turned on from the
    // command line.
    BOOL    bVerbose ;
    // Undecorate all symbol names.
    BOOL    bUndecorate ;
    // The file to process.
    TCHAR   szFile[ MAX_PATH ] ;
} OPTIONS , * POPTIONS ;

/*//////////////////////////////////////////////////////////////////////
                               Prototypes
//////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------
FUNCTION        :   ResolveCommandLine
DISCUSSION      :
    Processes the command line options.
PARAMETERS      :
    stOpts  - The structure to fill with the selected options.
    argc    - The parameter count.
    argv    - The args.
RETURNS         :
    TRUE  - The options were processed correctly.
    FALSE - There was an error on the command line.
----------------------------------------------------------------------*/
BOOL ResolveCommandLine ( OPTIONS & stOpts , int argc , TCHAR * argv[]);

#endif  // _COMMANDLINE_H


