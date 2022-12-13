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
	 * @brief ��ť
	 *
	 * <p>�� 
	 *		��ť�ؼ�����Ϊ���ְ�ť����ͨ��ť�Ͳ��ɻָ���ť(���¿ɲ��ɻָ�)
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
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * </table> 
	 * \n
	 * ˽�����ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TIP     </td><td> �� IUIContainer </td><td> </td></tr>
	 * <tr><td> PROPERTYID_BACKGROUND  </td><td> �� IUIContainer </td><td> </td></tr>

	 * <tr><td> PROPERTYID_BUTTONTYPE </td><td> string </td><td> ����ȡֵ��"NormalButton"(��ͨ��ť),"UnreversedButton"(���ɻָ���ť) </td></tr>
	 * <tr><td> PROPERTYID_SIGNALED   </td><td> int </td><td> �����ʾ����ΪSignaled״̬ </td></tr>
	 * <tr><td> PROPERTYID_BG_HOVER ������</td><td> imageset </td><td> ��ͣ״̬�ı���, �ֽⷽʽ�� IUIContainer ������Ӻ�׺_HOVER </td></tr>
	 * <tr><td> PROPERTYID_BG_PRESSED ����</td><td> imageset </td><td> ����״̬�ı���, �ֽⷽʽ�� IUIContainer ������Ӻ�׺_PRESSED </td></tr>
	 * <tr><td> PROPERTYID_BG_DISABLE ����</td><td> imageset </td><td> ������״̬�µı���, �ֽⷽʽ�� IUIContainer ������Ӻ�׺_DISABLE </td></tr>
	 * <tr><td> PROPERTYID_TEXTFONT_NORMAL   </td><td> string </td><td> �������������, �ַ�����ʽ�� EVT_FONTSET ���ڳ���������ʱ��ֽ�</td></tr>
	 * <tr><td> PROPERTYID_TEXTFONT_HOVER    </td><td> string </td><td> ��ͣ״̬�µ����� </td></tr>
	 * <tr><td> PROPERTYID_TEXTFONT_PRESSED  </td><td> string </td><td> ����״̬�µ����� </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR         </td><td> DWORD </td><td> ��������µ�������ɫ </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR_HOVER   </td><td> DWORD </td><td> ��ͣ����µ�������ɫ </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR_PRESSED </td><td> DWORD </td><td> ����״̬�µ�������ɫ </td></tr>
	 * <tr><td> PROPERTYID_DEFAULTBUTTON </td><td> bool </td><td> �Ƿ���ȱʡִ�а�ť </td></tr>
	 * <tr><td> PROPERTYID_ITEMMARGIN    </td><td> int </td><td> ������߿��� </td></tr>
	 * <tr><td> PROPERTYID_AUTOSIZE_IMAGE</td><td> bool </td><td> �Ƿ�����ӦͼƬ��С��ֻ��PROPERTYID_BGMODEΪ EBGM_ONEIMAGE ʱ��Ч</td></tr>
	 * <tr><td> PROPERTYID_AUTOSIZE </td><td> EVT_BOOL </td><td> �Ƿ���������ӦͼƬ��С </td></tr>
	 * <tr><td> PROPERTYID_TABSTOP </td><td> bool </td><td> �Ƿ���ӦTab���л����� </td></tr>
	 * <tr><td> PROPERTYID_CAPTION </td><td> string </td><td> �������� </td></tr>
	 * <tr><td> PROPERTYID_SOUND_CLICK </td><td> string </td><td> ���ʱ�����ļ�·�� </td></tr>
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
	 * <tr><td> EI_BUTTON_CLICK </td><td> EPARAMID_BUTTON_CLICK_X	int\n
										EPARAMID_BUTTON_CLICK_Y		int
										</td><td> �����ť��Ϣ </td></tr>
	 * <tr><td> EI_SIGNALED </td><td> �� </td><td> ���signal״̬ </td></tr>
	 * <tr><td> EI_UNSIGNALED </td><td> �� </td><td> ȡ��signal״̬ </td></tr>
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