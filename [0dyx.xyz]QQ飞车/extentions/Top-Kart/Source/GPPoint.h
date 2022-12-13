////////////////////////////////////////////////
//FileName:    PropPoint.h
//Author:      PigHuang
//Date:        2007-01-19
//Description: 道具放置点
////////////////////////////////////////////////

#pragma once

#include "NiMemObject.h"
#include "NiPoint2.h"
#include "NiPoint3.h"
#include "NiMatrix3.h"
#include "NiRefObject.h"
#include "Top-KartPCH.h"
#include "GamePropCollisionReport.h"
#include "physxtask.h"

class CGPPoint;

///道具点管理类update间隔时间(ms)
const UINT conPropPointUpdateStepTime=800;
///路面上道具盒默认重生的时间
const UINT conStaticBoxReCreateTime=3000;

///道具放置点管理类
class	CGPPointMgr
	:	public NiMemObject 
{
public:
	///道具点的类型
	typedef enum enmGPPointType
	{
		EM_STATIC_BOX	=	1,	///地图空间里静态的道具盒
		EM_DYNAMIC_PROP	=	2	///动态的道具，例如香蕉皮，大糖泡
	}GPPOINTTYPE;
	///道具放置点特性
	typedef struct tagParam
	{
		float	m_fRadius;		///碰撞球半径
		NiPoint3	m_kTranslate;	///放置坐标
		NiMatrix3	m_kRotate;		///放置旋转矩阵	
		NiNodePtr m_pkBox;		///道具模型
		CGamePropCollisionReport*	m_pGPReport;	///回调接口
		USHORT	m_nType;		///道具点类型
		USHORT	m_nTypeAux;		///道具点类型辅助数据（对于道具盒是道具盒在地图上的编号）
		///EM_STATIC_BOX使用
		USHORT	m_nGroupId;		///所属道具组的编号,预留功能，EM_STATIC_BOX使用
		DWORD	m_dwGPID;		///道具编号，EM_DYNAMIC_PROP使用
		DWORD	m_dwGPIndex;	///EM_DYNAMIC_PROP使用
		tagParam::tagParam();
	}Param;
	///道具点统一回调类
	class GPPointReport : public CGamePropCollisionReport
	{
	public:
		virtual OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData);
	};
	///初始化
	CGPPointMgr();
	virtual ~CGPPointMgr();
	////////////////////////////////////////////
	///初始化整个道具点类,使用本类前续调用
	static	void	Init(void);
	///释放所有资源,本类最后释放时调用
	static	void	Destroy(void);
	///创建道具点，失败返回NULL
	static	CGPPoint*	CreatePoint(CGPPointMgr::Param* pParam, bool bActive = true);
	///通过Id删除道具点，成功返回true
	static	bool	RemovePoint(USHORT ushId);
	///设置道具盒模型吃掉后重生的间隔时间(ms),只对道具盒模型有效
	static	void	SetReCreateTime(UINT	nStepTi);
	///获取道具盒模型吃掉后重生的间隔时间(ms),只对道具盒模型有效
	static	UINT	GetReCreateTime(void);
	///通过Id获取道具点
	static	CGPPoint*	GetPoint(USHORT ushId);
	///清除所有道具点
	static	void	CleanAllPoint(void);
	///检查道具盒点时间并重新生成
	static	void	UpdateStaticBox(void);
	///获取道具点统一回调实例，用于配置物理引擎
	static	GPPointReport*	GetReportTrigger(void);

protected:
	static	CRITICAL_SECTION critSec;
	static	std::map<USHORT,CGPPoint*>	ms_arrPropPoint;	///道具点数据链
	static	USHORT	ms_nLastPtIdx;	///记录最后创建的point的索引号
	static	UINT ms_nReCreateTime;	///道具盒吃掉后重生的间隔时间(ms)
	static	UINT ms_nLastUpdateTime;	///记录最后一次UpdateStaticBox函数运行
		///时间，避免重复频繁运行
	static	GPPointReport	ms_kPointReport;	///道具点物理碰撞统一回调
};

///////////////////////////////////////////////
///道具放置点类
class	CGPPoint
	:	public NiMemObject 
{
	friend class CGPPointMgr;
public:

	///请用CGPPointMgr::CreatePoint函数创建GPPoint
	CGPPoint();
	virtual ~CGPPoint();
	///设置道具点是否激活,激活状态下显示道具模型并参与碰撞测试
	bool	SetActive(bool bAct);
	///获取道具点是否激活状态
	bool	GetActive(void);
	///获取回调接口
	CGamePropCollisionReport*	GetReport(void);
	///获取其他参数对象
	const CGPPointMgr::Param*	GetParam(void);
	///获取最后一次切换状态的时间
	UINT	GetLastStaChgTime(void);
	///取得编号
	USHORT GetID();
	///记录它已经被触发了
	bool GetTriggered();
	bool SetTriggered(bool);

protected:
	USHORT	m_nIndexId;		///本点编号
	bool	m_bActive;		///本道具点是否激活状态	
	CGPPointMgr::Param	m_stParam;		///其他参数
	UINT	m_nLastStaChgTime;	///最后一次切换状态的时间(::GetTickCount())
	HACTOR	m_hActor;
	bool	m_bTriggered;
};