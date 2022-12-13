
#ifndef __WIDGET_H__
#define __WIDGET_H__

#pragma warning (disable:4786)

#include <deque>
#include <map>
#include <set>
#include <stack>

#include "vfilewrap.h"
#include "gameui.h"


//重要的全局变量///////////////////////////////
extern int UI_MAX_INTEGER;
extern stUICoreStyle g_UICoreStyle;
extern std::vector<std::string> g_vecScriptFuncs;
extern CallBackFunc g_vecCallbackFuncs[UI_Callback_MAXNUM];
extern std::set<IWgtData*> g_vecSecuritySet;
//重要的全局变量///////////////////////////////

class CUIImage;
class CUICursor;
class CUIWidget;



//界面定义(消息)
enum UIMsgType
{

	//MESSAGE
	MSGTYPE_UPDATE,
	MSGTYPE_PAINT,
	MSGTYPE_CHAR,
	MSGTYPE_IME_CHAR,
	MSGTYPE_KEYPRESS,
	MSGTYPE_KEYDOWN,
	MSGTYPE_KEYUP,
	MSGTYPE_LEFTBTNDOWN,
	MSGTYPE_LEFTBTNUP,
	MSGTYPE_LBTNDBLCLK,
	MSGTYPE_RIGHTBTNDOWN,
	MSGTYPE_RIGHTBTNUP,
	MSGTYPE_MOUSEMOVE,
	MSGTYPE_MOUSEIN,
	MSGTYPE_MOUSEOUT,
	MSGTYPE_GAINFOCUS,
	MSGTYPE_LOSTFOCUS,
	MSGTYPE_TIMER,
	MSGTYPE_MOUSEWHEEL,
	MSGTYPE_MOUSEIDLE,

	
};

//界面定义(通知)
enum UINotifyType
{


//SELF NOTIFY//////////////////////////////////////////////////////////////////////////

	NTFTYPE_AJUSTTING=0,//窗口调整大小
	NTFTYPE_DRAGGING,//控件位置更新


//CHILD NOTIFY//////////////////////////////////////////////////////////////////////////

	NTFTYPE_SPINARROW,
	NTFTYPE_SCROLLBAR_POS,
	NTFTYPE_SCROLLBAR_RANGE,
	NTFTYPE_SLIDERBAR,

	
	NTFTYPE_COMMAND_OK,//按钮
	NTFTYPE_COMMAND_CANCEL,//按钮
	NTFTYPE_COMMAND_CLICK,//按钮
	NTFTYPE_COMMAND_CHECK,//检查框

	NTFTYPE_CHILD_DRAGGING,//子控件位置更新	
	NTFTYPE_CHILD_SHOWHIDE,//子窗口可见性改变

	//NTFTYPE_ITEM_DRAG,//子项目拖动
	//NTFTYPE_ITEM_DROP,//子项目放下
	
	
};

enum UIParse
{

	UI_PARSE_EXACT	=0,
	UI_PARSE_NEAR	=1,
	UI_PARSE_APPROX	=2,


};


enum UICmd
{

	UI_Cmd_NONE=0,
	UI_Cmd_SHOW,
	UI_Cmd_HIDE,
	UI_Cmd_TOGGLE,

};


enum EdgeLine
{
	EDGE_LINE_NONE	=-1,
	
	EDGE_LINE_UP,
	EDGE_LINE_DOWN,
	EDGE_LINE_LEFT,
	EDGE_LINE_RIGHT,

	EDGE_LINE_MAX,
};


enum EdgeFlag
{
	EDGE_FLAG_NONE	=0,
	EDGE_FLAG_UP	=1,
	EDGE_FLAG_DOWN	=2,
	EDGE_FLAG_LEFT	=4,
	EDGE_FLAG_RIGHT	=8,
};

enum EdgeCheck
{
	EDGE_CHECK_NONE		=0,	
	EDGE_CHECK_SHOW		=1,	
	EDGE_CHECK_UPDATE	=2,	
	EDGE_CHECK_AUTO	    =4,	
};

enum RectFlag
{
	RECT_FLAG_WINDOW	=0,
	RECT_FLAG_CLIENT	=1,
	RECT_FLAG_CAPTION	=2,
	RECT_FLAG_DRAG		=4,
};

//add by lincoln [2006:7:18] [11:54]
//{{{界面库效率优化
enum UpdateMute
{
	UPDATE_MUTE_NONE	=0,//allways update
	UPDATE_MUTE_VISIBLE	=1,//not update when invisible
};
//}}}

//m_AlphaState的最低位：0--decrease,1-increase
//m_AlphaState的次低位：0--normal,1-paused
enum UIAlphaState
{
	ALPHA_STATE_DECREASE	= 0,  //需要让窗口的透明度减少
	ALPHA_STATE_INCREASE	= 1,  //需要让窗口的透明度增加
	ALPHA_STATE_PAUSE		= 2,  //当前是否暂停了
};

//每个窗口包含4个状态：clickdown,mouseover,normal,disable
const int FRAME_STATE_MAX=4;
enum
{
	
	FRAME_STATE_CLICK		=0,
	FRAME_STATE_OVER		=1,
	FRAME_STATE_NORMAL		=2,
	FRAME_STATE_DISABLE		=3,
		
};

//状态切换按钮最多可容纳的状态
const int FRAME_SCHEME_MAX=50;
struct  stFrameScheme
{
	
	//帧范围
	int start[FRAME_STATE_MAX];
	int end[FRAME_STATE_MAX];

	//用默认的帧值0初始化
	stFrameScheme()
	{
		Clear(0);
	}

	//给所有帧指定帧值
	stFrameScheme(int value)
	{
		Clear(value);
	}


