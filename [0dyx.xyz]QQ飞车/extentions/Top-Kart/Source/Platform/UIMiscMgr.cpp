////////////////////////////////////////////////
//     FileName:    UIMiscMgr.cpp
//     Author:      Fish
//     Date:        2006-10-11 15:39:35
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "UIMiscMgr.h"
#include "strex.h"
#include "common.h"

#define INVALID_IMG		-1
//////////////////////////////////////////////////////////////////////////

CUIMiscMgr::CUIMiscMgr(CUIMiscCallback* pCB):
m_pCallback(pCB),
m_pDisplay(NULL)
{
}

CUIMiscMgr::~CUIMiscMgr()
{
	UnInit();
}

void CUIMiscMgr::Init(IDisplay* Display)
{
	m_pDisplay = Display;
	LoadChatFaces();
	LoadCtrlColor();
	LoadChatActions();
}

void CUIMiscMgr::UnInit()
{
	for ( StrKeyToImageMap::iterator it = m_ChatFaceMap.begin(); it != m_ChatFaceMap.end(); it++ )
	{
		//(*it).second->Release();
		delete (*it).second;
	}
	m_ChatFaceMap.clear();
	m_pDisplay = NULL;
}

void CUIMiscMgr::DrawChatFace(int index, int frame, int left, int top)
{
	if ( index >= m_ChatFaceMap.size() )
		index = 0;
	StrKeyToImageMap::iterator it = m_ChatFaceMap.begin();
	for ( int i = 0; i < index; i++, it++);
	CImageEx* pImage = it->second;
	int totframe = pImage->m_nFrame;
	if ( frame >=  totframe )
		frame = frame % totframe;
	if ( frame < 0 )
		frame = 0;
	pImage->Draw(frame, left, top);
}

BOOL CUIMiscMgr::DoChatAction(UINT uin, LPCSTR str)
{
	int maxmatchlen = 0;
	int index = 0, i = 0;
	StrKeyToActionMap::const_iterator maxmatchit = NULL;
	for ( StrKeyToActionMap::const_iterator it = m_ChatActionMap.begin(); it != m_ChatActionMap.end(); it++ )
	{
		int tmpmatchlen = strimatch(it->first.c_str(), str);
		if ( tmpmatchlen == it->first.length() && tmpmatchlen == strlen(str) && tmpmatchlen > maxmatchlen )
		{
			maxmatchlen = tmpmatchlen;
			maxmatchit = it;
			index = i;
		}
		i++;
	}
	if ( maxmatchlen > 0 && maxmatchit != NULL )
	{
		if ( m_pCallback )
		{
			m_pCallback->PlayerDoAction(uin, maxmatchit->second);
		}
		return TRUE;
	}	

	//本地语言
	StrKeyToActionMap::const_iterator itFind = m_ChatActionMapLocal.find(str);
	if ( itFind !=  m_ChatActionMapLocal.end() )
	{
		if ( m_pCallback )
		{
			m_pCallback->PlayerDoAction(uin, maxmatchit->second);
		}
		return TRUE;
	}

	//找不到匹配的字符串，播放默认动作
	m_pCallback->PlayerDoAction(uin, 1216);
	
	return FALSE;
}

BOOL CUIMiscMgr::ParseField(LPCSTR name, LPSTR outstr, INT* outlen)
{
	if ( strcmp(name, "PNAME") == 0 )
	{
		LPCSTR szName = "";
		if ( m_pCallback )
			szName = m_pCallback->GetPlayerName();
		strcpy(outstr, szName);
		*outlen = strlen(szName);
		return TRUE;
	}
	else if ( strcmp(name, "TIME") == 0 )
	{
		LPCSTR szName = "2006年4月24日";
		strcpy(outstr, szName);
		*outlen = strlen(szName);		
		return TRUE;
	}
	return FALSE;
}

void CUIMiscMgr::LoadChatFaces()
{
	char file[MAX_PATH] = "\0";
	_fullpath(file, "faces.txt", MAX_PATH);
	CtxtCtrlr txtCtrlr;
	if ( !txtCtrlr.Open(file) )
		return;

	int index = 0;
	while(1)
	{
		if ( !txtCtrlr.Next() )
			break;
		CtxtRecordset* pRecordset = txtCtrlr.GetRecordset();
		if ( pRecordset->GetFieldCount() < 2 )
			continue;

		//CComObject<CImageEx>* spImageEx = NULL;
		//CComObject<CImageEx>::CreateInstance(&spImageEx);
		CImageEx* pImageEx = new CImageEx;
		pImageEx->Init(m_pDisplay);
	//	CImageEx* pImage = new CImageEx;
		if ( pImageEx->Load(pRecordset->GetField(1)) )
		{
			m_ChatFaceMap[pRecordset->GetField(0)] = pImageEx;
		}
		else
		{
			delete pImageEx;
		}
	}
	m_bLoaded = TRUE;
}

