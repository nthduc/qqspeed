#ifndef __TSLIDER_I_H__
#define __TSLIDER_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITSlider;

	TENIO_MAKE_TYPEID_MAP(ITSlider, COMPONENTID_TSLIDER);

	/**
	 * @ingroup tenuilib
	 * @brief uilib slider�ؼ� 
	 *
	 * <p>�� 
	 * ������΢��slider�ؼ����ƣ�ͨ�����ͼ��̿��ƻ�����ƶ������Զ���ͼƬ </p>
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
	 * <tr><td> PROPERTYID_IMAGE </td><td> #EVT_STRING </td><td> ������ͨ״̬����ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_HOVER </td><td> #EVT_STRING </td><td> ���鸡��״̬����ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_PRESSED </td><td> #EVT_STRING </td><td> ���鰴��״̬����ͼ </td></tr>
	 * <tr><td> PROPERTYID_IMG_DISABLE </td><td> #EVT_STRING </td><td> ������Ч״̬����ͼ </td></tr>
	 * <tr><td> PROPERTYID_SLIDERTOPIMAGE </td><td> #EVT_STRING </td><td> �����ϲ���ͼ </td></tr>
	 * <tr><td> PROPERTYID_SLIDERBOTTOMIMAGE </td><td> #EVT_STRING </td><td> ���͵ײ���ͼ </td></tr>
	 * <tr><td> PROPERTYID_SLIDERCHANNELHEIGHT </td><td> #EVT_INT </td><td> ˮƽ/��ֱslider���͸߶�/��� </td></tr>
	 * <tr><td> PROPERTYID_SLIDERISHORIZON </td><td> #EVT_BOOL </td><td> �Ƿ�Ϊˮƽslider </td></tr>
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
	 * <tr><td> EI_SLIDER_THUMBPOSCHANGED </td><td> EPARAMID_SLD_TYPE\n 
	 * &nbsp;&nbsp;&nbsp;&nbsp;���� : int\n 
	 * &nbsp;&nbsp;&nbsp;&nbsp;���� : slider����,SLIDER_HORZΪˮƽslider,SLIDER_VERTΪ��ֱslider</td>
	 * <td> ����λ�ñ仯ʱ�������¼� </td></tr>
	 * </table>  
	 * </p>
	 *
	 * @sa  IControl , IUILib , IUILibEvent , IVariableParam 
	 */
	
	 	
	class ITSlider : public IControl
	{
	public:
		/**
		 * @brief ��ȡ�д�С��ÿ��һ�¼��̷������Slider�ƶ�һ��
		 * @return �����д�С
		 */
		virtual int GetLineSize() = 0;
		/**
		 * @brief �����д�С��ÿ��һ�¼��̷������Slider�ƶ�һ��
		 * @param nSize �д�С
		 * @return ����Slider�ı�ǰ���д�С
		 */
		virtual int SetLineSize(int nSize) = 0;
		/**
		 * @brief ��ȡҳ��С��ÿ��һ��PgUp��PgDn��Slider�ƶ�һҳ
		 * @return ����ҳ��С
		 */
		virtual int GetPageSize() = 0;
		/**
		 * @brief ����ҳ��С��ÿ��һ��PgUp��PgDn��Slider�ƶ�һҳ
		 * @param nSize ҳ��С
		 * @return ����Slider�ı�ǰ��ҳ��С
		 */
		virtual int SetPageSize(int nSize) = 0;
		/**
		 * @brief ��ȡSlider��Χ
		 * @param nMin Slider��Χ��߽�
		 * @param nMax Slider��Χ�ұ߽�
		 */
		virtual void GetRange(int& nMin, int& nMax) = 0;
		/**
		 * @brief ����Slider��Χ
		 * @param nMin Slider��Χ��߽�
		 * @param nMax Slider��Χ�ұ߽�
		 * @param bRedraw �Ƿ��ػ�Slider
		 */
		virtual void SetRange(int nMin, int nMax, BOOL bRedraw) = 0;
		/**
		 * @brief ��ȡSlider��ǰλ��
		 * @return ����Slider��ǰλ��
		 */
		virtual int GetPos() = 0;
		/**
		 * @brief ����Slider��ǰλ��
		 * @param nPos Slider��ǰλ��
		 * @param bRedraw �Ƿ��ػ�Slider
		 * @return �ɹ�����TRUE�����򷵻�FALSE
		 */
		virtual BOOL SetPos(int nPos, BOOL bRedraw) = 0; 
	};
};

#endif

// InterfaceName:TSlider
// SubFolderName:uilib
