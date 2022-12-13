/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
// Necessary Includes
//////////////////////////////////////////////////////////////////////*/
#include "STDAFX.h"
#include "Array.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Constants & Defines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Typedefs and Classes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes and Inlines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// EXTERNALLY VISIBLE CODE STARTS HERE
//////////////////////////////////////////////////////////////////////*/

BOOL AddArrayItem ( LPFASTARRAYINFO pAInfo , BYTE * pItem )
{
    // Is this the first time through?
    if ( 0 == pAInfo->ulActiveSlots  )
    {
        // Allocate the array.
        pAInfo->pArray = (BYTE*)malloc ( pAInfo->ulStructSize *
                                            pAInfo->ulReallocUnits ) ;
        if ( NULL == pAInfo->pArray )
        {
            return ( FALSE ) ;
        }
        pAInfo->ulTotalSlots = pAInfo->ulReallocUnits ;

        memcpy ( pAInfo->pArray , pItem , pAInfo->ulStructSize ) ;

        pAInfo->ulActiveSlots++ ;
    }
    else
    {
        // Is there room at the Inn?
        if ( pAInfo->ulActiveSlots == pAInfo->ulTotalSlots )
        {
            // Bump up the total slots.
            pAInfo->ulTotalSlots += pAInfo->ulReallocUnits ;

            // Reallocate and them stick it in.
            pAInfo->pArray = (BYTE*)realloc ( pAInfo->pArray ,
                                              pAInfo->ulStructSize *
                                               pAInfo->ulTotalSlots );
            if ( NULL == pAInfo->pArray )
            {
                return ( FALSE ) ;
            }
        }
        BYTE * pSlot = (BYTE*)((INT_PTR)pAInfo->pArray +
                                      ( pAInfo->ulActiveSlots *
                                         pAInfo->ulStructSize  ) ) ;

        memcpy ( pSlot , pItem , pAInfo->ulStructSize ) ;

        pAInfo->ulActiveSlots++ ;
    }
    return ( TRUE ) ;
}

void SortArrayInfo ( LPFASTARRAYINFO pAInfo )
{
    qsort ( pAInfo->pArray        ,
            pAInfo->ulActiveSlots ,
            pAInfo->ulStructSize  ,
            pAInfo->pfnCompare     ) ;
}

BYTE * FindArrayItem ( LPFASTARRAYINFO pAInfo , BYTE * pToFind )
{
    // Take the easy one.
    if ( 0 == pAInfo->ulActiveSlots )
    {
        return ( NULL ) ;
    }

    BYTE * pRet = (BYTE*)bsearch ( pToFind               ,
                                   pAInfo->pArray        ,
                                   pAInfo->ulActiveSlots ,
                                   pAInfo->ulStructSize  ,
                                   pAInfo->pfnCompare     ) ;
    return ( pRet ) ;
}

BYTE * GetFirstArrayItem ( LPFASTARRAYINFO pAInfo )
{
    if ( 0 == pAInfo->ulActiveSlots )
    {
        return ( NULL ) ;
    }

    pAInfo->ulCurrIterator = 0 ;

    return ( (BYTE*)( (INT_PTR)pAInfo->pArray +
                         ( pAInfo->ulStructSize *
                               pAInfo->ulCurrIterator ) ) ) ;
}

BYTE * GetNextArrayItem ( LPFASTARRAYINFO pAInfo )
{
    pAInfo->ulCurrIterator++ ;

    if ( pAInfo->ulCurrIterator >= pAInfo->ulActiveSlots )
    {
        return ( NULL ) ;
    }
    return ( (BYTE*) ( (INT_PTR)pAInfo->pArray +
                                ( pAInfo->ulStructSize *
                                    pAInfo->ulCurrIterator ) ) ) ;
}

ULONG GetArrayCount ( LPFASTARRAYINFO pAInfo )
{
    return ( pAInfo->ulActiveSlots ) ;
}

void ClearAllArrayInfo ( LPFASTARRAYINFO pAInfo )
{
    if ( NULL != pAInfo->pArray )
    {
        free ( pAInfo->pArray ) ;
        pAInfo->pArray = NULL ;
    }
    pAInfo->ulActiveSlots = 0 ;
    pAInfo->ulTotalSlots = 0 ;

}