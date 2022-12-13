
#include "variant.h"
#include "k_debug.h"
extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

CVar::CVar( const char *name ) 
        : m_bError(false)
{
	if( name ) m_Name = name;
}

CVar::~CVar()
{
}

int     CVar::Integer()
{
	assert( GetType() == VAR_FLOAT );
	return int(double(*(CVarFloat *)this));
}

double  CVar::Double()
{
	assert( GetType() == VAR_FLOAT );
	return double(*(CVarFloat *)this);
}

float   CVar::Float()
{
	assert( GetType() == VAR_FLOAT );
	return float(double(*(CVarFloat *)this));
}

const char *CVar::String()
{
	assert( GetType() == VAR_STRING );
	return (const char *)(*(CVarString *)this);
}

CVar    *CVar::Element( int i )
{
	assert( GetType() == VAR_ARRAY );
	return (*(CVarArray *)this)[i];
}

CVar    *CVar::Element( const std::string &name )
{
	assert( GetType() == VAR_TABLE );
	return (*(CVarTable *)this)[name];
}

Point2D *CVar::Vector3( Point2D &vec )
{
	assert( GetType() == VAR_ARRAY );
	vec.x = Element(0)->Integer();
	vec.y = Element(1)->Integer();

	return &vec;
}

stColorQuad *CVar::ColorQuad( stColorQuad &color )
{
	assert( GetType() == VAR_ARRAY );
	color.r = (unsigned char)Element(0)->Integer();
	color.g = (unsigned char)Element(1)->Integer();
	color.b = (unsigned char)Element(2)->Integer();
	if( ((CVarArray *)this)->GetNum() == 4 ) color.a = (unsigned char)Element(3)->Integer();
	else color.a = 255;

	return &color;
}

//add+wangqi
//[2004:5:26] [17:00]
stRect *CVar::Rect( stRect &rect )
{
	assert( GetType() == VAR_ARRAY );
	
	rect.ox = Element(0)->Integer();
	rect.oy = Element(1)->Integer();
	rect.width  = Element(2)->Integer();
	rect.height = Element(3)->Integer();	
	
	
	return &rect;
}



//add+wangqi
//[2004:18:2] [22:36]
//取部分数据
//////////////////////////////////////////////////////////////////////////
int CVar::StringArray( const char* *pbuf, int maxNum )
{
	assert( GetType() == VAR_ARRAY );
	CVarArray &array = *((CVarArray *)this);
	
	//检查内存泄漏
	if(maxNum > array.GetNum())
		maxNum = array.GetNum();
	
	if(!pbuf)
		maxNum=0;

	for( int i=0; i<maxNum; i++ ) 
		pbuf[i] = array[i]->String();

	return maxNum;
	
}

int CVar::IntegerArray( int *pbuf, int maxNum )
{
	assert( GetType() == VAR_ARRAY );
	CVarArray &array = *((CVarArray *)this);
	
	//检查内存泄漏
	if(maxNum > array.GetNum())
		maxNum = array.GetNum();
	
	if(!pbuf)
		maxNum=0;

	for( int i=0; i<maxNum; i++ ) 
		pbuf[i] = array[i]->Integer();

	return maxNum;
}

int CVar::FloatArray( float *pbuf, int maxNum )
{
	assert( GetType() == VAR_ARRAY );
	CVarArray &array = *((CVarArray *)this);
	
	//检查内存泄漏
	if(maxNum > array.GetNum())
		maxNum = array.GetNum();

	if(!pbuf)
		maxNum=0;

	for( int i=0; i<maxNum; i++ ) 
		pbuf[i] = array[i]->Float();

	return maxNum;
}

//-------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------CVarFloat--------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
CVarFloat::CVarFloat( const char *name, double value ) : CVar(name), m_Value(value)
{
}

VarType CVarFloat::GetType()
{
    return VAR_FLOAT;
}

void CVarFloat::GetFromStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    if( !lua_isnumber(ls, -1) ) m_bError = true;
    else
    {
        m_bError = false;
        m_Value = lua_tonumber( ls, -1 );
    }
}

void CVarFloat::SetToStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_pushnumber( ls, m_Value );
}

void CVarFloat::GetFromScript( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    GetFromStack( pstate );
    lua_pop( ls, 1 );
}

