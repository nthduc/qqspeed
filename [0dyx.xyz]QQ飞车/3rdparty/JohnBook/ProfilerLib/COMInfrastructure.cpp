/*//////////////////////////////////////////////////////////////////////
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright (c) 1997-2003 John Robbins -- All rights reserved.

COMInfrastructure.cpp - Takes care of the background COM junk for
                        writing .NET profilers.
- Borrowed from Matt Pietrek who borrowed it from the original .NET
  profiler samples.
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"

#include "ProfilerLib.h"

#ifndef UNICODE
#define UNICODE
#endif  // UNICODE

#ifndef _UNICODE
#define _UNICODE
#endif  // _UNICODE

HRESULT RegisterClassBase ( REFCLSID        rclsid          ,
                            const TCHAR *    szDesc         ,
                            const TCHAR *    szProgID       ,
                            const TCHAR *    szIndepProgID  ,
                            TCHAR *          szOutCLSID      ) ;

HRESULT UnregisterClassBase ( REFCLSID      rclsid          ,
                              const TCHAR * szProgID        ,
                              const TCHAR * szIndepProgID   ,
                              TCHAR *       szOutCLSID       ) ;

BOOL SetRegValue ( TCHAR * szKeyName ,
                   TCHAR * szKeyword ,
                   TCHAR * szValue    ) ;

BOOL DeleteKey ( const TCHAR * szKey , const TCHAR * szSubkey ) ;

BOOL SetKeyAndValue ( const TCHAR * szKey    ,
                      const TCHAR * szSubkey ,
                      const TCHAR * szValue   ) ;

#define ARRAY_SIZE( s ) (sizeof( s ) / sizeof( s[0] ))
#define MAX_LENGTH 256

#define COM_METHOD( TYPE ) TYPE STDMETHODCALLTYPE

HINSTANCE G_hInst;

class CClassFactory : public IClassFactory
{
public      :
    CClassFactory( )
    {
        m_refCount = 1 ;
    }

    COM_METHOD( ULONG ) AddRef( )
    {
        return ( InterlockedIncrement( &m_refCount ) ) ;
    }

    COM_METHOD( ULONG ) Release( )
    {
        return ( InterlockedDecrement ( &m_refCount ) ) ;
    }

    COM_METHOD( HRESULT ) QueryInterface ( REFIID  riid        ,
                                           void ** ppInterface  ) ;

    // IClassFactory methods
    COM_METHOD( HRESULT ) LockServer( BOOL /*fLock*/ )
    {
        return ( S_OK ) ;
    }

    COM_METHOD( HRESULT ) CreateInstance( IUnknown * pUnkOuter   ,
                                          REFIID     riid        ,
                                          void **    ppInterface  ) ;

private     :

    long m_refCount ;
} ;

CClassFactory g_ProfilerClassFactory ;

STDAPI DllUnregisterServer( )
{
    TCHAR szCLSID [ 64 ] ;
    OLECHAR szWID [ 64 ] ;
    TCHAR rcProgID[ 128 ] ;
    TCHAR rcIndProgID[ 128 ] ;


    // format the prog ID values.
    _stprintf ( rcProgID         ,
                _T ( "%s.%s" )   ,
                G_szProgIDPrefix ,
                G_szProfilerGUID  ) ;
    _stprintf ( rcIndProgID    ,
                _T ( "%s.%d" ) ,
                rcProgID       ,
                1               ) ;

    UnregisterClassBase( G_CLSID_PROFILER ,
                         rcProgID         ,
                         rcIndProgID      ,
                         szCLSID           ) ;

    DeleteKey ( szCLSID , _T ( "InprocServer32" ) ) ;

    StringFromGUID2 ( G_CLSID_PROFILER    ,
                      szWID               ,
                      ARRAY_SIZE( szWID )  ) ;

    DeleteKey( _T ( "CLSID" ) , szCLSID ) ;

    return ( S_OK ) ;
}

