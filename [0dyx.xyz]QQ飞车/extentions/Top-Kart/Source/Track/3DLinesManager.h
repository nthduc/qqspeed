#pragma once
#include "nimemobject.h"
#include "NiRenderer.h"
#include "d3d9.h"
#include "d3dx9core.h"

#define _DX9

class C3DLinesManager :
	public NiMemObject
{
	C3DLinesManager(ID3DXSprite * pSprite, IDirect3DDevice9 * pDevice);
public:	
	virtual ~C3DLinesManager(void);

	static C3DLinesManager * Create(NiRenderer * pRenderer);
	static C3DLinesManager * Get3DLinesManager();
	static void				 Destroy3DLinesManager();
	
	void DrawLine(const NiPoint3 & kStart, const NiPoint3 & kEnd, const NiColorA & kColor);
	void DrawLine(const NiPoint3 * akPoints, const NiColorA * akColors, unsigned int uiNum);

	void DrawBox(const NiPoint3 & k1, const NiPoint3 & k2, 
		const NiPoint3 & k3, const NiPoint3 & k4, const NiColorA & kColor);
	void DrawBoxWire(const NiPoint3 & k1, const NiPoint3 & k2, 
		const NiPoint3 & k3, const NiPoint3 & k4, const NiColorA & kColor);

	void DrawRect(const NiPoint3 & k1, const NiPoint3 & k2, const NiColorA & kColor);
	void DrawRect(const NiPoint3 & k1, const NiPoint3 & k2, 
		const NiPoint3 & k3, const NiPoint3 & k4, const NiColorA & kColor);

	void DrawBox(const NiPoint3 & k, float halfWidth, const NiColorA & kColor);
	void DrawBox(const NiPoint3 & k1, const NiPoint3 & k2, float halfwidth, const NiColorA & kColor);

	void OnRestDevice();
	void OnLostDevice();
	bool BeLostDevice(){return m_bLostDevice;}
	void SetMatrix(const NiMatrix3 & kMatrix);

protected:
	void StartSprite();
	void EndSprite();

protected:
	ID3DXSprite		 * m_pSprite;
	IDirect3DDevice9 * m_pD3dDevice;

	bool			   m_bOnSprite;
	bool			   m_bLostDevice;

	static	C3DLinesManager * ms_p3DLinesMgr;
};