	//给所有帧指定帧值
	void Clear(int value)
	{
		for(int i=0;i<FRAME_STATE_MAX;i++)
		{ 
			start[i]=value;
			end[i]=value;
		}
	}


	//设置连续等距离的帧（起始帧，每个状态的帧数）
	void Set(int org,int off)
	{

		for(int i=0;i<FRAME_STATE_MAX;i++)
		{ 
			start[i]=org+off*i;
			end[i]=start[i]+off-1;
		}

	}
	
};

struct  stUIPathPoint
{

	//ID
	int id;

	//路径点位置
	int x;
	int y;
	
	//到底路径点后，执行的函数或者脚本
	int   command;
	
	//命令参数
	float alpha;
	float scale;
	int   width;
	int   height;


	
	
	stUIPathPoint()
	{
		id=-1;
		x=0;
		y=0;
		command=0;
		alpha=1.0;
		scale=1.0;
		width=0;
		height=0;
	}

	bool operator < (const stUIPathPoint & other) const
	{
		return id < other.id;

	}



};

enum  UIPathCmd
{

	UI_PATHCMD_NONE				=0,
	UI_PATHCMD_ALPHA			=1,
	UI_PATHCMD_SCALE			=2,
	UI_PATHCMD_SIZE				=4,
	UI_PATHCMD_PAUSE			=8,
	UI_PATHCMD_CYCLE			=16,
	UI_PATHCMD_FLIP				=32,
	UI_PATHCMD_RELATIVE			=64,

};

//处理拖放的结构
struct stDragInfo
{
	int type;//对应的窗口(购物栏,技能栏等)
	int index;//窗口中的第几个道具

	int hosttype;
	int hostindex;

	int itemtype;//对于物品的子类

	int maxnum;//拖动物品的最大数目
	int itemnum;//拖动物品的数目(-1:表示由客户端弹出数量输入框决定数目)
	
	stRect rect;//拖放开始时刻item的原始位置,与大小
	CUIWidget* pDragSrc;
	
	bool bAttach;
	bool bAjust;
	
	stDragInfo()
	{
		Reset();		
	}

	void Reset()
	{
		type=UI_DRAG_TYPE_NONE;
		index=-1;

		hosttype=UI_DRAG_TYPE_NONE;
		hostindex=-1;

		itemtype=UI_ITEM_NONE;

		itemnum=-1;

		rect=stRect();
		pDragSrc=0;
		
		bAttach=false;
		bAjust=false;

	}

	
	
};


//拼图索引
enum UIPatchId
{

	PATCHID_UP_LEFT		=0,
	PATCHID_UP_RIGHT,
	PATCHID_DOWN_LEFT,
	PATCHID_DOWN_RIGHT,
	
	//注意：顺序不能改变
	PATCHID_UP_MIDDLE,
	PATCHID_DOWN_MIDDLE,
	PATCHID_LEFT_MIDDLE,
	PATCHID_RIGHT_MIDDLE,
	
	//中间空白	
	PATCHID_MIDDLE,

	//数目
	PATCHID_NUMBER,	

};


enum UITouchFlag
{

	UI_TOUCH_NONE   	  = 0,
	UI_TOUCH_TOP_OUT	  = 1,
	UI_TOUCH_BOTTOM_OUT  = 1<<1,
	UI_TOUCH_LEFT_OUT    = 1<<2,	
	UI_TOUCH_RIGHT_OUT   = 1<<3,
	
};



//处理伙伴窗口的结构
//tooltip,msgbox,右键窗口,特殊伙伴等
struct stBuddyInfo
{
	int style;
	std::string path;
	CUIWidget* pBuddy;
	
	stBuddyInfo()
	{
		style=BUDDY_STYLE_SELF;
		path="";
		pBuddy=NULL;
	}
	
	stBuddyInfo(int stl,const char* pth,CUIWidget* pby)
	{					
		style=stl;
		path=pth;
		pBuddy=pby;
	}
	
	
};


struct stScriptFunc
{
	short funcId;
	std::string funcPath;

	stScriptFunc()
	{
		funcId=0;
		funcPath="";
	}

	stScriptFunc(short id,const char* path)
	{
		funcId=id;
		funcPath=path;
	}

};

struct stScreenDesc
{
	std::vector<std::string> luaPaths;

	stScreenDesc()
	{
		luaPaths.clear();
	}
};

//得到窗口对应的table中的一个变量(比如rect,比如children)
extern CVar *GetWidgetVar(const std::string idpath, int pos,CVarTable* pFatherVar);
extern BOOL LoadTemplateFile(const char *szFileName,std::string& strBuffer);
extern CUIWidget *LoadUIWidget( const char *pname, CVar *pvar, CUIWidget *pfather );

class CUIWidget : public IWgtData
{
	

public:

	CUIWidget();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	virtual ~CUIWidget();

	///纯虚函数///////////////////////////////////////////////////////////////////////
	
	
public:
	
	virtual bool IsClickDown();
	virtual bool IsMouseOver();
	virtual bool IsEnabled();
	virtual bool IsFocused();
	virtual bool IsChecked();

	//客户端要查询或者修改的内容
	virtual const char *GetWidgetID();//窗口id
	virtual const char *GetWidgetPathID();//窗口完整的(.children.)id
	virtual const char *GetWidgetPath();//窗口路径(不带.children.)
	
	virtual stCustomBuffer& GetCustomBuffer();

	virtual int  GetQueryType(){ return m_QueryType; }
	virtual int  GetQueryIndex(){ return m_QueryIndex; }
	virtual int  GetDragType(){ return m_DragType; }
	virtual int  GetDragIndex(){ return m_DragIndex; }
	virtual int  GetHostType(){ return m_HostType; }
	virtual int  GetHostIndex(){ return m_HostIndex; }


