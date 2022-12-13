
#pragma once

NAMESPACEBEGIN


class IHandleAlloc PUBLICBASECLASS
{
public:
    virtual void  Reset()=0;
    virtual void  Release()=0;

    virtual unsigned int AllocHandle()=0;
    virtual void  FreeHandle(unsigned int handle)=0;
};

NAMESPACEEND
