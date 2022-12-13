#ifndef __TLEVEL_I_H__
#define __TLEVEL_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITLevel;

	TENIO_MAKE_TYPEID_MAP(ITLevel, COMPONENTID_TLEVEL);

	 /**
	  * @ingroup tenuilib
	  * @brief uilib Level控件
	  *
	  * <p>　 
	  *	类似微软进度条的控件，可用于显示长事务的进度，也可用于显示用户当前的等级。等级由左至右递增，    
	  * 可自定义图片，图片后可跟文字，通过开关可控制文字的显示与否</p>
	  * <p>
	  *　　 属性列表:<br>
	  * \n
	  * 公共属性：
	  * <table>
	  * <tr>
	  * <td class="indexkey"> 属性ID </td>
	  * <td class="indexkey"> 类型 </td>
	  * <td class="indexkey"> 描述 </td>
	  * </tr>
	  * <tr><td>见 IUIContainer </td><td>  </td><td>  </td></tr>
	  * </table> 
	  * \n
	  * 私有属性：
	  * <table>
	  * <tr>
	  * <td class="indexkey"> 属性ID </td>
	  * <td class="indexkey"> 类型 </td>
	  * <td class="indexkey"> 描述 </td>
	  * </tr>
	  * <tr><td> PROPERTYID_TEXTCOLOR </td><td> #EVT_DWORD </td><td> 文字颜色 </td></tr>
	  * <tr><td> PROPERTYID_CAPTION </td><td> #EVT_STRING </td><td> 文字内容 </td></tr>
	  * <tr><td> PROPERTYID_LOGFONT </td><td> #EVT_FONTSET </td><td> 文字字体 </td></tr>
	  * <tr><td> PROPERTYID_LEVELTOPIMAGE </td><td> #EVT_STRING </td><td> level上层贴图 </td></tr>
	  * <tr><td> PROPERTYID_LEVELBOTTOMIMAGE </td><td> #EVT_STRING </td><td> level底层贴图 </td></tr>
	  * <tr><td> PROPERTYID_LEVELHEIGHT </td><td> #EVT_INT </td><td> level高 </td></tr>
	  * <tr><td> PROPERTYID_LEVELWIDTH </td><td> #EVT_INT </td><td> level宽 </td></tr>
	  * <tr><td> PROPERTYID_TEXTVISIBLE </td><td> #EVT_BOOL </td><td> 是否显示文字 </td></tr>
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
	  * <tr><td> 无 </td><td>  </td><td>  </td></tr>
	  * </table>  
	  * </p>
	  *
	  * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	  */	 
	class ITLevel : public IControl
	{
	public:
		/**
		 * @brief 设置Level范围
		 * @param nLowerRange Level范围左边界
		 * @param nUpperRange Level范围右边界
		 * @param bRedraw 是否重绘Level
		 */
		virtual void SetRange(int nLowerRange, int nUpperRange, BOOL bRedraw) = 0;
		/**
		 * @brief 获取Level范围
		 * @param nLowerRange Level范围左边界
		 * @param nUpperRange Level范围右边界
		 */
		virtual void GetRange(int& nLowerRange, int& nUpperRange) = 0;
		/**
		 * @brief 设置Level当前位置
		 * @param nPos Level当前位置
		 * @param bRedraw 是否重绘Level
		 * @return 如果设置成功，返回TRUE，否则返回FALSE
		 */
		virtual BOOL SetPos(int nPos, BOOL bRedraw) = 0;
		/**
		 * @brief 获取Level当前位置
		 * @return 返回Level当前位置
		 */
		virtual int GetPos() = 0;
		/**
		 * @brief 为Level当前位置增加一个偏移量，移动到偏移后的位置
		 * @param nPos 增加的偏移量
		 * @param bRedraw 是否重绘Level
		 * @return 返回Level改变前的位置
		 */
		virtual int OffsetPos(int nPos, BOOL bRedraw) = 0;
		/**
		 * @brief Level文字显示开关
		 * @param bIsVisible 文字是否可见
		 */
		virtual void SetTextVisible(BOOL bIsVisible) = 0;
		/**
		 * @brief 获取Level文字是否可见
		 * @return 返回文字是否可见
		 */
		virtual BOOL GetTextVisible() = 0;
		/**
		 * @brief 设置Level文字颜色
		 * @param dwColor 文字颜色
		 */
		virtual void SetTextColor(COLORREF dwColor) = 0;
//		virtual COLORREF GetTextColor() = 0;
		/**
		 * @brief 设置Level文字内容
		 * @param lpTextCaption 文字内容
		 */
		virtual void SetTextCaption(LPCTSTR lpTextCaption) = 0;
		/**
		 * @brief 获取Level文字内容
		 * @return 返回文字内容
		 */
		virtual LPCTSTR GetTextCaption() = 0;
		/**
		 * @brief 设置Level步进值， StepIt()将按照此步进值增加Level的偏移量
		 * @param nStep 步进值
		 * @return 返回Level改变前的步进值
		 */
		virtual int SetStep(int nStep) = 0;
		/**
		 * @brief 步进Level
		 * @param bRedraw 是否重绘Level
		 * @return 返回Level改变前的位置
		 */
		virtual int StepIt(BOOL bRedraw) = 0;
	};
};

#endif

// InterfaceName:TLevel
// SubFolderName:uilib
