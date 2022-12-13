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
	 * @brief �����б�
	 *
	 * <p>�� 
	 *		
	 * </p>
	 * <p>
	 *���� �����б�<br>
	 * \n
	 * �������ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> �� IUIContainer  </td><td>  </td><td>  </td></tr>
	 * </table> 
	 * \n
	 * ˽�����ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
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
	 *���� �¼��б�:<br>
	 * <table>
	 * <tr>
	 * <td class="indexkey"> �¼�ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> ������Ϣ�� IUIContainer  </td><td> </td><td></td></tr>
	 * <tr><td> EI_LISTBOX_SELECTITEM </td><td> �� </td><td> ѡ����ʱ�������¼� </td></tr>
	 * </table>  
	 * </p>
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITListBox : public IControl
	{
	public:
		
		/**
		 *  @brief ���б�β�����һ��
		 *  @param lpszText ������ʾ����
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */
		virtual BOOL AppendItem(LPCTSTR lpszText) = 0;

		/**
		 *  @brief ɾ����
		 *  @param nIndex ��ɾ��������������0��ʼ
		 *  @return ɾ���ɹ�����TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL DeleteItem(int nIndex) = 0;

		/**
		 *  @brief ɾ��������
		 *  @sa 
		 */	
		virtual void DeleteAll() = 0;
		
		/**
		 *  @brief ����б������
		 *  @return �����б��������
		 *  @sa 
		 */	
		virtual int  GetItemCount() = 0;
		
		/**
		 *  @brief ��õ�ǰ��ѡ�е��к�
		 *  @return ���ص�ǰ�б��б�ѡ�е��к�
		 *  @sa 
		 */	
		virtual int  GetSelItem() = 0;

		/**
		 *  @brief ���õ�ǰѡ����
		 *  @param nIndex ��ѡ�е���������
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL SetSelItem(int nIndex) = 0;
		
		/**
		 *  @brief ���õ�ǰ��ѡ�е��ַ���
		 *  @param lpszSelText ��ѡ���ַ����������Ѵ����������б���
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL SetSelString(LPCTSTR lpszSelText) = 0;
		
		/**
		 *  @brief �����кŻ�ø��е��ַ���
		 *  @param nIndex �к�
		 *  @param lpszText ����ַ�����ָ��
		 *  @param nMaxLen	�ɴ�ŵ�����ַ���
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL GetStringByIndex(int nIndex, LPTSTR lpszText, UINT nMaxLen) = 0;

		/**
		 *  @brief ����и�
		 *  @return �����и�
		 *  @sa 
		 */
		virtual int  GetItemHeight() = 0;
	};
};

#endif

// InterfaceName:TListBox
// SubFolderName:uilib
