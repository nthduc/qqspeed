// regsvr.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "XMLRegistry.h"
#include "common.h"
#include "CoCreate.h"
#include "Network/network_i.h"
#include "Network/network_i.c"

void XMLTest1();
void XMLTest2();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CoInitialize(NULL);
	{
		/*
		{
			CXMLRegistry reg(TRUE);
			reg.UnRegisterServer("1234-5678");
		}
		{
			CXMLRegistry reg(TRUE);
			reg.RegisterServer("GraphLayer", "8888-9999", "GraphLayer.dll");
		}
		{
			CXMLRegistry reg(TRUE);
			reg.RegisterServer("Network", "9999-9999", "Network.dll");
		}
		{
			CXMLRegistry reg(TRUE);
			reg.RegisterServer("GraphLayer", "8888-9999", "GraphLayer.dll");
		}
		{
			CXMLRegistry reg(TRUE);
			reg.RegisterServer("xxx", "4444-9999", "Network.dll");
		}
		{
			CXMLRegistry reg(TRUE);
			reg.UnRegisterServer("4444-9999");
		}

		{
			CXMLRegistry reg(TRUE);
			reg.RegisterServer("GameApp", "0000-1111", "GameApp.dll");
		}
		*/
		CComPtr<INetwork> spNetwork;
		XCO::CoCreateInstance(CLSID_NetworkImpl, NULL, 0, IID_INetwork, (void**)&spNetwork);
	}
	CoUninitialize();
	return 0;
}

void XMLTest1()
{
	HRESULT hr = E_FAIL;	
	{
		CComPtr<IXMLDOMDocument> spXMLDoc;
		hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
			IID_IXMLDOMDocument, (void**)&spXMLDoc);
		KK_CHECK(hr);

		VARIANT_BOOL bSuccessed;
		hr = spXMLDoc->load(CComVariant("Registry.xml"), &bSuccessed);

		CComPtr<IXMLDOMElement> spRootElement;
		spXMLDoc->createElement(CComBSTR("Registry"), &spRootElement);
		spXMLDoc->putref_documentElement(spRootElement);

		CComPtr<IXMLDOMElement> spCLSIDElement;
		spXMLDoc->createElement(CComBSTR("CLSID"), &spCLSIDElement);

		CComPtr<IXMLDOMNode> spCLSIDNode;
		spRootElement->appendChild(spCLSIDElement, &spCLSIDNode);

		{
			CComPtr<IXMLDOMElement> spAppElement;
			CComPtr<IXMLDOMNode> spAppNode;
			spXMLDoc->createElement(CComBSTR("x8888-8888"), &spAppElement);
			spCLSIDElement->appendChild(spAppElement, &spAppNode);

			CComPtr<IXMLDOMElement> spChildElement;
			CComPtr<IXMLDOMNode> spChildNode;
			spXMLDoc->createElement(CComBSTR("InprocServer32"), &spChildElement);
			spChildElement->put_text(CComBSTR("xxx.dll"));
			spAppElement->appendChild(spChildElement, &spChildNode);

			spChildElement = NULL;
			spChildNode = NULL;
			spXMLDoc->createElement(CComBSTR("ProgID"), &spChildElement);
			spChildElement->put_text(CComBSTR("xxxProg"));
			spAppElement->appendChild(spChildElement, &spChildNode);
		}

		{
			CComPtr<IXMLDOMElement> spAppElement;
			CComPtr<IXMLDOMNode> spAppNode;
			spXMLDoc->createElement(CComBSTR("x8888-8888"), &spAppElement);
			spCLSIDElement->appendChild(spAppElement, &spAppNode);

			CComPtr<IXMLDOMElement> spChildElement;
			CComPtr<IXMLDOMNode> spChildNode;
			spXMLDoc->createElement(CComBSTR("InprocServer32"), &spChildElement);
			spChildElement->put_text(CComBSTR("www.dll"));
			spAppElement->appendChild(spChildElement, &spChildNode);

			spChildElement = NULL;
			spChildNode = NULL;
			spXMLDoc->createElement(CComBSTR("ProgID"), &spChildElement);
			spChildElement->put_text(CComBSTR("wwwProg"));
			spAppElement->appendChild(spChildElement, &spChildNode);
		}
		spXMLDoc->save(CComVariant("Registry.xml"));
	}
KK_EXIT:
	return;
}

void XMLTest2()
{
}