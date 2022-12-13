
#ifndef __SC_SCRIPT_H__
#define __SC_SCRIPT_H__

#include "kernel.h"

struct lua_State;
typedef int (*SCRIPT_CFUNC)( lua_State * );

enum VarType
{
    VAR_NIL,
    VAR_FLOAT,
    VAR_STRING,
    VAR_ARRAY,
    VAR_TABLE,
	VAR_FUNC,
    VAR_UNKNOWN = 0xffffffff
};

class CScriptState
{
public:
    CScriptState();
    CScriptState( lua_State *ls );
    ~CScriptState();

    int  NewThread(); //return thread id
    void DeleteThread( int id );
    void ActiveThread( int id );

	int DoRawFile( const char *filename ); //not use vfile system
    int DoFile( const char *filename );
    int DoString( const char *pstr );

    void RegisterFunc( const char *name, SCRIPT_CFUNC pfunc );
    void UnRegisterFunc( const char *name );

    VarType GetStackTopType();

    lua_State *GetSysState();

	const char *GetLastError();

	bool IsObjectHaveMethod( const char *objname, const char *methodname );
	bool IsGlobalFuncExist( const char *funcname );

	//call directly
	int  CallObjectMethod( const char *objname, const char *methodname, void *userdata=NULL );
	int  CallGlobalFunction( const char *funcname, void *userdata=NULL );

	//call coroutine, return coroutine id
	int  CallObjectCoroutine( const char *objname, const char *methodname, void *userdata=NULL );
	int  CallGlobalCoroutine( const char *funcname, void *userdata=NULL );
	int  CallGlobalCoroutine( const char *funcname, int param1, int param2=0, int param3=0, int param4=0 );
	int  CallCoroutineFile( const char *filename );
	int  CallCoroutineString( const char *pstr );
	int  GetCoroutineState( int id ); //0--运行中， 1--完成
	int  GetNumRunCoroutine();

	void Update();

protected:
    lua_State *m_pState;
	bool m_bNeedClose;
    bool m_bError;
	std::string m_ErrorStr;

	struct stCoroutine
	{
		lua_State *pthread;
		int id;
	};
	std::vector<stCoroutine>m_CoroutinePool;
	int m_IDAccum;
	int m_nRunCoroutine; //当前正在运行的coroutine

	int AccquireCoroutineSlot();
	int ProtectDoBuffer( const char *pbuf, int datalen, const char *pname );
};


//add+wangqi
//[2004:6:1] [14:11]
//为了处理子table
void BeginTableElement( lua_State *ls, const char *tablename );
void BeginTableElement( lua_State *ls, int tableid);

//add+wangqi
//[2004:6:1] [14:11]
//为了处理子table
void EndTableElement( lua_State *ls ,int index=-1 );

//add+wangqi
//[2004:5:20] [21:42]
//为了处理动态的文本数组
extern void SetTableElement( lua_State *ls, int key, const char *pvalue,  int index=-1 );

//add+wangqi
//[2004:5:20] [21:42]
//为了处理动态的数值数组
extern void SetTableElement( lua_State *ls, int key, double value,  int index=-1 );

extern void SetTableElement( lua_State *ls, const char *pname, const char *pstr, int index=-1 );
extern void SetTableElement( lua_State *ls, const char *pname, int data, int index=-1 );
extern void SetTableElement( lua_State *ls, const char *pname, unsigned int data, int index=-1 );
extern void SetTableElement( lua_State *ls, const char *pname, long data, int index=-1 );
extern void SetTableElement( lua_State *ls, const char *pname, unsigned long data, int index=-1 );
extern void SetTableElement( lua_State *ls, const char *pname, double data, int index=-1 );
extern void SetTableElement( lua_State *ls, const char *pname, SCRIPT_CFUNC pfunc, int index=-1 );
extern void SetTableElement( lua_State *ls, const char *pname, bool b, int index=-1 );

extern bool GetTableElement( lua_State *ls, const char *pname, char *pbuf, int maxbuf, int index=-1 );
extern int GetTableElementInt( lua_State *ls, const char *pname, int index=-1 );

#endif