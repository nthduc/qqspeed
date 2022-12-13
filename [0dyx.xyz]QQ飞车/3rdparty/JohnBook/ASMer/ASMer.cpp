/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

void NOPFuncOne ( void )
{
    __asm NOP
    __asm NOP
}

void NOPFuncTwo ( void )
{
    __asm
    {
        NOP
        NOP
    }
}

void PushPop ( void )
{
    __asm
    {
        // Save the values in EAX, ECX, and EDX.
        PUSH EAX
        PUSH ECX
        PUSH EDX

        // Do some operation here that might destroy the values in each
        // of those registers.

        // Restore the previously saved registers. Notice that they are
        // removed from the stack in LIFO order.
        POP EDX
        POP ECX
        POP EAX
    }
}

void SwapRegistersWithPushAndPop ( void )
{
    __asm
    {
        // Swap the EAX and EBX values. The sequence gives you an idea
        // of how it could be done.
        PUSH EAX
        PUSH EBX

        POP EAX
        POP EBX
    }
}

void SwapRegisters ( void )
{
    __asm
    {
        // The EAX register is a temporary holder so I put it on the
        // stack so I don't destroy it's value.
        // Swap the ECX and EBX values.
        PUSH EAX
        MOV  EAX , ECX
        MOV  ECX , EBX
        MOV  EBX , EAX
        POP  EAX
    }
}

void SubtractExample ( void )
{
    __asm
    {
        // Set the registers and do a subtraction.  The formula for
        // this subtract example is EAX = Value(EAX) - Value(EBX)
        MOV EAX , 5
        MOV EBX , 2
        SUB EAX , EBX
    }
}

#pragma warning( disable : 4731 )
void SimplePrologAndEpilogExample ( void )
{
    __asm
    {
        // Standard prolog setup
        PUSH EBP        // Save the stack frame register.
        MOV  EBP , ESP  // Set the local function stack frame to ESP.
        SUB  ESP , 20h  // Make room on the stack for 0x20 bytes of
                        // local variables. The SUB instruction appears
                        // only if the function has local variables.
    }

    __asm
    {
        // Standard epilog teardown
        MOV ESP , EBP   // Restore the stack value.
        POP EBP         // Restore the saved stack frame register.
    }
}

// Global memory.
int g_iVal = 0 ;

void AccessGlobalMemory ( void )
{
    __asm
    {
        // Set the global variable to 48,059.
        MOV g_iVal , 0BBBBh

        // If symbols are loaded, the Disassembly window will show:
        // MOV DWORD PTR [g_iVal (4030B4h)],0BBBBh

        // If symbols are NOT loaded, the Disassembly window shows:
        // MOV DWORD PTR [4030B4h],0BBBBh
    }
}

void AccessParameter ( int iParam )
{
    __asm
    {
        // Move the value if iParam into EAX.
        MOV EAX , iParam

        // If symbols are loaded, the Disassembly window will show:
        // MOV EAX,DWORD PTR [iParam]

        // If symbols are NOT loaded, the Disassembly window shows:
        // MOV EAX,DWORD PTR [EBP+8]
    }
}

void AccessLocalVariable ( void )
{
    int iLocal ;

    __asm
    {
        // Set the local variable to 23.
        MOV iLocal , 017h

        // If symbols are loaded, the Disassembly window will show:
        // MOV DWORD PTR [iLocal],017h

        // If symbols are NOT loaded, the Disassembly window shows:
        // MOV [EBP-4],017h
    }
}

void AccessLocalsAndParamsExample ( int * pParam1 , int * pParam2 )
{
    int iLocal1 = 3 ;
    int iLocal2 = 0x42 ;

    iLocal1 = *pParam1 ;
    iLocal2 = *pParam2 ;
}

// The code that calls AccessLocalsAndParamsExample
void DoTheCall ( void )
{
    int iVal1 = 0xDEADBEEF ;
    int iVal2 = 0xBADDF00D ;
    __asm
    {
        LEA  EAX , DWORD PTR [iVal2]
        PUSH EAX
        LEA  EAX , DWORD PTR [iVal1]
        PUSH EAX
        CALL AccessLocalsAndParamsExample
        ADD  ESP , 8
    }
}


