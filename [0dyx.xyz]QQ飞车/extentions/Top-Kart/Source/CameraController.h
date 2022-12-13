////////////////////////////////////////////////
//     FileName:    CameraController.h
//     Author:      Rayhuang
//     Date:        2007-3-11
//     Description: ���������
////////////////////////////////////////////////

#pragma once 

#include "CameraBase.h"

class CCameraController : public NiMemObject
{
public:
	CCameraController();
	~CCameraController();	

	//�����߼������eCamTypeΪ�߼�������ͣ�spCamΪ�����
	void AddLogicCamera(unsigned int eCamType, NiCameraPtr spCam = 0);

	//�����߼����
	void AddLogicCamera(CCameraBase* pkCamBase);

	//�����߼����
	CCameraBase* GetLogicCamera(unsigned int eCamType);

	//���õ�ǰ��Ծ��ǰ̨�߼����
	void SetForeCam(unsigned int eCamType);

	//���ص�ǰ��Ծ��ǰ̨�߼����
	CCameraBase* GetForeCam();

    //�����߼��������Ծ��������ж����ǰ̨��ֻ��һ����
	void ActivateLogicCam(unsigned int eCamType);

	//��ֹ�߼����
	void DeactivateLogicCam(unsigned int eCamType);

	//��ֹ���е��߼����
	void DeactivateAllCam();

	//����
	void Update(float fTime);

	//����ű�����
	void LoadConfig();

	//����ָ���ű�����( �³����в�ͬ����  hoverzhao_for new car ) 
	void LoadConfig( const char * pFilePath );

protected:
	//��ʼ�����߼����
	void Init();

private:
	//�߼��������
	CCameraBase* m_apkCamera[CCameraBase::MAX_CAM_TYPES];

	//��ǰ��Ծ��ǰ̨������±�
	unsigned int m_unForeCamID;

	//����Ƿ��Ծ
	bool m_bActiveCam[CCameraBase::MAX_CAM_TYPES];
};