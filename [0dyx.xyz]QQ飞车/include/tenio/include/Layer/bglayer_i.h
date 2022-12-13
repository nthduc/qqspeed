#ifndef __BGLAYER_I_H__
#define __BGLAYER_I_H__

#include "layer_i.h"
#include "../uilib/variableparam_i.h"
#include "bgdefine.h"

namespace Tenio
{
	class IBGLayer : public IJuneLayer
	{
	public:
		virtual ENMBGMODE GetBGLayerMode() = 0;
		// 如果属性有改动则返回true，反之返回false
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
		virtual void SetSize(int cx, int cy) = 0;
		virtual HRGN GetBGLayerRgn() = 0;
		// 以下当背景层中图片为多帧图片时使用
		virtual int GetFrameNum() = 0;
		virtual int GetFrameIndexbyDelayInfo(UINT nDelay) = 0;
		virtual int SetCurrentFrame(int nCurFrame) = 0;
	};
}

#endif