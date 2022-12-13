#ifndef __TENIOPOINTER_HELPER_H__
#define __TENIOPOINTER_HELPER_H__

namespace Tenio
{
IComponent* CreateComponentInModule(HMODULE hModule, DWORD dwComponentId);
HMODULE GetFactoryDLLHandle(LPCTSTR lpszRegistry);
void GetTenioFactoryDLLPath(LPCTSTR lpszRegistry, LPTSTR lpszOutput,
							int nCharCount);
void TranslateComponentRelativePath(LPCTSTR lpszRegistryPath, 
	LPCTSTR lpszComponentDLLPath, LPTSTR lpszOutput, int nCharCount);
void CombineWithPathSeparator(LPCTSTR lpszLeft, LPCTSTR lpszRight,
							  LPTSTR lpszOutput, int nCharCount);
void ReadIniString(LPCTSTR lpszFile, LPCTSTR lpszSectionName,
						  LPCTSTR lpszKeyName, LPCTSTR lpszDefaultValue, LPTSTR lpszValue,
						  int nCharCount);
void TrimString(LPCTSTR lpszSource, LPTSTR lpszDest, int nCharCount);
BOOL IsWhiteChar(const TCHAR& ch);
void GetRealRegistryFile(LPCTSTR lpszRegistry, LPTSTR lpszRealRegistry, 
						 int nCharCount);
void TrackRegistry2(LPCTSTR lpszSource, LPTSTR lpszDest, int nCharCount);
BOOL IsStringEmpty(LPCTSTR lpszString);
BOOL CombineRelativePath(LPCTSTR lpszOriginFile, LPCTSTR lpszRelativePath,
						 LPTSTR lpszCombinedPath, int nCharCount);
BOOL StringCopyFix(LPTSTR lpszDest, int	nMaxCharCount, LPCTSTR lpszSrc,
				   int	nToCopyCharCount);
BOOL StringCat(LPTSTR lpszDest, int nCharCount, LPCTSTR lpszSource);
BOOL StringCopy(LPTSTR lpszDest, int nMaxCharCount, LPCTSTR lpszSrc);
BOOL StringCheck(LPCTSTR lpszString, size_t nMaxCharCount);
BOOL Relative2AbsolutePath(LPCTSTR lpszRelativePath, LPTSTR	lpszOutput,
						   int	nCharCount);
LPCTSTR GetRootPath();
LPCTSTR FindRightSideSeparator(LPCTSTR lpszPath);
BOOL IsRelativePath(LPCTSTR lpszPath);
void TTenio_Trace(LPCTSTR lpszFormat, ...);

enum EPARSESTATUS
{
	EPS_FREE	   = 0,
	EPS_INSENTENCE = 1
};

};

#endif