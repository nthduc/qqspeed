
#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#include "widget.h"
#include "gameui.h"

#include <queue>


enum
{
	UI_TEXTFLAG_NONE  =0,
	UI_TEXTFLAG_HLINK =1,
};

struct stUIPen
{
	DWORD dwdraw;
	DWORD dwmagic;//(const)
	DWORD dwlink;//(const)

	int  font;
	int  size;//(const)
	int  style;
	
	int  state;//(const)

	int  flag;

	stUIPen()
	{
		dwdraw  =0xffffffff;			
		dwmagic =0xffffffff;
		dwlink  =0xff0000ff;
		
		font=0;
		size=12;
		style=0;

		state=UI_ITEM_STATE_DEFAULT;

		flag=UI_TEXTFLAG_NONE;
	}	
	
};


struct stRichChar
{
	int  type;
	int  index;

	int  width;
	int  height;

	char keybuf[UI_KEYCHAR_MAX];
	
	stRichChar()
	{
		type=RICH_CHAR_NONE;
		index=-1;
		
		width=0;
		height=0;

		memset(keybuf,0,UI_KEYCHAR_MAX);
	}
	
	stRichChar(stKeyChar keyChar)
	{
		type=keyChar.type;
		index=keyChar.index;
		
		width=keyChar.width;
		height=keyChar.height;
		
		memset(keybuf,0,UI_KEYCHAR_MAX);
		strncpy(keybuf,keyChar.key,keyChar.keynum);

	}

	void GetKeyChar(stKeyChar& keyChar)
	{
		keyChar.type=type;
		keyChar.index=index;

		keyChar.width=width;
		keyChar.height=height;

		keyChar.keynum=strlen(keybuf);
		strncpy(keyChar.key,keybuf,UI_KEYCHAR_MAX);

	}
	
	
	
};



struct stRichLine
{
	//起始终止的字符索引
	int nFirst;
	int nLast;

	//行高度
	int nHeight;

	stRichLine()
	{
		nFirst=-1;
		nLast=-1;		

		nHeight=0;
	}

	stRichLine(int first)
	{
		nFirst=first;
		nLast=first;

		nHeight=0;
	}


};


class CUIRichText
{
	
public:
	
	CUIRichText();	
	~CUIRichText();
	
	
	void ParseTxt(const char* pBuf,int richMode,int richflag);

	//add+jension 2006-2-14 9:46
	//{{{聊天频道用户超链接屏蔽
	bool RemoveCtrlChar(int flag);
	//}}}

	void GetRawString(std::string& strBuf);
	
	int  GetSize();

	int  GetTextExtent(int start,int length,int textSize);
	int  EstimateLine( int& width,int& height,int size,int space,bool bCalcLine=false);
	
	void DrawRichText( stRect rect,int size,int space,DWORD dwColor,DWORD dwMagic,DWORD dwLink,int style,int font,int state=UI_ITEM_STATE_FOCUSED);
	int  ParseKeyChar(stKeyChar& keyChar, int px,int py,int sx,  int sy, int width,int height,int size,int space,int style);

	//add+jension 2006-02-20 11:20
	//{{{聊天窗口行首标题显示错误
	int GetRichLineHeight(int i);
	//}}}


private:
		
	bool IsHalfDBCSPre(int pos);
	
	std::vector<stRichChar> m_RichChars;

	//存储显示布局信息(EstimateLine的时候，计算m_RichLines)
	std::vector<stRichLine>m_RichLines;	

	
};



class CUIButton : public CUIWidget
{

public:

	CUIButton();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIButton();

	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void RenderSelf();
	virtual void UpdateSelf( float deltatime );

	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void DefaultFrameScheme();
	virtual void OnCommand();
	
	void SetCommandId(UINotifyType id);
	UINotifyType GetCommandId();


	void SetMsgStyle(BOOL bPopMsg);
	BOOL GetMsgStyle();

protected:

	UINotifyType m_CommandId;//push button后发送给父亲的命令


private:

	double m_AccelTimer;
	double m_AccelThreshold;

	
};

class CUISpin : public CUIButton
{
public:
	CUISpin();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUISpin();
	

	virtual void OnCommand();
	virtual void ClearParameter();

	
};


//radio和check公工的类
class CUICheck : public CUIButton
{
public:

	CUICheck();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUICheck();
	
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void OnSchemeFrame();
	virtual void DefaultFrameScheme();
	virtual void OnCommand();
	virtual void ClearParameter();

};

//TABWIN是一个普通窗口,但是它的dragrect必须特殊指定,dragrect可以在winrect之外
//当check时候,孩子可见,否则,不可见
class CUITabWin : public CUIWidget
{

	
public:
	CUITabWin();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUITabWin();
	
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );	
	
	virtual void RenderSelf();	
	virtual void RenderChild();
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void SetChecked( bool bCheck );

	virtual void DrawBarEdge();
	virtual void DrawDebugBarEdge();

	void SetTabCover( CUIImage *pimg );
	void SetTabCover( const char *filename );

	void SetTabWinType(int type);
	int  GetTabWinType();


	virtual BOOL IsPointIn( int x, int y );

	BOOL IsPointInTabRect( int x, int y );

	
	void	SetTabRect( int ox, int oy, int width, int height );
	void	DefaultTabRect();

	stRect  GetTabRect(){return m_TabRect;}
	stRect  GetAbsTabRect();
	stRect  GetHotWinRect();

private:

	CUIImage *m_pTabCover;//绘制在拖放区dragrect的图片

	stRect m_TabRect;

	int m_TabWinType;//dragrect相对于窗口的偏移方式

};

class CUILabel : public CUIWidget
{

	
public:

	CUILabel();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUILabel();

	virtual void AjustSelf();

	virtual void SetCaption( const char *pname );
	
