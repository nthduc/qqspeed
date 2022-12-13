/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Mar 09 15:38:47 2006
 */
/* Compiler settings for E:\coding\fishhong_QQTang_200410_Proj_int\IED_QQTang_VOB\QQTang\Client\source\IDL\IQQTPing.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IPingSink = {0x23F98A6C,0x5CDB,0x43A6,{0xBF,0x32,0x12,0x34,0x56,0x14,0x14,0x76}};


const IID IID_IPingMgr = {0xA986B5C9,0x71A3,0x44d0,{0x98,0xBA,0xE4,0x32,0x1F,0x12,0x34,0x56}};


const CLSID CLSID_CPingMgr = {0xEDCD6AE4,0x0A21,0x4d19,{0xA1,0xDD,0x87,0x83,0x60,0x65,0x43,0x21}};


#ifdef __cplusplus
}
#endif

