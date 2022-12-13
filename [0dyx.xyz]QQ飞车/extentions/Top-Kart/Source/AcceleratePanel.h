#pragma once
#include "NiNode.h"
#include "NiMemObject.h"
#include "NiRefObject.h"
#include "NiScreenElements.h"
#include "NiScreenTexture.h"
#include "NiTexturingProperty.h"
#include "RenderBase.h"
#include "singlet.h"

struct TextContainer; 

class CAcceleratePanel: public CRenderBase2D, public CSingleT<CAcceleratePanel>
{
public:
	CAcceleratePanel(float totalAngle = 0, float maxSpeed = 0, unsigned int radius = 100);
	~CAcceleratePanel();
//	void Create(float totalAngle,float maxSpeed);
	void MoveTo(float x, float y);
	void SetTotalAngle(float angle);
	void SetMaxSpeed(float maxSpeed);
	void SetBaseTexture(char * fileName);
	void SetDynamicTexture(char * fileName);
	void SetPointerTexture(char* fileName);
	void SetRadius(unsigned int radius);
	virtual void Update(float fTime);
	virtual void Draw(NiRendererPtr spRenderer);

private:
	NiScreenElementsPtr		m_spBaseElement;
	NiScreenElementsPtr		m_spDynamicElement;
	NiScreenElementsPtr		m_spPointerElement;
//	NiNodePtr				m_spPointerBoardNode;
	TextContainer*			m_pkSpeedText;
	//NiString2DPtr			m_spSpeedString;
	float					m_fTotalAngle;
	float					m_fPointerAngleTotal;
	float					m_fMaxSpeed;
	unsigned int			m_fRadius;
	float					m_fU;
	float					m_fV;
	unsigned int			m_fWidth;
	unsigned int			m_fHeight;
};