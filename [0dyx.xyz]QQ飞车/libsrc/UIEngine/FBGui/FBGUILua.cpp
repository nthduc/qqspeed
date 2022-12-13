#include "FBGUIBase.h"
#include "FBGUICore.h"
#include "FBGUIEvent.h"
#include "FBGUIWindow.h"
#include "FBGUIElement.h"
#include "FBGUISystem.h"
#include "FBGUILua.h"
#include "FBGUIMemory.h"

// ************************************************************ //
// CLuaModule
// ************************************************************ //
template<> CGUILuaModule* CSingleton< CGUILuaModule >::spSingleton = NULL;

CGUILuaModule::CGUILuaModule( const char* pFileName ) : mFileName( pFileName ), mpLuaModule( NULL )
{
	if ( mFileName.empty( ) == true )
		return;

	mpLuaModule = lua_open( );
	luaL_dofile( mpLuaModule, mFileName.c_str( ) );
	lua_register( mpLuaModule, "CreateFont", CGUILuaInterface::CreateGUIFont );
	lua_register( mpLuaModule, "DestroyFont", CGUILuaInterface::DestroyGUIFont );
	lua_register( mpLuaModule, "GetFont", CGUILuaInterface::GetGUIFont );
	lua_register( mpLuaModule, "CreateWindow", CGUILuaInterface::CreateGUIWindow );
	lua_register( mpLuaModule, "DestroyWindow", CGUILuaInterface::DestroyGUIWindow );
	lua_register( mpLuaModule, "GetWindow", CGUILuaInterface::GetGUIWindow );
	lua_register( mpLuaModule, "GetSheet", CGUILuaInterface::GetGUISheet );
	lua_register( mpLuaModule, "SetEvent", CGUILuaInterface::SetGUIEvent );
	lua_register( mpLuaModule, "SetWindowText", CGUILuaInterface::SetGUIWindowText );
	lua_register( mpLuaModule, "GetWindowText", CGUILuaInterface::GetGUIWindowText );
	lua_register( mpLuaModule, "GetWindowRect", CGUILuaInterface::GetGUIWindowRect );
	lua_register( mpLuaModule, "GetWindowSize", CGUILuaInterface::GetGUIWindowSize );
	lua_register( mpLuaModule, "GetClientRect", CGUILuaInterface::GetGUIClientRect );
	lua_register( mpLuaModule, "GetClientSize", CGUILuaInterface::GetGUIClientSize );
	lua_register( mpLuaModule, "ScreenToWindow", CGUILuaInterface::ScreenToWindow );
	lua_register( mpLuaModule, "ScreenToClient", CGUILuaInterface::ScreenToClient );
	lua_register( mpLuaModule, "MessageBox", CGUILuaInterface::GUIMessageBox );
	lua_register( mpLuaModule, "GetEditText", CGUILuaInterface::GetGUIEditText );
	lua_register( mpLuaModule, "SetEditText", CGUILuaInterface::SetGUIEditText );
	lua_register( mpLuaModule, "DoModal", CGUILuaInterface::GUIDoModal );
	lua_register( mpLuaModule, "SetWindowArchor", CGUILuaInterface::SetGUIWindowArchor );
	lua_register( mpLuaModule, "CreateCanvas", CGUILuaInterface::CreateCanvas );
	lua_register( mpLuaModule, "DestroyCanvas", CGUILuaInterface::DestroyCanvas );
	lua_register( mpLuaModule, "DrawLine", CGUILuaInterface::DrawLine );
	lua_register( mpLuaModule, "DrawRect", CGUILuaInterface::DrawRect );
	lua_register( mpLuaModule, "LoadTexture", CGUILuaInterface::LoadTexture );
	lua_register( mpLuaModule, "CreateTexture", CGUILuaInterface::CreateTexture );
	lua_register( mpLuaModule, "DumpText", CGUILuaInterface::DumpText );
}

CGUILuaModule::~CGUILuaModule( )
{
	if ( mpLuaModule == NULL )
		return;

	lua_close( mpLuaModule );
}

