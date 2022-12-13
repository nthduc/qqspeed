#ifndef __UICONFIG_I_H__
#define __UICONFIG_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IUIConfig;

	TENIO_MAKE_TYPEID_MAP(IUIConfig, COMPONENTID_UICONFIG);

	class IUIConfig : public IComponent
	{
	public:
		virtual void SetResRootPath(LPCTSTR lpszResRootPath) = 0;
		virtual void SetStyleRootPath(LPCTSTR lpszStyleRootPath) = 0;
		virtual void SetLayoutRootPath(LPCTSTR lpszLayoutRootPath) = 0;
		virtual void SetScriptRootPath(LPCTSTR lpszScriptRootPath) = 0;
		virtual void SetName2IDFileRootPath(LPCTSTR lpszName2IDFileRootPath) = 0;
		
		virtual BOOL GetResRootPath(LPTSTR lpszResRootPath, UINT nMaxLen) = 0;
		virtual BOOL GetStyleRootPath(LPTSTR lpszStyleRootPath, UINT nMaxLen) = 0;
		virtual BOOL GetLayoutRootPath(LPTSTR lpszLayoutRootPath, UINT nMaxLen) = 0;
		virtual BOOL GetScriptRootPath(LPTSTR lpszScriptRootPath, UINT nMaxLen) = 0;
		virtual BOOL GetName2IDFileRootPath(LPTSTR lpszName2IDFileRootPath, UINT nMaxLen) = 0;
	};
};

#endif

// InterfaceName:UIConfig
// SubFolderName:uilib
