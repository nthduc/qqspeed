#ifndef __TSLIDER_I_H__
#define __TSLIDER_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITSlider;

	TENIO_MAKE_TYPEID_MAP(ITSlider, COMPONENTID_TSLIDER);

	/**
	 * @ingroup tenuilib
	 * @brief uilib slider控件 
	 *
	 * <p>　 
	 * 功能与微软slider控件相似，通过鼠标和键盘控制滑块的移动，可自定义图片 </p>
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
	 * <tr><td> PROPERTYID_IMAGE </td><td> #EVT_STRING </td><td> 滑块普通状态的贴图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_HOVER </td><td> #EVT_STRING </td><td> 滑块浮上状态的贴图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_PRESSED </td><td> #EVT_STRING </td><td> 滑块按下状态的贴图 </td></tr>
	 * <tr><td> PROPERTYID_IMG_DISABLE </td><td> #EVT_STRING </td><td> 滑块无效状态的贴图 </td></tr>
	 * <tr><td> PROPERTYID_SLIDERTOPIMAGE </td><td> #EVT_STRING </td><td> 滑竿上层贴图 </td></tr>
	 * <tr><td> PROPERTYID_SLIDERBOTTOMIMAGE </td><td> #EVT_STRING </td><td> 滑竿底层贴图 </td></tr>
	 * <tr><td> PROPERTYID_SLIDERCHANNELHEIGHT </td><td> #EVT_INT </td><td> 水平/垂直slider滑竿高度/宽度 </td></tr>
	 * <tr><td> PROPERTYID_SLIDERISHORIZON </td><td> #EVT_BOOL </td><td> 是否为水平slider </td></tr>
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
	 * <tr><td> EI_SLIDER_THUMBPOSCHANGED </td><td> EPARAMID_SLD_TYPE\n 
	 * &nbsp;&nbsp;&nbsp;&nbsp;类型 : int\n 
	 * &nbsp;&nbsp;&nbsp;&nbsp;描述 : slider类型,SLIDER_HORZ为水平slider,SLIDER_VERT为垂直slider</td>
	 * <td> 滑块位置变化时触发该事件 </td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */
	
	 	
	class ITSlider : public IControl
	{
	public:
		/**
		 * @brief 获取行大小，每按一下键盘方向键，Slider移动一行
		 * @return 返回行大小
		 */
		virtual int GetLineSize() = 0;
		/**
		 * @brief 设置行大小，每按一下键盘方向键，Slider移动一行
		 * @param nSize 行大小
		 * @return 返回Slider改变前的行大小
		 */
		virtual int SetLineSize(int nSize) = 0;
		/**
		 * @brief 获取页大小，每按一下PgUp或PgDn，Slider移动一页
		 * @return 返回页大小
		 */
		virtual int GetPageSize() = 0;
		/**
		 * @brief 设置页大小，每按一下PgUp或PgDn，Slider移动一页
		 * @param nSize 页大小
		 * @return 返回Slider改变前的页大小
		 */
		virtual int SetPageSize(int nSize) = 0;
		/**
		 * @brief 获取Slider范围
		 * @param nMin Slider范围左边界
		 * @param nMax Slider范围右边界
		 */
		virtual void GetRange(int& nMin, int& nMax) = 0;
		/**
		 * @brief 设置Slider范围
		 * @param nMin Slider范围左边界
		 * @param nMax Slider范围右边界
		 * @param bRedraw 是否重绘Slider
		 */
		virtual void SetRange(int nMin, int nMax, BOOL bRedraw) = 0;
		/**
		 * @brief 获取Slider当前位置
		 * @return 返回Slider当前位置
		 */
		virtual int GetPos() = 0;
		/**
		 * @brief 设置Slider当前位置
		 * @param nPos Slider当前位置
		 * @param bRedraw 是否重绘Slider
		 * @return 成功返回TRUE，否则返回FALSE
		 */
		virtual BOOL SetPos(int nPos, BOOL bRedraw) = 0; 
	};
};

#endif

// InterfaceName:TSlider
// SubFolderName:uilib
