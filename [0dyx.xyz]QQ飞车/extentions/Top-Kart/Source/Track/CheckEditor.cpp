#include "Top-KartPCH.h"
#include "checkeditor.h"
#include "stdio.h"
#include "NiTriShape.h"
#include "NiApplication.h"
#include "NiCollision.h"
#include "3DLinesManager.h"
#include "KartStatusManager.h"
#include "../Utility.h"
#include <fstream>
#include "../GPPoint.h"
#include "../GPNetInterCommn.h"
#include "../Platform/GlobalInterface.h"
#include "../gpeffect.h"
#include "../gpcontrol.h"
#include "Network/msgheader.h"
#include "../gpsettings.h"
#include "../soundmgr.h"

#ifdef NON_3D
extern IDisplay * g_pDisplay;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define LOG_CHECKPT
#ifdef LOG_CHECKPT
FILE* g_pFileCheckPointLog = NULL;
#endif

static char sErrTxt[4096] = "";
//todo:���ɵ��óԺ�����Ч��Ŀǰ��ֱ���趨��idֵ��Ӧͳһ�ŵ������ļ�
const UINT conStaticBoxTriggerEffectId=24;
//todo:���ɵ��ߺ��ӣ�Ŀǰ��ֱ���趨��idֵ��Ӧͳһ�ŵ������ļ�
const UINT conStaticBoxEffectId=31;

void AddErrTxt(const char * txt, ...)
{
	va_list va;
	va_start(va, txt);
	_vsnprintf(sErrTxt, 4096,txt, va);
	va_end(va);
}

void ClearErrTxt()
{
	sErrTxt[0] = 0;
}

const char * GetLastErrTxt()
{
	return sErrTxt;
}

#define RADIUS 150.f
#define SQ_RADIUS (RADIUS * RADIUS)
#define MAX_DISTANCE 9999.f

CCheckPoint::CCheckPoint(const NiPoint3 & kPt, 
						 const NiMatrix3 & kRot, 
						 const NiFixedString name)
: kPoint(kPt)
, kRotate(kRot)
, kName(name)
, fRadius(RADIUS)
, fDistance(MAX_DISTANCE)
, m_bHiLight(false)
{
}

CCheckPoint::~CCheckPoint()
{
	unsigned int uiNum = arrLinkFrom.GetSize();
	for (unsigned int i = 0; i < uiNum; ++i)
	{
		NiDelete arrLinkFrom.GetAt(i);
	}
	uiNum = arrLinkTo.GetSize();
	for (unsigned int i = 0; i < uiNum; ++i)
	{
		NiDelete arrLinkTo.GetAt(i);
	}
	arrLinkFrom.RemoveAll();
	arrLinkTo.RemoveAll();
}

void CCheckPoint::AddLinkTo(CCheckPoint * pCkPt)
{
	assert(pCkPt);

	if (pCkPt) 
	{
		for (unsigned int i = 0; i < arrLinkTo.GetSize(); ++i)
		{
			if (pCkPt == arrLinkTo.GetAt(i)->pkChkPt) {
				return;
			}
		}

		ChkPtPtrPointWithName * kLink = NiNew ChkPtPtrPointWithName;
		kLink->kName = pCkPt->kName;
		kLink->pkChkPt = pCkPt;

		arrLinkTo.Add(kLink);
	}
	else
	{
		AddErrTxt("CCheckPoint::AddLinkTo, Null CheckPoint Can't Be Added to the To List");
	}
}

void CCheckPoint::DelLinkTo(CCheckPoint * pCkPt)
{
	assert(pCkPt);

	if (pCkPt) 
	{
		for (size_t i = 0; i < arrLinkTo.GetSize(); ++i)
		{
			if (pCkPt == arrLinkTo.GetAt(i)->pkChkPt) 
			{
				arrLinkTo.RemoveAt(i);
				//arrLinkTo.erase(arrLinkTo.begin() + i);
				return;
			}
		}

		AddErrTxt("CCheckPoint::DelLinkTo, Link(%s) not find!", (const char *)pCkPt->kName);
	}
	else
	{
		AddErrTxt("CCheckPoint::DelLinkTo, Null Link can't be NiDeleted!");
	}
}

void CCheckPoint::AddLinkFrom(CCheckPoint * pCkPt)
{
	assert(pCkPt);

	if (pCkPt) 
	{
		for (unsigned int i = 0; i < arrLinkFrom.GetSize(); ++i)
		{
			if (pCkPt == arrLinkFrom.GetAt(i)->pkChkPt) 
			{
				return;
			}
		}

		ChkPtPtrPointWithName * kLink = NiNew ChkPtPtrPointWithName;
		kLink->kName = pCkPt->kName;
		kLink->pkChkPt = pCkPt;

		arrLinkFrom.Add(kLink);
	}
	else
	{
		AddErrTxt("CCheckPoint::AddLinkFrom, Null CheckPoint Can't Be Added to the To List");
	}
}

void CCheckPoint::DelLinkFrom(CCheckPoint * pCkPt)
{
	assert(pCkPt);

	if (pCkPt) 
	{
		for (size_t i = 0; i < arrLinkFrom.GetSize(); ++i)
		{
			if (pCkPt == arrLinkFrom.GetAt(i)->pkChkPt) 
			{
				arrLinkFrom.RemoveAt(i);
				return;
			}
		}

		AddErrTxt("CCheckPoint::DelLinkFrom, Link(%s) not find!", (const char *)pCkPt->kName);
	}
	else
	{
		AddErrTxt("CCheckPoint::DelLinkFrom, Null Link can't be NiDeleted!");
	}
}


void CCheckPoint::AddLinkFromName(const NiFixedString & name)
{
	// �������ӵ��Լ�
	if (name == kName) 
	{
		assert(0 && "CCheckPoint::AddLinkFromName,�������ӵ��Լ�");
		return;
	}

	for (unsigned int i = 0; i < arrLinkFrom.GetSize(); ++i)
	{
		// �����ظ����
		if (name == arrLinkFrom.GetAt(i)->kName) 
		{
			return;
		}
	}

	ChkPtPtrPointWithName * kLink = NiNew ChkPtPtrPointWithName;
	kLink->kName = name;
	kLink->pkChkPt = NULL;

	arrLinkFrom.Add(kLink);
}

void CCheckPoint::SetLinkFromNameLink(const NiFixedString & name, CCheckPoint * pCkPt)
{
	for (unsigned int i = 0; i < arrLinkFrom.GetSize(); ++i)
	{
		if (name == arrLinkFrom.GetAt(i)->kName) 
		{
			arrLinkFrom.GetAt(i)->pkChkPt = pCkPt;
			return;
		}
	}
}

void CCheckPoint::AddLinkToName(const NiFixedString & name)
{
	// �������ӵ��Լ�
	if (name == kName) 
	{
		assert(0 && "CCheckPoint::AddLinkToName,�������ӵ��Լ�");
		return;
	}

	for (unsigned int i = 0; i < arrLinkTo.GetSize(); ++i)
	{
		// �����ظ����
		if (name == arrLinkTo.GetAt(i)->kName) 
		{
			return;
		}
	}

	ChkPtPtrPointWithName * kLink = NiNew ChkPtPtrPointWithName;
	kLink->kName = name;
	kLink->pkChkPt = NULL;

	arrLinkTo.Add(kLink);
}

void CCheckPoint::SetLinkToNameLink(const NiFixedString & name, CCheckPoint * pCkPt)
{
	for (unsigned int i = 0; i < arrLinkTo.GetSize(); ++i)
	{
		if (name == arrLinkTo.GetAt(i)->kName) 
		{
			arrLinkTo.GetAt(i)->pkChkPt = pCkPt;
			return;
		}
	}
}

void CCheckPoint::SetDistance(float distance)
{
	if (distance < fDistance) 
	{
		fDistance = distance;
	}
}

