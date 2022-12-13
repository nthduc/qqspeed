//////////////////////////////////////////////////////////////////////////
// 文件名称：   Log.h
// 创建人员：   简小征（ASTER）
// 创建日期：   2004.08.16
// 文件描述：   QQ堂游戏中日志类定义
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOG_H__064C2F03_689E_41cc_A3F4_84D15866D4F8__INCLUDED_)
#define AFX_LOG_H__064C2F03_689E_41cc_A3F4_84D15866D4F8__INCLUDED_

//void __qqt_ipc_log(UINT uColor, char* pFormat, ...);
/*
#ifdef _DEBUG
#define LOG_ERROR(info) \
CLog::GetInstance()->Write(CLog::LOG_COLOR_RED, "%s，源文件 = %s，行号 = %d。", info, __FILE__, __LINE__) 
#define LOG_WARNING(info) \
CLog::GetInstance()->Write(CLog::LOG_COLOR_YELLOW, "%s，源文件 = %s，行号 = %d。", info, __FILE__, __LINE__)
#define LOG_SUCCESS(info) \
CLog::GetInstance()->Write(CLog::LOG_COLOR_GREEN, "%s，源文件 = %s，行号 = %d。", info, __FILE__, __LINE__)
//#define FTC CTimeCount tc(__FILE__, __LINE__);
#define FTC /##/

//#define TC  CTimeCount tc(__FILE__, __LINE__);
#define TC  /##/
#else
#define LOG_SUCCESS(info) /##/
#define LOG_WARNING(info) /##/
#define LOG_ERROR(info) /##/
#define FTC             /##/
#define TC              /##/
#endif
*/

void flog_warning(char *fmt, ...);
void flog_error(char *fmt, ...);
void flog_success(char *fmt, ...);

void ipc_log_warning(char *fmt, ...);
void ipc_log_error(char *fmt, ...);
void ipc_log_success(char *fmt, ...);

void double_log_warning(char *fmt, ...);
void double_log_error(char *fmt, ...);
void double_log_success(char *fmt, ...);

void net_log(BOOL bSend, int nSeq, int msgid, int dataid, int nLen, char *str, char *key, char *fmt, ...);
void net_log_recv(int nSeq, int msgid, int dataid, int nLen, char *fmt, ...);
void net_log_send(int nSeq, int msgid, int dataid, int nLen, char *str, char *key, char *fmt, ...);
void net_log_send_now_other_file(int nLen, char *pBuf);
void net_log_send_now(int nLen, char *pBuf);

#define LOG_SUCCESS(info) /##/
#define LOG_WARNING(info) /##/
#define LOG_ERROR(info) /##/
#define FTC             /##/
#define TC              /##/

//#define _QQT_ENABLE_FLOG_
//#define _QQT_ENABLE_IPC_LOG_
//#define _QQT_ENALBE_DOUBLE_LOG_

#ifdef _QQT_ENABLE_FLOG_
#define FLOG_ERROR    flog_error
#define FLOG_SUCCESS  flog_success
#define FLOG_WARNING  flog_warning
#else
#define FLOG_ERROR    /##/
#define FLOG_SUCCESS  /##/
#define FLOG_WARNING  /##/
#endif

#ifdef _QQT_ENABLE_IPC_LOG_
#define ILOG_ERROR    ipc_log_error
#define ILOG_SUCCESS  ipc_log_success
#define ILOG_WARNING  ipc_log_warning
#else
#define ILOG_ERROR    /##/
#define ILOG_SUCCESS  /##/
#define ILOG_WARNING  /##/
#endif

#ifdef _QQT_ENALBE_DOUBLE_LOG_
#define DLOG_ERROR    double_log_error
#define DLOG_SUCCESS  double_log_success
#define DLOG_WARNING  double_log_warning
#else
#define DLOG_ERROR    /##/
#define DLOG_SUCCESS  /##/
#define DLOG_WARNING  /##/
#endif

class CLog
{
public:
    enum LOG_COLOR
    {
        LOG_COLOR_RED = 0,
        LOG_COLOR_GREEN,
        LOG_COLOR_YELLOW, 
        LOG_COLOR_WHITE
    };

    static CLog * GetInstance();    
    void Write(enum CLog::LOG_COLOR logColor, char * szString, ...);

    ~CLog();
protected:
    CLog();
    static CLog * s_pLog;
    char m_szFilename[256];
    char* GetColor(enum CLog::LOG_COLOR color);
};


struct CTimeCount
{
    CTimeCount(char * file, int line)
    {
        strncpy(_file, file, 260);        
        _line = line;
        _begintime = ::GetTickCount();
        //CLog::GetInstance()->Write(CLog::LOG_COLOR_GREEN, "进入函数源文件 = %s，行号 = %d。",_file, _line);
    }
    ~CTimeCount()
    {
        int _totaltime = ::GetTickCount() - _begintime;
        if (_totaltime > 1000)
        {
            CLog::GetInstance()->Write(CLog::LOG_COLOR_RED, "执行函数耗时3 = %d 毫秒，源文件 = %s，行号 = %d。",
            _totaltime, _file, _line);
        }
        else if (_totaltime > 100)
        {
            CLog::GetInstance()->Write(CLog::LOG_COLOR_YELLOW, "执行函数耗时2 = %d 毫秒，源文件 = %s，行号 = %d。",
            _totaltime, _file, _line);
        }
        else if (_totaltime > 30)
        {
            CLog::GetInstance()->Write(CLog::LOG_COLOR_GREEN, "执行函数耗时1 = %d 毫秒，源文件 = %s，行号 = %d。",
            _totaltime, _file, _line);
        }
    }
private:
    char _file[260];
    int _line, _begintime;
};
#endif // !defined(AFX_LOG_H__064C2F03_689E_41cc_A3F4_84D15866D4F8__INCLUDED_)