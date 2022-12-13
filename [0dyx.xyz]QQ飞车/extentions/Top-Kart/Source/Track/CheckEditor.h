#pragma once
#include "nimemobject.h"
#include "NiPoint3.h"
#include "NiRefObject.h"
#include "../GamePropCollisionReport.h"
#include "../LevelManager.h"
#include "common.h"
#include "stack"
#include <list>
#include <map>
#include <deque>

#define MAX_PLAYER_NUM 8
const UINT conSearchDepth=8;	///搜索深度，超过这个深度则判定无法获取路径

class CCheckPoint;
class CPropPoint;

///赛道静态道具盒回调函数类
class GPStaticBox : public CGamePropCollisionReport
{
public:
	virtual OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData);
};

class IOnEnterPoint
	: public NiMemObject
{
public:
	virtual void OnLapUp() = 0;
	virtual void OnEnterPoint(CCheckPoint * pChkPt) = 0;
	virtual void OnLeavePoint(CCheckPoint * pChkPt) = 0;
};



#define MAX_TEXT_LEN 4096

class CCheckPoint 
	: public NiMemObject
{
public:
	CCheckPoint(const NiPoint3 & kPt, 
		const NiMatrix3 & kRot,
		const NiFixedString name);
	~CCheckPoint();

	void AddLinkTo(CCheckPoint * pCkPt);
	void AddLinkFrom(CCheckPoint * pCkPt);

	void DelLinkTo(CCheckPoint * pCkPt);
	void DelLinkFrom(CCheckPoint * pCkPt);


	void AddLinkToName(const NiFixedString & name);
	void SetLinkToNameLink(const NiFixedString & name, CCheckPoint * pCkPt);

	void AddLinkFromName(const NiFixedString & name);
	void SetLinkFromNameLink(const NiFixedString & name, CCheckPoint * pCkPt);

	void HighLight();
	void Draw();

	void SetDistance(float distance);

	bool	  m_bHiLight;

	NiPoint3		kPoint;
	NiMatrix3		kRotate;
	NiFixedString	kName;
	NiFixedString   kParentName;
	float			fRadius;
	float			fDistance;	///本节点到终点最短路径距离
	float			fTrackHeight;	///checkpoint有效处理的高度值，超过则本checkpoint不参与判断路径
	int				nIndex;

	struct ChkPtPtrPointWithName
		: public NiMemObject
	{
		NiFixedString	kName;
		CCheckPoint *	pkChkPt;
	};

	NiTPrimitiveArray<ChkPtPtrPointWithName*>  arrLinkFrom;
	NiTPrimitiveArray<ChkPtPtrPointWithName*>  arrLinkTo;
};


enum RunDirection
{
	dirUncertain	 = 0,
	dirClockWise	 = 1,
	dirAntiClockWise = 2,
};

#define MAX_LAP_LEN   100000000
#define MAX_POOLS_NUM 10
const float conLoadMapModify=0.15f; //todo:修正地图读入时z坐标不贴地问题,地图编辑器改进后撤除 pighuang 20070119
const UINT conMaxTransChkPtNum=200;	//单次穿越轨迹容纳的最大checkpoint数