	virtual void RenderSelf();
	virtual void UpdateSelf( float deltatime );
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();
	virtual int  ParseKeyChar(int x,int y,stKeyChar& keyChar);

private:
	

	CUIRichText m_RichCaption;

	int m_FlashTimer;
	int m_FlashFlag;//0--隐藏, 1--显示
	int m_FlashLive;//显示时间
	int m_FlashDeath;//隐藏时间

};




class CUILineEdit : public CUIWidget
{

	
public:

	CUILineEdit();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUILineEdit();

	virtual void ClearParameter();

	int  GetEditMethod();
	void SetEditMethod( int method );
	void Clear();

	//add by lincoln [2007:4:1] [13:45]
	//{{{密码控件改进
	void Encrypt();
	void Decrypt();
	//}}}
	
	void  SetStep(double step);
	
	virtual void AddChild( CUIWidget *pwidget );
	virtual void DelChild( CUIWidget *pwidget );
	
	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);

	virtual void DrawCaption();
	virtual void SetCaption( const char *pname );
	
	//是否接受中文输入
	virtual BOOL IsAcceptIme();
	
	//是否接受字符输入
	virtual BOOL IsAcceptInput();

	void SetSeperator(const char* pChar);
	void SetRange(double min,double max);

	//校正值的范围
	bool CheckValueRange();
	void ValueToCaption();
	void CaptionToValue();

	virtual void RenderSelf();
	virtual void UpdateSelf( float deltatime );
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void OnInit();

	void SelectAllText();
	int  SetSel(int start,int end);
	int  ClearSel();
	int  CancelSel(bool bLeft,bool bMove=true);
	
	virtual void OnPaste();
	virtual void OnCopy();
	virtual void OnCut();
	virtual void OnInsert(const char* pText);

	virtual int OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual	int OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );

	void UpdateInputFlag();
	bool QueryInputFlag(char key);

	void CheckForReason();
	
	BOOL IsCursorVisible();

	
protected:

		
	void AddStringToHistory( const char* text );

	void SetEditSelColor(stColorQuad color){ m_EditSelColor=color; }
	void SetGrayColor(stColorQuad color){ m_GrayColor=color; }

	int m_EditMethod;
	int m_MaxChar;//必须保证maxchar总是偶数

	int m_CursorPos;//当前光标位置的字符索引
	int m_DispBegin;//显示开始位置的字符索引


	int m_SelBegin;

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//显示时间
	int m_CursorDeath;//隐藏时间

	std::string m_History[UI_EDIT_HISTORY];
	int m_NumHistory;
	int m_CurHistory;

	double m_Step;
	double m_Value;

	double m_MaxValue;
	double m_MinValue;

	//当为数值输入,且值在此范围中,则隐藏
	double m_Upper;
	double m_Lower;


	enum
	{
		INPUT_FLAG_NONE=0,
		INPUT_FLAG_CHAR=1,
		INPUT_FLAG_WORD=2,
	};

	int m_InputFlag;//0--可以输入汉字,1--不能输入汉字,2--不能输入
	
	//选中状态条颜色
	stColorQuad m_EditSelColor;

	//disable状态绘制颜色
	stColorQuad m_GrayColor;
	

	std::string m_Seperator;
	

	//add by lincoln [2007:4:1] [13:45]
	//{{{密码控件改进
	unsigned char m_EncryptCaption[UI_DEFAULT_BUFSIZE];
	int m_nSizeEncrypt;
	//bool m_bEncrypted;
	//}}}

};




//支持图标嵌入的单行编辑框
//m_Caption中存储了原始的字符串
//而m_RichCaption中存储了高级结构的字符串
class CUIRichEdit : public CUIWidget
{
	
	
	
	
public:
	
	CUIRichEdit();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIRichEdit();

	virtual void UpdateSelf( float deltatime );
	virtual void RenderSelf();
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual	int  OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	
	virtual void GetVar(stUIVar& uiVar);	
	virtual void SetVar(stUIVar& uiVar);	

	//是否接受中文输入
	virtual BOOL IsAcceptIme();
	
	//是否接受字符输入
	virtual BOOL IsAcceptInput();

	virtual void DrawCaption();
	virtual void   SetCaption( const char *pname );
	virtual const char *GetCaption();

	virtual int  ParseKeyChar(int x,int y,stKeyChar& keyChar);
	
	void MergeRichChar(int start=0);

	void GetRawString(std::string& strBuf,int start,int length);

	int GetTextExtent(int start,int length);


	int PosToChar(int pos);

	void AjustForReason();

	bool InputRawChar(unsigned char rawChar);

	bool InsertRichChar(int pos,stRichChar rChar);
	int  EraseRichChar(int pos);

	int ParseMaxSize();

	void MoveCursor(int pos);
	void MoveSelBegin(int pos);
	void MoveDispBegin(int pos);

	bool IsHalfDBCSPre(int pos);


	void SelectAllText();
	int  SetSel(int start,int end);
	int  ClearSel();
	int  CancelSel(bool bLeft,bool bMove=true);

	void Clear();
	
	virtual void OnPaste();
	virtual void OnCopy();
	virtual void OnCut();
	virtual void OnInsert(const char* pText);
	

	void AddStringToHistory( const char* text );
	

	void SetEditSelColor(stColorQuad color){ m_EditSelColor=color; }
	void SetGrayColor(stColorQuad color){ m_GrayColor=color; }

	void CheckForReason();

	void DrawAdvertise();

	BOOL IsCursorVisible();
	
private:

	//存储操作和显示字符串的数组
	std::vector<stRichChar>m_RichCaption;
	
	//指richchar的个数
	int m_MaxChar;
	int m_DispBegin;

	int m_SelBegin;

	//rich char的光标位置
	int m_CursorPos;

	//选中状态条颜色
	stColorQuad m_EditSelColor;

