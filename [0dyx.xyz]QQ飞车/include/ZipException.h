// ZipException.h: interface for the CZipException class.
//
// Copyright (C) 2000 Tadeusz Dracz
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZIPEXCEPTION_H__E3546921_D728_11D3_B7C7_E77339672847__INCLUDED_)
#define AFX_ZIPEXCEPTION_H__E3546921_D728_11D3_B7C7_E77339672847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CZipException : public CException  
{
public:
	DECLARE_DYNAMIC(CZipException)
	// sygnalize whether the error occured during zip or unzip
	bool m_bDuringZip;
	// convert zlib library error code to a ZipException code
	static int ZipErrToCause(int iZipError);
	// name of the zip file where the error occured
	CString m_szFileName;

	enum
	{
		noError,
		generic,
		streamEnd,
		needDict,
		errNo,
		streamError,
		dataError,
		memError,
		bufError,
		versionError,
		badFuncParam,
		badZipFile,
		badCrc,
	};
	// cause - takes one of the codes above
	int m_iCause;
	CZipException(int iCause = generic, LPCTSTR lpszZipName = NULL, bool bDuringZip = true);
	virtual ~CZipException();

};

// throw zip exception
// Parameters:
//		iZipError	- zlib error code
//		lpszZipName - name of the file where the error occured
//		bzipFile	- true if the error occured while compression, 
//	    			  false - decompression
void AfxThrowZipException(int iZipError = 1000, LPCTSTR lpszZipName = NULL, bool bZipFile = true);

#endif // !defined(AFX_ZIPEXCEPTION_H__E3546921_D728_11D3_B7C7_E77339672847__INCLUDED_)
