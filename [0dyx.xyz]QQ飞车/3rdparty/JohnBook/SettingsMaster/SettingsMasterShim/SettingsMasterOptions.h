/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
John Robbins

Code Originally By Leo Notenboom Feb 2002 MSDN Magazine.
Thanks for letting me use it, Leo!
----------------------------------------------------------------------*/
// SettingsMasterOptions.h : Declaration of the CSettingsMasterOptions
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "SettingsMasterShim.h"

// CSettingsMasterOptions
class ATL_NO_VTABLE CSettingsMasterOptions :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<EnvDTE::IDTToolsOptionsPage, &EnvDTE::IID_IDTToolsOptionsPage, &EnvDTE::LIBID_EnvDTE, 7, 0>,
	public IPersistStreamInitImpl<CSettingsMasterOptions>,
	public IOleControlImpl<CSettingsMasterOptions>,
	public IOleObjectImpl<CSettingsMasterOptions>,
	public IOleInPlaceActiveObjectImpl<CSettingsMasterOptions>,
	public IViewObjectExImpl<CSettingsMasterOptions>,
	public IOleInPlaceObjectWindowlessImpl<CSettingsMasterOptions>,
	public ISupportErrorInfo,
	public IPersistStorageImpl<CSettingsMasterOptions>,
	public ISpecifyPropertyPagesImpl<CSettingsMasterOptions>,
	public IQuickActivateImpl<CSettingsMasterOptions>,
	public IDataObjectImpl<CSettingsMasterOptions>,
	public IProvideClassInfo2Impl<&CLSID_SettingsMasterOptions, NULL, &LIBID_SettingsMasterShimLib>,
	public CComCoClass<CSettingsMasterOptions, &CLSID_SettingsMasterOptions>,
	public CComCompositeControl<CSettingsMasterOptions>
{
public:

	CSettingsMasterOptions()
	{
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_SETTINGSMASTEROPTIONS)

BEGIN_COM_MAP(CSettingsMasterOptions)
	COM_INTERFACE_ENTRY(EnvDTE::IDTToolsOptionsPage)
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

BEGIN_PROP_MAP(CSettingsMasterOptions)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CSettingsMasterOptions)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	CHAIN_MSG_MAP(CComCompositeControl<CSettingsMasterOptions>)
END_MSG_MAP()

	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);

BEGIN_SINK_MAP(CSettingsMasterOptions)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CSettingsMasterOptions>::OnAmbientPropertyChange(dispid);
	}
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_ISettingsMasterOptions,
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

// ISettingsMasterOptions

	enum { IDD = IDD_SETTINGSMASTEROPTIONS };

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct() ;

	void FinalRelease()
	{
	}

	CComPtr<EnvDTE::_DTE> m_pDTE;

	STDMETHOD(OnAfterCreated)(EnvDTE::_DTE *DTEObject)
	{
		m_pDTE = DTEObject;
		CComQIPtr<EnvDTE::IDTToolsOptionsPage> pToolsOptions(m_varUnwrappedObject.punkVal);
		return pToolsOptions->OnAfterCreated(DTEObject);
	}

    STDMETHOD(GetProperties)(IDispatch **PropertiesObject)
	{
		CComQIPtr<EnvDTE::IDTToolsOptionsPage> pToolsOptions(m_varUnwrappedObject.punkVal);
		return pToolsOptions->GetProperties(PropertiesObject);
	}

    STDMETHOD(OnOK)()
	{
		CComQIPtr<EnvDTE::IDTToolsOptionsPage> pToolsOptions(m_varUnwrappedObject.punkVal);
		return pToolsOptions->OnOK();
	}

    STDMETHOD(OnCancel)()
	{
		CComQIPtr<EnvDTE::IDTToolsOptionsPage> pToolsOptions(m_varUnwrappedObject.punkVal);
		return pToolsOptions->OnCancel();
	}

    STDMETHOD(OnHelp)()
	{
		CComQIPtr<EnvDTE::IDTToolsOptionsPage> pToolsOptions(m_varUnwrappedObject.punkVal);
		return pToolsOptions->OnHelp();
	}

	STDMETHOD(HostUserControl)(BSTR Assembly, BSTR Class, IUnknown** ppControlObject);
	void Reset();
	CComPtr<ICorRuntimeHost> m_pHost;
	CComPtr<mscorlib::_AppDomain> m_pDefaultDomain;
	CComPtr<mscorlib::_ObjectHandle> m_pObjHandle;
	CComVariant m_varUnwrappedObject;
	HWND m_hWndForm;
};

OBJECT_ENTRY_AUTO(__uuidof(SettingsMasterOptions), CSettingsMasterOptions)
