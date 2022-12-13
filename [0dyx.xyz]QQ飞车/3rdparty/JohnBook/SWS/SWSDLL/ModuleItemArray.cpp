/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
                              The Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "SWSDLL.h"
#include "ModuleItemArray.h"

// The comparison function.
CModuleItem * CModuleItemArray :: Add ( DWORD64 dwAddr )
{
    // Allocate a new module.
    CModuleItem * pItem = new CModuleItem ( ) ;
    if ( FALSE == pItem->Initialize ( dwAddr ) )
    {
        delete pItem ;
        return ( NULL ) ;
    }

    EnterCriticalSection ( &m_CS ) ;
    
    m_pModItems.Add ( pItem ) ;

    // Yes, the OS headers define InterlockedExchangePointer so that it
    // causes an error building for X86 and /Wp64.  Great.
    #ifdef _M_IX86
    #pragma warning ( disable : 4311 4312 )
    #endif
    InterlockedExchangePointer ( &m_pLastUsed , pItem ) ;
    #ifdef _M_IX86
    #pragma warning ( default : 4311 4312 )
    #endif

    LeaveCriticalSection ( &m_CS ) ;

    return ( pItem ) ;
}

BOOL CModuleItemArray :: IncrementFunctionEntry ( DWORD64 dwAddr )
{
    EnterCriticalSection ( &m_CS ) ;
    if ( NULL != m_pLastUsed )
    {
        if ( TRUE == m_pLastUsed->AddressIsInThisModule ( dwAddr ) )
        {
            BOOL bRet = m_pLastUsed->IncrementAddressCount ( dwAddr ) ;
            LeaveCriticalSection ( &m_CS ) ;
            return ( bRet ) ;
        }
    }

    // Only loop if there are multiple modules.  If there's only one
    // module, it's in the m_pLastUsed field.
    if ( m_pModItems.GetSize ( ) > 1 )
    {
        for ( int i = 0 ; i <  m_pModItems.GetSize ( ) ; i++ )
        {
            if ( TRUE ==
                     m_pModItems[i]->AddressIsInThisModule ( dwAddr ) )
            {
                // Yes, the OS headers define
                // InterlockedExchangePointer so that it causes an
                // error building for X86 and /Wp64.  Great.
                #ifdef _M_IX86
                #pragma warning ( disable : 4311 4312 )
                #endif
                InterlockedExchangePointer ( (PVOID)&m_pLastUsed   ,
                                             (PVOID)m_pModItems[i]  ) ;
                #ifdef _M_IX86
                #pragma warning ( default : 4311 4312 )
                #endif

                BOOL bRet =
                     m_pModItems[i]->IncrementAddressCount ( dwAddr ) ;
                LeaveCriticalSection ( &m_CS ) ;
                return ( bRet ) ;
            }
        }
    }
    // It's a new module.
    CModuleItem * pItem = Add ( dwAddr ) ;

    // Yes, the OS headers define InterlockedExchangePointer so that it
    // causes an error building for X86 and /Wp64.  Great.
    #ifdef _M_IX86
    #pragma warning ( disable : 4311 4312 )
    #endif
    InterlockedExchangePointer ( (PVOID)&m_pLastUsed , (PVOID)pItem ) ;
    #ifdef _M_IX86
    #pragma warning ( default : 4311 4312 )
    #endif
    
    LeaveCriticalSection ( &m_CS ) ;

    return ( pItem->IncrementAddressCount ( dwAddr ) ) ;
}



