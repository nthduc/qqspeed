/********************************************************************
	����:	��wincoqin
	ʱ��:	 2007/01/23
	Ŀ��:	 �Գ����ڵ�������� ��С���������Ⱦ����
*********************************************************************/


#pragma once
#include "nimemobject.h"

class CVarTable;
class NiAVObject;
class NiTObjectArray;
class NiCamera;
class NiNode;
NiSmartPointer(NiNode);
NiSmartPointer(NiAVObject);

class CSceneHelper :
	public NiMemObject
{
public:
	CSceneHelper();
	virtual ~CSceneHelper(void);

public:
	// ����
	void CulledScene(NiCamera * pkCamera);
	// ���ز���
	bool InitParam(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius);
	// ��ʼ��
	bool Init(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius);
	// ����Ŀ��
	void SetNode(NiNode*);

protected:
	// �ݹ��ʼ��
	void RecurviseInitScene(NiNode * pkNode);
	// ���㵱ǰ�������ƶ������
	float CaculateDistance(NiAVObject * pkObj);
	// ������ƶ����������Ž�
	float CaculateAngle(NiAVObject * pkObj, float fDist);
	// �ж��Ƿ���������߷�Χ��
	bool  IsInCameraDir(NiAVObject * pkObj);

protected:
	NiTObjectArray<NiAVObjectPtr> m_arrVisibleScene;

	NiCamera * m_pkCurCamera;
	NiNodePtr  m_spCurNode;

	bool	   m_bInitialized;

	// ����
	float	m_fAllVisibleDistance;
	float	m_fMinAngle;
	float   m_fMaxProcessBoundRadius;
};
