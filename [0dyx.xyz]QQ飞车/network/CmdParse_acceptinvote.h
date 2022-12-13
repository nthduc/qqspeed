// CmdParse_acceptinvote.h: interface for the CCmdParse_acceptinvote class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDPARSE_ACCEPTINVOTE_H__426A41F7_1876_405F_921A_05F24BB660C5__INCLUDED_)
#define AFX_CMDPARSE_ACCEPTINVOTE_H__426A41F7_1876_405F_921A_05F24BB660C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdParse.h"

class CCmdParse_acceptinvote : public CCmdParse  
{
public:
	CCmdParse_acceptinvote();
	virtual ~CCmdParse_acceptinvote();

	short GetServerID()		{	return m_shServerID;	}
	short GetSectionID()	{	return m_shSectionID;	}
	short GetRoomID()		{	return m_shRoomID;		}
	DWORD GetWndHandle()    {   return m_hWndHandle;	}

	virtual BOOL SetCmdLine(LPCTSTR szCmdLine);

private:
	short m_shServerID;
	short m_shSectionID;
	short m_shRoomID;
	DWORD m_hWndHandle;
};

#endif // !defined(AFX_CMDPARSE_ACCEPTINVOTE_H__426A41F7_1876_405F_921A_05F24BB660C5__INCLUDED_)
