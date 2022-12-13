
#include "Top-KartPCH.h"
#include "AcceleratePanel.h"
#include "DescConstant.h"
#include "nifop.h"
#include "screendraw.h"
#include "ScreenOverlay.h"

extern float g_fXUIScale;
extern float g_fYUIScale;

void RotateScreenElements(NiScreenElements * pkScreenElem, const NiPoint2 & kPos, float fAngle)
{
	NiGeometryData * pkGeometryData = pkScreenElem->GetModelData();
	NiPoint3* pkVertex = pkGeometryData->GetVertices();//pkScreenElem->GetVertices(0);
	unsigned short usVerticesNum = pkGeometryData->GetVertexCount();

	// Deg to rad
	float fRad = (fAngle);

	//m_fTotalRotate += fRad;
	float fSin = 0.0f, fCos = 0.0f;
	NiSinCos(fRad, fSin, fCos);

	float fX1 = 0.0f, fY1 = 0.0f, fLen = 0.0f;

	for (int i = 0; i < (int)usVerticesNum; i++)
	{
		fX1 = (pkVertex->x - kPos.x);
		fY1 = (pkVertex->y - kPos.y);		
		pkVertex->x = (fX1 * fCos - fY1 * fSin) + kPos.x;
		pkVertex->y = (fY1 * fCos + fX1 * fSin) + kPos.y;
		pkVertex++;
	}
	pkGeometryData->MarkAsChanged(NiGeometryData::VERTEX_MASK);
	pkScreenElem->UpdateBound();
}

CAcceleratePanel::CAcceleratePanel(float totalAngle ,float maxSpeed, unsigned int radius )
{
	m_fTotalAngle = totalAngle/180*NI_PI;
	m_fMaxSpeed = maxSpeed;
	m_fRadius = radius;
	m_fWidth = 110;
	m_fHeight = 110;
	m_fPointerAngleTotal = 0;

	NiFontPtr pkSpeedFont = NiFont::Create(NiRenderer::GetRenderer(),
		NiApplication::ConvertMediaFilename("ScreenOverLay/HUD.nff"));

	m_pkSpeedText = new TextContainer;

	m_pkSpeedText->m_spString/* m_spSpeedString*/ = NiNew NiString2D(pkSpeedFont,NiFontString::COLORED, 32, 
		" ", NiColorA(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);


	m_spBaseElement = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false,1));
	m_spBaseElement->Insert(4);

	m_spDynamicElement = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false,1));
	m_spDynamicElement->Insert(4);

	m_spPointerElement = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false,1));
	m_spPointerElement->Insert(4);
}


CAcceleratePanel::~CAcceleratePanel()
{
	m_spBaseElement = 0;
	m_spDynamicElement = 0;
	m_spPointerElement = 0;
	delete m_pkSpeedText;
	m_pkSpeedText = 0;
}


void CAcceleratePanel::SetTotalAngle(float angle)
{
	m_fTotalAngle = angle/180*NI_PI;
}

void CAcceleratePanel::SetMaxSpeed(float maxSpeed)
{
	m_fMaxSpeed = maxSpeed;
}

void CAcceleratePanel::SetRadius(unsigned int radius)
{

	m_fRadius = radius;
}

void CAcceleratePanel::SetBaseTexture(char * fileName)
{

	NiTexturingPropertyPtr	baseTexPro;
	NiSourceTexture *		baseTex;
	NiAlphaPropertyPtr		baseAlphaPro;	

	baseTexPro = NiNew NiTexturingProperty;
	baseTex = NiSourceTexture::Create(
		NiApplication::ConvertMediaFilename(fileName));
	baseTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	baseTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	baseTexPro->SetBaseTexture(baseTex);

	m_fWidth = baseTex->GetWidth();
	m_fHeight = baseTex->GetHeight();

	baseAlphaPro = NiNew NiAlphaProperty;
	baseAlphaPro->SetAlphaBlending(true);
	baseAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	baseAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	m_spBaseElement->AttachProperty(baseTexPro);
	m_spBaseElement->AttachProperty(baseAlphaPro);
	m_spBaseElement->SetTextures(0, 0, 0, 0, 1, 1);

	m_spBaseElement->UpdateProperties();
	m_spBaseElement->UpdateEffects();
	m_spBaseElement->Update(0);
}

void CAcceleratePanel::SetDynamicTexture(char * fileName)
{
	NiTexturingPropertyPtr	dynTexPro;
	NiAlphaPropertyPtr		dynAlphaPro;

	dynTexPro = NiNew NiTexturingProperty(NiApplication::ConvertMediaFilename(fileName));

	dynTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	dynTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	dynAlphaPro = NiNew NiAlphaProperty;
	dynAlphaPro->SetAlphaBlending(true);
	dynAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	dynAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	m_spDynamicElement->AttachProperty(dynTexPro);
	m_spDynamicElement->AttachProperty(dynAlphaPro);


	m_spDynamicElement->UpdateProperties();
	m_spDynamicElement->UpdateEffects();
	m_spDynamicElement->Update(0);
}

