/*
**  Engine.h
**  engine header file.
**
**  Jim, 2003.6.23.
**
*/
#ifndef  _ENGINE_H_
#define  _ENGINE_H_

// I N C L U D E S ///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4100)   //unreferenced formal parameter
#pragma warning(disable:4201)   //nonstandard extension used : nameless struct/union
#pragma warning(disable:4505)   //unreferenced local function has been removed

#define _WIN32_WINNT    (0x0500)
#include <objbase.h>
#include <basetsd.h>
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <string.h>
#include <process.h>
#include <direct.h>
#include <stdlib.h>
#include <ctype.h>
#include <io.h>
#include <time.h>
#include <fcntl.h>
#include <malloc.h>
#include <dos.h>
#include <stdio.h>
#include <stdarg.h>
#include <lmcons.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
//#include <dxerr8.h>


// L I B R A R I E S /////////////////////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib,"winmm")
#pragma comment(lib,"ddraw")
#pragma comment(lib,"dsound")
#pragma comment(lib,"dplayx")
#pragma comment(lib,"dxguid")



// T Y P E D E F S ///////////////////////////////////////////////////////////////////////////////////////////////////
typedef signed char        SCHR;
typedef unsigned char      UCHR;
typedef signed char        SSTR;
typedef unsigned char      USTR;
typedef signed short int   SHINT;
typedef unsigned short int UHINT;
typedef signed long int    SLONG;
typedef signed __int64     SHYPER;
typedef unsigned __int64   UHYPER;


// D E F I N E S /////////////////////////////////////////////////////////////////////////////////////////////////////
#undef  ANSI
#ifdef  __STDC__
#define ANSI
#endif
#ifdef  __GNUC__
#define ANSI
#endif
#ifdef  WIN32
#define ANSI
#endif

#ifdef  ANSI
#define PROTOTYPE(func,para)      func para
#define ARGUMENTS(para)           para
#else
#define PROTOTYPE(func,para)      func()
#define ARGUMENTS(para)           ()
#endif


#define VOID        void


//-------------------------------------------------------------------------------------------------
// For make Dynamic-Link Library(DLL), we will define MAKEDLL upon with system's predefine _USRDLL.
//-------------------------------------------------------------------------------------------------
#undef  MAKEDLL
#ifdef  _USRDLL
#define MAKEDLL
#endif//_USRDLL


//-------------------------------------------------------------------------------------------------
// For make Static Library(LIB), we will define MAKELIB upon with system's predefine _LIB.
//-------------------------------------------------------------------------------------------------
#undef  MAKELIB
#ifdef  _LIB
#define MAKELIB
#endif//_LIB


//-------------------------------------------------------------------------------------------------
// For make Win32 Debug version, we will define DEBUG upon with system's predefine _DEBUG.
//-------------------------------------------------------------------------------------------------
#undef  DEBUG
#ifdef  _DEBUG
#define DEBUG
#endif//_DEBUG



//-------------------------------------------------------------------------------------------------
// For make our DLL.
//-------------------------------------------------------------------------------------------------
#undef  EXPORT
#undef  FNBACK

#ifdef  MAKEDLL
#define EXPORT      extern __declspec(dllexport)  
#define FNBACK      CALLBACK
#else//!MAKEDLL
#define EXPORT      extern
#define FNBACK    
#endif//MAKEDLL


//-------------------------------------------------------------------------------------------------
#ifndef TRUE
#define TRUE    (1==1)
#endif
#ifndef FALSE
#define FALSE   (1!=1)
#endif


#define TTN_OK              0
#define TTN_ACTIVE          1
#define TTN_ERROR           -1
#define TTN_NOT_OK          -2

#define SCREEN_BPP          16

#define COPY_PUT            0x00
#define PEST_PUT            0x01


#define GAME_CONTINUE       0
#define GAME_TEAM_OVER      1
#define GAME_BREAK          2
#define GAME_END            3
#define GAME_DEVIL_END      4
#define GAME_DEVIL_DEAD     5
#define GAME_LOAD           6
#define GAME_RUN_OK         7
#define GAME_EXEC           8
#define GAME_OVER           -1
#define GAME_DUMMY          100



