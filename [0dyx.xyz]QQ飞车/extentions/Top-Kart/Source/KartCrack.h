#pragma once

#define MAX_TRACK_NUM 50
#include "NiSmartPointer.h"
#include <vector>
#include <list>
#include <algorithm>
//残影特效
class Swoosh;

class CKartCrack
{

	struct CrackAnim
	/*	: public NiRefObject 不继承应用对象 pighuang 20070109*/
	{
		CrackAnim(NiNode * _anim, float _life)
			: spAnim(_anim), fLife(_life), fOriginLife(_life), bDeleted(false),
			bUsing(false)
		{
			assert(_anim);
			pkTriStrips = (NiTriStrips*)(_anim->GetObjectByName("Plane01"));
			assert(pkTriStrips);
		}
		//virtual ~CrackAnim()
		virtual ~CrackAnim()
		{
			NiDelete(spAnim);
			//spAnim = NULL;
		}
		NiNode* spAnim;
		NiTriStrips* pkTriStrips;
		float	fLife;
		float	fOriginLife;
		bool	bDeleted;
		bool	bUsing;	///true=本效果单元使用中，不能分配使用；false=可以分配使用
	};

	//typedef NiPointer<CrackAnim> CrackAnimPtr;
public:
	typedef enum enumShowType
	{
		ENM_SHOWJUCHI = 0,
		ENM_SHOWGUIJI,
		ENM_SHOWALL,
		ENM_SHOWNONE
	}ESHOWTYPE;

public:
	CKartCrack(void);
	~CKartCrack(void);

	void Init(NiNode* pNode);

	void Draw(NiCamera* spCamera);
	
	void AddPoints(const int& nIndex,const NiPoint3& pPos,const NiPoint3& pNormal,const NiPoint3& pDir, float driftState ,int nShowType = ENM_SHOWALL);

	void Destroy();


	void AddNewTrack();

	void Update(float fDeltaTime);

private:

	void InitG(NiTriShapePtr& pShape);
	void InitCrackAnim();
	void AddCrackAnim(const NiMatrix3 & kRotate, const NiPoint3 & kTranslate, float fLife);

	unsigned int       m_uiMaxTriangleCount;
	unsigned int       m_uiMaxVertexCount;

	NiTriShapePtr      m_spShadowGeometryRight[MAX_TRACK_NUM];
	NiTriShapePtr      m_spShadowGeometryLeft[MAX_TRACK_NUM];

	NiSourceTexturePtr m_spTexture;

	unsigned int      m_uiTrackIndex;

	/*
	NiTObjectArray<NiNodePtr> m_arrFreeCrackAnims;
	NiTPointerList<CrackAnimPtr> m_arrUsingCrackAnims;
	*/
	std::vector< std::list<CrackAnim*> > m_arrFreeCrackAnims;
	std::list<CrackAnim*> m_arrUsingCrackAnims;
	unsigned int				 m_unEffectNum;

	
	// Add by　wincoqin, 2007/1/19, 
	// 目的：控制轨迹的最大长度
	float			   m_fCrackMaxLength;		// 划痕（轨迹的最大长度），
	float			   m_fCurRightCrackLength;	// 右边划痕长度
	float			   m_fCurLeftCrackLength;	// 左边划痕长度
	NiPoint3		   m_kPreCrackPointLeft;    // 前一个左轮轨迹点位置
	NiPoint3		   m_kPreCrackPointRight;   // 前一个右轮轨迹点位置
	bool			   m_bCanCreateCrackAnim;   // 是否应该创建漂移特效
	// Add end;

	float			   m_fCrackAnimLife;
	float			   m_fUpTime;
	float			   m_fUpRate;
	float			   m_fElapseTime;
	float			   m_fElapseRate;
	float			   m_fLength;
	float			   m_fWidth;
	//用作随机处理漂移时轮胎摩擦地面产生锯齿图的效果
	std::vector<UINT> m_veCrackAnimId;	//保存需要处理的锯齿图效果id
	UINT m_nCurrAnimId;	//当前摩擦的锯齿图id
	//加入残影特效
	Swoosh* m_pkSwoosh;
};
