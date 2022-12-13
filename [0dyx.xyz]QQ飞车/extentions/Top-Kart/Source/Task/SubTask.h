////////////////////////////////////////////////
//     FileName:    SubTask.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: ������
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "AtomLevel.h"
#include "Observer.h"
#include "AtomBase.h"
#include "GarbagePool.h"
#include <queue>
using namespace std;

class CSubTask : public CObserver, public CSource
{
	friend class CTask;
public:
	CSubTask();
	~CSubTask();
	bool Init(const PSUBTASKCFG& pSubTCfg);	//��ʼ��������
	void Start();				//��ʼִ��������
	void Update(float fTime);	//����
	DWORD GetState();			//�����������״̬������׼���ڣ�ִ���ڣ���ɣ�ʧ�ܣ�
	void SetState(DWORD dwState);	//����״̬	
	void UpdateMessage();		//�������²��շ�����Ϣ�����²�ͨ�ţ�
private:
	CAtomLevel* _PopAtomLevel();//����һ��level 
	void _Finish();				//�����
	void _Fail();				//ʧ����
	
private:
	queue<CAtomLevel*> m_qAtomLevel;	//level����
	CAtomLevel* m_pCurAtomLevel;		//��ǰ��Ծ��level
	list<CAtomBase*> m_listSubTaskPeriodAtom;	//�����������ڴ��ڵ�ԭ������
	PSUBTASKCFG m_pSubTaskCfg;			//�����������
	CGarbagePool<CAtomLevel> m_levelGarbagePool;	//level�������ճ�
	DWORD m_dwState;		//������ĵ�ǰ״̬
};
