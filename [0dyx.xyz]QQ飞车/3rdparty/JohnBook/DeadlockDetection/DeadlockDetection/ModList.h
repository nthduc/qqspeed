/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
A simple class to keep track of the loaded modules.
----------------------------------------------------------------------*/

#ifndef _MODLIST_H
#define _MODLIST_H

class CModList
{
////////////////////////////////////////////////////////////////////////
//                Public Constructors and Destructors
////////////////////////////////////////////////////////////////////////
public      :
    CModList ( void )
    {
        m_hHeap = NULL ;
        m_phMods = NULL ;
        m_uiCount = 0 ;
        m_uiAvailSlots = 0 ;
    }


    virtual ~CModList ( void )
    {
        if ( NULL != m_phMods )
        {
            VERIFY ( HeapFree ( m_hHeap , 0 , m_phMods ) ) ;
            m_phMods = NULL ;
        }
        m_uiCount = 0 ;
        m_hHeap = NULL ;
        m_uiAvailSlots = 0 ;
    }

////////////////////////////////////////////////////////////////////////
//                           Public Methods
////////////////////////////////////////////////////////////////////////
public      :
    // Sets the heap.
    void SetHeap ( HANDLE hHeap )
    {
        m_hHeap = hHeap ;
    }

    // Adds a single module to the list.
    void Add ( HMODULE hMod ) ;
    
    // This function works, but I'm not using it any more so I don't
    // want it messing up the code coverage values.
    // Removes the specified module from the list.
    //void Remove ( HMODULE hMod ) ;
    
    // This function works, but I'm not using it any more so I don't
    // want it messing up the code coverage values.
    // Changes all the values the current list based on the array
    // passed in.
    //void ReplaceArray ( HMODULE * pMod , DWORD dwCount ) ;

    // Returns the count.
    UINT GetCount ( void )
    {
        return ( m_uiCount ) ;
    }

    // Clears out the list.
    void Clear ( void )
    {
        if ( m_uiCount > 0 )
        {
            HeapFree ( m_hHeap , 0 , m_phMods ) ;
            m_phMods = NULL ;
            m_uiCount = 0 ;
            m_uiAvailSlots = 0 ;
        }
    }
    
    // Resets the list but does not release memory.
    void EmptyItems ( void )
    {
        if ( m_uiCount > 0 )
        {
            m_uiCount = 0 ;
            ZeroMemory ( m_phMods                            ,
                         sizeof ( HMODULE ) * m_uiAvailSlots  ) ;
        }
    }

    // Sees if the module is already in the list.
    BOOL IsInList ( HMODULE hMod )
    {
        if ( 0 == m_uiCount )
        {
            return ( FALSE ) ;
        }
        for ( UINT i = 0 ; i < m_uiCount ; i++ )
        {
            if ( m_phMods[ i ] == hMod )
            {
                return ( TRUE ) ;
            }
        }
        return ( FALSE ) ;
    }

    // Allows access to the individual modules.
    HMODULE operator[ ] ( int i ) ;

    CModList & operator= ( const CModList & x ) ;

////////////////////////////////////////////////////////////////////////
//                        Private Data Members
////////////////////////////////////////////////////////////////////////
private     :
    // The heap to allocate from.
    HANDLE    m_hHeap   ;
    // The allocated memory that holds the module list.
    HMODULE * m_phMods  ;
    // The number of items in the module list.
    UINT      m_uiCount ;
    // The current total spaces in the module list.
    UINT      m_uiAvailSlots ;

} ;

#endif  // _MODLIST_H


