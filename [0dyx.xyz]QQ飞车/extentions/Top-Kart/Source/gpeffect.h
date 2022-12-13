////////////////////////////////////////////////
//     FileName:    gpeffect.h
//     Author:      Fish
//     Date:        2007-1-26 10:18:48
//     Description: 封装特效接口，给道具系统使用，
//				不排除今后做小量修改用于整个客户端
////////////////////////////////////////////////

#include "gpdefine.h"
#include "NifAnimationInfo.h"
#include "singlet.h"
#include "map"
#include "string"
using namespace std;

//todo:模型池初始化时保存的模型个数
const UINT conEffectPoolInitNum=2;
/*
CGPEffectMgr:
道具特效管理器
*/
class CGPEffectMgr : public CSingleT<CGPEffectMgr>
{
public:
	CGPEffectMgr();
	virtual ~CGPEffectMgr();
	BOOL LoadEffects();
	NiNode* NewEffect(DWORD, BOOL bClone = TRUE);
	///释放占用的模型资源，重新入池，对于使用NewEffect函数克隆模式分配的模型都要使用本方法释放
		///要使用本方法释放，成功返回true
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
	//std::vector< std::list<NiNodePtr> >m_arrUnusePool;	///未使用的特效模型数据池，提高处理效率
	//std::vector< std::list<NiNodePtr> >m_arrUsingPool;	///使用中的特效模型数据池，等待释放
	//CRITICAL_SECTION critSec;
};

#include "gpaffectbase.h"

/*
CGPAffect_Effect:
对应于EAI_Effect，参加gpdefine.h中的说明
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