void CallSomeFunctions ( void )
{
    __asm
    {
        // Call the imported function, GetLastError, which takes no
        // parameters.  EAX will hold the return value.  This is a
        // call through the IAT so it is a call through a pointer.
        CALL DWORD PTR [GetLastError]

        // If symbols are loaded, the Disassembly window will show:
        // CALL DWORD PTR [__imp__GetLastError@0 (00402004)]

        // If symbols are NOT loaded, the Disassembly window shows:
        // CALL DWORD PTR [00402004]

        ////////////////////////////////////////////////////////////////
        // Call a function inside this file.
        CALL NOPFuncOne

        // If symbols are loaded, the Disassembly window will show:
        // CALL NOPFuncOne (00401000)

        // If symbols are NOT loaded, the Disassembly window shows:
        // CALL 00401000
    }
}

void NOTExample ( void )
{
    __asm
    {
        MOV EAX , 0FFh
        MOV EBX , 1
        NOT EAX     // EAX now holds 0FFFFFF00h.
        NOT EBX     // EBX now holds 0FFFFFFFEh.
    }
}

void NEGExample ( void )
{
    __asm
    {
        MOV EAX , 0FFh
        MOV EBX , 1
        NEG EAX     // EAX now holds 0FFFFFF01h ( 0 - 0FFh ).
        NEG EBX     // EBX now holds 0FFFFFFFFh ( 0 - 1 ).
    }
}

void LEAExamples ( void )
{
    int * pInt ;
    int iVal ;

    // The following instruction sequence is identical to the C code:
    // pInt = &iVal ;
    __asm
    {
        LEA EAX , iVal
        MOV [pInt] , EAX
    }

    ////////////////////////////////////////////////////////////////////

    char szBuff [ MAX_PATH ] ;

    // Another example of accessing a pointer through LEA.  This is
    // identical to the C code:
    // GetWindowsDirectory ( szBuff , MAX_PATH ) ;
    __asm
    {
        PUSH 104h           // Push MAX_PATH as the second parameter.
        LEA  ECX , szBuff   // Get the address of szBuff.
        PUSH ECX            // Push the address of szBuff as the first
                            // parameter.
        CALL DWORD PTR [GetWindowsDirectory]
    }
}

void JumpExamples ( int i )
{
    // Here is the C code statement.  Notice the conditional is "i > 0".
    // the compiler generates the opposite.  The assembler that I show
    // is very similar to what the compiler generates.  Different
    // optimization methods generates different code.
    // if ( i > 0 )
    // {
    //     printf ( "%i > 0\n" ) ;
    // }
    char szGreaterThan[] = "%i > 0\n" ;
    __asm
    {
        CMP  i , 0          // Compare i to zero by subtracting (i - 0).
        JLE  JE_LessThanOne // If i is less than zero jump around. to
                            // the label.
        PUSH i              // Push the parameter on the stack.
        LEA  EAX , szGreaterThan // Push the format string.
        PUSH EAX
        CALL DWORD PTR [printf]  // Call printf.  Notice that you can
                            // tell printf probably comes from a DLL
                            // because I am calling through a pointer.
        ADD  ESP , 8        // printf is __cdedcl so I need to clean up
                            // the stack in the caller.

JE_LessThanOne:             // With the inline assembler, you can jump
                            // to any C label.
    }

    ////////////////////////////////////////////////////////////////////

    // Take the absolute value of the parameter and check again.
    // The C code:
    // int y = abs ( i ) ;
    // if ( y >=5 )
    // {
    //      printf ( "abs(i) >= 5\n" ) ;
    // }
    // else
    // {
    //     printf ( "abs(i) < 5\n" ) ;
    // }

    char szAbsGTEFive[] = "abs(i) >= 5\n" ;
    char szAbsLTFive[] = "abs(i) < 5\n" ;
    __asm
    {
        MOV  EBX , i        // Move i's value into EBX.
        CMP  EBX , 0        // Compare EBX to zero (EBX - 0)
        JG  JE_PosNum       // If result is greater than zero, EBX
                            // is negative.
        NEG  EBX            // Turn negative into positive.

JE_PosNum:

        CMP  EBX , 5        // Compare EBX to 5.
        JL   JE_LessThan5    // Jump if less than five.
        LEA  EAX , szAbsGTEFive // Get the pointer to the correct string
                                // into EAX.
        JMP  JE_DoPrintf    // Go to the printf call.

JE_LessThan5:

        LEA  EAX , szAbsLTFive // Get the correct pointer into EAX.

JE_DoPrintf:

        PUSH EAX            // Push the string.
        CALL DWORD PTR [printf] // Print it.
        ADD  ESP , 4        // Restore the stack.
    }
}

