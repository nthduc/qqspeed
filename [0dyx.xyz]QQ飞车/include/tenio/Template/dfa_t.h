//////////////////////////////////////////////////////////////////////////
//	DFA (Deterministic Finite Automaton)
//  ���׵�ȷ������״̬��ģ��
//  ˵���� 
//		һ��������
//		����״̬���ĳ���ʵ�ַ�ʽ�ǣ� ����switch/case����if/else����
//  ��������ֱ�۵ķ�ʽ��ʹ��һ�������жϣ��Լ�С�ɵ�״̬����˵����ʣ�
//  ����,�����ķ�ʽ�Ƚ�ԭʼ�����Ӵ��״̬������ά����
//
//      ���ԣ���ͼ��ģ��ķ�ʽ���򻯣����� ״̬�ı�Ǩ�͹��� ��������
//
//      ����ʹ�÷�����
//		1������״̬����
//
//		typedef enum enmState
//		{
//			state_1,
//			state_2,
//			... ...
//			state_n
//		};
//
//		2������״̬��Ǩ·����������Ԫ�飩
//
//			AddStateTransform(state_1, Message_1, Action_1, state_2);
//			AddStateTransform(state_2, Message_2, Action_2, state_2);
//			... ...
//			AddStateTransform(state_8, Message_8, Action_8, state_n);
//		
//		3�����߼�������ʵ�ʱ����Ǩ״̬
//
//			TransformState(Message_x);
//
//		4������״̬�����������״̬��ʼ��
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