////////////////////////////////////////////////
//     FileName:    gui_i.h
//     Author:      Fish
//     Date:        2006-11-29 14:19:56
//     Description: []
////////////////////////////////////////////////
#pragma once

#include "framework/comdef.h"
#include "../gameui.h"

struct IBubbleMgr
{
	virtual BOOL AddBubble(UINT id, LPCSTR text, Point2D pos, INT life = -1, INT textsize = 12, INT space = 0, INT color = 0, INT width = -1, INT height = -1, INT margin = 10) = 0;
	virtual void Clear() = 0;
};

class	CScriptState;
class	IWgtData;
class	IDisplay;
enum	Msgbox_Cmd;

DECL_IID(IGUI, "8435198B-B4A4-4A3C-B830-B330CE230F35")
struct DECLSPEC_NOVTABLE IGUI : IUnknown
{
	STDMETHOD(CreateDevice)	()			PURE;
	STDMETHOD(DestroyDevice)()			PURE;
	STDMETHOD(LoadUI)(LPCSTR)			PURE;
	STDMETHOD(UnloadUI)()				PURE;
	STDMETHOD(RenderUI)()				PURE;
	STDMETHOD(Update)(DWORD)			PURE;
	STDMETHOD(EnterUILayout)(LPCSTR)	PURE;
	STDMETHOD_(CScriptState*,	GetScriptState)()	PURE;
	STDMETHOD_(LPCSTR,			GetCurLayout)()		PURE;
	STDMETHOD_(LPCSTR,			GetLastLayout)()	PURE;// Add by winco, 5/23/2007
	STDMETHOD_(IDisplay*,		GetDisplay)()		PURE;
	STDMETHOD_(IBubbleMgr*,		GetBubbleMgr)()		PURE;
	STDMETHOD(RegisterUICallBack)(UICallbackEvent eventName,CallBackFunc pFunc) PURE;
};

DECL_CLSID(GUI)