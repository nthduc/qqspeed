
#ifndef __WIDGET_H__
#define __WIDGET_H__

#pragma warning (disable:4786)

#include <deque>
#include <map>
#include <set>
#include <stack>

#include "vfilewrap.h"
#include "gameui.h"


//��Ҫ��ȫ�ֱ���///////////////////////////////
extern int UI_MAX_INTEGER;
extern stUICoreStyle g_UICoreStyle;
extern std::vector<std::string> g_vecScriptFuncs;
extern CallBackFunc g_vecCallbackFuncs[UI_Callback_MAXNUM];
extern std::set<IWgtData*> g_vecSecuritySet;
//��Ҫ��ȫ�ֱ���///////////////////////////////

class CUIImage;
class CUICursor;
class CUIWidget;



//���涨��(��Ϣ)
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

//���涨��(֪ͨ)
enum UINotifyType
{


//SELF NOTIFY//////////////////////////////////////////////////////////////////////////

	NTFTYPE_AJUSTTING=0,//���ڵ�����С
	NTFTYPE_DRAGGING,//�ؼ�λ�ø���


//CHILD NOTIFY//////////////////////////////////////////////////////////////////////////

	NTFTYPE_SPINARROW,
	NTFTYPE_SCROLLBAR_POS,
	NTFTYPE_SCROLLBAR_RANGE,
	NTFTYPE_SLIDERBAR,

	
	NTFTYPE_COMMAND_OK,//��ť
	NTFTYPE_COMMAND_CANCEL,//��ť
	NTFTYPE_COMMAND_CLICK,//��ť
	NTFTYPE_COMMAND_CHECK,//����

	NTFTYPE_CHILD_DRAGGING,//�ӿؼ�λ�ø���	
	NTFTYPE_CHILD_SHOWHIDE,//�Ӵ��ڿɼ��Ըı�

	//NTFTYPE_ITEM_DRAG,//����Ŀ�϶�
	//NTFTYPE_ITEM_DROP,//����Ŀ����
	
	
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
//{{{�����Ч���Ż�
enum UpdateMute
{
	UPDATE_MUTE_NONE	=0,//allways update
	UPDATE_MUTE_VISIBLE	=1,//not update when invisible
};
//}}}

//m_AlphaState�����λ��0--decrease,1-increase
//m_AlphaState�Ĵε�λ��0--normal,1-paused
enum UIAlphaState
{
	ALPHA_STATE_DECREASE	= 0,  //��Ҫ�ô��ڵ�͸���ȼ���
	ALPHA_STATE_INCREASE	= 1,  //��Ҫ�ô��ڵ�͸��������
	ALPHA_STATE_PAUSE		= 2,  //��ǰ�Ƿ���ͣ��
};

//ÿ�����ڰ���4��״̬��clickdown,mouseover,normal,disable
const int FRAME_STATE_MAX=4;
enum
{
	
	FRAME_STATE_CLICK		=0,
	FRAME_STATE_OVER		=1,
	FRAME_STATE_NORMAL		=2,
	FRAME_STATE_DISABLE		=3,
		
};

//״̬�л���ť�������ɵ�״̬
const int FRAME_SCHEME_MAX=50;
struct  stFrameScheme
{
	
	//֡��Χ
	int start[FRAME_STATE_MAX];
	int end[FRAME_STATE_MAX];

	//��Ĭ�ϵ�ֵ֡0��ʼ��
	stFrameScheme()
	{
		Clear(0);
	}

	//������ָ֡��ֵ֡
	stFrameScheme(int value)
	{
		Clear(value);
	}


	//������ָ֡��ֵ֡
	void Clear(int value)
	{
		for(int i=0;i<FRAME_STATE_MAX;i++)
		{ 
			start[i]=value;
			end[i]=value;
		}
	}


	//���������Ⱦ����֡����ʼ֡��ÿ��״̬��֡����
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

	//·����λ��
	int x;
	int y;
	
	//����·�����ִ�еĺ������߽ű�
	int   command;
	
