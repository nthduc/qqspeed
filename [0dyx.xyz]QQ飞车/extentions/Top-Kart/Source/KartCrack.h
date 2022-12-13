#pragma once

#define MAX_TRACK_NUM 50
#include "NiSmartPointer.h"
#include <vector>
#include <list>
#include <algorithm>
//��Ӱ��Ч
class Swoosh;

class CKartCrack
{

	struct CrackAnim
	/*	: public NiRefObject ���̳�Ӧ�ö��� pighuang 20070109*/
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
		bool	bUsing;	///true=��Ч����Ԫʹ���У����ܷ���ʹ�ã�false=���Է���ʹ��
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

	
	// Add by��wincoqin, 2007/1/19, 
	// Ŀ�ģ����ƹ켣����󳤶�
	float			   m_fCrackMaxLength;		// ���ۣ��켣����󳤶ȣ���
	float			   m_fCurRightCrackLength;	// �ұ߻��۳���
	float			   m_fCurLeftCrackLength;	// ��߻��۳���
	NiPoint3		   m_kPreCrackPointLeft;    // ǰһ�����ֹ켣��λ��
	NiPoint3		   m_kPreCrackPointRight;   // ǰһ�����ֹ켣��λ��
	bool			   m_bCanCreateCrackAnim;   // �Ƿ�Ӧ�ô���Ư����Ч
	// Add end;

	float			   m_fCrackAnimLife;
	float			   m_fUpTime;
	float			   m_fUpRate;
	float			   m_fElapseTime;
	float			   m_fElapseRate;
	float			   m_fLength;
	float			   m_fWidth;
	//�����������Ư��ʱ��̥Ħ������������ͼ��Ч��
	std::vector<UINT> m_veCrackAnimId;	//������Ҫ����ľ��ͼЧ��id
	UINT m_nCurrAnimId;	//��ǰĦ���ľ��ͼid
	//�����Ӱ��Ч
	Swoosh* m_pkSwoosh;
};
