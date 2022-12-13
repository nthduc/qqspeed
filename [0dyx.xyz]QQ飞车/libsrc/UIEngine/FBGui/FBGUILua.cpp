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
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>脚本错误</C>)", (char*) lua_tostring( mpLuaModule, 0 ) );
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
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>脚本错误</C>)", (char*) lua_tostring( mpLuaModule, 0 ) );
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
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>脚本错误</C>)", (char*) lua_tostring( mpLuaModule, 0 ) );
		return -1;
	}

	return 0;
}

// ************************************************************ //
// CGUILuaInterface
// ************************************************************ //

// 功能:		输出调试文本
// 函数原型:	void DumpText( string )
int CGUILuaInterface::DumpText( lua_State* pLua )
{
	const char* pDebugText = lua_tostring( pLua, 1 );
	CGUISystem::GetSingleton( ).DumpText( pDebugText );
	return 1;
}

// 功能:		装入纹理
// 函数原型:	CGUITexture* LoadTexture( string )
int CGUILuaInterface::LoadTexture( lua_State* pLua )
{
	const char* pTexName	= lua_tostring( pLua, 1 );
	CGUITexture* tpTexture	= NULL;
	GUI_LOADTEXTURE( tpTexture, pTexName );
	if ( tpTexture != NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "纹理[%s]不存在, (<C255,0,255,0>脚本错误</C>)", pTexName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpTexture );
	return 1;
}

// 功能:		建立纹理
// 函数原型:	CGUITexture* CreateTexture( string, int, int )
int CGUILuaInterface::CreateTexture( lua_State* pLua )
{
	const char* pTexName	= lua_tostring( pLua, 1 );
	int tTexWidth			= lua_tonumber( pLua, 2 );
	int tTexHeight			= lua_tonumber( pLua, 3 );
	CGUITexture* tpTexture	= CGUISystem::GetSingleton( ).GetRenderDriver( )->CreateTexture( pTexName, tTexWidth, tTexHeight );
	if ( tpTexture != NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "纹理[%s]建立失败, (<C255,0,255,0>脚本错误</C>)", pTexName );
		return 0;
	}
	
	lua_pushlightuserdata( pLua, tpTexture );
	return 1;
}

// 功能:		建立画布
// 函数原型:	CGUICanvas* CreateCanvas( string, int )
int CGUILuaInterface::CreateCanvas( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// 得到窗口名称
	int tType = lua_tonumber( pLua, 2 );				// 得到画布类型
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
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

// 功能:		销毁画布
// 函数原型:	void DestroyCanvas( int )
int CGUILuaInterface::DestroyCanvas( lua_State* pLua )
{
	CGUICanvas* tpCanvas = (CGUICanvas*) lua_touserdata( pLua, 1 );
	GUI_DELETEPTR( tpCanvas );
	return 0;
}

// 功能:		绘制矩形
// 函数原型:	void DrawRect( int, int, int, int, int, int, int )
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
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>脚本错误</C>)", "无效画布" );
		return 0;
	}

	tpCanvas->DrawRect( CGUIRect( tX1, tY1, tX2, tY2 ), GUI_MAKECOLOUR( tAlpha1, tRed1, tGreen1, tBlue1 ),
		GUI_MAKECOLOUR( tAlpha2, tRed2, tGreen2, tBlue2 ) );
	return 0;
}

// 功能:		绘制直线
// 函数原型:	void DrawLine( int, int, int, int, int, int )
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
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>脚本错误</C>)", "无效画布" );
		return 0;
	}

	tpCanvas->DrawLine( CGUIPoint( tX1, tY1 ), CGUIPoint( tX2, tY2 ), GUI_MAKECOLOUR( tAlpha1, tRed1, tGreen1, tBlue1 ) );
	return 0;
}

