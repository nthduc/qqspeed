#pragma once

namespace FBGUI
{

class CGUIDataType
{
public:
	unsigned int mDataType;
	unsigned int mDataValue;

public:
	CGUIDataType( unsigned int vDataType = GUI_INT, unsigned int vDataValue = 0 ) 
		: mDataType( vDataType ), mDataValue( vDataValue )	{	}
};

class CGUIEvent
{
public:
	typedef void ( CGUIWindow::*MemberFunc )( void );

public:
	int					mEventType;
	MemberFunc			mpEventHandler;
	CGUIWindow*			mpWindow;

public:
	CGUIEvent( int vEventType, CGUIWindow* pWindow, MemberFunc pEventHandler )
		: mEventType( vEventType ), mpWindow( pWindow ), mpEventHandler( pEventHandler ) {	}
};

class CGUIScriptEvent
{
public:
	int				mEventType;
	std::string		mFunc;

public:
	CGUIScriptEvent( int vEventType, const char* pFunc ) : mEventType( vEventType ), mFunc( pFunc )	{	}
};

class CGUISink
{
	friend class CGUIEventSet;
	friend class CGUISinkManager;

public:
	CGUIDataType	mData[ 8 ];
	int				mCount;

public:
	CGUISink( ) : mCount( 0 )	{	}

public:
	void AddArg( const CGUIDataType& rData )
		{ mData[ mCount ++ ] = rData; }

	virtual void Fire( CGUIEvent* pEvent ) = 0;
	virtual void Fire( CGUIScriptEvent* pEvent );

	virtual int GetType( ) = 0;
};

// �¼������������е��¼��������ɳ�Ա��������ģ�Ҳ�����ɽű����������
// CGUISink����Ϊ����������ṩ�¼���ȼ����������ýű����ǳ�Ա����
class CGUIEventSet
{
public:
	std::vector< CGUIEvent* >		mEvents;
	std::vector< CGUIScriptEvent* > mScriptEvents;

public:
	CGUIEventSet( )		{	}
public:
	~CGUIEventSet( );

public:
	// ����¼�
	void AddEvent( CGUIEvent* pEvent );

	// ��ӽű��¼�
	void AddScriptEvent( CGUIScriptEvent* pEvent );
	int FireEvent( CGUISink* pSink );
};

};