void CAcceleratePanel::SetPointerTexture(char * fileName)
{
	NiSourceTexturePtr		spTex = NiSourceTexture::Create(
		NiApplication::ConvertMediaFilename(fileName));

	/*
	NiStream kStream;
	if ( !kStream.Load(NiApplication::ConvertMediaFilename("board.nif")) )
	{
		return;
	}
	m_spPointerBoardNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
	assert(m_spPointerBoardNode);
	NiGeometry* pkGeometry = CNifOp::FindGeometry(m_spPointerBoardNode);
	NiTexturingProperty* pkTexProp = (NiTexturingProperty*)pkGeometry->GetProperty(NiProperty::TEXTURING);
	if ( pkTexProp )
	{
		pkTexProp->SetBaseTexture(spTex);
	}
	else
	{
		NiTexturingPropertyPtr spTexProp = NiNew NiTexturingProperty;
		spTexProp->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
		spTexProp->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
		spTexProp->SetBaseTexture(spTex);
		pkGeometry->AttachProperty(spTexProp);
	}

	m_spPointerBoardNode->UpdateProperties();
	m_spPointerBoardNode->UpdateEffects();
	m_spPointerBoardNode->Update(0);
	*/
	
	m_fWidth = spTex->GetWidth();
	m_fHeight = spTex->GetHeight();

	NiTexturingPropertyPtr spTexProp = NiNew NiTexturingProperty;
	spTexProp->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	spTexProp->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	spTexProp->SetBaseTexture(spTex);

	NiAlphaPropertyPtr baseAlphaPro = NiNew NiAlphaProperty;
	baseAlphaPro->SetAlphaBlending(true);
	baseAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	baseAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	m_spPointerElement->AttachProperty(spTexProp);
	m_spPointerElement->AttachProperty(baseAlphaPro);
	m_spPointerElement->SetTextures(0, 0, 0, 0, 1, 1);

	m_spPointerElement->UpdateProperties();
	m_spPointerElement->UpdateEffects();
	m_spPointerElement->Update(0);
	
}

void CAcceleratePanel::MoveTo(float x, float y)
{
	unsigned int uiScreenWidth = GameMain::GetScreenWidth();
	unsigned int uiScreenHeight = GameMain::GetScreenHeight();
	x = float(uiScreenWidth - m_fWidth) / uiScreenWidth;
	y = float(uiScreenHeight - m_fHeight) / uiScreenHeight;
	m_fU = x;
	m_fV = y;
	m_spBaseElement->SetRectangle(0, x, y, float(m_fWidth)/uiScreenWidth, float(m_fHeight)/uiScreenHeight);
	m_spBaseElement->UpdateBound();

	m_spPointerElement->SetRectangle(0, x, y, float(m_fWidth)/uiScreenWidth, float(m_fHeight)/uiScreenHeight);
	m_spPointerElement->UpdateBound();

	m_pkSpeedText->m_spString/*m_spSpeedString*/->SetPosition(x*uiScreenWidth+78, y*uiScreenHeight + uiScreenHeight + 130);
	m_pkSpeedText->m_uiOriginX = x*uiScreenWidth+78;
	m_pkSpeedText->m_uiOriginY = y*uiScreenHeight + uiScreenHeight + 130;
	
}