// 功能:		弹出对话框
// 函数原型:	CGUIWindow* GUIDoModal( string, string, int, int, int, int, int, int )
int CGUILuaInterface::GUIDoModal( lua_State* pLua )
{
	const char* pType = lua_tostring( pLua, 1 );	// 得到类型名称
	const char* pName = lua_tostring( pLua, 2 );	// 得到窗口名称
	int tStyle	= lua_tonumber( pLua, 3 );			// 得到窗口风格
	int tLeft	= lua_tonumber( pLua, 4 );			// 得到窗口横坐标
	int tTop	= lua_tonumber( pLua, 5 );			// 得到窗口纵坐标
	int tCX		= lua_tonumber( pLua, 6 );			// 得到窗口宽度
	int tCY		= lua_tonumber( pLua, 7 );			// 得到窗口高度
	void* tpUserData	 = (void*)  lua_touserdata( pLua, 9 );		// 得到窗口数据
	
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).DoModal( pType, pName, tStyle, CGUIRect( CGUIPoint( tLeft, tTop ), CGUISize( tCX, tCY ) ), tpUserData );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// 功能:		弹出消息框
// 函数原型:	CGUIWindow* MessageBox( string, string, int )
int CGUILuaInterface::GUIMessageBox( lua_State* pLua )
{
	const char* pMessage = lua_tostring( pLua, 1 );		// 得到消息内容
	const char* pCaption = lua_tostring( pLua, 2 );		// 得到标题
	int tStyle = lua_tonumber( pLua, 3 );				// 得到风格
	CGUIWindow* tpWindow = CGUISystem::GetSingleton( ).Message( pMessage, pCaption, tStyle );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "消息窗口建立失败(<C255,0,255,0>脚本错误</C>)" );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// 功能:		得到输入框文本
// 函数原型:	string GetGUIEditText( string )
int CGUILuaInterface::GetGUIEditText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// 得到窗口名称
	CGUIWindow* tpEdit = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpEdit == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	if ( ::strcmp( tpEdit->GetType( ).c_str( ), "GUIEdit" ) != 0 )
	{
		CGUISystem::GetSingleton( ).DumpText( "指定窗口类型不是Edit(<C255,0,255,0>脚本错误</C>)" );
		return 0;
	}

	std::string tText = ( (CGUIEdit*) tpEdit )->GetEditText( );
	lua_pushstring( pLua, ( (CGUIEdit*) tpEdit )->GetEditText( ).c_str( ) );
	return 1;
}

// 功能:		设置输入框文本
// 函数原型:	void SetGUIEditText( string )
int CGUILuaInterface::SetGUIEditText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// 得到窗口名称
	const char* pText = lua_tostring( pLua, 2 );
	CGUIWindow* tpEdit = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpEdit == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

 	if ( ::strcmp( tpEdit->GetType( ).c_str( ), "GUIEdit" ) == 0 )
	{
		CGUISystem::GetSingleton( ).DumpText( "指定窗口类型不是Edit(<C255,0,255,0>脚本错误</C>)" );
		return 0;
	}

	return 0;	
}

// 功能:		设置窗口锚
// 函数原型:	void SetGUIWindowArchor( string, int )
int CGUILuaInterface::SetGUIWindowArchor( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	int tArchor = lua_tonumber( pLua, 2 );			// 得到窗口锚
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	tpWindow->SetArchor( tArchor );
	return 0;
}

// 功能:		得到窗口坐标
// 函数原型:	int GetGUIWindowRect( string )
int CGUILuaInterface::GetGUIWindowRect( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	CGUIRect tRect = tpWindow->GetWindowRect( );
	lua_pushnumber( pLua, tRect.mTopLeft.mX );
	lua_pushnumber( pLua, tRect.mTopLeft.mY );
	lua_pushnumber( pLua, tRect.mBottomRight.mX );
	lua_pushnumber( pLua, tRect.mBottomRight.mY );
	return 4;
}

