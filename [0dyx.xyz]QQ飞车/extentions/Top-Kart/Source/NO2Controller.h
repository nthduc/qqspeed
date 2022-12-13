////////////////////////////////////////////////
//     FileName:    NO2Controller.h
//     Author:      Rayhuang
//     Date:        2007-2-6
//     Description: С�����������
////////////////////////////////////////////////


#pragma once 

#include "singlet.h"

class CNO2Controller: public CSingleT<CNO2Controller>
{
public:
	CNO2Controller();
	~CNO2Controller();

	//����С����
	void ActivateNO2();

	//ֹͣС����
	void DeactivateNO2();

	//����ֹͣ����ʱ��
	void SetTerminateTime(unsigned int unElapse);

	//����С���Ƿ��ڼ���״̬
	bool GetActive();

	//����
	void Update();

private:

	unsigned int	m_unTerminateTime;			//ֹͣ��ʱ��
	unsigned int	m_unStartTime;				//��ʼ��ʱ��
	bool			m_bIsActive;				//�Ƿ��ڼ���״̬

};
