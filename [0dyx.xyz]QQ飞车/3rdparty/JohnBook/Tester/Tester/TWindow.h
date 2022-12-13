/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#pragma once
#include "resource.h"       // main symbols
#include "Helpers.h"

// ITWindow
[
	object,
	uuid("63690418-8142-4E20-85E6-2CB50508CA74"),
	dual,	helpstring("ITWindow Interface"),
	pointer_default(unique)
]
__interface ITWindow : IDispatch
{
    [propget, id(1), helpstring("The HWND wrapped by this interface")] HRESULT hWnd([out, retval] LONG_PTR* pVal);
    [propput, id(1), helpstring("The HWND wrapped by this interface")] HRESULT hWnd([in] LONG_PTR newVal);
    [propget, id(2), helpstring("Returns the window title")] HRESULT Caption([out, retval] BSTR* pVal);
    [propget, id(3), helpstring("Returns the title for the window")] HRESULT Title([out, retval] BSTR* pVal);
    [propget, id(4), helpstring("Returns the windows class name for the TWindow")] HRESULT ClassName([out, retval] BSTR* pVal);
    [propget, id(5), helpstring("Returns the top window coordinate")] HRESULT Top([out, retval] LONG* pVal);
    [propget, id(6), helpstring("Returns the bottom window coordinate")] HRESULT Bottom([out, retval] LONG* pVal);
    [propget, id(7), helpstring("Returns the left window coordinate")] HRESULT Left([out, retval] LONG* pVal);
    [propget, id(8), helpstring("Returns the right window coordinate")] HRESULT Right([out, retval] LONG* pVal);
    [propget, id(9), helpstring("Determines if the TWindow points to a valid window")] HRESULT Valid([out, retval] VARIANT_BOOL* pVal);
    [propget, id(10), helpstring("Determines if the window is enabled or not")] HRESULT Enabled([out, retval] VARIANT_BOOL* pVal);
    [propget, id(11), helpstring("Returns the parent TWindow for the current window")] HRESULT Parent([out, retval] ITWindow ** pVal);
    [propget, id(12), helpstring("Returns the process ID for the window")] HRESULT ProcessId([out, retval] LONG* pVal);
    [propget, id(13), helpstring("Returns the thread ID for the window")] HRESULT ThreadId([out, retval] LONG* pVal);
    [propget, id(14), helpstring("Returns the ID for the window")] HRESULT Id([out, retval] LONG* pVal);
    [propget, id(15), helpstring("Determines if the specified window is visible")] HRESULT Visible([out, retval] VARIANT_BOOL* pVal);
    [id(16), helpstring("Sets the window size on the screen")] HRESULT SetWindowSize(LONG X, LONG Y, LONG Width, LONG Height);
    [id(17), helpstring("Sets the window to the foreground window")] HRESULT SetForegroundTWindow(void);
    [id(18), helpstring("Sets the focus to the window")] HRESULT SetFocusTWindow(void);
    [id(19), helpstring("Finds the specified child window")] HRESULT FindChildTWindow([in] BSTR szClass, [in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(20), helpstring("Finds the specified child window"), hidden] HRESULT FindChildWindow([in] BSTR szClass, [in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(21), helpstring("Finds the first child window with the designated class")] HRESULT FindChildTWindowByClass([in] BSTR szClass, [out,retval] ITWindow ** pVal);
    [id(22), helpstring("(Back compat.) Finds the first child window with the designated class"), hidden] HRESULT FindChildWindowByClass([in] BSTR szClass, [out,retval] ITWindow ** pVal);
    [id(23), helpstring("Finds the first child window with the designated title")] HRESULT FindChildTWindowByTitle([in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(24), helpstring("(Back compat.)Finds the first child window with the designated title"), hidden] HRESULT FindChildWindowByTitle([in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(25), helpstring("Finds the child window with the designated ID")] HRESULT FindChildTWindowWithID([in] LONG iID, [out,retval] ITWindow ** pVal);
    [id(26), helpstring("(Back compat.)Finds the child window with the designated ID"), hidden] HRESULT FindChildWindowWithID([in] LONG iID, [out,retval] ITWindow ** pVal);
    [id(27), helpstring("(Back compat.)Enumerates the child windows into a collection"), hidden] HRESULT EnumChildren([out,retval] ITWindows ** pVal);
    [propget, id(28), helpstring("The collection of children windows below this TWindow")] HRESULT Children([out, retval] ITWindows ** pVal);
};



// CTWindow
[
	coclass,
	threading("apartment"),
	support_error_info("ITWindow"),
	vi_progid("Tester.TWindow"),
	progid("Tester.TWindow.1"),
	version(1.0),
	uuid("767D32EB-7B39-44DF-94EF-E7608AFB0F01"),
	helpstring("TWindow is a wrapper around an HWND for the Tester Utility"),
	default("ITWindow")
]
class ATL_NO_VTABLE TWindow :
	public ITWindow ,
	public ISupportErrorInfoImpl< &__uuidof (TWindow) >
{
public      :
	
	TWindow ( )
	{
	    m_hWnd = 0 ;
	}
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct ( )
	{
		return ( S_OK ) ;
	}
	
	void FinalRelease ( )
	{
	}

protected   :
    // The hWnd wrapped by this interface.
    HWND    m_hWnd ;
    
    ////////////////////////////////////////////////////////////////////
    // Helper functions.
    ////////////////////////////////////////////////////////////////////
    // Does the work to checks the pointer and gets the window rect.
    // This is called by Top, Left, Right, and Bottom.
    HRESULT GetWindowRect ( LONG * pVal , LPRECT lpRect ) ;
    
    // A wrapper around the AtlReportError function.
    HRESULT Error ( UINT uiResID , HRESULT hRes = E_UNEXPECTED ) ;

    // All the stuff plopped in by the wizards.
public      :

    STDMETHOD(get_hWnd)(LONG_PTR* pVal);
    STDMETHOD(put_hWnd)(LONG_PTR newVal);
    STDMETHOD(get_Caption)(BSTR* pVal);
    STDMETHOD(get_Title)(BSTR* pVal);
    STDMETHOD(get_ClassName)(BSTR* pVal);
    STDMETHOD(get_Top)(LONG* pVal);
    STDMETHOD(get_Bottom)(LONG* pVal);
    STDMETHOD(get_Left)(LONG* pVal);
    STDMETHOD(get_Right)(LONG* pVal);
    STDMETHOD(get_Valid)(VARIANT_BOOL* pVal);
    STDMETHOD(get_Enabled)(VARIANT_BOOL* pVal);
    STDMETHOD(get_Parent)(ITWindow ** pVal);
    STDMETHOD(get_ProcessId)(LONG* pVal);
    STDMETHOD(get_ThreadId)(LONG* pVal);
    STDMETHOD(get_Id)(LONG* pVal);
    STDMETHOD(get_Visible)(VARIANT_BOOL* pVal);
    STDMETHOD(SetWindowSize)(LONG X, LONG Y, LONG Width, LONG Height);
    STDMETHOD(SetForegroundTWindow)(void);
    STDMETHOD(SetFocusTWindow)(void);
    STDMETHOD(FindChildWindow)(BSTR szClass, BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindChildTWindow)(BSTR szClass, BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindChildTWindowByClass)(BSTR szClass, ITWindow ** pVal);
    STDMETHOD(FindChildWindowByClass)(BSTR szClass, ITWindow ** pVal);
    STDMETHOD(FindChildTWindowByTitle)(BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindChildWindowByTitle)(BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindChildTWindowWithID)(LONG iID, ITWindow ** pVal);
    STDMETHOD(FindChildWindowWithID)(LONG iID, ITWindow ** pVal);
    STDMETHOD(EnumChildren)(ITWindows ** pVal);
    STDMETHOD(get_Children)(ITWindows ** pVal);
};