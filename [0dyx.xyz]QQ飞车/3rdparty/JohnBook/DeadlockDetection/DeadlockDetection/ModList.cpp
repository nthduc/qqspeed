/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
A simple class to keep track of the loaded modules.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "Modlist.h"

// The number of slots allocated each time.
const k_MINADDUNIT = 20 ;
// The number of bytes to increase the memory size by each time.
const k_ALLOCBYTESIZE = (sizeof ( HMODULE ) * k_MINADDUNIT ) ;

//void CModList :: ReplaceArray ( HMODULE * pMod , DWORD dwCount )
//{
//    if ( m_uiAvailSlots < ( dwCount + k_MINADDUNIT ) )
//    {
//        // Need to allocate a new array.
//        if ( NULL != m_phMods )
//        {
//            VERIFY ( HeapFree ( m_hHeap , 0 , m_phMods ) ) ;
//        }
//        m_uiAvailSlots = dwCount + k_MINADDUNIT ;
//        m_phMods = (HMODULE*)HeapAlloc ( m_hHeap                   ,
//                                         HEAP_ZERO_MEMORY          ,
//                                         m_uiAvailSlots *
//                                                sizeof ( HMODULE )  ) ;
//    }
//    else
//    {
//        ZeroMemory ( m_phMods , m_uiAvailSlots * sizeof ( HMODULE ) ) ;
//    }
//    memcpy ( m_phMods , pMod , dwCount * sizeof ( HMODULE ) ) ;
//    m_uiCount = dwCount ;
//}

void CModList :: Add ( HMODULE hMod )
{
    if ( 0 == m_uiCount )
    {
        // This is the first, so allocate.
        m_phMods = (HMODULE *)HeapAlloc ( m_hHeap            ,
                                          HEAP_ZERO_MEMORY   ,
                                          k_ALLOCBYTESIZE     ) ;
        m_uiAvailSlots = k_MINADDUNIT ;
        m_phMods[ 0 ] = hMod ;
    }
    else
    {
        if ( m_uiCount == m_uiAvailSlots )
        {
            m_phMods = (HMODULE *)HeapReAlloc ( m_hHeap               ,
                                                HEAP_ZERO_MEMORY      ,
                                                m_phMods              ,
                                                (sizeof(HMODULE)*
                                                  (m_uiCount+1)  )+
                                                     k_ALLOCBYTESIZE  );
            m_uiAvailSlots += k_MINADDUNIT ;
        }
        m_phMods[ m_uiCount ] = hMod ;
    }
    m_uiCount++ ;
}

//void CModList :: Remove ( HMODULE hMod )
//{
//    if ( m_uiCount > 0 )
//    {
//        UINT iIndex = (UINT)-1 ;
//        // Find it.
//        for ( UINT i = 0 ; i < m_uiCount ; i++ )
//        {
//            if ( m_phMods[ i ] == hMod )
//            {
//                iIndex = i ;
//                break ;
//            }
//        }
//        ASSERT ( iIndex != (UINT)-1 ) ;
//        if ( iIndex != -1 )
//        {
//            if ( iIndex == ( m_uiCount - 1 ) )
//            {
//                m_phMods[ iIndex ] = 0 ;
//
//            }
//            else
//            {
//                void * pDest = (byte*)m_phMods +
//                                            (sizeof(HMODULE) * iIndex) ;
//                void * pSrc = (byte*)m_phMods +
//                                         (sizeof(HMODULE) * (iIndex+1));
//                size_t iSize = (m_uiCount - iIndex - 1) *
//                                                       sizeof(HMODULE) ;
//                memmove ( pDest , pSrc , iSize ) ;
//                m_phMods [ m_uiCount - 1 ] = 0 ;
//            }
//            m_uiCount-- ;
//        }
//    }
//}

HMODULE CModList :: operator[ ] ( int i )
{
    if ( ( 0 == m_uiCount ) || ( (UINT)i > m_uiCount ) )
    {
        ASSERT ( FALSE ) ;
        return ( 0 ) ;
    }
    return ( m_phMods[ i ] ) ;
}

CModList & CModList :: operator= ( const CModList & x )
{
    Clear ( ) ;
    m_uiCount = x.m_uiCount ;
    m_uiAvailSlots = x.m_uiAvailSlots ;

    m_phMods = (HMODULE *)HeapAlloc ( m_hHeap            ,
                                      HEAP_ZERO_MEMORY   ,
                                      m_uiAvailSlots *
                                                  sizeof ( HMODULE ) ) ;

    CopyMemory ( m_phMods ,
                 x.m_phMods ,
                 m_uiCount * sizeof ( HMODULE ) ) ;
    return ( *this ) ;
}


