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

		// ʹ�� pSceneUpdate ����ˢ������
		virtual void SetSceneUpdate(IJuneSceneUpdate* pSceneUpdate) = 0;

		// ����һ���õ�Ƭ���õ�Ƭ���е�β��, ��������õ�Ƭ
		// �����ж����е�λ��(��һ��λ��Ϊ1), �����򷵻� 0
		virtual int AppendImage(LPCTSTR lpszFileName) = 0;

		// �л���λ��Ϊ nSlidePosition �Ļõ�Ƭ
		virtual BOOL SwitchTo(int nSlidePostion) = 0;

		// ��򵥵Ķ���ģʽ
		// nStepTime  : ÿ���õ�Ƭ֮���л�ʱ��ʱ����, ��λ����
		// nLoopCount : ���������ظ����ٱ�, ��� nLoopCount = 0 ��ͣ���ظ�
		virtual void SetAutoSwitch(int nStepTime, int nLoopCount) = 0;

		// ��ʼ���Ŷ���
		virtual BOOL StartAutoSwitch() = 0;
	};
};

#endif