// 功能:		得到窗口大小
// 函数原型:	int GetGUIWindowSize( string )
int CGUILuaInterface::GetGUIWindowSize( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	CGUISize tSize = tpWindow->GetWindowSize( );
	lua_pushnumber( pLua, tSize.mCX );
	lua_pushnumber( pLua, tSize.mCY );
	return 2;
}

// 功能:		得到客户区坐标
// 函数原型:	int GetGUIClientRect( string )
int CGUILuaInterface::GetGUIClientRect( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	CGUIRect tRect = tpWindow->GetClientRect( );
	lua_pushnumber( pLua, tRect.mTopLeft.mX );
	lua_pushnumber( pLua, tRect.mTopLeft.mY );
	lua_pushnumber( pLua, tRect.mBottomRight.mX );
	lua_pushnumber( pLua, tRect.mBottomRight.mY );
	return 4;
}

// 功能:		得到客户区大小
// 函数原型:	int GetGUIClientSize( string )
int CGUILuaInterface::GetGUIClientSize( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	CGUISize tSize = tpWindow->GetClientSize( );
	lua_pushnumber( pLua, tSize.mCX );
	lua_pushnumber( pLua, tSize.mCY );
	return 2;
}

// 功能:		屏幕坐标转到窗口坐标
// 函数原型:	int ScreenToWindow( string )
int CGUILuaInterface::ScreenToWindow( lua_State* pLua )
{
	int tArgNum = lua_gettop( pLua );
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
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
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>脚本错误</C>)", "未知的函数类型" );
		return 0;
	}
}

// 功能:		屏幕坐标转到客户区坐标
// 函数原型:	int ScreenToClient( string )
int CGUILuaInterface::ScreenToClient( lua_State* pLua )
{
	int tArgNum = lua_gettop( pLua );
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
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
		CGUISystem::GetSingleton( ).DumpText( "%s(<C255,0,255,0>脚本错误</C>)", "未知的函数类型" );
		return 0;
	}
}

// 功能:		设置窗口文本
// 函数原型:	void SetGUIWindowText( string )
int CGUILuaInterface::SetGUIWindowText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// 得到窗口名称
	const char* pText = lua_tostring( pLua, 2 );
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	tpWindow->SetText( pText );
	return 0;
}

// 功能:		得到窗口文本
// 函数原型:	string GetGUIWindowText( string )
int CGUILuaInterface::GetGUIWindowText( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	const char* pText = tpWindow->GetText( ).c_str( );
	lua_pushstring( pLua, pText );
	return 1;
}

// 功能:		建立字体 
// 函数原型:	CGUIFont* CreateFont( string, string, int, int, int, int, bool, int )
int CGUILuaInterface::CreateGUIFont( lua_State* pLua )
{
	const char* pFontName	= lua_tostring( pLua, 1 );	// 得到字体名称
	const char* pFaceName	= lua_tostring( pLua, 2 );	// 得到字体表面名称
	int tFontHeight			= lua_tonumber( pLua, 3 );	// 得到字体高度
	int tEdge		= lua_tonumber( pLua, 4 );			// 得到字体边宽
	int tHOffset	= lua_tonumber( pLua, 5 );			// 得到字体边横向偏移
	int tVOffset	= lua_tonumber( pLua, 6 );			// 得到字体纵横向偏移
	bool tAnsi		= lua_toboolean( pLua, 7 );			// 得到字体是否抗锯齿
	int tCodePage	= lua_tonumber( pLua, 8 );			// 得到字体代码页
	if ( tCodePage == 0 )
	{
		CGUIFont* tpFont = CGUIFontManager::GetSingleton( ).Create( pFontName, pFaceName, tFontHeight, tEdge, tHOffset, tVOffset, tAnsi, NULL );
		lua_pushlightuserdata( pLua, tpFont );
	}
	else
		lua_pushlightuserdata( pLua, NULL );

	return 1;
}