	//客户端自定义的数据
	//本来custom buffer具备了自定义扩展能力
	//但是，对于非常少的窗口自定义数据，可以用这个
	virtual int  GetCustomState();
	virtual int  GetCustomStyle();	
	virtual int  GetCustomId();
	virtual int  GetCustomType();
	virtual int  GetCustomIndex();
	virtual const char* GetCustomName();
	virtual const char* GetCustomText();
	virtual const char* GetCustomString();
	

	//客户端需要的绘制参数接口
	virtual stRect GetWinRect(){return m_WinRect;}//绝对窗口矩形
	virtual stRect GetAbsoluteClientRect();//绝对客户区矩形	
	virtual stRect GetAbsoluteCaptionRect();//绝对标题区矩形
	virtual stRect GetAbsoluteDragRect();//绝对拖放区矩形

	virtual double GetRenderFactor();//窗口当前透明因子
	virtual double GetScaleFactor();//窗口当前缩放因子


	virtual WidgetType GetType(){ return m_Type; }
	virtual int  GetState();
	virtual int  GetStyle();
	virtual int  GetPattern();
	virtual int  GetExtendStyle();

	virtual void SetVar(stUIVar& uiVar);
	virtual void GetVar(stUIVar& uiVar);
	
	virtual int  GetNumChild();
	virtual IWgtData* GetWgtChild( int i );
	virtual IWgtData* GetWgtFather();

	virtual std::vector<stAccelItem>& GetWgtAccels();
		
	
	//虚函数//////////////////////////////////////////////////////////////////////
	

	virtual BOOL IsPointIn( int x, int y );
	virtual stColorQuad GetPointColor( int x, int y );

	//是否接受中文输入
	virtual BOOL IsAcceptIme();
	
	//是否接受字符输入
	virtual BOOL IsAcceptInput();

	//在子窗口中找widget, 按照idpath
	virtual CUIWidget *FindWidgetByID( const std::string &idpath, int pos );

	
	virtual void DefaultFrameScheme();
	virtual CUIWidget *FindWidgetOnPoint( int x, int y );
	virtual CUIWidget *FindWidgetForAccel( const stAccelItem& accel );

	virtual stDragInfo ParseDragInfo(int x,int y);
	virtual bool ItemDrop(int x,int y);
	virtual int DrawDragItem(stDrawItem data);

	virtual int DragEnter(int x,int y);
	virtual int DragLeave(int x,int y);

	
	virtual void AddChild( CUIWidget *pwidget );
	virtual void DelChild( CUIWidget *pwidget );

	virtual void SetCaption( const char *pname );
	virtual const char *GetCaption();

	virtual void SetChecked( bool bCheck );

	virtual stColorQuad GetRenderColor(stColorQuad srcColor);

	virtual int  ParseDragIndex(int x,int y,int style);

	virtual const char* GetTipText(int nIndex);
	virtual void SetTipText( const char *pTip, int nIndex=-1);

	//在载入孩子之后设置自己的参数
	virtual void PostSetParameter( CVar *pvar, CUIWidget *pfather );
	
