///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32_WCE
#ifndef LUAPLUS_LIB
#define LUAPLUS_LIB
#endif LUAPLUS_LIB
#endif _WIN32_WCE

#ifdef _MSC_VER
	#ifndef LUAPLUS_LIB
		#ifdef LUAPLUS_BUILDDLL			
			#define LUA_API __declspec(dllexport)
		#endif
		#ifdef LUAPLUS_USEDLL
			#define LUA_API __declspec(dllimport)
			#if defined(_DEBUG) || defined(DEBUG)
				#pragma comment(lib, "lua5d.lib")
				#pragma comment(lib, "LuaPlusD_1081.lib")
			#else
				#pragma comment(lib, "lua5.lib")
				#pragma comment(lib, "LuaPlus_1081.lib")
			#endif
		#endif
	#else //!LUASTATEDLL
		#define LUA_API
	#endif
	#define LUAPLUS_CLASS LUA_API
#else !_MSC_VER
    #ifndef LUA_API
        #define LUA_API extern
    #endif
    #ifndef LUAPLUS_CLASS
        #define LUAPLUS_CLASS LUA_API
    #endif
#endif _MSC_VER

#define LUALIB_API LUA_API


