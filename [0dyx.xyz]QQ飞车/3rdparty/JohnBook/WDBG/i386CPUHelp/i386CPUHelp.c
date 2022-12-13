/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "pch.h"
#include "CPUHelp.h"
#include "LocalAssist.h"

// C4311: '' : pointer truncation from '' to ''
// C4312: '' : conversion from '' to '' of greater size
// C4244: '' : conversion from '' to '', possible loss of data
#pragma warning ( disable :  4311 )
#pragma warning ( disable :  4312 )
#pragma warning ( disable :  4244 )

// This is the TRACE0 flag bit in the 386 flags.
#define TF_BIT 0x100
// The breakpoint opcode for i386 and derivatives.
#define BREAK_OPCODE 0xCC


int CPUHELP_DLLINTERFACE __stdcall
    SetBreakpoint ( PDEBUGPACKET dp      ,
                    LPCVOID      ulAddr  ,
                    OPCODE *     pOpCode  )
{
    DWORD dwReadWrite = 0 ;
    BYTE bTempOp = BREAK_OPCODE ;
    BOOL bReadMem ;
    BOOL bWriteMem ;
    BOOL bFlush ;
    MEMORY_BASIC_INFORMATION mbi ;
    DWORD dwOldProtect ;


    ASSERT ( FALSE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET ) ) )  ;
    ASSERT ( FALSE == IsBadWritePtr ( pOpCode , sizeof ( OPCODE ) ) ) ;
    if ( ( TRUE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET ) ) ) ||
         ( TRUE == IsBadWritePtr ( pOpCode , sizeof ( OPCODE ) ) )   )
    {
        TRACE0 ( "SetBreakpoint : invalid parameters\n!" ) ;
        return ( FALSE ) ;
    }

    // Read the opcode at the location.
    bReadMem = DBG_ReadProcessMemory ( dp->hProcess   ,
                                      (LPCVOID)ulAddr ,
                                      &bTempOp        ,
                                      sizeof ( BYTE ) ,
                                      &dwReadWrite     ) ;
    ASSERT ( FALSE != bReadMem ) ;
    ASSERT ( sizeof ( BYTE ) == dwReadWrite ) ;
    if ( ( FALSE == bReadMem              ) ||
         ( sizeof ( BYTE ) != dwReadWrite ) )
    {
        return ( FALSE ) ;
    }

    // Is this new breakpoint about to overwrite an existing
    // breakpoint opcode?
    if ( BREAK_OPCODE == bTempOp )
    {
        return ( -1 ) ;
    }

    // Get the page attributes for the debuggee.
    DBG_VirtualQueryEx ( dp->hProcess                        ,
                         (LPCVOID)ulAddr                     ,
                         &mbi                                ,
                         sizeof ( MEMORY_BASIC_INFORMATION )  ) ;

    // Force the page to copy-on-write in the debuggee.
    if ( FALSE == DBG_VirtualProtectEx ( dp->hProcess           ,
                                         mbi.BaseAddress        ,
                                         mbi.RegionSize         ,
                                         PAGE_EXECUTE_READWRITE ,
                                         &mbi.Protect            ) )
    {
        ASSERT ( !"VirtualProtectEx failed!!" ) ;
        return ( FALSE ) ;
    }


    // Save the opcode I’m about to whack.
    *pOpCode = (void*)bTempOp ;

    bTempOp = BREAK_OPCODE ;
    dwReadWrite = 0 ;
    // The opcode was saved, so now set the breakpoint.
    bWriteMem = DBG_WriteProcessMemory ( dp->hProcess     ,
                                        (LPVOID)ulAddr   ,
                                        (LPVOID)&bTempOp ,
                                        sizeof ( BYTE )  ,
                                        &dwReadWrite      ) ;
    ASSERT ( FALSE != bWriteMem ) ;
    ASSERT ( sizeof ( BYTE ) == dwReadWrite ) ;


    if ( ( FALSE == bWriteMem             ) ||
         ( sizeof ( BYTE ) != dwReadWrite ) )
    {
        return ( FALSE ) ;
    }

    // Change the protection back to what it was before I blasted the
    // breakpoint in.
    VERIFY ( DBG_VirtualProtectEx ( dp->hProcess    ,
                                    mbi.BaseAddress ,
                                    mbi.RegionSize  ,
                                    mbi.Protect     ,
                                    &dwOldProtect    ) ) ;

    // Flush the instruction cache in case this memory was in the CPU
    // cache.
    bFlush = DBG_FlushInstructionCache ( dp->hProcess    ,
                                         (LPCVOID)ulAddr ,
                                         sizeof ( BYTE )  ) ;
    ASSERT ( TRUE == bFlush ) ;

    return ( TRUE ) ;
}

