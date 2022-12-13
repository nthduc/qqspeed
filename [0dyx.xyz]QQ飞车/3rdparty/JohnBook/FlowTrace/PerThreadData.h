/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Windows Applications
Copyright © 2002-2003 John Robbins
----------------------------------------------------------------------*/
#pragma once

// The format string used to build up the complete output filename.
static const TCHAR * k_FILENAMEFMT = _T ( "%s%p.FLOW" ) ;
// The exception report format.
static const TCHAR * k_EXCEPTIONREPORTFMT =
                     _T ( "Unwinding from %s exception (%d methods)" ) ;
// A dummy exception name.
static const TCHAR * k_EXCEPTDEFAULTNAME = _T("**UNKNOWN EXCEPTION**");

// The class stuffed into thread local storage.
class CPerThreadData
{
public      :
    CPerThreadData ( void )
    {
        m_pFile = NULL ;
        m_uiIndentLevel = 1 ;
        m_uiExceptionUnwinds = 0 ;
        m_ThreadID = 0 ;
        StringCchCopy ( m_szExceptName                               ,
                        sizeof ( m_szExceptName ) / sizeof ( TCHAR ) ,
                        k_EXCEPTDEFAULTNAME                           );
    }

    virtual ~CPerThreadData ( void )
    {
        if ( NULL != m_pFile )
        {
            fclose ( m_pFile ) ;
            m_pFile = NULL ;
        }
    }

public      :

    BOOL Initialize ( LPCTSTR szFileNamePrefix , ThreadID threadID )
    {
        m_ThreadID = threadID ;
        
        TCHAR szFile[ MAX_PATH ] ;
        StringCchPrintf ( szFile            ,
                          MAX_PATH          ,
                          k_FILENAMEFMT     ,
                          szFileNamePrefix  ,
                          m_ThreadID         ) ;
        m_pFile = _tfopen ( szFile , _T ( "wt" ) ) ;
        ASSERT ( NULL != m_pFile ) ;
        return ( NULL != m_pFile ) ;
    }
    
    void WriteData ( LPCTSTR szClass , LPCTSTR szMethod )
    {
        ASSERT ( NULL != m_pFile ) ;
        _ftprintf ( m_pFile                 ,
                   _T ( "%*c%s.%s\n" )      ,
                   m_uiIndentLevel          ,
                   _T ( ' ' )               ,
                   szClass                  ,
                   szMethod                  ) ;
    }
    
    void WriteMessage ( LPCTSTR szMsg )
    {
        _ftprintf ( m_pFile , _T ( "%s\n" ) , szMsg ) ;
    }

    inline void IncrementIndentLevel ( void )
    {
        m_uiIndentLevel++ ;
    }

    inline void DecrementIndentLevel ( void )
    {
        m_uiIndentLevel-- ;
        ASSERT ( m_uiIndentLevel > 0 ) ;
    }

    inline void IncrementExceptionUnwind ( void )
    {
        m_uiExceptionUnwinds++ ;
    }
    
    inline UINT GetExceptionUnwinds ( void )
    {
        return ( m_uiExceptionUnwinds ) ;
    }
    
    inline ThreadID GetThreadID ( void )
    {
        return ( m_ThreadID ) ;
    }
    
    inline void SetLastExceptionName ( LPCTSTR szExceptName )
    {
        StringCchCopy ( m_szExceptName                               ,
                        sizeof ( m_szExceptName ) / sizeof ( TCHAR ) ,
                        szExceptName                                  );
    }

    void ReportExceptionAndUnwinds ( void )
    {
        ASSERT ( m_uiExceptionUnwinds < m_uiIndentLevel ) ;

        TCHAR szMsg[ 400 ] ;
        StringCchPrintf ( szMsg                               ,
                          sizeof ( szMsg ) / sizeof ( TCHAR ) ,
                          k_EXCEPTIONREPORTFMT                ,
                          m_szExceptName                      ,
                          m_uiExceptionUnwinds                 ) ;

        WriteMessage ( szMsg ) ;
        
        m_uiIndentLevel -= m_uiExceptionUnwinds ;
        m_uiExceptionUnwinds = 0 ;
    }
    
protected   :
    // The file handle for this thread.
    FILE * m_pFile ;
    
    // The indent level.
    UINT m_uiIndentLevel ;
    
    // The number of exception unwinds seen.
    UINT m_uiExceptionUnwinds ;
    
    // The thread ID.
    ThreadID m_ThreadID ;
    
    // The last exception seen.
    TCHAR m_szExceptName[ MAX_PATH ] ;
} ;

