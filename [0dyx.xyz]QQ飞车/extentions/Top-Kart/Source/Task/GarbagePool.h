////////////////////////////////////////////////
//     FileName:    GarbagePool.h
//     Author:      rayhuang
//     Date:        2007-6-9
//     Description: 垃圾回收池
////////////////////////////////////////////////

#pragma once 
#include <list>
using namespace std;

template<class T> class CGarbagePool
{
public:
	CGarbagePool();
	~CGarbagePool();
	void ClearAllGarbage();			//清除所有的垃圾
	void AddGarbage(T* pData);		//增加垃圾
	void ReuseAllGarbage(list<T*>& listData);	//重用所有垃圾
private:
	list<T*> m_listGarbagePool;		//垃圾池
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
