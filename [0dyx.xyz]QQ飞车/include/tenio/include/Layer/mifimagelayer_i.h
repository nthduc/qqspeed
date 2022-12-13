#ifndef __MIFIMAGELAYER_I_H__
#define __MIFIMAGELAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"

namespace Tenio
{
	class IMifImageLayer;
	class IAnimateImageEvent;

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

	TENIO_MAKE_TYPEID_MAP(IMifImageLayer, COMPONENTID_MIFIMAGELAYER);
	TENIO_MAKE_EVENT_MAP(IMifImageLayer, IAnimateImageEvent);

	/**
	 * @ingroup tenuilib
	 * @brief mif 动画播放的层对象
	 *
	 * <p>
	 *     IMifImageLayer 是播放 mif 动画的层对象， 对于mif的每一帧， 使用 IImageBox 对象实现。
	 *     屏蔽了对于底层绘制引擎的处理，本身处理了动画的逻辑控制。同时触发相应的几个事件。
	 *	   支持mif动画文件外，也支持显示 IImageBox 支持的所有的所有类型图像。
	 * </p>
	 *
	 * @see IJuneLayer, IComponent
	 */
	
	class IMifImageLayer : 
		public IJuneLayer,
		public IEventProducer,
		public IComponent
	{
	public:
		virtual BOOL DrawFrameAt(IJuneCanvas* pCanvas, int x, int y, int nFrameIndex) = 0;	
		/**
		 * @brief 装载图像文件
		 * @param lpszFileName [in] 文件名
		 * @return 装载成功，返回TRUE，否则返回FALSE
		 */
		virtual BOOL LoadImage(LPCTSTR lpszFileName) = 0;

		virtual void UnloadImage() = 0;

		/**
		 * @brief 开始播放
		 * @param bReply [in] 循环播放
		 */
		virtual void StartPlay(BOOL bReply = FALSE) = 0;


		/**
		 * @brief 停住播放
		 */
		virtual void StopPlay() = 0;

		/**
		 * @brief 获取mif的帧总数
		 */
		virtual int GetFrameCount() = 0;

		/**
		 * @brief 获取mif的动画的当前帧
		 */
		virtual int GetCurrentFrame() = 0;


		/**
		 * @brief 获取该帧的延迟等待时间
		 * @param nIndex [in] 帧码
		 */
		virtual int GetFrameDelay(int nIndex) = 0;

		virtual void GetExtInfo(MIF_FILE_HEADER_EXT* pExtInfo) = 0;
		
		virtual UINT* GetFrameDelay() = 0;
	};

	/**
	 * @ingroup tenuilib
	 * @brief IMifImageLayer 的事件回调接口
	 *
	 * <p>
	 *     IMifImageLayer 的事件回调接口。
	 * </p>
	 *
	 * @see IMifImageLayer, IEvent, IComponent
	 */
	class IAnimateImageEvent : public IEvent
	{
	public:
		/**
		 * @brief 帧事件
		 */
		virtual void OnFrameChange(LONG nFrame, LPVOID lpFrom) = 0;
		
		/**
		 * @brief 特定帧事件，每一个动画目前支持一个特定帧， 用户可以响应这个事件做处理。这个特定帧是在mif文件中设定。
		 */
		virtual void OnFrameTrigger(LONG nFrame, LPVOID lpFrom) = 0;

		/**
		 * @brief 动画启动
		 */
		virtual void OnAnimateStart() = 0;

		/**
		 * @brief 动画结束
		 */
		virtual void OnAnimateEnd() = 0;

	};
};

#endif

// InterfaceName:MifImageLayer
// SubFolderName:Layer
