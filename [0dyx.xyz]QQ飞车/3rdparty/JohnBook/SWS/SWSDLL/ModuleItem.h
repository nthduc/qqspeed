/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _MODULEITEM_H
#define _MODULEITEM_H

#include "SWSFile.h"

/*//////////////////////////////////////////////////////////////////////
                           CModuleItem Class
//////////////////////////////////////////////////////////////////////*/
// This class holds the information about a particular module.
class CModuleItem
{
/*//////////////////////////////////////////////////////////////////////
                  Public Construction and Destruction
//////////////////////////////////////////////////////////////////////*/
public      :
    CModuleItem ( void )
    {
        m_pcHeader = NULL ;
        m_pFile = NULL ;
        m_dwLoadAddr = 0 ;
    }

    virtual ~CModuleItem ( void )
    {
        if ( NULL != m_pFile )
        {
            delete m_pFile ;
        }
    }

    // Initialize this class based on the particular address.
    BOOL Initialize ( DWORD64 dwAddr ) ;

    // Returns TRUE if the specified address is in this module.
    BOOL AddressIsInThisModule ( DWORD64 dwAddr ) ;

    // Increments the address value.
    BOOL IncrementAddressCount ( DWORD64 dwAddr ) ;

    // Returns TRUE if this image has been relocated in memory
    // from it's preferred load address.
    BOOL IsRelocated ( void )
    {
        return ( 0 != m_dwLoadAddr ) ;
    }

    DWORD64 ConvertRelocatedAddress ( DWORD64 dwAddr )
    {
        DWORD64 dwConvertedAddr = dwAddr ;
        if ( 0 != m_dwLoadAddr )
        {
            // Convert the address to one in the file.
            dwConvertedAddr = m_pcHeader->dwLoadAddr +
                              ( dwAddr - m_dwLoadAddr ) ;
        }
        return ( dwConvertedAddr ) ;

    }


/*//////////////////////////////////////////////////////////////////////
                          Private Data Members
//////////////////////////////////////////////////////////////////////*/
private     :
    // The pointer to the file associated with this module.
    CSWSFile * m_pFile ;
    // The load address for this module.  If this is anything other
    // than zero, the module was relocated.
    DWORD64 m_dwLoadAddr ;
    // The constant pointer to the file header so I can do the fast
    // lookups on particular addresses.
    LPSWSFILEHEADER m_pcHeader ;
} ;

#endif  // _MODULEITEM_H


