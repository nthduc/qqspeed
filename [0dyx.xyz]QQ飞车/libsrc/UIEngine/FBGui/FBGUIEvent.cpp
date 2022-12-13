#include "FBGUIBase.h"
#include "FBGUILua.h"
#include "FBGUIEvent.h"

// ************************************************************ //
// CGUISink
// ************************************************************ //

void CGUISink::Fire( CGUIScriptEvent* pEvent )
{
	CGUILuaModule::GetSingleton( ).FireEvent( pEvent->mFunc.c_str( ), this ); 
}

// ************************************************************ //
// CGUIEventSet
// ************************************************************ //
template<> CGUISinkManager* CSingleton< CGUISinkManager >::spSingleton = NULL;

CGUIEventSet::~CGUIEventSet( )
{
	for ( int i = 0; i < int( mEvents.size( ) ); i ++ )
		delete mEvents[ i ];

	for ( int i = 0; i < int( mScriptEvents.size( ) ); i ++ )
		delete mScriptEvents[ i ];
}

void CGUIEventSet::AddEvent( CGUIEvent* pEvent )
{
	for ( int i = 0; i < int( mEvents.size( ) ); i ++ )
	{
		if ( mEvents[ i ]->mEventType == pEvent->mEventType )
		{
			delete mEvents[ i ];
			mEvents.erase( mEvents.begin( ) + i );
			break;
		}
	}

	mEvents.push_back( pEvent ); 
}

void CGUIEventSet::AddScriptEvent( CGUIScriptEvent* pEvent )
{
	for ( int i = 0; i < int( mScriptEvents.size( ) ); i ++ )
	{
		if ( mScriptEvents[ i ]->mEventType == pEvent->mEventType )
		{
			delete mScriptEvents[ i ];
			mScriptEvents.erase( mScriptEvents.begin( ) + i );
			break;
		}
	}

	mScriptEvents.push_back( pEvent ); 
}

int CGUIEventSet::FireEvent( CGUISink* pSink )
{
	int tRtn = GUI_ERR_EVENTNOTFOUND;
	for ( int i = 0; i < int( mEvents.size( ) ); i ++ )
	{
		if ( pSink->GetType( ) == mEvents[ i ]->mEventType )
		{
			pSink->Fire( mEvents[ i ] );
			tRtn = GUI_ERR_SUCCESS;
		}
	}

	for ( int i = 0; i < int( mScriptEvents.size( ) ); i ++ )
	{
		if ( pSink->GetType( ) == mScriptEvents[ i ]->mEventType )
		{
			pSink->Fire( mScriptEvents[ i ] );
			tRtn = GUI_ERR_SUCCESS;
		}
	}

	return tRtn;
}