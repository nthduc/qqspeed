/********************************************************************
　created         : 2006/6/1
　filename        : uilib_i.h
　author          : season
　version         : 1.0
　description     : UI库对外接口，由UI库开发人员实现
　history         :
　1 day	          : 2006/6/1
　　author        : season
　　modification: : created	
*********************************************************************/

#ifndef __UILIB_I_H__
#define __UILIB_I_H__

#include "../teniointerface.h"
#include "../june/canvas_i.h"
#include "../june/canvasupdate_i.h"
#include "variableparam_i.h"
#include "uiconfig_i.h"
#include "uiscriptmanager_i.h"
#include "uiclock_i.h"
#include "../layer/scene_i.h"
#include "../layer/sceneupdate_i.h"

namespace Tenio
{
	class IUILib;
	class IUILibEvent;

	TENIO_MAKE_TYPEID_MAP(IUILib, COMPONENTID_UILIB);

	//////////////////////////////////////////////////////////////////////////
	/**
	 * @ingroup tenuilib
	 * @brief UI库对外接口
	 *
	 * <p>
	 *　　 本类是TenioUILib对外的主接口。功能如下:
	 * <ol>
	 *   <li>读取XML文件恢复UI布局</li>
	 *   <li>创建或销毁控件，设置控件属性，绘制控件</li>
	 *   <li>获得控件的指针和ID</li>
	 * </ol>
	 * </p>
	 * <p>
	 *	每一个UILib对象对应一个顶层窗口(topwindow)体系。每个顶层窗口体系中控件的名称和ID是唯一的。<br>
	 *	用户使用UI库时，必须首先创建一个IUILib对象，<br>
	 *  用户可以使用UI编辑器编辑UI布局文件，也可以使用 IUILib 对象创建单个控件。<br>	
	 *  每个控件的创建和销毁必须通过调用UILib的接口执行。<br>
	 * </p>
	 *
	 * @sa 
	 */
	class IUILib : public IComponent
	{
	public:

		/**
		 *  @brief 恢复一个完整的TopWindow布局
		 *  @param lpszXMLFile	描述布局信息的xml文件名，可以是完整路径名；若不是，则默认是相对于可执行目录下config/layout目录
		 *  @return 成功则返回TRUE，失败返回FALSE
		 *  @sa 　RestoreFromXMLNode() 
		 */		
		virtual BOOL RestoreFromXMLFile(LPCTSTR lpszXMLFile) = 0;

		/**
		 *  @brief 从xml文件中的某个结点恢复子控件布局
		 *  @param dwParentID 该结点控件的父控件ID，父控件必须已经由同一个uilib对象创建
		 *  @param lpszXMLPath 被恢复的结点在xml文件中的路径。路径格式:xxx.xml/node1/node2/../nodename
		 *  @return 成功则返回TRUE，失败返回FALSE
		 *  @sa RestoreFromXMLFile() 
		 */
		virtual BOOL RestoreFromXMLNode(DWORD dwParentID, LPCTSTR lpszXMLPath) = 0;

		/**
		 *  @brief 使用xml文件创建多个control， 内部多个control的名字是组合而成 parentname_childname
		 *  @param dwParentID 父控件ID
		 *  @param lpszXMLFileName xml文件
		 *  @return 若创建成功，返回TRUE；反之为FALSE
		 *  @sa CreateControl()
		 */
		virtual BOOL CreateCompositeControlFromXML(DWORD dwParentID, LPCTSTR lpszXMLFileName) = 0;

		/**
		 *  @brief 使用xml文件中的properties结点创建控件
		 *  @param lpszXMLPath 控件结点在xml文件中的路径。路径格式:xxx.xml/node1/node2/../nodename
		 *  @param pParam 接受设置的控件属性参数
		 *  @return 若加载成功，返回TRUE；反之为FALSE
		 *  @sa CreateControl()
		 */
		virtual BOOL LoadControlProperties(
			LPCTSTR lpszXMLPath, 
			IVariableParam* pParam) = 0;

		/**
		 *  @brief 设置此UILib对应topwindow的父窗口句柄
		 *  @param 　pParent 父窗口句柄
		 */	
		virtual void SetRootParentHandle(HWND hWnd) = 0;

		/**
		 *  @brief 清空该uilib创建的所有控件
		 *  @return 
		 *  @sa CreateControl() , DeleteControl() , 
		 */		
		virtual void ClearAll() = 0;

