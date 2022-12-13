#include "Top-KartPCH.h"
#include ".\speedline.h"

#define DEG_45        NI_PI/4.0f


CSpeedLine::CSpeedLine(int nParticlesNum, NiCamera* pCam)
{
	m_pCamera = pCam;
	InitParametres();
	Initialize();
}

CSpeedLine::~CSpeedLine(void)
{
	m_spLineShape = 0;
	//NiDelete[] m_pkTexCoords;
	//NiDelete[] m_pkVertices;

	if (m_pParticles)
	{
		delete[] m_pParticles;
		m_pParticles = NULL;
	}

	m_spLineShape = 0;
	
	m_spRoot = 0;
}

void CSpeedLine::Initialize()
{
	m_spRoot = NiNew NiNode;

	// The layout of each quad:
	//     1---0
	//     | \ |
	//     |  \|
	//     | __|
	//     2   3
	//初始化粒子数组
	m_pParticles = new SpeedParticle[m_nParticlesNum];
	for (int np = 0; np < m_nParticlesNum; np++)
	{
		m_pParticles[np].bActive = false;
	}
	
	for (int i = 0; i < 8; i++)
	{
		m_anActiveParticles[i] = 0;
	}

	//初始化粒子
	int iNumVertices = m_nParticlesNum * 4;
	int iNumTriangles = m_nParticlesNum * 2;
	//创建定点列表
	m_pkVertices = NiNew NiPoint3[iNumVertices];
	for(int i = 0; i < iNumVertices; i++)
	{
		m_pkVertices[i].x = i+1.0f;
		m_pkVertices[i].y = i+2.0f;
		m_pkVertices[i].z = i+3.0f;
	}
	//定义三角形列表
	int iTriIndices = iNumTriangles * 3;
	//unsigned short* pausTriList = new unsigned short[iTriIndices];
	unsigned short* pausTriList = NiAlloc(unsigned short, iTriIndices);
	for (int i = 0; i < m_nParticlesNum; i++)
	{
		pausTriList[6*i] = 4*i;
		pausTriList[6*i+1] = 4*i+3;
		pausTriList[6*i+2] = 4*i+1;
		pausTriList[6*i+3] = 4*i+3;
		pausTriList[6*i+4] = 4*i+2;
		pausTriList[6*i+5] = 4*i+1;
	}
	//颜色
	NiColorA* pkColor = NiNew NiColorA[iNumVertices];
	for (int i = 0; i < iNumVertices; i++)
	{
		pkColor[i] = NiColorA::WHITE;
	}
	//纹理坐标                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
	m_pkTexCoords = NiNew NiPoint2[iNumVertices];
	for (int i = 0; i < iNumVertices; i++)
	{
		//float fAngle = (float)i * NI_PI * 2.0f / nPointCount;

		switch(i%4){
			case 0:
				m_pkTexCoords[i] = NiPoint2(0.0f, 1.0f);
				break;
			case 1:
				m_pkTexCoords[i] = NiPoint2(1.0f, 1.0f);
				break;
			case 2:
				m_pkTexCoords[i] = NiPoint2(1.0f, 0.0f);
				break;
			case 3:
				m_pkTexCoords[i] = NiPoint2(0.0f, 0.0f);
				break;
		}
	}

	m_spLineShape = NiNew NiTriShape(iNumVertices,m_pkVertices,
		NULL,pkColor,m_pkTexCoords
		,1, NiTriShapeData::NBT_METHOD_NONE, 
		iNumTriangles, pausTriList);



	NiTexturingPropertyPtr spMapTex = NiNew NiTexturingProperty(
		NiApplication::ConvertMediaFilename("effect/speedline.dds"));
	spMapTex->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spMapTex->SetBaseClampMode(NiTexturingProperty::CLAMP_S_WRAP_T);
	m_spLineShape->AttachProperty(spMapTex);

	m_spLineShape->Update(0.0f);
	m_spRoot->AttachChild(m_spLineShape);
	m_spRoot->UpdateNodeBound();

	NiAlphaProperty* pkAlpha = NiNew NiAlphaProperty;
	pkAlpha->SetAlphaBlending(true);
	m_spLineShape->AttachProperty(pkAlpha);

	m_spRoot->UpdateProperties();
	m_spRoot->Update(0.0f);
}


void CSpeedLine::UpdateAllParticles(float dt)
{
	for(int i = 0; i < m_nParticlesNum; i++)
	{
		if (m_pParticles[i].bActive)
		{
			UpdateIndividual(i, dt);
		}
	}
}

