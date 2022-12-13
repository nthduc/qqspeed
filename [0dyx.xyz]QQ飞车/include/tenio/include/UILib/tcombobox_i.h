#ifndef __TCOMBOBOX_I_H__
#define __TCOMBOBOX_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITComboBox;

	TENIO_MAKE_TYPEID_MAP(ITComboBox, COMPONENTID_TCOMBOBOX);

	/**
	 * @ingroup tenuilib
	 * @brief 组合框
	 *
	 * <p>　 
	 *		同windows系统的组合框。
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
	 * <tr><td> PROPERTYID_TABSTOP </td><td> #EVT_BOOL </td><td> 是否响应Tab键切换焦点 </td></tr>
	 * <tr><td> PROPERTYID_DROPLIST </td><td> #EVT_BOOL </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBG </td><td> #EVT_STRING </td><td> 字符串格式同iamgeset类型，但在程序中自行设置时不需分解。下同 </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBGHOVER </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBGPRESSED </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBGDISABLE </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_LISTBOXBACKGROUND </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_VISIBLEITEMNUM </td><td> #EVT_INT </td><td> 下接列表可见行数 </td></tr>
	 * <tr><td> PROPERTYID_POPUPLISTBOX </td><td> #EVT_BOOL </td><td> 弹出的列表是否可伸出主窗口以外 </td></tr>
	 * <tr><td> PROPERTYID_BUTTONWIDTH </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONHEIGHT </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_LISTBOXBORDER </td><td> #EVT_BOOL </td><td>  </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_LOGFONT </td><td> #EVT_FONTSET </td><td> 需分解 </td></tr>
	 * <tr><td> PROPERTYID_NUMBERONLY </td><td> #EVT_BOOL </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_MAXCHARACTERS </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_HASBORDER </td><td> #EVT_BOOL </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BORDERCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_ITEMHEIGHT </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_ITEMMARGIN </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_SELECTTEXTCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_SELECTBGCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_SCROLLIMAGE </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
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
	 * <tr><td> EI_COMBOBOX_SELCHANGED </td><td> 无 </td><td> 下拉列表的选择发生了改变 </td></tr>
	 * <tr><td> EI_COMBOBOX_TEXTCHANGED </td><td> 无 </td><td> Edit框文字内容发生改变 </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * </table>  
	 * </p>
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITComboBox : public IControl
	{
	public:
		/**
		 *  @brief 在下拉列表尾部添加一行
		 *  @param lpszText 该行显示内容
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */
		virtual BOOL AppendItem(LPCTSTR lpszText) = 0;

		/**
		 *  @brief 删除行
		 *  @param nIndex 被删除的行索引，以0开始
		 *  @return 删除成功返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL DeleteItem(int nIndex) = 0;

		/**
		 *  @brief 删除所有行
		 *  @sa 
		 */	
		virtual void DeleteAll() = 0;
		
		/**
		 *  @brief 获得下拉列表的行数
		 *  @return 返回下拉列表的总行数
		 *  @sa 
		 */	
		virtual int  GetItemCount() = 0;
		
		/**
		 *  @brief 获得当前被选中的行号
		 *  @return 返回当前下拉列表中被选中的行号
		 *  @sa 
		 */	
		virtual int  GetCurSel() = 0;

		/**
		 *  @brief 设置当前选中项
		 *  @param nIndex 被选中的项索引号
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL SetCurSel(int nIndex) = 0;
		
		/**
		 *  @brief 设置当前被选中的字符串
		 *  @param lpszSelText 被选中字符串，必须已存在于下拉列表中
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL SetSelString(LPCTSTR lpszSelText) = 0;

		/**
		 *  @brief 获得编辑框中显示的文字
		 *  @param lpszText 存放文字的buffer指针
		 *  @param nMaxLen　可存放的最大字符数
		 *  @sa 
		 */	
		virtual void GetEditString(LPTSTR lpszText, UINT nMaxLen) = 0;
	};
};

#endif

// InterfaceName:TComboBox
// SubFolderName:uilib