int CGUILuaModule::FireEvent( const char* pFun, CGUISink* pSink )
{
	if ( mpLuaModule == NULL )
		return -1;

	lua_getglobal( mpLuaModule, pFun );
	for ( int i = 0; i < pSink->mCount; i ++ )
	{
		switch( pSink->mData[ i ].mDataType )
		{
			case GUI_INT:
			case GUI_UINT:
			case GUI_FLOAT:
			case GUI_CHAR:
				lua_pushnumber( mpLuaModule, pSink->mData[ i ].mDataValue );
				break;
			case GUI_BOOL:
				lua_pushboolean( mpLuaModule, pSink->mData[ i ].mDataValue );
				break;
			case GUI_PCHAR:
				lua_pushstring( mpLuaModule, (char*) pSink->mData[ i ].mDataValue );
				break;
			case GUI_PPOINT:
			{
				CGUIPoint* tpPoint = (CGUIPoint*) pSink->mData[ i ].mDataValue;
				lua_pushnumber( mpLuaModule, tpPoint->mX );
				lua_pushnumber( mpLuaModule, tpPoint->mY );
				break;
			}
			case GUI_PSIZE:
			{
				CGUISize* tpSize = (CGUISize*) pSink->mData[ i ].mDataValue;
				lua_pushnumber( mpLuaModule, tpSize->mCX );
				lua_pushnumber( mpLuaModule, tpSize->mCY );
				break;
			}
			case GUI_PRECT:
			{
				CGUIRect* tpRect = (CGUIRect*) pSink->mData[ i ].mDataValue;
				lua_pushnumber( mpLuaModule, tpRect->mTopLeft.mX );
				lua_pushnumber( mpLuaModule, tpRect->mTopLeft.mY );
				lua_pushnumber( mpLuaModule, tpRect->Width( ) );
				lua_pushnumber( mpLuaModule, tpRect->Height( ) );
				break;
			}
			case GUI_PVOID:
			case GUI_PCANVAS:
			case GUI_PWINDOW:
			case GUI_PFONT:
				lua_pushlightuserdata( mpLuaModule, (void*) pSink->mData[ i ].mDataValue );
				break;
		}
	}

	if ( lua_pcall( mpLuaModule, pSink->mCount, 0, 0 ) != 0 )
	{
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>�ű�����</C>)", (char*) lua_tostring( mpLuaModule, 0 ) );
		return -1;
	}

	return 0;
}

int CGUILuaModule::OnSystemStartup( const char* pFun )
{
	if ( mpLuaModule == NULL )
		return -1;

	lua_getglobal( mpLuaModule, pFun );
	if ( lua_pcall( mpLuaModule, 0, 0, 0 ) != 0 )
	{
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>�ű�����</C>)", (char*) lua_tostring( mpLuaModule, 0 ) );
		return -1;
	}

	return 0;
}

int CGUILuaModule::OnInitInstance( const char* pFun )
{
	if ( mpLuaModule == NULL )
		return -1;

	lua_getglobal( mpLuaModule, pFun );
	if ( lua_pcall( mpLuaModule, 0, 0, 0 ) != 0 )
	{
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>�ű�����</C>)", (char*) lua_tostring( mpLuaModule, 0 ) );
		return -1;
	}

	return 0;
}

// ************************************************************ //
// CGUILuaInterface
// ************************************************************ //

// ����:		��������ı�
// ����ԭ��:	void DumpText( string )
int CGUILuaInterface::DumpText( lua_State* pLua )
{
	const char* pDebugText = lua_tostring( pLua, 1 );
	CGUISystem::GetSingleton( ).DumpText( pDebugText );
	return 1;
}

// ����:		װ������
// ����ԭ��:	CGUITexture* LoadTexture( string )
int CGUILuaInterface::LoadTexture( lua_State* pLua )
{
	const char* pTexName	= lua_tostring( pLua, 1 );
	CGUITexture* tpTexture	= NULL;
	GUI_LOADTEXTURE( tpTexture, pTexName );
	if ( tpTexture != NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������, (<C255,0,255,0>�ű�����</C>)", pTexName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpTexture );
	return 1;
}

// ����:		��������
// ����ԭ��:	CGUITexture* CreateTexture( string, int, int )
int CGUILuaInterface::CreateTexture( lua_State* pLua )
{
	const char* pTexName	= lua_tostring( pLua, 1 );
	int tTexWidth			= lua_tonumber( pLua, 2 );
	int tTexHeight			= lua_tonumber( pLua, 3 );
	CGUITexture* tpTexture	= CGUISystem::GetSingleton( ).GetRenderDriver( )->CreateTexture( pTexName, tTexWidth, tTexHeight );
	if ( tpTexture != NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]����ʧ��, (<C255,0,255,0>�ű�����</C>)", pTexName );
		return 0;
	}
	
	lua_pushlightuserdata( pLua, tpTexture );
	return 1;
}