	//在载入孩子之前设置自己的参数
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );

	//构造函数只能设置m_Type，指针初始值，及只需初始化一次的变量
	//ClearParameter负责可重复的初始化操作
	virtual void ClearParameter();

	virtual void ConfirmScripts( CVar *pvar );
	
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );

	//孩子和父亲之间的通知消息,可以用于传递值.
	//如果不处理,一般也不再继续向上级传递
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	
	virtual	int  OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	
	virtual void OnPaste();
	virtual void OnCopy();
	virtual void OnCut();
	virtual void OnInsert(const char* pText);

	virtual void OnCmdOk();
	virtual void OnCmdCancel();
	virtual void EndWidget(bool bFlag);
	
	virtual void OnGainFocus();
	virtual void OnLostFocus();


	//到达或者原本就在某点时执行命令
	virtual void OnPathArrive(stUIPathPoint pt);
	virtual void OnPathCommand(stUIPathPoint pt);
	
	//向某点移动过程中，执行更新操作
	virtual void OnPathMove(stUIPathPoint pt, float factor);
	virtual void OnRecoverPathPoint();

	virtual void OnAlignDirty();
	
	virtual void OnLoad();
	virtual void OnInit();
	virtual void OnDenit();
	
	virtual void OnSchemeFrame();	
	virtual void OnPlusFrame();
	virtual void OnAffirmFrame();
	
	virtual void UpdateSelf( float deltatime );
	virtual void PostUpdateSelf( float deltatime );	
		
	virtual void ParseCursor(int x,int y);
	
	virtual int  ParseKeyChar(int x,int y, stKeyChar& keyChar);

	virtual int	 GetTable(int channel,lua_State *ls);
	virtual int	 SetTable(int channel,lua_State *ls);
	
	virtual void ActiveSelf(bool bActive);
	virtual void AjustSelf();
	
	virtual void PostRenderSelf();
	virtual void RenderSelf();
	virtual void RenderChild();
	virtual int  ChildQueryMoveTo(int &x,int &y,CUIWidget* pChild);
	

	virtual void DrawCaption();

	virtual void DrawBarEdge();
	virtual void DrawDebugBarEdge();

	virtual void OnSave(FILE* fp);
    virtual void OnSaveRecursive(FILE* fp);

	//////////////////////////////////非虚函数//////////////////////////////////////

	
	const char* GetScriptText(){return m_ScriptText.c_str();}
	void SetScriptText(const char* pText){m_ScriptText=pText;}
	

	void GetTemplString(int id,std::string& resultStr);
	int  GetTemplStringNum();
		
	void FormatTemplString(int idx,std::string& bufTemplate);

	CUIWidget* AddTemplate(int id);
	CUIWidget* AddTemplate(int id,int index);
	
	void ClearTemplates(int id);
	int  GetTemplatesNum(int id);
	void UpdateMove(float deltatime);
	void UpdateAlpha();

	//调用更新脚本函数
	void UpdateMornitor();

	void UpdateWakeArea();

	void DrawImageAreaH(stRect area,CUIImage* pImge,DWORD color,int frame=0,bool bClip=TRUE);
	void DrawImageAreaV(stRect area,CUIImage* pImge,DWORD color,int frame=0,bool bClip=TRUE);
	void DrawImageArea(stRect area,CUIImage* pImge,DWORD color,int frame=0,bool bClip=TRUE);

	void DrawBackPic();
	void DrawEffectPic();

	void DrawAdornPic();
	void DrawPatchPic();

	CUIWidget* GetChild( int i );

	CUIWidget *GetFather();
	CUIWidget *GetGrandpa();
	CUIWidget *GetUncle(int alignId);//根据alignid查找叔叔
	CUIWidget *GetBrother(int alignId);//根据alignid查找兄弟

	int  GetFrameState();
	void UpdateFrame(float deltatime);

	BOOL IsWakeWidget();

	BOOL IsStaticWidget();
	void SetAsStaticWidget(BOOL bStatic);


	BOOL IsTemplateWgt();
	void SetAsTemplateWgt(BOOL bTemplate);
	
	CUIWidget* StaticQueryFather();

	CUIWidget* GetTopWidget();

	BOOL IsTopMost();
	void SetAsTopMost(BOOL bTopMost);
	void HideTopMost();
	void ShowTopMost();
	CUIWidget* GetFirstTopMost();

		
	void HideTooltip(bool bMouseOut,int x,int y);
	int ParseTooltip(int nDragIndex,stRect refRect,int xcursor=0,int ycursor=0);

	void HideInfotip();
	int  ParseInfotip(int nDragIndex);
	
	void SetTipFlag(int flag);
	int  GetTipFlag();

	void SetTipPos( int ox, int oy, int alignType, int alignStyle);
	
	void SetCursorName(const char *pCursor);
	const char* GetCursorName();

	
	void SetHorizonSizer(const char *pSizer);
	void SetVerticalSizer(const char *pSizer);

	const char* GetHorizonSizer();
	const char* GetVerticalSizer();
	
	void SetQueryType(int type){ m_QueryType= type; }
	void SetQueryIndex(int idx){ m_QueryIndex= idx; }

	void SetDragType(int type){ m_DragType= type; }
	void SetDragIndex(int idx){ m_DragIndex= idx; }
	void SetHostType(int type){ m_HostType= type; }
	void SetHostIndex(int idx){ m_HostIndex= idx; }

	int  GetAreaId(){ return m_AreaId; }
	void SetAreaId(int idx){ m_AreaId= idx; }

	void ReAlignChild();

	int SnapRect(stRect dstRect,stRect& srcRect,int nSnapH,int nSnapV);

	void AlignRectToSelf(stRect& srcRct,int nAlignType,int nAlignStyle,int nMarginH=0,int nMarginV=0);
	void AlignToWidget(CUIWidget* pDstWgt,int nAlignType,int nAlignStyle,int nMarginH=0,int nMarginV=0);
	void AlignToRect(stRect alignRect,int nAlignStyle,int nMarginH=0,int nMarginV=0);
	void AlignRect(stRect dstRect,stRect& srcRect,int nAlignStyle,int nMarginH=0,int nMarginV=0);

	void Move(  int dx, int dy );		
	void MoveTo(  int x, int y );
	void MoveToX( int x );
	void MoveToY( int y );	


	
	void ExtendTo( int value,int edgeFlag,int rectFlag);	
	void ExtendEdge( int addition,int edgeFlag,int rectFlag);	
	int  AjustWidth(int addition,int rectFlag);
	int  AjustHeight(int addition,int rectFlag);
	
	void SetBackColor( stColorQuad color );
	void SetDrawColor( stColorQuad color );
	void SetFocusColor( stColorQuad color );
	void SetEdgeColor( stColorQuad color );
	
	void SetMagicColor( stColorQuad color );
	void SetLinkColor( stColorQuad color );

    stColorQuad GetBackColor( );
	stColorQuad GetDrawColor( );
	stColorQuad GetFocusColor( );
	stColorQuad GetEdgeColor( );

	stColorQuad GetMagicColor( );
	stColorQuad GetLinkColor( );
	
	stColorQuad GetCaptionColor( );

	void SetPreLayer( int i );
	int  GetPreLayer();
			
	void SetPlusLayer( int i );
	int  GetPlusLayer();

	void SetInitLayer(int i);
	int  GetInitLayer();

	int GetLayer();

	void SetDrawFlag(int flag); 
	int GetDrawFlag(); 

	void SetCurFrame(int frame); 
	int GetCurFrame(); 

	bool IsImeEnabled();
	void EnableIme( bool bFlag);
	
	bool IsAlignDirty();
	void SetAlignDirty( bool bDirty);

	bool IsUpdateDirty();
	void SetUpdateDirty( bool bDirty);

	void SetFocused(bool bFocus);
	
	bool IsActived();
	void SetActived( bool bActive);	

	bool IsSorted();
	void SetSorted(bool bSort);

	bool IsAligned();
	void SetAligned(bool bAlign);
	
	bool IsAjusted();
	void SetAjusted(bool bAjust);
	
	bool IsLoaded();
	void SetLoaded(bool bLoad);

	void SetGroupId(int id);
	int  GetGroupId();

	void SetAlignId(int id);
	int  GetAlignId();

	void SetRectId(int id,CUIWidget* pfather);
	void SetRectId(int id);
	int  GetRectId();

	void SetAlignToId(int id);
	int  GetAlignToId();

	void SetMarginH(int margin);
	int  GetMarginH();


	void SetMarginV(int margin);
	int  GetMarginV();

	int  GetSnapH();	
	void SetSnapH(int snap);

	int  GetSnapV();	
	void SetSnapV(int snap);

	int  GetAjustEdge();	
	void SetAjustEdge(int edge);

	int  GetAjustState();	
	void SetAjustState(int state);

	int  GetAjustFlag();	
	void SetAjustFlag(int flag);

	int  GetEdgeThick();	
	void SetEdgeThick(int thick);

	int  GetEdgeCheck();	
	void SetEdgeCheck(int check);

	//add by lincoln [2006:7:18] [11:54]
	//{{{界面库效率优化
	int  GetUpdateMuteFlag();
	void SetUpdateMuteFlag(int flag);
	//}}}
	int  GetTemplateId();	
	void SetTemplateId(int index);

	int  GetMouseDelta();	
	void SetMouseDelta(int delta);

	int  GetMinHeight();	
	void SetMinHeight(int height);

	int  GetMaxHeight();	
	void SetMaxHeight(int height);

	int  GetMinWidth();	
	void SetMinWidth(int width);
	
	int  GetMaxWidth();	
	void SetMaxWidth(int width);
	
	int  GetTextFont();
	void SetTextFont(int font);
	
	int  GetTextSize();
	void SetTextSize(int size);

	void SetRowSpace(int space); 
	int  GetRowSpace(); 
	
	int  GetRichMode();
	void SetRichMode(int mode);

	int  GetRichFlag();
	void SetRichFlag(int flag);	
	
	void SetTabStop(int id);
	int  GetTabStop();


	void SetChildStop(int id);
	int  GetChildStop();
	
	void SetGroupStop(int id);
	int  GetGroupStop();

	
	void SetTextStyle(int style);
	int  GetTextStyle();

	void SetPattern(int pattern);

	void SetStyle(int style);

	void SetExtendStyle(int style);

	void SetCustomState(int state);
	
	void SetCustomStyle(int style);
	
	void SetCustomId(int id);
	
	void SetCustomType(int type);
	
	void SetCustomIndex(int index);


	void SetCustomName(const char* txt);
	void SetCustomText(const char* txt);
	void SetCustomString(const char* txt);

	void SetAlphaSpeed(double speed);
	double GetAlphaSpeed();

	void SetAlphaMin(double factor);
	void SetAlphaMax(double factor);

	void SetAlphaFactor(double factor);

	void SetAlphaState(int state);
	int	 GetAlphaState();
	
	void  SetScaleFactor(double factor);
	void SetAutoScale(BOOL Value);

	void SetIdleTime(float time);
	float  GetIdleTime();
	
	double GetPathSpeed();	
	void   SetPathSpeed(double speed);

	double GetFrameSpeed();	
	void   SetFrameSpeed(double speed);
	
	void SetAlignType(int type);
	int  GetAlignType();

	void SetAlignStyle(int style);
	int  GetAlignStyle();

	void SetWidgetID( const char *pstr );//窗口id
		
	void RegisterBuddyWgt(int type,int style,const char* pathid="",CUIWidget* pBuddy=NULL);
	CUIWidget* GetSafetBuddyWgt(int type);
	CUIWidget* GetBuddyWgt(int type);
	
	void EnableSize( BOOL cansize, BOOL effect_x=TRUE,     BOOL effect_y=TRUE );
	void EnableMove( BOOL canmove, BOOL effect_x=TRUE,     BOOL effect_y=TRUE );
	
	void EnableWidget( BOOL bEnable );
	
	void ShowWidget( BOOL isshow );
	
	void CheckWidgetEdge();

	BOOL IsVisible();
	BOOL IsRawVisible();
	BOOL IsInteractive();

	BOOL IsVisibleEx();
	BOOL IsEnabledEx();


	BOOL IsChildFocused();
	BOOL IsDescendFocused();

	BOOL IsChildHot();
	BOOL IsDescendHot();
	
	BOOL IsInDescend(CUIWidget* pWgt);

	BOOL IsVertical();
	BOOL IsPrimary();
	BOOL IsPopup();

	BOOL IsEditable();
	void SetEditable(BOOL bEdit);
	
	BOOL IsDragging();
	void EnableDragging( bool bDrag );	

	BOOL IsDiscarded();
	void SetDiscarded( bool bEdit );
	
	BOOL IsAcceptIdle();
	void AcceptIdle( bool bIdle );
	
	BOOL IsPathMovePaused();
	void PausePathMove( bool bPause );

	BOOL IsPathMoveFliped();
	void FlipPathMove( bool bFlip );

	BOOL IsMovable();
	BOOL IsMovableX();
	BOOL IsMovableY();

	BOOL IsSizable();
	BOOL IsSizableX();
	BOOL IsSizableY();

	int GetOriginX();
	int GetOriginY();

	int GetWidth();
	int GetHeight();

	void SetWidth(int width);
	void SetHeight(int height);
	void SetSize(int width,int height);
	
	int GetEndX();
	int GetEndY();

	void   SetWinRect(stRect rect){ m_WinRect=rect; }
	stRect GetRelativeWinRect();

	void SetWinRect( int ox, int oy, int width, int height );
	void SetPosition( int left, int top, int right, int bottom );
	void SetRelativePos( int left, int top, int right, int bottom, CUIWidget *pfather ); //设置的坐标是相对于父窗口的


	void   SetDragRect(stRect rect){ m_DragRect=rect; }
	void   SetDragRect( int ox, int oy, int width, int height );
	void   DefaultDragRect();
	stRect GetDragRect(){   return m_DragRect;  }

	void   SetClientRect(stRect rect){ m_ClientRect=rect; }
	stRect GetClientRect(){  return m_ClientRect;  }
	void   SetClientRect( int ox, int oy, int width, int height );
	void   DefaultClientRect();
	
	void   SetCaptionRect(stRect rect){ m_CaptionRect=rect; }
	stRect GetCaptionRect(){  return m_CaptionRect;  }
	
	void   SetCaptionRect( int ox, int oy, int width, int height );
	void   DefaultCaptionRect();

	int    GetItemDragFlag(){return m_ItemDragFlag;}
	void   SetItemDragFlag(int flag){m_ItemDragFlag=flag;}

	
	int    GetItemDropFlag(){return m_ItemDropFlag;}
	void   SetItemDropFlag(int flag){m_ItemDropFlag=flag;}

	BOOL IsInItemDragRect( int x, int y );
	BOOL IsInItemDropRect( int x, int y );		

	BOOL IsPointInWinRect( int x, int y );
	BOOL IsPointInClientRect( int x, int y );
	BOOL IsPointInCaptionRect( int x, int y );
	BOOL IsPointInDragRect( int x, int y );
	BOOL IsRawInDragRect( int x, int y );
	BOOL IsPointInDescendant(int x,int y);

	int  IsPointInAjustRect(int x,int y);
	bool IsPointInsideWinShape(int x,int y);

	int IsOutofScreen();
	void PullIntoScreen();
	stRect GetFreeRect(int width,int height);

	Point2D GetBestTooltipPos(int xcursor,int ycursor,int w,int h,stRect refRect);

	void SetCaptionOffset( Point2D offsetPt ,int index);
	Point2D GetCaptionOffset(int index);
	
    void SetPatchPic( CUIImage *pimg ,int index);
    void SetPatchPic( const char *filename ,int index);
	CUIImage* GetPatchPic(int index);

	stRect GetPatchRect(){ return m_PatchRect; }
    void SetPatchRect(int x,int y,int w,int h){ m_PatchRect=stRect(x,y,w,h); }

	void SetAdornPic( CUIImage *pimg ,int index);
    void SetAdornPic( const char *filename ,int index);
	CUIImage* GetAdornPic(int index);
	Point2D GetAdornPicPos(int index){ return m_AdornPicPos[index]; }
    void SetAdornPicPos(int x,int y,int index){ m_AdornPicPos[index]=Point2D(x,y); }

    void SetBackPic( CUIImage *pimg );
    void SetBackPic( const char *filename );
	CUIImage* GetBackPic(){ return m_pBackPic; }
	Point2D GetBackPicPos(){ return m_BackPicPos; }
    void SetBackPicPos(int x,int y); 

	void SetCoverPic( const char *filename );
	void SetCoverPic( CUIImage *pimg);
	CUIImage* GetCoverPic(){ return m_pCoverPic; }
	Point2D GetCoverPicPos(){ return m_CoverPicPos; }
    void SetCoverPicPos(int x,int y); 

	void SetEffectPic( const char *filename );
	void SetEffectPic( CUIImage *pimg);
	CUIImage* GetEffectPic(){ return m_pEffectPic; }
	Point2D GetEffectPicPos(){ return m_EffectPicPos; }
    void SetEffectPicPos(int x,int y); 

	void SetCheckCover( CUIImage *pimg );
	void SetCheckCover( const char *filename );

	void SetHotCover( CUIImage *pimg );
	void SetHotCover( const char *filename );


	int  GetSoundId(int index){ return m_SoundId[index]; }
    void SetSoundId(int id,int index){ m_SoundId[index]=id; }	


	int    GetNumChildRect(){return m_vecChildRect.size();}
	stRect GetChildRect(int index);
    void   SetChildRect(int index,stRect rcChild);
	
	void SetFrameScheme(int scheme);
	void AddFrameScheme(stFrameScheme frmScheme);


	
	BOOL IsAccelIn(const stAccelItem& accel);
	int  ProcessAccelerator(const stAccelItem& accel, WPARAM wparam, LPARAM lparam);

	void ClearPathPoint();
	void PushPathPoint(stUIPathPoint pt);
		
	CUIWidget* FindChildById(const char* pid);

	CUIWidget *FindChildByAlignId(int id);
	
	void ExecCallBackFunc(UICallbackEvent eventName,CallbackData pData=NULL);


	//调用脚本的消息回调函数
	int CallScript( const char *pfunc,const char* arg="");

	int CallSafeScript(short funcid,const char* arg ="");
	int CallSafeScriptEx(short funcid,const char *fmt="", ...);

	BOOL HasScriptFunc(short funcid);
	
	const stScriptFunc* GetScriptFunc(short funcid);
	
	void RenderCustomItem();

	void RenderCustomBuffer();

	void OnLoadRecursive();
	void OnInitRecursive();
	void OnDenitRecursive();


	
	CUIWidget*  GetNextStopSibling();
	CUIWidget*  GetNextStopChild();

	
	void SelectChild(int groupId,int groupstop);
	void SelectSelf();
	int  GetSelect(int groupId);
	CUIWidget* GetGroupChild(int groupId,int groupstop);
	
	void RenderRecursive();
	void UpdateRecursive( float deltatime );
	void ActiveRecursive(bool bActive);

	void AlignChildren();	
	void AlignRecursive();	
	void AlignSelf();


	void AjustChildren();
	void AjustRecursive();
	
	void FitBkImageSize();

		
	void SortSelf();
	void SortByAlignId();
	void SortByTabStop();
	void SortByGroupStop();
	
	void UpdateChildPreLayer();


	void ArrangeChildRect();

	//成员变量//////////////////////////////////////////////////////////////////////

