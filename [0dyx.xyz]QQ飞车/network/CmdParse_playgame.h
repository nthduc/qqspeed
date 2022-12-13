// CmdParse_playgame.h: interface for the CCmdParse_playgame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDPARSE_PLAYGAME_H__5F6949C1_02E8_4CED_AAAD_A4FD8E723347__INCLUDED_)
#define AFX_CMDPARSE_PLAYGAME_H__5F6949C1_02E8_4CED_AAAD_A4FD8E723347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdParse.h"

class CCmdParse_playgame : public CCmdParse  
{
public:
	CCmdParse_playgame();
	virtual ~CCmdParse_playgame();

	virtual BOOL SetCmdLine(LPCTSTR szCmdLine);

	void GetKey(BYTE *byKey);
	int GetGameID()				{	return m_iGameID;	}
	DWORD GetWndHandle()        {	return m_hWndHandle;	}
private:
	CString m_strKey;
	int m_iGameID;
	DWORD m_hWndHandle;
};

#endif // !defined(AFX_CMDPARSE_PLAYGAME_H__5F6949C1_02E8_4CED_AAAD_A4FD8E723347__INCLUDED_)