// ����:		��������
// ����ԭ��:	CGUICanvas* CreateCanvas( string, int )
int CGUILuaInterface::CreateCanvas( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// �õ���������
	int tType = lua_tonumber( pLua, 2 );				// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	CGUICanvas* tpCanvas = NULL;
	switch( tType )
	{
	case CANVAS_WINDOW:
		tpCanvas = new CGUIWindowCanvas( tpWindow );
		break;
	case CANVAS_CLIENT:
		tpCanvas = new CGUIClientCanvas( tpWindow );
		break;
	}

	lua_pushlightuserdata( pLua, tpCanvas );
	return 1;
}

// ����:		���ٻ���
// ����ԭ��:	void DestroyCanvas( int )
int CGUILuaInterface::DestroyCanvas( lua_State* pLua )
{
	CGUICanvas* tpCanvas = (CGUICanvas*) lua_touserdata( pLua, 1 );
	GUI_DELETEPTR( tpCanvas );
	return 0;
}

// ����:		���ƾ���
// ����ԭ��:	void DrawRect( int, int, int, int, int, int, int )
int CGUILuaInterface::DrawRect( lua_State* pLua )
{
	CGUICanvas* tpCanvas = (CGUICanvas*) lua_touserdata( pLua, 1 );
	int tX1 = lua_tonumber( pLua, 2 );
	int tY1 = lua_tonumber( pLua, 3 );
	int tX2 = lua_tonumber( pLua, 4 );
	int tY2 = lua_tonumber( pLua, 5 );
	int tAlpha1 = lua_tonumber( pLua, 6 );
	int tRed1	= lua_tonumber( pLua, 7 );
	int tGreen1	= lua_tonumber( pLua, 8 );
	int tBlue1	= lua_tonumber( pLua, 9 );
	int tAlpha2 = lua_tonumber( pLua, 10 );
	int tRed2	= lua_tonumber( pLua, 11 );
	int tGreen2	= lua_tonumber( pLua, 12 );
	int tBlue2	= lua_tonumber( pLua, 13 );

	if ( tpCanvas == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>�ű�����</C>)", "��Ч����" );
		return 0;
	}

	tpCanvas->DrawRect( CGUIRect( tX1, tY1, tX2, tY2 ), GUI_MAKECOLOUR( tAlpha1, tRed1, tGreen1, tBlue1 ),
		GUI_MAKECOLOUR( tAlpha2, tRed2, tGreen2, tBlue2 ) );
	return 0;
}

// ����:		����ֱ��
// ����ԭ��:	void DrawLine( int, int, int, int, int, int )
int CGUILuaInterface::DrawLine( lua_State* pLua )
{
	CGUICanvas* tpCanvas = (CGUICanvas*) lua_touserdata( pLua, 1 );
	int tX1 = lua_tonumber( pLua, 2 );
	int tY1 = lua_tonumber( pLua, 3 );
	int tX2 = lua_tonumber( pLua, 4 );
	int tY2 = lua_tonumber( pLua, 5 );
	int tAlpha1 = lua_tonumber( pLua, 6 );
	int tRed1	= lua_tonumber( pLua, 7 );
	int tGreen1	= lua_tonumber( pLua, 8 );
	int tBlue1	= lua_tonumber( pLua, 9 );

	if ( tpCanvas == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>�ű�����</C>)", "��Ч����" );
		return 0;
	}

	tpCanvas->DrawLine( CGUIPoint( tX1, tY1 ), CGUIPoint( tX2, tY2 ), GUI_MAKECOLOUR( tAlpha1, tRed1, tGreen1, tBlue1 ) );
	return 0;
}

