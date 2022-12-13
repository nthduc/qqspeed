/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#ifndef _BINARYIMAGE_H
#define _BINARYIMAGE_H

// The class that manages a single binary image.
class CBinaryImage : public CObject
{
public:
        static BOOL GetModules(LPCTSTR pModuleName, CMapStringToPtr& ModuleList);
        static BOOL SearchForModule(CString& fullpath);
        static BOOL InitIgnoredImage(CMapStringToPtr& ModuleList, BOOL bFullList=TRUE);
protected   :
    // Necassary MFC macros and functions.
    CBinaryImage ( void ) ;
    DECLARE_SERIAL ( CBinaryImage )
public      :
    virtual void Serialize ( CArchive & ar ) ;

////////////////////////////////////////////////////////////////////////
//                Public Construction and Desctruction
////////////////////////////////////////////////////////////////////////
public      :
    // szFileName is the complete filename of the binary file.
    CBinaryImage ( LPCTSTR szFileName ) ;

    // Ye old copy constructor.
    CBinaryImage ( const CBinaryImage & cInit ) ;

    // Simple destructor.
    virtual ~CBinaryImage ( void ) ;

////////////////////////////////////////////////////////////////////////
//                     Public Validation Methods
////////////////////////////////////////////////////////////////////////
public      :
    // Returns TRUE if the image passed in the constructor is a valid
    // binary image.  Returns FALSE otherwise.
    BOOL IsValidImage ( BOOL bShowErrors = TRUE ) ;

////////////////////////////////////////////////////////////////////////
//                 Public Property Retrieval Methods
////////////////////////////////////////////////////////////////////////
public      :
    // Returns the load address for this image.
    DWORD GetLoadAddress ( void ) const ;

    // Returns the full name for this image.
    const CString GetFullName ( void ) const ;
    const CString GetName ( void ) const ;

    // Returns the binary image characteristics.
    DWORD GetCharacteristics ( void ) const ;

    // Returns a string that contains any additional information that
    // the class would like to return about this image.
    const CString GetAdditionalInfo ( void ) const ;

    // Gets the extra data for this class.
    DWORD GetExtraData ( void ) const ;

////////////////////////////////////////////////////////////////////////
//                  Public Property Setting Methods
////////////////////////////////////////////////////////////////////////
public      :
    // This class is responsible for handling it's own property changing
    // and setting.  This is the function that will be called when the
    // user wants to change the property of a binary image.
    // This returns TRUE if any of the properties changed.
    BOOL SetProperties ( void ) ;

    // This function forces the load address for this instance.  It is
    // only to be called by controller classes that need to do the
    // forcing, i.e., when the address the class has conflicts with
    // an existing load address from another class.
    void SetLoadAddress ( DWORD dwAddr ) ;

    // Sets the extra data property for this class.  Think of this as
    // something like the item data LPARAM value in Tree Controls.
    // This is set by the controling class.
    void SetExtraData ( DWORD dwData ) ;

////////////////////////////////////////////////////////////////////////
//                          Public Operators
////////////////////////////////////////////////////////////////////////
public      :
    CBinaryImage & operator = ( const CBinaryImage & rhs ) ;

////////////////////////////////////////////////////////////////////////
//                       Protected Data Members
////////////////////////////////////////////////////////////////////////
protected   :

    // The full name of the binary image.  This is set by the
    // constructor.
    CString     m_szFullName        ;

    // The load address for this image.
    DWORD       m_dwLoadAddress     ;

    // The characteristics of the binary.  This is straight out of the
    // PE header.
    DWORD       m_dwCharacteristics ;

    // The flag that indicates if this instance points to a valid
    // binary.
    BOOL        m_bIsValid          ;

    // The extra data parameter.
    DWORD       m_dwExtraData       ;

} ;

#endif      // _BINARYIMAGE_H


