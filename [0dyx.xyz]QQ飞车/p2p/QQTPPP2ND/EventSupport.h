#ifndef _EVENT_SUPPORT_H_
#define _EVENT_SUPPORT_H_

#ifdef _DEBUG
#ifdef DEBUG_NEW
#define new DEBUG_NEW
#endif
#endif

#ifndef TRACE
inline void TRACE(LPCTSTR, ...){}
#endif //

//C++通用事件支持宏 Copyright (c) Tencent Inc. 2001

//本代码是经过对Visual C++ 7.0新引入的事件机制进行研究和反编译,
//接近乎相同的原理,编写而来.
//优点: 
//	任何C++类都可作为事件源和事件接收者
//	事件接收者的数目不限
//	支持编译时的类型检测
//	接口简单

//Written by sjh (jhshu@tencent.com)
//Version 1.1	(2001-09)

//Version 1.2	(2001-10)
	//取消了DECLARE_EVENT_SUPPORT宏
	//加入DECLARE_EVENT_RECEIVER宏
	//自动判断事件接收对象是否可用,防止接收对象被delete后的访问从突
//Version 1.3  (2002-6-12)
	//加入多线程支持
/*

使用说明:
	EVENT:
				用来在事件源中定义一个事件,事件的参数可自由定义.
				EVENT( 函数名, (参数定义), (相应的参数列表) ) 

	HookEvent:
				为事件源添加一个事件接收者.
				Source.HookEvent(事件名,接收对象指针,接收函数)

	UnhookEvent:
				删除一个事件源的事件接收者.
				Source.UnhookEvent(事件名,接收对象指针,接收函数)

	RaiseEvent:
				触发事件源的事件.
				Source.RaiseEvent(事件名(事件参数))

	DECLARE_EVENT_RECEIVER(classname)
	
代码示例:

//事件源
class CSource 
{
public:
	
	//为CSource添加一个 void OnTimer(void) 的事件
	EVENT(OnTimer,(),());	

	//为CSource添加一个 void OnData(BYTE* pBuf, DWORD nLen) 的事件
	EVENT(OnData,(BYTE* pBuf,DWORD nLen),(pBuf,nLen));

public:
	void FireOnTimer()//内部也可以激发事件
	{
		RaiseEvent( OnTimer() );//使用宏触发OnTimer事件

		OnTimer();//也可直接调用OnTimer
	}
}

//接收者
class CReceiver
{
	DECLARE_EVENT_RECEIVER(CReceiver)
public:
	void OnTimer();
	void OnData(BYTE* pBuf,DWORD nLen);
};

 ...

CSource s;
CReceiver r;

s.HookEvent(OnTimer, &r, &CReceiver::OnTimer);
s.HookEvent(OnData, &r, &CReceiver::OnData);

s.RaiseEvent(OnTimer());	//激发s的OnTimer事件

BYTE buf[128];
s.RaiseEvent(OnData(buf,128)); //激发s的OnData事件

 ...

s.UnhookEvent(OnData,&r,&CReceiver::OnData);
*/

#define EVENT(f,p,v) DECLARE_EVENT(public,f,p,v)