// ����:		�����Ի���
// ����ԭ��:	CGUIWindow* GUIDoModal( string, string, int, int, int, int, int, int )
int CGUILuaInterface::GUIDoModal( lua_State* pLua )
{
	const char* pType = lua_tostring( pLua, 1 );	// �õ���������
	const char* pName = lua_tostring( pLua, 2 );	// �õ���������
	int tStyle	= lua_tonumber( pLua, 3 );			// �õ����ڷ��
	int tLeft	= lua_tonumber( pLua, 4 );			// �õ����ں�����
	int tTop	= lua_tonumber( pLua, 5 );			// �õ�����������
	int tCX		= lua_tonumber( pLua, 6 );			// �õ����ڿ��
	int tCY		= lua_tonumber( pLua, 7 );			// �õ����ڸ߶�
	void* tpUserData	 = (void*)  lua_touserdata( pLua, 9 );		// �õ���������
	
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).DoModal( pType, pName, tStyle, CGUIRect( CGUIPoint( tLeft, tTop ), CGUISize( tCX, tCY ) ), tpUserData );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// ����:		������Ϣ��
// ����ԭ��:	CGUIWindow* MessageBox( string, string, int )
int CGUILuaInterface::GUIMessageBox( lua_State* pLua )
{
	const char* pMessage = lua_tostring( pLua, 1 );		// �õ���Ϣ����
	const char* pCaption = lua_tostring( pLua, 2 );		// �õ�����
	int tStyle = lua_tonumber( pLua, 3 );				// �õ����
	CGUIWindow* tpWindow = CGUISystem::GetSingleton( ).Message( pMessage, pCaption, tStyle );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "��Ϣ���ڽ���ʧ��(<C255,0,255,0>�ű�����</C>)" );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// ����:		�õ�������ı�
// ����ԭ��:	string GetGUIEditText( string )
int CGUILuaInterface::GetGUIEditText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// �õ���������
	CGUIWindow* tpEdit = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpEdit == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	if ( ::strcmp( tpEdit->GetType( ).c_str( ), "GUIEdit" ) != 0 )
	{
		CGUISystem::GetSingleton( ).DumpText( "ָ���������Ͳ���Edit(<C255,0,255,0>�ű�����</C>)" );
		return 0;
	}

	std::string tText = ( (CGUIEdit*) tpEdit )->GetEditText( );
	lua_pushstring( pLua, ( (CGUIEdit*) tpEdit )->GetEditText( ).c_str( ) );
	return 1;
}

// ����:		����������ı�
// ����ԭ��:	void SetGUIEditText( string )
int CGUILuaInterface::SetGUIEditText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// �õ���������
	const char* pText = lua_tostring( pLua, 2 );
	CGUIWindow* tpEdit = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpEdit == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

 	if ( ::strcmp( tpEdit->GetType( ).c_str( ), "GUIEdit" ) == 0 )
	{
		CGUISystem::GetSingleton( ).DumpText( "ָ���������Ͳ���Edit(<C255,0,255,0>�ű�����</C>)" );
		return 0;
	}

	return 0;	
}

// ����:		���ô���ê
// ����ԭ��:	void SetGUIWindowArchor( string, int )
int CGUILuaInterface::SetGUIWindowArchor( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	int tArchor = lua_tonumber( pLua, 2 );			// �õ�����ê
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	tpWindow->SetArchor( tArchor );
	return 0;
}

// ����:		�õ���������
// ����ԭ��:	int GetGUIWindowRect( string )
int CGUILuaInterface::GetGUIWindowRect( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	CGUIRect tRect = tpWindow->GetWindowRect( );
	lua_pushnumber( pLua, tRect.mTopLeft.mX );
	lua_pushnumber( pLua, tRect.mTopLeft.mY );
	lua_pushnumber( pLua, tRect.mBottomRight.mX );
	lua_pushnumber( pLua, tRect.mBottomRight.mY );
	return 4;
}

// ����:		�õ����ڴ�С
// ����ԭ��:	int GetGUIWindowSize( string )
int CGUILuaInterface::GetGUIWindowSize( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	CGUISize tSize = tpWindow->GetWindowSize( );
	lua_pushnumber( pLua, tSize.mCX );
	lua_pushnumber( pLua, tSize.mCY );
	return 2;
}

// ����:		�õ��ͻ�������
// ����ԭ��:	int GetGUIClientRect( string )
int CGUILuaInterface::GetGUIClientRect( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	CGUIRect tRect = tpWindow->GetClientRect( );
	lua_pushnumber( pLua, tRect.mTopLeft.mX );
	lua_pushnumber( pLua, tRect.mTopLeft.mY );
	lua_pushnumber( pLua, tRect.mBottomRight.mX );
	lua_pushnumber( pLua, tRect.mBottomRight.mY );
	return 4;
}

