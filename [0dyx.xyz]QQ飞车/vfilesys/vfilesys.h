

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Sep 25 11:04:52 2006
 */
/* Compiler settings for \Coding\Project\include\framework\vfilesys.idl:
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

#ifndef __vfilesys_h__
#define __vfilesys_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IVFileSystem_FWD_DEFINED__
#define __IVFileSystem_FWD_DEFINED__
typedef interface IVFileSystem IVFileSystem;
#endif 	/* __IVFileSystem_FWD_DEFINED__ */


#ifndef __VFileSysImp_FWD_DEFINED__
#define __VFileSysImp_FWD_DEFINED__

#ifdef __cplusplus
typedef class VFileSysImp VFileSysImp;
#else
typedef struct VFileSysImp VFileSysImp;
#endif /* __cplusplus */

#endif 	/* __VFileSysImp_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_vfilesys_0000 */
/* [local] */ 

typedef BYTE *PBYTE;

typedef DWORD VFHANDLE;



extern RPC_IF_HANDLE __MIDL_itf_vfilesys_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vfilesys_0000_v0_0_s_ifspec;

#ifndef __IVFileSystem_INTERFACE_DEFINED__
#define __IVFileSystem_INTERFACE_DEFINED__

/* interface IVFileSystem */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVFileSystem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("313A97EA-329E-4B88-8505-197A5422DCF2")
    IVFileSystem : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE FileOpen( 
            LPCSTR fname,
            LPCSTR mode,
            VFHANDLE *fh) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileClose( 
            VFHANDLE fh) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileRead( 
            VFHANDLE fh,
            PBYTE buffer,
            INT size,
            INT count,
            INT *__MIDL_0011) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileWrite( 
            VFHANDLE fh,
            PBYTE buffer,
            INT size,
            INT count,
            INT *__MIDL_0012) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileSeek( 
            VFHANDLE fh,
            LONG offset,
            INT origin,
            INT *__MIDL_0013) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileTell( 
            VFHANDLE fh,
            INT *__MIDL_0014) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileEOF( 
            VFHANDLE fh,
            BOOL *__MIDL_0015) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVFileSystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVFileSystem * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVFileSystem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVFileSystem * This);
        
        HRESULT ( STDMETHODCALLTYPE *FileOpen )( 
            IVFileSystem * This,
            LPCSTR fname,
            LPCSTR mode,
            VFHANDLE *fh);
        
        HRESULT ( STDMETHODCALLTYPE *FileClose )( 
            IVFileSystem * This,
            VFHANDLE fh);
        
        HRESULT ( STDMETHODCALLTYPE *FileRead )( 
            IVFileSystem * This,
            VFHANDLE fh,
            PBYTE buffer,
            INT size,
            INT count,
            INT *__MIDL_0011);
        
        HRESULT ( STDMETHODCALLTYPE *FileWrite )( 
            IVFileSystem * This,
            VFHANDLE fh,
            PBYTE buffer,
            INT size,
            INT count,
            INT *__MIDL_0012);
        
        HRESULT ( STDMETHODCALLTYPE *FileSeek )( 
            IVFileSystem * This,
            VFHANDLE fh,
            LONG offset,
            INT origin,
            INT *__MIDL_0013);
        
        HRESULT ( STDMETHODCALLTYPE *FileTell )( 
            IVFileSystem * This,
            VFHANDLE fh,
            INT *__MIDL_0014);
        
        HRESULT ( STDMETHODCALLTYPE *FileEOF )( 
            IVFileSystem * This,
            VFHANDLE fh,
            BOOL *__MIDL_0015);
        
        END_INTERFACE
    } IVFileSystemVtbl;

    interface IVFileSystem
    {
        CONST_VTBL struct IVFileSystemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVFileSystem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVFileSystem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVFileSystem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVFileSystem_FileOpen(This,fname,mode,fh)	\
    (This)->lpVtbl -> FileOpen(This,fname,mode,fh)

#define IVFileSystem_FileClose(This,fh)	\
    (This)->lpVtbl -> FileClose(This,fh)

#define IVFileSystem_FileRead(This,fh,buffer,size,count,__MIDL_0011)	\
    (This)->lpVtbl -> FileRead(This,fh,buffer,size,count,__MIDL_0011)

#define IVFileSystem_FileWrite(This,fh,buffer,size,count,__MIDL_0012)	\
    (This)->lpVtbl -> FileWrite(This,fh,buffer,size,count,__MIDL_0012)

#define IVFileSystem_FileSeek(This,fh,offset,origin,__MIDL_0013)	\
    (This)->lpVtbl -> FileSeek(This,fh,offset,origin,__MIDL_0013)

#define IVFileSystem_FileTell(This,fh,__MIDL_0014)	\
    (This)->lpVtbl -> FileTell(This,fh,__MIDL_0014)

#define IVFileSystem_FileEOF(This,fh,__MIDL_0015)	\
    (This)->lpVtbl -> FileEOF(This,fh,__MIDL_0015)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IVFileSystem_FileOpen_Proxy( 
    IVFileSystem * This,
    LPCSTR fname,
    LPCSTR mode,
    VFHANDLE *fh);


void __RPC_STUB IVFileSystem_FileOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVFileSystem_FileClose_Proxy( 
    IVFileSystem * This,
    VFHANDLE fh);


void __RPC_STUB IVFileSystem_FileClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVFileSystem_FileRead_Proxy( 
    IVFileSystem * This,
    VFHANDLE fh,
    PBYTE buffer,
    INT size,
    INT count,
    INT *__MIDL_0011);


void __RPC_STUB IVFileSystem_FileRead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVFileSystem_FileWrite_Proxy( 
    IVFileSystem * This,
    VFHANDLE fh,
    PBYTE buffer,
    INT size,
    INT count,
    INT *__MIDL_0012);


void __RPC_STUB IVFileSystem_FileWrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVFileSystem_FileSeek_Proxy( 
    IVFileSystem * This,
    VFHANDLE fh,
    LONG offset,
    INT origin,
    INT *__MIDL_0013);


void __RPC_STUB IVFileSystem_FileSeek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVFileSystem_FileTell_Proxy( 
    IVFileSystem * This,
    VFHANDLE fh,
    INT *__MIDL_0014);


void __RPC_STUB IVFileSystem_FileTell_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVFileSystem_FileEOF_Proxy( 
    IVFileSystem * This,
    VFHANDLE fh,
    BOOL *__MIDL_0015);


void __RPC_STUB IVFileSystem_FileEOF_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVFileSystem_INTERFACE_DEFINED__ */



#ifndef __VFILESYSLib_LIBRARY_DEFINED__
#define __VFILESYSLib_LIBRARY_DEFINED__

/* library VFILESYSLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_VFILESYSLib;

EXTERN_C const CLSID CLSID_VFileSysImp;

#ifdef __cplusplus

class DECLSPEC_UUID("D5162801-84C9-485C-A5FA-C36802909024")
VFileSysImp;
#endif
#endif /* __VFILESYSLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


