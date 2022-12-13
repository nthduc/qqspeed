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
	 * @brief 含DC的Canvas
	 *
	 * <p>
	 *　　 Canvas的一种类型，含windows的HDC。
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
		 *  @brief 关联一个HDC
		 *  @param hDc 由外部创建的DC句柄，当hDc为NULL时，取消关联的HDC
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL AttachDc(HDC hDc) = 0;

		/**
		 *  @brief 获得关联DC
		 *  @return 返回关联的DC句柄，可以是NULL
		 *  @sa 
		 */		
		virtual HDC GetDc() = 0;
	};
};

#endif