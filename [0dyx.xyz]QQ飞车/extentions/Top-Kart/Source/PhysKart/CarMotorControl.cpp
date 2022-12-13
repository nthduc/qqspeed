/********************************************************************
	作者:	　wincoqin
	时间:	  2007/01/26
    文件名:   CarMotorControl.cpp
	目的:   	
*********************************************************************/
#include "top-kartpch.h"

// libfile


// userfile
#include "CarMotorControl.h"
#include "stringex.h"
#include "common.h"

CCarMotorControl::CCarMotorControl()
{
	m_fMotorForce = 0;
	m_nUseAccelItemStatus = 0;
	m_fCompensateParam = 1.0f;

	for (int i = 0; i < MAX_ACCEL_STATE_NUM; ++i)
	{
		m_fArrItemAccelCoef[i] = 0;
	}
}

CCarMotorControl::~CCarMotorControl()
{

}

float CCarMotorControl::GetScaleMotorForce(const float fCurSpeed, 
											const float fSteering, 
											const DWORD dwCarMotionDir, 
											const int nAccelStatus,
											const int nUseAccelItemStatus)
{
	m_fSteering = fSteering;
	m_fCurSpeed = NxMath::abs(fCurSpeed);
	m_fMotorForce = 0;
	m_nUseAccelItemStatus = nUseAccelItemStatus;
	if (nUseAccelItemStatus > MAX_ACCEL_STATE_NUM)
		m_nUseAccelItemStatus = 0;

	// 没有按加速键也没有按减速
	if (nAccelStatus == 0) 
	{
		return m_fMotorForce;
	}

	// 根据车的行径状态采用不同方法获取引擎动力
	bool bForward = false;
	bool bAccel   = false;
	if (CTencentCar::ECMD_STOP != dwCarMotionDir && m_fCurSpeed > 0.1f)
	{
		if (dwCarMotionDir & CTencentCar::ECMD_FORWARD)
		{
			bForward = true;
			bAccel   = (nAccelStatus > 0);
		}
		else if (dwCarMotionDir & CTencentCar::ECMD_BACK)
		{
			bForward = false;
			bAccel   = (nAccelStatus < 0);
		}
	}
	else
	{
		bAccel = true;
		if (nAccelStatus > 0)
		{			
			bForward = true;
		}
		else
			bForward = false;
	}
	

	if (bForward && bAccel) 
	{
		m_easOperationStatus = EAS_FORWARDACC;
		ProcessForwardAccel();
	}
	else if (bForward && !bAccel)
	{
		m_easOperationStatus = EAS_FORWARDDEC;
		ProcessForwardDecel();
	}
	else if (!bForward && bAccel)
	{
		m_easOperationStatus = EAS_BACKWARDACC;
		ProcessBackwardAccel();
	}
	else
	{
		m_easOperationStatus = EAS_FORWARDDEC;
		ProcessBackwardDecel();
	}

	return m_fMotorForce;
}

bool CCarMotorControl::LoadConfig(const char * pcConfig)
{
	CVarTable MotorConfig("MotorSetting");

	bool bRet = false;
	
	if( bRet = CVar::LoadVarsFromFile(&MotorConfig, pcConfig))
	{
		bRet = bRet && LoadAccelStatus(MotorConfig.Element("AccelStatus"));
		bRet = bRet && InitConfig(MotorConfig.Element("ForwardAccel"), m_kForwardAccelCurve);
		bRet = bRet && InitConfig(MotorConfig.Element("ForwardDecel"), m_kForwardDecelCurve);
		bRet = bRet && InitConfig(MotorConfig.Element("BackwardAccel"), m_kBackwardAccelCurve);
		bRet = bRet && InitConfig(MotorConfig.Element("BackwardDecel"), m_kBackwardDecelCurve);
	}	

	return bRet;
}

