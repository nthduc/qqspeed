
#pragma once

NAMESPACEBEGIN


class IFileMapper PUBLICBASECLASS
{
public:
    virtual int MapFile(const TCHAR* vfilepath, VROOT_h hroot, long lparam=0)=0;
    virtual VROOT_h FindFile(const TCHAR* vfilepath, long* lparam=NULL)=0;
    virtual void Reset()=0;
    virtual void Release()=0;
};


NAMESPACEEND