//DECLARE_EVENT定义事件
//access为事件激发函数的访问类型
//如果事件只允许在内部被激发,可以将access设为protected
#define DECLARE_EVENT(access,f,p,v) \
	private:\
	struct _event_node_##f\
	{\
		virtual bool _NodeValid() = 0;\
		virtual void _Invoke p = 0;\
		virtual bool _IsEqual(void* pt,void* pf,bool bD) = 0;\
		bool bDeleted;\
		_event_node_##f* _pNext;\
	};\
	template<class T,class F>\
	struct _handler_##f: public _event_node_##f\
	{\
		T* _pThis;\
		F _pFun;\
		int _nPtrData;\
		bool _NodeValid ()\
		{\
			bool _b = false;\
			try{if(memcmp((BYTE*)_nPtrData,"################",16)==0)_b=true;}catch(...){}\
			return _b;\
		}\
		void _Invoke p\
		{\
			(_pThis->*_pFun)v;\
		}\
		bool _IsEqual(void* pt,void* pf,bool bD)\
		{\
			if( (void*)_pThis==pt && (pf==NULL || *((void**)&_pFun)==pf) && bD == bDeleted)\
				return true;\
			return false;\
		}\
	};\
	struct _nodes_##f\
	{\
		CRITICAL_SECTION m_cs;\
		_nodes_##f(){_pNodes=NULL;InitializeCriticalSection(&m_cs);}\
		~_nodes_##f(){DeleteCriticalSection(&m_cs);_event_node_##f* pNode = _pNodes;_event_node_##f* pTemp;while(pNode){pTemp=pNode->_pNext;delete pNode;pNode=pTemp;}}\
		_event_node_##f* _pNodes;\
		void _Lock(){EnterCriticalSection(&m_cs);}\
		void _Unlock(){LeaveCriticalSection(&m_cs);}\
	}_xNodes_##f;\
	public:\
	template<class T,class F>\
	long HookEvent_##f(T* pThis,F pFun)\
	{\
		if(pThis)\
		{\
			_xNodes_##f._Lock();\
			_handler_##f<T,F>* pNode = new _handler_##f<T,F>;\
			pNode->_pThis = pThis;\
			pNode->_nPtrData = (int)pThis + pThis->_xEventReceiver.nOffset;\
			pNode->_pFun = pFun;\
			pNode->_pNext = _xNodes_##f._pNodes;\
			pNode->bDeleted=false;\
			_xNodes_##f._pNodes = pNode;\
			_xNodes_##f._Unlock();\
			return 1;\
		}\
		return 0;\
	}\
	template<class T,class F>\
	long UnhookEvent_##f(T* pThis,F pFun)\
	{\
		_xNodes_##f._Lock();\
		_event_node_##f* pNode = _xNodes_##f._pNodes;\
		_event_node_##f* pPre = NULL;\
		long nResult = 0;\
		void* pTemp = pFun==NULL ? NULL : *((void**)&pFun);\
		while(pNode)\
		{\
			if(pNode->_IsEqual(pThis,pTemp,pNode->bDeleted))\
			{\
				pNode->bDeleted = true;\
				if(pTemp){nResult=1;break;}\
			}\
			else\
			{\
				pPre = pNode;\
				pNode = pNode->_pNext;\
			}\
		}\
		_xNodes_##f._Unlock();\
		return nResult;\
	}\
	void RemoveAllEvent_##f()\
	{\
		_xNodes_##f._Lock();\
		_event_node_##f* pNode = _xNodes_##f._pNodes;\
		_event_node_##f* pTemp = NULL;\
		while(pNode)\
		{\
			pTemp = pNode->_pNext;\
			delete pNode;\
			pNode = pTemp;\
		}\
		_xNodes_##f._pNodes = NULL;\
		_xNodes_##f._Unlock();\
	}\
	access:\
	void f p\
	{\
		_xNodes_##f._Lock();\
		{\
			_event_node_##f* pNode = _xNodes_##f._pNodes;\
			_event_node_##f* pPre = NULL;\
			while(pNode)\
			{\
				if( !pNode->bDeleted && (pNode->_NodeValid()) )\
				{\
					if (pNode->_pNext)\
					{ (pNode->_Invoke v);pPre = pNode; pNode = pNode->_pNext;}\
					else\
					{\
						_xNodes_##f._Unlock();\
						\
						{\
							(pNode->_Invoke v);\
						}\
						return;\
					}\
				}\
				else\
				{TRACE("Detected died event receiver , removed ok .\n");if(pPre){pPre->_pNext = pNode->_pNext; delete pNode; pNode = pPre->_pNext;}\
				else{_xNodes_##f._pNodes = pNode->_pNext; delete pNode; pNode = _xNodes_##f._pNodes?_xNodes_##f._pNodes:NULL;}\
				}\
			}\
		}\
		_xNodes_##f._Unlock();\
	}\

#define HookEvent(e,t,f) HookEvent_##e(t,f)
#define UnhookEvent(e,t,f) UnhookEvent_##e(t,f)
#define RemoveAllEvent(e) RemoveAllEvent_##e()
#define RaiseEvent(e) e

#define DECLARE_EVENT_RECEIVER(classname) \
	public:\
	struct _event_Receiver\
	{\
		_event_Receiver(){memset(data,'#',16);nOffset=offsetof(classname,_xEventReceiver);}\
		~_event_Receiver(){memset(data,0,16);}\
		BYTE data[16];\
		int nOffset;\
	}_xEventReceiver;\




#endif //_EVENT_SUPPORT_H_