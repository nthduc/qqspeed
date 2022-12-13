////////////////////////////////////////////////
//     FileName:    LevelManager.h
//     Author:      Fish
//     Date:        2007-2-28 14:51:02
//     Description: []
////////////////////////////////////////////////

#ifndef __LEVELMANAGER_H__
#define __LEVELMANAGER_H__

#include "roadmap.h"
#include "singlet.h"
#include "network/msg.h"
#include "Track/PlayerPosUpdateTask.h"
#include "scene.h"

class CKartPlayBack;
class CLevelManager : public CSingleT<CLevelManager>, public CRoadMap, public CScene
{
public:
	enum eCameraType
	{
		CameraInvalid = 0,
		GameCamera  ,
		OverCamera  , 
	};

	CLevelManager();
	~CLevelManager();

	HRESULT Enter(CNotifyGameBeginPara*);
	void	Leave();
	void	Update(float fTime);
	void	Reset();
//	void	UseCamera(eCameraType CameraType);
	void	Render();		//’˝≥£‰÷»æ
	void	Draw();	//‰÷»æ∆¡ƒª‘™Àÿ
	void	CacheRender();	//ª∫≥Â–‘‰÷»æ(‰÷»æµΩÃ˘Õº)

	CNotifyGameBeginPara* GetGameBeginPara() { return &m_kGameBeginPara; }
	NiCamera*		GetCamera();
	CKartPlayBack*	GetPlayBack()	{ return m_pkPlayBack;	}
	CKartCrack*		GetKartCrack()	{ return &m_kKartCrack;	}
	float			GetLevelTime()	{ return m_fElapseTime;	}
	void			SetCountDownTime(int nTime);
	int			GetCountDownTime();

private:
	CNotifyGameBeginPara	m_kGameBeginPara;
	CKartCrack		m_kKartCrack;
	CKartPlayBack*	m_pkPlayBack;
	NiCameraPtr		m_spGameCamera;

	CPlayerPosUpdateTask	m_PlayerPosUpdateTask;
	float	m_fAppStartTime;
	float	m_fPrevFrameTime;
	float	m_fElapseTime;
	int		m_nCountDownTime;

	void SetShadow();
	void RenderShadow(NiCamera* pkCamera);
};

#endif
