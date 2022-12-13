
#pragma once

NAMESPACEBEGIN


class IBuffer PUBLICBASECLASS
{
public:
    virtual int Init(int usedsize, int buffersize)=0;
    virtual void Release()=0;

    virtual int Realloc(int buffernewsize)=0;
	
    virtual int GetBufferSize()=0;
    virtual int GetUsedSize()=0;
	virtual int SetUsedSize(int size)=0;
    
    virtual void* GetBuffer()=0;
};



NAMESPACEEND
