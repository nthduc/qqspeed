#pragma once
#include "NiNode.h"
#include "NiMemObject.h"
#include "NiRefObject.h"
#include "NiScreenElements.h"
#include "NiScreenTexture.h"
#include "NiFixedString.h"
// Using notes: you'd better set a name for the node, so u can easily search this node
// by it's name

class CScreenElement
	: public NiNode
{
	CScreenElement(const char * sResName);
public:	
	static CScreenElement * CreateScreenElement(const char * sResName);
	virtual ~CScreenElement();

	enum Property
	{
		Prop_Alpha		= 0,
		Prop_Light		= 1,
		Prop_Material	= 2,
		Prop_Texture	= 3,
		Prop_Color		= 4,
		Prop_Scale		= 5,
		Prop_Rotate		= 6,
		Prop_Figure		= 7,
		Prop_Num
	};
	
	// Set a name for this ScreenElement
	// void SetName(const char * sName);
	void ChangeTexture(const char * sResName);
	// 
	void AddLight(const NiPoint3 & kLightDir, const NiColor & kLightColor);
	// Set the color of the element
	void SetColor(const NiColorA & kColor);
	// Set the scale of the element on device
	void SetScale(float scale);
	// Rotate the element on device, rotate around z axis
	void SetRotate(float angle);
	// Set the left-up points of the element on device
	void SetPosAndSize(const NiPoint2& kPoint, unsigned int width, unsigned height);
	// Set the outer figure of the element
	void SetFigure(NiPoint2 * aPoints, unsigned int unPointsNum);
	// If finish all the settings, then using this function to apply setting
	bool ApplySetting(bool bWithAlpha, NiAlphaProperty::AlphaFunction alphSrc = NiAlphaProperty::ALPHA_ONE, 
		NiAlphaProperty::AlphaFunction alphaDest = NiAlphaProperty::ALPHA_ONE);
	//
	void Draw(NiRenderer * pkRenderer);

protected:
	bool					m_bProp[Prop_Num];

	NiFixedString			m_kResName;
	NiColorA				m_kColor;
	float					m_fScale;
	NiPoint2				m_kPos;
	unsigned int			m_unWidth;
	unsigned int			m_unHeight;
	unsigned int			m_uiMapPointCount;
	NiPoint2			  * m_pkTexCoords;

	NiScreenElementsPtr		m_skScreenElement;
	NiTexturingPropertyPtr	m_skTexturingProperty;
	static unsigned int ms_unCreatedScreenElemCounts;
};

NiSmartPointer(CScreenElement);
typedef NiTObjectArray<CScreenElementPtr> CScreenElementArray;

class CScreenElementsManager
	: public NiMemObject
{
public:
	CScreenElementsManager(void);
	virtual ~CScreenElementsManager(void);	

public:
	void	 Draw();
	CScreenElement * GetScreenElement(const NiFixedString & name);
	void	 AddScreenElement(CScreenElement * pkScreenElement);
	void	 RemoveScreenElement(const NiFixedString & name);
	void	 RemoveAll();

#ifdef _DEBUG
	void	 DumpAllElements();
#endif	

protected:
	NiNodePtr m_skWindowUIRoot;
};


