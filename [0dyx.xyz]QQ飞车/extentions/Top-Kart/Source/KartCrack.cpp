#include "Top-KartPCH.h"
#include ".\kartcrack.h"
#include "kernel.h"
#include "variant.h"
#include "Utility.h"
#include "Profiling.h"
#include "common.h"
#include "framework/GameApp_i.h"
#include "Platform/GlobalInterface.h"
//残影特效
#include "Swoosh.h"
#include "Play.h"
#include "NxPlayMgr.h"
#include "./PhysKart/Kart.h"

#define MAX_CRACK_ANIM 16

static NiMatrix3 kYZRotate;
CKartCrack::CKartCrack(void)
{
	m_uiMaxTriangleCount = 200;
	m_uiTrackIndex = 0;
	NiMatrix3 kYRot180;
	kYRot180.MakeYRotation(NI_PI);
	NiMatrix3 kZRot90;
	kZRot90.MakeZRotation(NI_PI/2.0f);
	kYZRotate = kYRot180 * kZRot90;
	m_unEffectNum=0;
	m_arrFreeCrackAnims.reserve(1);
	m_veCrackAnimId.reserve(1);
	m_nCurrAnimId=0;
	m_fCrackMaxLength = 10.f;
	m_fCurLeftCrackLength  = 0.0f;
	m_fCurRightCrackLength = 0.0f;
	m_bCanCreateCrackAnim = false;
	m_pkSwoosh=NULL;

}

CKartCrack::~CKartCrack(void)
{
  Destroy();
}

void CKartCrack::Init(NiNode* pNode)
{
	m_spTexture = NiSourceTexture::Create(NiApplication::ConvertMediaFilename("effect/chelun.dds"));

	for(int i = 0; i < MAX_TRACK_NUM; i++)
	{
		InitG(m_spShadowGeometryLeft[i]);
		InitG(m_spShadowGeometryRight[i]);
	}

	InitCrackAnim();

}

void CKartCrack::InitCrackAnim()
{
	CVarTable varTable("crack_anim");
	CVar::LoadVarsFromFile(&varTable, NiApplication::ConvertMediaFilename("effect/crack_anim.lua"));

	m_fCrackAnimLife = varTable.Element("anim_life")->Float();
	m_fUpTime = varTable.Element("anim_uptime")->Float();
	m_fUpRate = varTable.Element("anim_uprate")->Float();
	m_fElapseTime = varTable.Element("anim_elapse")->Float();
	m_fElapseRate = varTable.Element("anim_elapserate")->Float();
	m_fLength	= varTable.Element("anim_length")->Float();
	m_fWidth	= varTable.Element("anim_width")->Float();

	m_fCrackMaxLength = varTable.Element("anim_maxlength")->Float();

	CVarArray * pAnims = (CVarArray*)varTable.Element("anims");
	int nNum = pAnims->GetNum();

	CVar* pVar = NULL;
	NiStream kStream;
	CrackAnim* pCrkAni=NULL;
	CStringEx strTmp;
	//预留空间
	m_veCrackAnimId.resize(nNum);
	m_arrFreeCrackAnims.resize(nNum);
	for (int i = 0; i < nNum; ++i)
	{
		pVar = pAnims->Element(i);
		if (pVar && VAR_STRING == pVar->GetType()) 
		{
			strTmp.Format("effect/%s", pVar->String());
			//if (kStream.Load(NiApplication::ConvertMediaFilename(MakeString("effect/%s", pVar->String()))))
			if (kStream.Load(NiApplication::ConvertMediaFilename(strTmp.c_str())))
			{
				NiNode * pkNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
				NiNode * pkNew=NULL;
				pkNode->SetScale(0.9);
				pkNode->UpdateEffects();
				pkNode->UpdateNodeBound();
				pkNode->Update(0.0f);
				//todo:暂时处理每个效果都预留MAX_CRACK_ANIM+1个特效
				for(UINT nCntIns=0;nCntIns<=MAX_CRACK_ANIM;++nCntIns)
				{
					pkNew=NiDynamicCast(NiNode,pkNode->Clone());
					pkNew->UpdateProperties();
					pkNew->Update(GetMainApp()->GetCurrentTime());
					pCrkAni=new CrackAnim(pkNew,m_fCrackAnimLife);
					m_arrFreeCrackAnims[i].push_back(pCrkAni);
				}
				//m_arrFreeCrackAnims.Add(pkNode);
			}
		}
		//初始化随机调用摩擦纹id
		m_veCrackAnimId[i]=i;
	}
	//打乱效果排列
	std::random_shuffle(m_veCrackAnimId.begin(),m_veCrackAnimId.end());
	//m_unEffectNum = m_arrFreeCrackAnims.GetEffectiveSize();
	m_unEffectNum=(UINT)m_arrFreeCrackAnims.size();
	//运算后应该两者是相等的
	_ASSERTE(nNum==m_unEffectNum);
}

