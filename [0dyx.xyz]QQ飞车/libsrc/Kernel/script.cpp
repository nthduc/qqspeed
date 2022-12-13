

#include "script.h"
#include "vfilewrap.h"
#include "variant.h"
#include "k_debug.h"

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

//add+wangqi
//[2004:6:1] [14:11]
//为了处理子table
void BeginTableElement( lua_State *ls, const char *tablename )
{
	
	lua_pushstring( ls, tablename );
	lua_newtable(ls);
	
}

//add+wangqi
//[2004:6:1] [14:11]
//为了处理子table
void BeginTableElement( lua_State *ls, int tableid)
{
	
	lua_pushnumber( ls, tableid );
	lua_newtable(ls);
	
}


//add+wangqi
//[2004:6:1] [14:11]
//为了处理子table
void EndTableElement( lua_State *ls ,int index )
{
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

//add+wangqi
//[2004:5:20] [21:42]
//为了处理动态的文本数组
void SetTableElement( lua_State *ls, int key, const char *pvalue,  int index )
{

	lua_pushnumber( ls, key );
	lua_pushstring( ls, pvalue );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );

}

//add+wangqi
//[2004:5:20] [21:42]
//为了处理动态的数值数组
void SetTableElement( lua_State *ls, int key, double value,  int index )
{
	lua_pushnumber( ls, key );
	lua_pushnumber( ls, value );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, const char *pstr, int index )
{
	lua_pushstring( ls, pname );
	lua_pushstring( ls, pstr );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, int data, int index )
{
	lua_pushstring( ls, pname );
	lua_pushnumber( ls, data );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, unsigned int data, int index )
{
	lua_pushstring( ls, pname );
	lua_pushnumber( ls, data );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, long data, int index )
{
	lua_pushstring( ls, pname );
	lua_pushnumber( ls, data );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, unsigned long data, int index )
{
	lua_pushstring( ls, pname );
	lua_pushnumber( ls, data );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, double data, int index )
{
	lua_pushstring( ls, pname );
	lua_pushnumber( ls, data );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, bool b, int index )
{
	lua_pushstring( ls, pname );
	lua_pushboolean( ls, b );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

void SetTableElement( lua_State *ls, const char *pname, SCRIPT_CFUNC pfunc, int index )
{
	lua_pushstring( ls, pname );
	lua_pushcfunction( ls, pfunc );
	if( index < 0 ) index -= 2;
	lua_settable( ls, index );
}

bool GetTableElement( lua_State *ls, const char *pname, char *pbuf, int maxbuf, int index )
{
	lua_pushstring( ls, pname );
	if( index < 0 ) index --;
	lua_gettable( ls, index );

	bool bret=false;
	const char *psrc = lua_tostring( ls, -1 );
	if ( psrc )
	{
		assert( strlen(psrc) < maxbuf );
		strcpy( pbuf, psrc );
		bret=true;
	}
	lua_pop( ls, 1 );
	return bret;
}

int GetTableElementInt( lua_State *ls, const char *pname, int index )
{
	lua_pushstring( ls, pname );
	if( index < 0 ) index --;
	lua_gettable( ls, index );

	int data = (int)lua_tonumber( ls, -1 );
	lua_pop( ls, 1 );

	return data;
}

//------------------------------------------------------------------------------------------------------------
//----------------------------------------------class CScriptState--------------------------------------------
//------------------------------------------------------------------------------------------------------------
CScriptState::CScriptState() : m_bError(false), m_bNeedClose(true), m_IDAccum(0), m_nRunCoroutine(0)
{
    lua_State *pstate = lua_open();
    assert( pstate );

	/* load the optional libs */
	lua_baselibopen( pstate );
	lua_tablibopen( pstate );
	lua_iolibopen( pstate );
	lua_strlibopen( pstate );
	lua_mathlibopen( pstate );
	lua_dblibopen( pstate );

	m_pState = pstate;
	m_CoroutinePool.reserve( 256 );
}

CScriptState::CScriptState( lua_State *ls ) : m_bError(false), m_bNeedClose(false), m_IDAccum(0), m_nRunCoroutine(0)
{
    m_pState = ls;
}

CScriptState::~CScriptState()
{
    if( m_bNeedClose ) lua_close( m_pState );
}

const char *CScriptState::GetLastError()
{
	if( m_bError )
	{
		m_bError = false;
		return m_ErrorStr.c_str();	
	}
	return NULL;
}

lua_State *CScriptState::GetSysState()
{
    return m_pState;
}

static char *ReadScriptFile( const char *filename, int &datalen, bool &needdel, bool rawfile=false )
{
	const int bufsize = 4096;
	static char buffer[bufsize];
	char *pbuf = buffer;

    assert( filename && filename[0] );

	if( rawfile )
	{
		FILE *fp = fopen( filename, "rb" );
		if( !fp ) return 0;

		fseek( fp, 0, SEEK_END );
		datalen = ftell(fp);
		fseek( fp, 0, SEEK_SET );
		if( datalen+1 > bufsize ) pbuf = new char[datalen+1];
		fread( pbuf, datalen, 1, fp );
		fclose(fp);
	}
	else
	{
		FILE_V* fp = vfopen( filename, "r" );
		if( !fp )
		{
			ReportError( "%s open error", filename );
			return 0;
		}
		datalen = vfsize(fp);
		if( datalen+1 > bufsize ) pbuf = new char[datalen+1];
		vfread( pbuf, datalen, 1, fp );
		vfclose(fp);
	}
	pbuf[datalen] = 0;

	if( pbuf != buffer ) needdel = true;
	else needdel = false;

	return pbuf;
}

int CScriptState::ProtectDoBuffer( const char *pbuf, int datalen, const char *pname )
{
	int status = luaL_loadbuffer( m_pState, pbuf, datalen, pname );
	if( status == 0 ) status = lua_pcall( m_pState, 0, LUA_MULTRET, 0 );

	if( status )
	{
		m_bError = true;
		m_ErrorStr = lua_tostring(m_pState, -1);
		MessageBox(NULL, m_ErrorStr.c_str(), "脚本错误", MB_OK | MB_ICONHAND);
		lua_pop( m_pState, 2 );
		assert(0);
		return -2;
	}

	return 0;
}

int CScriptState::DoRawFile( const char *filename ) //not use vfile system
{
	bool needdel;
	int datalen;
	char *pbuf = ReadScriptFile( filename, datalen, needdel, true );

	int ret = ProtectDoBuffer( pbuf, datalen, filename );
	if( needdel ) delete[] pbuf;
	return ret;
}

int CScriptState::DoFile( const char *filename )
{
	bool needdel;
	int datalen;
	char *pbuf = ReadScriptFile( filename, datalen, needdel, false );

	int ret = ProtectDoBuffer( pbuf, datalen, filename );
	if( needdel ) delete[] pbuf;
	return ret;
}

int CScriptState::DoString( const char *pstr )
{
	if( pstr == 0 )
	{
		m_bError = true;
		m_ErrorStr = "null string when call DoString()";
		return -2;
	}
	return ProtectDoBuffer( pstr, strlen(pstr), "dostring" );
}

void CScriptState::RegisterFunc( const char *name, SCRIPT_CFUNC pfunc )
{
    assert( name && name[0] );
    assert( pfunc );
    lua_register( GetSysState(), name, pfunc );
}

void CScriptState::UnRegisterFunc( const char *name )
{
    assert( name && name[0] );
    lua_register( GetSysState(), name, NULL );
}

VarType CScriptState::GetStackTopType()
{
    lua_State *ls = GetSysState();
    int type = lua_type( ls, -1 );

    if( type == LUA_TNIL ) return VAR_NIL;
    else if( type == LUA_TNUMBER ) return VAR_FLOAT;
    else if( lua_isstring(ls, -1) ) return VAR_STRING;
	else if( lua_isfunction(ls, -1) ) return VAR_FUNC;
    else if( lua_istable(ls, -1) )
	{
		lua_rawgeti( ls, -1, 1 );
		type = lua_type(ls, -1);
		lua_pop(ls, 1);
		if( type == LUA_TNIL )
		{
			return VAR_TABLE;
/*
			lua_pushnil( ls );
			if( lua_next(ls, -2) ) //空的table当作array
			{
				lua_pop( ls, 2 );
				return VAR_TABLE;
			}
			else return VAR_ARRAY;
*/
		}
		else return VAR_ARRAY;
	}
    else return VAR_UNKNOWN;
}

static int report( lua_State *ls, int status)
{
	const char *msg;
	if( status ) 
	{
		msg = lua_tostring(ls, -1);
		if (msg == NULL) msg = "(error with no message)";
		assert(0);
		lua_pop(ls, 1);
		exit(0);
	}
	return status;
}

static int CallFunctionOnStack( lua_State *ls, void *userdata, int basearg )
{
	int nargs = basearg;
	if( userdata )
	{
		lua_pushlightuserdata( ls, userdata );
		nargs++;
	}
	int ret = lua_pcall( ls, nargs, 1, 0 );
	report( ls, ret );

	ret = lua_tonumber( ls, -1 );
	lua_pop( ls, 1 );

	return ret;
}

int  CScriptState::CallObjectMethod( const char *objname, const char *methodname, void *userdata )
{
	lua_State *ls = GetSysState();

	lua_getglobal( ls, objname );
	if( !lua_istable(ls, -1)) return 0;
//	assert( lua_istable(ls, -1) );
	lua_pushstring( ls, methodname );
	lua_rawget( ls, -2 );
	if( !lua_isfunction(ls, -1)) return 0;
//	assert( lua_isfunction(ls, -1) );   //get function

	lua_pushvalue( ls, -2 );   //push object
	int ret = CallFunctionOnStack( ls, userdata, 1 );

	lua_pop( ls, 1 );

	return ret;
}

int  CScriptState::CallGlobalFunction( const char *funcname, void *userdata )
{
	lua_State *ls = GetSysState();

	lua_getglobal( ls, funcname );
	return CallFunctionOnStack( ls, userdata, 0 );
}

bool CScriptState::IsGlobalFuncExist( const char *funcname )
{
	lua_State *ls = GetSysState();
	lua_getglobal( ls, funcname );

	bool ret = lua_isfunction(ls, -1);
	lua_pop(ls, 1);

	return ret;
}

bool CScriptState::IsObjectHaveMethod( const char *objname, const char *methodname )
{
	lua_State *ls = GetSysState();

	lua_getglobal( ls, objname );
	assert( lua_istable(ls, -1) );
	lua_pushstring( ls, methodname );

	lua_rawget( ls, -2 );
	bool ret = lua_isfunction(ls, -1);

	lua_pop( ls, 2 );

	return ret;
}

int CScriptState::AccquireCoroutineSlot()
{
	int i;
	for( i=0; i<m_CoroutinePool.size(); i++ )
	{
		if( m_CoroutinePool[i].pthread == 0 ) break;
	}
	if( i == m_CoroutinePool.size() )
	{
		assert( m_CoroutinePool.size() < 256 ); //256个已经够了, 否则可能引起错误，在update中间Accquire
		m_CoroutinePool.resize( m_CoroutinePool.size()+1 );
	}

	m_CoroutinePool[i].id = m_IDAccum++;
	m_CoroutinePool[i].pthread = lua_newthread( GetSysState() );

	return i;
}

//是否结束
static bool StartCoroutineOnStack( lua_State *ls, void *userdata, int basearg )
{
	int nargs = basearg;
	if( userdata )
	{
		lua_pushlightuserdata( ls, userdata );
		nargs++;
	}
	int ret = lua_resume( ls, nargs );
	report( ls, ret );
	if( lua_isnumber(ls, -1) ) return true;
	return false;
}

int CScriptState::CallObjectCoroutine( const char *objname, const char *methodname, void *userdata )
{
	int index = AccquireCoroutineSlot();
	lua_State *ls = m_CoroutinePool[index].pthread;

	lua_getglobal( ls, objname );
	assert( lua_istable(ls, -1) );
	lua_pushstring( ls, methodname );
	lua_rawget( ls, -2 );
	assert( lua_isfunction(ls, -1) );   //get function

	lua_pushvalue( ls, -2 );   //push object
	if( StartCoroutineOnStack(ls, userdata, 1) ) m_CoroutinePool[index].pthread = 0;
	else m_nRunCoroutine++;

	return m_CoroutinePool[index].id;
}

int CScriptState::CallGlobalCoroutine( const char *funcname, void *userdata )
{
	int index = AccquireCoroutineSlot();
	lua_State *ls = m_CoroutinePool[index].pthread;

	lua_getglobal( ls, funcname );
	if( StartCoroutineOnStack(ls, userdata, 0) ) m_CoroutinePool[index].pthread = 0;
	else m_nRunCoroutine++;

	return m_CoroutinePool[index].id;
}

int CScriptState::CallGlobalCoroutine( const char *funcname, int param1, int param2, int param3, int param4 )
{
	int index = AccquireCoroutineSlot();
	lua_State *ls = m_CoroutinePool[index].pthread;

	lua_getglobal( ls, funcname );
	lua_pushnumber( ls, param1 );
	lua_pushnumber( ls, param2 );
	lua_pushnumber( ls, param3 );
	lua_pushnumber( ls, param4 );
	if( StartCoroutineOnStack(ls, NULL, 4) ) m_CoroutinePool[index].pthread = 0;
	else m_nRunCoroutine++;

	return m_CoroutinePool[index].id;
}
int CScriptState::CallCoroutineFile( const char *filename )
{
	int index = AccquireCoroutineSlot();
	lua_State *ls = m_CoroutinePool[index].pthread;

	bool needdel;
	int datalen;
	char *pbuf = ReadScriptFile( filename, datalen, needdel );

	int ret = luaL_loadbuffer( ls, pbuf, datalen, filename );
	assert( ret == 0 );
	if( needdel ) delete[] pbuf;

	if( StartCoroutineOnStack(ls, 0, 0) ) m_CoroutinePool[index].pthread = 0;
	else m_nRunCoroutine++;


	return m_CoroutinePool[index].id;
}

int CScriptState::CallCoroutineString( const char *pstr )
{
	int index = AccquireCoroutineSlot();
	lua_State *ls = m_CoroutinePool[index].pthread;

	int ret = luaL_loadbuffer( ls, pstr, strlen(pstr), pstr );
	assert( ret == 0 );

	if( StartCoroutineOnStack(ls, 0, 0) ) m_CoroutinePool[index].pthread = 0;
	else m_nRunCoroutine++;

	return m_CoroutinePool[index].id;
}

int CScriptState::GetCoroutineState( int id ) //0--运行中， 1--完成
{
	for( int i=0; i<m_CoroutinePool.size(); i++ )
	{
		stCoroutine &co = m_CoroutinePool[i];
		if( co.id==id && co.pthread ) return 0;
	}
	return 1;
}

int CScriptState::GetNumRunCoroutine()
{
	return m_nRunCoroutine;
}

void CScriptState::Update()
{
	for( int i=0; i<m_CoroutinePool.size(); i++ )
	{
		stCoroutine &co = m_CoroutinePool[i];
		if( co.pthread == 0 ) continue;

		int ret = lua_resume( co.pthread, 0 );
/*		report( co.pthread, ret );
		assert( ret == 0 );
#ifdef _DEBUG
		if( ret )
		{
			const char *perror = lua_tostring( co.pthread, -1 );
		}
#endif*/
		if( ret!=0 || lua_isnumber(co.pthread, -1) )
		{
			co.pthread = 0;
			m_nRunCoroutine--;
		}
	}
}