	//�������
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

//�����ϷŵĽṹ
struct stDragInfo
{
	int type;//��Ӧ�Ĵ���(������,��������)
	int index;//�����еĵڼ�������

	int hosttype;
	int hostindex;

	int itemtype;//������Ʒ������

	int maxnum;//�϶���Ʒ�������Ŀ
	int itemnum;//�϶���Ʒ����Ŀ(-1:��ʾ�ɿͻ��˵�����������������Ŀ)
	
	stRect rect;//�Ϸſ�ʼʱ��item��ԭʼλ��,���С
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


//ƴͼ����
enum UIPatchId
{

	PATCHID_UP_LEFT		=0,
	PATCHID_UP_RIGHT,
	PATCHID_DOWN_LEFT,
	PATCHID_DOWN_RIGHT,
	
	//ע�⣺˳���ܸı�
	PATCHID_UP_MIDDLE,
	PATCHID_DOWN_MIDDLE,
	PATCHID_LEFT_MIDDLE,
	PATCHID_RIGHT_MIDDLE,
	
	//�м�հ�	
	PATCHID_MIDDLE,

	//��Ŀ
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



//�����鴰�ڵĽṹ
//tooltip,msgbox,�Ҽ�����,�������
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

//�õ����ڶ�Ӧ��table�е�һ������(����rect,����children)
extern CVar *GetWidgetVar(const std::string idpath, int pos,CVarTable* pFatherVar);
extern BOOL LoadTemplateFile(const char *szFileName,std::string& strBuffer);
extern CUIWidget *LoadUIWidget( const char *pname, CVar *pvar, CUIWidget *pfather );

class CUIWidget : public IWgtData
{
	

public:

	CUIWidget();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	virtual ~CUIWidget();

	///���麯��///////////////////////////////////////////////////////////////////////
	
	
public:
	
	virtual bool IsClickDown();
	virtual bool IsMouseOver();
	virtual bool IsEnabled();
	virtual bool IsFocused();
	virtual bool IsChecked();

	//�ͻ���Ҫ��ѯ�����޸ĵ�����
	virtual const char *GetWidgetID();//����id
	virtual const char *GetWidgetPathID();//����������(.children.)id
	virtual const char *GetWidgetPath();//����·��(����.children.)
	
	virtual stCustomBuffer& GetCustomBuffer();

	virtual int  GetQueryType(){ return m_QueryType; }
	virtual int  GetQueryIndex(){ return m_QueryIndex; }
	virtual int  GetDragType(){ return m_DragType; }
	virtual int  GetDragIndex(){ return m_DragIndex; }
	virtual int  GetHostType(){ return m_HostType; }
	virtual int  GetHostIndex(){ return m_HostIndex; }


	//�ͻ����Զ��������
	//����custom buffer�߱����Զ�����չ����
	//���ǣ����ڷǳ��ٵĴ����Զ������ݣ����������
	virtual int  GetCustomState();
	virtual int  GetCustomStyle();	
	virtual int  GetCustomId();
	virtual int  GetCustomType();
	virtual int  GetCustomIndex();
	virtual const char* GetCustomName();
	virtual const char* GetCustomText();
	virtual const char* GetCustomString();
	

	//�ͻ�����Ҫ�Ļ��Ʋ����ӿ�
	virtual stRect GetWinRect(){return m_WinRect;}//���Դ��ھ���
	virtual stRect GetAbsoluteClientRect();//���Կͻ�������	
	virtual stRect GetAbsoluteCaptionRect();//���Ա���������
	virtual stRect GetAbsoluteDragRect();//�����Ϸ�������

	virtual double GetRenderFactor();//���ڵ�ǰ͸������
	virtual double GetScaleFactor();//���ڵ�ǰ��������


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
		
	
	//�麯��//////////////////////////////////////////////////////////////////////
	

	virtual BOOL IsPointIn( int x, int y );
	virtual stColorQuad GetPointColor( int x, int y );

	//�Ƿ������������
	virtual BOOL IsAcceptIme();
	
	//�Ƿ�����ַ�����
	virtual BOOL IsAcceptInput();

