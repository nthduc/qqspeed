#ifndef __TCHECKBOX_I_H__
#define __TCHECKBOX_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITCheckBox;

	TENIO_MAKE_TYPEID_MAP(ITCheckBox, COMPONENTID_TCHECKBOX);

	/**
	 * @ingroup tenuilib
	 * @brief uilib checkbox控件 
	 *
	 * <p>　 
	 * 功能与微软checkbox控件相似,可设置普通、浮上、按下和无效时的图片</p>
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
	 * <tr><td> PROPERTYID_IMAGE </td><td> #EVT_STRING </td><td> checkbox普通状态的底图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_HOVER </td><td> #EVT_STRING </td><td> checkbox浮上状态的底图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_PRESSED </td><td> #EVT_STRING </td><td> checkbox按下状态的底图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_DISABLE </td><td> #EVT_STRING </td><td> checkbox无效状态的底图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_CHECKED </td><td> #EVT_STRING </td><td> checkbox普通状态的选中图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_HOVERCHECKED </td><td> #EVT_STRING </td><td> checkbox浮上状态的选中图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_PRESSEDCHECKED </td><td> #EVT_STRING </td><td> checkbox按下状态的选中图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_DISABLECHECKED </td><td> #EVT_STRING </td><td> checkbox无效状态的选中图 </td></tr>
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
	 * <tr><td> EI_SIGNALED </td><td> 无 </td><td> 勾选checkbox时触发该事件 </td></tr>
	 * <tr><td> EI_UNSIGNALED </td><td> 无 </td><td> 取消选择checkbox时触发该事件 </td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITCheckBox : public IControl
	{
	public:
		/**
		 * @brief 获取checkbox状态
		 * @return checkbox被选中返回TRUE，否则返回FALSE
		 */
		virtual BOOL IsChecked() = 0;
		/**
		 * @brief 设置checkbox状态
		 * @param bIsChecked checkbox状态
		 */
		virtual void SetChecked(BOOL bIsChecked) = 0;
	};
};

#endif

// InterfaceName:TCheckBox
// SubFolderName:UILib
