/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _VERBOSEMACROS_H
#define _VERBOSEMACROS_H

// The macros that take care of the verbose output for SWSDLL.DLL.
#define VERBOSE(sz)                             \
    if ( NULL != pfnVerboseOutput )             \
    {                                           \
        pfnVerboseOutput ( _T ( "%s" ) , sz ) ; \
    }
#define VERBOSE1(sz, p1)                \
    if ( NULL != pfnVerboseOutput )     \
    {                                   \
        pfnVerboseOutput ( sz , p1) ;   \
    }
#define VERBOSE2(sz, p1, p2)                    \
    if ( NULL != pfnVerboseOutput )             \
    {                                           \
        pfnVerboseOutput ( sz , p1 , p2 ) ;     \
    }
#define VERBOSE3(sz, p1, p2, p3)                \
    if ( NULL != pfnVerboseOutput )             \
    {                                           \
        pfnVerboseOutput ( sz , p1 , p2 , p3 ) ;\
    }
#define VERBOSE4(sz, p1, p2, p3, p4)             \
    if ( NULL != pfnVerboseOutput )              \
    {                                            \
    pfnVerboseOutput ( sz , p1 , p2 , p3 , p4 ) ;\
    }

#endif  // _VERBOSEMACROS_H

