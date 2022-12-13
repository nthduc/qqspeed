/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
CPUHelp.h - The CPU specific header files.
----------------------------------------------------------------------*/
#ifndef _CPUHELP_H
#define _CPUHELP_H

#ifdef __cplusplus
extern "C" {
#endif

// The defines that set up how the functions or classes are exported or
// imported.
#ifndef CPUHELP_DLLINTERFACE
#ifdef BUILDING_CPUHELP_DLL
#define CPUHELP_DLLINTERFACE __declspec ( dllexport )
#else
#define CPUHELP_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_CPUHELP_DLL
#endif  // CPUHELP_DLLINTERFACE


// The debug packet that must be passed to each CPU function.  It was
// easier to define this than look at the Dr. Watson disassembler source
// code.
typedef struct tag_DEBUGPACKET
{
    HANDLE  hProcess ;
    HANDLE  hThread  ;
    CONTEXT context  ;
} DEBUGPACKET , * PDEBUGPACKET ;


/*----------------------------------------------------------------------
FUNCTION    :   disasm
DISCUSSION  :
    The disassembler for the CPU.  I swiped the i386 version from the
Dr. Watson SDK sample.  There are no guarentees that it works.  It does
not appear to handle anything past the 486 instruction set.
PARAMETERS  :
    dp      - The DEBUGPACKET.  The hProcess and context fields must be
              filled out.
    pOffset - The pointer to the offset to start disassembling.  On
              the return, this points to the next instruction in memory.
    pchDst  - Pointer to the resulting string.
    fEAOut  - TRUE if you want to see the effective address.
RETURNS     :
    TRUE  - Everything was disassembled.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL CPUHELP_DLLINTERFACE __stdcall
    disasm ( PDEBUGPACKET dp      ,
             PULONG       pOffset ,
             PUCHAR       pchDst  ,
             BOOL         fEAout   ) ;


// The typedef for opcodes.  The specific CPU versions will truncate or
// expand as appropriate.
typedef void * OPCODE ;

/*----------------------------------------------------------------------
FUNCTION    :   SetBreakpoint
DISCUSSION  :
    Sets a CPU specific breakpoint.
PARAMETERS  :
    dp      - The DEBUGPACKET.  The hProcess field must be filled in.
    ulAddr  - The address to set the breakpoint.
    pOpCode - The pointer to the opcode value replaced by the CPU
              specific breakpoint.
RETURNS     :
    TRUE  - The breakpoint was set.
    FALSE - There was a problem.
    -1    - A breakpoint opcode is already at the location ulAddr.
----------------------------------------------------------------------*/
int CPUHELP_DLLINTERFACE __stdcall
    SetBreakpoint ( PDEBUGPACKET dp      ,
                    LPCVOID      ulAddr  ,
                    OPCODE *     pOpCode  ) ;

/*----------------------------------------------------------------------
FUNCTION    :   ClearBreakpoint
DISCUSSION  :
    Clears a CPU specific breakpoint.
PARAMETERS  :
    dp          - The DEBUGPACKET.  You must fill in all fields.
    ulAddr      - The address to clear the breakpoint.
    OpCode      - The opcode to overwrite the breakpoint with.
    bChangeRegs - If you are clearing a breakpoint that has just
                  executed, set this flag to TRUE and this function will
                  reset the instruction pointer to rexecute the
                  instruction at ulAddr.
RETURNS     :
    TRUE  - The breakpoint was cleared.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL CPUHELP_DLLINTERFACE __stdcall
    ClearBreakpoint ( PDEBUGPACKET dp          ,
                      LPCVOID      ulAddr      ,
                      OPCODE       OpCode      ,
                      BOOL         bChangeRegs  ) ;

/*----------------------------------------------------------------------
FUNCTION    :   SetSingleStep
DISCUSSION  :
    Handles setting the CPU into single step mode.
PARAMETERS  :
    dp - The DEBUGPACKET.  The context field must be filled in.
RETURNS     :
    TRUE  - The single step was set.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL CPUHELP_DLLINTERFACE __stdcall
    SetSingleStep ( PDEBUGPACKET dp ) ;

/*----------------------------------------------------------------------
FUNCTION    :   ClearSingleStep
DISCUSSION  :
    Handles clearing the CPU from single step mode.
PARAMETERS  :
    dp - The DEBUGPACKET.  The context field must be filled in.
RETURNS     :
    TRUE  - The single step was cleared.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL CPUHELP_DLLINTERFACE __stdcall
    ClearSingleStep ( PDEBUGPACKET dp ) ;

/*----------------------------------------------------------------------
FUNCTION    :   DisplayRegisters
DISCUSSION  :
    Displays all the registers.  Each register is on a line of it's own.
PARAMETERS  :
    dp     - The DEBUGPACKET.  The context field must be filled in.
    szRegs - A string that is at least 1024 characters long.  On output,
             this holds the register display.
    szCRLF - The carraige return/line feed to use.
RETURNS     :
    TRUE  - The registers are in szRegs
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL CPUHELP_DLLINTERFACE __stdcall
    DisplayRegisters ( PDEBUGPACKET dp     ,
                       LPTSTR       szRegs ,
                       LPCTSTR      szCRLF  ) ;

// Note:  If you would like to extend WDBG.EXE, you could set it up to
// evaluate and set individual registers.

/*----------------------------------------------------------------------
FUNCTION    :   ReturnInstructionPointer
DISCUSSION  :
    Returns the instruction pointer value for the given context.
PARAMETERS  :
    pCtx - The context to use.
RETURNS     :
    0  - There was a problem.
    !0 - The instruction pointer value.
----------------------------------------------------------------------*/
LPCVOID CPUHELP_DLLINTERFACE __stdcall
    ReturnInstructionPointer ( CONTEXT * pCtx ) ;

/*----------------------------------------------------------------------
FUNCTION    :   InitializeStackFrameWithContext
DISCUSSION  :
    Initializes the stack frame structure with the appropriate values in
the context to prepare for walking the stack.
PARAMETERS  :
    pStack - The stack to fill.
    pCtx   - The context to use.
RETURNS     :
    FALSE - There was a problem.
    TRUE  - The instruction pointer value.
----------------------------------------------------------------------*/
BOOL CPUHELP_DLLINTERFACE __stdcall
    InitializeStackFrameWithContext ( STACKFRAME64 * pStack ,
                                      CONTEXT *      pCtx    ) ;


#ifdef __cplusplus
}
#endif


#endif  // _CPUHELP_H

