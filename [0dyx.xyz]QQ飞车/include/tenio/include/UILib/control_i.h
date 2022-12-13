#ifndef __CONTROL_I_H__
#define __CONTROL_I_H__

#include "../teniointerface.h"
#include "../june/canvas_i.h"
#include "variableparam_i.h"

namespace Tenio
{
	class IControl;

	/**
	 * @ingroup tenuilib
	 * @brief 控件基类
	 *
	 * <p>
	 *　　 所有控件的基类，给出控件的基本成员函数。
	 * </p>
	 *
	 * @sa 
	 */	
	class IControl : public IComponent
	{
	public:
		/**
		 *  @brief 创建控件。此函数由uilib调用，不可以由用户直接调用。
		 *  @param lpszControlName　用户为控件取的名称，必须在一个 IUILib 对象管理内部是唯一的
		 *  @param pParent 指定父控件
		 *  @return 创建成功则返回TRUE，失败则返回FALSE
		 *  @sa IUILib::CreateControl()
		 */	
		virtual BOOL Create(LPCTSTR lpszControlName, IControl* pParent) = 0;

		/**
		 *  @brief 设置控件的父控件
		 *  @param pControl 指定父控件
		 *  @sa 
		 */
		virtual void SetParent(IControl* pControl) = 0;

		/**
		 *  @brief 设置控件属性
		 *  @param pParam 属性对象，保存控件的一个或多个属性值
		 *  @sa 
		 */	
		virtual void SetProperty(IVariableParam* pParam) = 0;

		/**
		 *  @brief 获得得父控件指针
		 *  @return 父控件指针，如果没有父控件，则返回NULL
		 *  @sa 
		 */	
		virtual IControl* GetParent() = 0;

		/**
		 *  @brief 获得控件ID
		 *  @return 返回控件ID，无效ID值为 INVALID_CONTROLID
		 *  @sa 
		 */
		virtual DWORD GetControlID() = 0;

		/**
		 *  @brief 获得控件的矩形大小
		 *  @param bScreen 是否返回屏幕坐标的矩形
		 *  @return 如果bScreen为TRUE，则返回屏幕坐标矩形；如果bScreen为FALSE，则返回相对于父控件坐标的矩形
		 *  @sa 
		 */
		virtual RECT GetRect(BOOL bScreen) = 0;

		/**
		 *  @brief 控件是否可见
		 *  @return 控件可见则返回TRUE，不可见则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL IsVisible() = 0;

		/**
		 *  @brief 控件是否可用
		 *  @return 控件可用则返回TRUE，不可见则返回TRUE
		 *  @sa 
		 */	
		virtual BOOL IsDisable() = 0;

		/**
		 *  @brief 绘制控件到指定的Canvas上
		 *  @param pCanvas 指定画布对象的指针
		 *  @param x　画布上绘制的起始x轴坐标
		 *  @param y  画布上绘制的起始y轴坐标
		 *  @return 
		 *  @sa 
		 */	
		virtual void Draw(IJuneCanvas* pCanvas, int x, int y) = 0;

		/**
		 *  @brief 改变控件矩形大小
		 *  @param rc　改变后的相对于父控件坐标的控件矩形
		 *  @sa 
		 */	
		virtual void Move(RECT& rc) = 0;

		/**
		 *  @brief 显示或隐藏控件
		 *  @param bShow TRUE为显示，FALSE为隐藏
		 *  @sa 
		 */	
		virtual void Show(BOOL bShow) = 0;

		/**
		 *  @brief 使控件可用或不可用
		 *  @param bEnable 是否可用
		 *  @sa 
		 */	
		virtual void Enable(BOOL bEnable) = 0;
		
		/**
		 *  @brief 刷新控件
		 *  @param rc 控件的刷新区域，为NULL时为全部刷新
		 *  @sa 
		 */
		virtual void ReDraw(RECT* rc) = 0;

		/**
		 *  @brief 设置tip字符串
		 *  @param lpszText 提示内容
		 *  @sa ITToolTip 
		 */	
		virtual void SetToolTipText(LPCTSTR lpszText) = 0;

		/**
		 *  @brief 获得焦点
		 *  @sa 
		 */	
		virtual void SetFocus() = 0;

		/**
		 *  @brief 获得鼠标控制
		 *  @sa 
		 */
		virtual void SetCapture() = 0;

		/**
		 *  @brief 释放鼠标控制
		 *  @sa 
		 */	
		virtual void ReleaseCapture() = 0;

		/**
		 *  @brief 锁定更新
		 *  @sa 
		 */	
		virtual void LockUpdate() = 0;

		/**
		 *  @brief 解锁更新，解锁时会刷新脏区域
		 *  @sa 
		 */		
		virtual void UnLockUpdate() = 0;

		/**
		 *  @brief 使控件显示在最前面
		 *  @sa 
		 */	
		virtual void BringToTop() = 0;

		/**
		 *  @brief 判断是否为自己的子孙控件
		 *  @param dwControlID 被判断的控件ID
		 *  @return 如果是返回TRUE，不是则返回FALSE
		 */		
		virtual BOOL IsMyOffspring(DWORD dwControlID) = 0;

		/**
		 *  @brief 显示tip(控件必须已设置tip)
		 */	
		virtual void ShowToolTip() = 0;
	};
}
#endif