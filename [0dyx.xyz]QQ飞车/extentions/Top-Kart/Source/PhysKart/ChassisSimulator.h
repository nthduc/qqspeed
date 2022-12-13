#pragma once

/*
Author : Primowang 2007.3.2

*/
#define          WHEELS        4
#define          LIFTERS		2

#define			NOT_DRIFTING    0
#define			LEFT_DRIFTING	1
#define			RIGHT_DRIFTING	2



class ChassisSimulator
{
public:
	ChassisSimulator(NxScene* pScene);//, NxVec3& pos);
	virtual ~ChassisSimulator(void);


	typedef struct tagMASSTRANSINFO
	{
		float fRadFrom;//开始的弧度
		float fRadTo;//结束的弧度
		float fRadCur;//当前的弧度
		float fVel;//弧度增加的速度
		NxVec3 vMaxDist;//质量转移的最大距离
	}MASSTRANSINFO;

	MASSTRANSINFO* m_pMassTrans;

	typedef enum tagDRIFTPERIOD
	{
		//DP_DRIFTING = 0,//漂移--一侧车轮抬起并保持的过程（这个过程取消，转动显示模型来模拟）
		DP_DRIFTEND_FALLING, //漂移结束，抬起一侧落下直到压缩到最低的过程
		DP_DRIFTEND_REBOUNCE//上面一个阶段结束，被压缩一侧弹起的过程
	}DRIFTPERIOD;

	DRIFTPERIOD m_eDriftPeriod;

	MASSTRANSINFO m_massTrans[2];

	typedef enum      tagRUNNINGSTATES
	{
		RS_INAIR,
		RS_ONEARTH
	} RuningState;

	RuningState m_state;
	RuningState m_statePre;
	void SetContactState(RuningState st);
	//PhysX stuff

	NxScene* m_pNxScene;
	NxMaterialIndex m_nNoneBounceMaterial;

	//车身的位置
	NxVec3 m_CarGlobalPosition;

	NxVec3 m_vPosition;

	//the specification
	NxVec3 m_vSpecChassis;
	
	float m_fChassisScale;
	NxVec3 m_vSpecBase;
	NxReal m_fSpecWheelRadius;

	//suspension
	float m_fSusSpring;
	float m_fSusDamping;
	float m_fSusValue;
	float m_fSusRestitution;
	float m_fSusLimitPlane;

	//z
	float m_fZBases;
	float m_fZChassis;
	float m_fZPillar;
	float m_fZWheels;

	//anchors
	float m_fAnchorZSuspensions;
	float m_fAnchorZPillar;
	float m_fAnchorChassis;


	//mass 
	float m_fMassChassis;
	float m_fMassWheel;


	//the chassis of the car
	NxActor* m_pActorChassis;
	NxD6Joint* m_pChassisJoint;

	//the suspensions of the car
	NxD6Joint* m_pSuspensions[WHEELS];

	//the wheels of the car
	NxActor* m_pActorWheels[WHEELS];

	//the bases of the wheels
	NxActor* m_pActorBases[WHEELS];

	//the lifter of the drifting. left side and right side
	//NxActor* m_pActorLifters[LIFTERS];

	int m_nDriftingState;
	float m_fDriftAngle;

	NxVec3 m_vBasePoints[WHEELS];

	NxVec3 m_vWheelsPos[WHEELS];


	NxVec3 m_vGlobalAnchors[WHEELS];

	float m_fBasesPosition[WHEELS];
	float m_fBasesDisplacementZ[WHEELS];

	NxVec3 m_vptSideTip[2];



	//the pillar
	NxD6Joint* m_pPillar;
	NxActor* m_pActorPillarBase;
	NxActor* m_pActorPillar;

	void InitNx();
	void CreateNoneBounceMaterial();
	void SetMaterial(NxActor* pActor, NxMaterialIndex nMtr);
	void InitSpecification();
	bool CreateBases();
//	bool CreateDriftingLifters();
	bool CreateChassis();
	bool CreateWheels();
	bool CreateSuspensions();
//	bool CreatePillarBase();
	bool CreatePillarActor();
	bool CreatePillar();
	//bool CreateChassisJoint();
	//bool ReconfigureChassisJoint(NxD6Joint* pJoint);


	void Offset(NxVec3& pos);

	NxVec3 GetBasePointGlobal(int id);

	void MoveBase(int nBaseId, float fZ);	
	void LiftBase(int nBaseId, float fZ);

	void SetBasePosition(float fB0, float fB1, float fB2, float fB3);
	
	void SetLifterValue(int nDriftState, float fLiftDisp);
	void SetChassisPosition(NxVec3 pos);

//#ifdef _DEBUG
	void InputProcess(float dt);
//#endif

	NxCCDSkeleton* CreateCCDSkeleton(NxScene* pScene, float size);
	NxActor* CreateCCDBox(NxScene* pScene, const NxVec3& pos, 
		const NxVec3& boxDim, const NxReal density, 
		const NxVec3& centerOffset, bool doDynamicCCD);

