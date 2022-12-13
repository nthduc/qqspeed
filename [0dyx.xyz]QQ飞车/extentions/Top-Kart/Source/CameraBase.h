////////////////////////////////////////////////
//     FileName:    CameraBase.h
//     Author:      Rayhuang
//     Date:        2007-3-7
//     Description: ���������
////////////////////////////////////////////////

#pragma once 

#include "NiNode.h"
#include "NiCamera.h"
#include "variant.h"
#include "Play.h"

template<class T>
class CLimitContainer
{
public:
	CLimitContainer(unsigned int unSize);
	~CLimitContainer();

	void Push(T tData);
	//void ReSize(unsigned int unSize);

	unsigned int Count();

	T Sum();
	T Average();

private:
	T* m_pContainer;

	unsigned int m_unSize;
	unsigned int m_unCurSize;
	unsigned int m_unBackID;

};
class CCameraBase : public NiMemObject
{
public:
	typedef enum enmCamType
	{
		ECT_MODEL,			//ģ���������
		ECT_RACE,			//�������õĸ������			
		ECT_RADAR,			//�ӵ������������ı����λ�ã�ֻ�ı�Ƕ�
		ECT_AIRSCAPE,		//������
		ECT_EAGLEMAP,		//С��ͼ�������

		MAX_CAM_TYPES		//������͵�����
	}ENMCAMTYPE;

public:
	CCameraBase(ENMCAMTYPE eCamType);
	virtual ~CCameraBase();

	//�������
	virtual void Update(float fTime);

	//�����������
	virtual void LoadConfig(CScriptState *pstate);

	//�õ����������
	unsigned int GetCameraType();
	
	//�������Ŀ�꣨Ϊplay����Ŀ�겻Ϊplay����ΪNULL��
	void SetTarget(Play* pTarget);

	//�����������
	void PlayAnim();

	//ֹͣ����
	void StopAnim();

	//�õ�����Ķ�����ʱ�䳤��
	float GetAnimTime();

	//����NiCamera�������
	void SetCamera(NiCameraPtr spCamera);

	//���ݽ��NiNode�������
	void SetCamera(NiNodePtr spCameraNode);

	//�������
	NiCameraPtr GetCamera();

	//����������(����Ϊ��Ա���ݣ�
	void AdjustFrustem();

	//����������
//	void AdjustFrustem(float fNear, float fFar, float fAspectRatio, float fFov);

	//�������λ��
	void SetTranslate(NiPoint3 vec3Tran);

	//�õ������λ��
	NiPoint3 GetTranslate();

	//�����������
	void SetRotate(NiMatrix3 matRot);

	//�õ�����ĳ���
	NiMatrix3 GetRotate();

	//����ǿ�����侵ͷ
	inline bool SetForceSpeedUp(bool bForce)
	{
		m_bForceSpeedUp = bForce;
		return m_bForceSpeedUp;
	}
	//�Ƿ�ǿ������
	inline bool IsForceSpeedUp()
	{
		return m_bForceSpeedUp;
	}

protected:
	NiCameraPtr m_spCamera;
	NiNodePtr	m_spCameraNode;
	Play*		m_spTarget;

	ENMCAMTYPE	m_eCamType;

	bool	 m_bForceSpeedUp;	//ǿ�����侵ͷ
	
	NiPoint3 m_vec3Up;			//�Ϸ���
	NiPoint3 m_vec3Right;		//�ҷ���
	NiPoint3 m_vec3Dir;			//�ӵ㷽��
	NiPoint3 m_vec3Translate;	//�����λ��

	//����ļ���������
	float m_fNearPlane;			//���ü���
	float m_fFarPlane;			//Զ�ü���
	float m_fAspectRotio;		//�߿��
	float m_fFov;				//���

};
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
//			class CRaceCamera:
//����	�������õĸ������
class CRaceCamera : public CCameraBase
{
public:
	CRaceCamera();
	virtual ~CRaceCamera();

