#ifndef __SLIDESHOW_I_H__
#define __SLIDESHOW_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "sceneupdate_i.h"

namespace Tenio
{
	class ISlideShow;

	TENIO_MAKE_TYPEID_MAP(ISlideShow, COMPONENTID_SLIDESHOW);

	class ISlideShow : public IJuneLayer, public IComponent
	{
	public:

		// 使用 pSceneUpdate 控制刷新请求
		virtual void SetSceneUpdate(IJuneSceneUpdate* pSceneUpdate) = 0;

		// 加入一个幻灯片到幻灯片序列的尾端, 返回这个幻灯片
		// 在已有队列中的位置(第一张位置为1), 错误则返回 0
		virtual int AppendImage(LPCTSTR lpszFileName) = 0;

		// 切换到位置为 nSlidePosition 的幻灯片
		virtual BOOL SwitchTo(int nSlidePostion) = 0;

		// 最简单的动画模式
		// nStepTime  : 每个幻灯片之间切换时的时间间隔, 单位毫秒
		// nLoopCount : 整个动画重复多少遍, 如果 nLoopCount = 0 则不停地重复
		virtual void SetAutoSwitch(int nStepTime, int nLoopCount) = 0;

		// 开始播放动画
		virtual BOOL StartAutoSwitch() = 0;
	};
};

#endif