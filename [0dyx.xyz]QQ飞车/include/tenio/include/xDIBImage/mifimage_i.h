#ifndef __IMIFIMAGE_I_H__
#define __IMIFIMAGE_I_H__

#include "../teniointerface.h"
#include "dibimage_i.h"

namespace Tenio
{
	typedef struct _mimage_header
	{
		UINT	unVersion;
		UINT	unWidth;
		UINT	unHeight;
		UINT	unAlphaInfo;
		UINT	unFrames;
	}MIF_FILE_HEADER;

	typedef struct _mimage_header_ext
	{
		UINT unLoopCount;			// 完整动画（包括子循环）循环次数 （0－无限循环，> 0 有限循环）
		UINT unLoopDelay;			// 完整动画循环间隔
		//
		UINT unHaveSubLoop;			// 是否有子循环		(0 无子循环  1 有子循环)	//在unAlphaInfo中表示//有重复
		UINT unSubLoopStartFrame;	// 子循环开始帧
		UINT unSubLoopEndFrame;		// 子循环结束帧
		UINT unSubLoopCount;		// 子循环动画循环次数 （0－无限循环，> 0 有限循环）
		UINT unSubLoopDelay;		// 子循环动画循环间隔
		UINT unTriggerFrame;		// 触发帧	---------  只对应于主循环，子循环不会触发
		char fileName01[50];		// 对应于主循环的声音文件名
		char fileName02[50];		// 对应于子循环的声音文件名	//如果以后有新的支持从任意点播放的声音模块 此字段可不用
	}MIF_FILE_HEADER_EXT;

	class IMifImage;

	TENIO_MAKE_TYPEID_MAP(IMifImage, COMPONENTID_IMIFIMAGE);

	class IMifImage : public IDIBImage
	{
	public:
		virtual void GetExtInfo(MIF_FILE_HEADER_EXT* pExtInfo) = 0;
	};
};

#endif

// InterfaceName:IMifImage
// SubFolderName:MGClock