STDAPI DllRegisterServer( )
{
    HRESULT hr = S_OK ;
    TCHAR  szModule[_MAX_PATH] ;

    DllUnregisterServer( );
    GetModuleFileName ( G_hInst , szModule , ARRAY_SIZE ( szModule ) ) ;

    // CLSID\\szID.
    TCHAR rcCLSID[ MAX_LENGTH ] ;
    // szProgIDPrefix.szClassProgID
    TCHAR rcProgID[ MAX_LENGTH ] ;
    // rcProgID.iVersion
    TCHAR rcIndProgID[ MAX_LENGTH ] ;
    // CLSID\\InprocServer32
    TCHAR rcInproc[MAX_LENGTH + 2];


    // format the prog ID values.
    _stprintf ( rcIndProgID      ,
                _T ( "%s.%s" )   ,
                G_szProgIDPrefix ,
                G_szProfilerGUID  ) ;
    _stprintf ( rcProgID        ,
                _T ( "%s.%d" )  ,
                rcIndProgID     ,
                1                ) ;

    // do the initial portion.
    hr =  RegisterClassBase ( G_CLSID_PROFILER ,
                              G_szProfilerName ,
                              rcProgID         ,
                              rcIndProgID      ,
                              rcCLSID           ) ;

    if ( SUCCEEDED( hr ) )
    {
        // set the server path.
        SetKeyAndValue ( rcCLSID, _T ( "InprocServer32" ) , szModule ) ;

        // add the threading model information.
        _stprintf ( rcInproc                ,
                    _T ( "%s\\%s" )         ,
                    rcCLSID                 ,
                    _T ( "InprocServer32" )  ) ;
        SetRegValue ( rcInproc                ,
                      _T ( "ThreadingModel" ) ,
                      _T( "Both" )             ) ;
    }
    else
    {
        DllUnregisterServer ( ) ;
    }

    return ( hr ) ;
}

STDAPI DllGetClassObject ( REFCLSID     rclsid ,
                           REFIID       riid   ,
                           LPVOID FAR * ppv     )
{
    HRESULT hr = E_OUTOFMEMORY ;

    if ( rclsid == G_CLSID_PROFILER )
    {
        hr = g_ProfilerClassFactory.QueryInterface ( riid, ppv ) ;
    }

    return ( hr ) ;
}

HRESULT CClassFactory :: QueryInterface ( REFIID  riid        ,
                                          void ** ppInterface  )
{
    if ( IID_IUnknown == riid )
    {
        *ppInterface = static_cast<IUnknown *> ( this ) ;
    }
    else if ( IID_IClassFactory == riid )
    {
        *ppInterface = static_cast<IClassFactory *> ( this ) ;
    }
    else
    {
        *ppInterface = NULL ;
        return ( E_NOINTERFACE ) ;
    }

    reinterpret_cast<IUnknown *>( *ppInterface )->AddRef ( ) ;

    return ( S_OK ) ;
}

HRESULT CClassFactory :: CreateInstance ( IUnknown * pUnkOuter ,
                                          REFIID     /*riid*/  ,
                                          void **    ppInstance )
{
    // aggregation is not supported by these objects
    if ( NULL != pUnkOuter )
    {
        return ( CLASS_E_NOAGGREGATION ) ;
    }

    ICorProfilerCallback * pProfilerCallback = AllocateProfilerCallback ();

    *ppInstance = (void *)pProfilerCallback ;

    return ( S_OK ) ;
}

HRESULT RegisterClassBase( REFCLSID      rclsid         ,
                           const TCHAR * szDesc         ,
                           const TCHAR * szProgID       ,
                           const TCHAR * szIndepProgID  ,
                           TCHAR *       szOutCLSID      )
{
    OLECHAR szWID[ 64 ] ; // helper for the class ID to register.

    StringFromGUID2 ( rclsid , szWID , ARRAY_SIZE( szWID ) ) ;

    _tcscpy ( szOutCLSID , _T ( "CLSID\\" ) ) ;
    _tcscat ( szOutCLSID , szWID ) ;

    // create ProgID keys.
    SetKeyAndValue ( szProgID , NULL , szDesc ) ;
    SetKeyAndValue ( szProgID , _T ( "CLSID" ) , szWID ) ;

    // create VersionIndependentProgID keys.
    SetKeyAndValue ( szIndepProgID , NULL , szDesc ) ;
    SetKeyAndValue ( szIndepProgID , _T ( "CurVer" ) , szProgID ) ;
    SetKeyAndValue ( szIndepProgID , _T ( "CLSID" ) , szWID ) ;

    // create entries under CLSID.
    SetKeyAndValue ( szOutCLSID , NULL, szDesc ) ;
    SetKeyAndValue ( szOutCLSID , _T ( "ProgID" ) , szProgID ) ;
    SetKeyAndValue ( szOutCLSID                        ,
                     _T ( "VersionIndependentProgID" ) ,
                     szIndepProgID                      ) ;
    SetKeyAndValue ( szOutCLSID , _T ( "NotInsertable" ) , NULL ) ;

    return ( S_OK ) ;
}