BOOL CPUHELP_DLLINTERFACE __stdcall
    ClearBreakpoint ( PDEBUGPACKET dp          ,
                      LPCVOID      ulAddr      ,
                      OPCODE       OpCode      ,
                      BOOL         bChangeRegs  )
{
    BYTE bTempOp = (BYTE)OpCode ;
    DWORD dwWrite = 0 ;
    BOOL bWriteMem ;
    BOOL bFlush ;
    MEMORY_BASIC_INFORMATION mbi ;
    DWORD dwOldProtect ;

    ASSERT ( FALSE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET) ) )  ;
    if ( TRUE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET ) ) )
    {
        TRACE0 ( "ClearBreakpoint : invalid parameters\n!" ) ;
        return ( FALSE ) ;
    }

    // Get the page attributes for the debuggee.
    DBG_VirtualQueryEx ( dp->hProcess                        ,
                         (LPCVOID)ulAddr                     ,
                         &mbi                                ,
                         sizeof ( MEMORY_BASIC_INFORMATION )  ) ;

    // Force the page to read only in the debuggee.
    if ( FALSE == DBG_VirtualProtectEx ( dp->hProcess           ,
                                         mbi.BaseAddress        ,
                                         mbi.RegionSize         ,
                                         PAGE_EXECUTE_READWRITE ,
                                         &mbi.Protect            ) )
    {
        ASSERT ( !"VirtualProtectEx failed!!" ) ;
        return ( FALSE ) ;
    }

    // Write out the opcode.
    bWriteMem = DBG_WriteProcessMemory ( dp->hProcess     ,
                                         (LPVOID)ulAddr   ,
                                         (LPVOID)&bTempOp ,
                                         sizeof ( BYTE )  ,
                                         &dwWrite          ) ;
    ASSERT ( FALSE != bWriteMem ) ;
    ASSERT ( 0 != dwWrite ) ;
    if ( ( FALSE == bWriteMem ) || ( 0 == dwWrite ) )
    {
        return ( FALSE ) ;
    }

    // Change the protection back to what it was before I removed the
    // BP.
    VERIFY ( DBG_VirtualProtectEx ( dp->hProcess    ,
                                    mbi.BaseAddress ,
                                    mbi.RegionSize  ,
                                    mbi.Protect     ,
                                    &dwOldProtect    ) ) ;

    bFlush = DBG_FlushInstructionCache ( dp->hProcess    ,
                                         (LPCVOID)ulAddr ,
                                         sizeof ( BYTE )  ) ;
    ASSERT ( TRUE == bFlush ) ;

    if ( TRUE == bChangeRegs )
    {
        // Decrement EIP so the CPU will execute the address just
        // changed.
        dp->context.Eip-- ;
        if ( FALSE == DBG_SetThreadContext ( dp->hThread  ,
                                             &dp->context  ) )
        {
            ASSERT ( !"SetThreadContext failed!" ) ;
            return ( FALSE ) ;
        }
    }

    return ( TRUE ) ;
}

BOOL CPUHELP_DLLINTERFACE __stdcall
    SetSingleStep ( PDEBUGPACKET dp )
{
    BOOL bSetContext ;
    ASSERT ( FALSE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET) ) )  ;
    if ( TRUE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET ) ) )
    {
        TRACE0 ( "SetSingleStep : invalid parameters\n!" ) ;
        return ( FALSE ) ;
    }

    // For the i386, just set the TF bit.
    dp->context.EFlags |= TF_BIT ;
    bSetContext = DBG_SetThreadContext ( dp->hThread , &dp->context ) ;
    ASSERT ( FALSE != bSetContext ) ;
    return ( bSetContext ) ;
}

