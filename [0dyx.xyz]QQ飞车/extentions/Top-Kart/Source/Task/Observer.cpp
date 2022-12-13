#include "Top-KartPCH.h"
#include "Observer.h"
CObserver::CObserver()
{

}

CObserver::~CObserver()
{
	
}


CSource::CSource()
{
	m_listObserver.clear();
}

CSource::~CSource()
{
	m_listObserver.clear();
}

void CSource::RegisterObserver(CObserver* pObserver)
{
	list<CObserver*>::const_iterator it = m_listObserver.begin();
	while(it != m_listObserver.end())//查找时候已有改观察者
	{
		if(*it == pObserver)
		{
			return;
		}
		++it;
	}
	m_listObserver.push_back(pObserver);	
}

void CSource::UnRegObserver(CObserver* pObserver)
{
	list<CObserver*>::iterator it = m_listObserver.begin();
	while(it != m_listObserver.end())
	{
		if(*it == pObserver)		//查找该观察者
		{
			m_listObserver.erase(it);
			return;
		}
		++it;
	}
}

void CSource::NotifyObserver()
{
	list<CObserver*>::const_iterator it = m_listObserver.begin();
	for(; it != m_listObserver.end(); ++it)
	{
		(*it)->UpdateMessage();
	}
}