void CKartCrack::InitG(NiTriShapePtr& pShape)
{
	m_uiMaxTriangleCount = 200;

	
	//NiVertexColorProperty* pkVC = NiNew NiVertexColorProperty;
	NiMaterialProperty* pkMat = NiNew NiMaterialProperty;
	

	//pkVC->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
	//pkVC->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	
	pkMat->SetEmittance(NiColor(1.0f, 1.0f, 1.0f));
	pkMat->SetAlpha(0.8f);
	

	 
	m_uiMaxVertexCount = 3 * m_uiMaxTriangleCount;

	NiPoint3* pkVertex = NiNew NiPoint3[m_uiMaxVertexCount];
	NiPoint2* pkTC     = NiNew NiPoint2[m_uiMaxVertexCount];

	assert(pkVertex);

	unsigned short* pusConnect = (unsigned short*)NiMalloc(
		sizeof(unsigned short) * (m_uiMaxTriangleCount * 3));
	assert(pusConnect);
	
	NiTriShapeDynamicData* pkTriData = NiNew NiTriShapeDynamicData(
		m_uiMaxVertexCount, pkVertex, 0,0,pkTC, 1, 
		NiGeometryData::NBT_METHOD_NONE, m_uiMaxTriangleCount, pusConnect);
	
	pShape = NiNew NiTriShape(pkTriData);

	pShape->CreateColors(true);

	assert(pkTriData);
	assert(pShape);

	pShape->SetName("ShadowGeometry");
	pShape->SetActiveVertexCount(0);
	pShape->SetActiveTriangleCount(0);

	// The shadow is alpha blended, so it must not write the ZBuffer.
	NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
	NiZBufferProperty* pkZ = NiNew NiZBufferProperty;
	NiTexturingProperty* pkTex = NiNew NiTexturingProperty;
 
	NiTexturingProperty::Map* pkMap = NiNew NiTexturingProperty::Map(
		m_spTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T,
		NiTexturingProperty::FILTER_BILERP);

	pkTex->SetBaseMap(pkMap);
	pkTex->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
	
 

	assert(pkZ);
	pkZ->SetZBufferTest(true);
	pkZ->SetZBufferWrite(false);
	pShape->AttachProperty(pkZ);

	pAlpha->SetAlphaBlending(true);
	
	//pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_ZERO);
	//pAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_SRCCOLOR);

	pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	pAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	pShape->AttachProperty(pAlpha);

	//pShape->AttachProperty(pkVC);
	pShape->AttachProperty(pkMat);
	pShape->AttachProperty(pkTex);


	

	pShape->UpdateEffects();
	pShape->UpdateProperties();
	pShape->Update(0.0f);
}

