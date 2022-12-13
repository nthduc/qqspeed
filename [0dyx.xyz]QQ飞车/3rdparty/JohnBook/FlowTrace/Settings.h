/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Windows Applications
Copyright © 2002-2003 John Robbins
----------------------------------------------------------------------*/
#pragma once

const TCHAR * k_EXTENSION = _T ( ".FLS" ) ;

const TCHAR * k_GENERALOPTSECTION          = _T ( "General Options" ) ;
const TCHAR * k_TURNOFFINLININGKEY         = _T ( "TurnOffInlining" ) ;
const TCHAR * k_IGNOREFINALIZERTHREADKEY   =_T("IgnoreFinalizerThread");
const TCHAR * k_SKIPSTARTUPONMAINTHREADKEY =
                                  _T ( "SkipStartupCodeOnMainThread" ) ;

class Settings
{
public      :
    Settings ( void )
    {
    }
    
    virtual ~Settings ( void )
    {
    }
    
    void Initialize ( LPCTSTR szPathAndMod )
    {
        
        StringCchCopy ( m_szIniFile                               ,
                        sizeof ( m_szIniFile ) / sizeof ( TCHAR ) ,
                        szPathAndMod                               ) ;
        StringCchCat ( m_szIniFile                               ,
                       sizeof ( m_szIniFile ) / sizeof ( TCHAR ) ,
                       k_EXTENSION                                ) ;
    }
    
    inline BOOL TurnOffInlining ( void )
    {
        return ( (BOOL)GetPrivateProfileInt ( k_GENERALOPTSECTION  ,
                                              k_TURNOFFINLININGKEY ,
                                              0                    ,
                                              m_szIniFile           ) );
    }
    
    inline BOOL IgnoreFinalizerThread ( void )
    {
        return ( (BOOL)
                  GetPrivateProfileInt ( k_GENERALOPTSECTION        ,
                                         k_IGNOREFINALIZERTHREADKEY ,
                                         1                          ,
                                         m_szIniFile                 ));
    }
    
    inline BOOL SkipStartupOnMainThread ( void )
    {
        return ( (BOOL)
                  GetPrivateProfileInt ( k_GENERALOPTSECTION          ,
                                         k_SKIPSTARTUPONMAINTHREADKEY ,
                                         1                            ,
                                         m_szIniFile                 ));
    }
    
protected   :
    TCHAR m_szIniFile[ MAX_PATH ] ;
} ;