bool CCarMotorControl::LoadAccelStatus(CVar * pVar)
{
	CVarArray * pVarArray = (CVarArray *) pVar;
	// 如果pVar为空或者类型不是数组类型，那么加载失败
	if (!pVarArray || (pVarArray->GetType() != VAR_ARRAY)) 
	{
		assert(0);
		return false;
	}

	int num = pVarArray->GetNum();
	num = num > MAX_ACCEL_STATE_NUM ? MAX_ACCEL_STATE_NUM : num;

	for (int i = 0; i < num; ++i)
	{
		m_fArrItemAccelCoef[i] = pVarArray->Element(i)->Float();
	}

	return true;
}

bool CCarMotorControl::InitConfig(CVar * pVar, Curve & kCurve)
{
	CVarArray * pVarArray = (CVarArray *) pVar;
	// 如果pVar为空或者类型不是数组类型，那么加载失败
	if (!pVarArray || (pVarArray->GetType() != VAR_ARRAY)) 
	{
		assert(0);
		return false;
	}
	
	kCurve.clear();
	
	int num = pVarArray->GetNum();

	CVarArray * pSubArray = NULL;
	for (int i = 0; i < num; ++i)
	{
		pSubArray = (CVarArray *) pVarArray->Element(i);
		
		if (!pSubArray || (pSubArray->GetType() != VAR_ARRAY) || pSubArray->GetNum() != 2) 
		{
			assert(0);
			return false;
		}

		// 赋值， 填充曲线
		kCurve[pSubArray->Element(0)->Float()] = pSubArray->Element(1)->Float();
	}

	return true;
}

void CCarMotorControl::ProcessForwardAccel()
{
	m_fMotorForce = GetInterpolatePointFromCurve(m_kForwardAccelCurve, m_fSteering, m_fCurSpeed) * m_fCompensateParam;
	//LogText("ForwardAccel = %f\n", m_fMotorForce);
}

void CCarMotorControl::ProcessForwardDecel()
{
	m_fMotorForce = GetInterpolatePointFromCurve(m_kForwardDecelCurve, m_fSteering, m_fCurSpeed);
	//LogText("ForwardDecel = %f\n", m_fMotorForce);
}

void CCarMotorControl::ProcessBackwardAccel()
{
	m_fMotorForce = GetInterpolatePointFromCurve(m_kBackwardAccelCurve, m_fSteering, m_fCurSpeed);
	//LogText("BackwardAccel = %f\n", m_fMotorForce);
}

void CCarMotorControl::ProcessBackwardDecel()
{
	m_fMotorForce = GetInterpolatePointFromCurve(m_kBackwardDecelCurve, m_fSteering, m_fCurSpeed);
	//LogText("BackwardDecel = %f\n", m_fMotorForce);
}

float CCarMotorControl::GetInterpolatePointFromCurve(const Curve & kCurve, 
													  const float fSteering, 
													  const float fCurSpeed)
{
	size_t size = kCurve.size();
	assert(size >= 2);

	if (size == 0)
		return 0.0f;
	
	constCurveIter IterEnd = kCurve.end();

	constCurveIter iterTemp = kCurve.begin();

	float x0 = 0.0f, y0 = 0.0f, x1 = 0.0f, y1 = 0.0f;
	float x = fCurSpeed, y = 0.0f;
	float fAccelCoef = 1.0f;
	if (m_easOperationStatus == EAS_FORWARDACC)
		fAccelCoef = m_fArrItemAccelCoef[m_nUseAccelItemStatus] ;

	if (fCurSpeed <= iterTemp->first)
		y = iterTemp->second;		
	else
	{
		for (constCurveIter iter = kCurve.begin(); iter != IterEnd; ++iter)
		{
			++iterTemp;

			if (iterTemp != IterEnd)
			{
				if (iterTemp->first >= fCurSpeed)
				{
					x0 = iter->first;
					y0 = iter->second;
					x1 = iterTemp->first;
					y1 = iterTemp->second;
					// 现性插值
					y = y0 + (x - x0) * (y1 - y0) / (x1 - x0);
					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				y = iter->second;
			}

		}
	}
	y *=  fAccelCoef;
	return y;
}