void CSpeedLine::UpdateIndividual(int nParticleIndex, float dt)
{
	m_pParticles[nParticleIndex].fLastTime += dt;

	if (m_pParticles[nParticleIndex].fLastTime > 
		m_pParticles[nParticleIndex].fLifespan)
	{
		m_pkVertices[nParticleIndex*4 + 0].x = 0.0f;
		m_pkVertices[nParticleIndex*4 + 0].y = 0.0f;
		m_pkVertices[nParticleIndex*4 + 0].z = 0.0f;

		m_pkVertices[nParticleIndex*4 + 1].x = 0.0f;
		m_pkVertices[nParticleIndex*4 + 1].y = 0.0f;
		m_pkVertices[nParticleIndex*4 + 1].z = 0.0f;

		m_pkVertices[nParticleIndex*4 + 2].x = 0.0f;
		m_pkVertices[nParticleIndex*4 + 2].y = 0.0f;
		m_pkVertices[nParticleIndex*4 + 2].z = 0.0f;

		m_pkVertices[nParticleIndex*4 + 3].x = 0.0f;
		m_pkVertices[nParticleIndex*4 + 3].y = 0.0f;
		m_pkVertices[nParticleIndex*4 + 3].z = 0.0f;

		m_pParticles[nParticleIndex].bActive = false;

		m_anActiveParticles[nParticleIndex%8]--;

		//NiOutputDebugString("go!\n");

	}
	else
	{
		//根据速度，移动粒子
		NiPoint3 vVel = m_pParticles[nParticleIndex].vDir * 
			m_pParticles[nParticleIndex].fSpeed;
		m_pkVertices[nParticleIndex*4 + 0] += vVel;
		m_pkVertices[nParticleIndex*4 + 1] += vVel;
		m_pkVertices[nParticleIndex*4 + 2] += vVel;
		m_pkVertices[nParticleIndex*4 + 3] += vVel;
	}
}

void CSpeedLine::Update(float dt, NiCamera* pCam, float fCarSpeedX)
{
	m_fKarSpeedX = fCarSpeedX;

	m_bForward = (m_fKarSpeedX>0.0f)? true:false;

	dt /= 1000.0f;
	//debug
	if (InputManager::keys[NiInputKeyboard::KEY_X])
		LoadSpecification();
	//~debug

	m_pCamera = pCam;

	UpdateParametres(dt);

	UpdateAllParticles(dt);

	if (((m_bForward) && fCarSpeedX > m_stParamForward.fMinDisplaySpeed)
		||(fCarSpeedX < m_stParamBackward.fMinDisplaySpeed))
        ActiveSomeParticles(dt);

	m_spLineShape->Update(0.0f);
	m_spRoot->UpdateNodeBound();
	m_spRoot->Update(0.0f);
	m_spLineShape->GetModelData()->MarkAsChanged(NiGeometryData::VERTEX_MASK);

}

void CSpeedLine::ActiveSomeParticles(float dt)
{
	float fSpeedCur = 10.0f;//GetSpeed
	//根据当前的速度来确定显示的

	float fSpeedMin = 3.0f;//低于这个速度不显示速度线
	if (fSpeedCur > fSpeedMin)
	{
		//每个方向应该显示的粒子数
		int nNumShouldActive = m_nMaxLineForward;//TODO:(fSpeedCur - fSpeedMin
		if (!m_bForward)
			nNumShouldActive = m_nParticlesNum / 8;

		for (int i = 0; i < 8; i++)
		{
			int nToActive = nNumShouldActive -m_anActiveParticles[i];
			if (nToActive > 0)
			{
				ActiveParticles(i, nToActive);
			}
		}
	}

}
void CSpeedLine::ActiveParticles(int nDirection, int num)
{
	int nHaveActivated = 0;
	int nParticlesPerDir = m_nParticlesNum / 8;
	for (int i = 0; i < nParticlesPerDir; i ++)
	{
		if ((nHaveActivated < num) && (!m_pParticles[i*8 + nDirection].bActive))
		{
			MakeReborn(i*8 + nDirection);
			m_anActiveParticles[nDirection]++;
			nHaveActivated++;
		}
	}
}

