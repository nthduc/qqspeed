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
	// pFun �ű���������, pErrorDesc ��������, ����0 �ɹ�, -1��ʾʧ��
	// ϵͳ������ʱ�򣬴�ʱSheet��û�н������������ﴴ������ȽϺ���
	int OnSystemStartup( const char* pFun );

	// pFun �ű���������, pErrorDesc ��������, ����0 �ɹ�, -1��ʾʧ��
	// ϵͳ������ϣ���ʱSheet�Ѿ������ｨ�����ڱȽϺ���
	int	OnInitInstance( const char* pFun );

	// pFun �ű���������, pErrorDesc ��������, ����0 �ɹ�, -1��ʾʧ��
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
	// ����:		��������ı�
	// ����ԭ��:	void DumpText( string )
	static int DumpText( lua_State* pLua );

	// ����:		װ������
	// ����ԭ��:	CGUITexture* LoadTexture( string )
	static int LoadTexture( lua_State* pLua );

	// ����:		��������
	// ����ԭ��:	CGUITexture* CreateTexture( string, int, int )
	static int CreateTexture( lua_State* pLua );

	// ����:		���ƾ���
	// ����ԭ��:	void DrawRect( int, int, int, int, int, int, int )
	static int DrawRect( lua_State* pLua );

	// ����:		����ֱ��
	// ����ԭ��:	void DrawLine( int, int, int, int, int, int )
	static int DrawLine( lua_State* pLua );

	// ����:		��������
	// ����ԭ��:	CGUICanvas* CreateWindowCanvas( string, int )
	static int CreateCanvas( lua_State* pLua );

	// ����:		���ٻ���
	// ����ԭ��:	void DestroyWindowCanvas( int )
	static int DestroyCanvas( lua_State* pLua );

	// ����:		�����Ի���
	// ����ԭ��:	CGUIWindow* GUIDoModal( string, string, int, int, int, int, int, int )
	static int GUIDoModal( lua_State* pLua );

	// ����:		������Ϣ��
	// ����ԭ��:	CGUIWindow* MessageBox( string, string, int )
	static int GUIMessageBox( lua_State* pLua );

	// ����:		�õ�������ı�
	// ����ԭ��:	string GetGUIEditText( string )
	static int GetGUIEditText( lua_State* pLua );

	// ����:		����������ı�
	// ����ԭ��:	void SetGUIEditText( string )
	static int SetGUIEditText( lua_State* pLua );

	// ����:		�õ���������
	// ����ԭ��:	int GetGUIWindowRect( string )
	static int GetGUIWindowRect( lua_State* pLua );

	// ����:		�õ����ڳߴ�
	// ����ԭ��:	void GetGUIWindowSize( string, int )
	static int GetGUIWindowSize( lua_State* pLua );

	// ����:		�õ��ͻ�������
	// ����ԭ��:	int GetGUIClientRect( string )
	static int GetGUIClientRect( lua_State* pLua );

	// ����:		�õ��ͻ����ߴ�
	// ����ԭ��:	void GetGUIClientSize( string, int )
	static int GetGUIClientSize( lua_State* pLua );

	// ����:		��Ļ����ת���ͻ�������
	// ����ԭ��:	int GetGUIClientRect( string )
	static int ScreenToClient( lua_State* pLua );

	// ����:		��Ļ����ת����������
	// ����ԭ��:	void GetGUIClientSize( string, int )
	static int ScreenToWindow( lua_State* pLua );

	// ����:		���ô���ê
	// ����ԭ��:	void SetGUIWindowArchor( string, int )
	static int SetGUIWindowArchor( lua_State* pLua );

	// ����:		���ô����ı�
	// ����ԭ��:	void SetGUIWindowText( string )
	static int SetGUIWindowText( lua_State* pLua );

	// ����:		�õ������ı�
	// ����ԭ��:	string GetGUIWindowText( string )
	static int GetGUIWindowText( lua_State* pLua );

	// ����:		�������� 
	// ����ԭ��:	CGUIFont* CreateFont( string, string, int, int, int, int, bool, int )
	static int CreateGUIFont( lua_State* pLua );

	// ����:		�������� 
	// ����ԭ��:	void DestroyFont( string )
	static int DestroyGUIFont( lua_State* pLua );

	// ����:		�õ�����
	// ����ԭ��:	CGUIFont* GetFont( string )
	static int GetGUIFont( lua_State* pLua );

	// ����:		�������� 
	// ����ԭ��:	CGUIWindow* CreateWindow( string, string, int, int, int, int, int, int, int )
	static int CreateGUIWindow( lua_State* pLua );

	// ����:		���ô����¼�
	// ����ԭ��:	void SetEvent( string, int, string )
	static int SetGUIEvent( lua_State* pLua );

	// ����:		���ٴ��� 
	// ����ԭ��:	void DestroyWindow( string )
	static int DestroyGUIWindow( lua_State *pLua );

	// ����:		�õ�����
	// ����ԭ��:	CGUIWindow* GetWindow( string )
	static int GetGUIWindow( lua_State* pLua );

	// ����:		�õ�����Sheet
	// ����ԭ��:	CGUIWindow* GetSheet( )
	static int GetGUISheet( lua_State* pLua );
};

};

};