private:

	std::map<int,stBuddyInfo> m_BuddyWgts;

	//和父亲的alphafactor相乘后的值，用于绘制
	double m_RenderFactor;

protected:

	CUIWidget *m_pFather; 
	std::vector<CUIWidget *>m_Children;


	WidgetType m_Type;

	std::string m_ID;
	std::string m_Caption;

	//标题显示的位置偏移
	Point2D m_CaptionOffset[FRAME_STATE_MAX];

	stRect m_TipPos;//强制tooltip窗口位置
	std::string m_TipText;
	int m_TipFlag;
	float m_IdleTime;//tooltip显示的延迟时间
	
	std::string m_ScriptText;

	std::string m_CursorName;//窗口鼠标名称
	std::string m_HorizonSizer;//调整宽度鼠标名称
	std::string m_VerticalSizer;//调整高度鼠标名称
	
	CUIImage *m_pPatchPic[PATCHID_NUMBER];
	stRect m_PatchRect;

	CUIImage *m_pAdornPic[UI_ADORNPIC_NUMBER];
	Point2D  m_AdornPicPos[UI_ADORNPIC_NUMBER];

	
	CUIImage *m_pBackPic;
	CUIImage *m_pCoverPic;	
	CUIImage *m_pEffectPic;
	CUIImage *m_pCheckCover;
	CUIImage *m_pHotCover;
	

	Point2D m_BackPicPos;//背景图片相对与窗口原点的的位置(缺省为0,0)
	Point2D m_CoverPicPos;//覆盖图片相对与窗口原点的的位置(缺省为0,0)
	Point2D m_EffectPicPos;//特效图片相对与窗口原点的的位置(缺省为0,0)
	Point2D m_CheckCoverPos;
	Point2D m_HotCoverPos;

	int m_SoundId[UI_SOUND_MAXNUM];
	
	int m_EdgeMargin[UI_SOUND_MAXNUM];
	
	//缺省绘制标志
	int m_DrawFlag;

	stColorQuad m_BackColor;
	stColorQuad m_DrawColor;
	stColorQuad m_FocusColor;
	stColorQuad m_EdgeColor;

	stColorQuad m_MagicColor;
	stColorQuad m_LinkColor;

	int m_State; 

	int m_CurFrame;  //当前图的第几桢
	
	int m_CurEffectFrame; //当前特效图的第几桢

	int m_QueryType;
	int m_QueryIndex;

	int m_DragType;
	int m_DragIndex;//默认的道具索引

	int m_HostType;
	int m_HostIndex;//默认的道具索引

	//下面两个值的和用于比较各个兄弟的绘制优先等级，越大则越后绘制，也就是越上层
	int   m_InitLayer;//初始值
	int   m_PlusLayer;//focus值
	int   m_PreLayer;//上次绘制时的顺序索引

	
	int  m_GroupId;
	int  m_GroupStop;
	
	int m_TabStop;
	int m_ChildStop;
	//add by lincoln [2006:7:18] [11:54]
	//{{{界面库效率优化
	int m_UpdateMuteFlag;
	//}}}

	int m_EdgeCheck;

	//当前哪几个边在被拖动调整
	int m_AjustState;

	//窗口哪几个边可以被拖动调整
	int m_AjustEdge;

	//窗口的边被调整时候，各个rect是否跟随调整
	int m_AjustFlag;

	//窗口调整区域的宽度
	int m_EdgeThick;


	//用于拖放的不规则边线，必须是单值函数折线
	LineSeg2D m_EdgeLines[EDGE_LINE_MAX];


	int m_MinHeight;
	int m_MaxHeight;
	
	int m_MinWidth;
	int m_MaxWidth;

	int m_MouseDelta;

	//当窗口在某个状态的时候，自动累计时间
	//当累计时间达到某个值，执行一次操作（切换frame）
	double m_AccumTime;

	//计时器ontimer的时间间隔
	//m_TimerSpace<=0表示不需要计时
	//每次updateself，m_TimerSpace减少一个deltatime
	//当m_TimerSpace从>0到<=0时候，ontimer一次
	double m_TimerSpace;

	double m_AlphaSpeed;//[0,1]
	double m_AlphaFactor;//[0,1]
	double m_AlphaMin;//[0,1]
	double m_AlphaMax;//[0,1]

	double m_ScaleFactor;
	BOOL m_bAutoScale;
	int m_AlphaState;

	double m_PathSpeed;
	
	
	int m_TextStyle;
	int m_TextFont;
	int m_TextSize;
	int m_RowSpace;
	int m_RichMode;
	int m_RichFlag;

	int m_Pattern;	
	int m_Style;	
	int m_ExtendStyle;


	int m_CustomState;
	int m_CustomStyle;

	int m_CustomId;
	int m_CustomType;
	int m_CustomIndex;

	std::string m_CustomName;
	std::string m_CustomText;
	std::string m_CustomString;

	//funcid,funcpath
	std::vector<stScriptFunc> m_ScriptFuncFlags;

	int m_TemplateId;

	
	int m_AlignType;
	int m_AlignStyle;
	int m_AlignId;
	int m_AlignToId;

	int m_AreaId;

	int m_MarginH;
	int m_MarginV;

	int m_SnapH;
	int m_SnapV;

	std::map<int,stUIVar>m_Mornitor;

	std::vector<stAccelItem> m_Accels;

	std::deque<stUIPathPoint> m_PathPoints;

	std::vector<std::string>m_vecTemplate;


	//动画帧
	std::vector<stFrameScheme>m_vecFrameScheme;
	int m_CurFrameScheme;
	int m_CurFrameState;

	double m_FrameSpeed;
	
	//扩展窗口矩形
	std::vector<stShape>m_vecWinShape;

	std::vector<stRect>m_vecWakeArea;

	//窗口是一个容器
	//m_vecChildRect存储了所有的子窗口的rect
	//当调用窗口的AutoArrange函数时,会自动根据子窗口的rectid来设置子窗口的相对位置
	std::vector<stRect>m_vecChildRect;
	int m_RectId;
		
	stRect m_WinRect;   //窗口绝对位置
	stRect m_DragRect;  //m_DragRect表示相对于窗口原点的一个鼠标拖动区域(包含在WinRect中)
	stRect m_ClientRect;//表示相对于窗口原点,窗口除去dragrect后的客户区域(包含在WinRect中)
	stRect m_CaptionRect;//表示相对于窗口原点,窗口标题区域(包含在WinRect中)

	
	//相对窗口坐标
	int m_ItemDragFlag;//道具拖动区域标志
	int m_ItemDropFlag;//道具释放区域标志

	stCustomBuffer m_CustomBuffer;
	
};