// ����:		�õ��ͻ�����С
// ����ԭ��:	int GetGUIClientSize( string )
int CGUILuaInterface::GetGUIClientSize( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	CGUISize tSize = tpWindow->GetClientSize( );
	lua_pushnumber( pLua, tSize.mCX );
	lua_pushnumber( pLua, tSize.mCY );
	return 2;
}

// ����:		��Ļ����ת����������
// ����ԭ��:	int ScreenToWindow( string )
int CGUILuaInterface::ScreenToWindow( lua_State* pLua )
{
	int tArgNum = lua_gettop( pLua );
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	if ( tArgNum == 2 )
	{
		int tX = lua_tonumber( pLua, 2 );
		int tY = lua_tonumber( pLua, 3 );
		CGUIPoint tPoint = tpWindow->ScreenToWindow( CGUIPoint( tX, tY ) );
		lua_pushnumber( pLua, tPoint.mX );
		lua_pushnumber( pLua, tPoint.mY );
		return 2;
	}
	else if ( tArgNum == 4 )
	{
		int tX1 = lua_tonumber( pLua, 2 );
		int tY1 = lua_tonumber( pLua, 3 );
		int tX2 = lua_tonumber( pLua, 4 );
		int tY2 = lua_tonumber( pLua, 5 );
		CGUIRect tRect = tpWindow->ScreenToWindow( CGUIRect( tX1, tY1, tX2, tY2 ) );
		lua_pushnumber( pLua, tRect.mTopLeft.mX );
		lua_pushnumber( pLua, tRect.mTopLeft.mY );
		lua_pushnumber( pLua, tRect.mBottomRight.mX );
		lua_pushnumber( pLua, tRect.mBottomRight.mY );
		return 4;
	}
	else
	{
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>�ű�����</C>)", "δ֪�ĺ�������" );
		return 0;
	}
}

// ����:		��Ļ����ת���ͻ�������
// ����ԭ��:	int ScreenToClient( string )
int CGUILuaInterface::ScreenToClient( lua_State* pLua )
{
	int tArgNum = lua_gettop( pLua );
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	if ( tArgNum == 2 )
	{
		int tX = lua_tonumber( pLua, 2 );
		int tY = lua_tonumber( pLua, 3 );
		CGUIPoint tPoint = tpWindow->ScreenToClient( CGUIPoint( tX, tY ) );
		lua_pushnumber( pLua, tPoint.mX );
		lua_pushnumber( pLua, tPoint.mY );
		return 2;
	}
	else if ( tArgNum == 4 )
	{
		int tX1 = lua_tonumber( pLua, 2 );
		int tY1 = lua_tonumber( pLua, 3 );
		int tX2 = lua_tonumber( pLua, 4 );
		int tY2 = lua_tonumber( pLua, 5 );
		CGUIRect tRect = tpWindow->ScreenToClient( CGUIRect( tX1, tY1, tX2, tY2 ) );
		lua_pushnumber( pLua, tRect.mTopLeft.mX );
		lua_pushnumber( pLua, tRect.mTopLeft.mY );
		lua_pushnumber( pLua, tRect.mBottomRight.mX );
		lua_pushnumber( pLua, tRect.mBottomRight.mY );
		return 4;
	}
	else
	{
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>�ű�����</C>)", "δ֪�ĺ�������" );
		return 0;
	}
}

// ����:		���ô����ı�
// ����ԭ��:	void SetGUIWindowText( string )
int CGUILuaInterface::SetGUIWindowText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// �õ���������
	const char* pText = lua_tostring( pLua, 2 );
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	tpWindow->SetText( pText );
	return 0;
}

// ����:		�õ������ı�
// ����ԭ��:	string GetGUIWindowText( string )
int CGUILuaInterface::GetGUIWindowText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	const char* pText = tpWindow->GetText( ).c_str( );
	lua_pushstring( pLua, pText );
	return 1;
}

