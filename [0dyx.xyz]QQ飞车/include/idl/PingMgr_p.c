

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "PingMgr_i.h"

#define TYPE_FORMAT_STRING_SIZE   25                                
#define PROC_FORMAT_STRING_SIZE   181                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _PingMgr_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } PingMgr_MIDL_TYPE_FORMAT_STRING;

typedef struct _PingMgr_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } PingMgr_MIDL_PROC_FORMAT_STRING;

typedef struct _PingMgr_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } PingMgr_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const PingMgr_MIDL_TYPE_FORMAT_STRING PingMgr__MIDL_TypeFormatString;
extern const PingMgr_MIDL_PROC_FORMAT_STRING PingMgr__MIDL_ProcFormatString;
extern const PingMgr_MIDL_EXPR_FORMAT_STRING PingMgr__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPingSink_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPingSink_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPingMgr_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPingMgr_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const PingMgr_MIDL_PROC_FORMAT_STRING PingMgr__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure NotifyPingResult */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 10 */	NdrFcShort( 0x20 ),	/* 32 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nIndex */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nTTL */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter fLossRate */

/* 36 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter nAverageime */

/* 42 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 44 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 48 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 50 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 52 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PingServer */

/* 54 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 56 */	NdrFcLong( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x3 ),	/* 3 */
/* 62 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 64 */	NdrFcShort( 0x32 ),	/* 50 */
/* 66 */	NdrFcShort( 0x8 ),	/* 8 */
/* 68 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 70 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0x0 ),	/* 0 */
/* 76 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ulSvrIP */

/* 78 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 80 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter usPort */

/* 84 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 86 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 88 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pnIndex */

/* 90 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 92 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nLimitTime */

/* 96 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 98 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CancelPing */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x4 ),	/* 4 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nIndex */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Initialize */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x5 ),	/* 5 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pPingSink */

/* 168 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const PingMgr_MIDL_TYPE_FORMAT_STRING PingMgr__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  8 */	NdrFcLong( 0x23f98a6c ),	/* 603556460 */
/* 12 */	NdrFcShort( 0x5cdb ),	/* 23771 */
/* 14 */	NdrFcShort( 0x43a6 ),	/* 17318 */
/* 16 */	0xbf,		/* 191 */
			0x32,		/* 50 */
/* 18 */	0x12,		/* 18 */
			0x34,		/* 52 */
/* 20 */	0x56,		/* 86 */
			0x14,		/* 20 */
/* 22 */	0x14,		/* 20 */
			0x76,		/* 118 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IPingSink, ver. 0.0,
   GUID={0x23F98A6C,0x5CDB,0x43A6,{0xBF,0x32,0x12,0x34,0x56,0x14,0x14,0x76}} */

#pragma code_seg(".orpc")
static const unsigned short IPingSink_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IPingSink_ProxyInfo =
    {
    &Object_StubDesc,
    PingMgr__MIDL_ProcFormatString.Format,
    &IPingSink_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPingSink_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PingMgr__MIDL_ProcFormatString.Format,
    &IPingSink_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _IPingSinkProxyVtbl = 
{
    &IPingSink_ProxyInfo,
    &IID_IPingSink,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IPingSink::NotifyPingResult */
};

const CInterfaceStubVtbl _IPingSinkStubVtbl =
{
    &IID_IPingSink,
    &IPingSink_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IPingMgr, ver. 0.0,
   GUID={0xA986B5C9,0x71A3,0x44d0,{0x98,0xBA,0xE4,0x32,0x1F,0x12,0x34,0x56}} */

#pragma code_seg(".orpc")
static const unsigned short IPingMgr_FormatStringOffsetTable[] =
    {
    54,
    108,
    144
    };

static const MIDL_STUBLESS_PROXY_INFO IPingMgr_ProxyInfo =
    {
    &Object_StubDesc,
    PingMgr__MIDL_ProcFormatString.Format,
    &IPingMgr_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPingMgr_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PingMgr__MIDL_ProcFormatString.Format,
    &IPingMgr_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) _IPingMgrProxyVtbl = 
{
    &IPingMgr_ProxyInfo,
    &IID_IPingMgr,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IPingMgr::PingServer */ ,
    (void *) (INT_PTR) -1 /* IPingMgr::CancelPing */ ,
    (void *) (INT_PTR) -1 /* IPingMgr::Initialize */
};

const CInterfaceStubVtbl _IPingMgrStubVtbl =
{
    &IID_IPingMgr,
    &IPingMgr_ServerInfo,
    6,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    PingMgr__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _PingMgr_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IPingSinkProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPingMgrProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _PingMgr_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IPingSinkStubVtbl,
    ( CInterfaceStubVtbl *) &_IPingMgrStubVtbl,
    0
};

PCInterfaceName const _PingMgr_InterfaceNamesList[] = 
{
    "IPingSink",
    "IPingMgr",
    0
};


#define _PingMgr_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _PingMgr, pIID, n)

int __stdcall _PingMgr_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _PingMgr, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _PingMgr, 2, *pIndex )
    
}

const ExtendedProxyFileInfo PingMgr_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _PingMgr_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PingMgr_StubVtblList,
    (const PCInterfaceName * ) & _PingMgr_InterfaceNamesList,
    0, /* no delegation */
    & _PingMgr_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