void CSpeedLine::Forward(int nParticleIndex, SpeedLineParam* pParam, bool bForward)
{
	int nDirection = nParticleIndex % 8;
	//srand( (unsigned)time( NULL ) );

	//int nRand = rand() % 45;
	int nRand = 1+(int) (60.0*rand()/(RAND_MAX+101.0));

	float fSign = (nRand % 2)? 1.0f: -1.0f;
	float fCoeff = fSign * nRand /100.0f;//角度集中在8个方向周围80%的区域内


	//粒子中心线的角度
	float fThetaY = ((float)nDirection) * DEG_45 + DEG_45*0.5 * fCoeff;

	//计算法线
	NiPoint3 vFrom;
	GetGlobalPointOnCone(pParam->fXCone1, fThetaY, vFrom, pParam);
	NiPoint3 vTo;
	GetGlobalPointOnCone(pParam->fXCone2, fThetaY, vTo, pParam);

	m_pParticles[nParticleIndex].vDir = vTo-vFrom;
	m_pParticles[nParticleIndex].vDir.Unitize();


	NiPoint3 v0, v1, v2, v3;

	NiPoint3 ptInner(pParam->fXCone1, 0.0f, 0.0f);
	ptInner = m_kCamTrans* ptInner;

	NiPoint3 vOut = vFrom - ptInner;

	//速度线“宽度”的方向.
	NiPoint3 vNoraml = vOut.UnitCross(m_pParticles[nParticleIndex].vDir);

	v0 = vFrom + vNoraml * pParam->fLineWidth;
	v1 = vFrom - vNoraml * pParam->fLineWidth;

	float fLength = (pParam->fXCone1- pParam->fXCone2) *cos(pParam->fThetaCone);//正值

	v2 = v1 + m_pParticles[nParticleIndex].vDir * fLength;
	v3 = v0 + m_pParticles[nParticleIndex].vDir * fLength;

	m_pkVertices = m_spLineShape->GetVertices();
	//设置定点数据
	m_pkVertices[nParticleIndex*4] = v0;
	m_pkVertices[nParticleIndex*4+1] = v1;
	m_pkVertices[nParticleIndex*4+2] = v2;
	m_pkVertices[nParticleIndex*4+3] = v3;


	m_pParticles[nParticleIndex].fLifespan = pParam->fLifeSpan * (0.5 + 0.5 * nRand /100.0f);//TODO:
	m_pParticles[nParticleIndex].fLastTime = 0.0f;
	m_pParticles[nParticleIndex].fSpeed = pParam->fSpeed * (0.6 + 0.4 * nRand /100.0f);
	m_pParticles[nParticleIndex].bActive = true;

	if (!bForward)
		m_pParticles[nParticleIndex].vDir *= -1.0f;

}
void CSpeedLine::Backward(int nParticleIndex)
{

}

void CSpeedLine::MakeReborn(int nParticleIndex)
{
	SpeedLineParam* pParam = (m_bForward)? &m_stParamForward:&m_stParamBackward;

	Forward(nParticleIndex, pParam, m_bForward);
}


void CSpeedLine::GetGlobalPointOnCone(float x, float fThetaY, NiPoint3& vPoint,SpeedLineParam* pParam)
{
	float fRdius = (pParam->fXCone - x) * tan(pParam->fThetaCone);
	vPoint.x = x;
	vPoint.z = fRdius * cos(fThetaY);
	vPoint.y = fRdius * sin(fThetaY)-pParam->fOffsetY;

	vPoint = m_kCamTrans * vPoint;
}

void CSpeedLine::InitParametres()
{
	m_bForward = true;
	LoadSpecification();
}

void CSpeedLine::UpdateParametres(float dt)
{
	m_kCamTrans = m_pCamera->GetWorldTransform();
}

void CSpeedLine::LoadSpecification()
{
	//char buff[256];
	//GetCurrentDirectory(256, buff);

	CScriptState ls;
	if(ls.DoFile("Config/SpeedLineCfg.lua") != 0)
		return;

	CVarTable vtParamLine("SpeedLineParam");
	vtParamLine.GetFromScript(&ls);
	m_nParticlesNum = vtParamLine.Element("LineNum")->Integer();
	m_nMaxLineForward = vtParamLine.Element("nMaxLineForward")->Integer();

	CVarTable vtForward("Forward");
	vtForward.GetFromScript(&ls);
	m_stParamForward.fThetaCone = vtForward.Element("fThetaCone")->Float()/180.0f * NI_PI;
	m_stParamForward.fXCone = vtForward.Element("fXCone")->Float();
	m_stParamForward.fXCone1 = vtForward.Element("fXCone1")->Float();
	m_stParamForward.fXCone2 = vtForward.Element("fXCone2")->Float();
	m_stParamForward.fLineWidth = vtForward.Element("fLineWidth")->Float();
	m_stParamForward.fOffsetY = vtForward.Element("fOffsetY")->Float();
	m_stParamForward.fSpeed = vtForward.Element("fSpeed")->Float();
	m_stParamForward.fLifeSpan = vtForward.Element("fLifeSpan")->Float();
	m_stParamForward.fMinDisplaySpeed = vtForward.Element("fMinDisplaySpeed")->Float();

	CVarTable vtBackward("Backward");
	vtBackward.GetFromScript(&ls);
	m_stParamBackward.fThetaCone = vtBackward.Element("fThetaCone")->Float()/180.0f * NI_PI;
	m_stParamBackward.fXCone = vtBackward.Element("fXCone")->Float();
	m_stParamBackward.fXCone1 = vtBackward.Element("fXCone1")->Float();
	m_stParamBackward.fXCone2 = vtBackward.Element("fXCone2")->Float();
	m_stParamBackward.fLineWidth = vtBackward.Element("fLineWidth")->Float();
	m_stParamBackward.fOffsetY = vtBackward.Element("fOffsetY")->Float();
	m_stParamBackward.fSpeed = vtBackward.Element("fSpeed")->Float();
	m_stParamBackward.fLifeSpan = vtBackward.Element("fLifeSpan")->Float();
	m_stParamBackward.fMinDisplaySpeed = vtBackward.Element("fMinDisplaySpeed")->Float();
}