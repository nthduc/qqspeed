/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

// The strings passed to each function.
static char * g_szStdCall   = "__stdcall"   ;
static char * g_szCdeclCall = "__cdecl"     ;
static char * g_szFastCall  = "__fastcall"  ;
static char * g_szNakedCall = "__naked"     ;

// The extern "C" turns off all C++ name decoration.
extern "C"
{

// The __cdecl function.
void CDeclFunction ( char *        szString ,
                     unsigned long ulLong   ,
                     char          chChar    ) ;

// The __stdcall function.
void __stdcall StdCallFunction ( char *        szString ,
                                 unsigned long ulLong   ,
                                 char          chChar    ) ;
// The __fastcall function.
void __fastcall FastCallFunction ( char *        szString ,
                                   unsigned long ulLong   ,
                                   char          chChar    ) ;

// The naked function.  The declspec goes on the definition, not the
// declaration.
int NakedCallFunction ( char *        szString  ,
                        unsigned long ulLong    ,
                        char          chChar     ) ;
}

void main ( void )
{
    // Call each function to generate the code.  I separate each of them
    // with a couple of NOP bytes to make it easier to read the
    // disassembly.
    __asm NOP __asm NOP
    CDeclFunction ( g_szCdeclCall , 1 , 'a' ) ;
    __asm NOP __asm NOP
    StdCallFunction ( g_szStdCall , 2 , 'b' ) ;
    __asm NOP __asm NOP
    FastCallFunction ( g_szFastCall , 3 , 'c' ) ;
    __asm NOP __asm NOP
    NakedCallFunction ( g_szNakedCall , 4 , 'd' ) ;
    __asm NOP __asm NOP

}

void CDeclFunction ( char *        szString ,
                     unsigned long ulLong   ,
                     char          chChar    )
{
    __asm NOP __asm NOP
}

void __stdcall StdCallFunction ( char *        szString ,
                                 unsigned long ulLong   ,
                                 char          chChar    )
{
    __asm NOP __asm NOP
}

void __fastcall FastCallFunction ( char *        szString ,
                                   unsigned long ulLong   ,
                                   char          chChar    )
{
    __asm NOP __asm NOP
}

__declspec(naked) int NakedCallFunction ( char *        szString  ,
                                          unsigned long ulLong    ,
                                          char          chChar     )
{
    __asm NOP __asm NOP
    // Naked functions must EXPLICITLY do a return.
    __asm RET
}