void CKartCrack::Draw(NiCamera* spCamera)
{
 
		NiRenderer* pkRenderer = NiRenderer::GetRenderer();

		if(pkRenderer)
		{
			pkRenderer->SetCameraData(spCamera);
	
			NiAccumulatorPtr spSorter = pkRenderer->GetSorter();
			pkRenderer->SetSorter(NULL);		
	
			for(int i = 0; i < MAX_TRACK_NUM; i++)
			{
				if(m_spShadowGeometryRight[i]->GetActiveTriangleCount() >= 1)
				{
					m_spShadowGeometryRight[i]->RenderImmediate(pkRenderer);
				}

				if(m_spShadowGeometryLeft[i]->GetActiveTriangleCount() >= 1)
				{
					m_spShadowGeometryLeft[i]->RenderImmediate(pkRenderer);
				}
				
				/*
				NiTListIterator kPos = m_arrUsingCrackAnims.GetHeadPos();
				while (kPos)
				{
					CrackAnim * pkAnim = m_arrUsingCrackAnims.GetNext(kPos);
					if (pkAnim->fLife >= 0.0f)
					{
						//LevelManager::Get()->ImmRender(spCamera, pkAnim->spAnim);
						pkAnim->pkTriStrips->RenderImmediate(pkRenderer);
					}
				}
				*/
				// wincoqin将一下代码移置循环外,因为他们没有MAX_TRACK_NUM这么多份, 这样是重复绘制:) 
				// 时间: 2007/1/19
				/*
				std::list<CrackAnim*>::iterator itLi=
					m_arrUsingCrackAnims.begin();
				for (;itLi!=m_arrUsingCrackAnims.end();++itLi)
				{
					((CrackAnim*)*itLi)->pkTriStrips->RenderImmediate(pkRenderer);
				}
				*/
			}
			// 移置循环外:) 
			std::list<CrackAnim*>::iterator itLi=
				m_arrUsingCrackAnims.begin();
			for (;itLi!=m_arrUsingCrackAnims.end();++itLi)
			{
				((CrackAnim*)*itLi)->pkTriStrips->RenderImmediate(pkRenderer);
			}
			//残影
			/*
			if (m_pkSwoosh)
			{
				m_pkSwoosh->m_pkTriShape->RenderImmediate(pkRenderer);
			}
			*/
			pkRenderer->SetSorter(spSorter);
		}
		
}

const float th = 0.1f;
const float tw = 0.04f;

void CKartCrack::AddNewTrack()
{
	m_uiTrackIndex++;
	m_uiTrackIndex %= MAX_TRACK_NUM;

	m_spShadowGeometryRight[m_uiTrackIndex]->SetActiveTriangleCount(0);
	m_spShadowGeometryRight[m_uiTrackIndex]->SetActiveVertexCount(0);

	m_spShadowGeometryRight[m_uiTrackIndex]->GetModelData()->MarkAsChanged(
		NiGeometryData::VERTEX_MASK |
		NiTriBasedGeomData::TRIANGLE_INDEX_MASK |
		NiTriBasedGeomData::TRIANGLE_COUNT_MASK);


	m_spShadowGeometryRight[m_uiTrackIndex]->Update(0.0f);

	m_spShadowGeometryLeft[m_uiTrackIndex]->SetActiveTriangleCount(0);
	m_spShadowGeometryLeft[m_uiTrackIndex]->SetActiveVertexCount(0);


	m_spShadowGeometryLeft[m_uiTrackIndex]->GetModelData()->MarkAsChanged(
		NiGeometryData::VERTEX_MASK |
		NiTriBasedGeomData::TRIANGLE_INDEX_MASK |
		NiTriBasedGeomData::TRIANGLE_COUNT_MASK);


	m_spShadowGeometryLeft[m_uiTrackIndex]->Update(0.0f);
	m_fCurLeftCrackLength  = 0.0f;
	m_fCurRightCrackLength = 0.0f;
	m_bCanCreateCrackAnim = false;
}

