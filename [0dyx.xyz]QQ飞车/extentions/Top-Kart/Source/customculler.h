////////////////////////////////////////////////
//     FileName:    customculler.h
//     Author:      Fish
//     Date:        2007-3-5 11:11:53
//     Description: �Զ���ļ��ù���(С�������)
////////////////////////////////////////////////

#ifndef __CUSTOMCULLER_H__
#define __CUSTOMCULLER_H__

#include "NiCullingProcess.h"

class CCustomCullingProcess : public NiCullingProcess
{
public:
	CCustomCullingProcess(NiVisibleArray* pkVisibleSet);
	bool InitParam(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius);

protected:
	virtual void AppendVirtual(NiGeometry& kVisible);

private:
	float ComputerDistance(NiGeometry * pkObj);
	float ComputerAngle(NiGeometry * pkObj, float fDist);

private:
	// ����
	float	m_fAllVisibleDistance;
	float	m_fMinAngle;
	float   m_fMaxProcessBoundRadius;
};
#endif
