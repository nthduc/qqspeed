#ifndef __IMAGEBOX_I_H__
#define __IMAGEBOX_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "../uilib/variableparam_i.h"

namespace Tenio
{
	class IJuneImageBox;

	TENIO_MAKE_TYPEID_MAP(IJuneImageBox, COMPONENTID_JUNEIMAGEBOX);

	typedef enum _tagIMAGEPAINTMODE
	{
		IPM_NORMAL = 1,
		IPM_STRETCH,
		IPM_TILE
	}IMAGEPAINTMODE;

	typedef enum _tagIMAGEOVERTURNMODE
	{
		IOM_NONE = 1,
		IOM_HORI,
		IOM_VERT,
		IOM_BOTH
	}IMAGEOVERTURNMODE;

	class IJuneImageBox : public IJuneLayer, public IComponent
	{
	public:
		// 从文件中读入图片, 支持 bmp jpg tga gif
		virtual BOOL LoadFromFile(LPCTSTR lpszFile) = 0;
		virtual BOOL LoadFromArray(
			BYTE* pArray,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwBitsPerPixel, 
			DWORD dwBytesPerLine, 
			BOOL bFlipImage) = 0;
		
		virtual void SetPaintMode(IMAGEPAINTMODE PaintMode, SIZE sz) = 0;
		
		/* 获得图像可见区域的rgn，
		 * 如果有alpha通道，则去除alpha值为零的部分
		 * 如果没有，则去除像素值为crMask的部分,crMask由property传入.
		 * 返回值生命周期由内部管理，外部不需要释放
		 */
		virtual HRGN GetImageRgn() = 0;

		virtual BOOL SetProperty(IVariableParam* pParam) = 0;

		/**
		 *  @brief 获得得当前绘制帧
		 *  @return 返回当前被绘制的帧的索引值
		 */	
		virtual int GetCurrentFrame() = 0;

		/**
		 *  @brief 设置当前绘制帧（如果设置值超过总帧数，则无论如何设置，绘制的都是第一帧）
		 *  @param nNewCurFrame 新的当前绘制帧
		 *  @return 返回旧的当前绘制帧
		 */		
		virtual int SetCurrentFrame(int nNewCurFrame) = 0;

		/**
		 *  @brief 获得得本图像层的帧数
		 *  @return 返回本图像层的帧数
		 */		
		virtual int GetFrameNum() = 0;

		/**
		 *  @brief 获得某帧延迟
		 *  @param nFrame　某帧序列号
		 *  @return 返回该帧延迟
		 */		
		virtual int GetFrameDelay(int nFrame) = 0;

		/**
		 *  @brief 根据帧延迟信息得到帧序列号，当延迟数有特殊含意时使用
		 *  @param nDelay 延迟信息
		 *  @return 返回帧序列号，当没有找到时返回-1
		 */		
		virtual int GetFrameIndexbyDelayInfo(UINT nDelay) = 0;

		virtual void StartAnimate() = 0;
		
		virtual void EndAnimate() = 0;
	};
};

#endif