////////////////////////////////////////////////
//     FileName:    ATWearer.h
//     Author:      Rayhuang
//     Date:        2007-4-26
//     Description: avatarӵ����
////////////////////////////////////////////////

#pragma once 
#include "ItemDefine.h"
#include "AvatarBase.h"
#include <list>
using namespace std;

class CAvatarBase;

class CATWearer
{
public:
	//���캯��
	CATWearer();

	//�͹�����
	~CATWearer();

	//����
	VOID Update(float fTime);

	//�Ƿ�ӵ��ĳavatar
	BOOL HaveAT(DWORD dwID);

	//�����߻��У�����ת��������ʱ�����
	VOID OnAttackByGP(GPINFO& tagGPInfo);

	//���������avatarӵ�ж���
	DWORD AnimTranform(CONST DWORD& dwID);

	//������������
	PHYSICPROPERTY GetPhysicPropt();

	//����avatar
	VOID AddAvatar(CAvatarBase* pAvatar);

	////����avatar
	VOID AddAvatar(const ItemUnit& tagUnit);

	//ɾ��avatar
	VOID RemoveAvatar(DWORD dwID);

protected:
	list<CAvatarBase*> m_listAvatar;
};