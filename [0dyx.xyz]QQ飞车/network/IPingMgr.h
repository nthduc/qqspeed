/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Mar 09 15:38:47 2006
 */
/* Compiler settings for E:\coding\fishhong_QQTang_200410_Proj_int\IED_QQTang_VOB\QQTang\Client\source\IDL\IQQTPing.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __IPingMgr_h__
#define __IPingMgr_h__

#ifdef __cplusplus
extern "C"{
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
//#include "IQQTMsgMgr.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

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
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPingSink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPingSink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPingSink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyPingResult )( 
            IPingSink __RPC_FAR * This,
            int nIndex,
            int nTTL,
            float fLossRate,
            int nAverageime);
        
        END_INTERFACE
    } IPingSinkVtbl;

    interface IPingSink
    {
        CONST_VTBL struct IPingSinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPingSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPingSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPingSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPingSink_NotifyPingResult(This,nIndex,nTTL,fLossRate,nAverageime)	\
    (This)->lpVtbl -> NotifyPingResult(This,nIndex,nTTL,fLossRate,nAverageime)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPingSink_NotifyPingResult_Proxy( 
    IPingSink __RPC_FAR * This,
    int nIndex,
    int nTTL,
    float fLossRate,
    int nAverageime);


void __RPC_STUB IPingSink_NotifyPingResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



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
            int __RPC_FAR *pnIndex,
            int nLimitTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelPing( 
            int nIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            IPingSink __RPC_FAR *pPingSink) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPingMgrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPingMgr __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPingMgr __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPingMgr __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PingServer )( 
            IPingMgr __RPC_FAR * This,
            unsigned long ulSvrIP,
            unsigned short usPort,
            int __RPC_FAR *pnIndex,
            int nLimitTime);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelPing )( 
            IPingMgr __RPC_FAR * This,
            int nIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IPingMgr __RPC_FAR * This,
            IPingSink __RPC_FAR *pPingSink);
        
        END_INTERFACE
    } IPingMgrVtbl;

    interface IPingMgr
    {
        CONST_VTBL struct IPingMgrVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPingMgr_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPingMgr_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPingMgr_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPingMgr_PingServer(This,ulSvrIP,usPort,pnIndex,nLimitTime)	\
    (This)->lpVtbl -> PingServer(This,ulSvrIP,usPort,pnIndex,nLimitTime)

#define IPingMgr_CancelPing(This,nIndex)	\
    (This)->lpVtbl -> CancelPing(This,nIndex)

#define IPingMgr_Initialize(This,pPingSink)	\
    (This)->lpVtbl -> Initialize(This,pPingSink)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPingMgr_PingServer_Proxy( 
    IPingMgr __RPC_FAR * This,
    unsigned long ulSvrIP,
    unsigned short usPort,
    int __RPC_FAR *pnIndex,
    int nLimitTime);


void __RPC_STUB IPingMgr_PingServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPingMgr_CancelPing_Proxy( 
    IPingMgr __RPC_FAR * This,
    int nIndex);


void __RPC_STUB IPingMgr_CancelPing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPingMgr_Initialize_Proxy( 
    IPingMgr __RPC_FAR * This,
    IPingSink __RPC_FAR *pPingSink);


void __RPC_STUB IPingMgr_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPingMgr_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
