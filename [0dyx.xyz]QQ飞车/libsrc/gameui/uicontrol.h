
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
	//��ʼ��ֹ���ַ�����
	int nFirst;
	int nLast;

	//�и߶�
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
	//{{{����Ƶ���û�����������
	bool RemoveCtrlChar(int flag);
	//}}}

	void GetRawString(std::string& strBuf);
	
	int  GetSize();

	int  GetTextExtent(int start,int length,int textSize);
	int  EstimateLine( int& width,int& height,int size,int space,bool bCalcLine=false);
	
	void DrawRichText( stRect rect,int size,int space,DWORD dwColor,DWORD dwMagic,DWORD dwLink,int style,int font,int state=UI_ITEM_STATE_FOCUSED);
	int  ParseKeyChar(stKeyChar& keyChar, int px,int py,int sx,  int sy, int width,int height,int size,int space,int style);

	//add+jension 2006-02-20 11:20
	//{{{���촰�����ױ�����ʾ����
	int GetRichLineHeight(int i);
	//}}}


private:
		
	bool IsHalfDBCSPre(int pos);
	
	std::vector<stRichChar> m_RichChars;

	//�洢��ʾ������Ϣ(EstimateLine��ʱ�򣬼���m_RichLines)
	std::vector<stRichLine>m_RichLines;	

	
};



class CUIButton : public CUIWidget
{

public:

	CUIButton();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	UINotifyType m_CommandId;//push button���͸����׵�����


private:

	double m_AccelTimer;
	double m_AccelThreshold;

	
};

class CUISpin : public CUIButton
{
public:
	CUISpin();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUISpin();
	

	virtual void OnCommand();
	virtual void ClearParameter();

	
};


//radio��check��������
class CUICheck : public CUIButton
{
public:

	CUICheck();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUICheck();
	
	virtual int  OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void OnSchemeFrame();
	virtual void DefaultFrameScheme();
	virtual void OnCommand();
	virtual void ClearParameter();

};

//TABWIN��һ����ͨ����,��������dragrect��������ָ��,dragrect������winrect֮��
//��checkʱ��,���ӿɼ�,����,���ɼ�
class CUITabWin : public CUIWidget
{

	
public:
	CUITabWin();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	CUIImage *m_pTabCover;//�������Ϸ���dragrect��ͼƬ

	stRect m_TabRect;

	int m_TabWinType;//dragrect����ڴ��ڵ�ƫ�Ʒ�ʽ

};

class CUILabel : public CUIWidget
{

	
public:

	CUILabel();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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
	int m_FlashFlag;//0--����, 1--��ʾ
	int m_FlashLive;//��ʾʱ��
	int m_FlashDeath;//����ʱ��

};




class CUILineEdit : public CUIWidget
{

	
public:

	CUILineEdit();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUILineEdit();

	virtual void ClearParameter();

	int  GetEditMethod();
	void SetEditMethod( int method );
	void Clear();

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
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
	
	//�Ƿ������������
	virtual BOOL IsAcceptIme();
	
	//�Ƿ�����ַ�����
	virtual BOOL IsAcceptInput();

	void SetSeperator(const char* pChar);
	void SetRange(double min,double max);

	//У��ֵ�ķ�Χ
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
	int m_MaxChar;//���뱣֤maxchar����ż��

	int m_CursorPos;//��ǰ���λ�õ��ַ�����
	int m_DispBegin;//��ʾ��ʼλ�õ��ַ�����


	int m_SelBegin;

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//��ʾʱ��
	int m_CursorDeath;//����ʱ��

	std::string m_History[UI_EDIT_HISTORY];
	int m_NumHistory;
	int m_CurHistory;

	double m_Step;
	double m_Value;

	double m_MaxValue;
	double m_MinValue;

	//��Ϊ��ֵ����,��ֵ�ڴ˷�Χ��,������
	double m_Upper;
	double m_Lower;


	enum
	{
		INPUT_FLAG_NONE=0,
		INPUT_FLAG_CHAR=1,
		INPUT_FLAG_WORD=2,
	};

	int m_InputFlag;//0--�������뺺��,1--�������뺺��,2--��������
	
	//ѡ��״̬����ɫ
	stColorQuad m_EditSelColor;

	//disable״̬������ɫ
	stColorQuad m_GrayColor;
	

	std::string m_Seperator;
	

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	unsigned char m_EncryptCaption[UI_DEFAULT_BUFSIZE];
	int m_nSizeEncrypt;
	//bool m_bEncrypted;
	//}}}

};




