#ifndef __SIGSHARE_H
#define __SIGSHARE_H

#include <windows.h>

class CShareMem
{
public:
    // open and close share mem 
    bool Open(const char* fname, int datasize);
    void Close();

public:
    char *GetData();
    int GetDataSize();
    const char* GetName();

public:
    CShareMem();
    virtual ~CShareMem();

protected:
    HANDLE            m_hFileMap;
    char              *m_pData;
    int               m_nDataSize;
    char              m_szName[MAX_PATH];
};


class CSigShare : public CShareMem
{
public:
    
public:
    // open and close share mem 
    bool Open(const char* fname="qqfo_client_SigShare");
    bool IsValid(); 

public: 
    // operate
    // set
    bool SetFlag(int flag);
    bool SetSiginfo(const void* siginfo);
    bool SetZoneUrl(const char* zoneurl);
	bool SetServerInfo(const void* serverinfo);

    // get, 在调用以下接口时需要调用IsValid作判断
    int GetFlag();
    void* GetSiginfo();
    char* GetZoneUrl();
	void* GetServerInfo();

public:
    CSigShare(){};
    virtual ~CSigShare(){};
};

extern CSigShare g_SigShare;

#endif // __SIGSHARE_H