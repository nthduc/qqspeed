
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MAPDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MAPDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MAPDLL_EXPORTS
#define MAPDLL_API __declspec(dllexport)
#else
#define MAPDLL_API __declspec(dllimport)
#endif

MAPDLL_API int MapDLLFunction ( void ) ;
MAPDLL_API char * MapDLLHappyFunc ( char * p ) ;


