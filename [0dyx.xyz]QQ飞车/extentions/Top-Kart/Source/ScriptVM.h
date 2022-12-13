#pragma once
#pragma warning(disable:4530)
#include <stdarg.h>
#include <vector>

struct lua_State;

class ScriptVM
{
public:
	ScriptVM(void);
	virtual ~ScriptVM(void);
	bool	Init(void);
	void	Destroy(void);
	void	ExecuteScriptFile(const char * sScriptName,bool bOnlyFirstTime = false);
	void	ExecuteScript(const char * sScript);
	void	ExecuteScriptFunc(const std::vector<const char *>& modules, const char * func, bool bAllowNonexist, const char * sig, ...);
	void	ExposeGlobalUserdata(void * va, const char * name, const char * type);
	void *	GetGlobalUserdata(const char * name, const char * verify_type = NULL);
	void *	GetUserdata(const std::vector<const char *>& modules, const char * name, const char * verify_type = NULL);
	void	OnEndofUpdate(); 
	void	SetDirty(bool bDirty){m_bIsDirty = bDirty;}
	lua_State * GetLuaState();    

protected:
	lua_State* L;
	bool	m_bIsDirty;
};

//tolua_begin
void lua_break(const char * sDebugInfo); //break point
//tolua_end
