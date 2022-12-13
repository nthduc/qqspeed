#pragma once
#include "NiMemObject.h"
#include "NiNode.h"
#include "NiCamera.h"
#include "NiTArray.h"
#include "NiTexturingProperty.h"
#include "NiScreenElements.h"
#include "NiRenderedTexture.h"
#include "CheckEditor.h"
#include "vector"


class CGlobalMap :
	public IOnEnterPoint
{
	struct ScreenCheckMark 
		: public NiMemObject
	{
		NiFixedString kName;
		NiString2DPtr spNameString;
		NiScreenElementsPtr spScreenMark;
		bool		  bHiLight;
	};

	CGlobalMap(void);
	virtual ~CGlobalMap(void);

public:
	static bool	Create();
	static CGlobalMap * Get();
	static void Destroy();

	bool Init();
	void ShowGlobalMap(bool bShow);
	bool IsVisible();
	void HiLight(const NiFixedString & kName, bool bHiLight);
	void HiLightAll(bool bHiLight);
	void Draw();
	void Update(float fTime);

	virtual void OnLapUp();
	virtual void OnEnterPoint(CCheckPoint * pChkPt);
	virtual void OnLeavePoint(CCheckPoint * pChkPt);

protected:
	ScreenCheckMark* CreateCheckMark(const NiPoint3& kPos, const NiFixedString & kName);
	void			 DestroyCheckMark(ScreenCheckMark*& pMark);
	void             ShiftState(bool bHiLight, ScreenCheckMark * pScreenElems);

	NiScreenElementsPtr		m_spKartTexture;
	
	std::vector<ScreenCheckMark*>   m_arrCheckMarks;
	
	NiScreenElementsPtr		m_spGlobalMap;
	NiFontPtr				m_spFont;

	NiPoint2				m_kLeftUp;
	NiPoint2				m_kRightDown;

	NiPoint2				m_kMapScreenLeftTop;

	float					m_fAspect;
	float					m_fMapSize;
	float					m_fCheckMarkSize;
	NiPoint2 *				m_pkTexCoords0;
	NiPoint2 *				m_pkTexCoords1;

	bool					m_bShow;

	static CGlobalMap * ms_pGlobalMap;
};
