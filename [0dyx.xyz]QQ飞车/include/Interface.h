
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

class ISimpleInterface
{
public:
	ISimpleInterface(){ m_RefCount = 1; }
	virtual int Release();

	int IncRef(){ m_RefCount++; return m_RefCount; }

protected:
	int m_RefCount;

	virtual ~ISimpleInterface(){}
};

#ifdef GFX_EXPORTS
#define QQ3DGFX_API __declspec(dllexport)
#else
#define QQ3DGFX_API __declspec(dllimport)
#endif

#endif //__INTERFACE_H__