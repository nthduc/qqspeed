#ifndef __TTOOLTIP_I_H__
#define __TTOOLTIP_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITToolTip;

	TENIO_MAKE_TYPEID_MAP(ITToolTip, COMPONENTID_TTOOLTIP);
	/**
	 * @ingroup tenuilib
	 * @brief 
	 *
	 * <p>　 
	 *     用于根据鼠标消息动态地显示其它控件的tip信息，<br>
	 *     或静态地显示某个tip信息, 控件的大小可动态随显示字符串的长度变化。
	 * </p>
	 * <p>　 
	 *     如何使用顶层窗口体系内全局唯一的tip对象方法请参见 IUILib::SetGlobalTipControlName()
	 * </p>
	 * <p>
	 *　　 属性列表<br>
	 * \n
	 * 公共属性：
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 属性ID </td>
	 * <td class="indexkey"> 类型 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> 见 IUIContainer  </td><td>  </td><td>  </td></tr>
	 * </table> 
	 * \n
	 * 私有属性：
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 属性ID </td>
	 * <td class="indexkey"> 类型 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> PROPERTYID_BACKGROUND  </td><td> 见 IUIContainer </td><td> </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR   </td><td> #EVT_DWORD </td><td> 文字颜色 </td></tr>
	 * <tr><td> PROPERTYID_CAPTION </td><td> #EVT_STRING </td><td> 文字内容 </td></tr>
	 * <tr><td> PROPERTYID_LOGFONT </td><td> EVT_FONTSET </td><td> 字体属性，此属性需要分解，见 ITStatic </td></tr>
	 * <tr><td> PROPERTYID_AUTOCHANGELINE </td><td> EVT_BOOL </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_MINWIDTH </td><td> #EVT_INT </td><td> 矩形最小宽度 </td></tr>
	 * <tr><td> PROPERTYID_MINHEIGHT </td><td> #EVT_INT </td><td> 矩形最小高度 </td></tr>
	 * <tr><td> PROPERTYID_MULTILINE </td><td> #EVT_BOOL </td><td> 是否支持以'\n'转义符换行 </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * </table> 
	 * </p>
	 * <p>
	 *　　 事件列表:<br>
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 事件ID </td>
	 * <td class="indexkey"> 参数 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> 公用消息见 IUIContainer  </td><td> </td><td></td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * </table>  
	 * </p>
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */
	
	class ITToolTip : public IControl
	{
	public:
		/**
		 *  @brief 为控件增加tip控制。
		 *  @param dwID 增加控件的ID，如果此ID已被加入过，则只改变它的文字内容
		 *  @param lpszText 控件的文字内容
		 *  @sa 
		 */
		virtual void AddTool(DWORD dwID, LPCTSTR lpszText) = 0;

		/**
		 *  @brief 移除一个控件的tip控制
		 *  @param dwID 被移除控件的ID
		 *  @sa 
		 */		
		virtual void RemoveTool(DWORD dwID) = 0;

		/**
		 *  @brief 移除所有的tip控制
		 *  @sa 
		 */
		virtual void RemoveAllTools() = 0;

		/**
		 *  @brief 对鼠标事件的处理
		 *  @param dwID 发出鼠标事件的控件ID
		 *  @param pParam 事件参数
		 *  @sa 
		 */
		virtual void RelayEvent(DWORD dwID, IVariableParam* pParam) = 0;

		/**
		 *  @brief 设置当前显示的tip内容
		 *  @param lpszText 需要显示的Tip内容
		 *  @sa 
		 */
		virtual void SetText(LPCTSTR lpszText) = 0;

		/**
		 *  @brief 设置显示延时
		 *  @param nDelayTime 显示延时，即执行 Show() 到它真正显示出来之间的时间
		 *  @sa 
		 */
		virtual void SetShowDelay(int nDelayTime) = 0;

		/**
		 *  @brief 设置静态显示时显示的时长
		 *  @param nShowTime 显示时长。当时长为0时，表示无限长，即不会自动隐藏
		 *  @sa 
		 */
		virtual void SetShowTime(int nShowTime) = 0;

		/**
		 *  @brief 淡出
		 *  @param nTime 淡出过程的总时长
		 *  @sa 
		 */
		virtual void FadeIn(int nTime) = 0;

		/**
		 *  @brief 淡入
		 *  @param nTime 淡入过程的总时长
		 *  @sa 
		 */	
		virtual void FadeOut(int nTime) = 0;
	};
};

#endif

// InterfaceName:TToolTip
// SubFolderName:uilib
