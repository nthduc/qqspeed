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
	 * @brief uilib checkbox�ؼ� 
	 *
	 * <p>�� 
	 * ������΢��checkbox�ؼ�����,��������ͨ�����ϡ����º���Чʱ��ͼƬ</p>
	 * <p>
	 *���� �����б�:<br>
	 * \n
	 * �������ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td>�� IUIContainer </td><td>  </td><td>  </td></tr>
	 * </table> 
	 * \n
	 * ˽�����ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR </td><td> #EVT_DWORD </td><td> ������ɫ </td></tr>
	 * <tr><td> PROPERTYID_CAPTION </td><td> #EVT_STRING </td><td> �������� </td></tr>
	 * <tr><td> PROPERTYID_LOGFONT </td><td> #EVT_FONTSET </td><td> �������� </td></tr>
	 * <tr><td> PROPERTYID_IMAGE </td><td> #EVT_STRING </td><td> checkbox��ͨ״̬�ĵ�ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_HOVER </td><td> #EVT_STRING </td><td> checkbox����״̬�ĵ�ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_PRESSED </td><td> #EVT_STRING </td><td> checkbox����״̬�ĵ�ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_DISABLE </td><td> #EVT_STRING </td><td> checkbox��Ч״̬�ĵ�ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_CHECKED </td><td> #EVT_STRING </td><td> checkbox��ͨ״̬��ѡ��ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_HOVERCHECKED </td><td> #EVT_STRING </td><td> checkbox����״̬��ѡ��ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_PRESSEDCHECKED </td><td> #EVT_STRING </td><td> checkbox����״̬��ѡ��ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_DISABLECHECKED </td><td> #EVT_STRING </td><td> checkbox��Ч״̬��ѡ��ͼ </td></tr>
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
	 * <tr><td> EI_SIGNALED </td><td> �� </td><td> ��ѡcheckboxʱ�������¼� </td></tr>
	 * <tr><td> EI_UNSIGNALED </td><td> �� </td><td> ȡ��ѡ��checkboxʱ�������¼� </td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */	
	class ITCheckBox : public IControl
	{
	public:
		/**
		 * @brief ��ȡcheckbox״̬
		 * @return checkbox��ѡ�з���TRUE�����򷵻�FALSE
		 */
		virtual BOOL IsChecked() = 0;
		/**
		 * @brief ����checkbox״̬
		 * @param bIsChecked checkbox״̬
		 */
		virtual void SetChecked(BOOL bIsChecked) = 0;
	};
};

#endif

// InterfaceName:TCheckBox
// SubFolderName:UILib