// M A C R O   D E F I N E S //////////////////////////////////////////////////////////////////////////////////////////
#define ABS(a)              (((a)<0)?-(a):(a))
#define FLOOR(a)            (((a)>0)?(int)(a):-(int)(-(a)))
#define CEILING(a)          ((a)==(int)(a)?(a):(a)>0?1+(int)(a):-(1+(int)(-a)))
#define ROUND(a)            ((a)>0?(int)(a+0.5):-(int)(0.5-a))
#define ZSGN(a)             (((a)<0)?-1:(a)>0?1:0)
#define SGN(a)              (((a)<0)?-1:1)
#define SQR(a)              ((a)*(a))
#define ASSERTX(x)          if(!(x)) fprintf(stderr,"Assert failed:x\n");
#define MIN(a,b)            (((a)<(b))?(a):(b))
#define MAX(a,b)            (((a)>(b))?(a):(b))
#define SWAP(a,b)           {a^=b;b^=a;a^=b;}
#define LERP(a,l,h)         ((l)+(((h)-(l))*(a)))
#define CLAMP(v,l,h)        (((v)<(l))?(l):(v)>(h)?(h):(v))
#define NEWSTRUCT(x)        (struct x *)(GlobalAlloc(GPTR, (unsigned)sizeof(struct x)))
#define NEWTYPE(x)          (x *)(GlobalAlloc(GPTR, (unsigned)sizeof(x)))
#define FREE(x);            {if((x) !=NULL){GlobalFree(x);(x)=NULL;}}

#define QUOTE(x)            #x
#define QQUOTE(y)           QUOTE(y)
#define REMIND(str)         __FILE__ "(" QQUOTE(__LINE__) "):" str


// S T R U C T S /////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagD2POINT
{
   SLONG x;
   SLONG y;
} D2POINT,*LPD2POINT;

typedef struct tagD3POINT
{
   SLONG x;
   SLONG y;
   SLONG z;
} D3POINT,*LPD3POINT;

typedef struct tagD2LINE
{
   SLONG x1,y1;
   SLONG x2,y2;
} D2LINE,*LPD2LINE;

typedef struct tagD3LINE
{
   SLONG x1,y1,z1;
   SLONG x2,y2,z2;
} D3LINE,*LPD3LINE;

typedef struct tagWINDOW
{
   SLONG x1,y1;
   SLONG x2,y2;
} WINDOW,*LPWINDOW;

typedef struct tagBODY
{
   SLONG x1,y1,z1;
   SLONG x2,y2,z2;
} BODY,*LPBODY;


