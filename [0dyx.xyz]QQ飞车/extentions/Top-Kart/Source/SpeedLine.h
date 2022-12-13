#pragma once



#include <NiSmartPointer.h>
#include <NiCamera.h>
#include <NiNode.h>
#include <NiScreenElements.h>
#include <NiScreenTexture.h>
#include <NiRenderTargetGroup.h>
#include <NiApplication.h>
//#include <vector>

class NiFrameRate;
NiSmartPointer(NiMaterialProperty);

class CSpeedLine
{
public:
	
	typedef struct tagParticle{
		float fLifespan;//生命周期
		float fLastTime;//经历的时间
		bool bActive;//是否活跃
		NiPoint3 vDir;//粒子运动方向
		float fSpeed;//速度
	}SpeedParticle;



	CSpeedLine(int nParticlesNum, NiCamera* pCam);
	~CSpeedLine(void);

	void Initialize();
	void Update(float dt, NiCamera* pCam, float fCarSpeedX);

	NiNodePtr GetParticleRoot(){return m_spRoot;}


private:
	
	float m_fKarSpeedX;
	bool m_bForward;

	NiNodePtr m_spRoot;

	NiCamera* m_pCamera;

	SpeedParticle* m_pParticles;
	int m_anActiveParticles[8];//8个方向上当前活跃的粒子数

	int m_nParticlesNum;//粒子的个数
	int m_nMaxLineForward;

	typedef struct tagSpeedLineParam{
		float fThetaCone;//圆锥顶角
		float fXCone;//圆锥顶点的X坐标
		float fXCone1;//速度线开始处的基准坐标
		float fXCone2;//速度线结束处的基准坐标
		float fLineWidth;//速度线基准宽度
		float fOffsetY;//屏幕竖直方向上的偏移
		float fSpeed;  //速度
		float fLifeSpan;//生命周期
		float fMinDisplaySpeed;
	} SpeedLineParam;

	SpeedLineParam m_stParamForward;
	SpeedLineParam m_stParamBackward;


	NiTransform m_kCamTrans;

	//NiScreenElementsPtr m_spScreenElm;
	NiTriShapePtr m_spLineShape;
	NiPoint2* m_pkTexCoords;
	NiPoint3* m_pkVertices;
	void ActiveParticles(int nDirection, int num);
	void UpdateIndividual(int nParticleIndex, float dt);
	void UpdateAllParticles(float dt);
	void ActiveSomeParticles(float dt);

	void Forward(int nParticleIndex, SpeedLineParam* pParam, bool bForward);
	void Backward(int nParticleIndex);
	void MakeReborn(int nParticleIndex);

	void GetGlobalPointOnCone(float x, float fThetaY, NiPoint3& vPoint, SpeedLineParam* pParam);

	void InitParametres();
	void UpdateParametres(float dt);

	void LoadSpecification();
};