class CLinks
	: public NiMemObject
{
	typedef NiTPrimitiveArray<CCheckPoint*> ChkPointsPool;
	typedef list<CCheckPoint*> ChkPointsPoolForMap;
	typedef	std::map<int, ChkPointsPoolForMap >	CHKPNTPOOL_MAP;
public:
	CLinks();
	virtual ~CLinks();

	///记录checkpoint变化信息的单元
	typedef	struct tagPtPairUn
	{
		NiPoint3	kPos;		///车子当前位置
		CCheckPoint*	pPtFrom;	///Prev点,已考虑反向赛道模式
		CCheckPoint*	pPtTo;	///Next点,已考虑反向赛道模式
		tagPtPairUn();
		///== arithmetic
		bool operator== (const tagPtPairUn &insA) const;
	}PtPairUn;

	///车子在两次checkpoint变化过程中经过的checkpoint轨迹记录单元，按运动轨迹排序
	typedef struct tagPtTrack
	{
		USHORT nNum;	///经过的checkpoint数
		CCheckPoint* arrPt[conMaxTransChkPtNum];	///经过的checkpoint
		tagPtTrack();
	}PtTrack;

	///按照某种路径深度进行搜索
	typedef struct tagSearchTrack
	{
		UINT nSearchDepth;	///搜索深度
		UINT nLastIndex;	///最后一个点在From或To链中的索引号
		RunDirection enDir;	///Clock代表搜索To的节点，AntiClock代表搜索From的节点
		PtPairUn insToPair;
		PtTrack	insTrack;	///搜索的路径
		std::set<CCheckPoint*>seMark;	///标记已经走过的轨迹
		tagSearchTrack()
		{
			nSearchDepth=conSearchDepth;
			nLastIndex=0;
			enDir=dirClockWise;
		}
	}SearchTrack;

	struct RunInfo 
	{   
		RunInfo();
		~RunInfo();

		typedef enum enmRunStatus
		{
			ERS_RIGHT = 0,
			ERS_WRONG = 1,
			ERS_UNKNOWN = 2,
		}ENMRUNSTATUS;

		void		  AddCheckPointToCurLap(CCheckPoint * pChkPt);
		CCheckPoint * GetLastRightCheckPoint(bool bAuto, bool bReallyRight = false);
		bool		  IsPointNext(CCheckPoint * pChkPtSrc, CCheckPoint * pChkPtDest);
		bool		  IsPointFrom(CCheckPoint * pChkPtSrc, CCheckPoint * pChkPtDest);
		void		  Clear();
		void		  AddWarningTime();
		void		  ResetWarningTime();

		RunDirection	enCurRightDir;
		//unsigned int	uiCurLap;
		//unsigned int	uiTotalLap;
		//ChkPointsPool	arrChkPtPools[MAX_POOLS_NUM];
		//改为可以使用负数圈数
		//int	iCurLap;
		//int	uiTotalLap;
		//CHKPNTPOOL_MAP	arrChkPtPools;		

		//NiFixedString   kStartPointName;   
		unsigned int    uiWarningTime;
		float		    fWarningStartTime;
		//CCheckPoint *   pkStartCheckPt;
		//std::stack<CCheckPoint *>	m_stackErrorPoint;
		//unsigned int    uiWrongLaps;
	};

	// operation
public:
	//todo:新写的checkpoint处理系统  pig 20070418
	///获取指定坐标在哪两个checkpoint之间，输入车子当前位置kPos，以及查询范围起点pLastFrom
	///即在查询起点一定深度范围内查询车子当前所在位置，如果pLastFrom=NULL则进行全checkpoint查询
	///结果放在PtPairUn
	void GetPtInTrack(const NiPoint3& kPos,PtPairUn* pLastPair,PtPairUn* pRecOut);
	///指定上次及本次所处的checkpoint对，计算出车子经过的checkpoint点轨迹，返回结果在pTrack
	void GetTrackList(PtPairUn* pLast,PtPairUn* pCurr,PtTrack* pTrack);
	///递归搜索，用于GetTrackList()，bGetTrack=true则返回指定的路径，=false则不处理路径问题，只根据
		///insTrack的起点进行指定深度的搜索，并把所有经过的点都放到seMark
	bool SearchDepth(SearchTrack* pSearchTrack,bool bGetTrack);
	///判断p1st向p2nd点运动是正向还是反向，内部已处理反向赛道模式问题，true是正向，false是反向
		///这两点需要有直接链接关系的点，中间不能隔了其他点
	bool TestRightDir(CCheckPoint* p1st,CCheckPoint* p2nd);
	///////////////////////////////////////
	///上一次所处的两个checkpoint，游戏开始时就是0，1点（反向模式时是0和最后一点）
	PtPairUn m_kLastPair;
	///最后经过的轨迹
	PtTrack	m_kLastTrack;
	///当前完成圈数，开始为0
	int	m_iCurLapTmp;
	///当前最好圈数成绩，最小1
	UINT m_iBestLapTmp;
	///总圈数
	int m_iTotalLapTmp;
	///经过的轨迹总记录
	std::deque<PtPairUn>m_dqTrackList;
	///最后一次计算GetPtInTrack()的时间
	UINT m_nLastCalcTrackTi;
	///上一次调用UpdatePos()时的坐标
	NiPoint3 m_kLastUptPos;
	///记录是否已经触发了该圈的相关提示
	std::set<int>m_seLapHits;





	bool LoadLinks(const char * path);
	bool Init();
	void UpdatePos(const NiPoint3 & kPos);
	CCheckPoint * GetLastRightCheckPoint(bool bAuto);
	bool IsLinksChain();
	void Clear();

	// Author: wincoqin
	// Operation: Add
	// Param: kRetPos 返回值(距离kCurPos的距离为fDistance)， kCurPos当前位置， 
	// fDistance 距离, pPreChkPt前一个CheckPoint的位置,使用时不填, 为内部参数
	// Return: 为true表示获取成功,否则失败
	bool GetForwardPoint(NiPoint3 & kRetPos, NiPoint3& kRetDir, const NiPoint3 & kCurPos, float fDistance, CCheckPoint * pPreChkPt = NULL);

	void Register(IOnEnterPoint * spEnterPoint);
	void UnRegister(IOnEnterPoint * spEnterPoint);

	//void UpdateRotationAndTrans(const NiNode * pkNode);
	const NiPoint3 & GetTranslation();
	const NiMatrix3& GetRotate();
	void SetTranslation(const NiPoint3 & kPoint);
	void SetRotate(const NiMatrix3 & kMatrix);
	void ApplyChange();
	RunDirection GetCurDir(NiPoint3 & kDirection);	
	void ClearRecord();

	const NiPoint3 & GetCheckPointPos(int index);
	const NiFixedString & GetCheckPointName(int index);
	int			     GetCheckPointsNum();

	bool ShouldShowWarning();
	bool ShouldReset();
	void ResetWarning();
	void AddWarningTime();
	void AddResetPoint();


	void Draw();
	void SetCorrectRunDir(RunDirection enRunDir);
	RunDirection GetCorrectRunDir();
	void SetLaps(unsigned int uiLaps);
	unsigned int GetCurLaps();
	unsigned int GetTotalLaps();
	void		 StartRecord();		 
	float		 GetBestLapTime();

	const NiTransform & GetStartPointTransform(unsigned int uiIndex) const;
	// 得到剩余路径长度
	float	            GetRestMileage();
	// 获取赛道总长
	float	GetLapTotalLen();

	//修改人： ray		描叙：计算和当前位置最近的checkpoint点
	CCheckPoint* GetNearlyCheckPt(const NiPoint3& curPos);

	//得到当前位置的上一checkpoint点
	CCheckPoint* GetPreChectPt(const NiPoint3& curPos); 
	

public:
	///处理不同模式下得道具点载入与卸载，模式切换时能处理
	void	GPPointDealer(USHORT	nsGameBaseMode);
	void ResetGoPath()
	{ 
		m_vLinkGoPath.clear();
		m_pLastPathPt=NULL;
		m_bCanAddPath=true;	///可以压入点
		//if ( m_pLastRightCheckPoint){
		//	AddGoPath(m_pLastRightCheckPoint);
		//}
	}
	///把checkpoint指针压入GoPath路径
	void AddGoPath(CCheckPoint* pChk);
	std::vector<CCheckPoint*> &  GetLinkGoPath() { return m_vLinkGoPath;}
	///根据最后压入m_vLinkGoPath的checkpoint,模拟剩余的路程checkpoint
	void SimWinGoPath(void);
	bool IsFinishRace();

	std::vector<CCheckPoint*>		m_vLinkGoPath;


protected:
	CCheckPoint * GetCheckPointByName(const NiFixedString & name);

	void		  RecuriseCaculateDistance(CCheckPoint *& pCheckPt, CCheckPoint *& pFatherPt, const NiFixedString & kHeadName);

	void		  UpdatePreCheckPt(CCheckPoint * pChkPt);
	void		  UpdateNextCheckPt(CCheckPoint * pChkPt, RunDirection dir);
	  

	ChkPointsPool					m_arrpCheckPoints;
	CCheckPoint *					m_pLinkHead;
	CCheckPoint *					m_pLastPathPt;	///记录最后一次压入m_vLinkGoPath的点
	bool m_bCanAddPath;	///是否允许压入m_vLinkGoPath点
	//CCheckPoint *					m_pLinkPre;
	//CCheckPoint *					m_pLinkNext;

	NiPoint3						m_kLocalTranslate;
	NiMatrix3						m_kLocalRotate;

	bool							m_bChanged;
	RunDirection					m_enRunDir;

	//CCheckPoint *					m_pLastRightCheckPoint;

	RunInfo						    m_stRunInfo;

	float							m_fRestDistance;
	float							m_fSingleLapDistance;// 一圈长度
	bool							m_bGotBestLapTime;
	float							m_fBestLapTime;// 单圈最好成绩
	float							m_fStartTime;

	NiTPrimitiveArray<IOnEnterPoint *>  m_arrRegisterPool;
	NiTransform						m_arrStartPtTransforms[MAX_PLAYER_NUM];
	///静态道具盒回调函数实例
	GPStaticBox	m_kGPStaticBoxReport;

	bool m_bIsFinishRace;
};

#include "CheckEditor.inl"