//֧��ͼ��Ƕ��ĵ��б༭��
//m_Caption�д洢��ԭʼ���ַ���
//��m_RichCaption�д洢�˸߼��ṹ���ַ���
class CUIRichEdit : public CUIWidget
{
	
	
	
	
public:
	
	CUIRichEdit();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	//�Ƿ������������
	virtual BOOL IsAcceptIme();
	
	//�Ƿ�����ַ�����
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

	//�洢��������ʾ�ַ���������
	std::vector<stRichChar>m_RichCaption;
	
	//ָrichchar�ĸ���
	int m_MaxChar;
	int m_DispBegin;

	int m_SelBegin;

	//rich char�Ĺ��λ��
	int m_CursorPos;

	//ѡ��״̬����ɫ
	stColorQuad m_EditSelColor;

	//disable״̬������ɫ
	stColorQuad m_GrayColor;

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//��ʾʱ��
	int m_CursorDeath;//����ʱ��

	std::string m_History[UI_EDIT_HISTORY];
	int m_NumHistory;
	int m_CurHistory;
	
	//����ı���������û��focus��ʱ�򣬻��Ƹ����ı���
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

	

	CUIMultiEdit();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	//�Ƿ������������
	virtual BOOL IsAcceptIme();
	
	//�Ƿ�����ַ�����
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


	//�洢��������ʾ�ַ���������
	std::vector<stRichChar>m_RichCaption;

	
	//�洢��ʾ������Ϣ
	std::vector<stRichLine>m_RichLines;	


	//ָrichchar�ĸ���
	int m_MaxChar;
	
	//��ǰ���������
	int m_LinePos;
	
	int m_SelBegin;

	//rich char�Ĺ��λ��
	int m_CursorPos;

	//enter key flag
	int m_ReturnFlag;

	//ѡ��״̬����ɫ
	stColorQuad m_EditSelColor;

	//disable״̬������ɫ
	stColorQuad m_GrayColor;

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//��ʾʱ��
	int m_CursorDeath;//����ʱ��


	int m_ContentHeight;
	int m_ContentPos;
	int m_CurPage;

	
};




//���Ի�������
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


	CUIShaper();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUIShaper();

	virtual void  RenderSelf();
	virtual void  SetParameter( CVar *pvar, CUIWidget *pfather );
	virtual void ClearParameter();
	
	void RenderLines();
	void AddLine(int x0,int y0,int x1,int y1,int color);

	void SetShapeMode(int mode);
	

protected:

	//����ڴ���ԭ�������
	std::vector<stUIShape> m_Lines;

	int m_ShapeMode;

};



//��menu��չΪһ���´���
//�����Զ����ݿɼ����ӵ����Լ��Ŀͻ�����С
//�ͻ��������е��Ӵ���ռ���������
//�����Ӵ���ʱ��,���Ͻ���ʼλ�þ��ǿͻ������
//������֮��,�ͻ����յ�Ϊ���к�������չ�����λ��
//�ͻ���ֻ�Ϳɼ��������.
//���ڵ�˳���Ƕ�̬��,ÿ�����ڵ�tabstop��ʾ�ô���˳��λ��
class CUIMenu : public CUIWidget
{
	
public:

	CUIMenu();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUIMenu();

	virtual void AjustSelf();

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void PostUpdateSelf( float deltatime );	
	virtual void ClearParameter();
};


class CUIConsole : public CUIWidget
{

public:

	CUIConsole();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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
		EDIT_H = 16,//�༭�и߶�
		LINE_H = 16,//ÿ�и߶�
		NUM_LINES = 20,//�������
		LINE_LEN = 256//ÿ����󳤶�
	};
	char m_AllInfo[NUM_LINES][LINE_LEN];//���������е��ַ�������

	int  m_NumInfo;//��ǰ����

	enum
	{
		CONSOLE_MODE_TALK		=0,
		CONSOLE_MODE_SCRIPT		=1,	
	};
	int  m_CmdMode; //0--�Ի�ģʽ��������ǶԻ����� 1--�ű�����ģʽ
	
};



class CUIScreen : public CUIWidget
{

public:

	CUIScreen();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	CUIDialog();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUIDialog();

	virtual void ClearParameter();
	
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );
	virtual void RenderSelf();


};


class CUISystemDialog : public CUIDialog
{

public:
	
