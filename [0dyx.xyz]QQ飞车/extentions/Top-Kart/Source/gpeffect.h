////////////////////////////////////////////////
//     FileName:    gpeffect.h
//     Author:      Fish
//     Date:        2007-1-26 10:18:48
//     Description: ��װ��Ч�ӿڣ�������ϵͳʹ�ã�
//				���ų������С���޸����������ͻ���
////////////////////////////////////////////////

#include "gpdefine.h"
#include "NifAnimationInfo.h"
#include "singlet.h"
#include "map"
#include "string"
using namespace std;

//todo:ģ�ͳس�ʼ��ʱ�����ģ�͸���
const UINT conEffectPoolInitNum=2;
/*
CGPEffectMgr:
������Ч������
*/
class CGPEffectMgr : public CSingleT<CGPEffectMgr>
{
public:
	CGPEffectMgr();
	virtual ~CGPEffectMgr();
	BOOL LoadEffects();
	NiNode* NewEffect(DWORD, BOOL bClone = TRUE);
	///�ͷ�ռ�õ�ģ����Դ��������أ�����ʹ��NewEffect������¡ģʽ�����ģ�Ͷ�Ҫʹ�ñ������ͷ�
		///Ҫʹ�ñ������ͷţ��ɹ�����true
	//bool ReleaseEffect(DWORD nEffect,NiNodePtr pkNode);

	void Update(DWORD);
	void Render(NiCamera* pCamera);

	//static NiCamera*	FindCamera(NiAVObject*);

	typedef map<DWORD, string>	EFFECT_MAP;
	//typedef map<DWORD, NiNodePtr>	EFFECT_MAP;
	//typedef map<DWORD, CNifAnimationInfo> NIFANIMATIONINFO_MAP;

private:
	//NiNodePtr _LoadEffect(const char* pszEffect);

private:
	EFFECT_MAP	m_Effects;
	//NIFANIMATIONINFO_MAP	m_AnimationInfo;
	//std::vector< std::list<NiNodePtr> >m_arrUnusePool;	///δʹ�õ���Чģ�����ݳأ���ߴ���Ч��
	//std::vector< std::list<NiNodePtr> >m_arrUsingPool;	///ʹ���е���Чģ�����ݳأ��ȴ��ͷ�
	//CRITICAL_SECTION critSec;
};

#include "gpaffectbase.h"

/*
CGPAffect_Effect:
��Ӧ��EAI_Effect���μ�gpdefine.h�е�˵��
*/
class CGPAffect_Effect : public CGPAffectBase
{
public:
	CGPAffect_Effect();
	~CGPAffect_Effect();

	//IGPAffect
	virtual void Start();
	virtual void Stop();
	virtual void Update(DWORD);

private:
	NiNodePtr	m_spNode;
	NiCamera*	m_pCamera;
};