static NiColorA kColor = NiColorA(1.0f, 0.0f, 0.0f, 1.0f);
static NiColorA kLinkColor = NiColorA(0.0f, 0.0f, 1.0f, 1.0f);
static NiColorA kHiLightColor = NiColorA(0.9f, 0.2f, 0.2f, 0.8f);
void CCheckPoint::HighLight()
{
	m_bHiLight = !m_bHiLight;
}
void CCheckPoint::Draw()
{
	C3DLinesManager * pMgr = C3DLinesManager::Get3DLinesManager();

	if (!pMgr) 
	{
		return;
	}

	//g_pDisplay->DrawText(kPoint._x + pkAnim->GetAnimWidth(), kPoint._y, kName.data(), 0xFF00FF00, 14);
	
	NiColorA kCommonColor = kColor;

	if (m_bHiLight)
		kCommonColor = kHiLightColor;

	pMgr->DrawBox(kPoint, 150.f, kCommonColor);

	for (unsigned int i = 0; i < arrLinkTo.GetSize(); ++i)
	{
		pMgr->DrawLine((kPoint ), (arrLinkTo.GetAt(i)->pkChkPt->kPoint), kCommonColor);


		NiPoint3 kPos;
		float fCoeff = 0.8f;
		kPos = kPoint * (1-fCoeff) + arrLinkTo.GetAt(i)->pkChkPt->kPoint * fCoeff;

		pMgr->DrawBox(kPos, 110.f, NiColorA(0.0f, 0.0f, 1.0f, 1.0f));
	}
	
}


CLinks::CLinks()
{
	m_kLocalTranslate = NiPoint3::ZERO;	
	m_kLocalRotate.MakeIdentity();
	m_pLinkHead = 0;
	m_bChanged = false;
	m_stRunInfo.Clear();
	ResetGoPath();
	m_kLastUptPos = NiPoint3::ZERO;
	m_iCurLapTmp=0;
	m_iBestLapTmp=0;
	m_iTotalLapTmp=0;
	m_nLastCalcTrackTi=0;
	m_seLapHits.clear();
	m_bIsFinishRace = false;

#ifdef LOG_CHECKPT
	g_pFileCheckPointLog = fopen(NiApplication::ConvertMediaFilename("CheckPoint.log"), "w+");
#endif
}

CLinks::~CLinks()
{
#ifdef LOG_CHECKPT
	if (g_pFileCheckPointLog) 
	{
		fflush(g_pFileCheckPointLog);
		fclose(g_pFileCheckPointLog);
		g_pFileCheckPointLog = NULL;
	}
#endif
}

void CLinks::Clear()
{
	m_iCurLapTmp=0;
	m_iBestLapTmp=0;
	m_iTotalLapTmp=0;
	m_stRunInfo.Clear();
	ResetGoPath();
	for (size_t i = 0; i < m_arrpCheckPoints.GetSize(); ++i)
	{
		NiDelete m_arrpCheckPoints.GetAt(i);
	}

	m_arrpCheckPoints.RemoveAll();
	//�ͷŵ��߷��õ�
	CGPPointMgr::Destroy();
}

void	CLinks::GPPointDealer(USHORT	nsGameBaseMode)
{
	_ASSERTE(nsGameBaseMode<EGBI_BASEMODE_MAX);

	switch(nsGameBaseMode)
	{
	case	EGBI_BASEMODE_RACE:
		{//��������ж�ص��ߵ�
			CGPPointMgr::CleanAllPoint();
		}
		break;
	case	EGBI_BASEMODE_ITEM:
		{//��������������ߵ�
			CStringEx strTmp,strPath;
			UINT nSceneId=CLevelManager::Get()->GetGameBeginPara()->m_nMapID;
			strPath.Format("Map/Common Map/Map0%d/",nSceneId);
			strTmp.Format("%scheckpoint.lua",strPath.c_str());
			CVarTable varPtTable("PropPointArray");
			CVar::LoadVarsFromFile(&varPtTable, NiApplication::ConvertMediaFilename(strTmp.c_str()));
			CVarArray* pSub1st,*pSub2nd;
			UINT nCnt=0;
			UINT nNum = varPtTable.GetNum();
			for (nCnt=0;nCnt<nNum;++nCnt)
			{
				strTmp.Format("Point_%d",nCnt);
				pSub1st = (CVarArray*)varPtTable.Element(strTmp.c_str());
				_ASSERTE(pSub1st);
				//set translate
				pSub2nd =(CVarArray*)pSub1st->Element("Position");
				_ASSERTE(pSub2nd);
				CGPPointMgr::Param stPaTmp;
				stPaTmp.m_fRadius=GPSettings::gpPropPointRadius;
				//todo:rotate�ݲ�����
				stPaTmp.m_kRotate.MakeIdentity();
				stPaTmp.m_kTranslate=NiPoint3(pSub2nd->Element(0)->Float(),
					pSub2nd->Element(1)->Float(),pSub2nd->Element(2)->Float()-conLoadMapModify);
				stPaTmp.m_kTranslate.z += GPSettings::gpStaticBoxFloatToGroud;
				stPaTmp.m_nType=CGPPointMgr::EM_STATIC_BOX;
				stPaTmp.m_nTypeAux=nCnt;
				stPaTmp.m_pGPReport=&m_kGPStaticBoxReport;
				//stPaTmp.m_pkBox=CGPEffectMgr::Get()->NewEffect(conStaticBoxEffectId);
				DWORD dwPropBox = CLevelManager::Get()->GetMapDesc()->dwPropBox;
				if ( dwPropBox == 0 )
				{
					dwPropBox = conStaticBoxEffectId;
				}
				stPaTmp.m_pkBox=CGPEffectMgr::Get()->NewEffect(dwPropBox);
				CGPPoint* pPropPt = CGPPointMgr::CreatePoint(&stPaTmp);
				_ASSERTE(pPropPt);
			}
		}
		break;
	default:
		{
			_ASSERTE(0);
		}
	}
}
CCheckPoint * CLinks::GetCheckPointByName(const NiFixedString & name)
{
	size_t size = m_arrpCheckPoints.GetSize();

	for (unsigned int i = 0; i < size; ++i)
	{
		if (name == m_arrpCheckPoints.GetAt(i)->kName) 
		{
			return m_arrpCheckPoints.GetAt(i);
		}	
	}

	return NULL;
}



bool CLinks::Init()
{
	m_bGotBestLapTime = false;
	m_fBestLapTime = 9999999.0f;
	ResetGoPath();
	m_enRunDir = dirClockWise;
	size_t size = m_arrpCheckPoints.GetSize();

	for (unsigned int i = 0; i < size; ++i)
	{
		CCheckPoint * pCkPt = m_arrpCheckPoints.GetAt(i);
		pCkPt->nIndex = i;
		size_t num = pCkPt->arrLinkFrom.GetSize();
		for (unsigned int j = 0; j < num; ++j)
		{
			pCkPt->SetLinkFromNameLink(pCkPt->arrLinkFrom.GetAt(j)->kName, 
				GetCheckPointByName(pCkPt->arrLinkFrom.GetAt(j)->kName));

			assert(pCkPt->arrLinkFrom.GetAt(j)->pkChkPt && "Can't Find this Check Point");
		}

		num = pCkPt->arrLinkTo.GetSize();
		for (unsigned int j = 0; j < num; ++j)
		{
			pCkPt->SetLinkToNameLink(pCkPt->arrLinkTo.GetAt(j)->kName, 
				GetCheckPointByName(pCkPt->arrLinkTo.GetAt(j)->kName));

			assert(pCkPt->arrLinkTo.GetAt(j)->pkChkPt && "Can't Find this Check Point");
		}

	}

	m_pLinkHead->SetDistance(0);


	unsigned int uiFromListNum = m_pLinkHead->arrLinkFrom.GetEffectiveSize();
	
	for (unsigned int i = 0; i < uiFromListNum; ++i)
	{
		CCheckPoint * pChkPt = m_pLinkHead->arrLinkFrom.GetAt(i)->pkChkPt;
		RecuriseCaculateDistance(pChkPt, m_pLinkHead, m_pLinkHead->kName);
	}

	unsigned int uiToListNum = m_pLinkHead->arrLinkTo.GetEffectiveSize();
	m_fSingleLapDistance = 999999.0f;
	float fTempDist = 0.0f;
	NiPoint3 kDir;
	for (unsigned int i = 0; i < uiToListNum; ++i)
	{
		CCheckPoint * pChkPtTo = m_pLinkHead->arrLinkTo.GetAt(i)->pkChkPt;
		kDir = m_pLinkHead->kPoint - pChkPtTo->kPoint;
		fTempDist = kDir.Length() + pChkPtTo->fDistance;

		if (fTempDist < m_fSingleLapDistance) 
		{
			m_fSingleLapDistance = fTempDist;
		}
	}
	AddGoPath(m_pLinkHead);
	return true;
}

