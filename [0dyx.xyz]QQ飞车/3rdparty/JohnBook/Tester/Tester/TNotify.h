/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#pragma once
#include "resource.h"       // main symbols
#include "TNotifyHlp.h"
#include "TWindow.h"

[export]
typedef enum
{
    ansExactMatch = 0,
    ansBeginMatch = 1,
    ansAnyLocMatch = 2
} AddNotifySearch ;

[export]
typedef enum
{
    antDestroyWindow = 1,
    antCreateWindow = 2,
    antCreateAndDestroy = 3
} AddNotifyType ;

// Forward declaration.
class TNotify ;

// The prototypes for the worker function inside TNotify.CPP.
HANDLE AddNotify ( TNotify *       tNot         ,
                   AddNotifyType   NotifyType   ,
                   AddNotifySearch NotifySearch ,
                   BSTR            Title         ) ;
void RemoveNotify ( TNotify * tNot , HANDLE hItem ) ;
void SpinTheTimerLoop ( LONG lTimes ) ;
                   


// ITNotify
[
	object,
	uuid("F49728CD-95AF-4BD7-B160-EA5116A59ACC"),
	dual,	helpstring("ITNotify Interface"),
	pointer_default(unique)
]
__interface ITNotify : IDispatch
{
    [id(1), helpstring("method AddNotification")] HRESULT AddNotification([in] AddNotifyType NotifyType , [in] AddNotifySearch SearchType , [in] BSTR WinStr, [out,retval] VARIANT_BOOL* pVal);
    [id(2), helpstring("method CheckNotification")] HRESULT CheckNotification(void);
    [id(3), helpstring("method ClearNotification")] HRESULT ClearNotification(void);
};

#ifdef CreateWindow
#undef CreateWindow
#endif
[
    dispinterface,
    uuid("698E7564-2A64-4988-8F1D-58131B6AA9CA"),
    helpstring("ITNotify Events Interface"),
]
__interface _ITNotify
{
    [id(1)] HRESULT DestroyWindow ( void ) ;
    [id(2)] HRESULT CreateWindow ( [in] ITWindow * tWin ) ;
} ;


// TNotify

[
	coclass,
	threading("apartment"),
	support_error_info("ITNotify"),
	vi_progid("Tester.TNotify"),
	progid("Tester.TNotify.1"),
	version(1.0),
	uuid("DEC3F5EB-6169-48CC-816B-1EE00BD26541"),
	helpstring("The object that handles a single window type notification"),
	support_error_info("ITNotify"),
	default("ITNotify"),
	event_source(com)
]
class ATL_NO_VTABLE TNotify :
	public ITNotify
{
public:
	TNotify()
	{
	    NotifyType = antCreateAndDestroy ;
	    hItem = INVALID_HANDLE_VALUE ;
	}

    __event __interface _ITNotify ;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease()
	{
	    if ( INVALID_HANDLE_VALUE != hItem )
	    {
	        ClearNotification ( ) ;
	    }
	}
	
	// Helper to fire the creation event.
	void RaiseCreateEvent ( ITWindow * tWin )
	{
	    __raise CreateWindow ( tWin ) ;
	}
	
	// Helper to fire the destroy event.
	void RaiseDestroyEvent ( void )
	{
	    __raise DestroyWindow ( ) ;
	}
	
	// The requested notification type for this instance.
	AddNotifyType NotifyType ;
	// The handle inside TNotifyHlp represented by this instance.
	HANDLE        hItem ;
	
    // Wrapper around Error. Their order is all wrong!
    HRESULT Error ( UINT uID , HRESULT hRes = E_UNEXPECTED )
    {
        return ( CComCoClass<ITNotify>::Error ( uID        ,
                                                GUID_NULL  ,
                                                hRes        ) ) ;
    }
	

public:
    STDMETHOD(AddNotification)(AddNotifyType NotifyType , AddNotifySearch SearchType , BSTR WinStr, VARIANT_BOOL* pVal);
    STDMETHOD(CheckNotification)(void);
    STDMETHOD(ClearNotification)(void);

};