void LoopingExample ( int q )
{
    // Here's the C code:
    // for ( ; q < 10 ; q++ )
    // {
    //     printf ( "q = %d\n" , q ) ;
    // }

    char szFmt[] = "q = %d\n" ;
    __asm
    {
        JMP  LE_CompareStep     // First time through, check against
                                // 10 immediately.

LE_IncrementStep:
        INC  q                  // Increment q

LE_CompareStep:
        CMP  q , 0Ah            // Compare q to 10.
        JGE   LE_End            // If it is >= 10, this func. is done.

        MOV  ECX , DWORD PTR [q] // Get the value of q into ECX.
        PUSH ECX                // Get the value onto the stack.
        LEA  ECX , szFmt        // Get the format string.
        PUSH ECX                // Push the format string on the stack.
        CALL DWORD PTR [printf] // Print the current iteration.
        ADD  ESP , 8            // Clean up the stack.

        JMP  LE_IncrementStep   // Increment q and start again.

LE_End:                         // The loop is done.

    }
}

void MemCPY ( char * szSrc , char * szDest , int iLen )
{
    __asm
    {

        MOV ESI , szSrc         // Set the source string.
        MOV EDI , szDest        // Set the destination string.

        MOV ECX , iLen          // Set the length to copy.

                                // Copy away!
        REP MOVS BYTE PTR [EDI] , BYTE PTR [ESI]
    }
}

int StrLEN ( char * szSrc )
{
    int iReturn ;
    __asm
    {
        XOR EAX , EAX           // Zero out EAX
        MOV EDI , szSrc         // Move the string to check into EDI.
        MOV ECX , 0FFFFFFFFh    // The maximum number of chars to check.

        REPNE SCAS BYTE PTR [EDI] // Compare until ECX=0 or found.

        CMP ECX , 0             // if ECX is zero, did not find a
        JE  StrLEN_NoNull       // NULL in the string.

        NOT ECX                 // ECX was counted down so convert it
                                // to a positive number.
        DEC ECX                 // Account for hitting the NULL.
        MOV EAX , ECX           // Return the count.
        JMP StrLen_Done         // Return.

StrLEN_NoNull:
        MOV EAX , 0FFFFFFFFh    // Since NULL was not found, return -1.

StrLEN_Done:

    }
    __asm MOV iReturn , EAX ;
    return ( iReturn ) ;
}

void MemSET ( char * szDest , int iVal , int iLen )
{
    _asm
    {
        MOV EAX , iVal          // EAX holds the fill value.
        MOV EDI , szDest        // Move the string into EDI
        MOV ECX , iLen          // Move the count into ECX.

        REP STOS BYTE PTR [EDI] // Fill the memory.
    }
}

int MemCMP ( char * szMem1 ,  char * szMem2 , int iLen )
{
    int iReturn ;
    __asm
    {
        MOV ESI , szMem1       // ESI holds the first memory.
        MOV EDI , szMem2       // EDI holds the second memory.
        MOV ECX , iLen         // The max bytes to compare.

                               // Compare the memory chunks.
        REPE CMPS BYTE PTR [ESI], BYTE PTR [EDI]

        JL  MemCMP_LessThan    // If szSrc < szDest.
        JG  MemCMP_GreaterThan // if szSrc > szDest.

                               // The memory is equal
        XOR EAX , EAX          // Return zero.
        JMP MemCMP_Done

MemCMP_LessThan:
        MOV EAX , 0FFFFFFFFh   // Return -1.
        JMP MemCMP_Done

MemCMP_GreaterThan:
        MOV EAX , 1            // Return 1.
        JMP MemCMP_Done

MemCMP_Done:
    }
    __asm MOV iReturn , EAX
    return ( iReturn ) ;
}

