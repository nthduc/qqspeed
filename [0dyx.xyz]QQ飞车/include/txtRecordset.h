// txtRecordset.h: interface for the CtxtRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TXTRECORDSET_H__70150D31_C86D_4883_9595_B454455333E2__INCLUDED_)
#define AFX_TXTRECORDSET_H__70150D31_C86D_4883_9595_B454455333E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>
#include <vector>
using namespace std;
typedef vector<string>	FieldsContainer;

class CtxtRecordset  
{
public:
	int GetFieldCount();
	BOOL SetFieldCount(int count);
	LPCSTR GetField(int index);
	BOOL SetField(int index, LPCSTR value);
	void Reset()
	{
		m_nFieldCount = 0;
		m_Fields.clear();
	}
	CtxtRecordset();
	virtual ~CtxtRecordset();

private:
	int		m_nFieldCount;
	FieldsContainer	m_Fields;
};

#endif // !defined(AFX_TXTRECORDSET_H__70150D31_C86D_4883_9595_B454455333E2__INCLUDED_)
