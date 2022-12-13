/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
// This define must occur before any headers are included.
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// Include CRTDBG.H after all other headers
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef NEW_INLINE_WORKAROUND
#define NEW_INLINE_WORKAROUND new ( _NORMAL_BLOCK ,\
                                    __FILE__ , __LINE__ )
#define new NEW_INLINE_WORKAROUND
#endif
#endif  // _DEBUG

// Save off the definition of new on the macro stack.
#pragma push_macro ( "new" )
// Undefine new so I don't blow the class declaration.
#ifdef new
#undef new
#endif

// behavior change: an object of POD type constructed with an
// initializer of the form () will be default-initialized
#pragma warning ( disable : 4345 )



class TestClass
{
public :

    // The placement syntax version of new with the proper prototype needed
    // to ensure the source and line information is recorded for the
    // allocation.
#ifdef _DEBUG
    // iSize        - The size of the allocation.
    // iBlockType   - The DCRT block type.
    // lpszFileName - The source file name.
    // nLine        - The source line.
    static void * operator new ( size_t nSize        ,
                                 int    iBlockType   ,
                                 char * lpszFileName ,
                                 int    nLine         )
    {
        // Put any special code in here you need.

        // Allocate the actual memory with _malloc_dbg and pass in all
        // the parameters to record the allocation location.
        return ( _malloc_dbg ( nSize         ,
                               iBlockType    ,
                               lpszFileName  ,
                               nLine          ) ) ;
    }
    
    static void * operator new ( size_t nSize )
    {
        return ( malloc ( nSize ) ) ;
    }
#endif  // _DEBUG
} ;

// Restore the new macro to the saved value (i.e., the
// NEW_INLINE_WORKAROUND).
#pragma pop_macro ( "new" )

void main ( void )
{
    // Turn on the full heap checking
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF       |
        _CRTDBG_CHECK_ALWAYS_DF    |
        _CRTDBG_DELAY_FREE_MEM_DF  |
        _CRTDBG_LEAK_CHECK_DF       ) ;

    // Allocate some more memory.
    TCHAR * pNew = new TCHAR[ 200 ] ;
    TCHAR * pNew2 = new TCHAR[ 200 ] ;
    TCHAR * pMemLeak = (TCHAR*)malloc ( 100 ) ;
    TestClass * pClass = NULL ;
    pClass = new TestClass ( ) ;

    _tcscpy ( pNew , _T ( "New'd memory..." ) ) ;
    _tcscpy ( pNew2 , _T ( "More new'd memory..." ) ) ;
    _tcscpy ( pMemLeak , _T ( "Malloc'd memory..." ) ) ;

}