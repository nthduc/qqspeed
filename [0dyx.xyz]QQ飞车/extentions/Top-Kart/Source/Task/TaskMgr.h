////////////////////////////////////////////////
//     FileName:    TaskMgr.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: ���������
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "TaskSys.h"
#include "singlet.h"

class CTaskMgr : public CSingleT<CTaskMgr>
{
public:
	CTaskMgr();
	~CTaskMgr();
	//��ʼ��
	void Init();

	//��������dwID
	bool EnterTask(CNotifySingleGameBeginPara* pPara);

	//�˳�����
	void LeaveTask();

	//����
	void Update(float fTime);

	//����ĳ���������
	PTASKCONFIG FindTaskCfg(DWORD dwID);

	//���ص�ǰ��Ϸ
	CTask* GetCurTask(); 

	//����������Ϣ�����������������棬�����棬��ɵģ�
	void AddTaskInfo(TASKINFO tagTask);

	//����idΪdwid���������Ϣ
	PTASKINFO GetTaskInfo(DWORD dwID);	

	//���������������������ģ�
	void AddTaskPack(DWORD dwPackID);

	//���õ�ǰ�������
	void SetCurTaskPack(DWORD dwPackID);

	//���ص�ǰ���������Ϣ
	PTASKPACKCFG GetCurTaskPack(); 	

	//����lua state
	CScriptState* GetLuaState();

	//�ж��Ƿ�����������
	bool IsFinishNewHandTask(DWORD dwID);


private:
	//������������
	void _LoadConfig();

	//����������Ϣ
	void _UpdateTaskInfo();

	//����idΪdwID������״̬
	DWORD _GetTaskAccessState(DWORD dwID);

	//����idΪdwID������״̬
	void  _SetTaskAccessState(DWORD dwID, DWORD dwState);
	

private:
	CScriptState*		m_pLuaState;
	TASKINFO_LIST		m_listTaskInfo;		//�����ʱ��״̬��Ϣ
	PTASKPACKCFG		m_pCurTPackCfg;		//��ǰ���������Ϣ
	TASKPACKCFG_LIST	m_listTPackCfg;		//�����������Ϣ
	TASKCONFIG_LIST		m_listTaskCfg;		//�������������
	CTask*				m_pCurTask;			//��ǰ����
};