void CVarFloat::SetToScript( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
	lua_getglobal( ls, m_Name.c_str() );
	if( !lua_isnumber(ls, -1) )
	{
        m_bError = true;
		lua_pop(ls, 1);
		return;
	}
	lua_pop(ls, 1);

    //set the value
    SetToStack( pstate );
    lua_setglobal( ls, m_Name.c_str() );
    m_bError = false;
}

//-------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------CVarFunc---------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
CVarFunc::CVarFunc( const char *name ) : CVar(name)
{
}

VarType CVarFunc::GetType()
{
	return VAR_FUNC;
}

void CVarFunc::GetFromScript( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    GetFromStack( pstate );
	lua_pop( ls, 1 );
}

void CVarFunc::SetToScript( CScriptState *pstate )
{
	assert(0);
}

void CVarFunc::GetFromStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
	m_bError = false;
    //if( !lua_isnumber(ls, -1) ) m_bError = true;
    //else
    //{
        //m_bError = false;
    //}
}

void CVarFunc::SetToStack( CScriptState *pstate )
{
	assert(0);
}

//-------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------CVarString-------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
CVarString::CVarString( const char *name ) : CVar(name)
{
}

CVarString::CVarString( const char *name, const char *value ) : CVar(name), m_Value(value)
{
}

VarType CVarString::GetType()
{
    return VAR_STRING;
}

void CVarString::GetFromStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    if( !lua_isstring(ls, -1) ) m_bError = true;
    else
    {
        m_bError = false;
        m_Value = std::string(lua_tostring(ls,-1)); 
    }
}

void CVarString::SetToStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_pushstring( ls, m_Value.c_str() );
}

void CVarString::GetFromScript( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    GetFromStack( pstate );
    lua_pop( ls, 1 );
}

void CVarString::SetToScript( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    if( !lua_isstring(ls, -1) )
    {
        m_bError = true;
        lua_pop( ls, 1 );
    }
    else
    {
        m_bError = false;
        lua_pop( ls, 1 );
        SetToStack( pstate );
        lua_setglobal( ls, m_Name.c_str() );
    }
}

//--------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------class CVarArray-----------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
CVarArray::CVarArray( const char *name ) : CVar(name)
{
}

CVarArray::~CVarArray()
{
    Reset();
}

void CVarArray::Reset()
{
    TVarVector::iterator iter;
    for( iter=m_Value.begin(); iter!=m_Value.end(); iter++ )
    {
        delete *iter;
    }
    m_Value.clear();
}

VarType CVarArray::GetType()
{
    return VAR_ARRAY;
}

void CVarArray::GetFromScript( CScriptState *pstate )
{
    Reset();

    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    GetFromStack( pstate );
    lua_pop( ls, 1 );
}

void CVarArray::SetToScript( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    if( !lua_istable(ls, -1) )
    {
        m_bError = true;
        lua_pop( ls, 1 );
        return;
    }

    lua_pop( ls, 1 );
    SetToStack( pstate );
    lua_setglobal( ls, m_Name.c_str() );
    m_bError = false;
}

void CVarArray::GetFromStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    if( !lua_istable(ls, -1) )
    {
        m_bError = true;
        return;
    }
    
    m_bError = false;
    for( int i=0; ; i++ )
    {
        lua_rawgeti( ls, -1, i+1 );
        
        CVar *pvar = 0;
        VarType type = pstate->GetStackTopType();
        if( type == VAR_NIL )
        {
            lua_pop( ls, 1 );
            break;
        }

        switch( type )
        {
        case VAR_FLOAT:
            pvar = new CVarFloat(NULL);
            break;
        case VAR_STRING:
            pvar = new CVarString(NULL);
            break;
        case VAR_ARRAY:
            pvar = new CVarArray(NULL);
            break;
        case VAR_TABLE:
            pvar = new CVarTable(NULL);
            break;
		case VAR_FUNC:
			pvar = new CVarFunc(NULL);
			break;
        default:
            m_bError = true;
            break;
        }
        if( pvar )
        {
            pvar->GetFromStack( pstate );
            if( pvar->IsError() ) m_bError = true;
            m_Value.push_back( pvar );
        }
        lua_pop( ls, 1 );
    }
}