void CLinks::RecuriseCaculateDistance(CCheckPoint *& pChkPt, CCheckPoint *& pFatherPt, const NiFixedString & kHeadName)
{
	if (pChkPt->kName == kHeadName) 
	{
		return;
	}

	float tempDist = 0.0f;
	NiPoint3 kDir;
	kDir = pChkPt->kPoint - pFatherPt->kPoint;
	tempDist = kDir.Length();	
	pChkPt->SetDistance(tempDist + pFatherPt->fDistance);	

	unsigned int uiFromListNum = pChkPt->arrLinkFrom.GetEffectiveSize();
    for (unsigned int i = 0; i < uiFromListNum; ++i)
	{	
		CCheckPoint * pChkPtFrom = pChkPt->arrLinkFrom.GetAt(i)->pkChkPt;

		RecuriseCaculateDistance(pChkPtFrom, pChkPt, kHeadName);
	}
}

bool CLinks::LoadLinks(const char * path)
{
	CGPPointMgr::Init();

	ResetGoPath();
	//����checkpoint
	CStringEx strTmp,strPath;
	UINT nSceneId=CLevelManager::Get()->GetGameBeginPara()->m_nMapID;
	if(nSceneId < 10)
	{
		strPath.Format("Map/Common Map/Map0%d/",nSceneId);
	}
	else
	{
		strPath.Format("Map/Common Map/Map%d/",nSceneId);
	}	
	strTmp.Format("%scheckpoint.lua",strPath.c_str());
	CVarTable varCkTable("CheckPointArray");
	CVar::LoadVarsFromFile(&varCkTable, NiApplication::ConvertMediaFilename(strTmp.c_str()));
	CVarArray* pSub1st,*pSub2nd;
	UINT nNum = varCkTable.GetNum();
	UINT nCnt=0,nSub1st=0,nTmp=0,nLinkTmp=0;
	//char caOutTmp[200]={0};
	NiPoint3	kTransGet;
	NiMatrix3	kRotateIdentity;
	kRotateIdentity.MakeIdentity();
	//���������ר�ø�ʽʱʹ��
	/*
	std::ofstream ofOut("map.txt",std::ios::out);
	ofOut<<"HeadID = 0\n"<<"CheckPointNum = "<<nNum<<"\n";
	*/
	//�������checkpoint
	for (nCnt=0;nCnt<nNum;++nCnt)
	{
		strTmp.Format("Point_%d",nCnt);
		pSub1st = (CVarArray*)varCkTable.Element(strTmp.c_str());
		_ASSERTE(pSub1st);
		pSub2nd =(CVarArray*)pSub1st->Element("Position");
		_ASSERTE(pSub2nd);
		kTransGet.x=pSub2nd->Element(0)->Float();
		kTransGet.y=pSub2nd->Element(1)->Float();
		kTransGet.z=pSub2nd->Element(2)->Float()-conLoadMapModify;
		/*if (nCnt < 10)
		{
			strTmp.Format("checkpoint0%d", nCnt);
		}
		else
		{
			strTmp.Format("checkpoint%d", nCnt);
		}*/
		strTmp.Format("checkpoint%d", nCnt);
		CCheckPoint* pCkPt  =NiNew CCheckPoint(kTransGet, kRotateIdentity, strTmp.c_str());

		//���������ר�ø�ʽʱʹ��
		/*
		ofOut<<nCnt<<" "<<strTmp<<" {"<<kTransGet.x<<","<<kTransGet.y<<","<< \
			kTransGet.z<<"} {";
		*/
		//��¼��һ���˵�
		if (0==nCnt)
		{
			m_pLinkHead = pCkPt;
			//m_pLinkPre = m_pLinkHead;
			//m_pLastRightCheckPoint = m_pLinkHead;
		}
		//����from��
		pSub2nd =(CVarArray*)pSub1st->Element("FromPoint");
		_ASSERTE(pSub2nd);
		nTmp=pSub2nd->GetNum();
		for (nSub1st=0;nSub1st<nTmp;++nSub1st)
		{
			//���������ר�ø�ʽʱʹ��
			/*
			if(nSub1st>0)
			{	
				ofOut<<",";
			}
			*/
			//
			nLinkTmp=pSub2nd->Element(nSub1st)->Integer();
			/*if (nLinkTmp < 10)
			{
				strTmp.Format("checkpoint0%d", nLinkTmp);
			}
			else
			{
				strTmp.Format("checkpoint%d", nLinkTmp);
			}*/
			strTmp.Format("checkpoint%d", nLinkTmp);
			pCkPt->AddLinkFromName(strTmp.c_str());
			//���������ר�ø�ʽʱʹ��
			/*
			ofOut<<nLinkTmp;
			*/
		}
		//���������ר�ø�ʽʱʹ��
		/*
		ofOut<<"} {";
		*/
		//����to��
		pSub2nd =(CVarArray*)pSub1st->Element("ToPoint");
		_ASSERTE(pSub2nd);
		nTmp=pSub2nd->GetNum();
		for (nSub1st=0;nSub1st<nTmp;++nSub1st)
		{
			//���������ר�ø�ʽʱʹ��
			/*
			if(nSub1st>0)
			{	
				ofOut<<",";
			}
			*/
			//
			nLinkTmp=pSub2nd->Element(nSub1st)->Integer();
			/*if (nLinkTmp < 10)
			{
				strTmp.Format("checkpoint0%d", nLinkTmp);
			}
			else
			{
				strTmp.Format("checkpoint%d", nLinkTmp);
			}*/
			strTmp.Format("checkpoint%d", nLinkTmp);
			pCkPt->AddLinkToName(strTmp.c_str());
			//���������ר�ø�ʽʱʹ��
			/*
			ofOut<<nLinkTmp;
			*/
		}
		//���������ר�ø�ʽʱʹ��
		/*
		ofOut<<"}\n";
		*/
		//��ȡ�õ�ĸ�������
		pSub2nd =(CVarArray*)pSub1st->Element("Property");
		_ASSERTE(pSub2nd);
		if(pSub2nd->Element("radius"))
		{
			//���������ר�ø�ʽʱʹ��
			/*
			if(nSub1st>0)
			{	
			ofOut<<",";
			}
			*/
			//checkpoint��Ч����ĸ߶�ֵ��������checkpoint�������ж�·��
			CStringEx strNum=pSub2nd->Element("radius")->String();
			pCkPt->fTrackHeight=atof(strNum.c_str());
			//���������ר�ø�ʽʱʹ��
			/*
			ofOut<<nLinkTmp;
			*/
		}
		//���������ר�ø�ʽʱʹ��
		/*
		ofOut<<"}\n";
		*/
		//��ӵ�checkpoint��
		m_arrpCheckPoints.Add(pCkPt);
	}
	//���������ר�ø�ʽʱʹ��
	/*
	ofOut.close();
	*/


	//����start point
	CVarTable varStTable("StartPointArray");
	strTmp.Format("%scheckpoint.lua",strPath.c_str());
	CVar::LoadVarsFromFile(&varStTable, NiApplication::ConvertMediaFilename(strTmp.c_str()));
	nNum = varStTable.GetNum();
	for (nCnt=0;nCnt<nNum;++nCnt)
	{
		m_arrStartPtTransforms[nCnt].m_fScale	= 1.0f;
		strTmp.Format("Point_%d",nCnt);
		pSub1st = (CVarArray*)varStTable.Element(strTmp.c_str());
		_ASSERTE(pSub1st);
		//set rotation
		pSub2nd =(CVarArray*)pSub1st->Element("Rotation");
		_ASSERTE(pSub2nd);
		m_arrStartPtTransforms[nCnt].m_Rotate.SetRow(0,pSub2nd->Element(0)->Float(),
			pSub2nd->Element(1)->Float(),pSub2nd->Element(2)->Float());
		m_arrStartPtTransforms[nCnt].m_Rotate.SetRow(1,pSub2nd->Element(3)->Float(),
			pSub2nd->Element(4)->Float(),pSub2nd->Element(5)->Float());
		m_arrStartPtTransforms[nCnt].m_Rotate.SetRow(2,pSub2nd->Element(6)->Float(),
			pSub2nd->Element(7)->Float(),pSub2nd->Element(8)->Float());
		//set translate
		
		m_arrStartPtTransforms[nCnt].m_Rotate = m_arrStartPtTransforms[nCnt].m_Rotate;

		pSub2nd =(CVarArray*)pSub1st->Element("Position");
		_ASSERTE(pSub2nd);
		m_arrStartPtTransforms[nCnt].m_Translate=NiPoint3(pSub2nd->Element(0)->Float(),
			pSub2nd->Element(1)->Float(),pSub2nd->Element(2)->Float()-conLoadMapModify);
	}

	//����property point
	GPPointDealer(NxSceneMgr::Get()->GetCurGameModeBase());
	/*
	if (EGBI_BASEMODE_ITEM==NxSceneMgr::Get()->GetCurGameModeBase())
	{//todo:�����������룬��������ͨ������id���жϱ���ģʽ�ķ�����Ҫ���¹滮
		CVarTable varPtTable("PropPointArray");
		CVar::LoadVarsFromFile(&varPtTable, NiApplication::ConvertMediaFilename("Map/Common Map/Map02/checkpoint.lua"));
		nNum = varPtTable.GetNum();
		for (nCnt=0;nCnt<nNum;++nCnt)
		{
			sprintf(caOutTmp,"Point_%d",nCnt);
			pSub1st = (CVarArray*)varPtTable.Element(caOutTmp);
			_ASSERTE(pSub1st);
			//set translate
			pSub2nd =(CVarArray*)pSub1st->Element("Position");
			_ASSERTE(pSub2nd);
			CGPPoint::Param stPaTmp;
			stPaTmp.m_fRadius=GPSettings::gpPropPointRadius;
			//todo:rotate�ݲ�����
			stPaTmp.m_kRotate.MakeIdentity();
			stPaTmp.m_kTranslate=NiPoint3(pSub2nd->Element(0)->Float(),
				pSub2nd->Element(1)->Float(),pSub2nd->Element(2)->Float()-conLoadMapModify);
			stPaTmp.m_nType=CGPPoint::EM_STATIC_BOX;
			stPaTmp.m_nTypeAux=nCnt;
			stPaTmp.m_pGPReport=&m_kGPStaticBoxReport;
			stPaTmp.m_pkBox=NULL;
			CGPPoint* pPropPt = CGPPoint::CreatePoint(&stPaTmp);
			_ASSERTE(pPropPt);
		}
	}
	*/

/*����startpoint lua�����������,ԭ��������vss��һ�汾Ϊ���� pighuang 20070117
	NiNodePtr spNodeStartPoint = (NiNode*)(spNodeHead->GetObjectByName("start"));
	if (spNodeStartPoint) 
	{
		unsigned int uiCount = spNodeStartPoint->GetChildCount();
		assert(uiCount == MAX_PLAYER_NUM);

		std::ofstream ofOut("_startpoint.lua",std::ios::out);
		ofOut<<"StartPointArray=\n{";

		for (unsigned int i = 0; i < uiCount; ++i)
		{
			NiAVObject * pObj = spNodeStartPoint->GetAt(i);

			assert(pObj);
			if (pObj) 
			{
				m_arrStartPtTransforms[i].m_fScale		= 1.0f;
				m_arrStartPtTransforms[i].m_Rotate		= pObj->GetRotate();
				m_arrStartPtTransforms[i].m_Translate	= pObj->GetTranslate();
			}
			ofOut<<"\n\tPoint_"<<i<<"=\n\t{\n\t\t";
			//�������
			ofOut<<"Position={"<<m_arrStartPtTransforms[i].m_Translate.x<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Translate.y<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Translate.z<<"},\n\t\t";
			ofOut<<"Rotation=\n\t\t{\n\t\t\t";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(0,0)<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(0,1)<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(0,2)<<",\n\t\t\t";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(1,0)<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(1,1)<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(1,2)<<",\n\t\t\t";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(2,0)<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(2,1)<<",";
			ofOut<<m_arrStartPtTransforms[i].m_Rotate.GetEntry(2,2)<<",\n\t\t},";
			ofOut<<"\n\t},";
		}

		ofOut<<"\n}";
		ofOut.close();

	}
*/
	Init();

	return true;
}

