
#pragma once

#pragma warning(disable : 4786)

#if !defined(VFS_PORTABLE)
#  include "teniodll.h"
#  define IRWFILTER TComponentImpl<IRWFilter, CZlibRWFilter, COMPONENTID_VFS_ZLIBRWFILTER, 1>
#  define RELEASEFUNC_ADDON void ComponentFinalize() {}
#else
#  define IRWFILTER IRWFilter
#  define RELEASEFUNC_ADDON
#endif

#  include "VFS.h"

NAMESPACEBEGIN

class CZlibRWFilter : public IRWFILTER
{
public:    
	RELEASEFUNC_ADDON
    virtual void Release();
	virtual int GetSaveSize(IBuffer* inbuf);
    virtual int PreSave(IBuffer* inbuf, IBuffer* outbuf);
    virtual int PostLoad(IBuffer* inbuf, IBuffer* outbuf);
};


NAMESPACEEND