	CUISystemDialog();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUISystemDialog();

	virtual int OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam );
	
};



class CUIMessageBox: public CUIDialog
{


public:


	CUIMessageBox();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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


//����һ��fontpicͼƬ,��ͼƬ��0-9������ͼƬ֡
//piclabel��ֻ����ʾ�����ַ�
//������ʾ�������������
//���Ľ�������%,/�������ַ��Ļ���
class CUIPicLabel: public CUIButton
{
	
public:

	CUIPicLabel();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	CUIChangeBar();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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
	//{{{changebar�Ľ�
	//int m_NumState;
	//}}}

	int m_Value;
	std::vector<std::string> m_StateString;



};

//��checked��ʱ�򣬿��������ֵ
class CUIKeyEdit:public CUIButton
{

public:

	CUIKeyEdit();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUIKeyEdit();

	//�Ƿ������������
	virtual BOOL IsAcceptIme();
	
	//�Ƿ�����ַ�����
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
	int m_CursorFlag;//0--����, 1--��ʾ
	int m_CursorLive;//��ʾʱ��
	int m_CursorDeath;//����ʱ��
	
};

class CUIProgressBar:public CUIWidget
{
	
public:

	CUIProgressBar();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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
	
	//��ǰ��ʾλ��
	//��ʾλ�ÿ���ʵ��λ��,ֱ�����
	int m_nVisPos;
	
	double m_DelayTime;
	double m_SlowTime;

	double m_CurSlowTimer;	
	double m_CurDelayTimer;
	


};




//CUIScrollBarֻ����һ��button������spin����
//��Ϊ�����ҵ��ĵ�һ��button��Ϊ��blcokbtn
//������spin��Ϣ��Ϊup��down spin���͵���Ϣ
class CUIScrollBar:public CUIWidget
{
	
	

public:

	CUIScrollBar();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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


//���л���groupidʱ,������groupid��ȵĺ�����ʾ,������������.
class CUISwitcher:public CUIWidget
{

public:

	CUISwitcher();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	CUITabList();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUITabList();

	virtual void UpdateSelf( float deltatime );
	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );

	void ArrangeContent();

};

//��һ��������(content rect),�����к�����ռ�ݵ�����
//���һ��������ȫ�����ͻ���,�Ͳ��ɼ�.����ɼ�
class CUIContainer:public CUIWidget
{

public:

	CUIContainer();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	//ȱʡ���캯��	
	stUILineData()
	{
		intData=-1;
		memset(strData,0,UI_LINEDATA_LEN);
	}


	//��ֵ���캯��
	stUILineData(int iData,const char* sData)
	{ 
		intData=iData;
		memcpy(strData,sData,UI_LINEDATA_LEN);
	}

	//��ֵ�����������
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

	//ĳ���Զ������ݣ����ڱ�ʾĳ�е�ĳ������
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


//һ�ֶїCһ���ĵ��������б�
class CUITextList:public CUIWidget
{
	

public:
	
	
	CUITextList();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	//����ײ�ѹ��һ��
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

	//�����������
	int m_MaxLine;

	//��֮��ļ��
	int m_LineMargin;

	//�����ٶ�
	int m_ScrollSpeed;

	//�ӳ�ʱ��
	double m_LockTime;

	//��ʱ��
	double m_ScrollTimer;
	
};




//��������ͷ�ҳ��������listbox
//ֻҪwidget items��ȫ�����ͻ���,�Ͳ��ɼ�
//���Ӱ���groupstop����,groupstop�����ڷ�Χ��[0,m_Children.siez()-1],
//����й���������,���䵱����ͨ���ӿ���(����ͨ��buddy���ù�����)
class CUIWidgetList:public CUIContainer
{
	
	

public:

	CUIWidgetList();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	//Ψһ��ʶ��	
	int m_Id;

	//����λ��
	int m_X;
	int m_Y;
		
	int m_Width;
	int m_Height;

	//�ӽڵ��Ƿ��۵�
	bool m_bCollapsed;

	//�ڵ㲼��ƫ�Ʋ���
	int m_OffsetX;
	int m_OffsetY;

	//�ڵ�ߴ����
	int m_SizeX;
	int m_SizeY;
	
	stRect m_IconRect;//�ڵ������۵�ͼ������ڽڵ��λ�úͳߴ�
	
	//�ӽڵ��б�
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

	CUITreeCtrl();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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


