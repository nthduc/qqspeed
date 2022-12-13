#include "Top-KartPCH.h"
#include <string.h>
#include "NiOS.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "ScriptVM.h"

#include "tolua++.h"
#include "Utility.h"

#define DEBUG_STACK 0

static void tolua_open_binding(lua_State * L);
static void stackDump(lua_State *L);

ScriptVM::ScriptVM(void)
{
	m_bIsDirty = true;
}

ScriptVM::~ScriptVM(void)
{
}

bool ScriptVM::Init(void)
{
	freopen("stdout.log", "w", stdout);
	freopen("stderr.log", "w", stderr);

	L = lua_open();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_math(L);
	luaopen_string(L);
	luaopen_debug(L);

	tolua_open_binding(L);

	lua_dofile(L,"Scripts\\startVM.lua");
	return true;
}

void ScriptVM::Destroy(void)
{
	lua_close(L);
	fflush(stdout);
	fflush(stderr);
}
void ScriptVM::ExecuteScriptFile(const char * sScriptName,bool bOnlyFirstTime)
{
	//lua_dofile(L,sScriptName);
	int nSize1 = lua_gettop(L);
	//get chunk name as modified script name
	std::string sChunkName(sScriptName);
	for(unsigned int i=0; i< sChunkName.length(); i++)
	{
		if(sChunkName[i] == '/' || sChunkName[i] == '.')
			sChunkName[i] = '_';
	}

	//get the chunk global

	if(m_bIsDirty || !bOnlyFirstTime)
	{
		luaL_loadfile(L, sScriptName);
		lua_setglobal(L, sChunkName.c_str());
		lua_getglobal(L, sChunkName.c_str());
		m_bIsDirty = false;
	}
	else
	{

		lua_getglobal(L, sChunkName.c_str());
		if( !lua_isfunction(L, -1) )//if not got,
		{
			//load it first
			luaL_loadfile(L, sScriptName);
			lua_setglobal(L, sChunkName.c_str());
			lua_getglobal(L, sChunkName.c_str());
		}
		else
		{
			if(bOnlyFirstTime)
				return;
		}
	}

	lua_pcall(L, 0, 0, 0);
	lua_settop(L, nSize1);

}

void ScriptVM::ExecuteScript(const char * sScript)
{
	lua_dostring(L, sScript);
}

lua_State * ScriptVM::GetLuaState()
{
	return L;
}

/*
 *	Execute Script Function func in the script. copy/pasted from the book "programming in LUA"
 */
static void error(lua_State *L, const char * pacFmt, ...)
{
#if defined(_DEBUG)
	char acTemp[2048];

	va_list args;
	va_start(args, pacFmt);
	_vsnprintf(acTemp, 2048, pacFmt, args);
	va_end(args);
	Log(acTemp);
#endif  //#if defined(_DEBUG)
}