void CVarArray::SetToStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();

    for( int i=0; i<m_Value.size(); i++ )
    {
        CVar *pvar = m_Value[i];
        assert(pvar);

        pvar->SetToStack( pstate );
        lua_rawseti( ls, -1, i+1 );  //!!!-1  or -2???
    }
}

//--------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------class CVarTable-----------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
CVarTable::CVarTable( const char *name ) : CVar(name)
{
}

CVarTable::~CVarTable()
{
    Reset();
}

void CVarTable::Reset()
{
    TVarMap::iterator iter;
    for( iter=m_Value.begin(); iter!=m_Value.end(); iter++ )
    {
        delete iter->second;
    }
    m_Value.clear();
}

void CVarTable::BeginIterate()
{
	m_Iter = m_Value.begin();
}

bool CVarTable::Iterate( const char *&pname, CVar *&pvar )
{
	if( m_Iter == m_Value.end() ) return false;

	pname = m_Iter->first.c_str();
	pvar = m_Iter->second;
	m_Iter++;
	return true;
}

CVar *CVarTable::operator[]( const std::string &name )
{
    TVarMap::iterator iter = m_Value.find(name);
    if( iter != m_Value.end() ) return iter->second;
    else return NULL;
}	

VarType CVarTable::GetType()
{
    return VAR_TABLE;
}

void CVarTable::GetFromScript( CScriptState *pstate )
{
    Reset();

    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    GetFromStack( pstate );
    lua_pop( ls, 1 );
}

void CVarTable::SetToScript( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    lua_getglobal( ls, m_Name.c_str() );
    if( !lua_istable(ls, -1) )
    {
        m_bError = true;
        lua_pop( ls, 1 );
        return;
    }
    SetToStack( pstate );
    lua_setglobal( ls, m_Name.c_str() );
    m_bError = false;
}

void CVarTable::GetFromStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    if( !lua_istable(ls, -1) )
    {
        m_bError = true;
        return;
    }
    
    m_bError = false;
    lua_pushnil( ls );
    while( lua_next(ls, -2) )
    {
		char pname[256];
		int luatype = lua_type(ls, -2);
		if( luatype == LUA_TSTRING ) strcpy( pname, lua_tostring(ls,-2) );
		else if( luatype == LUA_TNUMBER ) sprintf( pname, "%d", int(lua_tonumber(ls,-2)) );
		else
        {
            m_bError = true;
            lua_pop(ls, 2);
            return;
        }

        CVar *pvar = 0;
        VarType type = pstate->GetStackTopType();
        switch( type )
        {
        case VAR_FLOAT:
            pvar = new CVarFloat(pname);
            break;
        case VAR_STRING:
            pvar = new CVarString(pname);
            break;
        case VAR_ARRAY:
            pvar = new CVarArray(pname);
            break;
        case VAR_TABLE:
            pvar = new CVarTable(pname);
            break;
		case VAR_FUNC:
			pvar = new CVarFunc(pname);
			break;
        default:
            m_bError = true;
            break;
        }
        if( pvar )
        {
            pvar->GetFromStack( pstate );
            if( pvar->IsError() ) m_bError = true;
			if ( m_Value[pname] )
			{
				delete m_Value[pname];
			}
            m_Value[pname] = pvar;
        }
        lua_pop( ls, 1 );
    }
}

void CVarTable::SetToStack( CScriptState *pstate )
{
    lua_State *ls = pstate->GetSysState();
    if( !lua_istable(ls, -1) )
    {
        m_bError = true;
        return;
    }

    TVarMap::iterator iter;
    for( iter=m_Value.begin(); iter!=m_Value.end(); iter++ )
    {
        lua_pushstring( ls, iter->first.c_str() );
        iter->second->SetToStack( pstate );
        lua_settable( ls, -3 );
    }
}


bool CVar::LoadVarsFromFile( CVar *pvar1, const char *path )
{
	return CVar::LoadVarsFromFile( &pvar1, 1, path );
}

bool CVar::LoadVarsFromFile( CVar *pvar1, CVar *pvar2, const char *path )
{
	CVar *pvarbuf[] = { pvar1, pvar2 };
	return CVar::LoadVarsFromFile( &pvarbuf[0], 2, path );
}

