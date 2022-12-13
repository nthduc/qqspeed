#ifndef __XIMAGECANVAS_I_H__
#define __XIMAGECANVAS_I_H__

#include "canvas_i.h"
#include "../teniointerface.h"
#include "ximagelib_i.h"

namespace Tenio
{
	class IXImageCanvas;

	TENIO_MAKE_TYPEID_MAP(IXImageCanvas, COMPONENTID_XIMAGECANVAS);
	
	/**
	 * @ingroup tenuilib
	 * @brief ��xImage��Canvas
	 *
	 * <p>
	 *���� Canvas��һ�����ͣ���xImage��
	 * </p>
	 *
	 * @sa IJuneCanvas 
	 */

	class IXImageCanvas : public IComponent, public IJuneCanvas
	{
	public:
		/**
		 *  @brief ����һ��xImage
		 *  @param pxImage ���ⲿ������pxImage�������pxImageΪNULLʱ��ȡ��������xImage
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL AttachXImage(IxImageLib* pxImage) = 0;

		/**
		 *  @brief ��ù�����xImage
		 *  @return ���ع�����xImageָ��
		 *  @sa 
		 */		
		virtual IxImageLib* GetXImage() = 0;
	};
}; // Tenio

#endif // __XIMAGECANVAS_I_H__
