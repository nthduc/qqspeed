#ifndef __MIXER_I_H__
#define __MIXER_I_H__

#include "canvas_i.h"
#include "material_i.h"
#include "normaltext_i.h"
#include "junestruct.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief ����������ӿ�
	 *
	 * <p>
	 *���� 
	 * </p>
	 *
	 * @sa 
	 */
	class IJuneMixer
	{
	public:
		/**
		 *  @brief �������ƶ�����Ƶ�������
		 *  @param pCanvas Ŀ�Ļ���
		 *  @param pMaterial ���ƶ���
		 *  @param pMixParam �� MIXPARAM , NORMALPAINTMIXPARAM , NORMAL_TEXT_MIXPARAM 
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL Mix(IJuneCanvas* pCanvas, IMaterial* pMaterial, MIXPARAM* pMixParam) = 0;
	};
};

#endif