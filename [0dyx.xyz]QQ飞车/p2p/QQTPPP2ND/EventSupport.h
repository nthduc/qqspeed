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

//C++ͨ���¼�֧�ֺ� Copyright (c) Tencent Inc. 2001

//�������Ǿ�����Visual C++ 7.0��������¼����ƽ����о��ͷ�����,
//�ӽ�����ͬ��ԭ��,��д����.
//�ŵ�: 
//	�κ�C++�඼����Ϊ�¼�Դ���¼�������
//	�¼������ߵ���Ŀ����
//	֧�ֱ���ʱ�����ͼ��
//	�ӿڼ�

//Written by sjh (jhshu@tencent.com)
//Version 1.1	(2001-09)

//Version 1.2	(2001-10)
	//ȡ����DECLARE_EVENT_SUPPORT��
	//����DECLARE_EVENT_RECEIVER��
	//�Զ��ж��¼����ն����Ƿ����,��ֹ���ն���delete��ķ��ʴ�ͻ
//Version 1.3  (2002-6-12)
	//������߳�֧��
/*

ʹ��˵��:
	EVENT:
				�������¼�Դ�ж���һ���¼�,�¼��Ĳ��������ɶ���.
				EVENT( ������, (��������), (��Ӧ�Ĳ����б�) ) 

	HookEvent:
				Ϊ�¼�Դ���һ���¼�������.
				Source.HookEvent(�¼���,���ն���ָ��,���պ���)

	UnhookEvent:
				ɾ��һ���¼�Դ���¼�������.
				Source.UnhookEvent(�¼���,���ն���ָ��,���պ���)

	RaiseEvent:
				�����¼�Դ���¼�.
				Source.RaiseEvent(�¼���(�¼�����))

	DECLARE_EVENT_RECEIVER(classname)
	
����ʾ��:

//�¼�Դ
class CSource 
{
public:
	
	//ΪCSource���һ�� void OnTimer(void) ���¼�
	EVENT(OnTimer,(),());	

	//ΪCSource���һ�� void OnData(BYTE* pBuf, DWORD nLen) ���¼�
	EVENT(OnData,(BYTE* pBuf,DWORD nLen),(pBuf,nLen));

public:
	void FireOnTimer()//�ڲ�Ҳ���Լ����¼�
	{
		RaiseEvent( OnTimer() );//ʹ�ú괥��OnTimer�¼�

		OnTimer();//Ҳ��ֱ�ӵ���OnTimer
	}
}

//������
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

s.RaiseEvent(OnTimer());	//����s��OnTimer�¼�

BYTE buf[128];
s.RaiseEvent(OnData(buf,128)); //����s��OnData�¼�

 ...

s.UnhookEvent(OnData,&r,&CReceiver::OnData);
*/

#define EVENT(f,p,v) DECLARE_EVENT(public,f,p,v)

//DECLARE_EVENT�����¼�
//accessΪ�¼����������ķ�������
//����¼�ֻ�������ڲ�������,���Խ�access��Ϊprotected
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