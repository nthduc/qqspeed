#ifndef __ICONFACTORY_I_H__
#define __ICONFACTORY_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IIconFactory;

	TENIO_MAKE_TYPEID_MAP(IIconFactory, COMPONENTID_ICONFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief windows图标句柄工厂
	 *
	 * <p>
	 *　　 同名文件公用同一个图标句柄
	 * </p>
	 *
	 * @sa 
	 */
	class IIconFactory : public IComponent
	{
	public:
		/**
		 *  @brief 创建图标句柄
		 *  @param lpszIconFile 图标文件句，可以是全路径或相对路径，\n
		 *						如果是相对路径，则在设定的资源根路径下寻找
		 *  @return 返回图标句柄，失败则返回NULL
		 *  @sa 
		 */
		virtual HICON LoadIcon(LPCTSTR lpszIconFile) = 0;

		/**
		 *  @brief 销毁图标句柄
		 *  @param hIcon 待销毁的图标句柄
		 *  @sa 
		 */
		virtual void ReleaseIcon(HICON hIcon) = 0;

		/**
		 *  @brief 设置图标资源文件根目录，缺省是在可执行文件根目录下data/ui/
		 *  @param lpszResRootPath 资源根目录
		 *  @sa 
		 */		
		virtual void SetResRootPath(LPCTSTR lpszResRootPath) = 0;
	};
};


#endif

// InterfaceName:IconFactory
// SubFolderName:uicore
