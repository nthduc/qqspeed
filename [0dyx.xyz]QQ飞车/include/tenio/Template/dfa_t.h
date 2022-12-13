//////////////////////////////////////////////////////////////////////////
//	DFA (Deterministic Finite Automaton)
//  简易的确定有限状态机模板
//  说明： 
//		一、背景：
//		有限状态机的常用实现方式是， 采用switch/case或者if/else，这
//  无意是最直观的方式，使用一堆条件判断，对简单小巧的状态机来说最合适，
//  但是,这样的方式比较原始，对庞大的状态机难以维护。
//
//      所以，试图用模板的方式来简化，并让 状态的变迁和规则 清晰化。
//
//      二、使用方法：
//		1、定义状态集合
//
//		typedef enum enmState
//		{
//			state_1,
//			state_2,
//			... ...
//			state_n
//		};
//
//		2、增加状态变迁路径（规则三元组）
//
//			AddStateTransform(state_1, Message_1, Action_1, state_2);
//			AddStateTransform(state_2, Message_2, Action_2, state_2);
//			... ...
//			AddStateTransform(state_8, Message_8, Action_8, state_n);
//		
//		3、在逻辑处理的适当时机变迁状态
//
//			TransformState(Message_x);
//
//		4、启动状态机（从最初的状态开始）
//
//			StartFSM(start_state);
//
//
//						   --- by Coolhomli  [7/6/2006]
//
//////////////////////////////////////////////////////////////////////////


#pragma once

#include <list>
#include <map>
using namespace std;

namespace Tenio
{
	template<class RealClass>
		class TDFA
	{
	public:
		typedef void (RealClass::*TRANSFORMACTION)();
		typedef struct tagTransformArc
		{   			
			TRANSFORMACTION pAction;
			int nToState;
		}
		TRANSFORMARC;
		
		typedef map<int, map<int, TRANSFORMARC> >  STATE2TRANSFORMARCMAPMAP;
		
	protected:
		TDFA()
		{
			m_bStart = FALSE;
		}
		
		void StartDFA(int nStartState)
		{
			m_bStart = TRUE;
            m_nCurrentState = nStartState;
		}
		
		void StopDFA(int nStopState)
		{
			m_nCurrentState = nStopState;
			m_bStart = FALSE;
		}
		
		void AddStateTransform(int nFromState, UINT nMessage, TRANSFORMACTION pAction, int nToState)
		{
            TRANSFORMARC stTransformArc = {0};

			stTransformArc.pAction = pAction;
			stTransformArc.nToState = nToState;
			
			STATE2TRANSFORMARCMAPMAP::iterator it = m_mapState2TransformArc.find(nFromState);
			
			if (it != m_mapState2TransformArc.end()) 
			{
				map<int, TRANSFORMARC>& mapTransformArcRefer = it->second;
                mapTransformArcRefer[nMessage] = stTransformArc;
			}
			else
			{   
				map<int, TRANSFORMARC> mapTransformArc;
                mapTransformArc[nMessage] = stTransformArc;
				m_mapState2TransformArc[nFromState] = mapTransformArc;
			}
		}
		
		BOOL TransformState(UINT nMessage)
		{   
			BOOL bReturn = FALSE;

			if (m_bStart) 
			{
				STATE2TRANSFORMARCMAPMAP::iterator iterMapMap = m_mapState2TransformArc.find(m_nCurrentState);
				
				if (iterMapMap != m_mapState2TransformArc.end()) 
				{
					map<int, TRANSFORMARC>& mapTransformArc = iterMapMap->second;  
					
					map<int, TRANSFORMARC>::iterator iterMap = mapTransformArc.find(nMessage);
					
					if(iterMap != mapTransformArc.end())
					{   
						bReturn = TRUE;
						TRANSFORMARC& stTransformArc = iterMap->second;
						m_nCurrentState = stTransformArc.nToState;
						TRANSFORMACTION pAction = stTransformArc.pAction;
						if(pAction != NULL)
						{
							(Me()->*pAction)();	
						}										
					}				
				}
			}
			
			return bReturn;
		}
		
		int CurrentState()
		{
			return m_nCurrentState;
		}
		
		void SetCurrentState(int nState)
		{
			m_nCurrentState = nState;
		}
		
	private:
		
	private:
		RealClass* Me()
		{
			return static_cast<RealClass*>(this);
		}
		
		STATE2TRANSFORMARCMAPMAP m_mapState2TransformArc;
		int m_nCurrentState;
		BOOL m_bStart;
	};	
};