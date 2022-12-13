#ifndef __TTOOLTIP_I_H__
#define __TTOOLTIP_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITToolTip;

	TENIO_MAKE_TYPEID_MAP(ITToolTip, COMPONENTID_TTOOLTIP);
	/**
	 * @ingroup tenuilib
	 * @brief 
	 *
	 * <p>�� 
	 *     ���ڸ��������Ϣ��̬����ʾ�����ؼ���tip��Ϣ��<br>
	 *     ��̬����ʾĳ��tip��Ϣ, �ؼ��Ĵ�С�ɶ�̬����ʾ�ַ����ĳ��ȱ仯��
	 * </p>
	 * <p>�� 
	 *     ���ʹ�ö��㴰����ϵ��ȫ��Ψһ��tip���󷽷���μ� IUILib::SetGlobalTipControlName()
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
	 * <tr><td> PROPERTYID_BACKGROUND  </td><td> �� IUIContainer </td><td> </td></tr>
	 * <tr><td> PROPERTYID_TEXTCOLOR   </td><td> #EVT_DWORD </td><td> ������ɫ </td></tr>
	 * <tr><td> PROPERTYID_CAPTION </td><td> #EVT_STRING </td><td> �������� </td></tr>
	 * <tr><td> PROPERTYID_LOGFONT </td><td> EVT_FONTSET </td><td> �������ԣ���������Ҫ�ֽ⣬�� ITStatic </td></tr>
	 * <tr><td> PROPERTYID_AUTOCHANGELINE </td><td> EVT_BOOL </td><td>  </td></tr>
	 * <tr><td> PROPERTYID_MINWIDTH </td><td> #EVT_INT </td><td> ������С��� </td></tr>
	 * <tr><td> PROPERTYID_MINHEIGHT </td><td> #EVT_INT </td><td> ������С�߶� </td></tr>
	 * <tr><td> PROPERTYID_MULTILINE </td><td> #EVT_BOOL </td><td> �Ƿ�֧����'\n'ת������� </td></tr>
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
	 * <tr><td>  </td><td>  </td><td>  </td></tr>
	 * </table>  
	 * </p>
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */
	
	class ITToolTip : public IControl
	{
	public:
		/**
		 *  @brief Ϊ�ؼ�����tip���ơ�
		 *  @param dwID ���ӿؼ���ID�������ID�ѱ����������ֻ�ı�������������
		 *  @param lpszText �ؼ�����������
		 *  @sa 
		 */
		virtual void AddTool(DWORD dwID, LPCTSTR lpszText) = 0;

		/**
		 *  @brief �Ƴ�һ���ؼ���tip����
		 *  @param dwID ���Ƴ��ؼ���ID
		 *  @sa 
		 */		
		virtual void RemoveTool(DWORD dwID) = 0;

		/**
		 *  @brief �Ƴ����е�tip����
		 *  @sa 
		 */
		virtual void RemoveAllTools() = 0;

		/**
		 *  @brief ������¼��Ĵ���
		 *  @param dwID ��������¼��Ŀؼ�ID
		 *  @param pParam �¼�����
		 *  @sa 
		 */
		virtual void RelayEvent(DWORD dwID, IVariableParam* pParam) = 0;

		/**
		 *  @brief ���õ�ǰ��ʾ��tip����
		 *  @param lpszText ��Ҫ��ʾ��Tip����
		 *  @sa 
		 */
		virtual void SetText(LPCTSTR lpszText) = 0;

		/**
		 *  @brief ������ʾ��ʱ
		 *  @param nDelayTime ��ʾ��ʱ����ִ�� Show() ����������ʾ����֮���ʱ��
		 *  @sa 
		 */
		virtual void SetShowDelay(int nDelayTime) = 0;

		/**
		 *  @brief ���þ�̬��ʾʱ��ʾ��ʱ��
		 *  @param nShowTime ��ʾʱ������ʱ��Ϊ0ʱ����ʾ���޳����������Զ�����
		 *  @sa 
		 */
		virtual void SetShowTime(int nShowTime) = 0;

		/**
		 *  @brief ����
		 *  @param nTime �������̵���ʱ��
		 *  @sa 
		 */
		virtual void FadeIn(int nTime) = 0;

		/**
		 *  @brief ����
		 *  @param nTime ������̵���ʱ��
		 *  @sa 
		 */	
		virtual void FadeOut(int nTime) = 0;
	};
};

#endif

// InterfaceName:TToolTip
// SubFolderName:uilib
