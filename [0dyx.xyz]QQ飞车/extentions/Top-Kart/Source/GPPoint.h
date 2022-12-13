////////////////////////////////////////////////
//FileName:    PropPoint.h
//Author:      PigHuang
//Date:        2007-01-19
//Description: ���߷��õ�
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

///���ߵ������update���ʱ��(ms)
const UINT conPropPointUpdateStepTime=800;
///·���ϵ��ߺ�Ĭ��������ʱ��
const UINT conStaticBoxReCreateTime=3000;

///���߷��õ������
class	CGPPointMgr
	:	public NiMemObject 
{
public:
	///���ߵ������
	typedef enum enmGPPointType
	{
		EM_STATIC_BOX	=	1,	///��ͼ�ռ��ﾲ̬�ĵ��ߺ�
		EM_DYNAMIC_PROP	=	2	///��̬�ĵ��ߣ������㽶Ƥ��������
	}GPPOINTTYPE;
	///���߷��õ�����
	typedef struct tagParam
	{
		float	m_fRadius;		///��ײ��뾶
		NiPoint3	m_kTranslate;	///��������
		NiMatrix3	m_kRotate;		///������ת����	
		NiNodePtr m_pkBox;		///����ģ��
		CGamePropCollisionReport*	m_pGPReport;	///�ص��ӿ�
		USHORT	m_nType;		///���ߵ�����
		USHORT	m_nTypeAux;		///���ߵ����͸������ݣ����ڵ��ߺ��ǵ��ߺ��ڵ�ͼ�ϵı�ţ�
		///EM_STATIC_BOXʹ��
		USHORT	m_nGroupId;		///����������ı��,Ԥ�����ܣ�EM_STATIC_BOXʹ��
		DWORD	m_dwGPID;		///���߱�ţ�EM_DYNAMIC_PROPʹ��
		DWORD	m_dwGPIndex;	///EM_DYNAMIC_PROPʹ��
		tagParam::tagParam();
	}Param;
	///���ߵ�ͳһ�ص���
	class GPPointReport : public CGamePropCollisionReport
	{
	public:
		virtual OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData);
	};
	///��ʼ��
	CGPPointMgr();
	virtual ~CGPPointMgr();
	////////////////////////////////////////////
	///��ʼ���������ߵ���,ʹ�ñ���ǰ������
	static	void	Init(void);
	///�ͷ�������Դ,��������ͷ�ʱ����
	static	void	Destroy(void);
	///�������ߵ㣬ʧ�ܷ���NULL
	static	CGPPoint*	CreatePoint(CGPPointMgr::Param* pParam, bool bActive = true);
	///ͨ��Idɾ�����ߵ㣬�ɹ�����true
	static	bool	RemovePoint(USHORT ushId);
	///���õ��ߺ�ģ�ͳԵ��������ļ��ʱ��(ms),ֻ�Ե��ߺ�ģ����Ч
	static	void	SetReCreateTime(UINT	nStepTi);
	///��ȡ���ߺ�ģ�ͳԵ��������ļ��ʱ��(ms),ֻ�Ե��ߺ�ģ����Ч
	static	UINT	GetReCreateTime(void);
	///ͨ��Id��ȡ���ߵ�
	static	CGPPoint*	GetPoint(USHORT ushId);
	///������е��ߵ�
	static	void	CleanAllPoint(void);
	///�����ߺе�ʱ�䲢��������
	static	void	UpdateStaticBox(void);
	///��ȡ���ߵ�ͳһ�ص�ʵ��������������������
	static	GPPointReport*	GetReportTrigger(void);

protected:
	static	CRITICAL_SECTION critSec;
	static	std::map<USHORT,CGPPoint*>	ms_arrPropPoint;	///���ߵ�������
	static	USHORT	ms_nLastPtIdx;	///��¼��󴴽���point��������
	static	UINT ms_nReCreateTime;	///���ߺгԵ��������ļ��ʱ��(ms)
	static	UINT ms_nLastUpdateTime;	///��¼���һ��UpdateStaticBox��������
		///ʱ�䣬�����ظ�Ƶ������
	static	GPPointReport	ms_kPointReport;	///���ߵ�������ײͳһ�ص�
};

///////////////////////////////////////////////
///���߷��õ���
class	CGPPoint
	:	public NiMemObject 
{
	friend class CGPPointMgr;
public:

	///����CGPPointMgr::CreatePoint��������GPPoint
	CGPPoint();
	virtual ~CGPPoint();
	///���õ��ߵ��Ƿ񼤻�,����״̬����ʾ����ģ�Ͳ�������ײ����
	bool	SetActive(bool bAct);
	///��ȡ���ߵ��Ƿ񼤻�״̬
	bool	GetActive(void);
	///��ȡ�ص��ӿ�
	CGamePropCollisionReport*	GetReport(void);
	///��ȡ������������
	const CGPPointMgr::Param*	GetParam(void);
	///��ȡ���һ���л�״̬��ʱ��
	UINT	GetLastStaChgTime(void);
	///ȡ�ñ��
	USHORT GetID();
	///��¼���Ѿ���������
	bool GetTriggered();
	bool SetTriggered(bool);

protected:
	USHORT	m_nIndexId;		///������
	bool	m_bActive;		///�����ߵ��Ƿ񼤻�״̬	
	CGPPointMgr::Param	m_stParam;		///��������
	UINT	m_nLastStaChgTime;	///���һ���л�״̬��ʱ��(::GetTickCount())
	HACTOR	m_hActor;
	bool	m_bTriggered;
};