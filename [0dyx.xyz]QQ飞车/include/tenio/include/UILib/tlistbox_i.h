#ifndef __TLISTBOX_I_H__
#define __TLISTBOX_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITListBox;

	TENIO_MAKE_TYPEID_MAP(ITListBox, COMPONENTID_TLISTBOX);

	/**
	 * @ingroup tenuilib
	 * @brief 单列列表
	 *
	 * <p>　 
	 *		
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
	 * <tr><td> PROPERTYID_TEXTCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_ALIGNMODE </td><td> #EVT_ALIGNMODE </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_LOGFONT </td><td> #EVT_FONTSET </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_ITEMHEIGHT </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_ITEMMARGIN </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_HASBORDER </td><td> #EVT_BOOL </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BORDERCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_SELECTTEXTCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_SELECTBGCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_SCROLLIMAGE </td><td> #EVT_STRING </td><td>  </td></tr>
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
	 * <tr><td> EI_LISTBOX_SELECTITEM </td><td> 无 </td><td> 选择行时触发的事件 </td></tr>
	 * </table>  
	 * </p>
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITListBox : public IControl
	{
	public:
		
		/**
		 *  @brief 在列表尾部添加一行
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
		 *  @brief 获得列表的行数
		 *  @return 返回列表的总行数
		 *  @sa 
		 */	
		virtual int  GetItemCount() = 0;
		
		/**
		 *  @brief 获得当前被选中的行号
		 *  @return 返回当前列表中被选中的行号
		 *  @sa 
		 */	
		virtual int  GetSelItem() = 0;

		/**
		 *  @brief 设置当前选中项
		 *  @param nIndex 被选中的项索引号
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL SetSelItem(int nIndex) = 0;
		
		/**
		 *  @brief 设置当前被选中的字符串
		 *  @param lpszSelText 被选中字符串，必须已存在于下拉列表中
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL SetSelString(LPCTSTR lpszSelText) = 0;
		
		/**
		 *  @brief 根据行号获得该行的字符串
		 *  @param nIndex 行号
		 *  @param lpszText 存放字符串的指针
		 *  @param nMaxLen	可存放的最大字符数
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL GetStringByIndex(int nIndex, LPTSTR lpszText, UINT nMaxLen) = 0;

		/**
		 *  @brief 获得行高
		 *  @return 返回行高
		 *  @sa 
		 */
		virtual int  GetItemHeight() = 0;
	};
};

#endif

// InterfaceName:TListBox
// SubFolderName:uilib
