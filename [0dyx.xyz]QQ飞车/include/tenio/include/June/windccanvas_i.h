#ifndef __WINDCCANVAS_I_H__
#define __WINDCCANVAS_I_H__

#include "canvas_i.h"
#include "../teniointerface.h"

namespace Tenio
{
	class IWinDcCanvas;

	TENIO_MAKE_TYPEID_MAP(IWinDcCanvas, COMPONENTID_DCCANVAS);

	/**
	 * @ingroup tenuilib
	 * @brief ��DC��Canvas
	 *
	 * <p>
	 *���� Canvas��һ�����ͣ���windows��HDC��
	 * </p>
	 *
	 * @sa IJuneCanvas 
	 */
	
	class IWinDcCanvas :
		public IComponent,
		public IJuneCanvas
	{
	public:
		/**
		 *  @brief ����һ��HDC
		 *  @param hDc ���ⲿ������DC�������hDcΪNULLʱ��ȡ��������HDC
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL AttachDc(HDC hDc) = 0;

		/**
		 *  @brief ��ù���DC
		 *  @return ���ع�����DC�����������NULL
		 *  @sa 
		 */		
		virtual HDC GetDc() = 0;
	};
};

#endif