/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright (c) 1997-2003 John Robbins -- All rights reserved.

DESCRIPTION :
    ProfileLib.LIB is a simple set of libraries to make your .NET
profiler development easier.  The library will take care of all the
COM goo for you and provides a base class that implements all the
methods on ICorProfilerCallback so you only have to implements the
ones you need for your particular tool.

While the name "profiler" in the ICorProfilerCallback interface name
indicates the interface is for writing profilers, it really allows you
do to a whole lot more.  ICorProfilerCallback ROCKS!

Steps to ProfileLib.lib nirvana:

1. Set up your build to link against ProfileLib.LIB and include this
   file in your STDAFX.H file.
2. In one of your CPP files, define the following variables using the
   exact names specified.  Note that all strings are UNICODE strings.
   A. G_szProfilerGUID - The GUID string for your profiler.
   B. G_CLSID_PROFILER - The actual GUID for your profiler.
   C. G_szProgIDPrefix - The program ID prefix string.
   D. G_szProfilerName - Your profiler name.
3. Implement a DllMain in your code.  Linking in a DllMain is a pain in
   the behind so instead of making you do custom linking to avoid linker
   errors, it's easier to have you do your own.  Clip the following out
   and drop it into your code.  If you do your own, make sure you set
   G_hInst to the DLL HINSTANCE.

    extern "C" BOOL WINAPI DllMain ( HINSTANCE hInstance ,
                                     DWORD     dwReason  ,
                                     LPVOID               )
    {
        switch ( dwReason )
        {
            case DLL_PROCESS_ATTACH:
                DisableThreadLibraryCalls ( hInstance ) ;
                G_hInst = hInstance ;
                break ;
            default :
                break ;
        }
        return( TRUE ) ;
    }
4.  Derive your profiler class from CBaseProfilerCallback.
5.  Implement the function that allocates you class:
    ICorProfilerCallback * AllocateProfilerCallback ( ) ;
    This will be called by ProfileLib code to get your profiler started
    up.
6.  Include a .DEF file in your project based on EXAMPLE.DEF from
    ProfilerLib.

----------------------------------------------------------------------*/
#ifndef _COMINFRASTRUCTURE_H
#define _COMINFRASTRUCTURE_H

#pragma once
#include <cor.h>
#include <corhdr.h>
#include <corprof.h>
#include <cordebug.h>
#include "BaseProfilerCallback.h"
#pragma comment ( lib , "corguids.lib" )

// The COM object GUID string.
extern wchar_t * G_szProfilerGUID ;

// The COM object CLSID
extern GUID G_CLSID_PROFILER ;

// The program ID prefix.
extern wchar_t * G_szProgIDPrefix ;

// The name for your profiler.
extern wchar_t * G_szProfilerName ;

// The function you implement to return your ICorProfilerCallback
// class.
ICorProfilerCallback * AllocateProfilerCallback ( ) ;

// Declared for you.  In your DLL main, set this to the DLL HINSTANCE.
extern HINSTANCE G_hInst ;

#endif  // _COMINFRASTRUCTURE_H

