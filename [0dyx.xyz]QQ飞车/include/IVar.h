#pragma once

class IRef;
class IScriptState;
class IVarManager;
	
class IVar : public IRef
{
public:    
	virtual const char *GetName() = 0;

    virtual VarType GetType() = 0;
    virtual void GetFromScript( IScriptState *pstate ) = 0;
    virtual void SetToScript( IScriptState *pstate ) = 0;

    virtual void GetFromStack( IScriptState *pstate ) = 0;
    virtual void SetToStack( IScriptState *pstate ) = 0;

    virtual bool IsError() = 0;

	virtual int     Integer() = 0;
	virtual double  Double() = 0;
	virtual float   Float() = 0;
	virtual const char *String() = 0;
	virtual IVar    *Element( int i ) = 0;
	virtual IVar    *Element( const std::string &name ) = 0;
	//add+wangqi
	//[2004:18:2] [22:38]
	virtual int StringArray( const char* *pbuf, int maxNum ) = 0;
	virtual int IntegerArray( int *pbuf, int maxNum ) = 0;
	virtual int FloatArray( float *pbuf, int maxNum ) = 0;
};

class IVarFloat : public IVar
{
public:
    virtual double operator=( double d ) = 0;
    virtual operator double() = 0;

    virtual void GetFromScript( IScriptState *pstate ) = 0;
    virtual void SetToScript( IScriptState *pstate ) = 0;

    virtual void GetFromStack( IScriptState *pstate ) = 0;
    virtual void SetToStack( IScriptState *pstate ) = 0;
};

class IVarString : public IVar
{
public:
    virtual const char * operator=( const char *p ) = 0;
    virtual operator const char *() const = 0;
};

typedef std::vector<IVar *> TVarVector;
typedef std::map<std::string, IVar *> TVarMap;


class IVarArray : public IVar
{
public:
    virtual int   GetNum() = 0;
    virtual IVar *operator[]( int i ) = 0;
};

class IVarTable : public IVar
{
public:
	virtual int GetNum() = 0;
	virtual void BeginIterate() = 0;
	/**
	 * \param pname 元素的名字
	 * \param pvar  元素本身
     * \return 监测到是否有元素
	 */
	virtual bool Iterate( const char *&pname, IVar *&pvar ) = 0;
    virtual IVar *operator[]( const std::string &name ) = 0;
};

class IVarManager	
{
public:
	virtual IVar * Create(VarType var, LPCSTR lpcstrVarName, LPCSTR lpcstrFilePath) = 0;	
};