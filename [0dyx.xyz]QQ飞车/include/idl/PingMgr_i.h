

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Jun 05 07:30:35 2012
 */
/* Compiler settings for ..\include\Network\PingMgr.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PingMgr_i_h__
#define __PingMgr_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPingSink_FWD_DEFINED__
#define __IPingSink_FWD_DEFINED__
typedef interface IPingSink IPingSink;
#endif 	/* __IPingSink_FWD_DEFINED__ */


#ifndef __IPingMgr_FWD_DEFINED__
#define __IPingMgr_FWD_DEFINED__
typedef interface IPingMgr IPingMgr;
#endif 	/* __IPingMgr_FWD_DEFINED__ */


#ifndef __CPingMgr_FWD_DEFINED__
#define __CPingMgr_FWD_DEFINED__

#ifdef __cplusplus
typedef class CPingMgr CPingMgr;
#else
typedef struct CPingMgr CPingMgr;
#endif /* __cplusplus */

#endif 	/* __CPingMgr_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IPingSink_INTERFACE_DEFINED__
#define __IPingSink_INTERFACE_DEFINED__

/* interface IPingSink */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IPingSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("23F98A6C-5CDB-43A6-BF32-123456141476")
    IPingSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE NotifyPingResult( 
            int nIndex,
            int nTTL,
            float fLossRate,
            int nAverageime) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPingSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPingSink * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPingSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPingSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyPingResult )( 
            IPingSink * This,
            int nIndex,
            int nTTL,
            float fLossRate,
            int nAverageime);
        
        END_INTERFACE
    } IPingSinkVtbl;

    interface IPingSink
    {
        CONST_VTBL struct IPingSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPingSink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPingSink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPingSink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPingSink_NotifyPingResult(This,nIndex,nTTL,fLossRate,nAverageime)	\
    ( (This)->lpVtbl -> NotifyPingResult(This,nIndex,nTTL,fLossRate,nAverageime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPingSink_INTERFACE_DEFINED__ */


#ifndef __IPingMgr_INTERFACE_DEFINED__
#define __IPingMgr_INTERFACE_DEFINED__

/* interface IPingMgr */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IPingMgr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A986B5C9-71A3-44d0-98BA-E4321F123456")
    IPingMgr : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PingServer( 
            unsigned long ulSvrIP,
            unsigned short usPort,
            int *pnIndex,
            int nLimitTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelPing( 
            int nIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            IPingSink *pPingSink) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPingMgrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPingMgr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPingMgr * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPingMgr * This);
        
        HRESULT ( STDMETHODCALLTYPE *PingServer )( 
            IPingMgr * This,
            unsigned long ulSvrIP,
            unsigned short usPort,
            int *pnIndex,
            int nLimitTime);
        
        HRESULT ( STDMETHODCALLTYPE *CancelPing )( 
            IPingMgr * This,
            int nIndex);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IPingMgr * This,
            IPingSink *pPingSink);
        
        END_INTERFACE
    } IPingMgrVtbl;

    interface IPingMgr
    {
        CONST_VTBL struct IPingMgrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPingMgr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPingMgr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPingMgr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPingMgr_PingServer(This,ulSvrIP,usPort,pnIndex,nLimitTime)	\
    ( (This)->lpVtbl -> PingServer(This,ulSvrIP,usPort,pnIndex,nLimitTime) ) 

#define IPingMgr_CancelPing(This,nIndex)	\
    ( (This)->lpVtbl -> CancelPing(This,nIndex) ) 

#define IPingMgr_Initialize(This,pPingSink)	\
    ( (This)->lpVtbl -> Initialize(This,pPingSink) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPingMgr_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


