////////////////////////////////////////////////
//     FileName:    GameApp_i.h
//     Author:      Fish
//     Date:        2006-10-10 15:57:09
//     Description: (]
////////////////////////////////////////////////

#pragma once

interface IGameService;
EXTERN_C const IID IID_IGameApp;
struct DECLSPEC_UUID("F197D0E7-B1D3-43ae-8E7F-11D50D2AE205") DECLSPEC_NOVTABLE
IGameApp : IUnknown
{
	STDMETHOD(GetService)(REFIID, void**) PURE;
	STDMETHOD(RegService)(REFIID, IGameService*) PURE;
	STDMETHOD(GetService)(REFIID, IGameService**) PURE;
	STDMETHOD(GetMainWnd)(HWND*) PURE;
	STDMETHOD(ActiveExtension)(LPCSTR) PURE;
	STDMETHOD(GetVersion)(DWORD*) PURE;
	STDMETHOD(GetLUVersion)(DWORD*, DWORD*) PURE;
	STDMETHOD(EnableIME)(BOOL) PURE;
	STDMETHOD(IsIMEEnabled)(BOOL*) PURE;
};

EXTERN_C const IID IID_IGameService;
struct DECLSPEC_UUID("580030A7-1E3A-43a7-9B5C-7BDC6A7374C8") DECLSPEC_NOVTABLE
IGameService : IUnknown
{
	STDMETHOD(AppInit)(IGameApp*) PURE;
	STDMETHOD(AppUnInit)() PURE;
};

EXTERN_C const IID IID_IGameExtension;
struct DECLSPEC_UUID("1FB38CF3-46A6-491c-9CE4-00F7DA8A0672") DECLSPEC_NOVTABLE
IGameExtension : IUnknown
{
	STDMETHOD(Load)(IGameApp*) PURE;
	STDMETHOD(Active)() PURE;
	STDMETHOD(DeActive)() PURE;
	STDMETHOD(UnLoad)() PURE;
	STDMETHOD(GetName)(LPCSTR*) PURE;
};

EXTERN_C const IID IID_IGameUpdate;
struct DECLSPEC_UUID("D9005E37-57E7-47d0-85B9-CFF930C4F4DD") DECLSPEC_NOVTABLE
IGameUpdate : IUnknown
{
	STDMETHOD(Update)(DWORD) PURE;
};

EXTERN_C const IID IID_IGameWndMessage;
struct DECLSPEC_UUID("C0DDDB47-893D-4098-92BF-380975D9E07F") DECLSPEC_NOVTABLE
IGameWndMessage : IUnknown
{
	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) PURE;
};

EXTERN_C const IID IID_IGameSettings;
struct DECLSPEC_UUID("9FEC7F41-7BAC-440e-8792-24EC1D0731A5") DECLSPEC_NOVTABLE
IGameSettings : IUnknown
{
	STDMETHOD(GetIntValue)(LPCSTR item, INT*) PURE;
	STDMETHOD(SetIntValue)(LPCSTR item, INT) PURE;
};

EXTERN_C const CLSID CLSID_GameAppImpl;
EXTERN_C const CLSID LIBID_GameAppLib;