BOOL CPUHELP_DLLINTERFACE __stdcall
    ClearSingleStep ( PDEBUGPACKET dp )
{
    BOOL bSetContext ;
    ASSERT ( FALSE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET) ) )  ;
    if ( TRUE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET ) ) )
    {
        TRACE0 ( "ClearSingleStep : invalid parameters\n!" ) ;
        return ( FALSE ) ;
    }
    // For the i386, just set the TF bit.
    dp->context.EFlags &= ~TF_BIT ;
    bSetContext = DBG_SetThreadContext ( dp->hThread , &dp->context ) ;
    ASSERT ( FALSE != bSetContext ) ;
    return ( bSetContext ) ;
}


BOOL CPUHELP_DLLINTERFACE __stdcall
    DisplayRegisters ( PDEBUGPACKET dp     ,
                       LPTSTR       szRegs ,
                       LPCTSTR      szCRLF  )
{
    ASSERT ( FALSE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET) ) )  ;
    ASSERT ( FALSE == IsBadStringPtr ( szRegs , 1024 ) ) ;
    ASSERT ( FALSE == IsBadStringPtr ( szCRLF , 20 ) ) ;
    if ( ( TRUE == IsBadReadPtr ( dp , sizeof ( DEBUGPACKET ) ) ) ||
         ( TRUE == IsBadStringPtr ( szRegs , 1024 )             ) ||
         ( TRUE == IsBadStringPtr ( szCRLF , 20   )             )   )
    {
        TRACE0 ( "DisplayRegisters : invalid parameters\n!" ) ;
        return ( FALSE ) ;
    }

    wsprintf ( szRegs                   ,
               _T ( "EAX = 0x%08X%s" )
               _T ( "EBX = 0x%08X%s" )
               _T ( "ECX = 0x%08X%s" )
               _T ( "EDX = 0x%08X%s" )
               _T ( "ESI = 0x%08X%s" )
               _T ( "EDI = 0x%08X%s" )
               _T ( "EIP = 0x%08X%s" )
               _T ( "ESP = 0x%08X%s" )
               _T ( "EBP = 0x%08X%s" )
               _T ( "EFL = 0x%08X"   )  ,
               dp->context.Eax          ,
               szCRLF                   ,
               dp->context.Ebx          ,
               szCRLF                   ,
               dp->context.Ecx          ,
               szCRLF                   ,
               dp->context.Edx          ,
               szCRLF                   ,
               dp->context.Esi          ,
               szCRLF                   ,
               dp->context.Edi          ,
               szCRLF                   ,
               dp->context.Eip          ,
               szCRLF                   ,
               dp->context.Esp          ,
               szCRLF                   ,
               dp->context.Ebp          ,
               szCRLF                   ,
               dp->context.EFlags        ) ;
    return ( TRUE ) ;
}

LPCVOID CPUHELP_DLLINTERFACE __stdcall
    ReturnInstructionPointer ( CONTEXT * pCtx )
{
    ASSERT ( FALSE == IsBadReadPtr ( pCtx , sizeof ( CONTEXT ) ) ) ;
    if ( TRUE == IsBadReadPtr ( pCtx , sizeof ( CONTEXT ) ) )
    {
        return ( 0 ) ;
    }
    return ( (LPCVOID)(pCtx->Eip) ) ;
}

BOOL CPUHELP_DLLINTERFACE __stdcall
    InitializeStackFrameWithContext ( STACKFRAME64 * pStack ,
                                      CONTEXT *      pCtx    )
{
    ASSERT ( FALSE == IsBadReadPtr ( pCtx , sizeof ( CONTEXT ) ) ) ;
    ASSERT ( FALSE == IsBadWritePtr ( pStack , sizeof ( STACKFRAME64) ));
    if ( ( TRUE == IsBadReadPtr ( pCtx , sizeof ( CONTEXT ) )       ) ||
         ( TRUE == IsBadWritePtr ( pStack , sizeof ( STACKFRAME ) ) )  )
    {
        return ( FALSE ) ;
    }

    pStack->AddrPC.Offset       = pCtx->Eip ;
    pStack->AddrPC.Mode         = AddrModeFlat   ;
    pStack->AddrStack.Offset    = pCtx->Esp ;
    pStack->AddrStack.Mode      = AddrModeFlat   ;
    pStack->AddrFrame.Offset    = pCtx->Ebp ;
    pStack->AddrFrame.Mode      = AddrModeFlat   ;

    return ( TRUE ) ;
}