	//���Ӵ�������widget, ����idpath
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

	//�����뺢��֮�������Լ��Ĳ���
	virtual void PostSetParameter( CVar *pvar, CUIWidget *pfather );
	
	//�����뺢��֮ǰ�����Լ��Ĳ���
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );

	//���캯��ֻ������m_Type��ָ���ʼֵ����ֻ���ʼ��һ�εı���
	//ClearParameter������ظ��ĳ�ʼ������
	virtual void ClearParameter();

	virtual void ConfirmScripts( CVar *pvar );
	
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );

	//���Ӻ͸���֮���֪ͨ��Ϣ,�������ڴ���ֵ.
	//���������,һ��Ҳ���ټ������ϼ�����
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


	//�������ԭ������ĳ��ʱִ������
	virtual void OnPathArrive(stUIPathPoint pt);
	virtual void OnPathCommand(stUIPathPoint pt);
	
	//��ĳ���ƶ������У�ִ�и��²���
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

	//////////////////////////////////���麯��//////////////////////////////////////

	
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

	//���ø��½ű�����
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
	CUIWidget *GetUncle(int alignId);//����alignid��������
	CUIWidget *GetBrother(int alignId);//����alignid�����ֵ�

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
	//{{{�����Ч���Ż�
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

	void SetWidgetID( const char *pstr );//����id
		
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
	void SetRelativePos( int left, int top, int right, int bottom, CUIWidget *pfather ); //���õ�����������ڸ����ڵ�


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


	//���ýű�����Ϣ�ص�����
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

	//��Ա����//////////////////////////////////////////////////////////////////////

private:

	std::map<int,stBuddyInfo> m_BuddyWgts;

	//�͸��׵�alphafactor��˺��ֵ�����ڻ���
	double m_RenderFactor;

protected:

	CUIWidget *m_pFather; 
	std::vector<CUIWidget *>m_Children;


	WidgetType m_Type;

	std::string m_ID;
	std::string m_Caption;

	//������ʾ��λ��ƫ��
	Point2D m_CaptionOffset[FRAME_STATE_MAX];

	stRect m_TipPos;//ǿ��tooltip����λ��
	std::string m_TipText;
	int m_TipFlag;
	float m_IdleTime;//tooltip��ʾ���ӳ�ʱ��
	
	std::string m_ScriptText;

	std::string m_CursorName;//�����������
	std::string m_HorizonSizer;//��������������
	std::string m_VerticalSizer;//�����߶��������
	
	CUIImage *m_pPatchPic[PATCHID_NUMBER];
	stRect m_PatchRect;

	CUIImage *m_pAdornPic[UI_ADORNPIC_NUMBER];
	Point2D  m_AdornPicPos[UI_ADORNPIC_NUMBER];

	
	CUIImage *m_pBackPic;
	CUIImage *m_pCoverPic;	
	CUIImage *m_pEffectPic;
	CUIImage *m_pCheckCover;
	CUIImage *m_pHotCover;
	

	Point2D m_BackPicPos;//����ͼƬ����봰��ԭ��ĵ�λ��(ȱʡΪ0,0)
	Point2D m_CoverPicPos;//����ͼƬ����봰��ԭ��ĵ�λ��(ȱʡΪ0,0)
	Point2D m_EffectPicPos;//��ЧͼƬ����봰��ԭ��ĵ�λ��(ȱʡΪ0,0)
	Point2D m_CheckCoverPos;
	Point2D m_HotCoverPos;

	int m_SoundId[UI_SOUND_MAXNUM];
	
	int m_EdgeMargin[UI_SOUND_MAXNUM];
	
	//ȱʡ���Ʊ�־
	int m_DrawFlag;

	stColorQuad m_BackColor;
	stColorQuad m_DrawColor;
	stColorQuad m_FocusColor;
	stColorQuad m_EdgeColor;

	stColorQuad m_MagicColor;
	stColorQuad m_LinkColor;

	int m_State; 

	int m_CurFrame;  //��ǰͼ�ĵڼ���
	
