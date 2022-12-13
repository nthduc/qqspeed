#ifndef __TDIALOG_I_H__
#define __TDIALOG_I_H__

#include "control_i.h"

namespace Tenio
{
	class ITDialog;
	TENIO_MAKE_TYPEID_MAP(ITDialog, COMPONENTID_TDIALOG);

	/**
	 * @ingroup tenuilib
	 * @brief �Ի���
	 *
	 * <p>�� 
	 *	������ʽ�Ի���	
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
	 * <tr><td> PROPERTYID_BACKGROUND  </td><td> �� IUIContainer </td><td> </td></tr>
	 * <tr><td> PROPERTYID_SIZEBOX </td><td> #EVT_BOOL </td><td> �ǲ��������߿�ı��С </td></tr>
	 * <tr><td> PROPERTYID_ALWAYSONTOP </td><td> #EVT_BOOL </td><td> �Ƿ�������ǰ�� </td></tr>
	 * <tr><td> PROPERTYID_TOOLWINDOW </td><td> #EVT_BOOL </td><td> �Ƿ�����������û�б����� </td></tr>
	 * <tr><td> PROPERTYID_CAPTIONHEIGHT </td><td> #EVT_INT </td><td> ����������߶�(�ڴ���������갴�¿����϶��Ի���) </td></tr>
	 * <tr><td> PROPERTYID_MINWIDTH </td><td> #EVT_INT </td><td> ������С��� </td></tr>
	 * <tr><td> PROPERTYID_MINHEIGHT </td><td> #EVT_INT </td><td> ������С�߶� </td></tr>
	 * <tr><td> PROPERTYID_MAXWIDTH </td><td> #EVT_INT </td><td> ��������� </td></tr>
	 * <tr><td> PROPERTYID_MAXHEIGHT </td><td> #EVT_INT </td><td> �������߶� </td></tr>
	 * <tr><td> PROPERTYID_REGIONSET </td><td> #EVT_STRING </td><td> �Ի�����״���ã�Ŀǰ��֧�ְ�Mask��ɫֵ������״��\n
																	��ʽΪ: FROMBGIMAGES:0x0000ff ð�ź����ʮ�����Ʊ�ʾ��mask��ɫֵ��\n
																	�������Ҫ������״�����ַ���Ϊ NONE: </td></tr>
	 * <tr><td> PROPERTYID_ICON </td><td> #EVT_STRING </td><td> �Ի���������������ʾ��ͼ���ļ�·�� </td></tr>
	 * <tr><td> PROPERTYID_CAPTION </td><td> #EVT_STRING </td><td> �Ի���������������ʾ�ı������� </td></tr>
	 * <tr><td> PROPERTYID_SOUND_OPEN </td><td> #EVT_STRING </td><td> �Ի��򵯳�ʱ���ŵ������ļ�·�� </td></tr>
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
	 * <tr><td> �� </td><td>  </td><td>  </td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITDialog : public IControl
	{
	public:
		/**
		 *  @brief ���Ի���������Ļ�м�
		 *  @sa 
		 */		
		virtual void MoveToCenter() = 0;
		
		/**
		 *  @brief ��С���Ի���
		 *  @sa 
		 */		
		virtual void Minimize() = 0;

		/**
		 *  @brief ��󻯶Ի���
		 *  @sa 
		 */	
		virtual void Maximize() = 0;

		/**
		 *  @brief ���Ի���ָ�������С
		 *  @sa 
		 */		
		virtual void Restore() = 0;

		/**
		 *  @brief ��������ʽ�����Ի�����δʵ�֣�
		 *	@return ���عرնԻ���ǰ����ؼ���ID��һ���ǰ�ť��
		 *  @sa 
		 */	
		virtual DWORD DoModal() = 0;

		/**
		 *  @brief ��������ʽ�Ի���
		 *  @param dwInvokeControl ���ô˺����Ŀؼ�ID��һ���ǶԻ����ϵİ�ť��
		 */
		virtual void EndModal(DWORD dwInvokeControl) = 0;
		
	};
}

#endif