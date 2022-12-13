#pragma once
#include "WheellessCar.h"
#include "../InputManager.h"
/* ���˼·��

	���ܵķ��� ����������ʻʱ  �ڲ�ͬ���ٶ� ������£� ֻ����� תһȦ��ʱ��Ҳ�ǲ�ͬ�� ��һ������ȹ�ϵ��
	
	���Լ����   ���ٶ�W =  w0 + V ��ƽ����. ��Ҳ����ʹ��������ʽ��

	Ư�Ƶ���� �����˳������ת
*/
class WheellessCar;

class CDriftControl
{
public:
	enum eDriftingStatus
	{
		DRIFT_NONE,
		DRIFT_START,
	};

public:
	CDriftControl(WheellessCar * pWheelCar);
	~CDriftControl(void);


public:
	void DriftControlUpdate(bool bkeyShift, bool bkeyUp, bool bkeyLeft, bool bkeyRight, float dt);
	void StartDrift(bool bkeyLeft,bool bkeyRight);
	void EndDrift();
	void UpdateDrift(bool keyShift, bool keyUp, bool keyLeft, bool keyRight, float dt);
	void UpdateDriftKeyUp(bool keyShift, bool keyUp, bool keyLeft, bool keyRight, float dt);


	int  GetDriftStatus()  { return m_nDriftStatus;}
	NxReal	GetDriftingAngle() { return m_fDriftingAngle;}
	NxVec3  GetDriftGlobalVec() { return m_vDriftGlobalVel; }
	int		GetDriftDir() { return m_nDriftDir;}

	NxReal     GetDriftingAngleAccel();

protected:
	void	SetDriftStatus(eDriftingStatus nStatus )	{ m_nDriftStatus = nStatus;}
	void	UpdateDriftTrack();
	NxReal    GetAngleBetweenVelAndDrift();
	

protected:
	void	ModifyClockAngleFactor();
	float	GetClockAngle();

	
//-----------------------------------------------------------------------

	// ��Ư�� ֮  ������
	enum eAngleDriftMagical
	{
		EAM_NONE,
		EAM_EFFECT,
		EAM_EFFECTCENTER,
		EAM_BANA,
		EAM_FOG,
		EAM_MISSILE
	};

public:
	void	CaculateMagicalCenterPos(NxReal AngleVel);
	NxVec3	GetMagicalCenterPos() { return m_fCenter_AngelDrifting;}
protected:
	void  ResetAngelDrifting() { m_fAngle_AngelDrifting = 0.0f; }
	void  StopMagical();

	
	void  UpdateAngelDrifting(float vdt, NxReal AngleVel);

	int   GenerateMagical();
	void  TriggerMagical(int eMagical);
	
	void  GetAngelRadio();

public:
	bool  CanAngleDrifting() { return m_nIsAngelDrifting != 0; }

private:
	float m_fAngle_AngelDrifting;
	float m_fRadio_AngelDrifting;
	NxVec3 m_fCenter_AngelDrifting;
	std::vector<int> m_vecEffect;
	unsigned int  m_nIsAngelDrifting;

//-----------------------------------------------------------------------
	// ��Ư��֮ �貨΢��


protected:
	void IncVecPower ( NxReal AngelVel ,float dt);
	float GetVecPower ();
	void  ClearVecPower(){ m_fVecPower_AngelDrifting = 0.0f; m_fAccmulatePower = 0.0f;}

private:
	float m_fAccmulatePower;
	float m_fVecPower_AngelDrifting;




protected:

	WheellessCar * m_pWheelCar;
	int			   m_nDriftStatus;


protected:
	NxReal			m_nMinDriftingTriggerVel;
	NxReal			m_fDriftingTotalTime;


protected:
	NxVec3			m_vDriftGlobalVel;		//  Ư�Ʒ����ٶ�
	int				m_nDriftDir;			//  Ư�Ʒ��� ֻ������֮��
	NxReal			m_fDriftingAngle;		//  Ư�ƽǶ�
	NxReal			m_fDriftingAngleVel;    //  Ư�ƽ��ٶ�
	NxReal			m_fDriftingAngleVelMax;
	NxReal			m_fDriftingSideAngleCur; // ����̧�Ƕ�
	NxReal			m_fDriftingSideAngleMax;
	NxReal			m_fDriftingTime;

protected:
	bool			_bCreateNewTrack;
	int				_driftingType;
	NxU32			_collisionFlags;
	NxReal			m_fMaxFricAccel;
	NxReal			m_fDirftFactor;

	NiInputSystem	* m_pInputSystem;


protected:
	void ShiftProcess( bool bKeyShift , float dt) ;
	bool GetShiftDecide();
	NxReal			m_fShiftStartTime;
	NxReal			m_fShiftLstTime;	// ��סshit ����ʱ��
	NxReal			m_fShiftMaxLstTime;
	NxReal			m_fShiftRlsTime;


};
