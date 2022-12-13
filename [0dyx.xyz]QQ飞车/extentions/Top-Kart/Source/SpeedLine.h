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
		float fLifespan;//��������
		float fLastTime;//������ʱ��
		bool bActive;//�Ƿ��Ծ
		NiPoint3 vDir;//�����˶�����
		float fSpeed;//�ٶ�
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
	int m_anActiveParticles[8];//8�������ϵ�ǰ��Ծ��������

	int m_nParticlesNum;//���ӵĸ���
	int m_nMaxLineForward;

	typedef struct tagSpeedLineParam{
		float fThetaCone;//Բ׶����
		float fXCone;//Բ׶�����X����
		float fXCone1;//�ٶ��߿�ʼ���Ļ�׼����
		float fXCone2;//�ٶ��߽������Ļ�׼����
		float fLineWidth;//�ٶ��߻�׼���
		float fOffsetY;//��Ļ��ֱ�����ϵ�ƫ��
		float fSpeed;  //�ٶ�
		float fLifeSpan;//��������
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
