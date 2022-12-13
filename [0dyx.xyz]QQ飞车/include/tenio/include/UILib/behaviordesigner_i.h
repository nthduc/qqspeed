#ifndef __BEHAVIORDESIGNER_I_H__
#define __BEHAVIORDESIGNER_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	typedef enum _tagBEHAVIORMODE
	{
		BHM_BEELINE,        // ֱ���ƶ�
		BHM_INFLATE,        // �Ŵ���С
		BHM_THROB,          // ����
		BHM_CHILDRENSHOW    // �ӿؼ�˳����ʾ
	}BEHAVIORMODE;

	typedef enum _tagBEHAVIORACTION
	{
		EBA_START,          //��ʼ
		EBA_REVERSESTART,	//������ʼ
		EBA_STOP			//ֹͣ
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