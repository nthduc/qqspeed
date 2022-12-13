
#pragma once

NAMESPACEBEGIN

class IRWFilter PUBLICBASECLASS
{
public:    
    virtual void Release() = 0;
	virtual int GetSaveSize(IBuffer* inbuf)=0;
    virtual int PreSave(IBuffer* inbuf, IBuffer* outbuf) = 0;
    virtual int PostLoad(IBuffer* inbuf, IBuffer* outbuf) = 0;
};
 
NAMESPACEEND
