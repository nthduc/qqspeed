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
	 * @brief �����ؼ�
	 *
	 * <p>�� 
	 *		�������������ñ���ͼƬ�򱳾�ɫ��û�������κ�UI�߼�����һ�������Ŀؼ���<br>
	 *	������������һ����ص��ӿؼ�����createtype����Ϊ"ignore"ʱ���������޴������͡�
	 * </p>
	 * <p>
	 *���� �����б�:\n
	 *\n
	 * �������ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TYPEID </td><td> int </td><td> �ؼ�����ID��ֻ�ڴ���ʱ��Ч </td></tr>
	 * <tr><td> PROPERTYID_XOFFSET </td><td> int </td><td> �ؼ����Ͻ�����ڸ��������Ͻǵ�x���� </td></tr>
	 * <tr><td> PROPERTYID_YOFFSET </td><td> int </td><td> �ؼ����Ͻ�����ڸ��������Ͻǵ�y���� </td></tr>
	 * <tr><td> PROPERTYID_WIDTH   </td><td> int </td><td> �ؼ����ο�� </td></tr>
	 * <tr><td> PROPERTYID_HEIGHT  </td><td> int </td><td> �ؼ����θ߶� </td></tr>
	 * <tr><td> PROPERTYID_VISIBLE </td><td> bool </td><td> �Ƿ�ɼ� </td></tr>
	 * <tr><td> PROPERTYID_DISABLE </td><td> bool </td><td> �Ƿ���� </td></tr>
	 * <tr><td> PROPERTYID_TRANSPARENT  </td><td> bool </td><td> �Ƿ����Ϣ͸�� </td></tr>
	 * <tr><td> PROPERTYID_ALPHA   </td><td> byte </td><td> �ؼ�����͸���ȣ�ȡֵ��0 ~255֮�� </td></tr>
	 * </table>
	 * \n
	 * ˽�����ԣ�
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> PROPERTYID_TIP     </td><td> string </td><td> �������ȥʱ��ʾ����ʾ��Ϣ </td></tr>
	 * <tr><td> PROPERTYID_CREATETYPE  </td><td> string </td><td> �������ͣ�������ȡֵ:"ignore"(����),"win"(�д���),"winless"(�޴���)\n
										������ֻ�ڴ���ʱ����</td></tr>
	 * <tr><td> PROPERTYID_BACKGROUND  </td><td> imageset (*עһ) </td><td> �������� </td></tr>
	 * </table> 
	 * </p>
	 * <p>
	     ʹ�� IControl::SetProperty() �� IUILib::CreateControl() ʱ��Ҫʹ�õ���������ֵID��\n
		 ʹ������Ҫ��������ֵ�����ͽ�����ֵ���õ� IVariableParam �����С�boolֵ������ֵ���Կ�����int���͡�
	 * </p>
	 * <p>
	 *	*עһ: imageset������һ���������ͣ�ֻ�ڱ༭������������ֵʱʹ�á����ʽ��μӱ༭��ʹ���ĵ��� \n
	 *		   �ڳ�����������������ʱ���뽫��ֽ�Ϊ���ɼ����͵����Բ��������á�\n
	 *		   �ֽ������ID������ʾ��
	 * <table>
	 * <tr>
	 * <td class="indexkey"> ����ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> PROPERTYID_BGMODE   </td><td> int    </td><td> ����ģʽ �� enmBGMode </td></tr>
	 * <tr><td> PROPERTYID_BGIMAGE1 </td><td> string </td><td> ����topleft��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE2 </td><td> string </td><td> ��������top��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE3 </td><td> string </td><td> ��������topright��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE4 </td><td> string </td><td> ��������left��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE5 </td><td> string </td><td> ��������middle��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE6 </td><td> string </td><td> ��������right��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE7 </td><td> string </td><td> ��������bottomleft��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE8 </td><td> string </td><td> ��������bottom��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGIMAGE9 </td><td> string </td><td> ��������bottomright��ͼ </td></tr>	 
	 * <tr><td> PROPERTYID_BGCOLOR  </td><td> DWORD  </td><td> ����������ɫֵ </td></tr>	 
	 * </table> 
	 * \n
	 * 	������ģʽֵΪ EBGM_NONE ʱ��û�б���������Ҫ�������������ԣ�\n
	 *	������ģʽֵΪ EBGM_COLOR ʱ��������ɫ��䱳����ʹ�õ� PROPERTYID_BGCOLORֵ��\n
	 *	������ģʽֵΪ EBGM_ONEIMAGE ʱ��ʹ��һ��ͼƽ�̣�ʹ�õ� PROPERTYID_BGIMAGE5ֵ��\n
	 *	������ģʽֵΪ EBGM_3HIMAGES ʱ��ʹ������ͼˮƽƽ�̣�ʹ�õ� PROPERTYID_BGIMAGE4,PROPERTYID_BGIMAGE5,PROPERTYID_BGIMAGE6��\n
	 *	������ģʽֵΪ EBGM_3VIMAGES ʱ��ʹ������ͼ��ֱƽ�̣�ʹ�õ� PROPERTYID_BGIMAGE2,PROPERTYID_BGIMAGE5,PROPERTYID_BGIMAGE8��\n
	 *	������ģʽֵΪ EBGM_9IMAGES ʱ��ʹ�þ���ͼ��ʹ�õ�1 -- 9�Ÿ���ͼ�����ԣ�\n
	 * </p>
	 * <p>
	 *���� �¼��б�\n
	 * \n
	 * �����¼�(���пؼ����ᷢ��)��
	 * <table>
	 * <tr>
	 * <td class="indexkey"> �¼�ID </td>
	 * <td class="indexkey"> ���� </td>
	 * <td class="indexkey"> ���� </td>
	 * </tr>
	 * <tr><td> EI_POSITONCHANGED </td><td> �� </td><td> �ؼ����η����ı� </td></tr>
	 * <tr><td> EI_BASEMESSAGE </td><td> PARAMID_MESSAGE��int\n
										 PARAMID_WPARAM	DWORD\n
										 PARAMID_LPARAM	DWORD\n
										 PARAMID_RESULT	DWORD
										</td>
										<td> ����window��Ϣ��Ŀǰ������:\n
											WM_LBUTTONDOWN \n
											WM_LBUTTONUP \n
											WM_LBUTTONDBLCLK \n
											WM_SETFOCUS \n
											WM_KILLFOCUS����Ϣ</td></tr>
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