// ����:		�������� 
// ����ԭ��:	CGUIFont* CreateFont( string, string, int, int, int, int, bool, int )
int CGUILuaInterface::CreateGUIFont( lua_State* pLua )
{
	const char* pFontName	= lua_tostring( pLua, 1 );	// �õ���������
	const char* pFaceName	= lua_tostring( pLua, 2 );	// �õ������������
	int tFontHeight			= lua_tonumber( pLua, 3 );	// �õ�����߶�
	int tEdge		= lua_tonumber( pLua, 4 );			// �õ�����߿�
	int tHOffset	= lua_tonumber( pLua, 5 );			// �õ�����ߺ���ƫ��
	int tVOffset	= lua_tonumber( pLua, 6 );			// �õ������ݺ���ƫ��
	bool tAnsi		= lua_toboolean( pLua, 7 );			// �õ������Ƿ񿹾��
	int tCodePage	= lua_tonumber( pLua, 8 );			// �õ��������ҳ
	if ( tCodePage == 0 )
	{
		CGUIFont* tpFont = CGUIFontManager::GetSingleton( ).Create( pFontName, pFaceName, tFontHeight, tEdge, tHOffset, tVOffset, tAnsi, NULL );
		lua_pushlightuserdata( pLua, tpFont );
	}
	else
		lua_pushlightuserdata( pLua, NULL );

	return 1;
}

// ����:		�������� 
// ����ԭ��:	void DestroyFont( string )
int CGUILuaInterface::DestroyGUIFont( lua_State* pLua )
{
	const char* pFontName = lua_tostring( pLua, 1 );		// �õ���������
	CGUIFontManager::GetSingleton( ).Destroy( pFontName );
	return 0;
}

// ����:		�õ�����
// ����ԭ��:	CGUIFont* GetFont( string )
int CGUILuaInterface::GetGUIFont( lua_State* pLua )
{
	const char* pFontName = lua_tostring( pLua, 1 );		// �õ���������
	CGUIFont* tpFont = CGUIFontManager::GetSingleton( ).GetFont( pFontName );
	if ( tpFont == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����%s������(<C255,0,255,0>�ű�����</C>)", pFontName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpFont );
	return 1;
}

// ����:		�������� 
// ����ԭ��:	CGUIWindow* CreateWindow( string, string, int, int, int, int, int, int, int )
int CGUILuaInterface::CreateGUIWindow( lua_State* pLua )
{
	const char* pType = lua_tostring( pLua, 1 );	// �õ���������
	const char* pName = lua_tostring( pLua, 2 );	// �õ���������
	int tStyle	= lua_tonumber( pLua, 3 );			// �õ����ڷ��
	int tLeft	= lua_tonumber( pLua, 4 );			// �õ����ں�����
	int tTop	= lua_tonumber( pLua, 5 );			// �õ�����������
	int tCX		= lua_tonumber( pLua, 6 );			// �õ����ڿ��
	int tCY		= lua_tonumber( pLua, 7 );			// �õ����ڸ߶�
	CGUIWindow* tpParent = (CGUIWindow*) lua_touserdata( pLua, 8 );	// �õ����ڸ�����
	void* tpUserData	 = (void*)  lua_touserdata( pLua, 9 );		// �õ���������
	
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).Create( pType, pName, tStyle, CGUIRect( CGUIPoint( tLeft, tTop ), CGUISize( tCX, tCY ) ),	tpParent, tpUserData );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// ����:		���ٴ��� 
// ����ԭ��:	void DestroyWindow( string )
int CGUILuaInterface::DestroyGUIWindow( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// �õ���������
	CGUIWindowManager::GetSingleton( ).Destroy( pName );
	return 0;
}

int CGUILuaInterface::GetGUIWindow( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// �õ���������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// ����:		�õ�����Sheet
// ����ԭ��:	CGUIWindow* GetSheet( )
int CGUILuaInterface::GetGUISheet( lua_State* pLua )
{
	CGUIWindow* tpSheet = CGUISystem::GetSingleton( ).GetSheet( );
 	if ( tpSheet == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "ϵͳSheet������(<C255,0,255,0>�ű�����</C>)" );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpSheet );
	return 1;
}

// ����:		���ô����¼�
// ����ԭ��:	void SetEvent( string, int, string )
int CGUILuaInterface::SetGUIEvent( lua_State* pLua )
{
	const char* pName	= lua_tostring( pLua, 1 );	// �õ���������
	int tEventType		= lua_tonumber( pLua, 2 );	// �õ��¼�����
	const char* pFunc	= lua_tostring( pLua, 3 );	// �õ�������
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "����[%s]������(<C255,0,255,0>�ű�����</C>)", pName );
		return 0;
	}

	tpWindow->AddScriptEvent( new CGUIScriptEvent( tEventType, pFunc ) );
	return 0;
}