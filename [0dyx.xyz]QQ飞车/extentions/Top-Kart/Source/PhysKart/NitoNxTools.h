#pragma once 

//Primo  [3/29/2007]


class NiToNxTools{

public:

	NiToNxTools();
	~NiToNxTools();
	
	static void LoadStaticColliders(const char* szFilename, NiPhysXScene* pkPhysxScene, 
		NiNode* pkStaticParent, PhysXManager* pManager
		, PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData);

	static void LoadKinematicColliders(const char* szFilename, NiPhysXScene* pkPhysxScene, 
		NiNode* pkDynamicParent, PhysXManager* pManager
		, PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData);

	static void LoadDynamicColliders(const char* szFilename, NiPhysXScene* pkPhysxScene, 
		NiNode* pkDynamicParent, PhysXManager* pManager);

	//说明：
	
	static void LoadStaticObjects(NxScene* pScene, NiNode* pkNode, NiNode* pkParent
		, PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData);
	static void LoadDynamicObjects(NiPhysXScene* pScene, NiNode* pkNode, NiNode* pkParent, PhysXManager* pManager
		);
	static void LoadKinematicObjects(NiPhysXScene* pScene, NiNode* pkNode, NiNode* pkParent, PhysXManager* pManager
		, PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData);

	
	//坐标应该是全局的

	static NxActor* CreateConvexShapeFromTriShape(NxScene* pScene, const NiTriShape& triShape, float density);


};