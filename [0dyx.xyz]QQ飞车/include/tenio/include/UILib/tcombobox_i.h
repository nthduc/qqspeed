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
	 * @brief ��Ͽ�
	 *
	 * <p>�� 
	 *		ͬwindowsϵͳ����Ͽ�
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
	 * <tr><td> �� IUIContainer </td><td>  </td><td>  </td></tr>
	 * </table> 
	 * \n
	 * ˽�����ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TABSTOP </td><td> #EVT_BOOL </td><td> �Ƿ���ӦTab���л����� </td></tr>
	 * <tr><td> PROPERTYID_DROPLIST </td><td> #EVT_BOOL </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBG </td><td> #EVT_STRING </td><td> �ַ�����ʽͬiamgeset���ͣ����ڳ�������������ʱ����ֽ⡣��ͬ </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBGHOVER </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBGPRESSED </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONBGDISABLE </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_LISTBOXBACKGROUND </td><td> #EVT_STRING </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_VISIBLEITEMNUM </td><td> #EVT_INT </td><td> �½��б�ɼ����� </td></tr>
	 * <tr><td> PROPERTYID_POPUPLISTBOX </td><td> #EVT_BOOL </td><td> �������б��Ƿ��������������� </td></tr>
	 * <tr><td> PROPERTYID_BUTTONWIDTH </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_BUTTONHEIGHT </td><td> #EVT_INT </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_LISTBOXBORDER </td><td> #EVT_BOOL </td><td>  </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR </td><td> #EVT_DWORD </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_LOGFONT </td><td> #EVT_FONTSET </td><td> ��ֽ� </td></tr>
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
	 *���� �¼��б�:<br>
	 * <table>
	 * <tr>
	 * <td class="indexkey"> �¼�ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> ������Ϣ�� IUIContainer  </td><td> </td><td></td></tr>
	 * <tr><td> EI_COMBOBOX_SELCHANGED </td><td> �� </td><td> �����б��ѡ�����˸ı� </td></tr>
	 * <tr><td> EI_COMBOBOX_TEXTCHANGED </td><td> �� </td><td> Edit���������ݷ����ı� </td></tr>
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * </table>  
	 * </p>
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITComboBox : public IControl
	{
	public:
		/**
		 *  @brief �������б�β�����һ��
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
		 *  @brief ��������б������
		 *  @return ���������б��������
		 *  @sa 
		 */	
		virtual int  GetItemCount() = 0;
		
		/**
		 *  @brief ��õ�ǰ��ѡ�е��к�
		 *  @return ���ص�ǰ�����б��б�ѡ�е��к�
		 *  @sa 
		 */	
		virtual int  GetCurSel() = 0;

		/**
		 *  @brief ���õ�ǰѡ����
		 *  @param nIndex ��ѡ�е���������
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL SetCurSel(int nIndex) = 0;
		
		/**
		 *  @brief ���õ�ǰ��ѡ�е��ַ���
		 *  @param lpszSelText ��ѡ���ַ����������Ѵ����������б���
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL SetSelString(LPCTSTR lpszSelText) = 0;

		/**
		 *  @brief ��ñ༭������ʾ������
		 *  @param lpszText ������ֵ�bufferָ��
		 *  @param nMaxLen���ɴ�ŵ�����ַ���
		 *  @sa 
		 */	
		virtual void GetEditString(LPTSTR lpszText, UINT nMaxLen) = 0;
	};
};

#endif

// InterfaceName:TComboBox
// SubFolderName:uilib
