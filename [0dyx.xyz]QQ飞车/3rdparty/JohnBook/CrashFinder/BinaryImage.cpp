/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "BinaryImage.h"
#include "BinaryImageOptionsDlg.h"

#define MakePtr( cast , ptr , AddValue ) \
                                 (cast)( (DWORD)(ptr)+(DWORD)(AddValue))


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

// MFC stuff.
IMPLEMENT_SERIAL ( CBinaryImage , CObject , 1 )

CBinaryImage :: CBinaryImage ( void )
              : CObject ( )     ,
                m_szFullName ( )
{
    ASSERT ( this ) ;
    m_dwLoadAddress     = 0     ;
    m_dwCharacteristics = 0     ;
    m_bIsValid          = FALSE ;
    m_dwExtraData       = 0     ;
}

void CBinaryImage :: Serialize ( CArchive & ar )
{
    ASSERT ( this ) ;
    if( ar.IsStoring( ) )
    {
        ar << m_szFullName    ;
    }
    else
    {
        ar >> m_szFullName    ;
    }
}

////////////////////////////////////////////////////////////////////////
//                Public Construction and Desctruction
////////////////////////////////////////////////////////////////////////
CBinaryImage :: CBinaryImage ( LPCTSTR szFileName )
              : CObject ( )     ,
                m_szFullName ( )
{
    ASSERT ( this ) ;
    m_dwLoadAddress = 0 ;
    m_szFullName = szFileName ;
    m_bIsValid = FALSE ;
    m_dwCharacteristics = 0 ;
    m_dwExtraData = 0 ;

    // Validate the image without showing errors.
    IsValidImage ( FALSE ) ;
}

CBinaryImage :: CBinaryImage ( const CBinaryImage & cInit )
{
    ASSERT ( this ) ;
    m_szFullName        = cInit.m_szFullName        ;
    m_dwLoadAddress     = cInit.m_dwLoadAddress     ;
    m_dwCharacteristics = cInit.m_dwCharacteristics ;
    m_bIsValid          = cInit.m_bIsValid          ;
    m_dwExtraData       = cInit.m_dwExtraData       ;
}

CBinaryImage :: ~CBinaryImage ( void )
{
    ASSERT ( this ) ;
}

////////////////////////////////////////////////////////////////////////
//                     Public Validation Methods
////////////////////////////////////////////////////////////////////////
// Returns TRUE if the image passed in the constructor is a valid
// binary image.  Returns FALSE otherwise.
BOOL CBinaryImage :: IsValidImage ( BOOL bShowErrors )
{
    ASSERT ( this ) ;
    // If the m_bIsValid value was already true, then the image is OK.
    if ( TRUE == m_bIsValid )
    {
        return ( m_bIsValid ) ;
    }

    // The error message holder.
    CString sMsg ;

    // First check if the file actually exists.
    CFileFind cFFile ;

    BOOL bRet = cFFile.FindFile ( m_szFullName ) ;
    if ( FALSE == bRet )
    {
        if ( TRUE == bShowErrors )
        {
            sMsg.FormatMessage ( IDS_FILENOTFOUND , m_szFullName ) ;
            AfxMessageBox ( sMsg ) ;
        }
        m_bIsValid = FALSE ;
        return ( FALSE ) ;
    }

    // The file exists, now see if it is a real PE file.
    LOADED_IMAGE stLI ;
    if ( ( FALSE == MapAndLoad ( (LPTSTR)(LPCTSTR)m_szFullName ,
                                  NULL                         ,
                                  &stLI                        ,
                                  TRUE                         ,
                                  TRUE                          ) ) ||
         ( IMAGE_NT_SIGNATURE != stLI.FileHeader->Signature       )   )
    {
        if ( TRUE == bShowErrors )
        {
            sMsg.FormatMessage ( IDS_NOTPEIMAGE , m_szFullName ) ;
            AfxMessageBox ( sMsg ) ;
        }
        m_bIsValid = FALSE ;
        return ( FALSE ) ;
    }

    // The load address is ALWAYS based off the current image.
    m_dwLoadAddress = stLI.FileHeader->OptionalHeader.ImageBase ;

    // Save off the characteristics.
    m_dwCharacteristics = stLI.Characteristics ;

    // Unload the image.
    bRet = UnMapAndLoad ( &stLI ) ;
    ASSERT ( TRUE == bRet ) ;

    m_bIsValid = TRUE ;
    return ( m_bIsValid ) ;
}

