/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/

#ifndef _DBGCOMMANDLINE_H
#define _DBGCOMMANDLINE_H

class CDbgCommandLine : public CCommandLineInfo
{

public  :
    CDbgCommandLine ( )
    {
        m_iEventNum = 0 ;
        m_iProcessNum = 0 ;
        m_eState = eNormal ;
    }

    virtual void ParseParam ( LPCTSTR lpszParam ,
                              BOOL    bFlag     ,
                              BOOL    bLast      ) ;
    // The -e event number.
    int m_iEventNum ;
    // The -p process number.
    int m_iProcessNum ;

protected   :
    // The states.
    enum
    {
        eNormal            ,
        eWaitingForEvent   ,
        eWaitingForProcess
    } ;

    int m_eState ;

} ;

inline void CDbgCommandLine :: ParseParam ( LPCTSTR lpszParam ,
                                            BOOL    bFlag     ,
                                            BOOL    /*bLast*/  )
{
    if ( FALSE == bFlag )
    {
        if ( eWaitingForEvent == m_eState )
        {
            m_eState = eNormal ;
            m_iEventNum = _tstol ( lpszParam ) ;
        }
        else if ( eWaitingForProcess == m_eState )
        {
            m_eState = eNormal ;
            m_iProcessNum = _tstol ( lpszParam ) ;
        }
    }
    if ( TRUE == bFlag )
    {
        switch ( lpszParam[0] )
        {
            case _T ( 'e' ) :
                m_eState = eWaitingForEvent ;
                break ;
            case _T ( 'p' ) :
                m_eState = eWaitingForProcess ;
                break ;
            default :
                break ;
        }
    }
}



#endif  // _DBGCOMMANDLINE_H