const NiPoint3 & CLinks::GetTranslation()
{
	return m_kLocalTranslate;
}

const NiMatrix3 & CLinks::GetRotate()
{
	return m_kLocalRotate;
}

void CLinks::SetTranslation(const NiPoint3 & kPoint)
{
	if (kPoint != m_kLocalTranslate)
	{
		m_bChanged = true;
		m_kLocalTranslate = - m_kLocalTranslate;
		ApplyChange();
		m_kLocalTranslate = kPoint;
	}	
}

void CLinks::SetRotate(const NiMatrix3 & kMatrix)
{
	if (kMatrix != m_kLocalRotate)
	{
		m_bChanged = true;
		m_kLocalRotate = m_kLocalRotate.Inverse();
		ApplyChange();
		m_kLocalRotate = kMatrix;
	}
}

void CLinks::ApplyChange()
{
	int num = m_arrpCheckPoints.GetSize();
	CCheckPoint * pChkPt = NULL;
	for (int i = 0; i < num; ++i)
	{
		pChkPt = m_arrpCheckPoints.GetAt(i);
		pChkPt->kPoint = m_kLocalTranslate + m_kLocalRotate * pChkPt->kPoint;
	}
}

//����Ƕ�Ϊ����ֵ
inline float CalcAngle(const NiPoint3 & v0, const NiPoint3 & v1, const NiPoint3 & v2)
{
	NiPoint3 p1 = (v1 - v0);
	NiPoint3 p2 = (v2 - v0);
	
	NiPoint3::UnitizeVector(p1);
	NiPoint3::UnitizeVector(p2);	

	return p1 * p2;
}
//����Ƕ�Ϊ����ֵ
inline float CalcAngle(NiPoint3 & line1, NiPoint3 & line2)
{
	NiPoint3 p1 = line1;
	NiPoint3 p2 = line2;

	NiPoint3::UnitizeVector(p1);
	NiPoint3::UnitizeVector(p2);	

	return p1 * p2;
}

inline float GetDist(const NiPoint3 & p1, const NiPoint3 & p2)
{
	NiPoint3 p = p1 - p2;

	return NiSqrt(p.SqrLength());
}

///�㵽ֱ�ߵľ���
inline float GetPtToLineDist(const NiPoint3 & pObj,const NiPoint3 & p1, const NiPoint3 & p2)
{
	float fTmp=CalcAngle(pObj-p1,p2-p1);
	if (fabsf(fTmp)>0.999999f)	//С��0.08��
	{//��ֹ�Ƕȹ�����ֵ����
		fTmp=0.0f;
	}
	else
	{
		fTmp=sin(acos(fTmp));
	}
	return GetDist(pObj,p1)*fTmp;
}

///�㵽ֱ�ߵĽ���
inline NiPoint3 GetPtCrossLine(const NiPoint3 & pObj,const NiPoint3 & p1, const NiPoint3 & p2)
{
	float fTmp=CalcAngle(pObj-p1,p2-p1);
	float fLen=0.0f;
	NiPoint3 kResPt;
	if (fabsf(fTmp)>0.999999f)//С��0.08��
	{//��ֹ�Ƕȹ�����ֵ����
		kResPt=pObj;
	}
	else
	{
		fTmp=cos(acos(fTmp));
		fTmp=GetDist(pObj,p1)*fTmp;
		kResPt=p2-p1;
		NiPoint3::UnitizeVector(kResPt);
		kResPt *= fTmp;
		kResPt = p1 + kResPt;
	}
	return kResPt; //GetDist(pObj,p1)*fTmp;
}

