#pragma once

namespace FBGUI
{

namespace Lua
{

class CGUILuaModule : public CSingleton< CGUILuaModule >
{
public:
	std::string		mFileName;
	lua_State*		mpLuaModule;

public:
	CGUILuaModule( const char* pFileName = "" );
public:
	~CGUILuaModule( );

public:
	// pFun 脚本函数名称, pErrorDesc 错误描述, 返回0 成功, -1表示失败
	// 系统启动的时候，此时Sheet还没有建立，所以这里创建字体比较合适
	int OnSystemStartup( const char* pFun );

	// pFun 脚本函数名称, pErrorDesc 错误描述, 返回0 成功, -1表示失败
	// 系统启动完毕，此时Sheet已经，这里建立窗口比较合适
	int	OnInitInstance( const char* pFun );

	// pFun 脚本函数名称, pErrorDesc 错误描述, 返回0 成功, -1表示失败
	int FireEvent( const char* pFun, CGUISink* pSink );
};

class CGUILuaInterface
{
public:
	enum CANVAS_TYPE
	{
		CANVAS_WINDOW	= 0,
		CANVAS_CLIENT	= 1,
	};

public:
	// 功能:		输出调试文本
	// 函数原型:	void DumpText( string )
	static int DumpText( lua_State* pLua );

	// 功能:		装入纹理
	// 函数原型:	CGUITexture* LoadTexture( string )
	static int LoadTexture( lua_State* pLua );

	// 功能:		建立纹理
	// 函数原型:	CGUITexture* CreateTexture( string, int, int )
	static int CreateTexture( lua_State* pLua );

	// 功能:		绘制矩形
	// 函数原型:	void DrawRect( int, int, int, int, int, int, int )
	static int DrawRect( lua_State* pLua );

	// 功能:		绘制直线
	// 函数原型:	void DrawLine( int, int, int, int, int, int )
	static int DrawLine( lua_State* pLua );

	// 功能:		建立画布
	// 函数原型:	CGUICanvas* CreateWindowCanvas( string, int )
	static int CreateCanvas( lua_State* pLua );

	// 功能:		销毁画布
	// 函数原型:	void DestroyWindowCanvas( int )
	static int DestroyCanvas( lua_State* pLua );

	// 功能:		弹出对话框
	// 函数原型:	CGUIWindow* GUIDoModal( string, string, int, int, int, int, int, int )
	static int GUIDoModal( lua_State* pLua );

	// 功能:		弹出消息框
	// 函数原型:	CGUIWindow* MessageBox( string, string, int )
	static int GUIMessageBox( lua_State* pLua );

	// 功能:		得到输入框文本
	// 函数原型:	string GetGUIEditText( string )
	static int GetGUIEditText( lua_State* pLua );

	// 功能:		设置输入框文本
	// 函数原型:	void SetGUIEditText( string )
	static int SetGUIEditText( lua_State* pLua );

	// 功能:		得到窗口坐标
	// 函数原型:	int GetGUIWindowRect( string )
	static int GetGUIWindowRect( lua_State* pLua );

	// 功能:		得到窗口尺寸
	// 函数原型:	void GetGUIWindowSize( string, int )
	static int GetGUIWindowSize( lua_State* pLua );

	// 功能:		得到客户区坐标
	// 函数原型:	int GetGUIClientRect( string )
	static int GetGUIClientRect( lua_State* pLua );

	// 功能:		得到客户区尺寸
	// 函数原型:	void GetGUIClientSize( string, int )
	static int GetGUIClientSize( lua_State* pLua );

	// 功能:		屏幕坐标转到客户区坐标
	// 函数原型:	int GetGUIClientRect( string )
	static int ScreenToClient( lua_State* pLua );

	// 功能:		屏幕坐标转到窗口坐标
	// 函数原型:	void GetGUIClientSize( string, int )
	static int ScreenToWindow( lua_State* pLua );

	// 功能:		设置窗口锚
	// 函数原型:	void SetGUIWindowArchor( string, int )
	static int SetGUIWindowArchor( lua_State* pLua );

	// 功能:		设置窗口文本
	// 函数原型:	void SetGUIWindowText( string )
	static int SetGUIWindowText( lua_State* pLua );

	// 功能:		得到窗口文本
	// 函数原型:	string GetGUIWindowText( string )
	static int GetGUIWindowText( lua_State* pLua );

	// 功能:		建立字体 
	// 函数原型:	CGUIFont* CreateFont( string, string, int, int, int, int, bool, int )
	static int CreateGUIFont( lua_State* pLua );

	// 功能:		销毁字体 
	// 函数原型:	void DestroyFont( string )
	static int DestroyGUIFont( lua_State* pLua );

	// 功能:		得到字体
	// 函数原型:	CGUIFont* GetFont( string )
	static int GetGUIFont( lua_State* pLua );

	// 功能:		建立窗口 
	// 函数原型:	CGUIWindow* CreateWindow( string, string, int, int, int, int, int, int, int )
	static int CreateGUIWindow( lua_State* pLua );

	// 功能:		设置窗口事件
	// 函数原型:	void SetEvent( string, int, string )
	static int SetGUIEvent( lua_State* pLua );

	// 功能:		销毁窗口 
	// 函数原型:	void DestroyWindow( string )
	static int DestroyGUIWindow( lua_State *pLua );

	// 功能:		得到窗口
	// 函数原型:	CGUIWindow* GetWindow( string )
	static int GetGUIWindow( lua_State* pLua );

	// 功能:		得到窗口Sheet
	// 函数原型:	CGUIWindow* GetSheet( )
	static int GetGUISheet( lua_State* pLua );
};

};

};