void CUIMiscMgr::LoadCtrlColor()
{
	char szfile[MAX_PATH] = "\0";
	_fullpath(szfile, "ctrlc.txt", MAX_PATH);
	CtxtCtrlr txtCtrlr;
	if ( !txtCtrlr.Open(szfile) )
		return;

	int index = 0;
	while(1)
	{
		if ( !txtCtrlr.Next() )
			break;
		CtxtRecordset* pRecordset = txtCtrlr.GetRecordset();
		if ( pRecordset->GetFieldCount() < 1 )
			continue;
		DWORD c = 0xffffffff;
		sscanf(pRecordset->GetField(0), "%x", &c);

		if ( m_pCallback )
			m_pCallback->SetCtrlTextColor(index++, c);
	}
}

void CUIMiscMgr::LoadChatActions()
{
	char file[MAX_PATH] = "\0";
	_fullpath(file, "chatact.txt", MAX_PATH);
	CtxtCtrlr txtCtrlr;
	if ( !txtCtrlr.Open(file) )
		return;

	while(1)
	{
		if ( !txtCtrlr.Next() )
			break;
		CtxtRecordset* pRecordset = txtCtrlr.GetRecordset();
		if ( pRecordset->GetFieldCount() < 2 )
			continue;
		m_ChatActionMap[pRecordset->GetField(0)] = atoi(pRecordset->GetField(1));
		if ( pRecordset->GetFieldCount() > 2 )
		{
			m_ChatActionMapLocal[pRecordset->GetField(2)] = atoi(pRecordset->GetField(1));
		}
	}
}


void CUIMiscMgr::ParseKeyChar(stKeyChar* key)
{
	if ( key->key[0] != '/' || strlen(key->key) < 2 )
		return;

	if ( key->key[1] == 'C' && strlen(key->key) > 3 )
	{
		int index = -1;
		char sztmp[3] = "\0";
		strncpy(sztmp, &key->key[2], 2);
		sscanf(sztmp, "%d", &index);
		if ( index >= 0 && index < MAX_TEXT_COLOR )
		{
			key->type = RICH_CHAR_COLOR;
			key->index = index;
			key->keynum = 4;
			key->width = 0;
		}
	}
	else if( key->key[1] == '<' && strlen(key->key) > 3 )//链接格式符/<>
	{		
		if ( key->key[2] == '/' )
		{
			const char* pstart = key->key+1;
			const char* pend = strchr(key->key+2, '>');
			if ( pend )
			{
				key->keynum = int(pend-pstart)+2;
				key->type=RICH_CHAR_HLINK;
				key->index=0;
			}
		}
		else
		{
			const char* pstart=key->key+1;
			const char* pend=strchr(key->key+1,'>');
			if(pend)
			{
				key->keynum=int(pend-pstart)+2;
				key->type=RICH_CHAR_HLINK;
				key->index=(pend-pstart>1)?1:0;
				key->width=0;
				key->cmdid=RICHCHAR_CMD_CLICK+RICHCHAR_CMD_RCLICK;
			}
		}
	}	
	else if ( key->key[1] == '{' ) // /{}
	{
		const char* pend = strchr(key->key + 1, '}');
		if ( pend && pend - key->key < 20 )
		{
			char sztmp[UI_KEYCHAR_MAX] = "\0";
			strncpy(sztmp, key->key + 2, pend - key->key - 2);
			if ( trim_space(sztmp) )
			{
				INT tmplen = 0;
				if ( ParseField(sztmp, sztmp, &tmplen) )
				{
					char szright[UI_KEYCHAR_MAX] = "\0";
					strcpy(szright, pend + 1);
					key->type = RICH_CHAR_FIELD;
					key->index = 0;
					key->keynum = tmplen;
					key->keynumeat = pend - key->key + 1;
					strcpy(key->key, sztmp);
				}
			}
		}
	}
	else if ( strlen(key->key) > 1)
	{
		int maxmatchlen = 0;
		int index = 0, i = 0;
		StrKeyToImageMap::const_iterator maxmatchit = NULL;
		for ( StrKeyToImageMap::const_iterator it = m_ChatFaceMap.begin(); it != m_ChatFaceMap.end(); it++ )
		{
			int tmpmatchlen = strimatch(it->first.c_str(), &key->key[1]);
			if ( tmpmatchlen == it->first.length() && tmpmatchlen > maxmatchlen )
			{
				maxmatchlen = tmpmatchlen;
				maxmatchit = it;
				index = i;
			}
			i++;
		}
		if ( maxmatchlen > 0 && maxmatchit != NULL )
		{
			key->type = RICH_CHAR_ICON;
			key->index = index;
			key->keynum = maxmatchlen + 1;
			key->width = maxmatchit->second->m_AniWidth;
			key->height = maxmatchit->second->m_AniHeight;
		}
	}
}

