/********************************************************************
　created         : 2006/10/19  15:23
　filename        : uihelper.h
　author          : season
　version         : 1.0
　description     : 使用UILib的帮助类库
　history         :
　1 day	          : 2006/10/19
　　author        : season
　　modification: : created	
*********************************************************************/
#ifndef __UIHELPER_H__
#define __UIHELPER_H__

#include "tchar.h"
#include "stdio.h"

#include "../teniointerface.h"
#include "../objecttypeidmap.h"
#include "teniopointer_t.h"
#include "paramhelper.h"
#include "../uilib/uilibdefine.h"

#include "../uilib/uilib_i.h"
#include "../uilib/control_i.h"
#include "../uilib/tbutton_i.h"
#include "../uilib/tstatic_i.h"
#include "../uilib/tlist_i.h"
#include "../uilib/tcombobox_i.h"
#include "../uilib/tedit_i.h"
#include "../uilib/container_i.h"
#include "../uilib/tcheckbox_i.h"

namespace Tenio
{
	TENIO_MAKE_TYPEID_MAP(IControl, -2)


#define USEPARENTCREATOR(dwID)							\
	TSmartUI() : TSmartUI<void>(){}						\
	TSmartUI(CUILibHelper& UILib, LPCTSTR lpszControlName)	\
		: TSmartUI<void>(UILib, lpszControlName){}		\
	TSmartUI(CUILibHelper& UILib, LPCTSTR lpszControlPath, DWORD dwRelativeControl)\
		: TSmartUI<void>(UILib, lpszControlPath, dwRelativeControl){}	\
		TSmartUI(IUILib* pUILib, LPCTSTR lpszControlName)	\
		: TSmartUI<void>(pUILib, lpszControlName){}		\
		TSmartUI(IUILib* pUILib, LPCTSTR lpszControlPath, DWORD dwRelativeControl)\
		: TSmartUI<void>(pUILib, lpszControlPath, dwRelativeControl){}	\
		BOOL Create(IUILib* pUILib, LPCTSTR lpszControlName, DWORD dwParent, RECT& rc)\
	{\
		return Inner_Create(pUILib, lpszControlName, dwParent, rc, dwID);\
	}\
	

	//////////////////////////////////////////////////////////////////////////
	// UILib帮助实现类
	//////////////////////////////////////////////////////////////////////////
	class CUILibHelper
	{
	public:
		CUILibHelper()
		{
		}
		
		CUILibHelper(LPCTSTR lpszXMLFile, HWND hWndRootParent/* = NULL*/)
		{
			Restore(lpszXMLFile, hWndRootParent);
		}
		
		BOOL Restore(LPCTSTR lpszXMLFile, HWND hWndRootParent/* = NULL*/)
		{
			BOOL bSuccess = FALSE;
			if(m_pUILib)
			{
				m_pUILib->ClearAll();
				m_pUILib->SetRootParentHandle(hWndRootParent);
				bSuccess = m_pUILib->RestoreFromXMLFile(lpszXMLFile);
			}
			return bSuccess;
		}
		
		BOOL RestoreFromNode(LPCTSTR lpszXMLNode, DWORD dwParent)
		{
			BOOL bSuccess = FALSE;
			if(m_pUILib)
			{
				bSuccess = m_pUILib->RestoreFromXMLNode(dwParent, lpszXMLNode);
			}
			return bSuccess;
		}

		BOOL DeleteControl(DWORD dwControlID)
		{
			BOOL bSuccess = FALSE;
			if(m_pUILib)
			{
				bSuccess = m_pUILib->DeleteControl(dwControlID);
			}
			return bSuccess;
		}
		
		void Show(BOOL bShow)
		{
			if(m_pUILib)
			{
				IControl* pControl = (IControl*)m_pUILib->GetRootControlPtr();
				if(pControl != NULL)
				{
					pControl->Show(bShow);
				}
			}
		}
		
		// 先作为控件全路径查找控件ID，如果查不到则作为控件名查找
		DWORD GetControlID(LPCTSTR lpszControlPathorName)
		{
			DWORD dwControlID = INVALID_CONTROLID;
			if(m_pUILib)
			{
				dwControlID = m_pUILib->GetControlIDbyPath(lpszControlPathorName, INVALID_CONTROLID);
				if(dwControlID == INVALID_CONTROLID)
				{
					dwControlID = m_pUILib->GetControlID(lpszControlPathorName);
				}
			}
			return dwControlID;
		}
		
