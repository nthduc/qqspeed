#ifndef __SCENE_H__
#define __SCENE_H__

#include "string"
using namespace std;
class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual bool Load(LPCSTR pstrNIFFile);
	virtual void Unload();
	virtual bool Reload();
	virtual bool SetCamera(LPCSTR pstrCameraFile, LPCSTR pstrCameraName);
	virtual void SetCamera(NiCamera*);
	virtual NiNode*		GetScene();
	virtual NiCamera*	GetCamera();
	virtual NiCamera*	GetSceneCamera(LPCSTR pstrCameraName);

	virtual void Update(float fAccuTime);
	virtual void Render();
	virtual void CacheRender();
	virtual void Draw();

protected:
	NiCamera * RecurviseFindSceneCamera(NiAVObject * pkAVObj, const NiFixedString & kCameraName);

protected:
	NiNodePtr	m_spScene;
	NiCameraPtr	m_spCamera;
	string		m_strNIFFile;
	string		m_strCameraFile;
	string		m_strCameraName;
};

#include "singlet.h"
class CKartScene : public CSingleT<CKartScene>, public CScene
{
public:
	bool AddKartToPoint(const char * pcName, Play * pPlay);
	bool AddRoleToPoint(const char * pcName, Play * pPlay);
	bool RemoveKartFromPoint(const char * pcName, Play * pPlay);
	bool RemoveRoleFromPoint(const char * pcName, Play * pPlay);
};

#endif