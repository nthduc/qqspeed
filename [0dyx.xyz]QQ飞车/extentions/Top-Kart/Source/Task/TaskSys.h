////////////////////////////////////////////////
//     FileName:    TaskSys.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: ����
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "SubTask.h"
#include "AtomBase.h"
#include "Observer.h"
#include "GarbagePool.h"
#include <queue>
using namespace std;

class CTask : public CObserver
{
public:
	CTask();
	~CTask();
	bool Init(PTASKCONFIG pTaskCfg);	//��ʼ��
	void Start();						//��ʼ
	void Finish();						//���
	void Fail();						//ʧ��
	void Update(float fTime);			//����
	PTASKCONFIG GetTaskCfg();			//�������������
	DWORD GetState();					//��������ĵ�ǰ״̬
	void UpdateMessage();				//�����շ���Ϣ

private:
	CSubTask* _PopSubTask();			//�����µ�������
	void _InitSwitch();					//��ʼ�����ر�С��ͼ���������ȣ�

private:
	queue<CSubTask*>		m_qSubTask;				//���������
	CSubTask*				m_pCurSubTask;			//��ǰ������
	list<CAtomBase*>		m_listPeriodAtom;		//���������ڵ�ԭ������
	PTASKCONFIG				m_pTaskCfg;				//��������
	DWORD					m_dwState;				//����ĵ�ǰ״̬
	CGarbagePool<CSubTask>	m_subTaskGarbagePool;	//�������������ճ�
};