	//NxCCDSkeleton* CreateCCDSphereSkeleton(NxScene* pScene, const float r, 
	//	const int nRings = 16, const int nSegments = 16);

	NxActor* ChassisSimulator::CreateCCDSphere(NxScene* pScene, 
		const NxVec3& pos, float fRadius, const NxReal fMass, bool doDynamicCCD);

	NxCCDSkeleton* GetCCDSphereSkeletonFromNif(float fRadius);



	NxActor* CreateSphere(NxScene* pScene, 
		const NxVec3& pos, float fRadius, const NxReal fMass, 
		NxVec3 centerOffset = NxVec3(0,0,0));

	NxActor* CreateBox(NxScene* pScene, const NxVec3& pos, 
		const NxVec3& boxDim,const NxVec3& centerOffset,
		const NxReal fMass);

	NxD6Joint* CreateD6Joint(NxScene* pScene, NxActor* a0, 
		NxActor* a1, const NxVec3& globalAnchor, 
		const NxVec3& globalAxis);

	void ReconfigureSuspension(int nJoint);

	void ReconfigurePillar(NxD6Joint* pJoint);

	void Update(float dt, NxVec3 Vel, const NxVec3& velLocal, const NxVec3& vNormalGrd, int nDrift);
	void UpdateDriftingLifter(float dt);
//	void MoveLifter(int nId, float fZ);
	void SetClissionable();
	//~PhysX stuff

	void InitReset();
	void EndReset();
	void Reset();
	void FreezeActor(NxActor* pAcotr);
	void FreezeAllActors();
	void MeltActor(NxActor* pAcotr);
	void MoveTo(NxActor* pActor, NxVec3 pos);
	void DestroyAll();

	NxVec3 GetWheelPosition(int nWheelId);
	int GetContactState();

	int WhoShouldBeKinematic(const NxVec3& vel);
	int FindMax(float* pArray, int num);
	void MeltAllWheels();
	void FreezeAllWheels();
	void CaculateDisplacements();
	void MoveWheel(int nWId);
	void MoveWheels(float disp);
	void UpdateFreezState(float dt, const NxVec3& velGlobal, const NxVec3& vNormalGrd);
	void UpdateState(float dt, const NxVec3& velGlobal, const NxVec3& vNormalGrd);
	int GetKinematicWheelNum();
	int GetTrend(const NxVec3& vNormalGrd, const NxVec3& velGlobal, float& alpha);
	void MoveKinematicWheels();


	//-debug
	//NxActor* m_wheelBalls[4];
	//void CreateWheelBalls();
	//void SetBallPosition(NxVec3 pos, int nIndex);

	//保存当前轮子位置到接触地面需要发生的位移。升高为正，降低为负
	float m_fDistToMove[WHEELS];
	float m_fHeightPre[WHEELS];
	float m_fHeightCur[WHEELS];

	float m_fDriftOffset[WHEELS];
	float m_fDirftTime;
	void MoveBasesWhenOnEarth();
	void MoveBaseWhenTouchGround();
	void MoveBaseWithDispWhenInAir();
	int GetPNInfo(float* pArray, int num);
	int GetMinPositiveElementIndex(float* pArray, int num);
	int GetMaxElementIndex(float* pArray, int num);
	int GetMaxNagtiveElementIndex(float* pArray, int num);
	void MoveBaseWithDisp(int nBaseId, float fDisp);
	bool CanIFly(float fDispZThisFrame);
	void SetDisplaceMent(float* pArray, int num);
	void SetHeight(float* pArray, int num);
	int GetLowestWheel();
	int GetHeightest();
//	void UpdateSideTip(float dt, const NxVec3& velLoal, int nDrift);

	float* GetDriftOffsets();

	bool LoadSuspensionConfig();
	void StartMassTranslationWhenDrifting(int nDriftState);
	void UpdateMassTranslation(float dt);

	void OnDriftStart(int nDriftState);
	void OnDriftEnd(int nDriftState);

	bool m_bDriftPeriodFinished;
	void SetDriftPeriod(DRIFTPERIOD eP);
	void UpdateDrift(float dt);
	void NewDriftPeriod(int nStateCur);

	//

	bool m_bIsFreezed;
	float GetDistToGround();//获取离地面最近的轮子距地的距离
	void UpdateState2(float dt, const NxVec3& velGlobal, const NxVec3& vNormalGrd);

	float m_fZofLowestWheel;//由于模型会锁定，记录锁定中的车轮位置
	float m_afZofWheels[4]; //记录轮子当前的位置
	int m_nLowestWheel;
	void SaveWheelsZ();
	void UpdateWheelsZ(float dist);
	void SetWheelsZ(float* zs);

	float m_fOrientationEulerXPre;
	float m_fOrientationEulerYPre;
	NxMat33 GetChassisOrientationSmooth();
	void SetGlobalPosition(NxVec3 pos);

	float m_afDistanceToGround[4];
	void SetDistanceToGround(float *aDist);
	void UpdateContactState();
};