void CKartCrack::AddPoints(const int& nIndex,const NiPoint3& pPos,const NiPoint3& pNormal,const NiPoint3& pDir, float driftState,int nShowType)
{
	const NiPoint3 pP = pPos + pNormal * 0.02f;

	// 修改人: wincoqin 
	// 时间:　2007/1/19
	// 目的：　让轨迹长度在大雨制定长度的时候终止
	if ( nShowType == ENM_SHOWNONE )
	{
		return;
	}
	
	if (__max(m_fCurLeftCrackLength, m_fCurRightCrackLength) <= m_fCrackMaxLength 
		&& (nShowType == ENM_SHOWGUIJI || nShowType == ENM_SHOWALL)) 		
	{
		NiTriShape* pShape = nIndex == 0 ? m_spShadowGeometryRight[m_uiTrackIndex] : m_spShadowGeometryLeft[m_uiTrackIndex];

		unsigned int unTriCount = pShape->GetActiveTriangleCount();
		unsigned int unVerCount = pShape->GetActiveVertexCount();

		NiPoint3* pkWorldVerts = pShape->GetVertices();
		assert(pkWorldVerts);
		float fDistance = 0.0f;

		if(unVerCount >= 2)
		{
			NiPoint3 pPre = pkWorldVerts[unVerCount - 2];
			pPre += pkWorldVerts[unVerCount - 1];
			pPre /= 2;

			pPre -= pP;
			fDistance = pPre.Length();

			m_bCanCreateCrackAnim = true;
			if(fDistance<= 0.1)
			{
				goto Exit;
			}
		}
		else
			m_bCanCreateCrackAnim = false;
		

		if(unTriCount >= m_uiMaxTriangleCount - 2)
		{
			pShape->SetActiveTriangleCount(0);
			pShape->SetActiveVertexCount(0);

			unTriCount = 0;
			unVerCount = 0;			
		}
		

		NiPoint3 pPlaneDir,pPlanePll,pPlaneNor;


		pPlaneDir = pDir - pDir.Dot(pNormal) * pNormal;
		pPlaneDir.Unitize();
		pPlaneNor = pNormal;
		pPlanePll = pPlaneDir.Cross(pPlaneNor);
		pPlanePll.Unitize();

		NiPoint3 kTrackForwardDir;
		//	修改人:	wincoqin
		//  时间:	2007/02/07
		//  目的:	 修改一个会引起崩溃的bug，对unVerCount没有作判断		
		if ( unVerCount >= 2)
		{
			kTrackForwardDir = pPos - (pkWorldVerts[unVerCount - 1] + pkWorldVerts[unVerCount - 2])/ 2.f;
			kTrackForwardDir.Unitize();
			kTrackForwardDir = kTrackForwardDir.Cross(pPlaneNor);
			pPlanePll = (pPlanePll * 0.2f + kTrackForwardDir * 0.8f);
		}				

		NiPoint3 pTrackPLeft,pTrackPRight;

		pTrackPRight = pP +  pPlanePll * tw;
		pTrackPLeft = pP - pPlanePll * tw;


		pkWorldVerts[unVerCount] = pTrackPLeft;
		pkWorldVerts[unVerCount + 1] = pTrackPRight;



		NiColorA*  pkColors = pShape->GetColors();
		NiPoint2*  pkTX = pShape->GetTextures();

		assert(pkColors);

		pkColors[unVerCount] = NiColorA(1.0f,1.0f,1.0f,1.0f);
		pkColors[unVerCount + 1] = NiColorA(1.0f,1.0f,1.0f,1.0f);

		pkTX[unVerCount] = ((unVerCount / 2) % 2) == 0 ? NiPoint2(0.0f,1.0f) : NiPoint2(1.0f,1.0f);
		pkTX[unVerCount + 1] = ((unVerCount / 2) % 2) == 0 ? NiPoint2(0.0f,0.0f) : NiPoint2(1.0f,0.0f);

		if(unVerCount >= 2)
		{
			unsigned short usBase = unTriCount * 3;
			unsigned short* pusConnect = pShape->GetTriList();
			assert(pusConnect);
			pusConnect[usBase] = unVerCount - 2;
			pusConnect[usBase + 1] = unVerCount - 1;
			pusConnect[usBase + 2] = unVerCount;

			usBase  += 3;

			pusConnect[usBase] = unVerCount - 1;
			pusConnect[usBase + 1] = unVerCount + 1;
			pusConnect[usBase + 2] = unVerCount;

			pShape->SetActiveTriangleCount(unTriCount + 2);
		}


		pShape->SetActiveVertexCount(unVerCount + 2);


		pShape->GetModelData()->MarkAsChanged(
			NiGeometryData::VERTEX_MASK | NiGeometryData::COLOR_MASK |  NiGeometryData::TEXTURE_MASK |
			NiTriBasedGeomData::TRIANGLE_INDEX_MASK |
			NiTriBasedGeomData::TRIANGLE_COUNT_MASK);


		pShape->Update(0.0f);
	}	

	// Add Drift Effect, by wincoqin
	// nIndex == 0, 右边
	// nIndex == 1, 左边

	//todo:打开车轮与地面摩狠锯齿贴图特效
	//return;
Exit:
	if (m_unEffectNum > 0 && m_bCanCreateCrackAnim 
		&& (nShowType == ENM_SHOWJUCHI || nShowType == ENM_SHOWALL))
	{		
		// 修改人：　wincoqin
		// 时间：　　2007/1/19
		// 目的：　　增加漂移轨迹长度限制做出的相应修改
		/*
		NiPoint3 kDir = pkWorldVerts[unVerCount] - pkWorldVerts[unVerCount - 2];
		*/

		NiPoint3 kDir;
		NiPoint3 kPrePoint;
		if (nIndex == 0)
			kPrePoint = m_kPreCrackPointRight;
		else
			kPrePoint = m_kPreCrackPointLeft;

		kDir = pP - kPrePoint;

		float fDistance = kDir.Unitize();
		if (nIndex == 0)
			m_fCurRightCrackLength += fDistance;
		else
			m_fCurLeftCrackLength += fDistance;
		NiPoint3 kUp = pNormal;
		kUp.Unitize();
		NiPoint3 kRight = kDir.UnitCross(kUp);
		kUp = kRight.UnitCross(kDir);

		NiMatrix3 kRotate(kDir, kUp, kRight);
		NiMatrix3 kXRot90;
		//driftState = NxMath::clamp(driftState, -0.45f * NI_PI, 0.45f * NI_PI);
		kXRot90.MakeXRotation(-NI_PI/2.0f + driftState * 0.4f);
		kRotate = kRotate * kXRot90 * kYZRotate;

		int nExtraAnimNum = (int)floor(fDistance / m_fLength + 0.5f);

		NiPoint3 kTranslate = pP - m_fUpRate * pNormal * m_fUpTime * 0.001f;
		//AddCrackAnim(kRotate, kTranslate, m_fCrackAnimLife - nExtraAnimNum * 0.05f);
		float fDelta = 0.0f;
		float fCoef  = fDistance / (nExtraAnimNum + 1.f);

		int iMin=__min(nExtraAnimNum, MAX_CRACK_ANIM);
		for (int i = 0; i <iMin ; ++i)
		{				
			AddCrackAnim(kRotate, kTranslate, m_fCrackAnimLife - i * 0.05f);			
			kTranslate -= fCoef * kDir;
		}
	
	}

	// 添加人: wincoqin
	// 时间：　2007/1/19
	// 目的：　为漂移特效记录前一个特效点位置

	if (nIndex == 0)
		m_kPreCrackPointRight = pP;
	else
		m_kPreCrackPointLeft = pP;
	
}