	virtual void Update(float fTime);
	virtual void LoadConfig(CScriptState *pstate);

protected:
	bool IsSpeedUp();
	float GetAngleDelt(const NiPoint3 & curDir);


private:	
	//�������õ��ĵ�ǰ����
	float m_fCurDis;		//���복�ľ���
	float m_fNextDis;		//����õ��´εľ���
	float m_fRotAngle;		//���ҷ�����ת�ĽǶ�
	float m_fAngleScope;	//����ӵ㷽���ģ��ǰ������ĽǶȷ�Χ

	NiPoint3 m_vec3NextDir;	//�´ε�ǰ������
	NiPoint3 m_vec3NextUp;	//�´ε��Ϸ���

	//-------------------
	//����Ч��
	NiPoint3 m_vec3DemoPreDir;
	float m_fCurHeight;
	
	//-------------------

	//����Ľű����ò���
	float m_fMinDis;		//����복����С����
	float m_fMaxDis;		//����복��������
//	float m_fNormalMaxDis;	//��ͨ״̬ʱ������복��������
//	float m_fSpeedUpMaxDis;	//NO2ʱ������복��������
	float m_fHeight;		//��������ĸ߶�
	float m_fSpeedCoef;		//�ٶȲ���
	float m_fDistCoef;		//�������
	float m_fHeightCoef;	//�߶Ȳ���
	float m_fDriftCoef;		//Ư��ʱ��������
	
	float m_fRotCoef;		//���ҷ�����ת�ǶȲ���	
	float m_fForwardCoef1;	//ǰ������ǶȲ�ֵϵ��1�����ƣ�unit��
	float m_fForwardCoef2;	//ǰ������ǶȲ�ֵϵ��2��scale��
	float m_fForwardCoef3;	//���ٶ����ֲ��Ŀ���ǵ����ٶ�ԽС��ʱ������仯����Խ��)
	float m_fUpCoef1;		//�Ϸ���ǶȲ�ֵϵ��1�����ƣ�unit��
	float m_fUpCoef2;		//�Ϸ���ǶȲ�ֵϵ��2��scale��

	//------------------------------------------------------------------------
	CLimitContainer<NiMatrix3>* m_arrRotMat;
	CLimitContainer<NiPoint3>* m_arrPos;

	//��ʱ
	float m_fHeightRef;
	
};
//---------------------------------------------------------------------------------




//---------------------------------------------------------------------------------
//			class CModelCamera
//����	ģ���������
class CModelCamera : public CCameraBase
{
public:
	CModelCamera();
	virtual ~CModelCamera();

	virtual void Update(float fTime);
	virtual void LoadConfig(CScriptState *pstate);
};
//---------------------------------------------------------------------------------



//---------------------------------------------------------------------------------
//			class CRadarCamera :
//����	�ӵ������������ı����λ�ã�ֻ�ı�Ƕ�
class CRadarCamera : public CCameraBase
{
public:
	CRadarCamera();
	virtual ~CRadarCamera();

	virtual void Update(float fTime);
	virtual void LoadConfig(CScriptState *pstate);
};
//----------------------------------------------------------------------------------




//---------------------------------------------------------------------------------
//			class CAirScapeCamera :
//����	������
class CAirScapeCamera : public CCameraBase
{
public:
	CAirScapeCamera();
	virtual ~CAirScapeCamera();

	virtual void Update(float fTime);
	virtual void LoadConfig(CScriptState *pstate);

private:
	float m_fHeight;
};


//---------------------------------------------------------------------------------
//			class CAirScapeCamera :
//����	С��ͼ���

class CEagleMapCamera : public CCameraBase
{
public:
	CEagleMapCamera();
	virtual ~CEagleMapCamera();

	virtual void Update(float fTime);
	virtual void LoadConfig(CScriptState *pstate);

private: 
	float m_fHeight;
	NiPoint3	m_vec3NextRight;
};