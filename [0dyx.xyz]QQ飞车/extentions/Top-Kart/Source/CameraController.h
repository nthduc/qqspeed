////////////////////////////////////////////////
//     FileName:    CameraController.h
//     Author:      Rayhuang
//     Date:        2007-3-11
//     Description: 相机控制类
////////////////////////////////////////////////

#pragma once 

#include "CameraBase.h"

class CCameraController : public NiMemObject
{
public:
	CCameraController();
	~CCameraController();	

	//增加逻辑相机，eCamType为逻辑相机类型，spCam为相机体
	void AddLogicCamera(unsigned int eCamType, NiCameraPtr spCam = 0);

	//增加逻辑相机
	void AddLogicCamera(CCameraBase* pkCamBase);

	//返回逻辑相机
	CCameraBase* GetLogicCamera(unsigned int eCamType);

	//设置当前活跃的前台逻辑相机
	void SetForeCam(unsigned int eCamType);

	//返回当前活跃的前台逻辑相机
	CCameraBase* GetForeCam();

    //激活逻辑相机，活跃相机可以有多个（前台的只有一个）
	void ActivateLogicCam(unsigned int eCamType);

	//终止逻辑相机
	void DeactivateLogicCam(unsigned int eCamType);

	//终止所有的逻辑相机
	void DeactivateAllCam();

	//更新
	void Update(float fTime);

	//载入脚本配置
	void LoadConfig();

	//载入指定脚本配置( 新车会有不同配置  hoverzhao_for new car ) 
	void LoadConfig( const char * pFilePath );

protected:
	//初始化各逻辑相机
	void Init();

private:
	//逻辑相机数组
	CCameraBase* m_apkCamera[CCameraBase::MAX_CAM_TYPES];

	//当前活跃的前台相机的下标
	unsigned int m_unForeCamID;

	//相机是否活跃
	bool m_bActiveCam[CCameraBase::MAX_CAM_TYPES];
};