#define MIN_ANGLE 0.8F
#define LEN_TOLERANCE 1.0f
void CLinks::UpdatePos(const NiPoint3 & pos)
{
	//���¾�̬���ߺУ����Ե������¼���
	if (EGBI_BASEMODE_ITEM==NxSceneMgr::Get()->GetCurGameModeBase())
	{//������ģʽ�Ŵ���
		CGPPointMgr::UpdateStaticBox();
	}
	//
	//if (!m_pLinkPre) 
	//{
	//	assert(0);
	//	return;
	//}

	UINT nNowTi=::GetTickCount();
	if (GetDist(pos,m_kLastUptPos)>0.2f || (nNowTi-m_nLastCalcTrackTi)>1500)
	{
		CLinks::PtPairUn insRec;
		bool bResetFlag=false;
		GetPtInTrack(pos,&m_kLastPair,&insRec);
		m_kLastUptPos=pos;
		m_nLastCalcTrackTi=nNowTi;
		if(m_kLastPair.pPtFrom && insRec.pPtFrom && (m_kLastPair.pPtFrom!=insRec.pPtFrom || m_kLastPair.pPtTo!=insRec.pPtTo))
		{//��Ҫ����켣
			XTrace("-----LastPairPoint %d to %d-----",m_kLastPair.pPtFrom->nIndex,m_kLastPair.pPtTo->nIndex);
			XTrace("-----NextPairPoint %d to %d-----",insRec.pPtFrom->nIndex,insRec.pPtTo->nIndex);
/*
			CStringEx strkkk;
			UINT nPt0=128,nPt1=129,nPt2=127,nPt3=128;

			m_kLastPair.pPtFrom=GetCheckPointByName(strkkk.FormatPt("checkpoint%d",nPt0));
			m_kLastPair.pPtTo=GetCheckPointByName(strkkk.FormatPt("checkpoint%d",nPt1));

			insRec.pPtFrom=GetCheckPointByName(strkkk.FormatPt("checkpoint%d",nPt2));
			insRec.pPtTo=GetCheckPointByName(strkkk.FormatPt("checkpoint%d",nPt3));
*/

			GetTrackList(&m_kLastPair,&insRec,&m_kLastTrack);




			if (m_kLastTrack.nNum>2)
			{
				UINT nCntIns=1;
				//for (;nCntIns<(m_kLastTrack.nNum-1);++nCntIns)
				for (;nCntIns<m_kLastTrack.nNum;++nCntIns)
				{//��һ�㲻���������һ��Ҳ�ϱ������ϴα������һ���ظ�Ҳû���⣬�������ᴦ��

					///�������Ҫ�������棬�ᵼ�´��������㴩Խ��
					if(!m_vLinkGoPath.empty() && m_kLastTrack.arrPt[nCntIns]==m_vLinkGoPath.back())
					{
						//������
					}
					else
					{
						//����б����һ������0����Ҫ���ͣ������·��������н���
						if((nCntIns+1)==m_kLastTrack.nNum && 0==m_kLastTrack.arrPt[nCntIns]->nIndex)
						{//������
							
						}
						else
						{
							AddGoPath(m_kLastTrack.arrPt[nCntIns]);
						}
						
					}
					//�ж��Ƿ�Խ�յ�
					if(m_kLastTrack.arrPt[nCntIns]==m_pLinkHead && (nCntIns+1)<m_kLastTrack.nNum)
					{
						if (TestRightDir(m_kLastTrack.arrPt[nCntIns],m_kLastTrack.arrPt[nCntIns+1]))
						{//����Խ
							++m_iCurLapTmp;//Ȧ������1
						}
						else
						{
							--m_iCurLapTmp;//Ȧ����1
						}
						XTrace("---->>>��ǰȦ��=%d",m_iCurLapTmp);
						//������ش���
						if (m_iCurLapTmp>0 && m_iCurLapTmp < m_iTotalLapTmp &&
							m_seLapHits.find(m_iCurLapTmp)==m_seLapHits.end())
						{
							m_seLapHits.insert(m_iCurLapTmp);
							CKartStatusManager::Get()->OnTriggerLapsUp();							
							float fCurrentTime = GetMainApp()->GetCurrentTime();
							float fLapTime = fCurrentTime - m_fStartTime;
							if (m_fBestLapTime > fLapTime)
								m_fBestLapTime = fLapTime;
							m_bGotBestLapTime = true;
							StartRecord();
						} 
						else if(m_iCurLapTmp ==m_iTotalLapTmp &&
							m_seLapHits.find(m_iCurLapTmp)==m_seLapHits.end())
						{
							m_seLapHits.insert(m_iCurLapTmp);
							//�Ѿ����꣬�ȴ�����
							CKartStatusManager::Get()->OnTriggerRaceFinish(RACE_FINISH_NORMAL, 0);
							m_bIsFinishRace = true;
							float fCurrentTime = GetMainApp()->GetCurrentTime();
							float fLapTime = fCurrentTime - m_fStartTime;
							if (m_fBestLapTime > fLapTime)
								m_fBestLapTime = fLapTime;
							m_bGotBestLapTime = true;
							StartRecord();
						}
					}
				}		
			}
			else
			{//ǿ�и�λ�����checkpoint
				insRec=m_kLastPair;	///��ҪӰ��ԭ������ȷ·��
				bResetFlag=true;
			}
		}
		//
		if (insRec.pPtFrom)
		{
			m_kLastPair=insRec;
		}
		else
		{//��Ӧ�÷��ؿ�ֵ
			//_ASSERTE(0);
			bResetFlag=true;
		}
		//�ж��Ƿ���Ҫ��λ
		if (bResetFlag)
		{
			CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_WRONGWAY, NULL);
		}
	}


	//m_fRestDistance = GetDist(pos, m_pLinkPre->kPoint);
	m_fRestDistance = GetDist(pos, m_kLastPair.pPtTo->kPoint)+m_kLastPair.pPtTo->fDistance;
}

bool CLinks::IsLinksChain()
{
	size_t size = m_arrpCheckPoints.GetSize();
	for (unsigned int i = 0; i < size; ++i)
	{
		if (!m_arrpCheckPoints.GetAt(i)->arrLinkTo.GetEffectiveSize() 
			|| !m_arrpCheckPoints.GetAt(i)->arrLinkFrom.GetEffectiveSize()) 
		{
			return false;
		}		
	}

	return true;
}

int CLinks::GetCheckPointsNum()
{
	return m_arrpCheckPoints.GetSize();
}

const NiPoint3 & CLinks::GetCheckPointPos(int index)
{
	return m_arrpCheckPoints.GetAt(index)->kPoint;
}

const NiFixedString & CLinks::GetCheckPointName(int index)
{
	return m_arrpCheckPoints.GetAt(index)->kName;
}

CCheckPoint* CLinks::GetNearlyCheckPt(const NiPoint3& curPos)
{
	CCheckPoint* pRetNearPt =NULL;
	float fNearPtDist = MAX_LAP_LEN;
	float fTempDist;
	for(int i = 0; i < m_arrpCheckPoints.GetSize(); ++i)
	{
		fTempDist = GetDist(curPos, m_arrpCheckPoints.GetAt(i)->kPoint);
		if(fTempDist < fNearPtDist)
		{
			fNearPtDist	= fTempDist;
			pRetNearPt	= m_arrpCheckPoints.GetAt(i); 
		}
	}
	return pRetNearPt;
}

CCheckPoint* CLinks::GetPreChectPt(const NiPoint3& curPos)
{
	CCheckPoint* pNearPt = GetNearlyCheckPt(curPos);
	if(pNearPt)
	{
		CCheckPoint* pFrom	 = pNearPt->arrLinkFrom.GetAt(0)->pkChkPt;
		CCheckPoint* pTo     = pNearPt->arrLinkTo.GetAt(0)->pkChkPt;
		if(CalcAngle(pNearPt->kPoint, curPos, pFrom->kPoint) > CalcAngle(pNearPt->kPoint, curPos, pTo->kPoint))
		{
			return pFrom;
		}
		else
		{
			return pNearPt;
		}
	}
	return NULL;	
}


void CLinks::Draw()
{
	for (size_t i = 0; i < m_arrpCheckPoints.GetSize(); ++i)
	{
		m_arrpCheckPoints.GetAt(i)->Draw();
	}
	//
	//if (m_pLinkPre) 
	//{
	//	NiPoint3 & p1 = m_pLinkPre->kPoint;
	//	C3DLinesManager::Get3DLinesManager()->DrawBox(p1, 100.f, NiColorA(0.0f, 1.0f, 0.0f, 1.0f));	
	//}

	//if (m_pLinkPre && m_pLinkNext) 
	//{
	//	NiPoint3 & p1 = m_pLinkPre->kPoint;
	//	NiPoint3 & p2 = m_pLinkNext->kPoint;
	//	
	//	C3DLinesManager::Get3DLinesManager()->DrawBox(p1, p2, 80.f, NiColorA(0.0f, 0.0f, 1.0f, 1.0f));
	//}
}
static int g_nRightNum = 0;
void CLinks::RunInfo::AddCheckPointToCurLap(CCheckPoint * pChkPt)
{
	++g_nRightNum;
	/*
	arrChkPtPools[iCurLap].push_back(pChkPt);
#ifdef LOG_CHECKPT
	fprintf(g_pFileCheckPointLog, "Push RightPT (%s), Lap(%d)\n", (const char *)pChkPt->kName, iCurLap);	
	fprintf(g_pFileCheckPointLog, "��ȷ����� = %d\n", g_nRightNum);
	fflush(g_pFileCheckPointLog);
#endif
	*/
}

