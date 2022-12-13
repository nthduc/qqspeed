/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#pragma once
#include "resource.h"       // main symbols


// ITInput
[
	object,
	uuid("929EA3FA-3A82-4754-A1B0-7B4C16E17A86"),
	dual,	helpstring("ITInput Interface"),
	pointer_default(unique)
]
__interface ITInput : IDispatch
{
    [propget, id(1), helpstring("Dictates if playback occurs on multiple monitors or just the primary monitor")] HRESULT MultiMonitor([out, retval] VARIANT_BOOL* pVal);
    [propput, id(1), helpstring("Dictates if playback occurs on multiple monitors or just the primary monitor")] HRESULT MultiMonitor([in] VARIANT_BOOL newVal);
    [propget, id(2), helpstring("(Back compat.) No longer used, remove from your scripts!"), hidden] HRESULT AbsoluteScreenCoordinates([out, retval] VARIANT_BOOL* pVal);
    [propput, id(2), helpstring("(Back compat.) No longer used, remove from your scripts!"), hidden] HRESULT AbsoluteScreenCoordinates([in] VARIANT_BOOL newVal);
    [propget, id(3), helpstring("(Back compat.)No longer supported!"), hidden] HRESULT VirtualScreenCoordinates([out, retval] VARIANT_BOOL* pVal);
    [propput, id(3), helpstring("(Back compat.)No longer supported!"), hidden] HRESULT VirtualScreenCoordinates([in] VARIANT_BOOL newVal);
    [id(4), helpstring("Plays keystrokes and mouse usage")] HRESULT PlayInput([in] BSTR Str);
    [id(5), helpstring("(Back compat.)Plays keystrokes and mouse usage"), hidden] HRESULT PlayKeys([in] BSTR Str);
};



// TInput

[
	coclass,
	threading("apartment"),
	support_error_info("ITInput"),
	vi_progid("Tester.TInput"),
	progid("Tester.TInput.1"),
	version(1.0),
	uuid("9EE83F0E-21F9-45D7-974D-FAE9A57438DA"),
	helpstring("Allows users to send keystokes and mousestoke input to windows"),
    default("ITInput"),
    support_error_info("ITInput")
	
]
class ATL_NO_VTABLE TInput :
	public ITInput
{
public:
	TInput()
	{
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease()
	{
	}
	
protected   :
    // Wrapper around Error. Their order is all wrong!
    HRESULT Error ( UINT uID , HRESULT hRes = E_UNEXPECTED )
    {
        return ( CComCoClass<ITInput>::Error ( uID        ,
                                                GUID_NULL  ,
                                                hRes        ) ) ;
    }
	

public:

    STDMETHOD(get_MultiMonitor)(VARIANT_BOOL* pVal);
    STDMETHOD(put_MultiMonitor)(VARIANT_BOOL newVal);
    STDMETHOD(get_AbsoluteScreenCoordinates)(VARIANT_BOOL* pVal);
    STDMETHOD(put_AbsoluteScreenCoordinates)(VARIANT_BOOL newVal);
    STDMETHOD(get_VirtualScreenCoordinates)(VARIANT_BOOL* pVal);
    STDMETHOD(put_VirtualScreenCoordinates)(VARIANT_BOOL newVal);
    STDMETHOD(PlayInput)(BSTR Str);
    STDMETHOD(PlayKeys)(BSTR Str);
};

