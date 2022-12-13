#ifndef __TDIALOG_I_H__
#define __TDIALOG_I_H__

#include "control_i.h"

namespace Tenio
{
	class ITDialog;
	TENIO_MAKE_TYPEID_MAP(ITDialog, COMPONENTID_TDIALOG);

	/**
	 * @ingroup tenuilib
	 * @brief 对话框
	 *
	 * <p>　 
	 *	　弹出式对话框。	
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
	 * <tr><td> 见 IUIContainer </td><td>  </td><td>  </td></tr>
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
	 * <tr><td> PROPERTYID_SIZEBOX </td><td> #EVT_BOOL </td><td> 是不可拉动边框改变大小 </td></tr>
	 * <tr><td> PROPERTYID_ALWAYSONTOP </td><td> #EVT_BOOL </td><td> 是否总在最前面 </td></tr>
	 * <tr><td> PROPERTYID_TOOLWINDOW </td><td> #EVT_BOOL </td><td> 是否在任务栏上没有标题栏 </td></tr>
	 * <tr><td> PROPERTYID_CAPTIONHEIGHT </td><td> #EVT_INT </td><td> 标题栏区域高度(在此区域中鼠标按下可以拖动对话框) </td></tr>
	 * <tr><td> PROPERTYID_MINWIDTH </td><td> #EVT_INT </td><td> 矩形最小宽度 </td></tr>
	 * <tr><td> PROPERTYID_MINHEIGHT </td><td> #EVT_INT </td><td> 矩形最小高度 </td></tr>
	 * <tr><td> PROPERTYID_MAXWIDTH </td><td> #EVT_INT </td><td> 矩形最大宽度 </td></tr>
	 * <tr><td> PROPERTYID_MAXHEIGHT </td><td> #EVT_INT </td><td> 矩形最大高度 </td></tr>
	 * <tr><td> PROPERTYID_REGIONSET </td><td> #EVT_STRING </td><td> 对话框形状设置，目前仅支持按Mask颜色值设置形状，\n
																	格式为: FROMBGIMAGES:0x0000ff 冒号后面跟十六进制表示的mask颜色值。\n
																	如果不需要设置形状，则字符串为 NONE: </td></tr>
	 * <tr><td> PROPERTYID_ICON </td><td> #EVT_STRING </td><td> 对话框在任务栏上显示的图标文件路径 </td></tr>
	 * <tr><td> PROPERTYID_CAPTION </td><td> #EVT_STRING </td><td> 对话框在任务栏上显示的标题文字 </td></tr>
	 * <tr><td> PROPERTYID_SOUND_OPEN </td><td> #EVT_STRING </td><td> 对话框弹出时播放的声音文件路径 </td></tr>
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
	 * <tr><td> 无 </td><td>  </td><td>  </td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITDialog : public IControl
	{
	public:
		/**
		 *  @brief 将对话框移至屏幕中间
		 *  @sa 
		 */		
		virtual void MoveToCenter() = 0;
		
		/**
		 *  @brief 最小化对话框
		 *  @sa 
		 */		
		virtual void Minimize() = 0;

		/**
		 *  @brief 最大化对话框
		 *  @sa 
		 */	
		virtual void Maximize() = 0;

		/**
		 *  @brief 将对话框恢复正常大小
		 *  @sa 
		 */		
		virtual void Restore() = 0;

		/**
		 *  @brief 以阻塞方式弹出对话框（尚未实现）
		 *	@return 返回关闭对话框前激活控件的ID（一般是按钮）
		 *  @sa 
		 */	
		virtual DWORD DoModal() = 0;

		/**
		 *  @brief 结束阻塞式对话框
		 *  @param dwInvokeControl 调用此函数的控件ID（一般是对话框上的按钮）
		 */
		virtual void EndModal(DWORD dwInvokeControl) = 0;
		
	};
}

#endif