HRESULT UnregisterClassBase ( REFCLSID      rclsid        ,
                              const TCHAR * szProgID      ,
                              const TCHAR * szIndepProgID ,
                              TCHAR *       szOutCLSID     )
{
    OLECHAR szWID[64]; // helper for the class ID to register.

    StringFromGUID2 ( rclsid , szWID , ARRAY_SIZE ( szWID ) ) ;

    _tcscpy ( szOutCLSID , _T ( "CLSID\\" ) ) ;
    _tcscat ( szOutCLSID , szWID ) ;

    // delete the version independant prog ID settings.
    DeleteKey ( szIndepProgID , _T ( "CurVer" ) ) ;
    DeleteKey ( szIndepProgID , _T ( "CLSID" ) ) ;
    RegDeleteKey ( HKEY_CLASSES_ROOT , szIndepProgID ) ;

    // delete the prog ID settings.
    DeleteKey ( szProgID , _T ( "CLSID" ) ) ;
    RegDeleteKey ( HKEY_CLASSES_ROOT , szProgID ) ;

    // delete the class ID settings.
    DeleteKey ( szOutCLSID , _T ( "ProgID" ) ) ;
    DeleteKey ( szOutCLSID , _T ( "VersionIndependentProgID" ) ) ;
    DeleteKey ( szOutCLSID , _T ( "NotInsertable" ) ) ;
    RegDeleteKey ( HKEY_CLASSES_ROOT , szOutCLSID ) ;

    return ( S_OK ) ;
}

BOOL DeleteKey ( const TCHAR * szKey    ,
                 const TCHAR * szSubkey  )
{
    TCHAR rcKey[ MAX_LENGTH ] ; // buffer for the full key name.


    // init the key with the base key name.
    _tcscpy( rcKey, szKey );

    // append the subkey name (if there is one).
    if ( NULL != szSubkey )
    {
        _tcscat ( rcKey , _T ( "\\" ) ) ;
        _tcscat ( rcKey , szSubkey ) ;
    }

    // delete the registration key.
    RegDeleteKey ( HKEY_CLASSES_ROOT , rcKey ) ;

    return ( TRUE ) ;
}

BOOL SetKeyAndValue ( const TCHAR * szKey    ,
                      const TCHAR * szSubkey ,
                      const TCHAR * szValue   )
{
    HKEY hKey ;                 // handle to the new reg key.
    TCHAR rcKey[ MAX_LENGTH ] ; // buffer for the full key name.

    // init the key with the base key name.
    _tcscpy ( rcKey , szKey ) ;

    // append the subkey name (if there is one).
    if ( NULL != szSubkey )
    {
        _tcscat( rcKey , _T ( "\\" ) ) ;
        _tcscat( rcKey , szSubkey ) ;
    }

    // create the registration key.
    if ( ERROR_SUCCESS == RegCreateKeyEx ( HKEY_CLASSES_ROOT        ,
                                           rcKey                    ,
                                           0                        ,
                                           NULL                     ,
                                           REG_OPTION_NON_VOLATILE  ,
                                           KEY_ALL_ACCESS           ,
                                           NULL                     ,
                                           &hKey                    ,
                                           NULL                      ) )
    {
        // set the value (if there is one).
        if ( NULL != szValue )
        {
            RegSetValueEx ( hKey                ,
                            NULL                ,
                            0                   ,
                            REG_SZ              ,
                            (BYTE *)szValue     ,
                            (DWORD)(((_tcslen(szValue) + 1) *
                                            sizeof (TCHAR)))) ;
        }

        RegCloseKey ( hKey ) ;

        return ( TRUE ) ;
    }

    return ( FALSE ) ;
}

BOOL SetRegValue (TCHAR *szKeyName , TCHAR * szKeyword, TCHAR * szValue)
{
    HKEY hKey; // handle to the new reg key.

    // create the registration key.
    if ( ERROR_SUCCESS == RegCreateKeyEx ( HKEY_CLASSES_ROOT        ,
                                           szKeyName                ,
                                           0                        ,
                                           NULL                     ,
                                           REG_OPTION_NON_VOLATILE  ,
                                           KEY_ALL_ACCESS           ,
                                           NULL                     ,
                                           &hKey                    ,
                                           NULL                      ) )
    {
        // set the value (if there is one).
        if ( NULL != szValue )
        {
            RegSetValueEx ( hKey            ,
                            szKeyword       ,
                            0               ,
                            REG_SZ          ,
                            (BYTE *)szValue ,
                            (DWORD)((_tcslen(szValue) + 1) *
                                        sizeof ( TCHAR ))) ;
        }

        RegCloseKey ( hKey ) ;

        return ( TRUE) ;
    }

    return ( FALSE ) ;
}