bool CLinks::RunInfo::IsPointNext(CCheckPoint * pChkPtSrc, CCheckPoint * pChkPtDest)
{
	unsigned int size = pChkPtDest->arrLinkTo.GetEffectiveSize();

	CCheckPoint * pChkPt = NULL;
	for (unsigned int i = 0; i < size; ++i)
	{
		pChkPt = pChkPtDest->arrLinkTo.GetAt(i)->pkChkPt;
		if (pChkPtSrc == pChkPt) 
		{
			return true;
		}
	}

	return false;
}

bool CLinks::RunInfo::IsPointFrom(CCheckPoint * pChkPtSrc, CCheckPoint * pChkPtDest)
{
	unsigned int size = pChkPtDest->arrLinkFrom.GetEffectiveSize();

	CCheckPoint * pChkPt = NULL;
	for (unsigned int i = 0; i < size; ++i)
	{
		pChkPt = pChkPtDest->arrLinkFrom.GetAt(i)->pkChkPt;
		if (pChkPtSrc == pChkPt) 
		{
			return true;
		}
	}

	return false;
}

void CLinks::RunInfo::AddWarningTime()
{
	uiWarningTime++;
	if (uiWarningTime == 1) 
	{
		fWarningStartTime = GetMainApp()->GetCurrentTime();
	}
}



void CLinks::RunInfo::ResetWarningTime()
{
	uiWarningTime = 0;
	fWarningStartTime = 999999999999.0f;
}


void CLinks::AddResetPoint()
{
}

void CLinks::Register(IOnEnterPoint * spEnterPoint)
{
}

void CLinks::UnRegister(IOnEnterPoint * spEnterPoint)
{
}

float CLinks::GetRestMileage()
{
	float fTempRestMileage = 0.0f;
	if((m_iTotalLapTmp - m_iCurLapTmp-1)>=0)
	{
		fTempRestMileage = (m_iTotalLapTmp - m_iCurLapTmp-1) * m_fSingleLapDistance + m_fRestDistance;
	}
	else
	{
		fTempRestMileage=0.0f;
	}
	
	return fTempRestMileage;
}

float	CLinks::GetLapTotalLen()
{
	/*
	float fTmp=GetDist(m_pLinkHead->kPoint,m_pLinkHead->arrLinkTo.GetAt(0)->pkChkPt->kPoint)+
		m_pLinkHead->arrLinkTo.GetAt(0)->pkChkPt->fDistance;
	*/
	return m_fSingleLapDistance;
}

#define CONDITION_A 1
#define CONDITION_B 2
#define CONDITION_NONE 0

bool CLinks::GetForwardPoint(NiPoint3 & kRetPos, NiPoint3 & kRetDir, const NiPoint3 & kCurPos, float fDistance, CCheckPoint * pPreChkPt)
{
	int nCondition = CONDITION_NONE;
	if (fDistance > 0.0f)		
	{
		if (dirClockWise == m_stRunInfo.enCurRightDir) 
			nCondition = CONDITION_A;
		else if (dirAntiClockWise == m_stRunInfo.enCurRightDir)
			nCondition = CONDITION_B;
		else
			nCondition = CONDITION_NONE;
	}
	else if (fDistance <= 0.0f)
	{
		if (dirClockWise == m_stRunInfo.enCurRightDir) 
			nCondition = CONDITION_B;
		else if (dirAntiClockWise == m_stRunInfo.enCurRightDir)
			nCondition = CONDITION_A;
		else
			nCondition = CONDITION_NONE;
		fDistance = -fDistance;
	}
	else
	{
		nCondition = CONDITION_NONE;
	}

	if (!pPreChkPt)
		pPreChkPt = GetPreChectPt(kCurPos);
	if (pPreChkPt)
	{
		if (CONDITION_A == nCondition)
		{
			if (pPreChkPt->arrLinkTo.GetEffectiveSize()) 
			{
				CCheckPoint * pChkPt = pPreChkPt->arrLinkTo.GetAt(0)->pkChkPt;
				float fDistance1 = GetDist(kCurPos, pChkPt->kPoint);

				if (fDistance1 >= fDistance) 
				{

					// ���������һ��CheckPoint��͵�ǰCheckPoint��֮��
					NiPoint3 kDir = pChkPt->kPoint - pPreChkPt->kPoint;
					kDir.Unitize();

					NiPoint3 projPos = (float((kCurPos - pPreChkPt->kPoint) * kDir)/(kDir * kDir))*kDir + pPreChkPt->kPoint;
					kRetPos = projPos + fDistance * kDir;
					kRetDir = kDir;
					return true;
				}
				else
				{
					// �ݹ�Ѱ�������
					return GetForwardPoint(kRetPos, kRetDir, pChkPt->kPoint, fDistance - fDistance1, pChkPt);					
				}

			}
			
		}
		else if ( CONDITION_B == nCondition )
		{
			if (pPreChkPt->arrLinkFrom.GetEffectiveSize()) 
			{
				float fDistance1 = GetDist(kCurPos, pPreChkPt->kPoint);//pChkPt->kPoint);

				if (fDistance1 >= fDistance) 
				{

					// ���������һ��CheckPoint��͵�ǰCheckPoint��֮��
					CCheckPoint * pToPt = pPreChkPt->arrLinkTo.GetAt(0)->pkChkPt;
//					NiPoint3 kDir = pChkPt->kPoint - pPreChkPt->kPoint;
					NiPoint3 kDir = pPreChkPt->kPoint - pToPt->kPoint;
					kDir.Unitize();

					NiPoint3 projPos = (float((kCurPos - pToPt->kPoint) * kDir)/(kDir * kDir))*kDir + pToPt->kPoint ;
 					kRetPos = projPos + fDistance * kDir;
					kRetDir = kDir;
					return true;
				}
				else
				{
					CCheckPoint * pChkFromPt = pPreChkPt->arrLinkFrom.GetAt(0)->pkChkPt;
					return GetForwardPoint(kRetPos, kRetDir, pPreChkPt->kPoint, fDistance - fDistance1, pChkFromPt);					
				}

			}
		}
		
	}

	return false;
}

GPStaticBox::OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData)
{
	//���ߺ���ײ����
	CGPPoint* pGpTmp=CGPPointMgr::GetPoint((USHORT)lpGPData);
	//�ȱ���(ȡ������ģ�ͼ�������Ӧ)
	if(pGpTmp->GetActive())
	{//ֻ�б�����Ҳ���ʾ��ײ��Ч
		Play* pPlay=(Play*)lpHitObjData;
		if (pPlay && pPlay->IsHost())
		{
			IGPTarget* pHost = CGPControl::Get()->GetTargetIterator()->GetHost();
			//todo:���ɵ��óԺ�����Ч��Ŀǰ��hard code��idֵ
			pHost->Attach(CGPEffectMgr::Get()->NewEffect(conStaticBoxTriggerEffectId));
			//��������֪ͨ
			CGPNetIntercommn::RequsetGetProp((USHORT)pGpTmp->GetParam()->m_nTypeAux);
			CSoundMgr::Get()->PlaySound(GPSettings::eatsound);
		}
		pGpTmp->SetActive(false);
	}
}

void CLinks::GetTrackList(PtPairUn* pLast,PtPairUn* pCurr,PtTrack* pTrack)
{//Ҫ���ݿ���һ�����������������
	UINT nCnt=0;
	RunDirection enDir=dirClockWise;		///��¼��������
	SearchTrack insSearch;	//ʹ��Ĭ�ϵ��������
	insSearch.insTrack.arrPt[0]=pLast->pPtFrom;
	insSearch.insTrack.arrPt[1]=pLast->pPtTo;
	insSearch.insTrack.nNum=2;
	insSearch.seMark.insert(insSearch.insTrack.arrPt[0]);
	insSearch.seMark.insert(insSearch.insTrack.arrPt[1]);
	insSearch.insToPair=*pCurr;
	if (!SearchDepth(&insSearch,true))
	{//����������
		insSearch.seMark.clear();
		insSearch.insTrack.arrPt[0]=pLast->pPtTo;
		insSearch.insTrack.arrPt[1]=pLast->pPtFrom;
		insSearch.insTrack.nNum=2;
		insSearch.seMark.insert(insSearch.insTrack.arrPt[0]);
		insSearch.seMark.insert(insSearch.insTrack.arrPt[1]);
		if (!SearchDepth(&insSearch,true))
		{//����û���ҵ�������ʧ��
			insSearch.insTrack.nNum=0;
			_ASSERTE(0);
		}
	}
	//
	if (pTrack->nNum<conMaxTransChkPtNum)
	{
		pTrack->arrPt[pTrack->nNum]=NULL;
	}
	memcpy(pTrack,&insSearch.insTrack,sizeof(PtTrack));
}