	int m_CurEffectFrame; //��ǰ��Чͼ�ĵڼ���

	int m_QueryType;
	int m_QueryIndex;

	int m_DragType;
	int m_DragIndex;//Ĭ�ϵĵ�������

	int m_HostType;
	int m_HostIndex;//Ĭ�ϵĵ�������

	//��������ֵ�ĺ����ڱȽϸ����ֵܵĻ������ȵȼ���Խ����Խ����ƣ�Ҳ����Խ�ϲ�
	int   m_InitLayer;//��ʼֵ
	int   m_PlusLayer;//focusֵ
	int   m_PreLayer;//�ϴλ���ʱ��˳������

	
	int  m_GroupId;
	int  m_GroupStop;
	
	int m_TabStop;
	int m_ChildStop;
	//add by lincoln [2006:7:18] [11:54]
	//{{{�����Ч���Ż�
	int m_UpdateMuteFlag;
	//}}}

	int m_EdgeCheck;

	//��ǰ�ļ������ڱ��϶�����
	int m_AjustState;

	//�����ļ����߿��Ա��϶�����
	int m_AjustEdge;

	//���ڵı߱�����ʱ�򣬸���rect�Ƿ�������
	int m_AjustFlag;

	//���ڵ�������Ŀ��
	int m_EdgeThick;


	//�����ϷŵĲ�������ߣ������ǵ�ֵ��������
	LineSeg2D m_EdgeLines[EDGE_LINE_MAX];


	int m_MinHeight;
	int m_MaxHeight;
	
	int m_MinWidth;
	int m_MaxWidth;

	int m_MouseDelta;

	//��������ĳ��״̬��ʱ���Զ��ۼ�ʱ��
	//���ۼ�ʱ��ﵽĳ��ֵ��ִ��һ�β������л�frame��
	double m_AccumTime;

	//��ʱ��ontimer��ʱ����
	//m_TimerSpace<=0��ʾ����Ҫ��ʱ
	//ÿ��updateself��m_TimerSpace����һ��deltatime
	//��m_TimerSpace��>0��<=0ʱ��ontimerһ��
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


	//����֡
	std::vector<stFrameScheme>m_vecFrameScheme;
	int m_CurFrameScheme;
	int m_CurFrameState;

	double m_FrameSpeed;
	
	//��չ���ھ���
	std::vector<stShape>m_vecWinShape;

	std::vector<stRect>m_vecWakeArea;

	//������һ������
	//m_vecChildRect�洢�����е��Ӵ��ڵ�rect
	//�����ô��ڵ�AutoArrange����ʱ,���Զ������Ӵ��ڵ�rectid�������Ӵ��ڵ����λ��
	std::vector<stRect>m_vecChildRect;
	int m_RectId;
		
	stRect m_WinRect;   //���ھ���λ��
	stRect m_DragRect;  //m_DragRect��ʾ����ڴ���ԭ���һ������϶�����(������WinRect��)
	stRect m_ClientRect;//��ʾ����ڴ���ԭ��,���ڳ�ȥdragrect��Ŀͻ�����(������WinRect��)
	stRect m_CaptionRect;//��ʾ����ڴ���ԭ��,���ڱ�������(������WinRect��)

	
	//��Դ�������
	int m_ItemDragFlag;//�����϶������־
	int m_ItemDropFlag;//�����ͷ������־

	stCustomBuffer m_CustomBuffer;
	
};



//----------------------------------------------------------------------------------------
//--------------------------------class CUIRootWin--------------------------------------
//----------------------------------------------------------------------------------------
class CUIRootWin : public CUIWidget
{
	
public:
	
	CUIRootWin();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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
	float m_MouseIdleTime;//���ͣ����ʱ��
	
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
	stDragInfo m_DragInfo;//Ĭ��type=-1,��ǰû���Ϸ�;��type>=0,��ǰ���Ϸ�
	

	CScriptState *m_pScript;


	int  m_nEditMode;


};



extern CUIRootWin* g_pRootWgt;

#include "widget.inl"

#endif 

