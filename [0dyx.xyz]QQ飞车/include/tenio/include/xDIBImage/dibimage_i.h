#ifndef __IDIBIMAGE_I_H__
#define __IDIBIMAGE_I_H__

#include "../June/image_i.h"

namespace Tenio
{
	class IDIBImage : public IImage, public IComponent
	{
	public:
		/**
		 *  @brief 设置透明度
		 *  @param byAlphaSet alpha值，范围在0 ~ 255
		 *  @sa 
		 */	
		virtual void SetPaintAlpha(BYTE byAlphaSet) = 0;
		/**
		 *  @brief 加载图像文件
		 *  @param lpszFileName，图像文件全路径名
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL LoadImage(LPCTSTR lpszFileName) = 0;
	
		/**
		 *  @brief 从文件buffer加载图像
		 *  @param lpszFileName 图像文件名，主要是用于识别图像格式
		 *  @param pbyBuffer 图像文件数据存放的缓冲区指针
		 *  @param nSize　图像缓冲区大小
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL LoadImage(LPCTSTR lpszFileName, const BYTE* pbyBuffer, UINT nSize) = 0;		

		/**
		 *  @brief 获得与设备无关位图句柄
		 *  @return 返回DIBHandle
		 */	
		virtual HBITMAP GetDIBHandle(int nIndex) = 0;

		/**
		 *  @brief 获得与DIB buffer, 
		 *  @return 返回指针
		 */	
		virtual void* GetDIB(int nIndex) = 0;
	};
};

#endif

// InterfaceName:IMifImage
// SubFolderName:MGClock
