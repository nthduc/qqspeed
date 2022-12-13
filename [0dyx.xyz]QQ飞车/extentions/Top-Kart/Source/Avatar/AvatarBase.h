//     FileName:    AvatarBase.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar�߼�����
////////////////////////////////////////////////
#pragma once 
#include "ItemDefine.h"
#include <list>
using namespace std;

class CATWearer;
class CATBehaveBase;
class CATAlgorithmBase;
//CAvatarBase��Ҫ���ڱ���װ�����߼����ԣ�
//��װ���Ĺҽ���ʾ���뿪��ͨ��װ����ID��ϵ����
class CAvatarBase
{
public:
	CAvatarBase();

	virtual ~CAvatarBase();

	//������������װ������
	VOID Create(DWORD ID, AVATARPROPERTY tagPropt);

	//����
	VOID Update(float fTime);	

	//���ص���ID
	DWORD GetID();

	//ת������
	DWORD GPTransform(DWORD dwSource);

	//�ı��������ʱ��
	DWORD GPLifeTransfrom(DWORD dwCondition, DWORD dwLife);

	//�������Խӿ�
	PHYSICPROPERTY GetPhysicPropt();

	//�ı䶯��λ�������ж���
	DWORD AnimTransform(DWORD dwSource);

	//�Ƿ���Խ�������λ��
	BOOL CanSwitch();	

	//�Ƿ���Կ�������
	BOOL CanThroughFog();

	//�����������б��֣���Ϊ��
	VOID TriggerBehave(DWORD dwType, DWORD dwOccasion);

	//����avatar��ӵ����
	VOID SetWearer(CATWearer* pWearer);

protected:
	//��behave�ӻ�Ծ������ת�Ƶ��ǻ�Ծ������
	VOID DeactivateBehave(CATBehaveBase* pBehave); 

protected:
	CATWearer* m_pWearer;
	DWORD m_dwID;
	BOOL m_bCanSwitch;
	BOOL m_bNoFog;
	PHYSICPROPERTY m_tagPhyPropt;
	std::list<ANIMDATA> m_listAnim;
	std::list<CATBehaveBase*> m_listInactiveBehave;
	std::list<CATBehaveBase*> m_listActiveBehave;
	std::list<CATAlgorithmBase*> m_listGPTranAlgthm;
	std::list<CATAlgorithmBase*> m_listGPLifeAlgthm;
};