////////////////////////////////////////////////////////////////////////
//                 Public Property Retrieval Methods
////////////////////////////////////////////////////////////////////////
// Returns the load address for this image.
DWORD CBinaryImage :: GetLoadAddress ( void ) const
{
    ASSERT ( this ) ;
    return ( m_dwLoadAddress ) ;
}

// Returns the full name for this image.
const CString CBinaryImage :: GetFullName ( void ) const
{
    ASSERT ( this ) ;
    return ( m_szFullName ) ;
}

// Returns just the name of the image.
const CString CBinaryImage :: GetName ( void ) const
{
    ASSERT ( this ) ;

    TCHAR szFName [ MAX_PATH ] ;
    TCHAR szExt [ MAX_PATH ] ;

     _tsplitpath ( m_szFullName , NULL , NULL , szFName , szExt ) ;

     CString szTemp = szFName ;

     szTemp += szExt ;

     return ( szTemp ) ;
}

DWORD CBinaryImage :: GetCharacteristics ( void ) const
{
    ASSERT ( this ) ;
    return ( m_dwCharacteristics ) ;
}

// Returns a string that contains any additional information that
// the class would like to return about this image.
const CString CBinaryImage :: GetAdditionalInfo ( void ) const
{
    ASSERT ( this ) ;
    CString szTemp ;
    return ( szTemp ) ;
}

// Gets the extra data for this class.
DWORD CBinaryImage :: GetExtraData ( void ) const
{
    ASSERT ( this ) ;
    return ( m_dwExtraData ) ;
}

////////////////////////////////////////////////////////////////////////
//                  Public Property Setting Methods
////////////////////////////////////////////////////////////////////////
BOOL CBinaryImage :: SetProperties ( void )
{
    ASSERT ( this ) ;
    CBinaryImageOptionsDlg cDlg ;
    cDlg.m_dwAddr = m_dwLoadAddress ;
    if ( IDOK == cDlg.DoModal ( ) )
    {
        if ( cDlg.m_dwAddr != m_dwLoadAddress )
        {
            m_dwLoadAddress = cDlg.m_dwAddr ;
            return ( TRUE ) ;
        }
    }
    return ( FALSE ) ;
}

void CBinaryImage :: SetLoadAddress ( DWORD dwAddr )
{
    ASSERT ( this ) ;
    m_dwLoadAddress = dwAddr ;
}

void CBinaryImage :: SetExtraData ( DWORD dwData )
{
    ASSERT ( this ) ;
    m_dwExtraData = dwData ;
}


////////////////////////////////////////////////////////////////////////
//                          Public Operators
////////////////////////////////////////////////////////////////////////
CBinaryImage & CBinaryImage:: operator = ( const CBinaryImage & rhs )
{
    ASSERT ( this ) ;
    if ( this != &rhs )
    {
        m_szFullName        = rhs.m_szFullName        ;
        m_dwLoadAddress     = rhs.m_dwLoadAddress     ;
        m_dwCharacteristics = rhs.m_dwCharacteristics ;
        m_bIsValid          = rhs.m_bIsValid          ;
        m_dwExtraData       = rhs.m_dwExtraData       ;
    }
    return ( *this ) ;
}


