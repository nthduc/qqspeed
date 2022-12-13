
#ifndef __SC_VARIANT_H__
#define __SC_VARIANT_H__

#include "script.h"
#include <map>
#include <string>



class CVar 
{
public:
    CVar( const char *name );
    virtual ~CVar();
	const char *GetName(){ return m_Name.c_str(); }

    virtual VarType GetType() = 0;
    virtual void GetFromScript( CScriptState *pstate ) = 0;
    virtual void SetToScript( CScriptState *pstate ) = 0;

    virtual void GetFromStack( CScriptState *pstate ) = 0;
    virtual void SetToStack( CScriptState *pstate ) = 0;

    bool IsError(){ return m_bError; }

	int     Integer();
	double  Double();
	float   Float();
	const char *String();
	CVar    *Element( int i );
	CVar    *Element( const std::string &name );

	//help function
	Point2D *Vector3( Point2D &vec );
	stColorQuad *ColorQuad( stColorQuad &color );
	stRect *Rect( stRect &rect );

	//add+wangqi
	//[2004:18:2] [22:38]
	int StringArray( const char* *pbuf, int maxNum );
	int IntegerArray( int *pbuf, int maxNum );
	int FloatArray( float *pbuf, int maxNum );


	static bool LoadVarsFromFile( CVar *pvar1, const char *path );
	static bool LoadVarsFromFile( CVar *pvar1, CVar *pvar2, const char *path );
	static bool LoadVarsFromFile( CVar *pvar1, CVar *pvar2, CVar *pvar3, const char *path );
	static bool LoadVarsFromFile( CVar *pvar1, CVar *pvar2, CVar *pvar3, CVar *pvar4, const char *path );
	static bool LoadVarsFromFile( CVar **pvars, int numvar, const char *path );
	
	//add+wangqi
	static bool LoadVarsFromBuffer( CVar *pvar1, const char *buffer );
	static bool LoadVarsFromBuffer( CVar *pvar1, CVar *pvar2, const char *buffer );
	static bool LoadVarsFromBuffer( CVar *pvar1, CVar *pvar2, CVar *pvar3, const char *buffer );
	static bool LoadVarsFromBuffer( CVar *pvar1, CVar *pvar2, CVar *pvar3, CVar *pvar4, const char *buffer );
	static bool LoadVarsFromBuffer( CVar **pvars, int numvar, const char *buffer );

	//从脚本运行环境得到某个变量， 变量名字可以是 aa.bb.cc....  
	static CVar *CreateVarFromPath( CScriptState *pscript, const char *pname );

protected:
    std::string m_Name;
    bool m_bError;
};

class CVarFloat : public CVar//, public IVarFloat
{
public:
    CVarFloat( const char *name, double value=0 );
    double operator=( double d ){ return double(m_Value=d); }
    operator double() const{ return double(m_Value); }

    virtual VarType GetType();
    virtual void GetFromScript( CScriptState *pstate );
    virtual void SetToScript( CScriptState *pstate );

    virtual void GetFromStack( CScriptState *pstate );
    virtual void SetToStack( CScriptState *pstate );

protected:
    double m_Value;
};

class CVarString : public CVar//, public IVarString
{
public:
    CVarString( const char *name );
    CVarString( const char *name, const char *value );
    const char * operator=( const char *p ){ m_Value=p; return p; }
    operator const char *() const{ return m_Value.c_str(); }

    virtual VarType GetType();
    virtual void GetFromScript( CScriptState *pstate );
    virtual void SetToScript( CScriptState *pstate );

    virtual void GetFromStack( CScriptState *pstate );
    virtual void SetToStack( CScriptState *pstate );

protected:
    std::string m_Value;
};

typedef std::vector<CVar *> TVarVector;
typedef std::map<std::string, CVar *> TVarMap;


class CVarArray : public CVar//, public IVarArray
{
public:
    CVarArray( const char *name );
    ~CVarArray();
    int   GetNum(){ return m_Value.size(); }
    CVar *operator[]( int i ){ return m_Value[i]; }

    virtual VarType GetType();
    virtual void GetFromScript( CScriptState *pstate );
    virtual void SetToScript( CScriptState *pstate );

    virtual void GetFromStack( CScriptState *pstate );
    virtual void SetToStack( CScriptState *pstate );

protected:
    TVarVector m_Value;

    void Reset();
};

class CVarTable : public CVar//, public IVarTable
{
public:
    CVarTable( const char *name );
    ~CVarTable();

	int GetNum(){ return m_Value.size(); }
	void BeginIterate();
	/**
	 * \param pname 元素的名字
	 * \param pvar  元素本身
     * \return 监测到是否有元素
	 */
	bool Iterate( const char *&pname, CVar *&pvar );

    CVar *operator[]( const std::string &name );

    virtual VarType GetType();
    virtual void GetFromScript( CScriptState *pstate );
    virtual void SetToScript( CScriptState *pstate );

    virtual void GetFromStack( CScriptState *pstate );
    virtual void SetToStack( CScriptState *pstate );

protected:
    TVarMap m_Value;
	TVarMap::iterator m_Iter;

    void Reset();
};

class CVarFunc : public CVar
{
public:
	CVarFunc( const char *name );

    virtual VarType GetType();
    virtual void GetFromScript( CScriptState *pstate );
    virtual void SetToScript( CScriptState *pstate );

    virtual void GetFromStack( CScriptState *pstate );
    virtual void SetToStack( CScriptState *pstate );

private:

};


#endif