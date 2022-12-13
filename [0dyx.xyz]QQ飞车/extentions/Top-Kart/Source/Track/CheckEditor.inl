inline RunDirection CLinks::GetCurDir(NiPoint3 & kDirection)
{
	return m_enRunDir;
}

inline void CLinks::UpdatePreCheckPt(CCheckPoint * pChkPt)
{
}

inline void CLinks::UpdateNextCheckPt(CCheckPoint * pChkPt, RunDirection dir)
{
}

inline void CLinks::SetLaps(unsigned int uiLaps)
{
	assert(uiLaps <= MAX_POOLS_NUM);
	if (uiLaps > MAX_POOLS_NUM) 
	{
		uiLaps = MAX_POOLS_NUM;
	}
	m_iTotalLapTmp=uiLaps;//m_stRunInfo.uiTotalLap = uiLaps;
}

inline void CLinks::SetCorrectRunDir(RunDirection enRunDir)
{
	m_stRunInfo.enCurRightDir = enRunDir;
}

inline CCheckPoint * CLinks::GetLastRightCheckPoint(bool bAuto)
{
	return NULL;
}

inline bool CLinks::ShouldReset()
{
	float fCurTime = GetMainApp()->GetCurrentTime();
	if (m_stRunInfo.uiWarningTime >= 1 && fCurTime - m_stRunInfo.fWarningStartTime >= 3.0f) 
	{
		return true;
	}
	
	return false;
}

inline bool CLinks::ShouldShowWarning()
{
	if (m_stRunInfo.uiWarningTime >= 1 && m_stRunInfo.fWarningStartTime >= 1.0f) 
	{
		return true;
	}

	return false;
}

inline void CLinks::AddWarningTime()
{
	m_stRunInfo.AddWarningTime();
}

inline CLinks::RunInfo::RunInfo()
: enCurRightDir(dirUncertain)
, uiWarningTime(0)
, fWarningStartTime(99999999)
{	
}

inline CLinks::RunInfo::~RunInfo()
{
	Clear();
}

inline CCheckPoint * CLinks::RunInfo::GetLastRightCheckPoint(bool bAuto, bool bReallyRight)
{
	CCheckPoint * pRet = NULL;
	return pRet;
}


inline void CLinks::RunInfo::Clear()
{
	enCurRightDir   = (dirUncertain);
	
	
	//CLinks* pLink=CLevelManager::Get()->GetLinks();
	//pLink->m_iCurLapTmp=0;
	//pLink->m_iTotalLapTmp=0;


	//m_iCurLapTmp	    = (0);
	//m_iTotalLapTmp      = (0);
	//kStartPointName = ("");
	uiWarningTime   = (0);
	fWarningStartTime = 9999999999.f;
	//uiWrongLaps     = 0;
}

inline void CLinks::ResetWarning()
{
	m_stRunInfo.ResetWarningTime();	
}

inline RunDirection CLinks::GetCorrectRunDir()
{
	return m_stRunInfo.enCurRightDir;
}

inline unsigned int CLinks::GetCurLaps()
{
	UINT nRes=0;
	if(m_iCurLapTmp<0)
	{
		nRes=1;
	}
	else
	{
		nRes=m_iCurLapTmp+1;
	}
	//判断是否已跑的最大圈数
	if(nRes>m_iBestLapTmp)
	{
		m_iBestLapTmp=nRes;
	}
	else
	{
		nRes=m_iBestLapTmp;
	}
	return nRes;//m_stRunInfo.iCurLap+1;
}

inline unsigned int CLinks::GetTotalLaps()
{
	return m_iTotalLapTmp;//m_stRunInfo.uiTotalLap;
}

inline void CLinks::ClearRecord()
{
	m_stRunInfo.uiWarningTime = 0;
	m_iCurLapTmp=0;//m_stRunInfo.iCurLap = 0;
	m_iBestLapTmp=0;
	//m_pLinkPre = m_pLinkHead;
}

inline const NiTransform & CLinks::GetStartPointTransform(unsigned int uiIndex) const
{
	assert(uiIndex < MAX_PLAYER_NUM);
	return m_arrStartPtTransforms[uiIndex%MAX_PLAYER_NUM];
}

inline CLinks::tagPtPairUn::tagPtPairUn()
{
	pPtFrom=NULL;
	pPtTo=NULL;
}

inline CLinks::tagPtTrack::tagPtTrack()
{
	nNum=0;
}

///== arithmetic
inline bool CLinks::tagPtPairUn::operator== (const tagPtPairUn &insA) const
{
	if(insA.pPtFrom==pPtFrom && insA.pPtTo==pPtTo)
	{
		return true;
	}
	//
	return false;
}