void ScriptVM::ExecuteScriptFunc(const std::vector<const char *>&modules, const char * func, bool bAllowNonexist, const char * sig, ...)
{
	int nSize1 = lua_gettop(L);

	//debug
#if DEBUG_STACK
	printf("debug lua: stack size before ExecuteScriptFunc = %d\n", nSize1);
#endif

	va_list	vl;
	int	narg, nres;	/* number of arguments and results */
	va_start(vl, sig);

	//get the actual function
	if(modules.empty()) //func is global
	{
		lua_getglobal(L, func);
	}
	else
	{
		//trace down the modules
		std::vector<const char *>::const_iterator it = modules.begin();
		//get the global module name or the actual function name if there is no module
		lua_getglobal(L, *it);
		if(!lua_istable(L, -1))
		{
			error(L, "ExecuteScriptFunc: Invalid table name: %s\n", *it );
			goto failed;
		}

		for( ++it; it != modules.end(); ++it)
		{
			lua_pushstring(L, *it);
			lua_gettable(L, -2);
			if(!lua_istable(L, -1))
			{
				error(L, "ExecuteScriptFunc: Invalid table name: %s\n", *it );
				goto failed;
			}
		}
		//get the func
		lua_pushstring(L, func);
		lua_gettable(L, -2);
		if(!lua_isfunction(L, -1))
		{
			if(!bAllowNonexist)
				error(L, "ExecuteScriptFunc: Invalid function name: %s\n", func);
			goto failed;
		}
	}

	/* push	arguments */
	narg = 0;
	while (*sig){ /* push arguments	*/
		switch (*sig++){
			case 'd': /* double	argument */
				lua_pushnumber(L, va_arg(vl, double));
				break;
			case 'i': /* int argument */
				lua_pushnumber(L, va_arg(vl, int));
				break;
			case 's': /* string	argument */
				lua_pushstring(L, va_arg(vl, char *));
				break;
			case 'b': /* boolean argument */
				lua_pushboolean(L, va_arg(vl, bool));
				break;
			case 'u': /* light user data */
				lua_pushlightuserdata(L, va_arg(vl, void *));
				break;
			case '>':
				goto endwhile;
			default:
				error(L, "invalid option (%c)\n",	*(sig -	1));
				goto failed;
		}
		narg++;
		luaL_checkstack(L, 1, "too many	arguments");
	}endwhile:
	/* do the call */
	nres = strlen(sig);	/* number of expected results */
	if (lua_pcall(L, narg, nres, 0)	!= 0) /* do	the	call */
	{
		error(L, "error	running	function `%s': %s\n",
		func, lua_tostring(L, -1));
		goto failed;
	}
	/* retrieve	results	*/
	nres = -nres; /* stack index of	first result */
	while (*sig){ /* get results */
		switch (*sig++){
			case 'd': /* double	result */
				if (!lua_isnumber(L, nres))
					error(L, "wrong	result type\n");
				*va_arg(vl,	double *) =	lua_tonumber(L,	nres);
				break;
			case 'i': /* int result	*/
				if (!lua_isnumber(L, nres))
					error(L, "wrong	result type\n");
				*va_arg(vl,	int	*) = (int)lua_tonumber(L, nres);
				break;
			case 's': /* string	result */
				if (!lua_isstring(L, nres))
					error(L, "wrong	result type\n");
				*va_arg(vl,	const char **) = lua_tostring(L, nres);
				break;
			case 'b': /* boolean argument */
				if (!lua_isboolean(L, nres))
					error(L, "wrong	result type\n");
				*va_arg(vl,	bool *) = ( 0 != lua_toboolean(L, nres));
				break;
			case 'u': /* light user data */
				if (!lua_isuserdata(L, nres))
					error(L, "wrong	result type\n");
				*va_arg(vl,	void **) = lua_touserdata(L, nres);
				break;
			default:
				error(L, "invalid option (%c)\n",	*(sig -	1));
		}
		nres++;
	}

failed:
	va_end(vl);
	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	int nSize2 = lua_gettop(L);
	printf("debug lua: stack size after ExecuteScriptFunc = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
}

void ScriptVM::ExposeGlobalUserdata( void * va, const char * name, const char * type)
{
	int nSize1 = lua_gettop(L);
#if DEBUG_STACK
	//debug
	printf("debug lua: stack size before ExposeGlobalUserdata = %d\n", nSize1);
#endif

	tolua_pushusertype(L, va, type);
	lua_setglobal(L, name);

	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	int nSize2 = lua_gettop(L);
	printf("debug lua: stack size after ExposeGlobalUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
}

void * ScriptVM::GetGlobalUserdata(const char * name, const char * verify_type /*= NULL*/)
{
	int nSize1 = lua_gettop(L);
#if DEBUG_STACK
	//debug
	printf("debug lua: stack size before GetGlobalUserdata = %d\n", nSize1);
#endif
	
	lua_getglobal(L, name);

	//verify type
	if(verify_type)
	{
		tolua_Error tolua_err;
		if (
			!tolua_isusertype(L,1,verify_type,0,&tolua_err) ||
			!tolua_isnoobj(L,2,&tolua_err)
			)
		{
			tolua_error(L,"#ferror in function 'gbScriptVM::GetGlobalUserdata'.",&tolua_err);
			goto failed;
		}
	}

	void * pRet = tolua_tousertype(L, -1, 0);
	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	int nSize2 = lua_gettop(L);
	printf("debug lua: stack size after GetGlobalUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
	return pRet;

failed:
	//lua_settop(L,0);
	lua_settop(L, nSize1);
	return NULL;
}

void * ScriptVM::GetUserdata(const std::vector<const char *>& modules, const char * name, const char * verify_type/*= NULL*/)
{
	int nSize1 = lua_gettop(L);
#if DEBUG_STACK
	printf("debug lua: stack size before GetUserdata = %d\n", nSize1);
#endif
	if(modules.empty()) //userdata is global
	{
		lua_getglobal(L, name);
	}
	else
	{
		//trace down the modules
		std::vector<const char *>::const_iterator it = modules.begin();
		//get the global module name or the actual function name if there is no module
		lua_getglobal(L, *it);
		if(!lua_istable(L, -1))
		{
			error(L, "GetUserdata: Invalid table name: %s\n", *it );
			goto failed;
		}

		for( ++it; it != modules.end(); ++it)
		{
			lua_pushstring(L, *it);
			lua_gettable(L, -2);
			if(!lua_istable(L, -1))
			{
				error(L, "GetUserdata: Invalid table name: %s\n", *it );
				goto failed;
			}
		}
		//get the data
		lua_pushstring(L, name);
		lua_gettable(L, -2);
	}

	void * pRet = tolua_tousertype(L, -1, 0);
	//clear the stack
	lua_settop(L, nSize1);

#if DEBUG_STACK
	//debug
	int nSize2 = lua_gettop(L);
	printf("debug lua: stack size after GetUserdata = %d\n", nSize2);
	if(nSize1 != nSize2)
		stackDump(L);
#endif
	return pRet;

failed:
	lua_settop(L, nSize1);
	return NULL;
}

void ScriptVM::OnEndofUpdate()
{
	int nCount = lua_getgccount(L);
	lua_setgcthreshold(L, 0);
	lua_setgcthreshold(L, 100000);
}

static void stackDump (lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN: /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER: /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
		default: /* other values */
			printf("%s", lua_typename(L, t));
			break;
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}

extern int tolua_ScripFunctions_open (lua_State* tolua_S);
void tolua_open_binding(lua_State * L)
{
	tolua_ScripFunctions_open(L);
}

void lua_break(const char * sDebugInfo)
{
	OutputDebugString(sDebugInfo);
	DebugBreak();
}