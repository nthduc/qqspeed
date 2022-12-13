/*---------------------------------------------------------------------
         The only definition of the Expression Evaluator AddIns
                  Lifted from The EEAddIn Sample Project
---------------------------------------------------------------------*/

typedef struct tagDEBUGHELPER
{
    DWORD dwVersion ;
    BOOL (WINAPI *ReadDebuggeeMemory)( struct tagDEBUGHELPER * pThis  ,
                                       DWORD                   dwAddr ,
                                       DWORD                   nWant  ,
                                       VOID *                  pWhere ,
                                       DWORD *                 nGot   );
    // from here only when dwVersion >= 0x20000
    DWORDLONG (WINAPI *GetRealAddress)( struct tagDEBUGHELPER *pThis ) ;
    BOOL (WINAPI *ReadDebuggeeMemoryEx)( struct tagDEBUGHELPER *pThis  ,
                                         DWORDLONG              qwAddr ,
                                         DWORD                  nWant  ,
                                         VOID*                  pWhere ,
                                         DWORD *                nGot  );
    int (WINAPI *GetProcessorType)( struct tagDEBUGHELPER *pThis ) ;
} DEBUGHELPER ;

// The prototype each of your functions must be.
typedef HRESULT (WINAPI *CUSTOMVIEWER)( DWORD          dwAddress   ,
                                        DEBUGHELPER *  pHelper     ,
                                        int            nBase       ,
                                        BOOL           bUniStrings ,
                                        char *         pResult     ,
                                        size_t         max         ,
                                        DWORD          reserved     ) ;


