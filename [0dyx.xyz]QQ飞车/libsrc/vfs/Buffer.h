
#pragma once

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IBUFFER TComponentImpl<IBuffer, CBuffer, COMPONENTID_VFS_BUFFER, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IBUFFER IBuffer
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"
    
NAMESPACEBEGIN


enum {EXPANDSIZE_LIMIT=4*1024*1024};
   
class CBuffer : public IBUFFER
{
public:
    CBuffer();
    ~CBuffer();
    
    RELEASEFUNC_ADDON
    
    virtual int Init(int usedsize, int buffersize);
    virtual void Release();

    virtual int Realloc(int buffernewsize);
    virtual int GetBufferSize() {return m_nBufferSize;}
    virtual int GetUsedSize() {return m_nUsedSize;}
    virtual int SetUsedSize(int size);
    
	virtual void* GetBuffer() {return m_pBuffer;}
    
protected:
    void * m_pBuffer;
    int  m_nUsedSize;
    int  m_nBufferSize;
    int  m_nPos;
}; 




NAMESPACEEND