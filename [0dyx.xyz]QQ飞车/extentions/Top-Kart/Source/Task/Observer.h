////////////////////////////////////////////////
//     FileName:    Observer.h
//     Author:      rayhuang
//     Date:        2007-5-28
//     Description: �۲��ߺ���ϢԴ�ӿ�
////////////////////////////////////////////////

#pragma once 
#include <list>
using namespace std;

//�۲���
class CObserver
{
public:
	CObserver();
	~CObserver();
	virtual void UpdateMessage() = 0;
};


//��ϢԴ
class CSource
{
public:
	CSource();
	~CSource();
	void NotifyObserver();
	void RegisterObserver(CObserver* pObserver);
	void UnRegObserver(CObserver* pObserver);
private:
	list<CObserver*> m_listObserver;
};
