#pragma once
#include "BaseInterface.h"
#include "vector"


template< typename T >
class IBaseEventListenerContainer
	: public IInterface
{
public:
	struct LISTENER
	{
		LISTENER(T* Listener, int Cookie) : nCookie(Cookie), pListener(Listener){}
		UINT	nCookie;
		T*		pListener;
	};
	typedef std::vector<LISTENER>			ListenerContainer;
	typedef std::vector<int>				CookieContainer;

public:
	IBaseEventListenerContainer():m_nMaxCookie(0){ m_ListenerContainer.reserve(100); }
	virtual ~IBaseEventListenerContainer();

	HRESULT RegisterListener(T* Listener, UINT* Cookie);
	HRESULT UnRegisterListener(UINT nCookie);

protected:
	ListenerContainer	m_ListenerContainer;
	CookieContainer		m_CookieContainer;
	UINT				m_nMaxCookie;
};

template<class T>
IBaseEventListenerContainer<T>::~IBaseEventListenerContainer()
{
	for ( ListenerContainer::iterator it = m_ListenerContainer.begin(); it != m_ListenerContainer.end(); it++ )
	{
		SafeRelease(it->pListener);
	}
}

template<class T>
HRESULT IBaseEventListenerContainer<T>::RegisterListener(T* Listener, UINT* Cookie)
{
	assert(Listener && Cookie);

	UINT nCookie = 0;
	if ( !m_CookieContainer.empty() ) 
	{
		nCookie = m_CookieContainer.back();
		m_CookieContainer.pop_back();
	}
	else
	{
		nCookie = ++m_nMaxCookie;
	}
	*Cookie = nCookie;
	Listener->AddRef();
	m_ListenerContainer.push_back(LISTENER(Listener, nCookie));
	return S_OK;
}

template<class T>
HRESULT IBaseEventListenerContainer<T>::UnRegisterListener(UINT Cookie)
{
	assert(Cookie);
	for ( ListenerContainer::iterator it = m_ListenerContainer.begin(); it != m_ListenerContainer.end(); it++ )
	{
		if ( it->nCookie == Cookie) 
		{
			m_CookieContainer.push_back(Cookie);
			SafeRelease(it->pListener);
			m_ListenerContainer.erase(it);
			return S_OK;
		}
	}
	return S_FALSE;
}

// warning by wincoqin:
// To use following macros, u should know this:
// InterceptCall means if this message has been disposed,
// it will not be passed to the rest listeners; While common 
// call will continue pass the message however it was disposed
// by the pre-listener.
// If a listener returns S_OK, it stands that this message has
// been disposed. Other wise it has not been disposed.
// warning end;

#define Call_Listener1(func, param1) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening()) \
		{\
			iter->pListener->func(param1);\
		}\
	}

#define InterceptCall_Listener1(func, param1) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening()) \
		{\
			if (S_OK == iter->pListener->func(param1))\
				break;\
		}\
	}

#define Call_Listener2(func, param1, param2) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening()) \
		{\
			iter->pListener->func(param1, param2);\
		}\
	}

#define InterceptCall_Listener2(func, param1, param2) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening()) \
		{\
			if (S_OK == iter->pListener->func(param1, param2))\
				break;\
		}\
	}

#define Call_Listener3(func, param1, param2, param3) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening()) \
		{\
			iter->pListener->func(param1, param2, param3);\
		}\
	}	

#define InterceptCall_Listener3(func, param1, param2, param3) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening()) \
		{\
			if (S_OK == iter->pListener->func(param1, param2, param3))\
				break;\
		}\
	}	

#define Call_Listener4(func, param1, param2, param3, param4) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening())\
		{\
			iter->pListener->func(param1, param2, param3, param4);\
		}\
	}

#define InterceptCall_Listener4(func, param1, param2, param3, param4) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening())\
		{\
			if ( S_OK == iter->pListener->func(param1, param2, param3, param4))\
				break;\
		}\
	}

#define Call_Listener5(func, param1, param2, param3, param4, param5) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening())\
		{\
			iter->pListener->func(param1, param2, param3, param4, param5);\
		}\
	}

#define InterceptCall_Listener5(func, param1, param2, param3, param4, param5) \
	for (ListenerContainer::iterator iter = m_ListenerContainer.begin(); iter != m_ListenerContainer.end(); ++iter) \
	{\
		if (iter->pListener->IsListening())\
		{\
			if (S_OK == iter->pListener->func(param1, param2, param3, param4, param5))\
				break;\
		}\
	}