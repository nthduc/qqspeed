

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Dec 20 16:50:02 2006
 */
/* Compiler settings for \Kart\include\network\P2PAdapter.idl:
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


#ifndef __P2PAdapter_h__
#define __P2PAdapter_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __P2PAdapter_FWD_DEFINED__
#define __P2PAdapter_FWD_DEFINED__

#ifdef __cplusplus
typedef class P2PAdapter P2PAdapter;
#else
typedef struct P2PAdapter P2PAdapter;
#endif /* __cplusplus */

#endif 	/* __P2PAdapter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __P2PADAPTERLib_LIBRARY_DEFINED__
#define __P2PADAPTERLib_LIBRARY_DEFINED__

/* library P2PADAPTERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_P2PADAPTERLib;

EXTERN_C const CLSID CLSID_P2PAdapter;

#ifdef __cplusplus

class DECLSPEC_UUID("0F9306A2-B0FD-42FD-9BCF-8418A17C781A")
P2PAdapter;
#endif
#endif /* __P2PADAPTERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