	//�ϷŴ���
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
	stRect        ItemRect;//����ڿͻ���ԭ������꣬ÿ�λ���ʱ��ת��
	stColorQuad   ItemColor;
	std::string   ItemText;
	std::string   ItemTip;
	CUIRichText   richText;
	
} stListItem;

#define LISTBOX_MAX_ITEMS 1024

//ֻ���ṩһ��item,ÿ��item��(λ��,����,����ͼƬ,����ͼƬ���)
//���ԴӴ���ͽű���ָ����Щitem��λ�ô�С,����,ͼƬ������
//listbox,listctrl,treeview������ͨ���̳и����ʵ��
class CUIListBase : public CUIWidget
{
	
public:
		
	CUIListBase();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	
	//�ϷŴ���
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
	

	//listbase֮bug�޸�
	//Point2D m_CurScrollPos;

	int m_CurSel;//��ǰѡ��
	int m_CurHot;//��ǰ����

	int m_CurAnchor;//��ǰ���������


	//�������������
	int m_ContentWidth;

	stRect m_ContentRect;

	int m_nRows;
	int m_nCols;

	int m_ItemWidth;
	int m_ItemHeight;

	int m_ItemMarginH;
	int m_ItemMarginV;

	int m_ItemTextStyle;

	//item���Ʊ�־
	int m_ItemDrawFlag;

	int m_IndexOffset;

	
	//����������ɫ�����ص�����
	stColorQuad m_ItemDrawColor;//��ͨ״̬������ɫ
	stColorQuad m_ItemBkColor;//��ͨ״̬������ɫ
	stColorQuad m_ItemEdgeColor;//��ͨ״̬�߿���ɫ

	stColorQuad m_ItemSelColor;//ѡ��״̬������ɫ
	stColorQuad m_ItemSelBkColor;//ѡ��״̬������ɫ
	stColorQuad m_ItemSelEdgeColor;//ѡ��״̬������ɫ

	stColorQuad m_ItemFocusColor;//�ȵ�״̬������ɫ
	stColorQuad m_ItemFocusBkColor;//�ȵ�״̬���������ɫ
	stColorQuad m_ItemFocusEdgeColor;//�ȵ�״̬�߿���ɫ

	CUIImage* m_pItemDrawBack;
	CUIImage* m_pItemSelBack;
	CUIImage* m_pItemFocusBack;

	stRect m_ItemDrawPos;
	stRect m_ItemSelPos;
	stRect m_ItemFocusPos;	

	//�������е�icon�ĸ���֡,���Ƶ�ʱ��,�Զ�����ÿ��icon�ж���֡
	//fram_per_icon=m_pIconList.GetNumFrame()/m_VectorItems.size();
	CUIImage* m_pIconList;

};


//��listbase�������ṩ��Ŀ�Զ����й���
class CUIListBox : public CUIListBase
{
	
	
public:
	CUIListBox();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUIListBox();
	
	virtual void ArrangeContent();
	void ArrangeSingle();
	void ArrangeMulti();
	
};



class CUIListFile : public CUIListBox
{
	
public:

	CUIListFile();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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
	
	//��ǰ���ڵ�Ŀ¼����
	std::string m_Directory;
	
	
};

//λ����ֵʰȡ��
//��������,���߰�������ƶ���ʱ��,�����������ڿͻ�����λ��(����ֵ)
//����һ����־��,��ʾ��ǰ��λ��
class CUIPicker: public CUIWidget
{

public:

	CUIPicker();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
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

	//���ֵ
	double m_ResultValue;

	CUIImage *m_pPosCursor;
	
};



//ֻ�����������ӣ�dropwin��dropbtn
//���У�����popup���Եĺ�����dropwin
//dropbtn������һ��check
//�Զ�ע��primary
//���յ�btn click��Ϣʱ���л�dropwin�ɼ�
class CUIComboBox: public CUIWidget
{

	
public:


	CUIComboBox();//���캯��ֻ������m_Type��ֻ��ʼ��һ�εı���,��ClearParameter������ظ��ĳ�ʼ������
	~CUIComboBox();
	
	virtual void ClearParameter();

	virtual int  OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam );

	virtual void AddChild(CUIWidget* pwidget);
	virtual void DelChild(CUIWidget* pwidget);


	virtual void OnLoad();
	
private:

	//�л���ť
	CUIWidget*  m_pToggleWin;

	//��������
	CUIWidget* m_pDropWin;


};






#endif