void CKartCrack::AddCrackAnim(const NiMatrix3 & kRotate, const NiPoint3 & kTranslate, float fLife)
{
	assert(m_unEffectNum > 0);
	int index = 0;
	CrackAnim* pCrAni=NULL;
	if(m_nCurrAnimId>=m_unEffectNum)
	{//当所有效果调用完毕则重新打乱效果排列
		std::random_shuffle(m_veCrackAnimId.begin(),m_veCrackAnimId.end());
		m_nCurrAnimId=0;
	}
	/*
	if (m_unEffectNum > 1)		
		index = NxMath::rand(0, m_unEffectNum - 1);
	NiPointer<NiNode> spNode = NiDynamicCast(NiNode, 
		m_arrFreeCrackAnims.GetAt(m_veCrackAnimId[m_nCurrAnimId])->Clone());
	assert(spNode);
	*/
	//查找一个本轮未被使用的特效，并在其中选取一个未被使用的CrackAnim Unit
	std::list<CrackAnim*>::iterator itLi=
		(m_arrFreeCrackAnims[m_veCrackAnimId[m_nCurrAnimId]]).begin();
	for (;itLi!=(m_arrFreeCrackAnims[m_veCrackAnimId[m_nCurrAnimId]]).end();++itLi)
	{
		if(!((*itLi)->bUsing))
		{
			pCrAni=*itLi;
			break;
		}
	}
	//判断是否需要增加CrackAnim Unit
	if(!pCrAni)
	{
		_ASSERTE(0);
		pCrAni=m_arrFreeCrackAnims[m_veCrackAnimId[m_nCurrAnimId]].front();
		_ASSERTE(pCrAni);
		pCrAni=new CrackAnim(NiDynamicCast(NiNode,pCrAni->spAnim->Clone()),
			pCrAni->fOriginLife);	//原始生命时间也复制
		//pCrAni->spAnim->UpdateProperties();
		pCrAni->spAnim->Update(GetMainApp()->GetCurrentTime());
		_ASSERTE(pCrAni);
		m_arrFreeCrackAnims[m_veCrackAnimId[m_nCurrAnimId]].push_back(\
			pCrAni);
	}
	//
	m_nCurrAnimId++;

	pCrAni->spAnim->SetRotate(kRotate);
	pCrAni->spAnim->SetTranslate(kTranslate);
	//pCrAni->spAnim->UpdateProperties();
	pCrAni->spAnim->Update(GetMainApp()->GetCurrentTime());
	//unsigned int uiNum = m_arrUsingCrackAnims.GetSize();
	UINT uiNum=(UINT)m_arrUsingCrackAnims.size();
	//while (uiNum > MAX_CRACK_ANIM) 
	while (uiNum >= MAX_CRACK_ANIM) 
	{
		//m_arrUsingCrackAnims.RemoveTail();
		CrackAnim* pCrTmp=m_arrUsingCrackAnims.front();
		m_arrUsingCrackAnims.pop_front();
		//把效果单元复位，以便重新使用
		pCrTmp->bUsing=false;
		--uiNum;
	}

	//m_arrUsingCrackAnims.AddHead(NiNew CrackAnim(spNode, fLife));
	//标记使用
	pCrAni->bUsing=true;
	pCrAni->fLife=fLife;
	m_arrUsingCrackAnims.push_back(pCrAni);



/*	assert(m_unEffectNum > 0);
	int index = 0;
	if (m_unEffectNum > 1)		
		index = NxMath::rand(0, m_unEffectNum - 1);

	NiNodePtr spNode = NiDynamicCast(NiNode, m_arrFreeCrackAnims.GetAt(index)->Clone());
	assert(spNode);
	

	spNode->SetRotate(kRotate);
	spNode->SetTranslate(kTranslate);

	spNode->UpdateProperties();
	spNode->Update(GetMainApp()->GetCurrentTime());
	unsigned int uiNum = m_arrUsingCrackAnims.GetSize();
	while (uiNum > MAX_CRACK_ANIM) 
	{
		m_arrUsingCrackAnims.RemoveTail();
		--uiNum;
	}
	
	m_arrUsingCrackAnims.AddHead(NiNew CrackAnim(spNode, fLife));*/	
}

