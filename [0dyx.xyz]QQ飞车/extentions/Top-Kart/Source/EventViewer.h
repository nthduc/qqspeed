#pragma once
#include <NiMemObject.h>
#include <NiCamera.h>
#include <NiSmartPointer.h>
#include <NiRenderTargetGroup.h>
#include <NiRenderer.h>
#include <NiScreenElements.h>
#include <NiRenderedTexture.h>
#include <NiColor.h>

class ItemEventViewer;
class BackEventViewer;

#define DISSAPEAR_TIME 0.4F

class EventViewer
	: public NiMemObject
{
	friend class ItemEventViewer;
	friend class BackEventViewer;
protected:
	EventViewer(NiNode * pkNodeScene);
	virtual ~EventViewer(void);
public:
	enum ViewerType
	{
		VIEWER_NONE  = -1,
		BACK_VIEWER  = 0,
		EVENT_VIEWER = 1,
	};
	enum enViewerStage
	{
		STAGE_NORMAL	= 0,
		STAGE_DISSAPEAR = 1,
		STAGE_START		= 2, // Add new stage
	};	

	static EventViewer * CreateEventViewer(NiNode * pkNodeScene, EventViewer::ViewerType viewerType);	
	// operators
public:
	virtual void Update(float fDeltaTime) = 0;

	void DrawScreen();
	void RenderScreen(NiCullingProcess & kCuller);
	void Destroy();	

	void SetScale(float fXScale, float fYScale);
	void SetRotate(NiPoint2 kPos, float fAngle);
	// inlines
public:
	inline void Start()
	{
		m_enStage = STAGE_START;
		m_fCloseLife = DISSAPEAR_TIME;
		m_bStartEvent = true;
		SetScale(1.0f, 1.0f);
	}
	inline void Close()
	{
		if (!m_bVisible || STAGE_DISSAPEAR == m_enStage) 
		{
			return;
		}
		m_enStage = STAGE_DISSAPEAR;
		m_fCloseLife = DISSAPEAR_TIME;
		SetScale(1.0f, 1.0f);
	}
	inline void SetVisible(bool bVisible)
	{
		m_bVisible = bVisible;
	}
	inline bool IsVisible()
	{
		return m_bVisible;
	}
	inline void SetCameraAndPlayer(NiCamera * pkCamera)
	{
		assert(pkCamera);
		m_spCamera = pkCamera;
	}
	inline void SetScreenPosition(const NiPoint2 & kPoint)
	{
		m_kLeftUpPosition = NiPoint2(kPoint.x/m_fRenderWidth, kPoint.y/m_fRenderHeight);// = kPoint;
	}

protected:
	void ReplaceDisplayTextureWithRenderedTexture();
	NiScreenElements * CreateScreenElement(bool bWithRes = false);
	bool LoadSetting();
	bool ReSize(NiScreenElements * pkScreenElem, const NiPoint2 & kLeftUpPos, float fXScale, float fYScale);
	void Rotate(NiScreenElements * pkScreenElem, const NiPoint2 & kPos, float fAngle);

protected:
	NiNode *				m_pkNodeScene;
	NiRenderer *			m_pkRenderer;
	ViewerType				m_ViewerType;
	bool					m_bVisible;
	bool					m_bStartEvent;
	NiCameraPtr				m_spCamera;
	NiScreenElementsPtr	    m_spEventRegionMask;
	NiScreenElementsPtr		m_spEventRegion;
	NiRenderedTexturePtr	m_spTexture;
	NiRenderTargetGroupPtr  m_spRenderTargetGroup;	

	float					m_fViewerSize;
	float					m_fRenderWidth;
	float					m_fRenderHeight;
	float					m_fAspectRatio;

	enViewerStage			m_enStage;
	float					m_fCloseLife;


	NiPoint2					 m_kLeftUpPosition;
	float						 m_fElementWidth;
	float						 m_fElementHeight;
	float						 m_fXScale;
	float						 m_fYScale;

	float						 m_fTotalRotate;

	unsigned int				 m_uiVertexNum;
	NiTPrimitiveArray<NiPoint2*> m_arrRect;

	NiFixedString				 m_kResPath;

	static const NiColorA		 m_kBackGroundColor;
};


class ItemEventViewer
	: public EventViewer
{
public:
	ItemEventViewer(NiNode * pkNodeScene);
	virtual ~ItemEventViewer();

public:
	virtual void Update(float fDeltaTime);
};

class BackEventViewer
	: public EventViewer
{
public:
	BackEventViewer(NiNode * pkNodeScene);
	virtual ~BackEventViewer();

public:
	virtual void Update(float fDeltaTime);
};



class EventViewerMgr
	: public NiMemObject
{
	EventViewerMgr();
	virtual ~EventViewerMgr();
public:
	static bool CreateEventViewerMgr(NiNode * pkScene);
	static void Destroy();

	static void StartItemEvent(NiCamera * pkCamera);
	static void StartBackEvent(NiCamera * pkCamera);

	static void CloseItemEvent();
	static void CloseBackEvent();

	static void TurnOnViewer();
	static void TurnOffViewer();

	static void Render();
	static void Draw();
	static void Update(float fDeltaTime);


protected:
	bool Init(NiNode * pkScene);

	ItemEventViewer * m_pkItemEventViewer;
	BackEventViewer * m_pkBackEventViewer;

	bool m_bIsViewerOn;

	static EventViewerMgr * ms_pEventViewerMgr;
};