	//disable状态绘制颜色
	stColorQuad m_GrayColor;

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//显示时间
	int m_CursorDeath;//隐藏时间

	std::string m_History[UI_EDIT_HISTORY];
	int m_NumHistory;
	int m_CurHistory;
	
	//广告文本（当窗口没有focus的时候，绘制该行文本）
	std::string m_Advertise;
	
	
};

struct stEditTick
{
	int nPos;
	int nRow;

	stEditTick()
	{
		nPos=0;
		nRow=0;
	}

	stEditTick(int pos,int row)
	{
		nPos=pos;
		nRow=row;
	}
};


class CUIMultiEdit : public CUIWidget
{
	
	enum
	{
		UI_MULTIEDIT_MAXCOL=1000000,	
	};

	
	
public:	

	

	CUIMultiEdit();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIMultiEdit();

	virtual void AddChild( CUIWidget *pwidget );
	virtual void DelChild( CUIWidget *pwidget );

	virtual void UpdateSelf( float deltatime );
	virtual void RenderSelf();
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();
	
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual	int  OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	
	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);

	//是否接受中文输入
	virtual BOOL IsAcceptIme();
	
	//是否接受字符输入
	virtual BOOL IsAcceptInput();


	virtual void DrawCaption();
	virtual void   SetCaption( const char *pname );
	virtual const char *GetCaption();

	virtual int  ParseKeyChar(int x,int y,stKeyChar& keyChar);

	void OffsetContent(int offset);
	void ScrollContent(int pos);
	void UpdateScrollBar();

	void ReparseText();
	void CalcCharsLine();

	Point2D PosToGrid(int index);
	stEditTick GridToPos(int row,int col);
	
	int  HeightToRow(int height);
	int  WidthToChar(int row,int width);

	int  GridToWidth(int row,int col);
	
	Point2D GetCursorGrid();

	void  EnsureCursorVisible();

	void MoveCursorUp();
	void MoveCursorDown();

	void MoveCursorLeft();
	void MoveCursorRight();

	void MoveCursorLineHome();
	void MoveCursorLineEnd();

	void MoveCursorHome();
	void MoveCursorEnd();

	void MoveCursorOnPoint(Point2D pt);

	
	void MergeRichChar(int start=0);

	void GetRawString(std::string& strBuf,int start,int length);

	int GetTextExtent(int start,int length);

	bool InputRawChar(unsigned char rawChar);

	bool InsertRichChar(int pos,stRichChar rChar);
	int  EraseRichChar(int pos);

	int ParseMaxSize();


	void MoveCursor(int pos);
	void MoveSelBegin(int pos);
	void MoveLinePos(int pos);
	
	void ExtendSelBackward(int step);
	void ExtendSelForward(int step);

	bool IsHalfDBCSPre(int pos);


	void SelectAllText();
	int  SetSel(int start,int end);
	int  ClearSel();
	int  CancelSel(bool bLeft,bool bMove=true);

	void Clear();
	
	virtual void OnPaste();
	virtual void OnCopy();
	virtual void OnCut();
	virtual void OnInsert(const char* pText);

	void OnSetFont(int font);
	void OnSetColor(int color);	

	int GetColorPos(int pos);
	int GetFontPos(int pos);


	int GetColorPos();
	int GetFontPos();

	void SetEditSelColor(stColorQuad color){ m_EditSelColor=color; }
	void SetGrayColor(stColorQuad color){ m_GrayColor=color; }

	int GetReturnFlag(){return m_ReturnFlag;}

	void CheckForReason();

	BOOL IsCursorVisible();

private:


	//存储操作和显示字符串的数组
	std::vector<stRichChar>m_RichCaption;

	
	//存储显示布局信息
	std::vector<stRichLine>m_RichLines;	


	//指richchar的个数
	int m_MaxChar;
	
	//当前光标所在行
	int m_LinePos;
	
	int m_SelBegin;

	//rich char的光标位置
	int m_CursorPos;

	//enter key flag
	int m_ReturnFlag;

	//选中状态条颜色
	stColorQuad m_EditSelColor;

	//disable状态绘制颜色
	stColorQuad m_GrayColor;

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//显示时间
	int m_CursorDeath;//隐藏时间


	int m_ContentHeight;
	int m_ContentPos;
	int m_CurPage;

	
};




//可以绘制线条
class CUIShaper: public CUIWidget
{

public:

	

	struct stUIShape
	{
		
	  int sx;
	  int sy;
	  int ex;
	  int ey;
	  int color;

	  stUIShape()
	  {
		  sx=0;
		  sy=0;
		  ex=0;
		  ey=0;
		  color=0;
	  }
	  

	  
	  stUIShape(int x0,int y0,int x1,int y1,int c)
	  {
		  sx=x0;
		  sy=y0;
		  ex=x1;
		  ey=y1;
		  color=c;
	  }
	  
	};


	CUIShaper();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIShaper();

	virtual void  RenderSelf();
	virtual void  SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();
	
	void RenderLines();
	void AddLine(int x0,int y0,int x1,int y1,int color);

	void SetShapeMode(int mode);
	

protected:

	//相对于窗口原点的坐标
	std::vector<stUIShape> m_Lines;

	int m_ShapeMode;

};



//将menu扩展为一种新窗口
//窗口自动根据可见孩子调整自己的客户区大小
//客户区由所有的子窗口占据区域决定
//排列子窗口时候,左上角起始位置就是客户区起点
//排列完之后,客户区终点为所有孩子所扩展的最大位置
//客户区只和可见窗口相关.
//窗口的顺序是动态的,每个窗口的tabstop表示该窗的顺序位置
class CUIMenu : public CUIWidget
{
	
public:

	CUIMenu();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIMenu();

	virtual void AjustSelf();

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void PostUpdateSelf( float deltatime );	
	virtual void ClearParameter();
};


