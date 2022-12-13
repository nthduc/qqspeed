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
		//查看该窗口句柄对应的ID，如果为INVALID_CONTROLID,则不在此窗口体系中		
		virtual DWORD GetControlIDFromHWnd(HWND hWnd) = 0;	

		// 模糊查找，先用path方法，查不出来再用name方法
		virtual DWORD GetControlID(LPCTSTR lpszControlName) = 0;
		// 根据控件名查找控件ID，返回第一个控件名为lpszControlName的控件ID
		virtual DWORD GetControlIDbyName(LPCTSTR lpszControlName, DWORD dwRelativeControl) = 0;
		// 根据控件路径名查找控件ID。例：a.b.c.d
		virtual DWORD GetControlIDbyPath(LPCTSTR lpszControlPath, DWORD dwRelativeControl) = 0;

		virtual IControl* GetControl(DWORD dwID) = 0;
		virtual IControlCore* GetControlCore(DWORD dwID) = 0;
		virtual IUILibEvent* GetEventHandle(DWORD dwID) = 0;

		virtual BOOL GetNameByControlId(
			DWORD dwControlId, 
			LPTSTR lpszNameBuffer, 
			int nCharCount) = 0;

		/**
		 *  @brief 控件改名
		 *  @param dwControl 控件ID
		 *  @param lpszNewName,新控件名
		 *  @return　成功返回TRUE，失败返回FALSE
		 */		
		virtual BOOL RenameControl(DWORD dwControl, LPCTSTR lpszNewName) = 0;
		
		virtual void SetTopWindowParent(HWND hWnd) = 0;
		virtual HWND GetTopWindowParent() = 0;
		virtual DWORD SetFocusControl(DWORD dwID) = 0;
		virtual DWORD GetFocusControl() = 0;
		virtual DWORD SetCaptureControl(DWORD dwID) = 0;
		virtual DWORD GetCaptureControl() = 0;
		
		// 发送事件给外部使用者
		virtual void FireEvent(DWORD dwID, IVariableParam* pParam) = 0; 

		// 发送消息给ID为dwControlID的控件
		virtual BOOL SendMessageTo(
			UINT nMessage, 
			WPARAM wParam, 
			LPARAM lParam, 
			LRESULT* pResult, 
			DWORD dwControlID) = 0;		

		
		// 此处注册的监听器可以获取这个顶层窗口的所有UI消息
		virtual BOOL RegisterUIEventHandler(IUILibEvent* pUIEventHandler) = 0;
		virtual void UnregisterUIEventHandler(IUILibEvent* pUIEventHandler) = 0;

		virtual ITToolTip* GetGlobalTipHandle() = 0;
		virtual void SetGlobalTipHandle(ITToolTip* pToolTip) = 0;

		virtual void SetDefaultButtonID(DWORD dwDefaultButtonID) = 0;
		virtual DWORD GetDefaultButtonID() = 0;

		/**
		 *  @brief 查找有窗口句柄的父控件ID
		 *  @param 逻辑上的父控件ID
		 *  @return 具有真实窗口句柄的父控件ID
		 *  @sa 
		 */	
		virtual DWORD FindRealParent(DWORD dwParent) = 0;

		/**
		 *  @brief 获得矩形在Scene上的坐标
		 *  @param  dwControlID 控件ID
		 *  @param 　rc为相对于父窗口的矩形
		 *  @return 返回rc在Scene上的坐标
		 *  @sa 
		 */		
		virtual RECT GetRectInScene(DWORD dwControlID, RECT& rc) = 0;

		/**
		 *  @brief 获得得真正的父窗口句柄
		 *  @param 逻辑上的父控件ID
		 *  @return 返回真正父窗口句柄，可能是有窗口祖父控件的句柄，也可能是外部窗口句柄
		 */		
		virtual HWND FindRealParentHWnd(DWORD dwParent) = 0;

		/**
		 *  @brief 获得控件的屏幕矩形
		 *  @return 返回控件的屏幕矩形
		 *  @sa 
		 */
		virtual RECT GetScreenRect(DWORD dwControl) = 0;

		/**
		 *  @brief 将屏幕坐标转换成相对于顶层控件的坐标
		 *  @param 屏幕坐标
		 */	
		virtual void MapScreenToRoot(POINT& ptScreen) = 0;		

		/**
		 *  @brief 设置脚本执行器
		 *  @param pUIScriptManager 脚本执行器指针
		 */	
		virtual void SetScriptManager(IUIScriptManager* pUIScriptManager) = 0;		
	};
}


#endif