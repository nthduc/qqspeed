#ifndef __TESTOBJECTIMPL_T_H__
#define __TESTOBJECTIMPL_T_H__

#include <utility>
#include <map>
#include <string>
#include <list>

typedef   std::pair<std::string,std::string> pr;

#include "../Include/TestFrame/TestHost/testhost_i.h"
#include "../Include/teniointerface.h"
#include "../Include/TestFrame/teststructs.h"
#include "../Include/TestFrame/FileProcess/fileprocess_i.h"
#include "../Include/TestFrame/TestInfoReader/testinforeader_i.h"


namespace Tenio
{
template
	<
		class TComponents, 
		class TComponentsEvent
	>
class TTestObjectImpl
{
	public:
		BOOL AddEvent2Action(int nEventID,int nActionID)
		{
			BOOL bResult=FALSE;
			if(m_mapEvent2Action.insert(std::make_pair(nEventID,nActionID))!=NULL)				
				bResult=TRUE;
			return bResult;
		}
		void SetCurrentRunningAction(int nActionID)
		{
			nCurrentRunningAction = nActionID;
		}
		
		// 可以针对CASE，每个CASE执行前都可以设置，默认是5秒
		void SetAsyncOverTimer(DWORD dwTime)
		{
			m_pTestFrame->SetOverTime(dwTime);
		}
		BOOL IsEventProper(int nEventID)
		{
			BOOL bResult = FALSE;
			typedef std::multimap <int, int>::const_iterator CIT; 
			typedef std::pair<CIT, CIT> Range;
			Range range = m_mapEvent2Action.equal_range(nEventID);
			CIT i = range.first;
			while (i != range.second) {	
				if (i->second == nCurrentRunningAction)
				{
					bResult = TRUE;
					break;
				}
				++i;
			}
			return bResult;
		}
		void TestFrameSet(ITestHost* pTestFrame)
		{
			m_pTestFrame = pTestFrame;
		}
		BOOL ReportAsyncTest(BOOL bChecked,DWORD dwErrorID)
		{
			m_pTestFrame->OnAsyncTest(nCurrentRunningAction,bChecked,dwErrorID);
			return TRUE;
		}
		BOOL ReadPropertyFromXML(LPCSTR lpszFileName,LPCSTR lpszComponentName)
		{
			BOOL bResult = FALSE;
			if(m_pspTestInfoReader->ReadPropertyFromXML(lpszFileName,lpszComponentName,m_szPropertyName,m_szValue))
			{
				bResult=TRUE;
			}
			return bResult;
		}
		
		std::string GetValueFromProperty(std::string szPropertyName)
		{
			std::string szValue;
			for(int i=0;i<MAX_LENGTH;i++)
			{
				if(m_szPropertyName[i]==szPropertyName)
				{
					szValue=m_szValue[i];
				}
			}
			return szValue;
		}

		// Add by living 2007-02-05
		// 根据使用AddEvent2Action设定的规则, 判断现在接收到的Event是否合理的
		// 同时清空 CurrentAction
		
		BOOL ActiveEvent(DWORD dwEvent)
		{
			BOOL bEventIsProper = FALSE;

			if(IsEventProper(dwEvent))
			{
				bEventIsProper = TRUE;

				ReportAsyncTest(TRUE, 0);
			}
			else
			{
				bEventIsProper = FALSE;

				ReportAsyncTest(FALSE, 0);
			}

			SetCurrentRunningAction(0);

			return bEventIsProper;
		}

		TTenioPtr<TComponents> m_pspTestComponent;
		TTenioPtr<IFileProcess> m_pspFileProcess;
		TTenioPtr<ITestInfoReader> m_pspTestInfoReader;
		TComponentsEvent *m_pspTestComponentEvent;
		std::multimap<std::string, std::string> m_mapProperty2Value;
		ITestHost *m_pTestFrame;
		std::string m_szPropertyName[MAX_LENGTH];
		std::string m_szValue[MAX_LENGTH];
		int nCurrentRunningAction;
	private:
		std::multimap<int, int> m_mapEvent2Action;
};
};
#endif