void CKartCrack::Update(float fDeltaTime)
{
	PROFILE("CKartCrack::Update");
	std::list<CrackAnim*>liForDel;
	CrackAnim* pkAnim=NULL;
	std::list<CrackAnim*>::iterator itLi=
		m_arrUsingCrackAnims.begin();
	for (;itLi!=m_arrUsingCrackAnims.end();++itLi)
	{
		pkAnim=*itLi;
		//todo:DeltaTime是否参与消失计算导致贴图逐渐无法显示
		//pkAnim->fLife -= fDeltaTime * 0.001f;
		
		// 旋漂 特效开关----与是否属于飘逸状态无关
		/*
		if ( NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->GetDriftingControl()->CanAngleDrifting() ) 
		{
			pkAnim->fLife -=  pkAnim->fOriginLife*0.06f;
		}
		else*/
		{
			pkAnim->fLife -=  pkAnim->fOriginLife*0.18f;
		}

		if (pkAnim->fLife >= 0)
		{
			NiPoint3 kUp(0, 0, 1);
			NiMatrix3 kRotate = pkAnim->spAnim->GetRotate();
			kRotate.GetCol(2, kUp);
			//NiOutputDebugString(MakeString("Up = (%f, %f, %f)\n", kUp.x, kUp.y, kUp.z));
			NiPoint3 kTranslate = pkAnim->spAnim->GetTranslate();
			float fElapseRate = (pkAnim->fOriginLife - pkAnim->fLife) / pkAnim->fOriginLife;

			if (m_fUpTime > fElapseRate)
			{
				pkAnim->spAnim->SetTranslate(kTranslate + 
					(m_fUpRate * fElapseRate * pkAnim->fOriginLife) * kUp);
			}
			else if (m_fElapseTime < fElapseRate)
				pkAnim->spAnim->SetTranslate(kTranslate - 
				(m_fElapseRate * fElapseRate * pkAnim->fOriginLife) *  kUp);
			pkAnim->spAnim->UpdateSelected(0.0f);				
		}
		else
		{
			pkAnim->bDeleted = true;
			liForDel.push_back(pkAnim);
		}
	}
	//正式删除
	for (itLi=liForDel.begin();itLi!=liForDel.end();++itLi)
	{
		pkAnim=*itLi;
		m_arrUsingCrackAnims.remove(pkAnim);
		pkAnim->bUsing=false;
	}

/*修改车轮摩擦锯齿贴图时屏蔽 pighuang 20070109
	NiTListIterator kPos = m_arrUsingCrackAnims.GetHeadPos();
	NiTListIterator kPrePos;
	while (kPos)
	{
		kPrePos = kPos;
		CrackAnim * pkAnim = m_arrUsingCrackAnims.GetNext(kPos);
		pkAnim->fLife -= fDeltaTime * 0.001f;		

		if (pkAnim->fLife >= 0)
		{
			NiPoint3 kUp(0, 0, 1);
			NiMatrix3 kRotate = pkAnim->spAnim->GetRotate();
			kRotate.GetCol(2, kUp);
			//NiOutputDebugString(MakeString("Up = (%f, %f, %f)\n", kUp.x, kUp.y, kUp.z));
			NiPoint3 kTranslate = pkAnim->spAnim->GetTranslate();
			float fElapseRate = (pkAnim->fOriginLife - pkAnim->fLife) / pkAnim->fOriginLife;

			if (m_fUpTime > fElapseRate)
			{
				pkAnim->spAnim->SetTranslate(kTranslate + 
					(m_fUpRate * fElapseRate * pkAnim->fOriginLife) * kUp);
			}
			else if (m_fElapseTime < fElapseRate)
				pkAnim->spAnim->SetTranslate(kTranslate - 
				(m_fElapseRate * fElapseRate * pkAnim->fOriginLife) *  kUp);
			pkAnim->spAnim->Update(0.0f);				
		}
		else
		{
			pkAnim->bDeleted = true;
			m_arrUsingCrackAnims.RemovePos(kPrePos);
			pkAnim = NULL;
			kPos = kPrePos;
		}
	}
*/

/*
	NiTListIterator kPrePos;
	kPos = m_arrUsingCrackAnims.GetHeadPos();
	while (kPos)
	{
		kPrePos = kPos;
		CrackAnim * pkAnim = m_arrUsingCrackAnims.GetNext(kPos);
		if (pkAnim->bDeleted)
		{
			m_arrUsingCrackAnims.RemovePos(kPrePos);
			pkAnim = NULL;
			break;
		}
	}
*/	
}



