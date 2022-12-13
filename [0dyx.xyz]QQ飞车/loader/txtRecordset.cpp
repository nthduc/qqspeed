// txtRecordset.cpp: implementation of the CtxtRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "txtRecordset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtxtRecordset::CtxtRecordset():
m_nFieldCount(0)
{

}

CtxtRecordset::~CtxtRecordset()
{

}

BOOL CtxtRecordset::SetField(int index, LPCSTR value)
{
	if ( index >= m_Fields.size() )
	{
		m_Fields.resize(index + 1);
	}
	m_Fields[index] = value;
	return TRUE;
}

LPCSTR CtxtRecordset::GetField(int index)
{
	return m_Fields[index].c_str();
}

BOOL CtxtRecordset::SetFieldCount(int count)
{
	m_nFieldCount = count;
	m_Fields.resize(count);
	return TRUE;
}

int CtxtRecordset::GetFieldCount()
{
	return m_Fields.size();
}