		/**
		 *  @brief 创建一个控件
		 *  @param pParam 创建该控件所用到的参数和属性集
		 *  @param lpszName 由用户指定的控件名称，同父的子控件名不可重复。\n
		 *					如果为NULL，或长度为零，则由系统自动生成一个控件名。
		 *	@param dwParentID 指定该控件的父控件ID，可以由 IControl::GetControlID() , GetControlID() 等函数获得
		 *  @return 若创建成功，返回控件的ID；失败则返回　INVALID_CONTROLID
		 *  @sa DeleteControl() 
		 */
		virtual DWORD CreateControl(
			IVariableParam* pParam, 
			LPCTSTR lpszName, 
			DWORD dwParentID) = 0;

		/**
		 *  @brief 删除一个控件，包括它的所有子控件
		 *  @param dwID 需要删除控件的ID
		 *  @return 删除成功则返回TRUE，失败返回FALSE
		 *  @sa CreateControl() 
		 */	
		virtual BOOL  DeleteControl(DWORD dwID) = 0;

		/**
		 *  @brief 设置控件属性
		 *  @param dwID 控件ID
		 *  @param pParam 控件属性集。详情参见各控件的属性列表
		 *  @return 设置成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */
		virtual BOOL SetProperty(DWORD dwID, IVariableParam* pParam) = 0;

		/**
		 *  @brief 将控件绘制在指定的Canvas上
		 *  @param dwID 控件ID
		 *  @param pCanvas Canvas指针
		 *  @param x 在Canvas上的x坐标
		 *  @param y 在Canvas上的y坐标
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */
		virtual BOOL DrawControl(DWORD dwID, IJuneCanvas* pCanvas, int x, int y) = 0;
		
		/**
		 *  @brief 获得控件名
		 *  @param dwControl 控件ID
		 *  @param lpszName 存放控件名的字符串
		 *  @param nMaxLen 字符串最大长度
		 *  @return 成功获得取则返回TRUE，失败则返回FALSE
		 */	
		virtual BOOL GetControlName(DWORD dwControl, LPTSTR lpszName, UINT nMaxLen) = 0;

		/**
		 *  @brief 获得控件全路径名
		 *  @param dwControl 控件ID
		 *  @param lpszPath 存放控件路径名的字符串
		 *  @param nMaxLen 字符串最大长度
		 *  @return 成功获得取则返回TRUE，失败则返回FALSE
		 */	
		virtual BOOL GetControlFullPath(DWORD dwControl, LPTSTR lpszPath, UINT nMaxLen) = 0;
		
		/**
		 *  @brief 给控件重新命名
		 *  @param dwControl 控件ID
		 *  @param lpszNewName 控件新的名称
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 */
		virtual BOOL RenameControl(DWORD dwControl, LPCTSTR lpszNewName) = 0;

		/**
		 *  @brief 由控件名获得控件ID
		 *  @param lpszName 控件名称
		 *  @return 返回名为lpszName的第一个控件ID，如果控件不存在，则返回 INVALID_CONTROLID
		 *  @sa 
		 */
		virtual DWORD GetControlID(LPCTSTR lpszName) = 0;

		/**
		 *  @brief 根据控件路径名查找控件ID
		 *  @param lpszPath 控件路径名，相对于ID为dwParent的控件。\n
		 *			例如：		root
		 *						/  \			\n
							   a	f			\n
							  /\				\n
							 b	c				\n
								/\				\n
							   d  e				\n
					控件d　的全路径是："root.a.c.d";		\n
					控件d　相对于　控件a　的路径是："c.d";	\n
					
		 *  @param dwRelativeControl 路径名相对的控件ID。\n
					当此值为INVALID_CONTROLID时，根据控件全路径名查找。
		 *  @return 返回查找到的控件ID，失败则返回INVALID_CONTROLID
		 */		
		 virtual DWORD GetControlIDbyPath(LPCTSTR lpszPath, DWORD dwRelativeControl) = 0;		

		/**
		 *  @brief 由控件ID获得控件对象的指针
		 *  @param dwID 控件ID
		 *  @return 返回控件对象指针，若控件不存在则返回NULL
		 *  @sa GetControlPtr() , IControl 
		 */
		virtual LPVOID GetControlPtr(DWORD dwID) = 0;

		/**
		 *  @brief 获得顶层控件对象指针
		 *  @return 返回顶层控件对象指针，若不存在则返回FALSE
		 *  @sa 
		 */
		virtual LPVOID GetRootControlPtr() = 0;

		/**
		 *  @brief 设置焦点控件
		 *  @param dwID 指定获得焦点的控件，如果值为 INVALID_CONTROLID ，则仅使当前焦点控件失去焦点
		 *  @return 
		 *  @sa 
		 */
		virtual void SetFocusControl(DWORD dwID) = 0;