void CKartCrack::Destroy()
{
	m_uiMaxTriangleCount = 0;
	m_uiMaxVertexCount = 0;
	
	for(int i = 0; i < MAX_TRACK_NUM; i++)
	{
		m_spShadowGeometryRight[i] = NULL;
		m_spShadowGeometryLeft[i] = NULL;
	}

/*修改车轮摩擦锯齿贴图时屏蔽 pighuang 20070109
	for (unsigned int i = 0; i < m_arrFreeCrackAnims.GetSize(); ++i)
	{
		NiNodePtr spNode = m_arrFreeCrackAnims.GetAt(i);
		spNode = NULL;
	}
*/
	//清空所有预留摩擦锯齿效果
	CrackAnim* pCrAni=NULL;
	for (UINT nCnt=0;nCnt<m_unEffectNum;++nCnt)
	{
		while (!m_arrFreeCrackAnims[nCnt].empty())
		{
			pCrAni=m_arrFreeCrackAnims[nCnt].front();
			m_arrFreeCrackAnims[nCnt].pop_front();
			SAFE_DELETE(pCrAni);
		}
	}
	if(m_pkSwoosh)
	{
		SAFE_NIDELETE(m_pkSwoosh);
	}
	
/*修改车轮摩擦锯齿贴图时屏蔽 pighuang 20070109
	NiTListIterator kPos = m_arrUsingCrackAnims.GetHeadPos();
	while (kPos)
	{
		CrackAnim * pkAnim = m_arrUsingCrackAnims.GetNext(kPos);
		pkAnim = 0;
	}
*/

	//!!m_arrUsingCrackAnims记录的变量无需清理，因为都在m_arrFreeCrackAnims中处理

	//m_arrFreeCrackAnims.RemoveAll();
	//m_arrUsingCrackAnims.RemoveAll();
}
