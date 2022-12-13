#ifndef __FILEVER_H__
#define __FILEVER_H__

#include "winver.h"
#include "assert.h"
#pragma comment(lib, "version")

inline DWORD GetVersion( LPCSTR fname )
{
    UINT nSizeOfInfo = GetFileVersionInfoSize( fname, NULL );
    if( nSizeOfInfo == 0 ) 
	{
        return 0;
    }
    char*				pInfobuf = new char[ nSizeOfInfo + 2 ];
    VS_FIXEDFILEINFO *	pFileInfo;
    UINT				nSize;
    
    GetFileVersionInfo( fname, NULL , nSizeOfInfo , pInfobuf );
    VerQueryValue( pInfobuf , TEXT("\\") , (LPVOID*)&pFileInfo , &nSize );
    
    
    DWORD innerVer[5];
    
    innerVer[0] = pFileInfo->dwFileVersionMS >> 16;
    innerVer[1] = pFileInfo->dwFileVersionMS & 0xffff;
    innerVer[2] = ( pFileInfo->dwFileVersionLS >> 16 ) / 100;
    innerVer[3] = ( pFileInfo->dwFileVersionLS >> 16 ) - innerVer[2] * 100;
    innerVer[4] = pFileInfo->dwFileVersionLS & 0xffff;
    
    delete []pInfobuf;
    return	  innerVer[0] * 10000000
        + innerVer[1] * 100000
        + innerVer[2] * 10000
        + innerVer[3] * 100
        + innerVer[4] ;
}

inline void FormatVersion( char *pbuf, int maxbuf, char *fname )
{
    DWORD innerVer_compact = GetVersion( fname );
    DWORD innerVer[5];
    
    innerVer[0] = innerVer_compact / 10000000;
    innerVer_compact -= innerVer[0] * 10000000;
    innerVer[1] = innerVer_compact / 100000;
    innerVer_compact -= innerVer[1] * 100000;
    innerVer[2] = innerVer_compact / 10000;
    innerVer_compact -= innerVer[2] * 10000;
    innerVer[3] = innerVer_compact / 100;
    innerVer_compact -= innerVer[3] * 100;
    innerVer[4] = innerVer_compact;
    
    char tempstr[32];

    // main version
	sprintf( tempstr, "%d.", innerVer[0] );
	strcat( pbuf, tempstr );
    
    // sub version
    if( innerVer[1] < 10 ) strcat( pbuf, "0" );
	sprintf( tempstr, "%d ", innerVer[1] );
	strcat( pbuf, tempstr );
    
    // alpha - beta - formal
    switch( innerVer[2] ) 
    {
    case 0:
        strcat( pbuf, "Alpha" );
        break;
    case 1:
        strcat( pbuf, "Beta" );
        break;
    case 2:
        strcat( pbuf, "ÕýÊ½°æ" );
        break;

    default:
        return;
    }
    
    // sub-ver of alpha - beta - formal and build number
	sprintf( tempstr, "%02d Build%03d" , innerVer[3] , innerVer[4] );
	strcat( pbuf, tempstr );
	assert( strlen(pbuf) < maxbuf );
}


inline DWORD GetVersionStringInfo( LPCSTR fname, LPCSTR FieldName, int Language, int CodePage, char* StringInfo, int MaxLen = MAX_PATH)
{
	UINT nSizeOfInfo = GetFileVersionInfoSize( fname, NULL );
	if( nSizeOfInfo == 0 ) 
	{
		return 0;
	}
	char*				pInfobuf = new char[ nSizeOfInfo + 2 ];
	assert(pInfobuf);

	GetFileVersionInfo( fname, NULL , nSizeOfInfo , pInfobuf );
	
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;

	// Read the list of languages and code pages.
	UINT cbTranslate = 0;
	VerQueryValue(pInfobuf, 
		"\\VarFileInfo\\Translation",
		(LPVOID*)&lpTranslate,
		&cbTranslate);

	// Read the file description for each language and code page.

	char SubBlock[MAX_PATH] = "";
	for( int i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
	{
		if ( lpTranslate[i].wLanguage == Language 
			&& lpTranslate[i].wCodePage == CodePage 
			|| Language == -1 )
		{
			sprintf( SubBlock, 
				TEXT("\\StringFileInfo\\%04x%04x\\%s"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage,
				FieldName);

			// Retrieve file description for language and code page "i". 
			void* lpBuffer = NULL;
			UINT dwBytes = 0;
			VerQueryValue(pInfobuf, 
				SubBlock, 
				&lpBuffer, 
				&dwBytes); 
			strncpy(StringInfo, (char*)lpBuffer, MaxLen);
			delete[] pInfobuf;
			//delete[] lpTranslate;
			return min(dwBytes, MaxLen);
		}
	}
	delete[] pInfobuf;
	return 0;
}
#endif