class CUIConsole : public CUIWidget
{

public:

	CUIConsole();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIConsole();

	virtual void  RenderSelf();
	virtual void ClearParameter();	
	virtual void  SetVar(stUIVar& uiVar);

	void SetCmdMode( int i ){ m_CmdMode = i; }

	void EnterString( const char *pstr );
	void PushString( const char *pstr );
	void ExecString( const char *pstr );

protected:	
	
	enum
	{ 
		EDIT_H = 16,//编辑行高度
		LINE_H = 16,//每行高度
		NUM_LINES = 20,//最大行数
		LINE_LEN = 256//每行最大长度
	};
	char m_AllInfo[NUM_LINES][LINE_LEN];//保存所有行的字符创缓冲

	int  m_NumInfo;//当前行数

	enum
	{
		CONSOLE_MODE_TALK		=0,
		CONSOLE_MODE_SCRIPT		=1,	
	};
	int  m_CmdMode; //0--对话模式（输入的是对话）， 1--脚本命令模式
	
};



class CUIScreen : public CUIWidget
{

public:

	CUIScreen();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIScreen();

	virtual void OnInit();
	virtual void OnDenit();
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ConfirmScripts( CVar *pvar );
	
	
};

class CUIDialog : public CUIWidget
{

public:

	CUIDialog();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIDialog();

	virtual void ClearParameter();
	
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void RenderSelf();


};


class CUISystemDialog : public CUIDialog
{

public:
	
	CUISystemDialog();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUISystemDialog();

	virtual int OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	
};



class CUIMessageBox: public CUIDialog
{


public:


	CUIMessageBox();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIMessageBox();

	virtual void ClearParameter();

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);	

	void ShowMsgBox(Msgbox_Cmd nCmdShow,const char* pInfo,const char* pCaption="Message",int nMsgId=UI_MSGID_DEFAULT,CUIWidget* pWgt=0,int nHandle=0);

	virtual void RenderSelf();

	virtual void OnCmdOk();
	virtual void OnCmdCancel();
	
	void SetCallHandle(int handle){ m_CallHandle=handle; }
	int  GetCallHandle(){ return m_CallHandle; }

	
	void SetMsgId(int id){ m_MsgId=id; }
	int  GetMsgId(){ return m_MsgId; }

	
	void SetMsgInfo(const char* info){ m_MsgInfo=info; }
	const char *GetMsgInfo(){ return m_MsgInfo.c_str(); }


private:

	int m_MsgId;
	std::string m_MsgInfo;

	int m_CallHandle;

};


//包含一个fontpic图片,该图片有0-9个数字图片帧
//piclabel中只能显示数字字符
//用于显示特殊字体的数字
//待改进：例如%,/等特殊字符的绘制
class CUIPicLabel: public CUIButton
{
	
public:

	CUIPicLabel();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIPicLabel();


	virtual void AjustSelf();

	virtual void SetCaption( const char *pname );

	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void DrawCaption();

	
	void SetFontPic( CUIImage *psurface );
	void SetFontPic( const char *filename );

private:

	CUIImage* m_pFontPic;

};




#define CHANGEBAR_MAX_STRING 32

class CUIChangeBar:public CUIButton
{


public:

	CUIChangeBar();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIChangeBar();

	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );

	virtual void DefaultFrameScheme();
	virtual void OnSchemeFrame();

	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);	
	
	void SetBarString(int nIdx,const char* pstr);
	const char* GetBarString(int nIdx);

	void SetBarState(int state);
	void NextState(BOOL forward=true);

	void SetNumState(int num);
	int  GetNumState();

private:


	//add by lincoln [2006:2:21] [10:45]
	//{{{changebar改进
	//int m_NumState;
	//}}}

	int m_Value;
	std::vector<std::string> m_StateString;



};

//当checked的时候，可以输入键值
class CUIKeyEdit:public CUIButton
{

public:

	CUIKeyEdit();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIKeyEdit();

	//是否接受中文输入
	virtual BOOL IsAcceptIme();
	
	//是否接受字符输入
	virtual BOOL IsAcceptInput();

	virtual void DefaultFrameScheme();
	virtual void OnSchemeFrame();
	virtual void UpdateSelf( float deltatime );
		
	virtual void DrawCaption();
	
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual	int OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual=false);

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);

	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );	
	virtual void ClearParameter();

	BOOL IsCursorVisible();

private:
	
	BOOL OnMapKey(unsigned int key);

	std::map<int,std::string>m_KeyMap;

	int m_Value;	

	int m_CursorTimer;
	int m_CursorFlag;//0--隐藏, 1--显示
	int m_CursorLive;//显示时间
	int m_CursorDeath;//隐藏时间
	
};

class CUIProgressBar:public CUIWidget
{
	
public:

	CUIProgressBar();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIProgressBar();

	virtual void RenderSelf();
	virtual void UpdateSelf( float deltatime );

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void OnPlusFrame();	
	virtual void OnAffirmFrame();	
	
	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);
		
	void SetRange( int nLower,  int nUpper );
	void GetRange( int& nLower, int& nUpper );
	
	int  GetPos( );
	void SetPos( int nPos );	
	void OffsetPos( int nPos );
	
	void SetStep( int nStep );

	double GetDelayTime(){ return m_DelayTime; }
	void SetDelayTime(double value){ m_DelayTime=value;}

	double GetSlowTime(){ return m_SlowTime; }
	void SetSlowTime(double value){ m_SlowTime=value;}

	void SetProgressAimer( CUIImage *pimg );
	void SetProgressAimer( const char *filename );
		
	void SetProgressCover( CUIImage *pimg );
	void SetProgressCover( const char *filename );

	void SetProgressLayer( CUIImage *pimg );
	void SetProgressLayer( const char *filename );

	stRect GetProgressRect(int pos);

