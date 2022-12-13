
#ifndef __MAIL_H__
#define __MAIL_H__

#include <Windows.h>
#include <mapi.h>
#include <string>

class CMail
{
public:
	CMail( HWND hparent=0 );
	~CMail();

	void SetSubject( const char *pstr ){ m_Subject = pstr; }
	void SetMessage( const char *pstr ){ m_Message = pstr; }
	void SetAttachFile( const char *pstr ){ m_AttachFile = pstr; }
	void SetRecipient( const char *addr, const char *pname ){ m_Recipient=pname, m_EmailAddress=addr; }
	bool Send();

private:
	bool m_bInitOK;

	// MAPI function pointers
	LPMAPIADDRESS m_MAPIAddress;
	LPMAPIDETAILS m_MAPIDetails;		
	LPMAPIFINDNEXT m_MAPIFindNext;		
	LPMAPIFREEBUFFER m_MAPIFreeBuffer;
	LPMAPILOGOFF m_MAPILogoff;		
	LPMAPILOGON m_MAPILogon;			
	LPMAPIREADMAIL m_MAPIReadMail;		
	LPMAPIRESOLVENAME m_MAPIResolveName;	
	LPMAPISENDDOCUMENTS m_MAPISendDocuments;	
	LPMAPISENDMAIL m_MAPISendMail;
	LPMAPISAVEMAIL m_MAPISaveMail;

	HWND   m_hParent;
	ULONG  m_lhSession;

	std::string m_Subject;
	std::string m_Message;
	std::string m_AttachFilePath;
	std::string m_AttachFile;
	std::string m_Recipient;
	std::string m_ProfileName;
	std::string m_EmailAddress;

private:
	bool Logon();
	bool Logoff();
	bool IsMAPIInstalled();
};

#endif