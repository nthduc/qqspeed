#pragma once
#include "actor.h"
#include <map>
#include "QQCarLogicNet.h"
#include "./PhysKart/NxPhyActor.h"

									/*-----------------------------------------------------------------------
										Create by HoverZhao 2007.4.25 for Moster Npc 
									-----------------------------------------------------------------------*/

class CMonster;


class CMonsterMgr 
{
public:
	CMonsterMgr();
	~CMonsterMgr();

	typedef  enum   enumMonsterAction
	{
		MONSTER_ACTION_RUN = 0,
		MONSTER_ACTION_HURT,
		MONSTER_ACTION_STAND,
		MONSTER_ACTION_FRUN,
		MONSTER_ACTION_DIE,
		MONSTER_ACTION_ATTACK,
		MONSTER_ACTION_HAND,
		MONSTER_ACTION_IDLE,
		MONSERR_ACTION_SPELL,
		MONSTER_ACTION_MAX

	}ENUMMONSTERACTION;

	typedef struct tagMonster
	{
		ULONG		unID;
		float		fStartPos;
		float		fEndPos;
		float       fSpeed;
		ULONG		nSkill;
		NiPoint3    ActiveRegion[8];
		NiPoint3    MotionRegion[8];
		char		ResPath[MAX_PATH];

	}Monster, *PMonster;

	class CMonsterNet : public IQQCarLogicNet
	{
	public:
		CMonsterNet();
		~CMonsterNet();
	public:
		bool InitMonsterNet();
		void DestorMonsterNet();

		virtual void OnNetMessage ( UINT MsgID, void * pBody );

	protected:
		int m_iCookie;
		
	};

	class CMonsterEvent: public IQQCarLogicEvent
	{
		virtual void OnEvent( UINT EventID, void * pBody );
	};



public:
	static	CMonsterMgr *	Get()							{ return ms_pkMonsterMgr;}
	static	bool			Create();
	static	void			Destory();
	static	void			Update(float fTime);
	static  void			Draw( NiCamera * pkCamera );

	static  bool			IsArbi()						{ return ms_bIsArbi;}
	static  void			SetArbi( bool bIsArbi)			{ ms_bIsArbi = bIsArbi ;}

protected:
	bool		InitMonsterMgr();
	void		DestoryMonsterMgr();
	void		UpdateMonsterMgr(float fTime);
	void		DrawMonsterMgr( NiCamera * pkCamera );

public:
	CMonster *  CreateMonster ( Monster * pMonster);
	void		DispearMonster( ULONG unID );

	void		ActiveMonster	( ULONG  unID);
	void		DeactiveMonster	( ULONG  unID);
	CMonster *  GetMonster		( ULONG unID );

	std::set<Play*>  GetAllMonster () {
		std::set< Play * > MonsterList; MonsterList.clear();
		std::map< ULONG, CMonster* >::iterator iter = m_mapMonsters.begin();
			for( ; iter != m_mapMonsters.end(); iter++ ) {
				MonsterList.insert((Play*)iter->second);
			}
		return MonsterList;
	}


	

protected:
	std::map<ULONG,CMonster*>		m_mapMonsters;  
	static CMonsterMgr			*	ms_pkMonsterMgr;
	static CMonsterNet			*   ms_pkMonsterNet;
	static bool						ms_bIsArbi;

};

class CMonster : public Play , public NxPhyActor
{
public:
	CMonster();
	virtual ~CMonster(void);


public:
	void Update( float fTime );
	void Draw(NiCamera* pkCamera);


	bool InitMonster( CMonsterMgr::Monster * pMonster); 
	void UnInitMonster();
	void Active()		;
	void DeActive()		;
	bool IsActive()   { return m_bIsActive;}

	bool IsBrain()		{ return m_bIsBrain;	}
	void SetBrain()		;
	void RemoveBrain()  { m_bIsBrain = false;} 

	bool MonsterLook();

	DWORD GetID()			{ return m_unMonsterID ;}
	void SetID ( UINT unID ) { m_unMonsterID = unID; }


	class CMonsterBrain 
	{
	public:
		CMonsterBrain( CMonster * pMonster){ m_pMonster = pMonster ;}
		~CMonsterBrain(){};

	public:
		void Think( float fTime );

	protected:
		void    Look();
		Play  * GetFood();
		void	FindPath();
		void    ModifyMotion();
	
	protected:
		Play * m_kFood;
		CMonster * m_pMonster;

	};



protected:

	BOOL PlayAnimation(DWORD AnimID, BOOL bPlay);
	BOOL StopAnimation(DWORD AnimID, BOOL bPlay);

	bool InitLight( NiAVObject * pkNode );
	void LightMonster();
	void UnLigitMonster();

	//-----------------------------------------------------------------------
	virtual void AnimActivated(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
		float fEventTime) ;
	virtual void AnimDeactivated(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
		float fEventTime) ;
	virtual void TextKeyEvent(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, const NiFixedString& pcTextKey,
		const NiTextKeyMatch* pkMatchObject,
		float fCurrentTime, float fEventTime){}
	virtual void EndOfSequence(NiActorManager* pkManager,
		NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
		float fEventTime) ;
	//-----------------------------------------------------------------------


public:
	void		SetPos( const NiPoint3 & kPos )  { m_spRootNode->SetTranslate(kPos );}
	NiPoint3	GetPos( ) { return m_spRootNode->GetTranslate();} 
	NiPoint3    GetPhysXPos() { return GetPos();}
	void        SetPhysXPos(const NiPoint3& kPos ) { SetPos( kPos ) ; } 
	BOOL		ToggleSim(BOOL bToggle);
	BOOL		Attach(NiAVObject * spObj);
	BOOL		Detach(NiAVObject * pObj);
public:
	void		KillPlay( ULONG dwUin );


protected:
	NiActorManagerPtr			m_spActorMgr;
	NiNodePtr					m_spRootNode;
	NiLightPtr					m_spLightNode;
	NiTObjectArray<NiLightPtr>	m_arrMonsterLights;
	CMonsterBrain			*	m_kMonsterBrain;

private:
	ULONG		m_unMonsterID;
	bool		m_bIsActive;
	bool		m_bIsBrain;
	CMonsterMgr::Monster     m_stMonsterInfo;





};
