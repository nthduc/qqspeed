#ifndef __CONTAINER_I_H__
#define __CONTAINER_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class IUIContainer;

	TENIO_MAKE_TYPEID_MAP(IUIContainer, COMPONENTID_UICONTAINER);

	/**
	 * @ingroup tenuilib
	 * @brief 容器控件
	 *
	 * <p>　 
	 *		容器，可以设置背景图片或背景色。没有其它任何UI逻辑。是一个轻量的控件，<br>
	 *	可以用来包含一组相关的子控件。当createtype属性为"ignore"时创建的是无窗口类型。
	 * </p>
	 * <p>
	 *　　 属性列表:\n
	 *\n
	 * 公共属性：
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 属性ID </td>
	 * <td class="indexkey"> 类型 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TYPEID </td><td> int </td><td> 控件类型ID，只在创建时有效 </td></tr>
	 * <tr><td> PROPERTYID_XOFFSET </td><td> int </td><td> 控件左上角相对于父窗口左上角的x坐标 </td></tr>
	 * <tr><td> PROPERTYID_YOFFSET </td><td> int </td><td> 控件左上角相对于父窗口左上角的y坐标 </td></tr>
	 * <tr><td> PROPERTYID_WIDTH   </td><td> int </td><td> 控件矩形宽度 </td></tr>
	 * <tr><td> PROPERTYID_HEIGHT  </td><td> int </td><td> 控件矩形高度 </td></tr>
	 * <tr><td> PROPERTYID_VISIBLE </td><td> bool </td><td> 是否可见 </td></tr>
	 * <tr><td> PROPERTYID_DISABLE </td><td> bool </td><td> 是否可用 </td></tr>
	 * <tr><td> PROPERTYID_TRANSPARENT  </td><td> bool </td><td> 是否对消息透明 </td></tr>
	 * <tr><td> PROPERTYID_ALPHA   </td><td> byte </td><td> 控件绘制透明度，取值在0 ~255之间 </td></tr>
	 * </table>
	 * \n
	 * 私有属性：
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 属性ID </td>
	 * <td class="indexkey"> 类型 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TIP     </td><td> string </td><td> 鼠标移上去时显示的提示信息 </td></tr>
	 * <tr><td> PROPERTYID_CREATETYPE  </td><td> string </td><td> 创建类型，分三种取值:"ignore"(忽略),"win"(有窗口),"winless"(无窗口)\n
										此属性只在创建时可用</td></tr>
	 * <tr><td> PROPERTYID_BACKGROUND  </td><td> imageset (*注一) </td><td> 背景属性 </td></tr>
	 * </table> 
	 * </p>
	 * <p>
	     使用 IControl::SetProperty() 或 IUILib::CreateControl() 时需要使用到以上属性值ID　\n
		 使用者需要根据属性值的类型将属性值设置到 IVariableParam 对象中。bool值的属性值可以看作是int类型。
	 * </p>
	 * <p>
	 *	*注一: imageset类型是一个复合类型，只在编辑器中设置属性值时使用。其格式请参加编辑器使用文档。 \n
	 *		   在程序中自行设置属性时必须将其分解为若干简单类型的属性并单独设置。\n
	 *		   分解后属性ID如下所示：
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 属性ID </td>
	 * <td class="indexkey"> 类型 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> PROPERTYID_BGMODE   </td><td> int    </td><td> 背景模式 见 enmBGMode </td></tr>
	 * <tr><td> PROPERTYID_BGIMAGE1 </td><td> string </td><td> 背景topleft贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE2 </td><td> string </td><td> 背景区域top贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE3 </td><td> string </td><td> 背景区域topright贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE4 </td><td> string </td><td> 背景区域left贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE5 </td><td> string </td><td> 背景区域middle贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE6 </td><td> string </td><td> 背景区域right贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE7 </td><td> string </td><td> 背景区域bottomleft贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE8 </td><td> string </td><td> 背景区域bottom贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE9 </td><td> string </td><td> 背景区域bottomright贴图 </td></tr>	 
	 * <tr><td> PROPERTYID_BGCOLOR  </td><td> DWORD  </td><td> 背景区域颜色值 </td></tr>	 
	 * </table> 
	 * \n
	 * 	当背景模式值为 EBGM_NONE 时，没有背景，不需要设其它几个属性；\n
	 *	当背景模式值为 EBGM_COLOR 时，仅用颜色填充背景，使用到 PROPERTYID_BGCOLOR值；\n
	 *	当背景模式值为 EBGM_ONEIMAGE 时，使用一张图平铺，使用到 PROPERTYID_BGIMAGE5值；\n
	 *	当背景模式值为 EBGM_3HIMAGES 时，使用三张图水平平铺，使用到 PROPERTYID_BGIMAGE4,PROPERTYID_BGIMAGE5,PROPERTYID_BGIMAGE6；\n
	 *	当背景模式值为 EBGM_3VIMAGES 时，使用三张图垂直平铺，使用到 PROPERTYID_BGIMAGE2,PROPERTYID_BGIMAGE5,PROPERTYID_BGIMAGE8；\n
	 *	当背景模式值为 EBGM_9IMAGES 时，使用九张图，使用到1 -- 9九个贴图的属性；\n
	 * </p>
	 * <p>
	 *　　 事件列表：\n
	 * \n
	 * 公共事件(所有控件都会发生)：
	 * <table>
	 * <tr>
	 * <td class="indexkey"> 事件ID </td>
	 * <td class="indexkey"> 参数 </td>
	 * <td class="indexkey"> 描述 </td>
	 * </tr>
	 * <tr><td> EI_POSITONCHANGED </td><td> 无 </td><td> 控件矩形发生改变 </td></tr>
	 * <tr><td> EI_BASEMESSAGE </td><td> PARAMID_MESSAGE　int\n
										 PARAMID_WPARAM	DWORD\n
										 PARAMID_LPARAM	DWORD\n
										 PARAMID_RESULT	DWORD
										</td>
										<td> 基本window消息，目前仅发出:\n
											WM_LBUTTONDOWN \n
											WM_LBUTTONUP \n
											WM_LBUTTONDBLCLK \n
											WM_SETFOCUS \n
											WM_KILLFOCUS　消息</td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */

	class IUIContainer : public IControl
	{
	public:
	};
};

#endif

// InterfaceName:Container
// SubFolderName:UILib
