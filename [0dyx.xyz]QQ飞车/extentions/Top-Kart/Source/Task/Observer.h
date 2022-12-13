////////////////////////////////////////////////
//     FileName:    Observer.h
//     Author:      rayhuang
//     Date:        2007-5-28
//     Description: 观察者和消息源接口
////////////////////////////////////////////////

#pragma once 
#include <list>
using namespace std;

//观察者
class CObserver
{
public:
	CObserver();
	~CObserver();
	virtual void UpdateMessage() = 0;
};


//消息源
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