		// 只作为控件名查找
		DWORD GetControlIDFromName(LPCTSTR lpszControlName)
		{
			DWORD dwControlID = INVALID_CONTROLID;
			if(m_pUILib)
			{
				dwControlID = m_pUILib->GetControlID(lpszControlName);
			}
			return dwControlID;
		}
		
		// 只作为控件相对路径查找，当dwRelativeControl为INVALID_CONTROLID时，作为全路径查找
		DWORD GetControlIDFramPath(LPCTSTR lpszControlPath, DWORD dwRelativeControl)
		{
			DWORD dwControlID = INVALID_CONTROLID;
			if(m_pUILib)
			{
				dwControlID = m_pUILib->GetControlIDbyPath(lpszControlPath, dwRelativeControl);
			}
			return dwControlID;
		}
		
		
		IUILib* GetUILibPtr()
		{
			return (IUILib*)m_pUILib;
		}

		


	private:
		TTenioPtr<IUILib> m_pUILib;
	};

	//////////////////////////////////////////////////////////////////////////
	// 控件帮助实现缺省类
	//////////////////////////////////////////////////////////////////////////
	template<class TInterface = IControl>
	class TSmartUI
	{
	public:
		TSmartUI()
		{
			m_pControl = NULL;
		}

		TSmartUI(CUILibHelper& UILib, LPCTSTR lpszControlName)
		{
			IUILib* pUILib = UILib.GetUILibPtr();
			Attach(pUILib, lpszControlName);
		}
		TSmartUI(CUILibHelper& UILib, LPCTSTR lpszControlPath, DWORD dwRelativeControlID)
		{
			IUILib* pUILib = UILib.GetUILibPtr();			
			Attach(pUILib, lpszControlPath, dwRelativeControlID);
		}
		TSmartUI(IUILib* pUILib, LPCTSTR lpszControlName)
		{
			Attach(pUILib, lpszControlName);
		}
		TSmartUI(IUILib* pUILib, LPCTSTR lpszControlPath, DWORD dwRelativeControlID)
		{
			Attach(pUILib, lpszControlPath, dwRelativeControlID);
		}
		
		BOOL Attach(IUILib* pUILib, LPCTSTR lpszControlPath)
		{
			BOOL bSuccess = FALSE;
			Detach();

			if(pUILib != NULL)
			{
				DWORD dwID = pUILib->GetControlID(lpszControlPath);
				m_pControl = (IControl*) pUILib->GetControlPtr(dwID);
				if(m_pControl != NULL)
				{
					bSuccess = TRUE;
				}
			}
			return bSuccess;
		}

		BOOL Attach(IUILib* pUILib, LPCTSTR lpszControlPath, DWORD dwRelativeControlID)
		{
			BOOL bSuccess = FALSE;
			Detach();
			
			if(pUILib != NULL)
			{
				DWORD dwID = pUILib->GetControlIDbyPath(lpszControlPath, dwRelativeControlID);
				m_pControl = (IControl*) pUILib->GetControlPtr(dwID);
				if(m_pControl != NULL)
				{
					bSuccess = TRUE;
				}
			}
			return bSuccess;
		}			

		void Detach()
		{
			m_pControl = NULL;
		}

		virtual BOOL Create(IUILib* pUILib, LPCTSTR lpszControlName, DWORD dwParent, RECT& rc)
		{
			return Inner_Create(pUILib, lpszControlName, dwParent, rc, Type2ID<TInterface>::ComponentID());
		}

		BOOL Inner_Create(IUILib* pUILib, LPCTSTR lpszControlName, DWORD dwParent, RECT& rc, DWORD dwType)
		{
			BOOL bSuccess = FALSE;

			if(pUILib != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_TYPEID, (DWORD)dwType);
				m_paramHelper.SetRectParam(rc, m_pParam);
				DWORD dwControlID = pUILib->CreateControl(m_pParam, lpszControlName, dwParent);
				if(dwControlID != INVALID_CONTROLID)
				{
					m_pControl = (IControl*)pUILib->GetControlPtr(dwControlID);
					bSuccess = TRUE;
				}
			}
			return bSuccess;
		}

