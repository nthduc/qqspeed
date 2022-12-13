#pragma once
#include "BaseInterface.h"

interface IBaseListener
	: public BaseInterface
{
public:
	virtual LRESULT OnMessage(DWORD dwMsgID, WPARAM wParam, LPARAM lParam) { return S_OK; }
	virtual BOOL IsListening() { return TRUE; }
};