// 功能:		销毁字体 
// 函数原型:	void DestroyFont( string )
int CGUILuaInterface::DestroyGUIFont( lua_State* pLua )
{
	const char* pFontName = lua_tostring( pLua, 1 );		// 得到字体名称
	CGUIFontManager::GetSingleton( ).Destroy( pFontName );
	return 0;
}

// 功能:		得到字体
// 函数原型:	CGUIFont* GetFont( string )
int CGUILuaInterface::GetGUIFont( lua_State* pLua )
{
	const char* pFontName = lua_tostring( pLua, 1 );		// 得到字体名称
	CGUIFont* tpFont = CGUIFontManager::GetSingleton( ).GetFont( pFontName );
	if ( tpFont == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "字体%s不存在(<C255,0,255,0>脚本错误</C>)", pFontName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpFont );
	return 1;
}

// 功能:		建立窗口 
// 函数原型:	CGUIWindow* CreateWindow( string, string, int, int, int, int, int, int, int )
int CGUILuaInterface::CreateGUIWindow( lua_State* pLua )
{
	const char* pType = lua_tostring( pLua, 1 );	// 得到类型名称
	const char* pName = lua_tostring( pLua, 2 );	// 得到窗口名称
	int tStyle	= lua_tonumber( pLua, 3 );			// 得到窗口风格
	int tLeft	= lua_tonumber( pLua, 4 );			// 得到窗口横坐标
	int tTop	= lua_tonumber( pLua, 5 );			// 得到窗口纵坐标
	int tCX		= lua_tonumber( pLua, 6 );			// 得到窗口宽度
	int tCY		= lua_tonumber( pLua, 7 );			// 得到窗口高度
	CGUIWindow* tpParent = (CGUIWindow*) lua_touserdata( pLua, 8 );	// 得到窗口父窗口
	void* tpUserData	 = (void*)  lua_touserdata( pLua, 9 );		// 得到窗口数据
	
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).Create( pType, pName, tStyle, CGUIRect( CGUIPoint( tLeft, tTop ), CGUISize( tCX, tCY ) ),	tpParent, tpUserData );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// 功能:		销毁窗口 
// 函数原型:	void DestroyWindow( string )
int CGUILuaInterface::DestroyGUIWindow( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );		// 得到窗口名称
	CGUIWindowManager::GetSingleton( ).Destroy( pName );
	return 0;
}

int CGUILuaInterface::GetGUIWindow( lua_State* pLua )
{
	const char* pName = lua_tostring( pLua, 1 );	// 得到窗口名称
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpWindow );
	return 1;
}

// 功能:		得到窗口Sheet
// 函数原型:	CGUIWindow* GetSheet( )
int CGUILuaInterface::GetGUISheet( lua_State* pLua )
{
	CGUIWindow* tpSheet = CGUISystem::GetSingleton( ).GetSheet( );
 	if ( tpSheet == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "系统Sheet不存在(<C255,0,255,0>脚本错误</C>)" );
		return 0;
	}

	lua_pushlightuserdata( pLua, tpSheet );
	return 1;
}

// 功能:		设置窗口事件
// 函数原型:	void SetEvent( string, int, string )
int CGUILuaInterface::SetGUIEvent( lua_State* pLua )
{
	const char* pName	= lua_tostring( pLua, 1 );	// 得到窗口名称
	int tEventType		= lua_tonumber( pLua, 2 );	// 得到事件类型
	const char* pFunc	= lua_tostring( pLua, 3 );	// 得到处理函数
	CGUIWindow* tpWindow = CGUIWindowManager::GetSingleton( ).GetWindow( pName );
	if ( tpWindow == NULL )
	{
		CGUISystem::GetSingleton( ).DumpText( "窗口[%s]不存在(<C255,0,255,0>脚本错误</C>)", pName );
		return 0;
	}

	tpWindow->AddScriptEvent( new CGUIScriptEvent( tEventType, pFunc ) );
	return 0;
}