bool CVar::LoadVarsFromFile( CVar *pvar1, CVar *pvar2, CVar *pvar3, const char *path )
{
	CVar *pvarbuf[] = { pvar1, pvar2, pvar3 };
	return CVar::LoadVarsFromFile( &pvarbuf[0], 3, path );
}

bool CVar::LoadVarsFromFile( CVar *pvar1, CVar *pvar2, CVar *pvar3, CVar *pvar4, const char *path )
{
	CVar *pvarbuf[] = { pvar1, pvar2, pvar3, pvar4 };
	return CVar::LoadVarsFromFile( &pvarbuf[0], 4, path );
}

bool CVar::LoadVarsFromFile( CVar **pvars, int numvar, const char *path )
{
	CScriptState state;
	int ret = state.DoFile( path );
	if( ret == -2 )
	{
		assert(0);
		return false;
	}

	for( int i=0; i<numvar; i++ )
	{
		pvars[i]->GetFromScript( &state );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//add+wangqi 
//处理字符串中的变量
//////////////////////////////////////////////////////////////////////////
bool CVar::LoadVarsFromBuffer( CVar *pvar1, const char *buffer )
{
	return CVar::LoadVarsFromBuffer( &pvar1, 1, buffer );
}

bool CVar::LoadVarsFromBuffer( CVar *pvar1, CVar *pvar2, const char *buffer )
{
	CVar *pvarbuf[] = { pvar1, pvar2 };
	return CVar::LoadVarsFromBuffer( &pvarbuf[0], 2, buffer );
}

bool CVar::LoadVarsFromBuffer( CVar *pvar1, CVar *pvar2, CVar *pvar3, const char *buffer )
{
	CVar *pvarbuf[] = { pvar1, pvar2 ,pvar3};
	return CVar::LoadVarsFromBuffer( &pvarbuf[0], 3, buffer );
}

bool CVar::LoadVarsFromBuffer( CVar *pvar1, CVar *pvar2, CVar *pvar3, CVar *pvar4, const char *buffer )
{
	CVar *pvarbuf[] = { pvar1, pvar2 ,pvar3, pvar4};
	return CVar::LoadVarsFromBuffer( &pvarbuf[0], 4, buffer );
}

bool CVar::LoadVarsFromBuffer( CVar **pvars, int numvar, const char *buffer )
{
	CScriptState state;
	int ret = state.DoString( buffer );
	if( ret == -2 )
	{
		assert(0);
		return false;
	}
	
	for( int i=0; i<numvar; i++ )
	{
		pvars[i]->GetFromScript( &state );
	}
	return true;
}

CVar *CVar::CreateVarFromPath( CScriptState *pscript, const char *pname )
{
	char buffer[256];
	const char *phead = pname;
	const char *ptail = pname;
	lua_State *ls = pscript->GetSysState();
	int phrase = 0;
	bool bend;

	for(;;)
	{
		ptail = strchr( phead, '.' );
		if( ptail )
		{
			int len = ptail - phead;
			memcpy( buffer, phead, len );
			buffer[len] = 0;
			bend = false;
		}
		else
		{
			strcpy( buffer, phead );
			bend = true;
		}

		if( phrase == 0 )
		{
			lua_getglobal( ls, buffer );
			phrase++;
		}
		else
		{
			lua_pushstring( ls, buffer );
			lua_gettable( ls, -2 );
		}

		if( !bend )
		{
			if( !lua_istable(ls, -1) )
			{
				lua_pop( ls, 1 );
				return NULL;
			}
			phead = ptail + 1;
			continue;
		}

        CVar *pvar = NULL;
        VarType type = pscript->GetStackTopType();
        switch( type )
        {
        case VAR_FLOAT:
            pvar = new CVarFloat(pname);
            break;
        case VAR_STRING:
            pvar = new CVarString(pname);
            break;
        case VAR_ARRAY:
            pvar = new CVarArray(pname);
            break;
        case VAR_TABLE:
            pvar = new CVarTable(pname);
            break;
		case VAR_FUNC:
			pvar = new CVarFunc(pname);
			break;
        default:
            break;
        }
        if( pvar )
        {
            pvar->GetFromStack( pscript );
            if( pvar->IsError() )
			{
				delete pvar;
				pvar = NULL;
			}
        }
        lua_pop( ls, 1 );
		return pvar;
    }
	return NULL;
}
