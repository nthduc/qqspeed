

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Sep 18 17:32:35 2006
 */
/* Compiler settings for \Coding\Project\P2p\IQQTppp.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

#ifndef __IQQTppp_h__
#define __IQQTppp_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IQQTpppSink_FWD_DEFINED__
#define __IQQTpppSink_FWD_DEFINED__
typedef interface IQQTpppSink IQQTpppSink;
#endif 	/* __IQQTpppSink_FWD_DEFINED__ */


#ifndef __IQQTppp_FWD_DEFINED__
#define __IQQTppp_FWD_DEFINED__
typedef interface IQQTppp IQQTppp;
#endif 	/* __IQQTppp_FWD_DEFINED__ */


#ifndef __CP2PModule_FWD_DEFINED__
#define __CP2PModule_FWD_DEFINED__

#ifdef __cplusplus
typedef class CP2PModule CP2PModule;
#else
typedef struct CP2PModule CP2PModule;
#endif /* __cplusplus */

#endif 	/* __CP2PModule_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_IQQTppp_0000 */
/* [local] */ 


enum PPPSetGameStatus
    {	PPPSetGameStatus_NotInGame	= 0,
	PPPSetGameStatus_InGame	= 1
    } ;


extern RPC_IF_HANDLE __MIDL_itf_IQQTppp_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_IQQTppp_0000_v0_0_s_ifspec;

#ifndef __IQQTpppSink_INTERFACE_DEFINED__
#define __IQQTpppSink_INTERFACE_DEFINED__

