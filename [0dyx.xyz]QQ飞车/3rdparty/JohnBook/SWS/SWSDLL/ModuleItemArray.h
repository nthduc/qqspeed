/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _MODULEITEMARRAY_H
#define _MODULEITEMARRAY_H

#include "ModuleItem.h"

// The typedef for the array of file handles.
typedef JSimpleArray < CModuleItem * > MODITEMARRAY ;

/*//////////////////////////////////////////////////////////////////////
                           CModuleItem Class
//////////////////////////////////////////////////////////////////////*/
// The array class that holds module info pointers.
class CModuleItemArray
{
/*//////////////////////////////////////////////////////////////////////
                  Public Construction and Destruction
//////////////////////////////////////////////////////////////////////*/
public      :
    CModuleItemArray ( void )
    {
        m_pLastUsed = NULL ;
        InitializeCriticalSection ( &m_CS ) ;
    }

    virtual ~CModuleItemArray ( void )
    {
        for ( int i = 0 ; i < m_pModItems.GetSize ( ) ; i++ )
        {
            delete m_pModItems[i] ;
        }
        DeleteCriticalSection ( &m_CS ) ;
    }

    // Adds a new module to the array with the address as the
    // starting point.
    CModuleItem * Add ( DWORD64 dwAddr ) ;

    // Increments the appropriate function entry.
    BOOL IncrementFunctionEntry ( DWORD64 dwAddr ) ;


/*//////////////////////////////////////////////////////////////////////
                          Private Data Members
//////////////////////////////////////////////////////////////////////*/
private     :

    // The protection.
    CRITICAL_SECTION m_CS ;
    // The most recently used module.
    CModuleItem * m_pLastUsed ;
    
    // The module item array.
    MODITEMARRAY  m_pModItems ;
} ;


#endif  // _MODULEITEMARRAY_H