bool CLinks::TestRightDir(CCheckPoint* p1st,CCheckPoint* p2nd)
{
	UINT nCnt=0;
	CCheckPoint* pPt=NULL;
	bool bRes=true,bGetPt=false;
	//����To��
	UINT nLoop=p1st->arrLinkTo.GetSize();
	for (nCnt=0;false==bGetPt && nCnt<nLoop;++nCnt)
	{
		pPt=p1st->arrLinkTo.GetAt(nCnt)->pkChkPt;
		if (pPt==p2nd)
		{//�ҵ���
			bGetPt=true;
			if (dirClockWise==m_stRunInfo.enCurRightDir)
			{//����
				bRes=true;
			}
			else if (dirAntiClockWise==m_stRunInfo.enCurRightDir)
			{
				bRes=false;
			}
		}
	}
	////////////////��ʼ����From��
	nLoop=p1st->arrLinkFrom.GetSize();
	for (nCnt=0;false==bGetPt && nCnt<nLoop;++nCnt)
	{
		pPt=p1st->arrLinkFrom.GetAt(nCnt)->pkChkPt;
		if (pPt==p2nd)
		{//�ҵ���
			bGetPt=true;
			if (dirClockWise==m_stRunInfo.enCurRightDir)
			{//����
				bRes=false;
			}
			else if (dirAntiClockWise==m_stRunInfo.enCurRightDir)
			{
				bRes=true;
			}
		}
	}
	_ASSERTE(bGetPt);
	return bRes;
}

bool CLinks::SearchDepth(SearchTrack* pSearchTrack,bool bGetTrack)
{
	if (pSearchTrack->insTrack.nNum>=pSearchTrack->nSearchDepth ||
		pSearchTrack->insTrack.nNum>=conMaxTransChkPtNum)
	{//����������Ȼ�·������
		pSearchTrack->insTrack.nNum--;
		return false;
	}
	UINT nCnt=0;
	CCheckPoint* pPt=NULL;
	//����To��
	pSearchTrack->enDir=dirClockWise;
	UINT nLoop=pSearchTrack->insTrack.nNum-1;
	for (nCnt=0;nCnt<pSearchTrack->insTrack.arrPt[nLoop]->arrLinkTo.GetSize();++nCnt)
	{
		pPt=pSearchTrack->insTrack.arrPt[nLoop]->arrLinkTo.GetAt(nCnt)->pkChkPt;
		if (pSearchTrack->seMark.find(pPt)!=pSearchTrack->seMark.end())
		{//��ֹ�������
			continue;
		}
		else
		{
			pSearchTrack->seMark.insert(pPt);
		}
		if (bGetTrack && pPt==pSearchTrack->insToPair.pPtFrom || pPt==pSearchTrack->insToPair.pPtTo)
		{//�ҵ���
			pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pPt;
			pSearchTrack->insTrack.nNum++;
			//�������·��
			if(pPt==pSearchTrack->insToPair.pPtFrom)
			{
				if (pSearchTrack->seMark.find(pSearchTrack->insToPair.pPtTo)==pSearchTrack->seMark.end())
				{
					pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pSearchTrack->insToPair.pPtTo;
					pSearchTrack->insTrack.nNum++;
				}
				
			}
			else
			{
				if (pSearchTrack->seMark.find(pSearchTrack->insToPair.pPtFrom)==pSearchTrack->seMark.end())
				{
					pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pSearchTrack->insToPair.pPtFrom;
					pSearchTrack->insTrack.nNum++;
				}
			}		
			return true;
		}
		else/* if (pSearchTrack->insTrack.nNum<pSearchTrack->nSearchDepth)*/
		{//�ݹ�
			pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pPt;
			pSearchTrack->insTrack.nNum++;
			if(SearchDepth(pSearchTrack,bGetTrack))
			{
				return true;
			}
		}
	}
	////////////////��ʼ����From��
	pSearchTrack->enDir=dirAntiClockWise;
	for (nCnt=0;nCnt<pSearchTrack->insTrack.arrPt[nLoop]->arrLinkFrom.GetSize();++nCnt)
	{
		pPt=pSearchTrack->insTrack.arrPt[nLoop]->arrLinkFrom.GetAt(nCnt)->pkChkPt;
		if (pSearchTrack->seMark.find(pPt)!=pSearchTrack->seMark.end())
		{//��ֹ�������
			continue;
		}
		else
		{
			pSearchTrack->seMark.insert(pPt);
		}
		if (bGetTrack && pPt==pSearchTrack->insToPair.pPtFrom || pPt==pSearchTrack->insToPair.pPtTo)
		{//�ҵ���
			pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pPt;
			pSearchTrack->insTrack.nNum++;
			//�������·��
			if(pPt==pSearchTrack->insToPair.pPtFrom)
			{
				if (pSearchTrack->seMark.find(pSearchTrack->insToPair.pPtTo)==pSearchTrack->seMark.end())
				{
					pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pSearchTrack->insToPair.pPtTo;
					pSearchTrack->insTrack.nNum++;
				}

			}
			else
			{
				if (pSearchTrack->seMark.find(pSearchTrack->insToPair.pPtFrom)==pSearchTrack->seMark.end())
				{
					pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pSearchTrack->insToPair.pPtFrom;
					pSearchTrack->insTrack.nNum++;
				}
			}	
			return true;
		}
		else/* if (pSearchTrack->insTrack.nNum<pSearchTrack->nSearchDepth)*/
		{//�ݹ�
			pSearchTrack->insTrack.arrPt[pSearchTrack->insTrack.nNum]=pPt;
			pSearchTrack->insTrack.nNum++;
			if(SearchDepth(pSearchTrack,bGetTrack))
			{
				return true;
			}
		}
	}
	pSearchTrack->insTrack.nNum--;
	return false;
}