		virtual BOOL IsValid()
		{
			return (m_pControl != NULL);
		}
			
		BOOL CreateIndirect(
			IUILib* pUILib, 
			DWORD dwParent,
			LPCTSTR lpszControlName, 
			IVariableParam* pParam)
		{
			BOOL bSuccess = FALSE;

			if(pUILib != NULL)
			{
				DWORD dwControlID = pUILib->CreateControl(pParam, lpszControlName, dwParent);
				if(dwControlID != INVALID_CONTROLID)
				{
					m_pControl = (IControl*)pUILib->GetControlPtr(dwControlID);
					bSuccess = TRUE;
				}					
			}
			return bSuccess;
		}

		DWORD GetControlID()
		{
			DWORD dwControlID = INVALID_CONTROLID;
			if(m_pControl)
			{
				dwControlID = m_pControl->GetControlID();
			}
			return dwControlID;
		}

		void SetParent(IControl* pParent)
		{
			if(m_pControl != NULL)
			{
				m_pControl->SetParent(pParent);
			}
		}
		
		void SetProperty(IVariableParam* pParam)
		{
			if(m_pControl != NULL)
			{
				m_pControl->SetProperty(pParam);
			}
		}
		
		IControl* GetParent()
		{
			IControl* pParent = NULL;
			if(m_pControl != NULL)
			{
				pParent = m_pControl->GetParent();
			}
			return pParent;
		}
				
		RECT GetRect(BOOL bScreen)
		{
			RECT rc = {0};
			if(m_pControl != NULL)
			{
				rc = m_pControl->GetRect(bScreen);
			}
			return rc;
		}
		
		BOOL IsVisible()
		{
			BOOL bVisible = FALSE;

			if(m_pControl != NULL)
			{
				bVisible = m_pControl->IsVisible();
			}
			return bVisible;
		}
		
		BOOL IsDisable()
		{
			BOOL bDisable = FALSE;

			if(m_pControl != NULL)
			{
				bDisable = m_pControl->IsDisable();
			}
			return bDisable;
		}
				
		void Move(RECT& rc)
		{
			if(m_pControl != NULL)
			{
				m_pControl->Move(rc);
			}
		}

		void Offset(int x, int y)
		{
			if(m_pControl != NULL)
			{
				RECT rc = m_pControl->GetRect(FALSE);
				::OffsetRect(&rc, x - rc.left, y - rc.top);
				m_pControl->Move(rc);
			}
		}
		
		// w和h等于-1时不改变
		void SetSize(int w, int h)
		{
			if(m_pControl != NULL)
			{
				RECT rc = m_pControl->GetRect(FALSE);
				if(w != -1)
				{
					rc.right = rc.left + w;
				}
				if(h != -1)
				{
					rc.bottom = rc.top + h;
				}
				m_pControl->Move(rc);
			}
		}

		void Show(BOOL bShow)
		{
			if(m_pControl != NULL)
			{
				m_pControl->Show(bShow);
			}
		}	
		
		void Enable(BOOL bEnable)
		{
			if(m_pControl != NULL)
			{
				m_pControl->Enable(bEnable);
			}
		}
		
		void ReDraw(RECT* rc)
		{
			if(m_pControl != NULL)
			{
				m_pControl->ReDraw(rc);
			}
		}
		
		void BringToTop()
		{
			if(m_pControl != NULL)
			{
				m_pControl->BringToTop();
			}
		}
		
		void SetToolTipText(LPCTSTR lpszText)
		{
			if(m_pControl != NULL)
			{
				m_pControl->SetToolTipText(lpszText);
			}
		}
		
		void SetFocus()
		{		
			if(m_pControl != NULL)
			{
				m_pControl->SetFocus();
			}
		}
		
		void SetCapture()
		{
			if(m_pControl != NULL)
			{
				m_pControl->SetCapture();
			}
		}
		
		void ReleaseCapture()
		{
			if(m_pControl != NULL)
			{
				m_pControl->ReleaseCapture();
			}
		}
		
		void LockUpdate()
		{
			if(m_pControl != NULL)
			{
				m_pControl->LockUpdate();
			}
		}
		