		/**
		 *  @brief 获得焦点控件
		 *  @return 获得获得焦点的控件ID，如果值为 INVALID_CONTROLID ，则无
		 *  @sa 
		 */
		virtual DWORD GetFocusControl() = 0;

		/**
		 *  @brief 注册控件事件句柄
		 *  @param dwID 指定发送事件的控件ID
		 *  @param pEvent 事件接收者对象
		 *  @return 
		 *  @sa 使用详情请参考 IUILibEvent 
		 */		
		virtual void RegisterEventHandle(DWORD dwID, IUILibEvent* pEvent) = 0;

		/**
		 *  @brief 设置全局tip对象，该对象负责响应鼠标事件时显示控件tip属性标识的字符串
		 *  @param dwTipControlID tip控件ID。此控件对象必须以顶层控件为父控件。
		 */
		virtual void SetGlobalTipControl(DWORD dwTipControlID) = 0;

		/**
		 *  @brief 获得某些特殊变量
		 *  @param pParam 用于存放获得变量的对象指针
		 *  @param dwParamID 希望获得变量的ID，\n
							 目前支持 PARAMID_TOPWINDOWHANDLE ,即顶层窗口句柄
		 */		
		virtual void GetParam(IVariableParam* pParam, DWORD dwParamID) = 0;

		/**
		 *  @brief 获得UILib的config指针，singleton
		 *  @return 返回Config指针
		 *  @sa 
		 */
		virtual IUIConfig* GetConfig() = 0;

		/**
		 *  @brief 设置脚本处理器指针，用于脚本支持
		 *  @return  如果成功则返回IUIScriptManager指针，否则返回NULL
		 *  @sa 
		 */
		virtual void SetScriptManager(IUIScriptManager* pUIScriptManager) = 0;

		/**
		 *  @brief 获得当前使用的脚本处理器
		 *  @return 返回脚本处理器指针，可以是NULL
		 */
		virtual IUIScriptManager* GetScriptManager() = 0;
		
		/**
		 *  @brief 获得得uilib的统一时钟指针
		 *  @return 返回IUIClock指针。
		 */	
		virtual IUIClock* GetUIClock() = 0;

		/**
		 *  @brief 顶层控件无窗口时的消息入口
		 *  @param pParam 窗口消息参数
		 *  @return 阻止消息继续传播返回TRUE,否则返回FALSE
		 */		
		virtual BOOL OnOutWindowMessage(IVariableParam* pParam) = 0;

		/**
		 *  @brief 设置使用外部窗口作画布时的缓存画布更新回调函数
		 *	@param pScene
		 *  @param pSceneUpdate 可以为NULL
		 *  @param pCanvasUpdate 画布更新回调
		 */		
		virtual void SetRootScene(
			IJuneScene* pScene,
			IJuneSceneUpdate* pSceneUpdate) = 0;

		/**
		 *  @brief 获得顶层Scene,　设置使用外部窗口作画布时使用
		 *  @param ppScene 保存Scene的指针地址　(out)
		 *  @param ppSceneUpdate 保存SceneUpdate指针的地址
		 */
		virtual void GetRootScene(IJuneScene** ppScene, IJuneSceneUpdate** ppSceneUpdate) = 0;
		
		/**
		 *  @brief 手动触发UI事件
		 *  @param dwID, 发出事件的控件ID
		 *  @param pParam 事件参数
		 */
		virtual void SendUIEvent(DWORD dwID, IVariableParam* pParam) = 0;
	};

	/**
	 * @ingroup tenuilib
	 * @brief UILib事件接口
	 *
	 * <p>
	 *　　 当事件接收者需要接收控件事件的时候，需要继承并实现 IUILibEvent 接口。<br>
	 *     并通过 IUILib::RegisterEventHandle() 将自己注册到UILib中。如果子控件没有<br>
	 *	注册事件接收者，则它产生的事件向上传递，发送给父控件的事件接收者。依此类推。<br>
	 * </p>
	 *
	 * @sa IUILib 
	 */
	class IUILibEvent : public IEvent
	{
	public:
		/**
		 *  @brief 控件事件通知
		 *  @param dwID 发送事件的控件的ID
		 *  @param pParem 消息参数。它由以下参数组成：<br>
		 *				EPARAMID_EVENTID		事件ID<br>
		 *				EPARAMID_CONTROLNAME	发送事件的控件名<br>
		 *				EPARAMID_xxxxxx			若干事件参数，由事件类型决定<br>
		 *  @return 
		 *  @sa IVariableParam 
		 */	
		virtual void OnControlMessage(DWORD dwID, IVariableParam* pParam) = 0;
	};
}		
#endif