void CLinks::GetPtInTrack(const NiPoint3& kPos,PtPairUn* pLastPair,PtPairUn* pRecOut)
{
	//��С��ЧCheckPoint�ж�ֵ�������������ֵ�ж����ڸ�CheckPoint��Χ
	const float conMinIncludeDist=1.0f;
	//���ڸþ������ж�checkpoint��Ч���������ж�
	const float conMaxValidDist=45.0f;
    //cosֵ����λ�÷ֱ�������checkpoint������Z����нǱ�����ڱ�ֵ����Ч
	const float conMaxCrossAngleVal=-0.087;
	//����z���᷽���ϵ���Ч�߶ȣ������˸߶Ȳ������ж���Ч��Χ
	//const float conPtValidHeight=5.0f;
	CCheckPoint* pPt=NULL,*pPt2nd=NULL;
	CLinks::PtPairUn insPair;
	UINT nCnt=0,nCntIns=0,nNum=0;
	float fTmpAng=0.0f,fMaxCrossAngle=1.0f; //cos(0);
	float fTmpDist=0.0f,fMinDist=1000.0f,fTmp=0.0f;
	float fTmpLen1st=0.0f,fTmpLen2nd=0.0f;
	float fTmpZAix1st=0.0f,fTmpZAix2nd=0.0f; //cos(0);
	float fMinLastChoice=1000.0f;
	CLinks::PtPairUn insLastChoicePair;
	bool bSet=false,bSetOnce=false;
	SearchTrack insSearch;
	std::set<CCheckPoint*>::iterator itSet;
    if (!pLastPair->pPtFrom)
	{//ȫ�켣����
		nNum=m_arrpCheckPoints.GetSize();
	}
	else
	{
		insSearch.nSearchDepth=4;	//ʹ��ָ�����������
		insSearch.insTrack.arrPt[0]=pLastPair->pPtFrom;
		insSearch.insTrack.arrPt[1]=pLastPair->pPtTo;
		insSearch.insTrack.nNum=2;
		insSearch.seMark.insert(insSearch.insTrack.arrPt[0]);
		insSearch.seMark.insert(insSearch.insTrack.arrPt[1]);
		//insSearch.insToPair=;
		if (!SearchDepth(&insSearch,false))
		{//��������������Ҫ���seMark
			insSearch.insTrack.arrPt[0]=pLastPair->pPtTo;
			insSearch.insTrack.arrPt[1]=pLastPair->pPtFrom;
			insSearch.insTrack.nNum=2;
			insSearch.seMark.insert(insSearch.insTrack.arrPt[0]);
			insSearch.seMark.insert(insSearch.insTrack.arrPt[1]);
			if (!SearchDepth(&insSearch,false))
			{//�������
			}
		}
		else
		{
			_ASSERTE(0);
		}
		_ASSERTE(insSearch.seMark.size()>2);
		nNum=insSearch.seMark.size();
		itSet=insSearch.seMark.begin();
	}
	/////////////////
	//��ʼ����н�
	for (nCnt=0;nCnt<nNum;++nCnt)
	{
		if (!pLastPair->pPtFrom)
		{//ȫ�켣����
			if (dirClockWise==m_stRunInfo.enCurRightDir)
			{
				pPt=m_arrpCheckPoints.GetAt(nCnt);
			}
			else if (dirAntiClockWise==m_stRunInfo.enCurRightDir)
			{
				_ASSERTE(0);//����������ʵ��һ��������
			}
		}
		else
		{
			pPt=*itSet;
			++itSet;
		}
		//
		if (dirClockWise==m_stRunInfo.enCurRightDir)
		{
			NiPoint3 kPos1st,kPos2nd;
			for (nCntIns=0;nCntIns<pPt->arrLinkTo.GetSize();++nCntIns)
			{
				bSet=false;
				pPt2nd=pPt->arrLinkTo.GetAt(nCntIns)->pkChkPt;
				fTmpDist=GetDist(pPt->kPoint,kPos);
				kPos1st=kPos-pPt->kPoint;
				fTmpLen1st=NiSqrt(kPos1st.SqrLength());
				NiPoint3::UnitizeVector(kPos1st);
				kPos2nd=pPt2nd->kPoint-pPt->kPoint;///tmp use
				NiPoint3::UnitizeVector(kPos2nd);
				fTmp=kPos2nd.Dot(kPos1st);
				if(fTmpDist<conMinIncludeDist)
				{
					bSet=true;
				}
				else if(fTmp<0.0f)
				{//����kPos����������������checkpoint�鲻�ж�
					continue;
				}
				else if(fTmpDist>conMaxValidDist)
				{//����̫Զ�ĵ�������ж�
					if (fTmpDist<fMinLastChoice)
					{//ȷ�����ʼ���޷��ҵ����ʵ�ʱʹ����������
						fMinLastChoice=fTmpDist;
						insLastChoicePair.pPtFrom=pPt;
						insLastChoicePair.pPtTo=pPt2nd;
					}
					continue;
				}
				kPos2nd=kPos-pPt2nd->kPoint;
				fTmpLen2nd=NiSqrt(kPos2nd.SqrLength());
				NiPoint3::UnitizeVector(kPos2nd);
				fTmpZAix1st=kPos1st.Dot(NiPoint3(0.0f,0.0f,1.0f));
				fTmpZAix2nd=kPos2nd.Dot(NiPoint3(0.0f,0.0f,1.0f));
				fTmp=min(fabs(fTmpZAix1st*fTmpLen1st),fabs(fTmpZAix2nd*fTmpLen2nd));
				//if (fTmp>conPtValidHeight)
				_ASSERTE(pPt->fTrackHeight>1.0f && pPt2nd->fTrackHeight>1.0f);
				if (fTmp>pPt->fTrackHeight || fTmp>pPt2nd->fTrackHeight)
				{//��ֱ������ߣ�������
					continue;
				}
				//
				fTmpAng=CalcAngle(pPt->kPoint-kPos,pPt2nd->kPoint-kPos);
				if(!bSetOnce && max(fTmpZAix1st,fTmpZAix2nd)>conMaxCrossAngleVal)
				{
					bSet=true;
				}
				else if (fTmpAng<fMaxCrossAngle && max(fTmpZAix1st,fTmpZAix2nd)>conMaxCrossAngleVal)
				{//��z��н�����ָ���Ƕ�
					bSet=true;
				}
				//
				if (bSet)
				{//�и���ļн�
					bSetOnce=true;
					fMaxCrossAngle=fTmpAng;
					fMinDist=fTmpDist;
					insPair.pPtFrom=pPt;
					insPair.pPtTo=pPt2nd;
				}
			}
			if (fMinDist<conMinIncludeDist)
			{//�����������
				break;
			}
		}
		else if (dirAntiClockWise==m_stRunInfo.enCurRightDir)
		{
			_ASSERTE(0);//����������ʵ��һ��������
		}
	}
	//
	_ASSERTE(insPair.pPtFrom);
	if(!insPair.pPtFrom)// && pLastPair->pPtFrom)//��ֹ�ظ��ݹ�
	{///�Ҳ���checkpoint�Σ�ȫ������
		//GetPtInTrack(kPos,&insPair,&insLastChoicePair);
		_ASSERTE(insLastChoicePair.pPtFrom);
		insPair=insLastChoicePair;
	}
	else if (0==insPair.pPtFrom->nIndex || 0==insPair.pPtTo->nIndex)
	{//����漰������ڣ���Ҫ���������߽���������ȷ���������ߵ�ǰ���
		pPt=m_arrpCheckPoints.GetAt(0);
		fTmp=GetPtToLineDist(kPos,
			m_arrStartPtTransforms[0].m_Translate,m_arrStartPtTransforms[7].m_Translate);
		NiPoint3 kCross=GetPtCrossLine(pPt->arrLinkTo.GetAt(0)->pkChkPt->kPoint,
			m_arrStartPtTransforms[0].m_Translate,m_arrStartPtTransforms[7].m_Translate);
		kCross=pPt->arrLinkTo.GetAt(0)->pkChkPt->kPoint-kCross;
		NiPoint3::UnitizeVector(kCross);
		fTmpAng=CalcAngle(kCross,
			kPos-(m_arrStartPtTransforms[0].m_Translate+(m_arrStartPtTransforms[7].m_Translate-
			m_arrStartPtTransforms[0].m_Translate)/2.0f));
		//
		if (dirClockWise==m_stRunInfo.enCurRightDir)
		{
			if (fTmp>1.0f && fTmpAng<0.0f)
			{//�������ߺ�
				insPair.pPtFrom=pPt->arrLinkFrom.GetAt(0)->pkChkPt;
				insPair.pPtTo=pPt;
			} 
			else
			{
				insPair.pPtFrom=pPt;
				insPair.pPtTo=pPt->arrLinkTo.GetAt(0)->pkChkPt;
			}
		}
		else if (dirAntiClockWise==m_stRunInfo.enCurRightDir)
		{
			_ASSERTE(0);//����������ʵ��һ��������
		}
	}
	*pRecOut=insPair;
	pRecOut->kPos=kPos;
}

void CLinks::AddGoPath(CCheckPoint* pChk)
{
	_ASSERTE(pChk);
	if(m_bCanAddPath)
	{
		m_vLinkGoPath.push_back(pChk);
		m_pLastPathPt=pChk;
	}
}

void CLinks::SimWinGoPath(void)
{
	CCheckPoint* pTmp=NULL;
	if (m_pLastPathPt)
	{
		pTmp=m_pLastPathPt;
	}
	else
	{
		pTmp=m_pLinkHead;
		AddGoPath(pTmp);
	}
	//��ʼģ��ʣ��·��checkpoint
	do 
	{
		pTmp=pTmp->arrLinkTo.GetAt(0)->pkChkPt;
		AddGoPath(pTmp);
	} while(pTmp!=m_pLinkHead);
	m_bCanAddPath=false;//ֹͣѹ��㣬����ģ��·���������������
}

bool CLinks::IsFinishRace()
{
	return m_bIsFinishRace;
}

float CLinks::GetBestLapTime()
{
	if (!m_bGotBestLapTime)
		return 0.0f;
	return m_fBestLapTime;
}

void CLinks::StartRecord()
{
	m_fStartTime = GetMainApp()->GetCurrentTime();
}