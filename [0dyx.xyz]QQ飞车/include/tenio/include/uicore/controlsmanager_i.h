#ifndef __CONTROLSMANAGER_I_H__
#define __CONTROLSMANAGER_I_H__

#include "teniointerface.h"
#include "boundary/uiboundarymanager_i.h"
#include "../uilib/control_i.h"
#include "../uicore/controlcore_i.h"
#include "../uilib/uilib_i.h"
#include "../uilib/ttooltip_i.h"

namespace Tenio
{
	class IControlsManager;
	TENIO_MAKE_TYPEID_MAP(IControlsManager, COMPONENTID_TOPWINDOWINFO);
	class IControlsManager : public IComponent
	{
	public:
		virtual void SetUILibPtrs(
			IUILib* pUILib,
			IUIBoundaryManager* pBoundaryManager) = 0;
		virtual BOOL IsControlExist(
			LPCTSTR lpszControlName,
			DWORD dwParent,
			BOOL bAdd, 
			DWORD* pdwNew) = 0;
		virtual DWORD AddControl(
			LPCTSTR lpszControlName, 
			DWORD dwParent,
			IControl* pControl, 
			IControlCore* pControlCore) = 0;
		virtual BOOL SetWindowHandle(DWORD dwID, HWND hWnd) = 0;
		virtual BOOL SetEventHandle(DWORD dwID, IUILibEvent* pEvent) = 0;
		virtual BOOL SetListenToChildren(DWORD dwID, BOOL bListen) = 0;
		virtual BOOL DeleteControl(DWORD dwID) = 0;
		virtual void ClearAll() = 0;

		virtual HWND GetControlWin(DWORD dwID) = 0;
		//�鿴�ô��ھ����Ӧ��ID�����ΪINVALID_CONTROLID,���ڴ˴�����ϵ��		
		virtual DWORD GetControlIDFromHWnd(HWND hWnd) = 0;	

		// ģ�����ң�����path�������鲻��������name����
		virtual DWORD GetControlID(LPCTSTR lpszControlName) = 0;
		// ���ݿؼ������ҿؼ�ID�����ص�һ���ؼ���ΪlpszControlName�Ŀؼ�ID
		virtual DWORD GetControlIDbyName(LPCTSTR lpszControlName, DWORD dwRelativeControl) = 0;
		// ���ݿؼ�·�������ҿؼ�ID������a.b.c.d
		virtual DWORD GetControlIDbyPath(LPCTSTR lpszControlPath, DWORD dwRelativeControl) = 0;

		virtual IControl* GetControl(DWORD dwID) = 0;
		virtual IControlCore* GetControlCore(DWORD dwID) = 0;
		virtual IUILibEvent* GetEventHandle(DWORD dwID) = 0;

		virtual BOOL GetNameByControlId(
			DWORD dwControlId, 
			LPTSTR lpszNameBuffer, 
			int nCharCount) = 0;

		/**
		 *  @brief �ؼ�����
		 *  @param dwControl �ؼ�ID
		 *  @param lpszNewName,�¿ؼ���
		 *  @return���ɹ�����TRUE��ʧ�ܷ���FALSE
		 */		
		virtual BOOL RenameControl(DWORD dwControl, LPCTSTR lpszNewName) = 0;
		
		virtual void SetTopWindowParent(HWND hWnd) = 0;
		virtual HWND GetTopWindowParent() = 0;
		virtual DWORD SetFocusControl(DWORD dwID) = 0;
		virtual DWORD GetFocusControl() = 0;
		virtual DWORD SetCaptureControl(DWORD dwID) = 0;
		virtual DWORD GetCaptureControl() = 0;
		
		// �����¼����ⲿʹ����
		virtual void FireEvent(DWORD dwID, IVariableParam* pParam) = 0; 

		// ������Ϣ��IDΪdwControlID�Ŀؼ�
		virtual BOOL SendMessageTo(
			UINT nMessage, 
			WPARAM wParam, 
			LPARAM lParam, 
			LRESULT* pResult, 
			DWORD dwControlID) = 0;		

		
		// �˴�ע��ļ��������Ի�ȡ������㴰�ڵ�����UI��Ϣ
		virtual BOOL RegisterUIEventHandler(IUILibEvent* pUIEventHandler) = 0;
		virtual void UnregisterUIEventHandler(IUILibEvent* pUIEventHandler) = 0;

		virtual ITToolTip* GetGlobalTipHandle() = 0;
		virtual void SetGlobalTipHandle(ITToolTip* pToolTip) = 0;

		virtual void SetDefaultButtonID(DWORD dwDefaultButtonID) = 0;
		virtual DWORD GetDefaultButtonID() = 0;

		/**
		 *  @brief �����д��ھ���ĸ��ؼ�ID
		 *  @param �߼��ϵĸ��ؼ�ID
		 *  @return ������ʵ���ھ���ĸ��ؼ�ID
		 *  @sa 
		 */	
		virtual DWORD FindRealParent(DWORD dwParent) = 0;

		/**
		 *  @brief ��þ�����Scene�ϵ�����
		 *  @param  dwControlID �ؼ�ID
		 *  @param ��rcΪ����ڸ����ڵľ���
		 *  @return ����rc��Scene�ϵ�����
		 *  @sa 
		 */		
		virtual RECT GetRectInScene(DWORD dwControlID, RECT& rc) = 0;

		/**
		 *  @brief ��õ������ĸ����ھ��
		 *  @param �߼��ϵĸ��ؼ�ID
		 *  @return �������������ھ�����������д����游�ؼ��ľ����Ҳ�������ⲿ���ھ��
		 */		
		virtual HWND FindRealParentHWnd(DWORD dwParent) = 0;

		/**
		 *  @brief ��ÿؼ�����Ļ����
		 *  @return ���ؿؼ�����Ļ����
		 *  @sa 
		 */
		virtual RECT GetScreenRect(DWORD dwControl) = 0;

		/**
		 *  @brief ����Ļ����ת��������ڶ���ؼ�������
		 *  @param ��Ļ����
		 */	
		virtual void MapScreenToRoot(POINT& ptScreen) = 0;		

		/**
		 *  @brief ���ýű�ִ����
		 *  @param pUIScriptManager �ű�ִ����ָ��
		 */	
		virtual void SetScriptManager(IUIScriptManager* pUIScriptManager) = 0;		
	};
}


#endif