private:

	CUIImage *m_pProgressCover;
	CUIImage *m_pProgressLayer;
	CUIImage *m_pProgressAimer;

	int m_CurAimerFrame;

	int m_nPos;
	int m_nLower;
	int m_nUpper;
	int m_nStep;
	
	//当前显示位置
	//显示位置靠近实际位置,直到相等
	int m_nVisPos;
	
	double m_DelayTime;
	double m_SlowTime;

	double m_CurSlowTimer;	
	double m_CurDelayTimer;
	


};




//CUIScrollBar只能有一个button，两个spin孩子
//因为他将找到的第一个button认为是blcokbtn
//将所有spin消息作为up－down spin发送的消息
class CUIScrollBar:public CUIWidget
{
	
	

public:

	CUIScrollBar();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIScrollBar();

	virtual void AddChild( CUIWidget *pwidget );
	virtual void DelChild( CUIWidget *pwidget );

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);
	
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );	
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual int  ChildQueryMoveTo(int &x,int &y,CUIWidget* pChild);
	virtual void UpdateSelf( float deltatime );
	virtual void RenderSelf();

	virtual void OnAlignDirty();

	void SetPageSize( int nPageSize );

	void SetRange( int nLower,  int nUpper );
	void GetRange( int& nLower, int& nUpper );
	
	int  GetScrollPos( );
	int  SetScrollPos(int nPos, bool bNotify=false);	

	
	void OnBlockBtnMove();
	void SetBlockBtnPos();
	void UpdateBlockBtnPos();
	void UpdateBlockBtnSize();
	
	void HitAction(int x,int y);

	int ScreenToLogic(int cursorPos,int scrollRange);
	int LogicToScreen(int tickPos,int scrollRange);

	void SetMinBtnSize(int size){m_MinBtnSize=size;}
	void SetMaxBtnSize(int size){m_MaxBtnSize=size;}

	int GetMinBtnSize(){return m_MinBtnSize;}
	int GetMaxBtnSize(){return m_MaxBtnSize;}

	void SetMaxValue(int value);
	void SetMinValue(int value);

	BOOL IsScrollLocked();
	
private:

	void DrawSlotBar();
	int  OffScrollPos(int nPos);

	int m_nPos;
	int m_nLower;
	int m_nUpper;
	int m_nPageSize;
	
	int m_MinBtnSize;
	int m_MaxBtnSize;

	double m_AccelTimer;
	double m_AccelThreshold;

	Point2D m_ClickDownPos;

};


//当切换到groupid时,将所有groupid相等的孩子显示,其他孩子隐藏.
class CUISwitcher:public CUIWidget
{

public:

	CUISwitcher();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUISwitcher();
	
	virtual void OnInit();
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();
	
	virtual void    SetVar(stUIVar& uiVar);
	
	void SwitchTo(int grpid);

private:

	int m_CurGroupId;


};


class CUITabList:public CUIWidget
{


public:

	CUITabList();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUITabList();

	virtual void UpdateSelf( float deltatime );
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );

	void ArrangeContent();

};

//有一个内容区(content rect),是所有孩子所占据的区域
//如果一个孩子完全超出客户区,就不可见.否则可见
class CUIContainer:public CUIWidget
{

public:

	CUIContainer();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIContainer();

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual void PostUpdateSelf( float deltatime );
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void AjustSelf();

	virtual void ArrangeContent();

	void EnsureItemVisible(int index);

	void UpdateScrollBar();
	
	void  ScrollContentH(int pos);
	void  ScrollContentV(int pos);

	void  OffsetContentH(int offset);
	void  OffsetContentV(int offset);

	
	void GotoPageH(int nPage);
	void GotoPageV(int nPage);

	int  GetCurPageH();
	int  GetCurPageV();

	int  GetNumPageH();
	int  GetNumPageV();

	
	void SetItemMarginV(int margin){ m_ItemMarginV=margin; };
	int  GetItemMarginV(){	return m_ItemMarginV; };
	
	void SetItemMarginH(int margin){ m_ItemMarginH=margin; };
	int  GetItemMarginH(){	return m_ItemMarginH; };
	

protected:

	int m_ItemMarginH;
	int m_ItemMarginV;

	Point2D m_NumPage;
	Point2D m_CurPage;
	
	stRect m_ContentRect;


};


struct stUILineData
{

	int  intData;
	char strData[UI_LINEDATA_LEN];

	//缺省构造函数	
	stUILineData()
	{
		intData=-1;
		memset(strData,0,UI_LINEDATA_LEN);
	}


	//赋值构造函数
	stUILineData(int iData,const char* sData)
	{ 
		intData=iData;
		memcpy(strData,sData,UI_LINEDATA_LEN);
	}

	//赋值运算符号重载
	stUILineData& operator =( const stUILineData& data)
	{ 
		intData=data.intData;
		memcpy(strData,data.strData,UI_LINEDATA_LEN);
		
		return *this; 
	}


};

struct stTextLine
{
	CUIRichText richTxt;
	int color;	
	int oy;
	int height;

	//某种自定义数据，用于表示某行的某种属性
	stUILineData linedata;

	stTextLine(const char* pText,int iColor,stUILineData data,int mode,int flag)
	{
		richTxt.ParseTxt(pText,mode,flag);
		color=iColor;		
		linedata=data;
	}

	stTextLine()
	{
		color=0;		
		oy=0;
		height=0;
	}
	
};


//一种堆桟一样的单行文字列表
class CUITextList:public CUIWidget
{
	

public:
	
	
	CUITextList();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUITextList();

	virtual void AddChild( CUIWidget *pwidget );
	virtual void DelChild( CUIWidget *pwidget );

