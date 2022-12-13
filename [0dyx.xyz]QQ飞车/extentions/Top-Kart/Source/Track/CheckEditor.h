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
const UINT conSearchDepth=8;	///������ȣ��������������ж��޷���ȡ·��

class CCheckPoint;
class CPropPoint;

///������̬���ߺлص�������
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
	float			fDistance;	///���ڵ㵽�յ����·������
	float			fTrackHeight;	///checkpoint��Ч����ĸ߶�ֵ��������checkpoint�������ж�·��
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
const float conLoadMapModify=0.15f; //todo:������ͼ����ʱz���겻��������,��ͼ�༭���Ľ��󳷳� pighuang 20070119
const UINT conMaxTransChkPtNum=200;	//���δ�Խ�켣���ɵ����checkpoint��

class CLinks
	: public NiMemObject
{
	typedef NiTPrimitiveArray<CCheckPoint*> ChkPointsPool;
	typedef list<CCheckPoint*> ChkPointsPoolForMap;
	typedef	std::map<int, ChkPointsPoolForMap >	CHKPNTPOOL_MAP;
public:
	CLinks();
	virtual ~CLinks();

	///��¼checkpoint�仯��Ϣ�ĵ�Ԫ
	typedef	struct tagPtPairUn
	{
		NiPoint3	kPos;		///���ӵ�ǰλ��
		CCheckPoint*	pPtFrom;	///Prev��,�ѿ��Ƿ�������ģʽ
		CCheckPoint*	pPtTo;	///Next��,�ѿ��Ƿ�������ģʽ
		tagPtPairUn();
		///== arithmetic
		bool operator== (const tagPtPairUn &insA) const;
	}PtPairUn;

	///����������checkpoint�仯�����о�����checkpoint�켣��¼��Ԫ�����˶��켣����
	typedef struct tagPtTrack
	{
		USHORT nNum;	///������checkpoint��
		CCheckPoint* arrPt[conMaxTransChkPtNum];	///������checkpoint
		tagPtTrack();
	}PtTrack;

	///����ĳ��·����Ƚ�������
	typedef struct tagSearchTrack
	{
		UINT nSearchDepth;	///�������
		UINT nLastIndex;	///���һ������From��To���е�������
		RunDirection enDir;	///Clock��������To�Ľڵ㣬AntiClock��������From�Ľڵ�
		PtPairUn insToPair;
		PtTrack	insTrack;	///������·��
		std::set<CCheckPoint*>seMark;	///����Ѿ��߹��Ĺ켣
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
		//��Ϊ����ʹ�ø���Ȧ��
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
	//todo:��д��checkpoint����ϵͳ  pig 20070418
	///��ȡָ��������������checkpoint֮�䣬���복�ӵ�ǰλ��kPos���Լ���ѯ��Χ���pLastFrom
	///���ڲ�ѯ���һ����ȷ�Χ�ڲ�ѯ���ӵ�ǰ����λ�ã����pLastFrom=NULL�����ȫcheckpoint��ѯ
	///�������PtPairUn
	void GetPtInTrack(const NiPoint3& kPos,PtPairUn* pLastPair,PtPairUn* pRecOut);
	///ָ���ϴμ�����������checkpoint�ԣ���������Ӿ�����checkpoint��켣�����ؽ����pTrack
	void GetTrackList(PtPairUn* pLast,PtPairUn* pCurr,PtTrack* pTrack);
	///�ݹ�����������GetTrackList()��bGetTrack=true�򷵻�ָ����·����=false�򲻴���·�����⣬ֻ����
		///insTrack��������ָ����ȵ��������������о����ĵ㶼�ŵ�seMark
	bool SearchDepth(SearchTrack* pSearchTrack,bool bGetTrack);
	///�ж�p1st��p2nd���˶��������Ƿ����ڲ��Ѵ���������ģʽ���⣬true������false�Ƿ���
		///��������Ҫ��ֱ�����ӹ�ϵ�ĵ㣬�м䲻�ܸ���������
	bool TestRightDir(CCheckPoint* p1st,CCheckPoint* p2nd);
	///////////////////////////////////////
	///��һ������������checkpoint����Ϸ��ʼʱ����0��1�㣨����ģʽʱ��0�����һ�㣩
	PtPairUn m_kLastPair;
	///��󾭹��Ĺ켣
	PtTrack	m_kLastTrack;
	///��ǰ���Ȧ������ʼΪ0
	int	m_iCurLapTmp;
	///��ǰ���Ȧ���ɼ�����С1
	UINT m_iBestLapTmp;
	///��Ȧ��
	int m_iTotalLapTmp;
	///�����Ĺ켣�ܼ�¼
	std::deque<PtPairUn>m_dqTrackList;
	///���һ�μ���GetPtInTrack()��ʱ��
	UINT m_nLastCalcTrackTi;
	///��һ�ε���UpdatePos()ʱ������
	NiPoint3 m_kLastUptPos;
	///��¼�Ƿ��Ѿ������˸�Ȧ�������ʾ
	std::set<int>m_seLapHits;





	bool LoadLinks(const char * path);
	bool Init();
	void UpdatePos(const NiPoint3 & kPos);
	CCheckPoint * GetLastRightCheckPoint(bool bAuto);
	bool IsLinksChain();
	void Clear();

	// Author: wincoqin
	// Operation: Add
	// Param: kRetPos ����ֵ(����kCurPos�ľ���ΪfDistance)�� kCurPos��ǰλ�ã� 
	// fDistance ����, pPreChkPtǰһ��CheckPoint��λ��,ʹ��ʱ����, Ϊ�ڲ�����
	// Return: Ϊtrue��ʾ��ȡ�ɹ�,����ʧ��
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
	// �õ�ʣ��·������
	float	            GetRestMileage();
	// ��ȡ�����ܳ�
	float	GetLapTotalLen();

	//�޸��ˣ� ray		���𣺼���͵�ǰλ�������checkpoint��
	CCheckPoint* GetNearlyCheckPt(const NiPoint3& curPos);

	//�õ���ǰλ�õ���һcheckpoint��
	CCheckPoint* GetPreChectPt(const NiPoint3& curPos); 
	

public:
	///����ͬģʽ�µõ��ߵ�������ж�أ�ģʽ�л�ʱ�ܴ���
	void	GPPointDealer(USHORT	nsGameBaseMode);
	void ResetGoPath()
	{ 
		m_vLinkGoPath.clear();
		m_pLastPathPt=NULL;
		m_bCanAddPath=true;	///����ѹ���
		//if ( m_pLastRightCheckPoint){
		//	AddGoPath(m_pLastRightCheckPoint);
		//}
	}
	///��checkpointָ��ѹ��GoPath·��
	void AddGoPath(CCheckPoint* pChk);
	std::vector<CCheckPoint*> &  GetLinkGoPath() { return m_vLinkGoPath;}
	///�������ѹ��m_vLinkGoPath��checkpoint,ģ��ʣ���·��checkpoint
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
	CCheckPoint *					m_pLastPathPt;	///��¼���һ��ѹ��m_vLinkGoPath�ĵ�
	bool m_bCanAddPath;	///�Ƿ�����ѹ��m_vLinkGoPath��
	//CCheckPoint *					m_pLinkPre;
	//CCheckPoint *					m_pLinkNext;

	NiPoint3						m_kLocalTranslate;
	NiMatrix3						m_kLocalRotate;

	bool							m_bChanged;
	RunDirection					m_enRunDir;

	//CCheckPoint *					m_pLastRightCheckPoint;

	RunInfo						    m_stRunInfo;

	float							m_fRestDistance;
	float							m_fSingleLapDistance;// һȦ����
	bool							m_bGotBestLapTime;
	float							m_fBestLapTime;// ��Ȧ��óɼ�
	float							m_fStartTime;

	NiTPrimitiveArray<IOnEnterPoint *>  m_arrRegisterPool;
	NiTransform						m_arrStartPtTransforms[MAX_PLAYER_NUM];
	///��̬���ߺлص�����ʵ��
	GPStaticBox	m_kGPStaticBoxReport;

	bool m_bIsFinishRace;
};

#include "CheckEditor.inl"