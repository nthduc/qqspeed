#if !defined(_TYPEDEF_H_)
#define _TYPEDEF_H_


#if !defined(u32)
typedef unsigned int u32;
#endif

#if !defined(u16)
typedef unsigned __int16 u16;
#endif

#if !defined(u8)
typedef unsigned __int8 u8;
#endif

#if !defined(s32)
typedef int s32;
#endif

#if !defined(s16)
typedef __int16 s16;
#endif

#if !defined(s8)
typedef __int8 s8;
#endif

typedef void (*DISPFUNC) ( const char* msg ) ;
typedef const char* (*UIN2NAMEFUNC) ( u32 uin );

#endif