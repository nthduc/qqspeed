#ifndef __BEHAVIORDESIGNER_I_H__
#define __BEHAVIORDESIGNER_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	typedef enum _tagBEHAVIORMODE
	{
		BHM_BEELINE,        // 直线移动
		BHM_INFLATE,        // 放大缩小
		BHM_THROB,          // 搏动
		BHM_CHILDRENSHOW    // 子控件顺序显示
	}BEHAVIORMODE;

	typedef enum _tagBEHAVIORACTION
	{
		EBA_START,          //开始
		EBA_REVERSESTART,	//反方向开始
		EBA_STOP			//停止
	}BEHAVIORACTION;

	#define TEXT_BEHAVIOR_BEELINE		_T("Beeline")
	#define TEXT_BEHAVIOR_INFLATE		_T("Inflate")
	#define TEXT_BEHAVIOR_THROB	        _T("Throb")
	#define TEXT_BEHAVIOR_CHILDRENSHOW	_T("ChildrenShow")

	class IBehaviorDesigner;

	TENIO_MAKE_TYPEID_MAP(IBehaviorDesigner, COMPONENTID_BEHAVIORDESIGNER);

	class IBehaviorDesigner : public IControl
	{
	public:
		virtual void BehaviorStart(BOOL bReverse) = 0;
		virtual void BehaviorTerminate() = 0;
	};
};

#endif // __BEHAVIORDESIGNER_I_H__