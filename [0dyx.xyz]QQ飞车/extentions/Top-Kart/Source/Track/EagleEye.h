#pragma once

#include "NiMemObject.h"
#include "NiPoint2.h"
#include "NiScreenElements.h"
#include "NiScreenTexture.h"
#include "NiMaterialProperty.h"
#include "NiRenderer.h"
#include "NiCamera.h"
#include "NiMatrix3.h"
#include "NiTexturingProperty.h"
#include "NiRenderedTexture.h"
#include "singlet.h"

class CEagleEyeMap :
	public CSingleT<CEagleEyeMap>
{
	struct RemoteKartMark 
		: public NiMemObject
	{
		NiNodePtr spRemoteKart;
		unsigned int unUin;
	};

public:
	CEagleEyeMap();
	virtual ~CEagleEyeMap();
public:

	void Show(bool bShow){m_bShow = bShow;};
	bool Init(const char * pcFilePath);

	void ReplaceDisplayTextureWithRenderedTexture();

	void UpdateMap(const NiPoint3 & kKartPos, const NiMatrix3 & kRotate, unsigned int unUin = 0, bool bSelf = true);
	
	void Render();

	void Draw();

protected:
	void GetMappedKartPosition(const NiPoint3 & kKartPos);


	NiPoint3    m_kMappedPos;

	NiNodePtr	m_spMap;
	NiNodePtr   m_spKartMark;
	NiNodePtr   m_spKartRemoteRes;
	NiNodePtr	m_spMapBk;

	NiTPrimitiveArray<RemoteKartMark *> m_arrRemoteKartMarks;

	NiCameraPtr m_spCamera;
	NiRenderTargetGroupPtr m_spRenderTargetGroup;	

	NiTexturingPropertyPtr	m_spEyeMapTexProperty;
	NiScreenElementsPtr		m_spEyeMapRegion;
	NiScreenElementsPtr		m_spEyeMapMask;
	NiRenderedTexturePtr	m_spTexture;
	bool                    m_bShow;
};