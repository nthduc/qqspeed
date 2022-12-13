#ifndef __TBUTTON_I_H__
#define __TBUTTON_I_H__

#include "control_i.h"

namespace Tenio
{
	class ITButton;
	TENIO_MAKE_TYPEID_MAP(ITButton, COMPONENTID_TBUTTON)
	//////////////////////////////////////////////////////////////////////////
	/**
	 * @ingroup tenuilib
	 * @brief 按钮
	 *
	 * <p>　 
	 *		按钮控件，分为两种按钮：普通按钮和不可恢复按钮(按下可不可恢复)
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
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * </table> 
	 * \n
	 * 私有属性：
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 属性ID </td>
	 * <td class="indexkey"> 类型 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TIP     </td><td> 见 IUIContainer </td><td> </td></tr>
	 * <tr><td> PROPERTYID_BACKGROUND  </td><td> 见 IUIContainer </td><td> </td></tr>

	 * <tr><td> PROPERTYID_BUTTONTYPE </td><td> string </td><td> 两种取值："NormalButton"(普通按钮),"UnreversedButton"(不可恢复按钮) </td></tr>
	 * <tr><td> PROPERTYID_SIGNALED   </td><td> int </td><td> 非零表示设置为Signaled状态 </td></tr>
	 * <tr><td> PROPERTYID_BG_HOVER 　　　</td><td> imageset </td><td> 悬停状态的背景, 分解方式见 IUIContainer ，后面加后缀_HOVER </td></tr>
	 * <tr><td> PROPERTYID_BG_PRESSED 　　</td><td> imageset </td><td> 按下状态的背景, 分解方式见 IUIContainer ，后面加后缀_PRESSED </td></tr>
	 * <tr><td> PROPERTYID_BG_DISABLE 　　</td><td> imageset </td><td> 不可用状态下的背景, 分解方式见 IUIContainer ，后面加后缀_DISABLE </td></tr>
	 * <tr><td> PROPERTYID_TEXTFONT_NORMAL   </td><td> string </td><td> 正常情况下字体, 字符串格式见 EVT_FONTSET ，在程序中设置时需分解</td></tr>
	 * <tr><td> PROPERTYID_TEXTFONT_HOVER    </td><td> string </td><td> 悬停状态下的字体 </td></tr>
	 * <tr><td> PROPERTYID_TEXTFONT_PRESSED  </td><td> string </td><td> 按下状态下的字体 </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR         </td><td> DWORD </td><td> 正常情况下的文字颜色 </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR_HOVER   </td><td> DWORD </td><td> 悬停情况下的文字颜色 </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR_PRESSED </td><td> DWORD </td><td> 按下状态下的文字颜色 </td></tr>
	 * <tr><td> PROPERTYID_DEFAULTBUTTON </td><td> bool </td><td> 是否是缺省执行按钮 </td></tr>
	 * <tr><td> PROPERTYID_ITEMMARGIN    </td><td> int </td><td> 文字与边框间距 </td></tr>
	 * <tr><td> PROPERTYID_AUTOSIZE_IMAGE</td><td> bool </td><td> 是否自适应图片大小，只在PROPERTYID_BGMODE为 EBGM_ONEIMAGE 时有效</td></tr>
	 * <tr><td> PROPERTYID_AUTOSIZE </td><td> EVT_BOOL </td><td> 是否按文字自适应图片大小 </td></tr>
	 * <tr><td> PROPERTYID_TABSTOP </td><td> bool </td><td> 是否响应Tab键切换焦点 </td></tr>
	 * <tr><td> PROPERTYID_CAPTION </td><td> string </td><td> 文字内容 </td></tr>
	 * <tr><td> PROPERTYID_SOUND_CLICK </td><td> string </td><td> 点击时声音文件路径 </td></tr>
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
	 * <tr><td> EI_BUTTON_CLICK </td><td> EPARAMID_BUTTON_CLICK_X	int\n
										EPARAMID_BUTTON_CLICK_Y		int
										</td><td> 点击按钮消息 </td></tr>
	 * <tr><td> EI_SIGNALED </td><td> 无 </td><td> 获得signal状态 </td></tr>
	 * <tr><td> EI_UNSIGNALED </td><td> 无 </td><td> 取消signal状态 </td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITButton : public IControl
	{
	public:
	};
}

#endif