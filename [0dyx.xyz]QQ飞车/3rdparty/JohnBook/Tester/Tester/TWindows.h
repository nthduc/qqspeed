/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#pragma once
#include "resource.h"       // main symbols
#include "Helpers.h"

// A small wrapper on the CSimpleArray class to give me a begin and
// end method.  CSimpleArray does not support getting the element
// address one after the last element needed by the CComEnum::Init
// method, which is necessary to copy the data when the enumerator
// wants it.
template <class T, class TEqual = CSimpleArrayEqualHelper< T > >
class CBetterSimpleArray : public CSimpleArray< T , TEqual >
{
public      :

    T& begin ( )
    {
        ATLASSERT ( m_nSize > 0 ) ;
        return ( m_aT[0] ) ;
    }

    T& end ( )
    {
        ATLASSERT ( m_nSize > 0 ) ;
        return ( m_aT[m_nSize] ) ;
    }
} ;

// The typedef for the array ITWindows keeps everything in.
typedef CBetterSimpleArray< CComPtr< ITWindow > > TWindowArray ;

// ITWindows
[
	object,
	uuid("6920B655-667E-4B29-A769-A626DD740F42"),
	dual,	helpstring("ITWindows Interface"),
	pointer_default(unique),
]
__interface ITWindows : IDispatch
{
    [propget, id(1), helpstring("Returns the number of items in the collection")]
    HRESULT Count([out, retval] LONG* RetVal);
    [propget, id(DISPID_VALUE), helpstring("Given an index, returns an item in the collection.")]
    HRESULT Item([in] LONG Index, [out, retval] ITWindow ** RetVal);
    [propget, restricted, id(DISPID_NEWENUM),helpstring("returns an enumerator for the collection.")]    // Must be propget.
    HRESULT _NewEnum([out, retval] IUnknown** RetVal);
    [id(2) , helpstring ( "Adds an HWND or TWindow to the collection and returns the TWindow")]
    HRESULT Add ([in] VARIANT* item, [out, retval] ITWindow ** RetVal) ;
    [id(4) , helpstring ( "Removes the TWindow at the given index")]
    HRESULT Remove([in] LONG Index, [out, retval] ITWindow ** RetVal);
} ;

// TWindows
[
	coclass,
	threading("apartment"),
	support_error_info("ITWindows"),
	vi_progid("Tester.TWindows"),
	progid("Tester.TWindows.1"),
	version(1.0),
	uuid("8A4AF30A-D1DE-4123-9A91-E52861E7FEA7"),
	helpstring("The collection class that handles TWindow objects"),
    default("ITWindows")
]
class ATL_NO_VTABLE TWindows :
	public ITWindows ,
    public ISupportErrorInfoImpl< &__uuidof (TWindows) >
	
{
public:
	TWindows()
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
    TWindowArray m_aTWinArray ;
    
    // Wrapper around Error. Their order is all wrong!
    HRESULT Error ( UINT uID , HRESULT hRes = E_UNEXPECTED )
    {
        return ( CComCoClass<ITWindows>::Error ( uID        ,
                                                 GUID_NULL  ,
                                                 hRes        ) ) ;
    }

public:

    STDMETHOD(get_Count)(LONG* pVal);
    STDMETHOD(get_Item)(long Index, ITWindow ** retval);
    STDMETHOD(get__NewEnum)(IUnknown** retval);
    STDMETHOD(Add)(VARIANT* item, ITWindow ** RetVal);
    STDMETHOD(Remove)(LONG Index, ITWindow ** RetVal);
    
};

