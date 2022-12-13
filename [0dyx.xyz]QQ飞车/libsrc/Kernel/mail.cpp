
#include "mail.h"

CMail::CMail( HWND hparent ) : m_hParent(hparent), m_lhSession(0), m_bInitOK(false)
{
	if( !IsMAPIInstalled() ) return;

	// Get instance handle of MAPI32.DLL
	HINSTANCE			hlibMAPI	= LoadLibrary( "MAPI32.dll" );
	
	//  Get the addresses of all the API's supported by this object
	m_MAPILogon			= ( LPMAPILOGON			)	GetProcAddress ( hlibMAPI, "MAPILogon"			);
	m_MAPISendMail		= ( LPMAPISENDMAIL		)	GetProcAddress ( hlibMAPI, "MAPISendMail"		);
	m_MAPISendDocuments	= ( LPMAPISENDDOCUMENTS )	GetProcAddress ( hlibMAPI, "MAPISendDocuments"	);
	m_MAPIFindNext		= ( LPMAPIFINDNEXT		)	GetProcAddress ( hlibMAPI, "MAPIFindNext"		);
	m_MAPIReadMail		= ( LPMAPIREADMAIL		)	GetProcAddress ( hlibMAPI, "MAPIReadMail"		);
	m_MAPIResolveName	= ( LPMAPIRESOLVENAME	)	GetProcAddress ( hlibMAPI, "MAPIResolveName"	);
	m_MAPIAddress		= ( LPMAPIADDRESS		)	GetProcAddress ( hlibMAPI, "MAPIAddress"		);
	m_MAPILogoff		= ( LPMAPILOGOFF		)	GetProcAddress ( hlibMAPI, "MAPILogoff"			);
	m_MAPIFreeBuffer	= ( LPMAPIFREEBUFFER	)	GetProcAddress ( hlibMAPI, "MAPIFreeBuffer"		);   
	m_MAPIDetails		= ( LPMAPIDETAILS		)	GetProcAddress ( hlibMAPI, "MAPIDetails"		);
	m_MAPISaveMail		= ( LPMAPISAVEMAIL		)	GetProcAddress ( hlibMAPI, "MAPISaveMail"		);

	m_bInitOK = true;

	m_ProfileName = "outlook";
/*
	m_Recipient = "delta";
	m_Subject = "testmail";
	m_Message = "aaaaaaaaaaaaaaaaaaaaa";
	m_EmailAddress = "delta@tencent.com";
*/
}

CMail::~CMail()
{
}

bool CMail::Send()
{
	if( !m_bInitOK ) return false;

	MapiMessage message;
	MapiFileDesc attach;
	MapiRecipDesc recps[1];

	ZeroMemory( &message, sizeof(MapiMessage) );
	ZeroMemory( &attach, sizeof(MapiFileDesc) );
	ZeroMemory( &recps[0], sizeof(recps) );

//	if( !Logon() ) return false;
	
	std::string addr("SMTP:");
	addr += m_EmailAddress;
	recps[0].ulRecipClass = MAPI_TO;
	recps[0].lpszName	  = (char *)m_Recipient.c_str();
	recps[0].lpszAddress  = (char *)addr.c_str();

	message.nRecipCount	= 1;		// Must be set to the correct number of recipients.
	message.lpRecips	= recps;	// Address of list of names returned from MAPIAddress.		
	message.ulReserved	= 0L;
	message.lpszSubject	= (char *)m_Subject.c_str();
	message.lpszNoteText= (char *)m_Message.c_str();
	message.lpOriginator= NULL;	

	if( m_AttachFile.empty() )
	{
		message.nFileCount		= 0L;
		message.lpFiles		= NULL;
	}
	else
	{
		std::string fullpath  = m_AttachFilePath + m_AttachFile;
		attach.lpszFileName = (char *)m_AttachFile.c_str();
		attach.lpszPathName = (char *)fullpath.c_str();
		attach.nPosition = -1;

		message.nFileCount	= 1;
		message.lpFiles		= &attach;
	}

	DWORD result = m_MAPISendMail( m_lhSession, (ULONG)m_hParent, &message, MAPI_DIALOG|MAPI_LOGON_UI, 0L );
	if( result != SUCCESS_SUCCESS ) return false;
		
//	if( !Logoff() ) return false;
		
	return true;
}

bool CMail::IsMAPIInstalled()
{
	char   szAppName[32];
	char   szKeyName[32];
	char   szDefault = {'0'};
	char   szReturn = {'0'};
	DWORD  nSize = 0L;
	char   szFileName[256];
		
	
	strcpy ( szAppName, "MAIL" );
	strcpy ( szKeyName, "MAPI" );
		
	nSize = 1;
	strcpy ( szFileName, "WIN.INI" );
		
	DWORD SimpleMAPIInstalled = GetPrivateProfileString ( (TCHAR*) szAppName, 
			(TCHAR*) szKeyName, 
			(TCHAR*)&szDefault,
			(TCHAR*)&szReturn, 
			nSize, 
			(TCHAR*)szFileName);
		
	if( strcmp(&szDefault, &szReturn) == 0 ) return false;
	else return true;
}

bool CMail::Logon()
{
	if( !m_lhSession )	  // Always ask if there is an active session
	{
		//ULONG result = m_MAPILogon( (ULONG)m_hParent, (char *)m_ProfileName.c_str(), NULL, MAPI_NEW_SESSION/*|MAPI_LOGON_UI*/, 0, &m_lhSession );
		ULONG result = m_MAPILogon( (ULONG)m_hParent, (char *)m_ProfileName.c_str(), NULL, MAPI_NEW_SESSION, 0, &m_lhSession );
		
		if( result != SUCCESS_SUCCESS ) return false;
	}
	
	return true;
}

bool CMail::Logoff()
{
	if( m_lhSession )
	{
		ULONG result = m_MAPILogoff( m_lhSession, (ULONG)m_hParent, 0, 0 );
		if( result != SUCCESS_SUCCESS ) return false;

		m_lhSession = NULL;
	}
	return true;
}
