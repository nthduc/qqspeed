/********************************************************************
	作者:	　wincoqin
	时间:	 2007/01/23
	目的:	 对场景节点进行整理， 对小物件进行渲染管理
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
	// 更新
	void CulledScene(NiCamera * pkCamera);
	// 加载策略
	bool InitParam(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius);
	// 初始化
	bool Init(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius);
	// 设置目标
	void SetNode(NiNode*);

protected:
	// 递归初始化
	void RecurviseInitScene(NiNode * pkNode);
	// 计算当前相机离绘制对象距离
	float CaculateDistance(NiAVObject * pkObj);
	// 计算绘制对象对相机的张角
	float CaculateAngle(NiAVObject * pkObj, float fDist);
	// 判断是否处于相机视线范围内
	bool  IsInCameraDir(NiAVObject * pkObj);

protected:
	NiTObjectArray<NiAVObjectPtr> m_arrVisibleScene;

	NiCamera * m_pkCurCamera;
	NiNodePtr  m_spCurNode;

	bool	   m_bInitialized;

	// 策略
	float	m_fAllVisibleDistance;
	float	m_fMinAngle;
	float   m_fMaxProcessBoundRadius;
};
