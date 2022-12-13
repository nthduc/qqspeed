// VSNetToolWinShim.h : Declaration of the CVSNetToolWinShim
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "VSNetToolHostShim.h"

// CVSNetToolWinShim
class ATL_NO_VTABLE CVSNetToolWinShim :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IVSNetToolWinShim, &IID_IVSNetToolWinShim, &LIBID_VSNetToolHostShimLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CVSNetToolWinShim>,
	public IOleControlImpl<CVSNetToolWinShim>,
	public IOleObjectImpl<CVSNetToolWinShim>,
	public IOleInPlaceActiveObjectImpl<CVSNetToolWinShim>,
	public IViewObjectExImpl<CVSNetToolWinShim>,
	public IOleInPlaceObjectWindowlessImpl<CVSNetToolWinShim>,
	public ISupportErrorInfo,
	public IPersistStorageImpl<CVSNetToolWinShim>,
	public ISpecifyPropertyPagesImpl<CVSNetToolWinShim>,
	public IQuickActivateImpl<CVSNetToolWinShim>,
	public IDataObjectImpl<CVSNetToolWinShim>,
	public IProvideClassInfo2Impl<&CLSID_VSNetToolWinShim, NULL, &LIBID_VSNetToolHostShimLib>,
	public CComCoClass<CVSNetToolWinShim, &CLSID_VSNetToolWinShim>,
	public CComCompositeControl<CVSNetToolWinShim>
{
public:

	CVSNetToolWinShim()
	{
	    m_hWndForm = NULL ;
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_VSNETTOOLWINSHIM)

BEGIN_COM_MAP(CVSNetToolWinShim)
	COM_INTERFACE_ENTRY(IVSNetToolWinShim)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CVSNetToolWinShim)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CVSNetToolWinShim)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	CHAIN_MSG_MAP(CComCompositeControl<CVSNetToolWinShim>)
END_MSG_MAP()

	LRESULT OnSize ( UINT , WPARAM , LPARAM , BOOL& ) ;

BEGIN_SINK_MAP(CVSNetToolWinShim)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CVSNetToolWinShim>::OnAmbientPropertyChange(dispid);
	}
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_IVSNetToolWinShim,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IVSNetToolWinShim

	enum { IDD = IDD_VSNETTOOLWINSHIM };

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct ( ) ;
	
	void FinalRelease()
	{
	}

	STDMETHOD(HostUserControl2) ( IUnknown *pToolWindow ,
	                              BSTR Assembly         ,
	                              BSTR Class            ,
	                              BSTR SatelliteDLL     ,
	                              int ResourceID        ,
	                              IUnknown** ppControlObject ) ;
	STDMETHOD(HostUserControl) ( BSTR Assembly               ,
	                             BSTR Class                  ,
	                             IDispatch **ppControlObject ) ;
	void Reset ( ) ;
	
	CComPtr<ICorRuntimeHost> m_pHost ;
	CComPtr<mscorlib::_AppDomain> m_pDefaultDomain ;
	CComPtr<mscorlib::_ObjectHandle> m_pObjHandle ;
	CComVariant m_varUnwrappedObject ;
	HWND m_hWndForm ;
} ;

OBJECT_ENTRY_AUTO(__uuidof(VSNetToolWinShim), CVSNetToolWinShim)
