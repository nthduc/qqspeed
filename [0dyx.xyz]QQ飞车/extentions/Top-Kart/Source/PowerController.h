////////////////////////////////////////////////
//     FileName:    PowerController.h
//     Author:      Rayhuang
//     Date:        2007-3-21
//     Description: 能量条控制，包括个人赛和组队赛
////////////////////////////////////////////////


#ifndef POWERCONTROLLER_H
#define POWERCONTROLLER_H

#include "RenderBase.h"
#include "singlet.h"

class CBarItem;
class CBlinkItem;


//单人竞速赛氮气积累
class CSinglePower : public CRenderBase2D, public NiMemObject
{
public:
	CSinglePower();

	~CSinglePower();

	//更新，此处处理积累氮气的逻辑
	virtual void Update(float fTime);

	//渲染
	virtual void Draw(NiRendererPtr spRenderer);

	//设置活动性
	virtual void SetActivity(bool bActive);

	//积累氮气失败（如碰撞时要调用此接口）
	void AccumulateFail();

	//积累氮气成功
	void AccumulateSucs();

	//积累氮气（实时表现，并不一定成功）
	void AccumulatePower(unsigned int unPower);

	//积累氮气（实时表现，并不一定成功）
	void SetCurPower(unsigned int unPower);

	//重置氮气
	void Reset();

	//设置车辆积累氮气量的参数
	void SetKartCoef(float fCoef);

	//载入脚本
	void LoadConfig();

private:
	CBarItem	* m_pBarItem;		//氮气条
	CBlinkItem	* m_pBlinkItem;		//闪烁动画

	unsigned int m_unCurPower;			//当前氮气
	unsigned int m_unPrePower;			//上次氮气（当fail时，当前氮气应该为此值）
	unsigned int m_unMaxPower;			//最大氮气
	unsigned int m_unCompensatePower;	//当漂移速度增加时氮气的补偿值

	unsigned int m_unAnimPower;		//动画氮气值（只为了表现氮气递减的效果）
	bool m_bIsAnim;					//是否播放动画

	float m_fCurSpeed;				//当前速度（作积累氮气用）
	float m_fPreSpeed;				//上一帧速度

	float m_fKartCoef;			//这个参数根据每辆车的属性来配（可变的）。积累氮气参赛


	bool m_bIsCrash;			//TODO:是否碰撞，用于碰撞结束漂移的判断（待物理接口完善后去掉）
};


//组队氮气
class CTeamPower : public CRenderBase2D, public NiMemObject
{
public:
	CTeamPower();
	~CTeamPower();

	//更新，此处处理积累氮气的逻辑
	virtual void Update(float fTime);

	//渲染
	virtual void Draw(NiRendererPtr spRenderer);

	//设置活动性
	virtual void SetActivity(bool bActive);

	//积累氮气
	void AccumulatePower(unsigned int unPower);

	//设置当前氮气值
	void SetPower(unsigned int unPower);

	//重置
	void Reset();

	//载入脚本
	void LoadConfig();

private:

	CBarItem * m_pBarItem;			//氮气条
	CBlinkItem * m_pBlinkItem;		//闪烁动画

	unsigned int m_unCurPower;		//当前氮气值
	unsigned int m_unMaxPower;		//最大氮气值

	unsigned int m_unAnimPower;		//动画氮气值（只为了表现氮气递减的效果）
	bool m_bIsAnim;					//是否播放动画
};



//氮气控制类（包括控制单人竞速和组队赛的氮气）
class CPowerController : public CSingleT<CPowerController>
{
public:
	CPowerController();

	~CPowerController();

	//返回单人竞速氮气对象
	CSinglePower* GetSinglePower();

	//返回组队氮气对象
	CTeamPower*	GetTeamPower();

	//开始单人竞速氮气对象（设活跃性为true）
	void StartSgPower();

	//结束单人竞速氮气对象（设活跃性为false）
	void TerminateSgPower();

	//开始组队竞速氮气对象（设活跃性为true）
	void StartTeamPower();

	//开始组队竞速氮气对象（设活跃性为false）
	void TerminateTeamPower();

	//结束所有的氮气
	void StopAllPowerCtrl();

private:
	CSinglePower * m_pSinglePower;	//单人竞速氮气对象

	CTeamPower * m_pTeamPower;		//组队氮气对象
};

#endif