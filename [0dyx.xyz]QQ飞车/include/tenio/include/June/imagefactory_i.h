#ifndef __IMAGEFACTORY_I_H__
#define __IMAGEFACTORY_I_H__

#include "../teniointerface.h"
#include "image_i.h"

namespace Tenio
{
	class IImageFactory;

	TENIO_MAKE_TYPEID_MAP(IImageFactory, COMPONENTID_IMAGEFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief 创建图像对象的工厂
	 *
	 * <p>
	 *　　 图像对象创建工厂，同名文件共用同一个对象指针。创建出来的对象必须仍由Factory释放
	 * </p>
	 *
	 * @sa 
	 */
	class IImageFactory : public IComponent
	{
	public:
		/**
		 *  @brief 根据文件名创建图像对象
		 *  @param lpszFileName 文件名可以是全路径名或相对路径名，\n
		 *						如果是相对路径，Factory会在指定的相对路径下寻找
		 *  @return 返回创建后的图像对象指针，如果返回NULL，则表明创建失败
		 *  @sa 
		 */	
		virtual IImage* LoadImageFile(LPCTSTR lpszFileName) = 0;

		/**
		 *  @brief 根据像素阵列创建图像对像
		 *  @param pArray 像素阵列指针
		 *  @param dwWidth	图像宽度
		 *	@param dwHeight	图像高度
		 *	@param dwBitsPerPixer 每个像素所占位数,可以是 1,4,8,24,32
		 *	@param dwBytesPerLine 行对齐，每行字节数。
		 *	@param bFlipImage 是否需要调整参数，当图像是倒置时需要。
		 *  @return 成功则返回对象指针，失败则返回NULL
		 *  @sa 
		 */
		virtual IImage* LoadImageFromArray(
			BYTE* pArray,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwBitsPerPixel, 
			DWORD dwBytesPerLine, 
			BOOL bFlipImage) = 0;

		/**
		 *  @brief 释放对象
		 *  @param pImage 待释放对象指针
		 *  @sa 
		 */	
		virtual void ReleaseImage(IImage* pImage) = 0;

		/**
		 *  @brief 设置资源的根路径，缺省是在可执行文件根目录data\ui\下
		 *  @param lpszResRootPath 资源根路径全路径名
		 */
		virtual void SetResRootPath(LPCTSTR lpszResRootPath) = 0;		

		/**
		 *  @brief 获得资源的根路径
		 *  @param lpszResRootPath 存放路径的缓冲区
		 *  @param nMaxLen lpszResRootPath的最大长度
		 */		
		virtual void GetResRootPath(LPTSTR lpszResRootPath, UINT nMaxLen) = 0;
	};

};

#endif