	virtual int OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	
	virtual void UpdateSelf( float deltatime );
	virtual void RenderSelf();
	
	
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);

	virtual int	 GetTable(int channel,lua_State *ls);
	virtual int	 SetTable(int channel,lua_State *ls);


	virtual int ParseKeyChar(int x,int y,stKeyChar& keyChar);

	const char* GetLineText(int nIndex);
	const stUILineData*  GetLineData(int nIndex);

	void ClearLines();

	//向最底部压入一行
	void PushLine(const char* pText,unsigned int color=0,stUILineData* pdata=NULL);
	
	int FindLineOnPoint(int x,int y);

	void ArrangeContent();
	void OffsetContent(int offset);
	void ScrollContent(int pos);
	void UpdateScrollBar();

	void ScrollToBegin();
	void ScrollToEnd();

	void GotoPage(int nPage);

private:
	
	std::deque<stTextLine>m_AllLines;

	stRect m_TitleRect;

	int m_NumPage;
	int m_CurPage;
	int m_CurScrollPos;
	int m_ContentHeight;

	//最大容纳行数
	int m_MaxLine;

	//行之间的间距
	int m_LineMargin;

	//滚动速度
	int m_ScrollSpeed;

	//延迟时间
	double m_LockTime;

	//计时器
	double m_ScrollTimer;
	
};




//滚动处理和翻页处理类似listbox
//只要widget items完全超出客户区,就不可见
//孩子按照groupstop排列,groupstop必须在范围内[0,m_Children.siez()-1],
//如果有滚动条孩子,将其当作普通孩子看待(可以通过buddy设置滚动条)
class CUIWidgetList:public CUIContainer
{
	
	

public:

	CUIWidgetList();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIWidgetList();

	virtual void ClearParameter();

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);
	
	virtual void ArrangeContent();

	int  GetVisChildNum();
	int  SetVisChildNum(int num);

};



class CTreeNode : public CRefObject
{

public:

	//唯一标识符	
	int m_Id;

	//绝对位置
	int m_X;
	int m_Y;
		
	int m_Width;
	int m_Height;

	//子节点是否折叠
	bool m_bCollapsed;

	//节点布局偏移参数
	int m_OffsetX;
	int m_OffsetY;

	//节点尺寸参数
	int m_SizeX;
	int m_SizeY;
	
	stRect m_IconRect;//节点左侧的折叠图标相对于节点的位置和尺寸
	
	//子节点列表
	std::vector<CTreeNode*> m_vecNodes;

	CTreeNode* m_pMother;

public:

	CTreeNode();
	~CTreeNode();

	virtual IWgtData* GetHostWgt();

	void AddNode(int id,int offx,int offy,int sizex,int sizey,stRect iconRect);
	void ClearNodes();

	
	void UpdateNode();

	CTreeNode* GetNodeIconOnPoint(int x,int y);
	CTreeNode* GetNodeOnPoint(int x,int y);
	CTreeNode* GetPrevNode(int id,bool bInCollapse);
	CTreeNode* GetNextNode(int id,bool bInCollapse);
	
	bool ReQueryNode();
	bool ExtendNode();	
	bool CollapseNode();
	bool ToggleNode();
	
    stNodePath GetNodePath();

  		
	CTreeNode* GetNodeById(int id,bool bInCollapse);
	CTreeNode* GetNodeByPath(stNodePath path);
	
	bool ExtendNodeByPath(stNodePath path);
			
	bool IsRootNode();
	
	bool IsPointIn(int x,int y);
	bool IsPointInSelf(int x,int y);
	bool IsPointInIconRect(int x,int y);
	


};

class CTreeRoot: public CTreeNode
{

private:

	IWgtData* m_pHostWgt;

public:

	CTreeRoot(){m_pHostWgt=NULL;}
	~CTreeRoot(){m_pHostWgt=NULL;}

	virtual IWgtData* GetHostWgt(){ return m_pHostWgt; }

	void SetHostWgt(IWgtData* pWgt){m_pHostWgt=pWgt;}

};

class CUITreeCtrl:public CUIWidget
{
	
public:

	CUITreeCtrl();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUITreeCtrl();

	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );

	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void UpdateSelf( float deltatime );
	virtual void RenderSelf();

	virtual void AddChild( CUIWidget *pwidget );
	virtual void DelChild( CUIWidget *pwidget );

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);

	virtual void OnInit();

	void GotoPageH(int nPage);
	void GotoPageV(int nPage);

	void  ScrollContentH(int pos);
	void  OffsetContentH(int offset);

	void  ScrollContentV(int pos);
	void  OffsetContentV(int offset);

	void ArrangeContent();
	void UpdateScrollBar();

	void ComfortSelf();


	//拖放处理
	virtual bool ItemDrop(int x,int y);
	virtual int  ParseDragIndex(int x,int y,int style);
	virtual stDragInfo ParseDragInfo(int x,int y);

private:
	
	void RenderNodes(CTreeNode* pNode);
	void DrawNode(CTreeNode* pNode);
	void DrawGrid(CTreeNode* pNode);
	BOOL EnsureVisible(CTreeNode* pNode,int& offx,int& offy,bool bSelf=TRUE);
	
	BOOL EnsureExtended(stNodePath path);

	void OnClickNode(CTreeNode* pNode);
	void OnDBClickNode(CTreeNode* pNode);


	void OnNodeExtend();
	void OnNodeCollapse();
	
	bool m_bSelDirty;
	int m_FocusId;
	stNodePath m_SelectPath;

	stNodePath m_HistoryPath;

	CTreeRoot m_RootNode;

	
	Point2D m_NumPage;
	Point2D m_CurPage;
	Point2D m_CurScrollPos;

	stRect m_ContentRect;


};



typedef struct
{
	int           ItemState;		
	int           ItemData;//value
	int			  ItemRow;
	int			  ItemCol;
	int			  ItemIcon;
	stRect        ItemRect;//相对于客户区原点的坐标，每次绘制时的转换
	stColorQuad   ItemColor;
	std::string   ItemText;
	std::string   ItemTip;
	CUIRichText   richText;
	
} stListItem;

