#pragma once

//-----------------------------------------------------------------------
//            Create by Hoverzhao	
//-----------------------------------------------------------------------



#include <set>
#include "Play.h"
#include <NiCullingProcess.h>
#include "playerdata.h"
#include "NiLoopedThread.h"

class NxPlayMgr
{

	typedef std::set<Play*>::iterator PlayListIterator ;


protected:
	NxPlayMgr();
	~NxPlayMgr();

private:
	static std::set< Play * > m_AllPlay;
	static Play * m_pHostPlay;
	static std::set< Play * > m_AllRemotePlay;
public:

	static bool Init();
	static void Destory();

	bool InitThreads();
	void DestoryThreads();
	
	static void SetThreads( int nThreads ) 
	{ 
		if ( nThreads > NxPlayMgr::MAX_THREADS )
			nThreads = NxPlayMgr::MAX_THREADS;
		ms_uiCfgThreads = nThreads;
	} 
	
	static unsigned int ms_uiCfgThreads;


	static Play		*	CreatePlay( KPlayInfo & pInfo ,bool bHostPlay = false);

	static void		RemovePlay ( ULONG dwUin);

	static void		UpdateAllPlay(float fTime, float fFrameTime, bool bAnimationUpdate);


	static void		DrawAllPlay(NiCamera * pkCamera);
	//static void		RenderAllPlayPortrait();
	static void     RenderAllPlayName();// Add by wincoqin, for Render player's name

	static void		ShowAllPlayName(BOOL);
	static void		OnAllPlayLostDevice(bool bReset);

	//static void		PhysicAllPlay();
	static void		UpdateAllPhysicPlay(float fTime);
	static void     StopAllPhysicPlay();
	static void     StartAllPhysicPlay();
	static void		UnPhysicAllPlay();

	static void		ClearAllPlay();
	static void		ClearAllRemotePlay();
	static void		SetAllPlayAnim(NiFixedString strName);

	static Play		*	GetHostPlay(){ return m_pHostPlay ;}
	static Play		*	GetPlay(ULONG dwUin);

	static Play     *   GetNearPlayFromHostPlay();

	

	static  void	OnContactNotify(NxContactPair& pair, NxU32 events);


	static std::set< Play * > & GetAllPlay() { return m_AllPlay;}
	static std::set< Play * >  GetAllRemotePlay() ;


	//-----------------------------------------------------------------------

protected:

	static NxPlayMgr * ms_pNxPlayMgr;
	unsigned int m_uiMaxThreads;
	unsigned int m_uiNumThreads;

	void UpdateAllPlayThreads (float fTime, float fFrameTime, bool bAnimationUpdate);
	void DrawAllPlayThreads(NiCamera * pkCamera);
	enum
	{
		MAX_THREADS = 8,
		INITIAL_MODELS = 16
	};


public:
	class UpdatePlayTProc : public NiLoopedThreadProcedure
	{
	public:
		virtual bool LoopedProcedure(void* pvArg);

		static float ms_fTime;
		static float ms_fFrameTime;
		static bool  ms_bAnimationUpdate;

		PlayListIterator m_kPos;
		unsigned int m_uiChildren;
	};

	class DrawPlayTProc : public NiLoopedThreadProcedure
	{
	public:
		virtual bool LoopedProcedure(void * pvArg );
		static NiCamera * ms_pCamera;
		PlayListIterator m_kPos;
		unsigned int m_uiChildren;
	};
	
	UpdatePlayTProc m_akUpdateTProcs[MAX_THREADS];
	DrawPlayTProc   m_akDrawTProcs[MAX_THREADS];

	NiLoopedThread* m_apkUpdateThreads[MAX_THREADS];
	NiLoopedThread* m_apkDrawThreads[MAX_THREADS];




private:
/*
#ifndef _XCART_
	bool IsSafeVehicle(WheellessCar * pVehicle);
#else
	bool IsSafeVehicle(NxVehicle* pVehicle);
#endif
*/
};
