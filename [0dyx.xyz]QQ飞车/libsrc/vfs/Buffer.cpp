
#include "Buffer.h"

NAMESPACEBEGIN


static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);


#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CBuffer);
#endif

CBuffer::CBuffer() : m_nUsedSize(0), m_nBufferSize(0), m_pBuffer(NULL), m_nPos(0)
{
    
}

CBuffer::~CBuffer()
{
    if (m_pBuffer)
    {
        free(m_pBuffer);
    }
}

void CBuffer::Release()
{
#if defined(VFS_PORTABLE)
	delete this;
#else
	Free();
#endif
}

int CBuffer::Init(int usedsize, int buffersize)
{
    m_pBuffer = malloc(buffersize);
    if (NULL==m_pBuffer) return 0;
        
    m_nUsedSize = usedsize;
    m_nBufferSize = _msize( m_pBuffer );
    
    m_nPos = 0;

    return 1;
}

int CBuffer::Realloc(int buffernewsize)
{
    if (buffernewsize<=m_nBufferSize) { return 1; }
        
    int newsize;
        
    if (buffernewsize>EXPANDSIZE_LIMIT)
    {
        newsize = buffernewsize+EXPANDSIZE_LIMIT;
    }
    else
    {
        newsize = buffernewsize+buffernewsize;
    }
    
    m_pBuffer = realloc(m_pBuffer,newsize);
    
    newsize = _msize(m_pBuffer);

    m_nBufferSize = newsize;
    
    if (newsize<buffernewsize)
    {
        return 0;
    }

    return 1;
}

int CBuffer::SetUsedSize(int size) 
{
	if (size<0) 
	{
		m_nUsedSize=0;
		return 0;
	}
	else if (size>m_nBufferSize)
	{
		m_nUsedSize=m_nBufferSize;
		return 0;
	}
	else 
	{
		m_nUsedSize = size;
		return 1;
	}
}

IBuffer* ICreateBuffer(int usedsize, ILogSys* pLogSys)
{
	s_pLogSys = pLogSys;

    IBuffer* pbuffer = static_cast<IBuffer*>(new CBuffer());
	if (pbuffer==NULL)
	{
		ERRORLOG(_T("buffer interface create error"));
		return NULL;
	}

    if (0==pbuffer->Init(usedsize,usedsize))
    {
        pbuffer->Release();
		ERRORLOG(_T("buffer interface init failed"));
        return NULL;
    }
    return pbuffer;    
}


NAMESPACEEND
