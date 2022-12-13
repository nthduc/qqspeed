/********************************************************************
　created         : 2006/10/25  18:46
　filename        : uievent_t
　author          : season
　version         : 1.0
　description     : UILib消息处理模板类
					1,需要监听UI库控件消息者先继承该模板类:
						class Cxxxx : public TUIEventImpl<Cxxxx>
					2,初始化UILib指针，调用InitUILibEventHandle();
					3,注释事件处理函数：
						RegisterEventFun_xxx(lpszControlName, OnXXXX);
						lpszControlName为需要监听的控件名，
						OnXXXX为处理此事件的函数名
　history         :
　1 day	          : 2006/10/25
　　author        : season
　　modification: : created	
*********************************************************************/
#ifndef __UIEVENT_T_H__
#define __UIEVENT_T_H__
#include "../uilib/uilibdefine.h"
#include "../uilib/uilib_i.h"
#include "map"
using namespace std;

namespace Tenio
{
	//////////////////////////////////////////////////////////////////////////
	template<class TClass>
	class TUIEventImpl : public IUILibEvent
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		// 消息处理函数定义
		//////////////////////////////////////////////////////////////////////////
		typedef void (TClass::*PFCOMMON)();
		typedef void (TClass::*PFCLICKBUTTON)(int x, int y);
		typedef void (TClass::*PFSIGNALED)();
		typedef void (TClass::*PFUNSIGNALED)();
		typedef void (TClass::*PFBASEMESSAGE)(UINT nMessage, WPARAM wParam, 
			LPARAM lParam, LRESULT* pResult);
		//////////////////////////////////////////////////////////////////////////
		typedef map<DWORD, PFCOMMON> DWORD2PF;		//EventID　对应 函数指针
		typedef map<DWORD, DWORD2PF> DWORD2MAP;	//ControlID 对应　所有函数指针集
		//////////////////////////////////////////////////////////////////////////

	public:
		TUIEventImpl()
		{
			m_pUILib = NULL;
		}

		int GetEventVersion(){return 1;}

		void InitUILibEventHandle(IUILib* pUILib)
		{
			if(pUILib != NULL)
			{
				pUILib->RegisterEventHandle(ROOT_CONTROLID, (IUILibEvent*)this);
			}
			m_pUILib = pUILib;
		}

		void OnControlMessage(DWORD dwID, IVariableParam* pParam)
		{
			DWORD2MAP::iterator iter =  m_map.find(dwID);

			if(iter != m_map.end())
			{
				int nEventID = pParam->GetInt(EPARAMID_EVENTID, FALSE);
				DWORD2PF::iterator iterEvent = iter->second.find(nEventID);

				if(iterEvent != iter->second.end())
				{
					PFCOMMON pf = iterEvent->second;

					switch(nEventID)
					{
					case EI_BUTTON_CLICK:
						OnButtonClick(pParam, &pf);
						break;
					case EI_SIGNALED:
						OnSignaled(&pf);
						break;
					case EI_UNSIGNALED:
						OnUnSignaled(&pf);
						break;
					case EI_BASEMESSAGE:
						OnBaseMessage(pParam, &pf);
					default:;
					}
				}
			}
		}

		void UnRegisterEventFun(DWORD dwControlID)
		{
			if(dwControlID != INVALID_CONTROLID)
			{
				DWORD2MAP::iterator iter = m_map.find(dwControlID);
				if(iter != m_map.end())
				{
					m_map.erase(iter);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		void RegisterEventFun_Click(DWORD dwID, PFCLICKBUTTON pf)
		{
			RegisterEventFun(dwID, EI_BUTTON_CLICK, (PFCOMMON)pf);
		}
		
		void RegisterEventFun_Signaled(DWORD dwID, PFSIGNALED pf)
		{
			RegisterEventFun(dwID, EI_SIGNALED, (PFCOMMON)pf);
		}

		void RegisterEventFun_UnSignaled(DWORD dwID, PFUNSIGNALED pf)
		{
			RegisterEventFun(dwID, EI_UNSIGNALED, (PFCOMMON)pf);
		}
		
		void RegisterEventFun_BaseMessage(DWORD dwID, PFBASEMESSAGE pf)
		{
			RegisterEventFun(dwID, EI_BASEMESSAGE, (PFCOMMON)pf);
		}

	protected:
		void RegisterEventFun(DWORD dwID, DWORD dwEventID, PFCOMMON pf)
		{	
			if(dwID != INVALID_CONTROLID)
			{
				m_map[dwID][dwEventID] = pf;
			}
		}	

		inline TClass* Me()
		{
			return static_cast<TClass*>(this);
		}
		
		//////////////////////////////////////////////////////////////////////////
		void OnButtonClick(IVariableParam* pParam, PFCOMMON* pfCommon)
		{
			int x = pParam->GetInt(EPARAMID_BUTTON_CLICK_X, FALSE);
			int y = pParam->GetInt(EPARAMID_BUTTON_CLICK_Y, FALSE);
			
			PFCLICKBUTTON pf = *((PFCLICKBUTTON*)pfCommon);								
			(Me()->*pf)(x, y);
		}

		void OnSignaled(PFCOMMON* pfCommon)
		{
			PFSIGNALED pf = *((PFSIGNALED*)pfCommon);
			(Me()->*pf)();
		}

		void OnUnSignaled(PFCOMMON* pfCommon)
		{
			PFUNSIGNALED pf = *((PFUNSIGNALED*)pfCommon);
			(Me()->*pf)();
		}

		void OnBaseMessage(IVariableParam* pParam, PFCOMMON* pfCommon)
		{
			PFBASEMESSAGE pf = *((PFBASEMESSAGE*)pfCommon);
			UINT uMessage = pParam->GetDword(PARAMID_MESSAGE, FALSE);
			WPARAM wParam = pParam->GetDword(PARAMID_WPARAM, FALSE);
			LPARAM lParam = pParam->GetDword(PARAMID_LPARAM, FALSE);
			LRESULT* pResult = (LRESULT*)pParam->GetDword(PARAMID_RESULT, FALSE);
			(Me()->*pf)(uMessage, wParam, lParam, pResult);
		}

	private:
		DWORD2MAP m_map;
		IUILib* m_pUILib;
		
	};
}

#endif