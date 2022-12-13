/********************************************************************
	����:	��wincoqin
	ʱ��:	  2007/01/26
    �ļ���:   CarMotorControl.h
	Ŀ��:     ��������
*********************************************************************/
#pragma once

// libfile
#include <vector>

// userfile
#include "TencentCar.h"
// predefine
#define MAX_ACCEL_STATE_NUM 6

class CCarMotorControl
{
	typedef std::map<float, float> Curve;
	typedef Curve::iterator     CurveIter;
	typedef CurveIter::const_iterator constCurveIter;

	typedef enum enmAccelStatus
	{
		EAS_FORWARDACC = 0,
		EAS_FORWARDDEC = 1,
		EAS_BACKWARDACC= 2,
		EAS_BACKWARDDEC= 3,
	}ENMACCELSTATUS;

	

public:
	CCarMotorControl();
	virtual ~CCarMotorControl();

	bool LoadConfig(const char * pcConfig);


	float GetScaleMotorForce(const float fCurSpeed, const float fSteering, 
		const DWORD dwCarMotionDir, const int nAccelStatus, const int nUseAccelItemStatus = 0);

	void	StartIntensiveAcce();		//��ʼǿ�Ҽ��٣����߼��٣�
	void	StopIntensiveAcce();		//ֹͣǿ�Ҽ��٣����߼��٣�

protected:
	// ECMD_FORWARD && nAccelStatus = 1
	void ProcessForwardAccel();
	// ECMD_FORWARD && nAccelStatus = -1
	void ProcessForwardDecel();
	// ECMD_FORWARD && nAccelStatus = 1
	void ProcessBackwardDecel();
	// ECMD_FORWARD && nAccelStatus = -1
	void ProcessBackwardAccel();

	float GetInterpolatePointFromCurve(const Curve & kCurve, const float fSteering, 
		const float fCurSpeed);

	bool InitConfig(CVar * pVar, Curve & kCurve);
	bool LoadAccelStatus(CVar * pVar);

public:
	void SetAcceParam ( float fParam ) {
		m_fArrItemAccelCoef[0] = fParam; 
	}
	float GetAcceParam () { return m_fArrItemAccelCoef[0];}

	// ���ٲ�����������
	void SetAcceCompensateParam(float fParam) { m_fCompensateParam = fParam; }
	

protected:
	float  m_fMotorForce;
	float  m_fSteering;
	float  m_fCurSpeed;
	int    m_nUseAccelItemStatus;	
	float  m_fArrItemAccelCoef[MAX_ACCEL_STATE_NUM];

	ENMACCELSTATUS   m_easOperationStatus;

	Curve m_kForwardAccelCurve;
	Curve m_kForwardDecelCurve;
	Curve m_kBackwardDecelCurve;
	Curve m_kBackwardAccelCurve;

	float m_fCompensateParam;
};
