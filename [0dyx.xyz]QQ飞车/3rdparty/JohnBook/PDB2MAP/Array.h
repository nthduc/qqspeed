/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#ifndef _ARRAY_H
#define _ARRAY_H

/*//////////////////////////////////////////////////////////////////////
// Necessary Includes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Constants & Defines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Typedefs and Classes
//////////////////////////////////////////////////////////////////////*/
typedef int ( __cdecl *PFNCOMPARE )( const void * , const void *) ;

typedef struct tag_FASTARRAYINFO
{
    // The array itself.
    BYTE * pArray ;
    // The total number of slots in the array.
    ULONG  ulTotalSlots ;
    // The active slots in the array.
    ULONG  ulActiveSlots ;
    // The number of units to add a reallocation occurs.
    ULONG  ulReallocUnits ;
    // The size of each structure in the array.
    ULONG  ulStructSize ;
    // The comparison function.
    PFNCOMPARE pfnCompare ;
    // The current iterator position.
    ULONG  ulCurrIterator ;
} FASTARRAYINFO , * LPFASTARRAYINFO ;


/*//////////////////////////////////////////////////////////////////////
// Prototypes and Inlines
//////////////////////////////////////////////////////////////////////*/

BOOL AddArrayItem ( LPFASTARRAYINFO pAInfo , BYTE * pItem ) ;

void SortArrayInfo ( LPFASTARRAYINFO pAInfo ) ;

BYTE * FindArrayItem ( LPFASTARRAYINFO pAInfo , BYTE * pToFind ) ;

BYTE * GetFirstArrayItem ( LPFASTARRAYINFO pAInfo ) ;

BYTE * GetNextArrayItem ( LPFASTARRAYINFO pAInfo ) ;

ULONG GetArrayCount ( LPFASTARRAYINFO pAInfo ) ;

void ClearAllArrayInfo ( LPFASTARRAYINFO pAInfo ) ;

#endif // _ARRAY_H
