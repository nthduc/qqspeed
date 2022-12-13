#ifndef __JUNESTRUCT_H__
#define __JUNESTRUCT_H__

#include <windows.h>

namespace Tenio
{
	//! Canvas类型
	typedef enum enumCANVASFORMATID
	{
		ECI_WINDC	= 1,	 /*!< 底层为 Windows DC 的 Canvas */ 
		ECI_XIMAGE = 2,      /*!< 底层为 xImage的 Canvas */
		ECI_MGDISPLAY		 /*!< MiniGame引擎用*/ 
	}ENUMCANVASFORMATID;

	//! Material的类型
	typedef enum enumIMAGEFORMATID
	{
		EIF_CXIMAGE	= 1,		/*!< CxImage 格式的图片 */ 
		EIF_NORMALTEXT	= 2,    /*!< 普通文字 */ 
		EIF_MGIMAGE,				/*!< MiniGame引擎用的图像格式*/ 
		EIF_MIFIMAGE			/*!< mif文件外部扩展数据对象(当引擎不支持mif的时候,可以使用这个)*/ 
	}ENUMIMAGEFORMATID;

	//! 字体类型
	typedef enum enumFONTFORMATID
	{
		EFI_GDI	= 1,		/*!< 使用gdi的HFONT绘制的字体 */ 
		EFI_MINIGAME		/*!< MiniGame引擎用*/ 
	}ENUMFONTFORMATID;

	//! 混和模式
	typedef enum enumMIXMODE
	{
		EMM_NORMALPAINT = 1	 /*!< 把 Image 绘制到 Canvas 上(无特效)  */ 
	}ENUMMIXMODE;

	//! 绘制参数
	struct MIXPARAM
	{
		int nMixMode;	/*!< 取值为 enumMIXMODE */ 
	};

	typedef enum enumNORMALPAINTMIXMODE
	{
		ENM_STRETCH = 1,    /*!< 拉伸 */
		ENM_TILE           /*!< 平铺 */
	}ENUMNORMALPAINTMIXMODE;

	//! 图像绘制参数, 把 Image 内(nImageX,nImageY)开始的矩形绘制到 Canvas 的 (nCanvasX,nCanvasY)上面
	struct NORMALPAINTMIXPARAM : public MIXPARAM
	{
		ENUMNORMALPAINTMIXMODE ePaintMode; /*!< 绘制方式 */
		int nCanvasX;	/*!< 绘制到Canvas目的坐标的x值 */ 
		int nCanvasY;	/*!< 绘制到Canvas目的坐标的y值 */
		int nImageX;	/*!< 图像绘制起始x坐标 */
		int nImageY;    /*!< 图像绘制起始y坐标 */
		int nWidth;	    /*!< 绘制的矩形宽度, -1 表示取图片的默认宽度 */
		int nHeight;	/*!< 绘制的矩形高度, -1 表示取图片的默认高度 */
		unsigned char byImageAlphaSet; /*!< 图片的透明度 255 表示不透明 */
		int nDrawFrame;	/*!< 当前绘制帧的索引*/
	};

	//! 图像绘制参数，将Image平铺到canvas上
	struct STRETCHPAINTMIXPARAM : public NORMALPAINTMIXPARAM
	{
		int nImageWidth;
		int nImageHeight;
	};

	//! 字符绘制参数
	struct NORMAL_TEXT_MIXPARAM : public MIXPARAM
	{
		int iCanvasX;		    /*!< 绘制到Canvas目的坐标的x值 */
		int iCanvasY;		    /*!< 绘制到Canvas目的坐标的y值 */
		RECT rcClip;            /*!< 文本显示的剪切矩形 */
		LOGFONT LogFont;        /*!< 字体信息 */
		COLORREF colText;       /*!< 文本颜色 */
		COLORREF colSelectText; /*!< 选中文本的颜色 */
		COLORREF colSelectBk;   /*!< 选中文本的背景颜色 */
		UINT fAlignMode;        /*!< 文本对齐方式 */
		BOOL bAutoChangeLine;   /*!< 是否自动换行 */
		BYTE byTextAlphaSet;    /*!< 字体的透明度 255 表示不透明 */
		int nLineSpacing;       /*!< 行距 */ 
	};
};

#endif