		void UnLockUpdate()
		{
			if(m_pControl != NULL)
			{
				m_pControl->UnLockUpdate();
			}
		}		

		BOOL IsMyOffspring(DWORD dwControlID)
		{
			BOOL bRet = FALSE;
			if(m_pControl != NULL)
			{
				bRet = m_pControl->IsMyOffspring(dwControlID);
			}
		}

		void ShowToolTip()
		{
			if(m_pControl != NULL)
			{
				m_pControl->ShowToolTip();
			}
		}
			

		IControl* GetControlPtr()
		{
			return m_pControl;
		}

		//////////////////////////////////////////////////////////////////////////
		// 属性相关
		void SetTransparent(BOOL bTransparent)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_TRANSPARENT, (int)bTransparent);
				m_pControl->SetProperty(m_pParam);
			}			
		}

		void SetAlpha(BYTE byAlpha)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_ALPHA, (BYTE)byAlpha);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetTabStop(BOOL bTabStop)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_TABSTOP, (int)bTabStop);
				m_pControl->SetProperty(m_pParam);
			}			
		}

		void SetBackground(COLORREF crBG, int nFlag = PROPERTYID_BGMODE)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_paramHelper.SetBgParam(nFlag, crBG, m_pParam);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetBackground(LPCTSTR lpszImageFile, int nMode, int nFlag = PROPERTYID_BGMODE)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_paramHelper.SetBgParam(nFlag, lpszImageFile, nMode, m_pParam);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetBackground(
			LPCTSTR lpszImageFile1, 
			LPCTSTR lpszImageFile2, 
			LPCTSTR lpszImageFile3,
			BOOL bHorizontal,
			int nFlag = PROPERTYID_BGMODE)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_paramHelper.SetBgParam(
					nFlag, 
					lpszImageFile1, 
					lpszImageFile2, 
					lpszImageFile3,
					bHorizontal,
					m_pParam);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetBackground(
			LPCTSTR lpszTopLeft,
			LPCTSTR lpszTop,
			LPCTSTR lpszTopRight,
			LPCTSTR lpszLeft,
			LPCTSTR lpszMiddle,
			LPCTSTR lpszRight,
			LPCTSTR lpszBottomLeft,
			LPCTSTR lpszBottom,
			LPCTSTR lpszBottomRight,
			int nFlag = PROPERTYID_BGMODE)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_paramHelper.SetBgParam(
					nFlag,
					lpszTopLeft,
					lpszTop,
					lpszTopRight,
					lpszLeft,
					lpszMiddle,
					lpszRight,
					lpszBottomLeft,
					lpszBottom,
					lpszBottomRight,
					m_pParam);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetFont(LOGFONT& lf)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_paramHelper.SetFontSetParam(lf, m_pParam);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetLayoutMode(ENMLAYOUTMODE eLayoutMode)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_LAYOUTMODE, (int)eLayoutMode);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetOuterMargins(int nLeft, int nTop, int nRight, int nBottom)
		{
			TCHAR szValue[128] = {0};
			m_paramHelper.Margins2String(nLeft, nTop, nRight, nBottom, szValue, 128);
			m_pParam->Clear();
			m_pParam->SetValue(PROPERTYID_OUTERMARGINS, szValue);
		}

		void SetInnerMargins(int nLeft, int nTop, int nRight, int nBottom)
		{
			TCHAR szValue[128] = {0};
			m_paramHelper.Margins2String(nLeft, nTop, nRight, nBottom, szValue, 128);
			m_pParam->Clear();
			m_pParam->SetValue(PROPERTYID_INNERMARGINS, szValue);
		}	
		
		//////////////////////////////////////////////////////////////////////////
	public:
		IControl* m_pControl;

	protected:
		CParamHelper m_paramHelper;
		TTenioPtr<IVariableParam> m_pParam;
	};

	//////////////////////////////////////////////////////////////////////////
	// Static　特化
	//////////////////////////////////////////////////////////////////////////
	template<>
	class TSmartUI<ITStatic> : public TSmartUI<void>
	{
	public:
		USEPARENTCREATOR(COMPONENTID_TSTATIC)
		
		void SetText(LPCTSTR lpszText)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_CAPTION, lpszText);
				m_pControl->SetProperty(m_pParam);
			}
		}
		
		void SetTextColor(COLORREF cr)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_TEXTCOLOR, (DWORD)cr);
				m_pControl->SetProperty(m_pParam);
			}
		}	
	};

	//////////////////////////////////////////////////////////////////////////
	// Button
	//////////////////////////////////////////////////////////////////////////
	template<>
	class TSmartUI<ITButton> : public TSmartUI<void>
	{
	public:
		USEPARENTCREATOR(COMPONENTID_TBUTTON)
		
		BOOL Create(
			IUILib* pUILib, 
			LPCTSTR lpszControlName, 
			DWORD dwParent, 
			RECT& rc,
			LPCTSTR lpszCaption,
			BOOL bNormalButton /*= TRUE*/,
			BOOL bDefault/* = FALSE*/,
			BOOL bTabStop/* = FALSE*/)
		{
			BOOL bSuccess = Create(pUILib, lpszControlName, dwParent, rc);

			if(bSuccess)
			{
				m_pParam->Clear();
				
				std::tstring strValue = _T("NormalButton");
				if(!bNormalButton)
				{
					strValue = _T("UnreversedButton");
				}
				m_pParam->SetValue(PROPERTYID_CAPTION, lpszCaption);
				m_pParam->SetValue(PROPERTYID_BUTTONTYPE, strValue.c_str());
				m_pParam->SetValue(PROPERTYID_DEFAULTBUTTON, (int)bDefault);
				m_pParam->SetValue(PROPERTYID_TABSTOP, (int)bTabStop);
				m_pControl->SetProperty(m_pParam);
			}
			return bSuccess;
		}
		
		void SetSignaled(BOOL bSignal)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_SIGNALED, (int)bSignal);
				m_pControl->SetProperty(m_pParam);
			}
		}
		
		void SetText(LPCTSTR lpszText)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_CAPTION, lpszText);
				m_pControl->SetProperty(m_pParam);
			}
		}

		void SetTextColor(
			COLORREF crNormal, 
			COLORREF crHover/* = -1*/, 
			COLORREF crPressed/* = -1*/)
		{
			if(m_pControl != NULL)
			{
				if(crHover == -1)
				{
					crHover = crNormal;
				}
				if(crPressed == -1)
				{
					crPressed = crNormal;
				}
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_TEXTCOLOR, (DWORD)crHover);
				m_pParam->SetValue(PROPERTYID_TEXTCOLOR_HOVER, (DWORD)crHover);
				m_pParam->SetValue(PROPERTYID_TEXTCOLOR_PRESSED, (DWORD)crPressed);

				m_pControl->SetProperty(m_pParam);
			}
		}
		
		void SetHoverBg(COLORREF cr)
		{
			SetBackground(cr, PROPERTYID_BGMODE_HOVER);
		}

		void SetPressedBg(COLORREF cr)
		{
			SetBackground(cr, PROPERTYID_BGMODE_PRESSED);
		}

		void SetDisableBg(COLORREF cr)
		{
			SetBackground(cr, PROPERTYID_BGMODE_DISABLE);
		}
		
		void SetHoverBg(LPCTSTR lpszImageFile, int nMode)
		{
			SetBackground(lpszImageFile, nMode, PROPERTYID_BGMODE_HOVER);
		}

		void SetPressedBg(LPCTSTR lpszImageFile, int nMode)
		{
			SetBackground(lpszImageFile, PROPERTYID_BGMODE_PRESSED);
		}

		void SetDisableBg(LPCTSTR lpszImageFile, int nMode)
		{
			SetBackground(lpszImageFile, PROPERTYID_BGMODE_DISABLE);
		}
		
		void SetHoverBg(			
			LPCTSTR lpszImageFile1, 
			LPCTSTR lpszImageFile2, 
			LPCTSTR lpszImageFile3,
			BOOL bHorizontal)
		{
			SetBackground(lpszImageFile1, lpszImageFile2, lpszImageFile3, 
				bHorizontal, PROPERTYID_BGMODE_HOVER);
		}

		void SetPressedBg(
			LPCTSTR lpszImageFile1, 
			LPCTSTR lpszImageFile2, 
			LPCTSTR lpszImageFile3,
			BOOL bHorizontal)
		{
			SetBackground(lpszImageFile1, lpszImageFile2, lpszImageFile3, 
				bHorizontal, PROPERTYID_BGMODE_PRESSED);
		}

		void SetDisableBg(
			LPCTSTR lpszImageFile1, 
			LPCTSTR lpszImageFile2, 
			LPCTSTR lpszImageFile3,
			BOOL bHorizontal)
		{
			SetBackground(lpszImageFile1, lpszImageFile2, lpszImageFile3, 
				bHorizontal, PROPERTYID_BGMODE_DISABLE);
		}
		
		void SetHoverBg(
			LPCTSTR lpszTopLeft,
			LPCTSTR lpszTop,
			LPCTSTR lpszTopRight,
			LPCTSTR lpszLeft,
			LPCTSTR lpszMiddle,
			LPCTSTR lpszRight,
			LPCTSTR lpszBottomLeft,
			LPCTSTR lpszBottom,
			LPCTSTR lpszBottomRight)
		{
			SetBackground(
				lpszTopLeft,
				lpszTop,
				lpszTopRight,
				lpszLeft,
				lpszMiddle,
				lpszRight,
				lpszBottomLeft,
				lpszBottom,
				lpszBottomRight,
				PROPERTYID_BGMODE_HOVER);
		}

		void SetPressedBg(
			LPCTSTR lpszTopLeft,
			LPCTSTR lpszTop,
			LPCTSTR lpszTopRight,
			LPCTSTR lpszLeft,
			LPCTSTR lpszMiddle,
			LPCTSTR lpszRight,
			LPCTSTR lpszBottomLeft,
			LPCTSTR lpszBottom,
			LPCTSTR lpszBottomRight)
		{
			SetBackground(
				lpszTopLeft,
				lpszTop,
				lpszTopRight,
				lpszLeft,
				lpszMiddle,
				lpszRight,
				lpszBottomLeft,
				lpszBottom,
				lpszBottomRight,
				PROPERTYID_BGMODE_PRESSED);
		}
		void SetDisableBg(
			LPCTSTR lpszTopLeft,
			LPCTSTR lpszTop,
			LPCTSTR lpszTopRight,
			LPCTSTR lpszLeft,
			LPCTSTR lpszMiddle,
			LPCTSTR lpszRight,
			LPCTSTR lpszBottomLeft,
			LPCTSTR lpszBottom,
			LPCTSTR lpszBottomRight)
		{
			SetBackground(
				lpszTopLeft,
				lpszTop,
				lpszTopRight,
				lpszLeft,
				lpszMiddle,
				lpszRight,
				lpszBottomLeft,
				lpszBottom,
				lpszBottomRight,
				PROPERTYID_BGMODE_DISABLE);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// Edit
	//////////////////////////////////////////////////////////////////////////
	template<>
	class TSmartUI<ITEdit> : public TSmartUI<void>
	{
	public:
		USEPARENTCREATOR(COMPONENTID_TEDIT)

		void SetText(LPCTSTR lpszText)
		{
			if(m_pControl != NULL)
			{
				m_pParam->Clear();
				m_pParam->SetValue(PROPERTYID_CAPTION, lpszText);
				m_pControl->SetProperty(m_pParam);
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// CheckBox
	//////////////////////////////////////////////////////////////////////////
	template<>
	class TSmartUI<ITCheckBox> : public TSmartUI<void>
	{
	public:
		USEPARENTCREATOR(COMPONENTID_TCHECKBOX)

		BOOL IsValid()
		{
			return (m_pControl != NULL && 
				(m_pControl->GetComponentID() == COMPONENTID_TCHECKBOX));
		}
			
		void SetChecked(BOOL bChecked)
		{
			if(IsValid())
			{
				ITCheckBox* pCheckBox = static_cast<ITCheckBox*>(m_pControl);
				pCheckBox->SetChecked(bChecked);
			}
		}

		BOOL IsChecked()
		{
			BOOL bChecked = FALSE;
			if(IsValid())
			{
				ITCheckBox* pCheckBox = static_cast<ITCheckBox*>(m_pControl);
				bChecked = pCheckBox->IsChecked();
			}
			return bChecked;
		}
	};
}

#endif