#define LISTBOX_MAX_ITEMS 1024

//只是提供一组item,每个item由(位置,文字,背景图片,覆盖图片组成)
//可以从代码和脚本中指定这些item的位置大小,文字,图片等属性
//listbox,listctrl,treeview都可以通过继承该类而实现
class CUIListBase : public CUIWidget
{
	
public:
		
	CUIListBase();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIListBase();
	
	virtual void AddChild( CUIWidget *pwidget );
	virtual void DelChild( CUIWidget *pwidget );
	
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void RenderSelf();
	virtual void UpdateSelf( float deltatime );
	virtual void PostUpdateSelf( float deltatime );
	virtual void AjustSelf();

	virtual void OnInit();

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);	

	virtual int		GetTable(int channel,lua_State *ls);

	virtual const char* GetTipText(int nIndex);
	virtual void SetTipText( const char *pTip, int nIndex=-1);

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual int  OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual=false);

	
	//拖放处理
	virtual bool ItemDrop(int x,int y);
	virtual int  DrawDragItem(stDrawItem data);
	virtual int  ParseDragIndex(int x,int y,int style);
	virtual stDragInfo ParseDragInfo(int x,int y);

	virtual void ArrangeContent();
	virtual	void UpdateScrollBar();

	virtual int  ParseKeyChar(int x,int y,stKeyChar& keyChar);

	void SetItemMarginV(int margin){ m_ItemMarginV=margin; };
	int  GetItemMarginV(){	return m_ItemMarginV; };

	void SetItemMarginH(int margin){ m_ItemMarginH=margin; };
	int  GetItemMarginH(){	return m_ItemMarginH; };

	void SetItemHeight(int height){ m_ItemHeight=height; }
	int  GetItemHeight(){ return m_ItemHeight; }

	void SetItemWidth(int width){ m_ItemWidth=width; }
	int  GetItemWidth(){ return m_ItemWidth; }

	void SetItemTextStyle(int style){ m_ItemTextStyle=style; }	
	int  GetItemTextStyle()	{ return m_ItemTextStyle;	}
	

	int  GetRows(){return m_nRows;}
	int  GetCols(){return m_nCols;}


	void SetItemDrawColor(stColorQuad color) { m_ItemDrawColor	=color; };
	void SetItemBackColor(stColorQuad color) { m_ItemBkColor	=color; };
	void SetItemEdgeColor(stColorQuad color) { m_ItemEdgeColor	=color; };

	void SetItemFocusColor(stColorQuad color){ m_ItemFocusColor	=color; };
	void SetItemFocusBkColor(stColorQuad color){ m_ItemFocusBkColor	=color; };
	void SetItemFocusEdgeColor(stColorQuad color){ m_ItemFocusEdgeColor	=color; };

	void SetItemSelColor(stColorQuad color){ m_ItemSelColor	=color; };
	void SetItemSelBkColor(stColorQuad color){ m_ItemSelBkColor	=color; };
	void SetItemSelEdgeColor(stColorQuad color){ m_ItemSelEdgeColor	=color; };
	

	stColorQuad GetItemColor(int nIndex);
	void SetItemColor(int nIndex,stColorQuad color);

	void GotoPageH(int nPage);
	void GotoPageV(int nPage);

	int  GetCurPageH();
	int  GetCurPageV();
	
	int  GetNumPageH();
	int  GetNumPageV();

	void  ScrollContentH(int pos);
	void  OffsetContentH(int offset);

	void  ScrollContentV(int pos);
	void  OffsetContentV(int offset);
	

	int  GetItemCount();
	void SetItemCount(int count);
	
	int  GetCurSel();
	int  GetCurHot();
	void SetCurHot(int nIndex);
	
	BOOL IsValidIndex(int nIndex);
	int  IsSelected(int nIndex);		
	BOOL SetSelected(int nIndex, BOOL bSelect);
	
	
	int  TranslateIndex(int nRow,int nCol);
	int  ApproximateIndex(int nRow,int nCol,bool bRowPrior=true);
	
	
	int  GetSelCount() ;
	int  GetSelItems(int nMaxItems, int*rgIndex) ;
	void SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem);	
	
		
	void  AddItem( const char* ptext,const char* ptip=0,stColorQuad bkcolor=stColorQuad(0,0,0,0),int icon=-1,int data=-1,int row=-1,int col=-1,int state=UI_ITEM_STATE_DEFAULT);
	void  DeleteItem(int nIndex);

	void  DeleteAllItems();
	
	int   FindString(int nStartAfter,  const char* ptext) ;
	int   SelectString(int nStartAfter,  const char* ptext) ;
	
	void  DrawItem(int idx);

	int   IsPointInItemRect(int x,int y,int nIndex);
	
	int   FindOnPoint(int x,int y);
	int   FindNearPoint(int x,int y);
	int   FindApproxPoint(int x,int y);


	int   DisToItemCenter(int x,int y,int i);

	void  EnsureItemVisible(stListItem item);
	int   IsItemVisible(stListItem item);
	

	void ComfortSelf();
	void InitUpdate();


	int GetSelCol();
	int GetSelRow();
	const char* GetSelText();
	const char* GetSelTip();
	stColorQuad GetSelColor();
	

	int  GetItemRow(int nIndex);
	void SetItemRow(int nIndex,int nRow);

	int  GetItemCol(int nIndex);
	void SetItemCol(int nIndex,int nCol);
	
	int  GetItemData(int nIndex);
	void SetItemData(int nIndex,int nData);

	int  GetItemIcon(int nIndex);
	void SetItemIcon(int nIndex,int nIcon);

	const char* GetItemTip(int nIndex);
	void  SetItemTip(int nIndex,const char* tip);
	
	const char* GetItemText(int nIndex);
	void  SetItemText(int nIndex,const char* text);

	stRect GetAbsItemRect(int nIndex);
	stRect GetItemRect(int nIndex);
	void   SetItemRect(int nIndex,stRect rect);	


	void SetIconList( const char *filename );
	void SetIconList( CUIImage *psurface );

	void SetItemDrawBack( const char *filename );
	void SetItemDrawBack( CUIImage *psurface );
	
	void SetItemSelBack( const char *filename );
	void SetItemSelBack( CUIImage *psurface );
	
	void SetItemFocusBack( const char *filename );
	void SetItemFocusBack( CUIImage *psurface );

	void SetItemDrawPos(stRect pos){m_ItemDrawPos=pos;}
	void SetItemSelPos(stRect pos){m_ItemSelPos=pos;}
	void SetItemFocusPos(stRect pos){m_ItemFocusPos=pos;}

	void SetItemDrawFlag(int flag){m_ItemDrawFlag=flag;}

	void SetContentWidth(int width){ m_ContentWidth=width; }
	int  GetContentWidth(){ return m_ContentWidth; }


	void SetIndexOffset(int offset){ m_IndexOffset=offset; }
	int  GetIndexOffset(){ return m_IndexOffset; }


