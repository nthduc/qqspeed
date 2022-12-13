#include "singlet.h"
#include "txtCtrlr.h"

class CPlayerLevel : public CSingleT<CPlayerLevel>
{
public:
	CPlayerLevel(void)
	{
		m_bCreateSuccess = false;
		CHAR szLevelFile[MAX_PATH] = "\0";
#define LEVEL_CFG		"levels.txt"
		//FullName(LEVEL_CFG, szLevelFile);
		
		if ( !m_LevelCtrl.Open(NiApplication::ConvertMediaFilename(LEVEL_CFG)) )
		{
			assert(0);
		}
		else
			m_bCreateSuccess = true;
	}
	~CPlayerLevel(void)
	{
	}

	bool GetLevel(int Exp, int& Level, int& SubExp, int& LevelExp)
	{
		if (!m_bCreateSuccess)
			return false;
		Level = 0;
		SubExp = 0;
		LevelExp = 0;
		
		
		int nIndex = 1;
		int nLower = 0;
		int nHigh = 0;
		int nTotal = 0;
		bool bSuccess = m_LevelCtrl.First();
		for (; bSuccess; nIndex++, bSuccess = m_LevelCtrl.Next())
		{
			CtxtRecordset* pRecordset = m_LevelCtrl.GetRecordset();
			if ( pRecordset->GetFieldCount() < 1 )
				break;
			nHigh = atoi(pRecordset->GetField(0));
			nTotal += nHigh;
			if ( nTotal >= (Exp+1) )
				break;
			else
				nLower += nHigh;
		}
		
		/*
		while(1)
		{
			nIndex++;
			if ( ! m_LevelCtrl.Next() )
				break;
			CtxtRecordset* pRecordset = m_LevelCtrl.GetRecordset();
			if ( pRecordset->GetFieldCount() < 1 )
				break;
			nHigh = atoi(pRecordset->GetField(0));
			nTotal += nHigh;
			if ( nTotal >= Exp )
				break;
			else
				nLower += nHigh;
		}*/
		Level = nIndex;
		SubExp = Exp - nLower;
		LevelExp = nHigh;

		return true;
	}

private:
	CtxtCtrlr	m_LevelCtrl;
	bool		m_bCreateSuccess;
};

inline bool GetPlayerLevel(int exp, int & level, int & subexp, int & levelexp)
{
	if (!CPlayerLevel::Get())
		CPlayerLevel::Create();

	//int subexp = 0; 
	//int levelexp = 0;
	return CPlayerLevel::Get() ? CPlayerLevel::Get()->GetLevel(exp, level, subexp, levelexp) : false;
}