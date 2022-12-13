#ifndef __TLEVEL_I_H__
#define __TLEVEL_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITLevel;

	TENIO_MAKE_TYPEID_MAP(ITLevel, COMPONENTID_TLEVEL);

	 /**
	  * @ingroup tenuilib
	  * @brief uilib Level�ؼ�
	  *
	  * <p>�� 
	  *	����΢��������Ŀؼ�����������ʾ������Ľ��ȣ�Ҳ��������ʾ�û���ǰ�ĵȼ����ȼ��������ҵ�����    
	  * ���Զ���ͼƬ��ͼƬ��ɸ����֣�ͨ�����ؿɿ������ֵ���ʾ���</p>
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
	  * <tr><td> PROPERTYID_LEVELTOPIMAGE </td><td> #EVT_STRING </td><td> level�ϲ���ͼ </td></tr>
	  * <tr><td> PROPERTYID_LEVELBOTTOMIMAGE </td><td> #EVT_STRING </td><td> level�ײ���ͼ </td></tr>
	  * <tr><td> PROPERTYID_LEVELHEIGHT </td><td> #EVT_INT </td><td> level�� </td></tr>
	  * <tr><td> PROPERTYID_LEVELWIDTH </td><td> #EVT_INT </td><td> level�� </td></tr>
	  * <tr><td> PROPERTYID_TEXTVISIBLE </td><td> #EVT_BOOL </td><td> �Ƿ���ʾ���� </td></tr>
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
	  * <tr><td> �� </td><td>  </td><td>  </td></tr>
	  * </table>  
	  * </p>
	  *
	  * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	  */	 
	class ITLevel : public IControl
	{
	public:
		/**
		 * @brief ����Level��Χ
		 * @param nLowerRange Level��Χ��߽�
		 * @param nUpperRange Level��Χ�ұ߽�
		 * @param bRedraw �Ƿ��ػ�Level
		 */
		virtual void SetRange(int nLowerRange, int nUpperRange, BOOL bRedraw) = 0;
		/**
		 * @brief ��ȡLevel��Χ
		 * @param nLowerRange Level��Χ��߽�
		 * @param nUpperRange Level��Χ�ұ߽�
		 */
		virtual void GetRange(int& nLowerRange, int& nUpperRange) = 0;
		/**
		 * @brief ����Level��ǰλ��
		 * @param nPos Level��ǰλ��
		 * @param bRedraw �Ƿ��ػ�Level
		 * @return ������óɹ�������TRUE�����򷵻�FALSE
		 */
		virtual BOOL SetPos(int nPos, BOOL bRedraw) = 0;
		/**
		 * @brief ��ȡLevel��ǰλ��
		 * @return ����Level��ǰλ��
		 */
		virtual int GetPos() = 0;
		/**
		 * @brief ΪLevel��ǰλ������һ��ƫ�������ƶ���ƫ�ƺ��λ��
		 * @param nPos ���ӵ�ƫ����
		 * @param bRedraw �Ƿ��ػ�Level
		 * @return ����Level�ı�ǰ��λ��
		 */
		virtual int OffsetPos(int nPos, BOOL bRedraw) = 0;
		/**
		 * @brief Level������ʾ����
		 * @param bIsVisible �����Ƿ�ɼ�
		 */
		virtual void SetTextVisible(BOOL bIsVisible) = 0;
		/**
		 * @brief ��ȡLevel�����Ƿ�ɼ�
		 * @return ���������Ƿ�ɼ�
		 */
		virtual BOOL GetTextVisible() = 0;
		/**
		 * @brief ����Level������ɫ
		 * @param dwColor ������ɫ
		 */
		virtual void SetTextColor(COLORREF dwColor) = 0;
//		virtual COLORREF GetTextColor() = 0;
		/**
		 * @brief ����Level��������
		 * @param lpTextCaption ��������
		 */
		virtual void SetTextCaption(LPCTSTR lpTextCaption) = 0;
		/**
		 * @brief ��ȡLevel��������
		 * @return ������������
		 */
		virtual LPCTSTR GetTextCaption() = 0;
		/**
		 * @brief ����Level����ֵ�� StepIt()�����մ˲���ֵ����Level��ƫ����
		 * @param nStep ����ֵ
		 * @return ����Level�ı�ǰ�Ĳ���ֵ
		 */
		virtual int SetStep(int nStep) = 0;
		/**
		 * @brief ����Level
		 * @param bRedraw �Ƿ��ػ�Level
		 * @return ����Level�ı�ǰ��λ��
		 */
		virtual int StepIt(BOOL bRedraw) = 0;
	};
};

#endif

// InterfaceName:TLevel
// SubFolderName:uilib
