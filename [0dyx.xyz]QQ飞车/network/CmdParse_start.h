// CmdParse_start.h: interface for the CCmdParse_start class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDPARSE_START_H__75BF5A88_EF3F_4233_9EE4_2F7E1F9D7C9C__INCLUDED_)
#define AFX_CMDPARSE_START_H__75BF5A88_EF3F_4233_9EE4_2F7E1F9D7C9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdParse.h"

class CCmdParse_start : public CCmdParse  
{
public:
	CCmdParse_start();
	virtual ~CCmdParse_start();

	virtual BOOL SetCmdLine(LPCTSTR szCmdLine);
};

#endif // !defined(AFX_CMDPARSE_START_H__75BF5A88_EF3F_4233_9EE4_2F7E1F9D7C9C__INCLUDED_)