void CAcceleratePanel::Update(float fTime)
{
	float curSpeed = 0;
	if( NxPlayMgr::GetHostPlay()->GetKart())
	{
		curSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * GameSetting::SpeedDispCoef;
	}
	if(curSpeed > 100)
	{
		int temp = 0; 
	}
	if(curSpeed < 0) curSpeed = 0;
	unsigned int uiScreenWidth = GameMain::GetScreenWidth();
	unsigned int uiScreenHeight = GameMain::GetScreenHeight();
	float curAngle = curSpeed < m_fMaxSpeed ? curSpeed/m_fMaxSpeed*m_fTotalAngle : m_fTotalAngle;
	//	m_spDynamicElement->RemoveAll();
	if(curAngle < NiATan(float(m_fHeight)/m_fRadius))
	{
		//		m_spDynamicElement->Insert(3);
		m_spDynamicElement->SetVertex(0, 0, NiPoint2(float(m_fRadius)/uiScreenWidth + m_fU, float(m_fRadius)/uiScreenHeight + m_fV));
		m_spDynamicElement->SetTexture(0, 0, 0, NiPoint2(0.5, 0.5));

		m_spDynamicElement->SetVertex(0, 1, NiPoint2(m_fU, m_fV + (m_fRadius - m_fRadius*tan(curAngle))/uiScreenHeight));
		m_spDynamicElement->SetTexture(0, 1, 0, NiPoint2(0, 0.5 - m_fRadius*tan(curAngle)/m_fHeight));

		m_spDynamicElement->SetVertex(0, 2, NiPoint2(m_fU, float(m_fRadius)/uiScreenHeight + m_fV));
		m_spDynamicElement->SetTexture(0, 2, 0, NiPoint2(0, 0.5));


	}
	else
	{	
		//		m_spDynamicElement->Insert(4);
		m_spDynamicElement->SetVertex(0, 0, NiPoint2(float(m_fRadius)/uiScreenWidth + m_fU, float(m_fRadius)/uiScreenHeight + m_fV));
		m_spDynamicElement->SetTexture(0, 0, 0, NiPoint2(0.5, 0.5));

		m_spDynamicElement->SetVertex(0, 1, NiPoint2(m_fU + (m_fRadius - m_fRadius/tan(curAngle))/uiScreenWidth, m_fV));
		m_spDynamicElement->SetTexture(0, 1, 0, NiPoint2((m_fRadius - m_fRadius/tan(curAngle))/m_fWidth, 0));	

		m_spDynamicElement->SetVertex(0, 2, NiPoint2(m_fU, m_fV));
		m_spDynamicElement->SetTexture(0, 2, 0, NiPoint2(0, 0));

		m_spDynamicElement->SetVertex(0, 3, NiPoint2(m_fU, float(m_fRadius)/uiScreenHeight + m_fV));
		m_spDynamicElement->SetTexture(0, 3, 0, NiPoint2(0, 0.5));


	}	
	m_spDynamicElement->UpdateBound();

	NiGeometryData * pkGeometryData = m_spPointerElement->GetModelData();
	NiPoint3* pkVertex = pkGeometryData->GetVertices();//pkScreenElem->GetVertices(0);
	unsigned short usVerticesNum = pkGeometryData->GetVertexCount();

	float fx = m_fRadius * 1.414;
	float fRads[4] = { 135.0 * NI_PI / 180, 225.0 * NI_PI / 180, 
		315.0 * NI_PI / 180, 45.0 * NI_PI / 180 };
	NiPoint2 fUVs[4] = { NiPoint2(0, 0), NiPoint2(0, 1), NiPoint2(1, 1), NiPoint2(1, 0) };
	for ( int i = 0; i < 4; i++ )
	{
		float fTmpAngle = fRads[i] + curAngle;
		float tx0 = fx * cos(fRads[i]);
		float ty0 = fx * sin(fRads[i]);
		float tx = fx * cos(fTmpAngle);
		float ty = fx * sin(fTmpAngle);
		float dx = tx - tx0;
		float dy = ty - ty0;
		NiPoint2& kPt = fUVs[i];
		kPt.x += dx / m_fWidth;
		kPt.y -= dy / m_fHeight;
		m_spPointerElement->SetTexture(0, i, 0, kPt);
	}
	/*
	float fDelAngle = curAngle - m_fPointerAngleTotal;
	m_fPointerAngleTotal = curAngle;
	RotateScreenElements(m_spPointerElement, 
		NiPoint2(m_fU + m_fWidth * 0.5 / uiScreenWidth, m_fV + m_fHeight * 0.5 / uiScreenHeight), fDelAngle);
		*/
	//m_spPointerElement->Update(fTime);
	/*
	NiMatrix3 kRot;
	kRot.MakeZRotation(curAngle);
	NiGeometry* pkGeometry = CNifOp::FindGeometry(m_spPointerBoardNode);
	pkGeometry->SetRotate(kRot);
	pkGeometry->Update(fTime);
	*/

	m_pkSpeedText->AdjustPosition(g_fXUIScale, g_fYUIScale);
	if(curSpeed < 10)
		m_pkSpeedText->m_spString->sprintf("00%d", int(curSpeed));
	else if(curSpeed < 100)
		m_pkSpeedText->m_spString->sprintf("0%d", int(curSpeed));
	else
		m_pkSpeedText->m_spString->sprintf("%d", int(curSpeed));
	/*
	if(curSpeed < 10)
	m_spSpeedString->sprintf("00%d km/h", int(curSpeed));
	else if(curSpeed < 100)
	m_spSpeedString->sprintf("0%d km/h", int(curSpeed));
	else
	m_spSpeedString->sprintf("%d km/h", int(curSpeed));
	*/
}


void CAcceleratePanel::Draw(NiRendererPtr spRenderer)
{
	m_spBaseElement->Draw(spRenderer);
	m_spDynamicElement->Draw(spRenderer);
	//m_pkSpeedText->m_spString->Draw(spRenderer);
	m_spPointerElement->Draw(spRenderer);
/*	unsigned int uiScreenWidth = GameMain::GetScreenWidth();
	unsigned int uiScreenHeight = GameMain::GetScreenHeight();
	CScreenDraw::Draw(m_spPointerBoardNode, CNifOp::FindCamera(m_spPointerBoardNode), 
		m_fU * uiScreenWidth, m_fV * uiScreenHeight, m_fWidth, m_fHeight);
*/}