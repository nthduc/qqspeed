#ifndef __JUNECAVASUPDATE_I_H__
#define __JUNECAVASUPDATE_I_H__

#include "region_i.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief Canvas���»ص��ӿڣ����û�ʵ��
	 *
	 * <p>
	 *���� 
	 * </p>
	 *
	 * @sa 
	 */
	class IJuneCanvasUpdate
	{
	public:
		/**
		 *  @brief ֪ͨCanvas��Ҫ����
		 *  @param pRegion ֪ͨCanvas��Ҫ���µ�����
		 *  @sa 
		 */	
		virtual void OnJuneCanvasUpdate(IJuneRegion* pRegion) = 0; 
	};
};

#endif