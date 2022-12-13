#ifndef __UILUASCRIPTMANAGER_I_H__
#define __UILUASCRIPTMANAGER_I_H__

#include "../teniointerface.h"
#include "../UILib/uiscriptmanager_i.h"

struct lua_State;
typedef int (*LPLUA_CFUNC)(lua_State *);

namespace Tenio
{
	class IUILuaScriptManager;

	TENIO_MAKE_TYPEID_MAP(IUILuaScriptManager, COMPONENTID_UILUASCRIPTMANAGER);

	class IUILuaScriptManager : public IComponent, public IUIScriptManager
	{
	public:
		virtual void RegisterFunc(LPCTSTR lpszFuncName, LPLUA_CFUNC pfunc) = 0;
		virtual void UnRegisterFunc(LPCTSTR lpszFuncName) = 0;
	};
};

#endif
