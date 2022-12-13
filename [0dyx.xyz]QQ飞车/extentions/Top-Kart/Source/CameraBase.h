////////////////////////////////////////////////
//     FileName:    CameraBase.h
//     Author:      Rayhuang
//     Date:        2007-3-7
//     Description: 相机基础类
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
		ECT_MODEL,			//模型自身相机
		ECT_RACE,			//比赛中用的跟随相机			
		ECT_RADAR,			//视点跟随相机，不改变相机位置，只改变角度
		ECT_AIRSCAPE,		//鸟瞰相机
		ECT_EAGLEMAP,		//小地图所用相机

		MAX_CAM_TYPES		//相机类型的总数
	}ENMCAMTYPE;

public:
	CCameraBase(ENMCAMTYPE eCamType);
	virtual ~CCameraBase();

	//更新相机
	virtual void Update(float fTime);

	//加载相机配置
	virtual void LoadConfig(CScriptState *pstate);

	//得到相机的类型
	unsigned int GetCameraType();
	
	//设置相机目标（为play，若目标不为play则置为NULL）
	void SetTarget(Play* pTarget);

	//播放相机动画
	void PlayAnim();

	//停止动画
	void StopAnim();

	//得到相机的动画的时间长度
	float GetAnimTime();

	//根据NiCamera设置相机
	void SetCamera(NiCameraPtr spCamera);

	//根据结点NiNode设置相机
	void SetCamera(NiNodePtr spCameraNode);

	//返回相机
	NiCameraPtr GetCamera();

	//调整相机广角(数据为成员数据）
	void AdjustFrustem();

	//调整相机广角
//	void AdjustFrustem(float fNear, float fFar, float fAspectRatio, float fFov);

	//设置相机位置
	void SetTranslate(NiPoint3 vec3Tran);

	//得到相机的位置
	NiPoint3 GetTranslate();

	//设置相机朝向
	void SetRotate(NiMatrix3 matRot);

	//得到相机的朝向
	NiMatrix3 GetRotate();

	//设置强制喷射镜头
	inline bool SetForceSpeedUp(bool bForce)
	{
		m_bForceSpeedUp = bForce;
		return m_bForceSpeedUp;
	}
	//是否强制喷射
	inline bool IsForceSpeedUp()
	{
		return m_bForceSpeedUp;
	}

protected:
	NiCameraPtr m_spCamera;
	NiNodePtr	m_spCameraNode;
	Play*		m_spTarget;

	ENMCAMTYPE	m_eCamType;

	bool	 m_bForceSpeedUp;	//强制喷射镜头
	
	NiPoint3 m_vec3Up;			//上方向
	NiPoint3 m_vec3Right;		//右方向
	NiPoint3 m_vec3Dir;			//视点方向
	NiPoint3 m_vec3Translate;	//相机的位置

	//相机的剪裁面数据
	float m_fNearPlane;			//近裁减面
	float m_fFarPlane;			//远裁减面
	float m_fAspectRotio;		//高宽比
	float m_fFov;				//广角

};
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
//			class CRaceCamera:
//描叙：	比赛中用的跟随相机
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
	//计算所得到的当前参数
	float m_fCurDis;		//当离车的距离
	float m_fNextDis;		//计算得到下次的距离
	float m_fRotAngle;		//沿右方向旋转的角度
	float m_fAngleScope;	//相机视点方向和模型前进方向的角度范围

	NiPoint3 m_vec3NextDir;	//下次的前进方向
	NiPoint3 m_vec3NextUp;	//下次的上方向

	//-------------------
	//测试效果
	NiPoint3 m_vec3DemoPreDir;
	float m_fCurHeight;
	
	//-------------------

	//相机的脚本配置参数
	float m_fMinDis;		//相机离车的最小距离
	float m_fMaxDis;		//相机离车的最大距离
//	float m_fNormalMaxDis;	//普通状态时，相机离车的最大距离
//	float m_fSpeedUpMaxDis;	//NO2时，相机离车的最大距离
	float m_fHeight;		//相机离地面的高度
	float m_fSpeedCoef;		//速度参数
	float m_fDistCoef;		//距离参数
	float m_fHeightCoef;	//高度参数
	float m_fDriftCoef;		//漂移时相机会更近
	
	float m_fRotCoef;		//绕右方向旋转角度参数	
	float m_fForwardCoef1;	//前进方向角度插值系数1（趋势，unit）
	float m_fForwardCoef2;	//前进方向角度插值系数2（scale）
	float m_fForwardCoef3;	//角速度拟和植（目的是当角速度越小的时候，相机变化趋势越大)
	float m_fUpCoef1;		//上方向角度插值系数1（趋势，unit）
	float m_fUpCoef2;		//上方向角度插值系数2（scale）

	//------------------------------------------------------------------------
	CLimitContainer<NiMatrix3>* m_arrRotMat;
	CLimitContainer<NiPoint3>* m_arrPos;

	//临时
	float m_fHeightRef;
	
};
//---------------------------------------------------------------------------------




//---------------------------------------------------------------------------------
//			class CModelCamera
//描叙：	模型自身相机
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
//描叙：	视点跟随相机，不改变相机位置，只改变角度
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
//描叙：	鸟瞰相机
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
//描叙：	小地图相机

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