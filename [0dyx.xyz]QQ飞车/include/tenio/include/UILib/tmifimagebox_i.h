#ifndef __TMIFIMAGEBOX_I_H__
#define __TMIFIMAGEBOX_I_H__

#include "uilib/control_i.h"

namespace Tenio
{
	class ITMifImageBox;

	TENIO_MAKE_TYPEID_MAP(ITMifImageBox, COMPONENTID_TMIFIMAGEBOX);

	/**
	 * @ingroup tenuilib
	 * @brief mif 动画播放控件
	 *
	 * <p>
	 *     ITMifImageBox 是播放 mif 动画的控件， 内部使用 IMifImageLayer 实现主要功能。
	 *	   同时触发相应的几个事件。支持mif动画文件外，也支持显示 IImageBox 支持的所有的所有类型图像。
	 * </p>
	 *
	 * @see IControl
	 */
	class ITMifImageBox : public IControl
	{
	public:
		/**
		 * @brief 装载图像文件
		 * @param lpszFileName [in] 文件名
		 * @return 装载成功，返回TRUE，否则返回FALSE
		 */
		virtual BOOL LoadImage(LPCTSTR lpszFileName) = 0;

		/**
		 * @brief 开始播放
		 * @param bReply [in] 循环播放
		 */
		virtual void StartPlay(BOOL bReplay = FALSE) = 0;
	};
};

#endif

// InterfaceName:TMifImageBox
// SubFolderName:UILib
