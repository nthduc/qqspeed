#ifndef __LOGTIME_H__
#define __LOGTIME_H__

#include "nisystem.h"
#include "common.h"

#define MAX_TEXT_LEN	4096

class CLogTime
{
public:
	CLogTime(LPCSTR pszText)
	{
		assert(pszText);
		assert(strlen(pszText) < MAX_TEXT_LEN);
		strcpy(m_szText, pszText);
		m_fStartTime = NiGetCurrentTimeInSec();
	}

	~CLogTime()
	{
		m_fEndTime = NiGetCurrentTimeInSec();
		LogText(m_szText);
		LogText("Time:%fm", m_fEndTime - m_fStartTime);
	}

private:
	float m_fStartTime;
	float m_fEndTime;
	char  m_szText[MAX_TEXT_LEN];
};

#define LOG_TIME(text) CLogTime __log_time__(text);

#endif
