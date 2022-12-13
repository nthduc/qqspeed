////////////////////////////////////////////////
//     FileName:    ATBehaveBase.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar���б���
////////////////////////////////////////////////

#pragma once 
#include "ItemDefine.h"
#include "ATWearer.h"


//װ�����еı��λ���
class CATBehaveBase
{
public:
	CATBehaveBase(BEHAVEDATA tagBD);
	virtual ~CATBehaveBase();
	virtual void Start();				//��ʼ����
	virtual void Stop();				//ֹͣ����
	virtual void Update(float fTime);	//����
	void SetWearer(CATWearer* pTarget);	//��������ߣ�Ŀ�����
	void SetLifeTime(DWORD dwLift);		//����������
	void SetOccasion(DWORD dwOccasion);	//���ô���ʱ��
	void SetCondition(DWORD dwCondt);	//���ô�������
	void SetParam(DWORD dwParam);		//���ö������

	DWORD GetType();
	DWORD GetOccasion();				//���ش���ʱ��
	DWORD GetConditioin();				//���ش�������
	bool IsActive();					//�Ƿ񼤻�

protected:
	CATWearer* m_pWearer;
	DWORD m_dwType;			//����
	DWORD m_dwOccasion;		//����ʱ��
	DWORD m_dwCondition;	//��������
	DWORD m_dwLife;			//��������
	DWORD m_dwParam;		//����
	DWORD m_dwOption;		//���ɲ���
	DWORD m_dwStartTime;	//��ʼʱ��
	bool  m_bIsAcive;		//�Ƿ��ڼ���״̬	
};


