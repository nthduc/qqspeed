////////////////////////////////////////////////
//     FileName:    roadmap.h
//     Author:      Fish
//     Date:        2007-2-28 11:18:29
//     Description: 赛道地图的封装类
////////////////////////////////////////////////

#ifndef __ROADMAP_H__
#define __ROADMAP_H__

#include "Track/CheckEditor.h"
#include "variant.h"
#include "string"
#include "vector"
using namespace std;

class PhysXManager;
class CRoadMap
{
public:
	struct Map_Desc
	{
		//碰撞体信息
		struct Collider
		{
			string	kFileName;		
			int		nColliderType;
			int		nGroupType;
		};
		//镜子
		struct Mirror
		{
			string kMirrorName;
			string kNodeName;
			string kTriShapeName;
			NiPoint3	kCenterPos;
			NiPoint3	kRight;
			NiPoint3	kUp;
		};
		//小对象剪裁策略
		struct AppCullStrategy
		{
			float	fAllVisibleDistance;
			float	fMinAngle;
			float	fMaxProcessBoundRadius;
		};
		//Transform
		struct Transform
		{
			NiPoint3	kTrans;
			NiPoint3	kRot;
		};

		string	kMapName;	
		string	kCheckPointFileName;
		string	kPlaybackCameraFileName;
		string	kEagleMapFileName;
		string	kPickMapFileName;
		string	kSceneFileName;
		string	kIntroCameraName;
		DWORD	dwPropBox;
		string	kAwardName;
		string	kTeamAwardName;
		string  kAmbientSoundName;
		Transform		kAwardTransform;
		Transform		kAwardCamTransform;
		AppCullStrategy	kAppCullStrategy;
		vector<Mirror>		kArrMirrors;
		vector<Collider>	kArrStaticColliders;
		vector<Collider>	kArrAnimColliders;
		vector<Collider>	kArrActiveColliders;
		NiPoint3  kSusspensionPos;
		int	  nUsingSkyAlphaSorter;
	};

	HRESULT	LoadMap(DWORD MapID, DWORD nLaps);
	void	UnLoadMap();
	void	Update(float fTime);
	void	Render(NiCamera*);
	NiNode*	GetRootNode() { return m_spScene; }
	CRoadMap::Map_Desc* GetMapDesc() { return &m_kMapDesc; }

	void	AttachObject(NiNodePtr pNode);
	void	DetachObject(NiNodePtr pNode);

	CLinks*	GetLinks();
	PhysXManager*	GetPhysXManager();
	bool	GetPhysXDebugRender();
	void	SetPhysXDebugRender(const bool bRender) ;

	bool IsEnableCheckPoint()
	{
		return m_bEnableCheckPoint;
	}

	static void _ReadCollidersFromVar(LPCSTR, vector<CRoadMap::Map_Desc::Collider>& kArrColliders, CVar*);
	static void _ReadMirrorsFromVar(vector<CRoadMap::Map_Desc::Mirror>& kArrMirrors, CVar*);
	static void _ReadTransformFromVar(CRoadMap::Map_Desc::Transform& kTransform, CVar*);
	static void _ReadPointFromVar(NiPoint3& kPoint, CVar*);
	static void _ReadAppCullStrategy(CRoadMap::Map_Desc::AppCullStrategy& kStrategy, CVar*);
	static void _ReadSusPosFromVar(NiPoint3& kSusPos, CVar* pVar);	

	static void LoadCollidersInfo(vector<Map_Desc::Collider> & kArrColliders, PhysXManager *& pPhysXMgr, NiNode * pkFatherNode = NULL);

	 void LoadStaticColliders(vector<Map_Desc::Collider> & kArrColliders, PhysXManager *& pPhysXMgr, NiNode * pkFatherNode);

	 void LoadKinematicColliders(vector<Map_Desc::Collider> & kArrColliders, PhysXManager *& pPhysXMgr, NiNode * pkFatherNode);
protected:
	HRESULT LoadMapConfig(DWORD MapID);
	void    CreateFog(const float r,const float g,const float b,const float depth, const float ffar);
protected:
	Map_Desc	m_kMapDesc;
	CLinks		m_kLinks;
	NiNodePtr	m_spScene;
	NiNodePtr	m_spLevelRoot;
	NiNodePtr	m_spMoveNode;
	NiNodePtr	m_spAttachNode;
	NiNodePtr	m_spStaticRoot;
	NiNodePtr	m_spSky;

	PhysXManager*	m_pPhysXManager;

private:
	bool		m_bEnableCheckPoint;
};

#endif