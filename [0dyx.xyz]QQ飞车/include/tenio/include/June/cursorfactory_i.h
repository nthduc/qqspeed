#ifndef __CURSORFACTORY_I_H__
#define __CURSORFACTORY_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class ICursorFactory;

	TENIO_MAKE_TYPEID_MAP(ICursorFactory, COMPONENTID_CURSORFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief 光标资源工厂
	 *
	 * <p>
	 *　　 	 工厂提供通用的光标句柄，Factory销毁时负责销毁所有光标句柄，外部不需要销毁
	 * </p>
	 *
	 * @sa 
	 */
	
	class ICursorFactory : public IComponent
	{
	public:
		/**
		 *  @brief 获得通用光标句柄
		 *  @param lpszCursorName，资源名称，见the Microsoft Win32? API预定义的　MSDN LoadCursor
		 *  @return 返回光标句柄
		 *  @sa 
		 */	
		virtual HCURSOR GetCommonCursor(LPCTSTR lpszCursorName) = 0;

		/**
		 *  @brief 通过ID获得光标句柄
		 *  @param dwCursorID 光标ID
		 *  @return 返回光标句柄
		 */	
		virtual HCURSOR GetCursor(DWORD dwCursorID) = 0;		
	};
};

#endif

// InterfaceName:CursorFactory
// SubFolderName:uicore