//----------------------------------------------------------------------------------------
//--------------------------------class CUIRootWin--------------------------------------
//----------------------------------------------------------------------------------------
class CUIRootWin : public CUIWidget
{
	
public:
	
	CUIRootWin();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIRootWin();


	void AddRootChild( CUIWidget *pwidget );


	virtual int OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );

	virtual void ClearParameter();

	virtual void RenderSelf();
	virtual void PostRenderSelf();
	virtual void UpdateSelf( float deltatime );
	virtual void PostUpdateSelf( float deltatime );	

	virtual void OnSaveRecursive(FILE* fp);

	void DelScreen( const char* id );
	void AddScreen( CUIWidget *pscreen );
	void AddScreenFile(const char* id,const stScreenDesc& scrnDesc);
	void GotoScreen( const char *id );	
	CUIWidget * LoadScreen(const char* id);
	
	int GetNumScreen();
	CUIWidget* GetScreen(int i);

	CUIWidget* GetCurScreen();
	void SetCurScreen(CUIWidget* pNewScreen);
	
	void ReloadScreen();
	void DebugScreen();
	
	CUICursor *AddCursor( const char *pname, const char *path );
	CUICursor *FindCursor( const char *pname );
	void SetCurCursor( const char *pname );
	CUICursor *GetCurCursor();

	const char* GetCursorInfo(int& x,int& y);


	int  OnWindowMessage( UINT msg, WPARAM wparam, LPARAM lparam );

	CUIWidget *GetPopupWgt();
	CUIWidget *GetFocusWgt();
	CUIWidget *GetMouseWgt();
	CUIWidget *GetCaptureWgt();
	CUIWidget *GetDragWgt();
	CUIWidget *GetAjustWgt();
	CUIWidget *GetHotWgt();

	void SetPopupWgt( CUIWidget *pwidget );
	void SetFocusWgt( CUIWidget *pwidget );
	void SetMouseWgt( CUIWidget *pwidget );
	void SetCaptureWgt( CUIWidget *pwidget );
	void SetDragWgt(  CUIWidget *pwidget );
	void SetAjustWgt( CUIWidget *pwidget );
	void SetHotWgt( CUIWidget *pwidget );

	void OnDestroyWidget(CUIWidget *pwidget);

	CUIWidget *FindRootWidgetByID( const std::string &id );

	int OnAccelerator(UINT msgid, WPARAM wparam, LPARAM lparam);

	void ToggleConsole();
	void AddConsoleInfo( const char *pchar );
	void ShowMessageBox(Msgbox_Cmd nCmdShow,const char* pText="",const char* pCaption="Message",int nMsgId=0,CUIWidget* pWgt=0,int nHandle=0);

	CScriptState *GetScriptEnv();
	void SetScriptEnv(CScriptState *pScript);


	void AddUIScriptFunc();


	stDragInfo GetDragInfo(){ return m_DragInfo; }
	void SetDragInfo(stDragInfo info){ m_DragInfo=info; }


	bool CancelDrag();
	bool AcceptDrag(CUIWidget* pSource,int x,int y,int style);
	bool AcceptDrop(CUIWidget* pTarget,int x,int y);
	bool AjustDragNumber(bool bAdd);

	void HidePopup(int x,int y);

	CUIWidget* FindTargetOnPoint(int x,int y);
	
	int  GetEditMode(){return m_nEditMode;}
	void SetEditMode(int mode){m_nEditMode=mode;}


#ifdef _UIEDITMODE
	
	int  OnAccelEditMode(stAccelItem accel);

#endif

private:

	float m_Life;
	float m_MouseIdleTime;//鼠标停留的时间
	
	CUIWidget		*m_pCurScreen;	
	std::vector<CUIWidget *> m_AllScreen;	

	std::map<std::string,stScreenDesc> m_AllScreenDesc;
	
		
	CUICursor *m_pCurCursor;
	std::vector<CUICursor *>m_AllCursor;

	
	CUIWidget *m_pPopupWgt;
	CUIWidget *m_pFocusWgt; 
	CUIWidget *m_pMouseWgt; 
	CUIWidget *m_pCaptureWgt; 
	CUIWidget *m_pDragWgt;
	CUIWidget *m_pAjustWgt;
	CUIWidget *m_pHotWgt;


	Point2D    m_DragOffset;
	stDragInfo m_DragInfo;//默认type=-1,当前没有拖放;若type>=0,当前在拖放
	

	CScriptState *m_pScript;


	int  m_nEditMode;


};



extern CUIRootWin* g_pRootWgt;

#include "widget.inl"

#endif 

