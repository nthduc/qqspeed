/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#pragma once
#include "resource.h"       // main symbols
#include "Helpers.h"


// Includes necessary for this header to work.
#include "TWindow.h"
#include "TWindows.h"


// ITSystem
[
	object,
	uuid("63FB6C3F-6398-4500-AB85-984AC82B9B95"),
	dual,	helpstring("ITSystem Interface"),
	pointer_default(unique)
]
__interface ITSystem : IDispatch
{
    [id(1), helpstring("(Back compat.) Returns the TWindow that currently is in the foreground"), hidden] HRESULT GetForegroundTWindow([out,retval] ITWindow ** pVal);
    [propget, id(2), helpstring("Returns the TWindow that currently is in the foreground")] HRESULT ForegroundTWindow([out, retval] ITWindow ** pVal);
    [id(3), helpstring("(Back compat.) Returns the TWindow collection for all top level windows"), hidden] HRESULT EnumTopWindows([out,retval] ITWindows ** pVal);
    [propget, id(4), helpstring("Returns the top level TWindows collection")] HRESULT TopTWindows([out, retval] ITWindows ** pVal);
    [id(5), helpstring("(Back compat.) Find the top level window that has the exact class and title"), hidden] HRESULT FindTopWindow([in] BSTR szClass, [in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(6), helpstring("Find the top level TWindow that has the exact class and title")] HRESULT FindTopTWindow([in] BSTR szClass, [in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(7), helpstring("(Back compat.)Finds the top level windows that has the exact specified title."), hidden] HRESULT FindTopWindowByTitle([in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(8), helpstring("Finds the top level windows that has the exact specified title.")] HRESULT FindTopTWindowByTitle([in] BSTR szTitle, [out,retval] ITWindow ** pVal);
    [id(9), helpstring("(Back compat.)Finds the top level windows that has the exact specified class."), hidden] HRESULT FindTopWindowByClass([in] BSTR szClass, [out,retval] ITWindow ** pVal);
    [id(10), helpstring("Finds the top level windows that has the exact specified class.")] HRESULT FindTopTWindowByClass([in] BSTR szClass, [out,retval] ITWindow ** pVal);
    [id(11), helpstring("Starts a program.")] HRESULT Execute([in] BSTR szProgram,[out, retval] VARIANT_BOOL* pVal);
    [id(12), helpstring("Absolutely stalls for seconds")] HRESULT Sleep([in]DOUBLE Seconds);
    [id(13), helpstring("Absolutely stalls for milliseconds")] HRESULT Pause([in] LONG Milliseconds);
    [id(14), helpstring("Checks that the resolution of the primary monitor matches the resolution passed in")] HRESULT CheckResolution([in] LONG Width, [in] LONG Height , [out,retval] VARIANT_BOOL * pVal );
    [id(15), helpstring("Checks that the current resolution of the virtual screen matches the resolution passed in as well as the origin location")] HRESULT CheckVirtualResolution([in] LONG Width, [in] LONG Height , [in] LONG xOrigin, [in] LONG yOrigin, [out,retval] VARIANT_BOOL * pVal );
    [id(16), helpstring("Finds the specified foreground window and sets one of its child windows as the one with focus")] HRESULT SetSpecificFocus ( [in] BSTR ForeTitle , [in] BSTR ParentTitle , [in] BSTR ChildTitle , [in] LONG Id , [out,retval] VARIANT_BOOL * pVal );
};



// TSystem

[
	coclass,
	threading("apartment"),
	support_error_info("ITSystem"),
	vi_progid("Tester.TSystem"),
	progid("Tester.TSystem.1"),
	version(1.0),
	uuid("70A9FC86-0989-4EBF-B2AE-B31F846A7797"),
	helpstring("TSystem Class"),
	default("ITSystem"),
	support_error_info("ITSystem")
]
class ATL_NO_VTABLE TSystem :
	public ITSystem
{
public:
	TSystem()
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
        return ( CComCoClass<ITSystem>::Error ( uID        ,
                                                GUID_NULL  ,
                                                hRes        ) ) ;
    }


public:

    STDMETHOD(GetForegroundTWindow)(ITWindow ** pVal);
    STDMETHOD(get_ForegroundTWindow)(ITWindow ** pVal);
    STDMETHOD(EnumTopWindows)(ITWindows ** pVal);
    STDMETHOD(get_TopTWindows)(ITWindows ** pVal);
    STDMETHOD(FindTopWindow)(BSTR szClass, BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindTopTWindow)(BSTR szClass, BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindTopWindowByTitle)(BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindTopTWindowByTitle)(BSTR szTitle, ITWindow ** pVal);
    STDMETHOD(FindTopWindowByClass)(BSTR szClass, ITWindow ** pVal);
    STDMETHOD(FindTopTWindowByClass)(BSTR szClass, ITWindow ** pVal);
    STDMETHOD(Execute)(BSTR szProgram, VARIANT_BOOL* pVal);
    STDMETHOD(Sleep)(DOUBLE Seconds);
    STDMETHOD(Pause)(LONG Milliseconds);
    STDMETHOD(CheckResolution)(LONG Width, LONG Height, VARIANT_BOOL * pVal);
    STDMETHOD(CheckVirtualResolution)(LONG Width, LONG Height, LONG xOrigin, LONG yOrigin , VARIANT_BOOL * pVal );
    STDMETHOD(SetSpecificFocus)(BSTR ForeTitle, BSTR ParentTitle, BSTR ChildTitle, LONG Id, VARIANT_BOOL * pVal );
};