typedef struct tag_MyStruct
{
    DWORD dwFirst ;
    char  szBuff[ 256 ] ;
    int   iVal ;
} MyStruct , * PMyStruct ;

void FillStruct ( PMyStruct pSt )
{
    char szName[] = "Pam\n" ;

    __asm
    {
        MOV  EAX , pSt          // Place pSt into EAX.  Below, I am
                                // using the direct offsets in the
                                // assembly language to show what they
                                // look like in a disassembly.
                                // Inline assembler allows you to use
                                // the normal <struct>.<field>
                                // references.

        // C code : pSt->dwFirst = 23 ;
        MOV  DWORD PTR [EAX] , 17h

        // C code: pSt->iVal = 0x33 ;
        MOV  DWORD PTR [EAX + 0104h] , 0x33

        // C code: strcpy ( pSt->szBuff , szName ) ;
        LEA  ECX , szName       // Push szName on the stack.
        PUSH ECX

        LEA  ECX , [EAX + 4]    // Get to the szBuff field.
        PUSH ECX

        CALL strcpy
        ADD  ESP , 8            // strcpy is a __cdecl function.

        // C code: pSt->szBuff[ 1 ] = 'A' ;
        MOV  BYTE PTR [EAX + 5] , 41h

        // C code: printf ( pSt->szBuff ) ;
        MOV  EAX , pSt          // Get pSt back.  EAX was destroyed
                                // on the call to strcpy.
        LEA  ECX , [EAX + 4]
        PUSH ECX
        CALL DWORD PTR [printf]
        ADD ESP , 4             // printf is a __cdecl function.
    }
}

class AClass
{
public  :
    AClass ( ) { } ;
    virtual ~AClass ( ) { } ;

    void DoSomething ( char * szData ) ;

    void SetMemberVar ( int iVar ) ;

    int GetMemberVar ( void ) ;

protected   :
    int m_iVar ;

} ;

void AClass :: DoSomething ( char * szData )
{
    printf ( "AClass::DoSomething : %d %s\n" ,
             m_iVar , szData ) ;
}

void AClass :: SetMemberVar ( int iVar )
{
    m_iVar = iVar ;
}

int AClass :: GetMemberVar ( void )
{
    return ( m_iVar ) ;
}

void DoAClass ( void )
{
    AClass c ;

    c.SetMemberVar ( 5 ) ;

    c.DoSomething ( "Something" ) ;

    int iVar = c.GetMemberVar ( ) ;
    if ( iVar < 3 )
    {
        printf ( "iVar < 3\n" ) ;
    }
}

void main ( void )
{
    NOPFuncOne ( ) ;
    NOPFuncTwo ( ) ;
    PushPop ( ) ;
    SwapRegistersWithPushAndPop ( ) ;
    SwapRegisters ( ) ;
    SubtractExample ( ) ;
    SimplePrologAndEpilogExample ( ) ;
    AccessGlobalMemory ( ) ;
    AccessParameter ( 0x42 ) ;
    AccessLocalVariable ( ) ;

    DoTheCall ( ) ;

    CallSomeFunctions ( ) ;
    NOTExample ( ) ;
    NEGExample ( ) ;
    LEAExamples ( ) ;
    JumpExamples ( 0xA  ) ;
    JumpExamples ( -1  ) ;
    LoopingExample ( 5 ) ;

    char szSrc [ 25 ] ;
    char szDest[ 25 ] ;
    strcpy ( szSrc , "ABCDEFGHI" ) ;
    MemCPY ( szSrc , szDest , (int)strlen ( szSrc ) + 1 ) ;

    MemSET ( szDest , 0 , StrLEN ( szDest ) ) ;

    strcpy ( szDest , "ABCDEFXX" ) ;

    MemCMP ( szSrc , szDest , StrLEN ( szDest ) + 1 ) ;

    MyStruct stData ;
    FillStruct ( &stData ) ;

    DoAClass ( ) ;

}
