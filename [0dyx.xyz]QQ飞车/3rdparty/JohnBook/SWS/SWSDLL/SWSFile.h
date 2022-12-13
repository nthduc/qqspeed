/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _SWSFILE_H
#define _SWSFILE_H

#include "FileFormat.h"

// The compare function typedef.
typedef int (__cdecl *PFNCOMPARE )( const void * elem1 ,
                                    const void * elem2  ) ;
class CSWSFile
{
/*//////////////////////////////////////////////////////////////////////
                  Public Construction and Destruction
//////////////////////////////////////////////////////////////////////*/
public      :
    CSWSFile ( void ) ;
    virtual ~CSWSFile ( void ) ;
    
/*//////////////////////////////////////////////////////////////////////
                        Public Open Methods
                   (These are only for reading data!)
//////////////////////////////////////////////////////////////////////*/
public      :
    // Opens the file for reading and updating.
    BOOL Open ( LPCTSTR szFileName ) ;
    
    // Returns the header data.
    BOOL GetHeaderData ( DWORD &   dwLinkTime   ,
                         DWORD &   dwEntryCount ,
                         DWORD64 & dwLoadAddr   ,
                         DWORD &   dwImageSize  ,
                         DWORD &   dwFlags      ,
                         TCHAR *   szModuleName   ) ;

    // Returns the a pointer to the header data for READING only!
    // This function is implemented in this class.
    const LPSWSFILEHEADER GetReadOnlyHeaderData ( void ) ;
    
/*//////////////////////////////////////////////////////////////////////
                        Public Creation Methods
                   (These are only for writing data!)
//////////////////////////////////////////////////////////////////////*/
public      :
    // Creates a new file.  If the file already exists, it will be
    // overwritten.
    BOOL Create ( LPCTSTR szFileName ) ;
    
    // Sets the header data.  This can be called at any time and will
    // ensure that the file pointer is not disturbed.
    BOOL SetHeaderData ( DWORD   dwLinkTime   ,
                         DWORD   dwEntryCount ,
                         DWORD64 dwLoadAddr   ,
                         DWORD   dwImageSize  ,
                         DWORD   dwFlags      ,
                         LPCTSTR szModuleName  ) ;

    // Adds data to the file.
    BOOL AddData ( DWORD64 dwAddr , DWORD dwSize , DWORD dwExecCount ) ;
    
/*//////////////////////////////////////////////////////////////////////
                             Public Methods
//////////////////////////////////////////////////////////////////////*/
public      :
    // Both opening and creating can call this functions.
    void Close ( void ) ;

    // Deletes an opened file.
    void Delete ( void ) ;

    // Sorts by address.
    BOOL SortByAddress ( void ) ;
    
    // Sorts by execution count.
    BOOL SortByExecutionCount ( void ) ;
    
    // Gets the data at the specific index.
    LPSWSENTRY GetIndex ( int iIndex ) ;

    // Finds the specific entry with dwAddr as the address.
    LPSWSENTRY FindAddress ( DWORD64 dwAddr ) ;
    
/*//////////////////////////////////////////////////////////////////////
                        Protected Helper Methods
//////////////////////////////////////////////////////////////////////*/
protected   :
    // Frees all associated resources.
    void FreeResources ( void ) ;

    // Allocates the buffer used to store data points when creating a
    // file.
    BOOL AllocateDataBuffer ( UINT uiCount ) ;

    // Appends an element to the data buffer.
    BOOL AppendToDataBuffer ( PVOID pElement ) ;

/*//////////////////////////////////////////////////////////////////////
                         Protected Member Data
//////////////////////////////////////////////////////////////////////*/
protected   :
    // How this instance is supposed to be used.  TRUE means file is
    // brand new and can only have values set in it.
    BOOL m_bCreated ;

    // The file handle.  Used when opening an existing file and when
    // creating a file.
    HANDLE m_hFile ;

    // Variables used when opening an existing file.
    // The mapping handle.
    HANDLE m_hMapped ;
    // The mapped memory.
    LPBYTE m_pMappedMem ;

    // Variables used when creating a file.
    // The buffer to store the entry data.
    LPBYTE m_pBuffer ;
    // The total elements in m_pBuffer ;
    UINT m_uiTotalElems ;
    // The current number of elements in the buffer.
    UINT m_uiNumElems ;

    // The file name.
    TCHAR m_szFileName[ MAX_PATH ] ;
} ;

#endif  // _SWSFILE_H

