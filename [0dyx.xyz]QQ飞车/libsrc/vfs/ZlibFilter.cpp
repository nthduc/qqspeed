
#include "ZlibFilter.h"

#include "zlib.h"

#if defined(_DEBUG)
  #  pragma comment(lib,"zlibd.lib")
#else
  #  pragma comment(lib,"zlib.lib")
#endif

NAMESPACEBEGIN

#if !defined(VFS_PORTABLE)
	DEFINE_FACTORY_METHOD(CZlibRWFilter);
#endif


static ILogSys* s_pLogSys = NULL;

#define ERRORLOG(msg) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error %s\n"),__FILE__,__LINE__,msg);
#define ERRORLOG1(msg,p1) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1);
#define ERRORLOG2(msg,p1,p2) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2);
#define ERRORLOG3(msg,p1,p2,p3) if (s_pLogSys) s_pLogSys->printf(_T("%s(%d) : error "##msg"\n"),__FILE__,__LINE__,p1,p2,p3);



void CZlibRWFilter::Release()
{
#if !defined(VFS_PORTABLE)
	Free();
#endif
}

int CZlibRWFilter::GetSaveSize(IBuffer* inbuf)
{
	if (inbuf==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	return compressBound(inbuf->GetUsedSize());
}

int CZlibRWFilter::PreSave(IBuffer* inbuf, IBuffer* outbuf)
{
	if (inbuf==NULL || outbuf==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	unsigned long destlen=outbuf->GetBufferSize();
	if (Z_OK==compress((unsigned char*)outbuf->GetBuffer(),&destlen,(unsigned char*)inbuf->GetBuffer(),inbuf->GetUsedSize()))
	{
		outbuf->SetUsedSize(destlen);
		return destlen;
	}
	else
	{
		ERRORLOG(_T("RWFilter PreSave error!"));
		return 0;
	}
}

int CZlibRWFilter::PostLoad(IBuffer* inbuf, IBuffer* outbuf)
{
	if (inbuf==NULL || outbuf==NULL)
	{
		ERRORLOG(_T("param error!"));
		return 0;
	}

	unsigned long destlen = outbuf->GetBufferSize();
	if (Z_OK==uncompress((unsigned char*)outbuf->GetBuffer(),&destlen,(unsigned char*)inbuf->GetBuffer(),inbuf->GetUsedSize()))
	{
		outbuf->SetUsedSize(destlen);
		return destlen;
	}
	else
	{
		ERRORLOG(_T("RWFilter PostLoad error!"));
		return 0;
	}
}

IRWFilter* ICreateZlibRWFilter(ILogSys* pLogSys)
{
	s_pLogSys = pLogSys;

#if defined(VFS_PORTABLE)
	static CZlibRWFilter s_zlibrwfilter;
	IRWFilter* pfilter = static_cast<IRWFilter*>(&s_zlibrwfilter);
	return pfilter;
#else
	IRWFilter* pfilter = static_cast<IRWFilter*>(new CZlibRWFilter());
	return pfilter;
#endif

}


NAMESPACEEND