// E N U M S //////////////////////////////////////////////////////////////////////////////////////////////////////////
enum KEY_CODES 
{   Backspace   =   0x08,
Tab             =   0x09,
BackTab         =   0x0f,
Lf              =   0x0a,
Enter           =   0x0d,
Esc             =   0x1b,
Blank           =   0x20,
Plus            =   0x2b,
Comma           =   0x2c,
Dot             =   0x2e,
Minus           =   0x2d,
Zero            =   0x30,
Colon           =   0x3a,
KB_F1           =   0x3b,
KB_F2           =   0x3c,
KB_F3           =   0x3d,
KB_F4           =   0x3e,
KB_F5           =   0x3f,
KB_F6           =   0x40,
KB_F7           =   0x41,
KB_F8           =   0x42,
KB_F9           =   0x43,
KB_F10          =   0x44,
KB_F11          =   0x85,
KB_F12          =   0x86,
KB_F13          =   0x8d,
KB_F14          =   0x8e,
KB_F15          =   0x8f,
Home            =   0x47,
Up              =   0x48,
PgUp            =   0x49,
Left            =   0x4b,
Right           =   0x4d,
End             =   0x4f,
Dn              =   0x50,
PgDn            =   0x51,
Ins             =   0x52,
Del             =   0x53,
Shift_F1        =   0x54,
Shift_F2        =   0x55,
Shift_F3        =   0x56,
Shift_F4        =   0x57,
Shift_F5        =   0x58,
Shift_F6        =   0x59,
Shift_F7        =   0x5a,
Shift_F8        =   0x5b,
Shift_F9        =   0x5c,
Shift_F10       =   0x5d,
Shift_F11       =   0x87,
Shift_F12       =   0x88,
Shift_F13       =   0x90,
Shift_F14       =   0x91,
Shift_F15       =   0x92,
Ctrl_F1         =   0x5e,
Ctrl_F2         =   0x5f,
Ctrl_F3         =   0x60,
Ctrl_F4         =   0x61,
Ctrl_F5         =   0x62,
Ctrl_F6         =   0x63,
Ctrl_F7         =   0x64,
Ctrl_F8         =   0x65,
Ctrl_F9         =   0x66,
Ctrl_F10        =   0x67,
Ctrl_F11        =   0x89,
Ctrl_F12        =   0x8a,
Ctrl_F13        =   0x93,
Ctrl_F14        =   0x94,
Ctrl_F15        =   0x95,
Alt_F1          =   0x68,
Alt_F2          =   0x69,
Alt_F3          =   0x6a,
Alt_F4          =   0x6b,
Alt_F5          =   0x6c,
Alt_F6          =   0x6d,
Alt_F7          =   0x6e,
Alt_F8          =   0x6f,
Alt_F9          =   0x70,
Alt_F10         =   0x71,
Alt_F11         =   0x8b,
Alt_F12         =   0x8c,
Alt_F13         =   0x96,
Alt_F14         =   0x97,
Alt_F15         =   0x98,
Ctrl_End        =   0x75,
Ctrl_PgDn       =   0x76,
Ctrl_Home       =   0x77,
Alt_1           =   0x78,
Alt_2           =   0x79,
Alt_3           =   0x7a,
Alt_4           =   0x7b,
Alt_5           =   0x7c,
Alt_6           =   0x7d,
Alt_7           =   0x7e,
Alt_8           =   0x7f,
Alt_9           =   0x80,
Alt_0           =   0x81,
L_Shift         =   0x82,
R_Shift         =   0x83,
Ctrl            =   0x84,
Alt             =   0x85,
Num_5           =   0x87
};

enum KEY_SPECIAL_CODES 
{   S_Backspace =   0x08,
S_Tab           =   0x89,
S_BackTab       =   0x8f,
S_Lf            =   0x8a,
S_Enter         =   0x8d,
S_Esc           =   0x9b,
S_Blank         =   0xA0,
S_Plus          =   0xAb,
S_Comma         =   0xAc,
S_Dot           =   0xAe,
S_Minus         =   0xAd,
S_Zero          =   0xB0,
S_Colon         =   0xBa,
S_KB_F1         =   0xBb,
S_KB_F2         =   0xBc,
S_KB_F3         =   0xBd,
S_KB_F4         =   0xBe,
S_KB_F5         =   0xBf,
S_KB_F6         =   0xC0,
S_KB_F7         =   0xC1,
S_KB_F8         =   0xC2,
S_KB_F9         =   0xC3,
S_KB_F10        =   0xC4,
S_KB_F11        =   0xC5,
S_KB_F12        =   0xC6,
S_Home          =   0xC7,
S_Up            =   0xC8,
S_PgUp          =   0xC9,
S_Left          =   0xCb,
S_Right         =   0xCd,
S_End           =   0xCf,
S_Dn            =   0xD0,
S_PgDn          =   0xD1,
S_Ins           =   0xD2,
S_Del           =   0xD3,
};

enum    MOUSE_KEY_CODES
{   MS_Dummy    =   0x00,
MS_Move         =   0xF1,
MS_LDrag        =   0xF2,
MS_RDrag        =   0xF3,
MS_LDn          =   0xF4,
MS_LUp          =   0xF5,
MS_LDblClk      =   0xF6,
MS_RDn          =   0xF7,
MS_RUp          =   0xF8,
MS_RDblClk      =   0xF9,
MS_MDn          =   0xFA,
MS_MUp          =   0xFB,
MS_MDblClk      =   0xFC,
MS_Forward      =   0xFD,
MS_Backward     =   0xFE,
MS_MDrag        =   0xFF,
};


#endif//_ENGINE_H_
