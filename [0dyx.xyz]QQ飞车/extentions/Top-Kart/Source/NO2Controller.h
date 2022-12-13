////////////////////////////////////////////////
//     FileName:    NO2Controller.h
//     Author:      Rayhuang
//     Date:        2007-2-6
//     Description: 小喷连喷控制器
////////////////////////////////////////////////


#pragma once 

#include "singlet.h"

class CNO2Controller: public CSingleT<CNO2Controller>
{
public:
	CNO2Controller();
	~CNO2Controller();

	//激活小喷检测
	void ActivateNO2();

	//停止小喷检测
	void DeactivateNO2();

	//设置停止检测的时长
	void SetTerminateTime(unsigned int unElapse);

	//返回小喷是否处于激活状态
	bool GetActive();

	//更新
	void Update();

private:

	unsigned int	m_unTerminateTime;			//停止的时长
	unsigned int	m_unStartTime;				//开始的时间
	bool			m_bIsActive;				//是否处于激活状态

};
