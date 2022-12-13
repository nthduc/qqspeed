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
	 * @brief 含xImage的Canvas
	 *
	 * <p>
	 *　　 Canvas的一种类型，含xImage。
	 * </p>
	 *
	 * @sa IJuneCanvas 
	 */

	class IXImageCanvas : public IComponent, public IJuneCanvas
	{
	public:
		/**
		 *  @brief 关联一个xImage
		 *  @param pxImage 由外部创建的pxImage组件，当pxImage为NULL时，取消关联的xImage
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL AttachXImage(IxImageLib* pxImage) = 0;

		/**
		 *  @brief 获得关联的xImage
		 *  @return 返回关联的xImage指针
		 *  @sa 
		 */		
		virtual IxImageLib* GetXImage() = 0;
	};
}; // Tenio

#endif // __XIMAGECANVAS_I_H__
