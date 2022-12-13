////////////////////////////////////////////////
//     FileName:    GarbagePool.h
//     Author:      rayhuang
//     Date:        2007-6-9
//     Description: �������ճ�
////////////////////////////////////////////////

#pragma once 
#include <list>
using namespace std;

template<class T> class CGarbagePool
{
public:
	CGarbagePool();
	~CGarbagePool();
	void ClearAllGarbage();			//������е�����
	void AddGarbage(T* pData);		//��������
	void ReuseAllGarbage(list<T*>& listData);	//������������
private:
	list<T*> m_listGarbagePool;		//������
};

template<class T> CGarbagePool<T>::CGarbagePool()
{

}

template<class T> CGarbagePool<T>::~CGarbagePool()
{
	ClearAllGarbage();
}

template<class T> void CGarbagePool<T>::AddGarbage(T* pData)
{
	m_listGarbagePool.push_back(pData);
}

template<class T> void CGarbagePool<T>::ClearAllGarbage()
{
	list<T*>::iterator it = m_listGarbagePool.begin();
	for(; it != m_listGarbagePool.end(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_listGarbagePool.clear();
}

template<class T> void CGarbagePool<T>::ReuseAllGarbage(list<T*>& listData)
{
	listData.clear();
	list<T*>::iterator it = m_listGarbagePool.begin();
	for(; it != m_listGarbagePool.end(); ++it)
	{
		listData.push_back(*it);
	}
	m_listGarbagePool.clear();
}