protected:

	std::vector<stListItem>m_vectorItems;

	Point2D m_NumPage;
	Point2D m_CurPage;
	

	//listbase之bug修改
	//Point2D m_CurScrollPos;

	int m_CurSel;//当前选择
	int m_CurHot;//当前焦点

	int m_CurAnchor;//当前光标在其上


	//最大内容区域宽度
	int m_ContentWidth;

	stRect m_ContentRect;

	int m_nRows;
	int m_nCols;

	int m_ItemWidth;
	int m_ItemHeight;

	int m_ItemMarginH;
	int m_ItemMarginV;

	int m_ItemTextStyle;

	//item绘制标志
	int m_ItemDrawFlag;

	int m_IndexOffset;

	
	//下面三层颜色依次重叠绘制
	stColorQuad m_ItemDrawColor;//普通状态绘制颜色
	stColorQuad m_ItemBkColor;//普通状态背景颜色
	stColorQuad m_ItemEdgeColor;//普通状态边框颜色

	stColorQuad m_ItemSelColor;//选择状态绘制颜色
	stColorQuad m_ItemSelBkColor;//选择状态绘制颜色
	stColorQuad m_ItemSelEdgeColor;//选择状态绘制颜色

	stColorQuad m_ItemFocusColor;//热点状态绘制颜色
	stColorQuad m_ItemFocusBkColor;//热点状态背景填充颜色
	stColorQuad m_ItemFocusEdgeColor;//热点状态边框颜色

	CUIImage* m_pItemDrawBack;
	CUIImage* m_pItemSelBack;
	CUIImage* m_pItemFocusBack;

	stRect m_ItemDrawPos;
	stRect m_ItemSelPos;
	stRect m_ItemFocusPos;	

	//包含所有的icon的各个帧,绘制的时候,自动计算每个icon有多少帧
	//fram_per_icon=m_pIconList.GetNumFrame()/m_VectorItems.size();
	CUIImage* m_pIconList;

};


//在listbase基础上提供项目自动排列功能
class CUIListBox : public CUIListBase
{
	
	
public:
	CUIListBox();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIListBox();
	
	virtual void ArrangeContent();
	void ArrangeSingle();
	void ArrangeMulti();
	
};



class CUIListFile : public CUIListBox
{
	
public:

	CUIListFile();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIListFile();

	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual void SetVar(stUIVar& uiVar);
	virtual void GetVar(stUIVar& uiVar);	
	
	const char* GetPathName();
	const char* GetDirectory(){  return m_Directory.c_str();  }	
	void SetDirectory(  const char* name);

private:
	
	//当前所在的目录名称
	std::string m_Directory;
	
	
};

//位置数值拾取器
//当点击鼠标,或者按下鼠标移动的时候,计算鼠标相对于客户区的位置(比例值)
//带有一个标志器,标示当前的位置
class CUIPicker: public CUIWidget
{

public:

	CUIPicker();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIPicker();
		
	virtual void PostRenderSelf();
	
	virtual void SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();

	virtual int OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );

	virtual void    SetVar(stUIVar& uiVar);
	virtual void	GetVar(stUIVar& uiVar);	

	double  GetValueH(){return m_ValueH;}
	double  GetValueV(){return m_ValueV;}

	void SetValueH(double valueH);
	void SetValueV(double valueV);

	double  GetResultValue(){ return m_ResultValue; }
	void	SetResultValue(double value);
	void	ParseResultValue();

	Point2D GetPickerPos();
	void	SetPickerPos(int x,int y);

	void SetCursorPic( const char *path );




	
private:
	
	double m_ValueH;
	double m_ValueV;

	//结果值
	double m_ResultValue;

	CUIImage *m_pPosCursor;
	
};



//只能有两个孩子，dropwin，dropbtn
//其中，具有popup属性的孩子是dropwin
//dropbtn必须是一个check
//自动注册primary
//当收到btn click消息时，切换dropwin可见
class CUIComboBox: public CUIWidget
{

	
public:


	CUIComboBox();//构造函数只能设置m_Type及只初始化一次的变量,而ClearParameter负责可重复的初始化操作
	~CUIComboBox();
	
	virtual void ClearParameter();

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );

	virtual void AddChild(CUIWidget* pwidget);
	virtual void DelChild(CUIWidget* pwidget);


	virtual void OnLoad();
	
private:

	//切换按钮
	CUIWidget*  m_pToggleWin;

	//下拉窗口
	CUIWidget* m_pDropWin;


};






#endif