BOOL CBinaryImage::GetModules(LPCTSTR pModuleName, CMapStringToPtr& ModuleList)
{
        HINSTANCE hModule=NULL;
        CString name(pModuleName);
        name.MakeUpper();
        ModuleList[name]=NULL;

        hModule=LoadLibrary(pModuleName);
        if (hModule==NULL)
                return FALSE;

        PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule ;

    // Is this the MZ header?
    if ((TRUE==IsBadReadPtr(pDOSHeader, sizeof(IMAGE_DOS_HEADER))) ||
        (IMAGE_DOS_SIGNATURE!=pDOSHeader->e_magic))
    {
                FreeLibrary(hModule);
        SetLastErrorEx(ERROR_INVALID_PARAMETER, SLE_ERROR);
        return FALSE;
    }

    // Get the PE header.
    PIMAGE_NT_HEADERS pNTHeader=MakePtr(PIMAGE_NT_HEADERS,
                                        pDOSHeader,
                                        pDOSHeader->e_lfanew);

    // Is this a real PE image?
    if ((TRUE==IsBadReadPtr(pNTHeader, sizeof(IMAGE_NT_HEADERS))) ||
        (IMAGE_NT_SIGNATURE!=pNTHeader->Signature))
    {
                FreeLibrary(hModule);
        SetLastErrorEx(ERROR_INVALID_PARAMETER, SLE_ERROR);
        return FALSE;
    }

    // Get the pointer to the imports section.
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc=MakePtr(PIMAGE_IMPORT_DESCRIPTOR,
                 pDOSHeader,
                                 pNTHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress ) ;
    // Does it have an imports section?
    if (0==pImportDesc)
    {
                FreeLibrary(hModule);
        return FALSE;
    }
    // Loop through the import module descriptors looking for the
    // module whose name matches szImportMod.
        LPVOID pPtr;
    while (NULL!=pImportDesc->Name)
    {
                CString szModName = MakePtr(PSTR, pDOSHeader, pImportDesc->Name);
                if (SearchForModule(szModName))
                {
                        szModName.MakeUpper();
                        if (!ModuleList.Lookup(szModName, pPtr))
                        {
                                ModuleList[szModName]=NULL;
                                if (GetModules(szModName, ModuleList)==FALSE)
                                {
                                        FreeLibrary(hModule);
                                        return FALSE;
                                }

                        }
                }

        // Look at the next one.
        pImportDesc++;
    }


    FreeLibrary(hModule);
    return TRUE;
}

BOOL CBinaryImage::SearchForModule(CString& szModName)
{
        CString name, ext;
        int i=szModName.Find(_T('.'));
        if (i!=-1)
        {
                name=szModName.Left(i);
                ext=szModName.Mid(i);
        }
        else
        {
                name=szModName;
                ext=_T('.');
        }
        TCHAR buf[MAX_PATH+1];
        LPTSTR pFilePart;
        DWORD len=SearchPath(NULL, name, ext, MAX_PATH, buf, &pFilePart);
        if (len>0 && len<MAX_PATH)
        {
                szModName=buf;
                return TRUE;
        }
        else
        {
                szModName=name+ext;
                return FALSE;
        }

}

BOOL CBinaryImage::InitIgnoredImage(CMapStringToPtr& ModuleList, BOOL bFullList)
{
        HKEY hkey;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\MSJ\\CRASHFINDER"), 0, KEY_READ, &hkey)==ERROR_SUCCESS)
        {
                CString szValueName;
                TCHAR valueName[MAX_PATH+1];
                DWORD valuelen;
                DWORD i=0;
                DWORD type, content, valsize;
                do
                {
                        valuelen=MAX_PATH;
                        valsize=sizeof(content);
                        LONG retVal=RegEnumValue(hkey, i, valueName, &valuelen, 0, &type, (LPBYTE)&content, &valsize);
                        if (retVal!=ERROR_SUCCESS)
                                break;
                        if (type==REG_DWORD)
                        {
                                szValueName=valueName;
                                szValueName.MakeUpper();
                                if (bFullList)
                                {
                                        if (content>0)
                                                ModuleList[szValueName]=(LPVOID)1;
                                        else
                                                ModuleList[szValueName]=NULL;
                                }
                                else
                                {
                                        if (content>0)
                                                ModuleList[szValueName]=(LPVOID)1;
                                }
                        }
                        i++;
                } while (TRUE);
                RegCloseKey(hkey);
                return TRUE;
        }
        return FALSE;
}