/* interface IQQTpppSink */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IQQTpppSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("23F98A6C-5CDB-43A6-BF32-CE9327141476")
    IQQTpppSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnUdpLoginOk( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnRecvP2PDataFromSvr( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nBodyLen,
            /* [size_is][in] */ BYTE *pBody) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnGetPlayerInfoFromSvr( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnRecvClientTry2ConnectData( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ ULONG ulTestMyIP,
            /* [in] */ USHORT usTestMyPort) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnRecvP2PGameDataFromClient( 
            /* [in] */ short shSocketPlayerID,
            /* [in] */ int nSocketPlayerUin,
            /* [in] */ short shPlayerID,
            /* [in] */ int nBodyLen,
            /* [size_is][in] */ BYTE *pBody) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendTcpP2pData( 
            /* [in] */ int nPlayerCount,
            /* [in] */ short *asPlayerID,
            /* [in] */ int *aiUin,
            /* [in] */ int iLen,
            /* [size_is][in] */ BYTE *pData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQQTpppSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQQTpppSink * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQQTpppSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQQTpppSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnUdpLoginOk )( 
            IQQTpppSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnRecvP2PDataFromSvr )( 
            IQQTpppSink * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nBodyLen,
            /* [size_is][in] */ BYTE *pBody);
        
        HRESULT ( STDMETHODCALLTYPE *OnGetPlayerInfoFromSvr )( 
            IQQTpppSink * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin);
        
        HRESULT ( STDMETHODCALLTYPE *OnRecvClientTry2ConnectData )( 
            IQQTpppSink * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ ULONG ulTestMyIP,
            /* [in] */ USHORT usTestMyPort);
        
        HRESULT ( STDMETHODCALLTYPE *OnRecvP2PGameDataFromClient )( 
            IQQTpppSink * This,
            /* [in] */ short shSocketPlayerID,
            /* [in] */ int nSocketPlayerUin,
            /* [in] */ short shPlayerID,
            /* [in] */ int nBodyLen,
            /* [size_is][in] */ BYTE *pBody);
        
        HRESULT ( STDMETHODCALLTYPE *SendTcpP2pData )( 
            IQQTpppSink * This,
            /* [in] */ int nPlayerCount,
            /* [in] */ short *asPlayerID,
            /* [in] */ int *aiUin,
            /* [in] */ int iLen,
            /* [size_is][in] */ BYTE *pData);
        
        END_INTERFACE
    } IQQTpppSinkVtbl;

    interface IQQTpppSink
    {
        CONST_VTBL struct IQQTpppSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQQTpppSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQQTpppSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQQTpppSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQQTpppSink_OnUdpLoginOk(This)	\
    (This)->lpVtbl -> OnUdpLoginOk(This)

#define IQQTpppSink_OnRecvP2PDataFromSvr(This,shPlayerID,nBodyLen,pBody)	\
    (This)->lpVtbl -> OnRecvP2PDataFromSvr(This,shPlayerID,nBodyLen,pBody)

#define IQQTpppSink_OnGetPlayerInfoFromSvr(This,shPlayerID,nUin)	\
    (This)->lpVtbl -> OnGetPlayerInfoFromSvr(This,shPlayerID,nUin)

#define IQQTpppSink_OnRecvClientTry2ConnectData(This,shPlayerID,nUin,ulTestMyIP,usTestMyPort)	\
    (This)->lpVtbl -> OnRecvClientTry2ConnectData(This,shPlayerID,nUin,ulTestMyIP,usTestMyPort)

#define IQQTpppSink_OnRecvP2PGameDataFromClient(This,shSocketPlayerID,nSocketPlayerUin,shPlayerID,nBodyLen,pBody)	\
    (This)->lpVtbl -> OnRecvP2PGameDataFromClient(This,shSocketPlayerID,nSocketPlayerUin,shPlayerID,nBodyLen,pBody)

#define IQQTpppSink_SendTcpP2pData(This,nPlayerCount,asPlayerID,aiUin,iLen,pData)	\
    (This)->lpVtbl -> SendTcpP2pData(This,nPlayerCount,asPlayerID,aiUin,iLen,pData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IQQTpppSink_OnUdpLoginOk_Proxy( 
    IQQTpppSink * This);


void __RPC_STUB IQQTpppSink_OnUdpLoginOk_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTpppSink_OnRecvP2PDataFromSvr_Proxy( 
    IQQTpppSink * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nBodyLen,
    /* [size_is][in] */ BYTE *pBody);


void __RPC_STUB IQQTpppSink_OnRecvP2PDataFromSvr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTpppSink_OnGetPlayerInfoFromSvr_Proxy( 
    IQQTpppSink * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nUin);


void __RPC_STUB IQQTpppSink_OnGetPlayerInfoFromSvr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTpppSink_OnRecvClientTry2ConnectData_Proxy( 
    IQQTpppSink * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nUin,
    /* [in] */ ULONG ulTestMyIP,
    /* [in] */ USHORT usTestMyPort);


void __RPC_STUB IQQTpppSink_OnRecvClientTry2ConnectData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTpppSink_OnRecvP2PGameDataFromClient_Proxy( 
    IQQTpppSink * This,
    /* [in] */ short shSocketPlayerID,
    /* [in] */ int nSocketPlayerUin,
    /* [in] */ short shPlayerID,
    /* [in] */ int nBodyLen,
    /* [size_is][in] */ BYTE *pBody);


void __RPC_STUB IQQTpppSink_OnRecvP2PGameDataFromClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTpppSink_SendTcpP2pData_Proxy( 
    IQQTpppSink * This,
    /* [in] */ int nPlayerCount,
    /* [in] */ short *asPlayerID,
    /* [in] */ int *aiUin,
    /* [in] */ int iLen,
    /* [size_is][in] */ BYTE *pData);


void __RPC_STUB IQQTpppSink_SendTcpP2pData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQQTpppSink_INTERFACE_DEFINED__ */


#ifndef __IQQTppp_INTERFACE_DEFINED__
#define __IQQTppp_INTERFACE_DEFINED__

/* interface IQQTppp */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IQQTppp;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9F16F213-2E6F-4EB1-AFE1-E59B64754768")
    IQQTppp : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finalize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoginLocalPlayer( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ unsigned int uiServerIP,
            /* [in] */ unsigned short usSvrPort,
            /* [in] */ int shOtherPlayerID,
            /* [in] */ int nOtherUin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveAllRemotePlayers( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddHost( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ int shOtherPlayerID,
            /* [in] */ int nOtherUin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DelHost( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BroadCast( 
            /* [in] */ unsigned int uiLen,
            /* [size_is][in] */ unsigned char *pBuf,
            /* [in] */ int iIsGameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConfirmTransferIPPort( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConfirmReceiveData( 
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ ULONG ulIP,
            /* [in] */ USHORT usPort) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendP2pData2Player( 
            /* [in] */ short shPlayerID,
            /* [in] */ unsigned int uiUin,
            /* [in] */ unsigned int uiLen,
            /* [size_is][in] */ unsigned char *pBuf,
            /* [in] */ int iIsGameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetGameStatus( 
            /* [in] */ int iGameStatus,
            /* [in] */ unsigned int uiRoundMark) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSink( 
            /* [in] */ IQQTpppSink *pQQTpppSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelSink( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnRecvP2pTcpData( 
            /* [in] */ short sPlayerID,
            /* [in] */ int iUin,
            /* [in] */ int iLen,
            /* [size_is][in] */ BYTE *pData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQQTpppVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQQTppp * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQQTppp * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQQTppp * This);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IQQTppp * This);
        
        HRESULT ( STDMETHODCALLTYPE *Finalize )( 
            IQQTppp * This);
        
        HRESULT ( STDMETHODCALLTYPE *LoginLocalPlayer )( 
            IQQTppp * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ unsigned int uiServerIP,
            /* [in] */ unsigned short usSvrPort,
            /* [in] */ int shOtherPlayerID,
            /* [in] */ int nOtherUin);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveAllRemotePlayers )( 
            IQQTppp * This);
        
        HRESULT ( STDMETHODCALLTYPE *AddHost )( 
            IQQTppp * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ int shOtherPlayerID,
            /* [in] */ int nOtherUin);
        
        HRESULT ( STDMETHODCALLTYPE *DelHost )( 
            IQQTppp * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin);
        
        HRESULT ( STDMETHODCALLTYPE *BroadCast )( 
            IQQTppp * This,
            /* [in] */ unsigned int uiLen,
            /* [size_is][in] */ unsigned char *pBuf,
            /* [in] */ int iIsGameData);
        
        HRESULT ( STDMETHODCALLTYPE *ConfirmTransferIPPort )( 
            IQQTppp * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin);
        
        HRESULT ( STDMETHODCALLTYPE *ConfirmReceiveData )( 
            IQQTppp * This,
            /* [in] */ short shPlayerID,
            /* [in] */ int nUin,
            /* [in] */ ULONG ulIP,
            /* [in] */ USHORT usPort);
        
        HRESULT ( STDMETHODCALLTYPE *SendP2pData2Player )( 
            IQQTppp * This,
            /* [in] */ short shPlayerID,
            /* [in] */ unsigned int uiUin,
            /* [in] */ unsigned int uiLen,
            /* [size_is][in] */ unsigned char *pBuf,
            /* [in] */ int iIsGameData);
        
        HRESULT ( STDMETHODCALLTYPE *SetGameStatus )( 
            IQQTppp * This,
            /* [in] */ int iGameStatus,
            /* [in] */ unsigned int uiRoundMark);
        
        HRESULT ( STDMETHODCALLTYPE *SetSink )( 
            IQQTppp * This,
            /* [in] */ IQQTpppSink *pQQTpppSink);
        
        HRESULT ( STDMETHODCALLTYPE *CancelSink )( 
            IQQTppp * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnRecvP2pTcpData )( 
            IQQTppp * This,
            /* [in] */ short sPlayerID,
            /* [in] */ int iUin,
            /* [in] */ int iLen,
            /* [size_is][in] */ BYTE *pData);
        
        END_INTERFACE
    } IQQTpppVtbl;

    interface IQQTppp
    {
        CONST_VTBL struct IQQTpppVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQQTppp_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQQTppp_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQQTppp_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQQTppp_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#define IQQTppp_Finalize(This)	\
    (This)->lpVtbl -> Finalize(This)

#define IQQTppp_LoginLocalPlayer(This,shPlayerID,nUin,uiServerIP,usSvrPort,shOtherPlayerID,nOtherUin)	\
    (This)->lpVtbl -> LoginLocalPlayer(This,shPlayerID,nUin,uiServerIP,usSvrPort,shOtherPlayerID,nOtherUin)

#define IQQTppp_RemoveAllRemotePlayers(This)	\
    (This)->lpVtbl -> RemoveAllRemotePlayers(This)

#define IQQTppp_AddHost(This,shPlayerID,nUin,shOtherPlayerID,nOtherUin)	\
    (This)->lpVtbl -> AddHost(This,shPlayerID,nUin,shOtherPlayerID,nOtherUin)

#define IQQTppp_DelHost(This,shPlayerID,nUin)	\
    (This)->lpVtbl -> DelHost(This,shPlayerID,nUin)

#define IQQTppp_BroadCast(This,uiLen,pBuf,iIsGameData)	\
    (This)->lpVtbl -> BroadCast(This,uiLen,pBuf,iIsGameData)

#define IQQTppp_ConfirmTransferIPPort(This,shPlayerID,nUin)	\
    (This)->lpVtbl -> ConfirmTransferIPPort(This,shPlayerID,nUin)

#define IQQTppp_ConfirmReceiveData(This,shPlayerID,nUin,ulIP,usPort)	\
    (This)->lpVtbl -> ConfirmReceiveData(This,shPlayerID,nUin,ulIP,usPort)

#define IQQTppp_SendP2pData2Player(This,shPlayerID,uiUin,uiLen,pBuf,iIsGameData)	\
    (This)->lpVtbl -> SendP2pData2Player(This,shPlayerID,uiUin,uiLen,pBuf,iIsGameData)

#define IQQTppp_SetGameStatus(This,iGameStatus,uiRoundMark)	\
    (This)->lpVtbl -> SetGameStatus(This,iGameStatus,uiRoundMark)

#define IQQTppp_SetSink(This,pQQTpppSink)	\
    (This)->lpVtbl -> SetSink(This,pQQTpppSink)

#define IQQTppp_CancelSink(This)	\
    (This)->lpVtbl -> CancelSink(This)

#define IQQTppp_OnRecvP2pTcpData(This,sPlayerID,iUin,iLen,pData)	\
    (This)->lpVtbl -> OnRecvP2pTcpData(This,sPlayerID,iUin,iLen,pData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IQQTppp_Initialize_Proxy( 
    IQQTppp * This);


void __RPC_STUB IQQTppp_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_Finalize_Proxy( 
    IQQTppp * This);


void __RPC_STUB IQQTppp_Finalize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_LoginLocalPlayer_Proxy( 
    IQQTppp * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nUin,
    /* [in] */ unsigned int uiServerIP,
    /* [in] */ unsigned short usSvrPort,
    /* [in] */ int shOtherPlayerID,
    /* [in] */ int nOtherUin);


void __RPC_STUB IQQTppp_LoginLocalPlayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_RemoveAllRemotePlayers_Proxy( 
    IQQTppp * This);


void __RPC_STUB IQQTppp_RemoveAllRemotePlayers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_AddHost_Proxy( 
    IQQTppp * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nUin,
    /* [in] */ int shOtherPlayerID,
    /* [in] */ int nOtherUin);


void __RPC_STUB IQQTppp_AddHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_DelHost_Proxy( 
    IQQTppp * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nUin);


void __RPC_STUB IQQTppp_DelHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_BroadCast_Proxy( 
    IQQTppp * This,
    /* [in] */ unsigned int uiLen,
    /* [size_is][in] */ unsigned char *pBuf,
    /* [in] */ int iIsGameData);


void __RPC_STUB IQQTppp_BroadCast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_ConfirmTransferIPPort_Proxy( 
    IQQTppp * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nUin);


void __RPC_STUB IQQTppp_ConfirmTransferIPPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_ConfirmReceiveData_Proxy( 
    IQQTppp * This,
    /* [in] */ short shPlayerID,
    /* [in] */ int nUin,
    /* [in] */ ULONG ulIP,
    /* [in] */ USHORT usPort);


void __RPC_STUB IQQTppp_ConfirmReceiveData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_SendP2pData2Player_Proxy( 
    IQQTppp * This,
    /* [in] */ short shPlayerID,
    /* [in] */ unsigned int uiUin,
    /* [in] */ unsigned int uiLen,
    /* [size_is][in] */ unsigned char *pBuf,
    /* [in] */ int iIsGameData);


void __RPC_STUB IQQTppp_SendP2pData2Player_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_SetGameStatus_Proxy( 
    IQQTppp * This,
    /* [in] */ int iGameStatus,
    /* [in] */ unsigned int uiRoundMark);


void __RPC_STUB IQQTppp_SetGameStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_SetSink_Proxy( 
    IQQTppp * This,
    /* [in] */ IQQTpppSink *pQQTpppSink);


void __RPC_STUB IQQTppp_SetSink_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_CancelSink_Proxy( 
    IQQTppp * This);


void __RPC_STUB IQQTppp_CancelSink_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQQTppp_OnRecvP2pTcpData_Proxy( 
    IQQTppp * This,
    /* [in] */ short sPlayerID,
    /* [in] */ int iUin,
    /* [in] */ int iLen,
    /* [size_is][in] */ BYTE *pData);


void __RPC_STUB IQQTppp_OnRecvP2pTcpData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQQTppp_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


