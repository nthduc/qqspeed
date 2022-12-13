
#include "uicontrol.h"
#include "variant.h"
#include "uiimage.h"
#include "charstr.h"

extern IDisplay *g_pDisplay;

extern void UI_DebugPrint(const char *fmt, ...);

//////////////////////////////////////////////////////////////////////////
//////////////////////////���ֽ����ӡ��ȴ���////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���ַ���pstr�е�nchar���ַ�,���㵥�д�ӡʱ
//��������ַ���ˮƽ��������λ��
static int* UI_GetTextExtentPoint( const char *pstr, int nchar, int fontsize )
{

	static std::vector<int> vecOffset;

	if(vecOffset.size()<nchar+1)
		vecOffset.resize(nchar+1);

	for( int i=0; i<nchar; i++ )
	{
		vecOffset[i] = (i+1)*fontsize/2;
		if( IsDBCSCharHead(pstr, i) ) vecOffset[i] += fontsize/2;
	}

	return &vecOffset[0];
}


//��width�����,���д�ӡpstr�е�nchar���ַ�
//��֤ĩ�ַ�pstr[nchar-1]�ɼ�
//lengthxΪʵ�ʴ�ӡ���ַ����
//����ֵ--��ӡ�����ַ�������(Ӧ�ô��Ǹ��ַ���ʼ��ӡ)
static int UI_GetTextExtent( int fontsize, const char *pstr, int nchar, int width, int &lengthx )
{
	int nsize=strlen(pstr);
	if( (nchar<1)||(nsize<1) ) //�ַ�����Ϊ0,���ͷ��ӡ,��ӡ����Ϊ0
	{
		lengthx = 0;
		return 0;
	}

	if(nchar>nsize) nchar=nsize;
	const int* partial=UI_GetTextExtentPoint( pstr, nchar, fontsize );	
	

	//�ַ�ȫ����ӡ����
	int lastx = partial[nchar-1];
	
	//�������ȫ����ӡ,���ͷ��ӡ
	if( lastx <= width ) 
	{
		lengthx=lastx;
		return 0;
	}

	//�������
	for( int i=0; i<nchar; i++ )
	{
		lengthx = lastx - partial[i];
		if( (lengthx<=width) && !IsDBCSCharHead(pstr,i) )
		{			
			return i+1;
		}
	}

	//���һ���ַ��Ŀ��>width,���ܴ�ӡ
	lengthx=0;
	return 0;
}

//��width�����,���д�ӡpstr�е��ַ�
//���ַ�pstr[start]��ʼ��ӡ
//��width�ܹ���ӡ���ٸ��ֽ�
static int UI_GetNumPrintChar( const char *pstr, int width, int fontsize)
{

	int nchar = strlen(pstr);
	if(nchar<=0) return 0;
	
	const int* partial=UI_GetTextExtentPoint( pstr, nchar, fontsize );
	

	//�����Ȳ���һ���ַ���ȵ����
	if(partial[0]>width ) return 0;
	
	for( int i=0; i<nchar-1; i++ )
	{
		if( partial[i]<=width && partial[i+1]>width ) 
			return i+1;
	}
	return (nchar);
}

static BOOL UI_IsKeyCharHead(const stRichChar& currChar,const stRichChar& prevChar)
{


	//��ǰ�ַ�������ת���ַ�
	if( (currChar.type!=RICH_CHAR_CHAR) || (currChar.keybuf[0]!=g_UICoreStyle.escapeChar) )
		return FALSE;


	//��һ���ַ�������ת���ַ�
	if( (prevChar.type==RICH_CHAR_CHAR) && (prevChar.keybuf[0]==g_UICoreStyle.escapeChar) )
		return FALSE;	
	
	return TRUE;


}


static BOOL UI_MaskRichMode(const stKeyChar& keyData,int richMode)
{

	switch(keyData.type) 
	{

	case RICH_CHAR_ICON:
		return(((richMode&UI_RICHMODE_ICON)!=0)&&(keyData.index>=DEFAULT_ICON_BASE));

	case RICH_CHAR_HLINK:
		return(((richMode&UI_RICHMODE_HLINK)!=0)&&(keyData.index>=DEFAULT_HLINK_BASE));

	case RICH_CHAR_COLOR:
		return(((richMode&UI_RICHMODE_COLOR)!=0)&&(keyData.index>=DEFAULT_COLOR_BASE));

	case RICH_CHAR_FONT:
		return(((richMode&UI_RICHMODE_FONT)!=0)&&(keyData.index>=DEFAULT_FONT_BASE));

	case RICH_CHAR_STYLE:
		return(((richMode&UI_RICHMODE_STYLE)!=0)&&(keyData.index>=DEFAULT_STYLE_BASE));
	
	case RICH_CHAR_FIELD:
		return(((richMode&UI_RICHMODE_FIELD)!=0)&&(keyData.index>=DEFAULT_FIELD_BASE));

	default:
		return FALSE;
	}

}


static Point2D UI_GetCharSize(const stRichChar& richChar,int textSize)
{

	switch(richChar.type) 
	{

	case RICH_CHAR_LF:
		return Point2D(0,textSize);

	case RICH_CHAR_CHAR:
	case RICH_CHAR_FIELD:
		return Point2D(textSize*0.5*strlen(richChar.keybuf),textSize);

	case RICH_CHAR_WORD:
		return Point2D(textSize,textSize);

	case RICH_CHAR_ICON:
		return Point2D(richChar.width,richChar.height);

	case RICH_CHAR_COLOR:
	case RICH_CHAR_FONT:
	case RICH_CHAR_STYLE:
	case RICH_CHAR_HLINK:
		return Point2D(richChar.width,textSize);

	default:
		return Point2D(0,0);
	}


}



//���Ƶ��������ַ�
static void UI_DrawChar(int sx,int sy,stRichChar& richChar,stUIPen& uiPen,const stUIPen& defaultPen,bool bGraphic=true)
{

	switch(richChar.type)
	{

	case RICH_CHAR_WORD:
	case RICH_CHAR_CHAR:
	case RICH_CHAR_FIELD:
		{
			if(bGraphic)
			{
				g_pDisplay->SetFont(uiPen.font);
				

				//ƫ�ƻ���
				if(uiPen.style&dt_MagicBack)
				{
					g_pDisplay->DrawText(sx+1,sy+1,richChar.keybuf,uiPen.dwmagic,uiPen.size);
				}
				
				DWORD dwRealColor=uiPen.dwdraw;

				if(uiPen.flag&UI_TEXTFLAG_HLINK)
				{
					//��ǰ�ַ�λ����Ч�ĳ����ӿ��Ʒ��м�
					dwRealColor=uiPen.dwlink;
				}

				g_pDisplay->DrawText(sx,sy,richChar.keybuf,dwRealColor,uiPen.size,uiPen.style);
				
				//ָ���»��߷��
				if(uiPen.style&dt_MagicLine)
				{
					if(richChar.type==RICH_CHAR_CHAR)
						g_pDisplay->DrawLine(sx,sy+uiPen.size+1,sx+uiPen.size/2,sy+uiPen.size+1,dwRealColor);
					else
						g_pDisplay->DrawLine(sx,sy+uiPen.size+1,sx+uiPen.size,sy+uiPen.size+1,dwRealColor);
				}				
				else if(uiPen.style&dt_MagicAxis)//ָ��ɾ���߷��
				{
					if(richChar.type==RICH_CHAR_CHAR)
						g_pDisplay->DrawLine(sx,sy+uiPen.size/2,sx+uiPen.size/2,sy+uiPen.size/2,dwRealColor);
					else
						g_pDisplay->DrawLine(sx,sy+uiPen.size/2,sx+uiPen.size,sy+uiPen.size/2,dwRealColor);
				}
				



			}
		}
		break;

	case RICH_CHAR_ICON:
		{
			if(bGraphic) 
			{
				stDrawChar drawChar;
				drawChar.type=richChar.type;
				drawChar.index=richChar.index;
				drawChar.state=uiPen.state;
				drawChar.rect=stRect(sx,sy,richChar.width,richChar.width);		
				RawExecCallBack(UI_Callback_DrawKeyChar,(void*)&drawChar);

			}
		}
		break;

	case RICH_CHAR_COLOR:
		{

			//�ָ�Ĭ����ɫ
			if(richChar.index<=DEFAULT_COLOR_BASE)
				uiPen.dwdraw=defaultPen.dwdraw;
			else 
			{
				//�ı���ɫ���ǲ��ı�͸����
				DWORD dwColor=g_pDisplay->GetCtrlTextColor(richChar.index);
				stColorQuad newColor(dwColor);
				stColorQuad oldColor(defaultPen.dwdraw);
				newColor.a=oldColor.a;

				uiPen.dwdraw=newColor.c;

			}

		}
		break;

	case RICH_CHAR_FONT:
		{
			//�ָ�Ĭ������
			if(richChar.index<=DEFAULT_FONT_BASE)
				uiPen.font=defaultPen.font;
			else 
				uiPen.font=richChar.index;

		}
		break;


	case RICH_CHAR_STYLE:
		{
			//�ָ�Ĭ�Ϸ��
			if(richChar.index<=DEFAULT_STYLE_BASE)
				uiPen.style=defaultPen.style;
			else 
				uiPen.style=richChar.index;

		}
		break;

	case RICH_CHAR_HLINK:
		{

			//�������ӽ�����
			if(richChar.index<=DEFAULT_HLINK_BASE)
			{

				//ȥ�������ӱ�־
				uiPen.flag&=~UI_TEXTFLAG_HLINK;

				//�ָ�������
				uiPen.style=defaultPen.style;

				
			}
			//�������ӿ�ʼ��
			else 
			{
				stKeyChar keyChar;
				richChar.GetKeyChar(keyChar);				
				RawExecCallBack(UI_Callback_ValidKeyChar,(void*)&keyChar);


				//�������������Ч�����ϳ����ӱ�־
				if(keyChar.cmdid!=RICHCHAR_CMD_NONE)
					uiPen.flag|=UI_TEXTFLAG_HLINK;

				//����»���������
				uiPen.style|=dt_MagicLine;

			}

		}
		break;
	
	default:
		break;
	
	}	

}

//------------------------------------------------------------------------------------------
//----------------------------------class CUIButton-----------------------------------------
//------------------------------------------------------------------------------------------
CUIButton::CUIButton()
{
	m_Type = WGTTYPE_BUTTON;	
}

CUIButton::~CUIButton()
{
	
}

void CUIButton::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_CommandId=NTFTYPE_COMMAND_CLICK;

	m_ExtendStyle=UI_BTN_STYLE_POPUPMSG;

	m_CursorName=BUTTON_CURSOR_NAME;

	m_AccelTimer=0;
	m_AccelThreshold=0.4f;

}

void CUIButton::SetCommandId(UINotifyType id)
{
	m_CommandId=id;
}

UINotifyType CUIButton::GetCommandId()
{
	return m_CommandId;
}


//���һ��Ĭ�ϵ�frame state
void CUIButton::DefaultFrameScheme()
{

	m_vecFrameScheme.clear();

	stFrameScheme defFrmScheme;
	defFrmScheme.Set(0,1);
	m_vecFrameScheme.push_back(defFrmScheme);

	m_CurFrameScheme=0;

}


void CUIButton::RenderSelf()
{

	CUIWidget::RenderSelf();//���Ʊ����Ͳ�͸���ĺп�

	DrawCaption();
	
}


//�����Ƿ�����굯���ʱ��oncommand
void CUIButton::SetMsgStyle(BOOL bPopMsg)
{

	if(bPopMsg)
	m_ExtendStyle |= UI_BTN_STYLE_POPUPMSG;
	else 
	m_ExtendStyle &= ~UI_BTN_STYLE_POPUPMSG;

}

//�Ƿ�����굯���ʱ��oncommand
BOOL CUIButton::GetMsgStyle()
{
	return (m_ExtendStyle&UI_BTN_STYLE_POPUPMSG)!=0;
}


void CUIButton::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;

	
	if( (pelement=pvar->Element("defaultok")) )      
		SetCommandId(NTFTYPE_COMMAND_OK);
	
	if( (pelement=pvar->Element("defaultcancel")) ) 
		SetCommandId(NTFTYPE_COMMAND_CANCEL);
	
	if( (pelement=pvar->Element("threshold")) )      
		m_AccelThreshold=pelement->Float();

}

void CUIButton::UpdateSelf( float deltatime )
{
		
	if(m_ExtendStyle&UI_BTN_STYLE_SPIN)
	{
		if(m_State& WGTSTATE_CLICKDOWN) 
		{ 
			m_AccelTimer+=deltatime;
			if(m_AccelTimer>m_AccelThreshold) 
			{
				OnCommand();								
			}
		}
		else 
		{	 
			m_AccelTimer=0;
		}
	}

	CUIWidget::UpdateSelf( deltatime );

	
}


void CUIButton::OnCommand()
{

	//��������
	stUISound sndData;
	sndData.id=m_SoundId[UI_SOUND_CLICK];
	sndData.cmd=UI_SOUND_CLICK;
	sndData.mode=0;
	ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);
	
	if(m_pFather) m_pFather->OnNotify(m_CommandId,0,(unsigned int)this);
	CallSafeScript(SCRIPT_FUNC_ON_CLICK);	//�ű����ܸı�m_pFather�����Ա���ŵ�����
}

int CUIButton::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{ 
		
	case VK_NEXT:		
	case VK_PRIOR:
	case VK_RIGHT:
	case VK_LEFT:
	case VK_UP:
	case VK_DOWN:
		{
			//ת����Ϣ
			CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);
			CUIWidget * pSecondBuddy = GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY_SECOND);

			if (pBuddy && pSecondBuddy) 
			{
				return (pBuddy->OnKeyDown(wparam,lparam,bVirtual) || pSecondBuddy->OnKeyDown(wparam,lparam,bVirtual));
			}

			if(pBuddy)
				return pBuddy->OnKeyDown(wparam,lparam,bVirtual);
			else
				return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
		}
		break;

	default:
		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
		
	}
	
	return 1;

}


int CUIButton::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{

	switch( msgtype )
	{
	case MSGTYPE_LEFTBTNDOWN:			
		
		m_State |= WGTSTATE_CLICKDOWN;

		if(!GetMsgStyle()) OnCommand();

		break;

	case MSGTYPE_LEFTBTNUP:
		if( m_State & WGTSTATE_CLICKDOWN )
		{	
			m_State &= ~WGTSTATE_CLICKDOWN;	
			
			if(GetMsgStyle()) OnCommand();
										
		}	
		break;

	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );

	}

	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;



}

//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------
//----------------------------------class CUISpin-----------------------------------------
//------------------------------------------------------------------------------------------
CUISpin::CUISpin()
{
	m_Type = WGTTYPE_SPIN;
}

CUISpin::~CUISpin()
{

}

void CUISpin::ClearParameter()
{
	CUIButton::ClearParameter();

	SetCommandId(NTFTYPE_SPINARROW);

	m_ExtendStyle=UI_BTN_STYLE_SPIN;

}


void CUISpin::OnCommand()
{
	
	CallSafeScript(SCRIPT_FUNC_ON_CLICK);
				
	CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);
	if(pBuddy) pBuddy->OnNotify(GetCommandId(),m_Style,(unsigned int)this);

	CUIWidget * pSecondBuddy = GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY_SECOND);
	if (pSecondBuddy) 
	{
		pSecondBuddy->OnNotify(GetCommandId(),m_Style,(unsigned int)this);
	}
}

//------------------------------------------------------------------------------------------
//----------------------------------class CUICheck-----------------------------------------
//------------------------------------------------------------------------------------------
CUICheck::CUICheck()
{
	m_Type = WGTTYPE_CHECK;
}

CUICheck::~CUICheck()
{
	
}

void CUICheck::ClearParameter()
{
	CUIButton::ClearParameter();

	m_CommandId=NTFTYPE_COMMAND_CHECK;

}


//���Ĭ�ϵ�frame scheme
//check��uncheck��һ֡
//check:0֡
//uncheck:1֡
void CUICheck::DefaultFrameScheme()
{
	
	m_vecFrameScheme.clear();
	
	stFrameScheme uncheckFrmState(0);
	m_vecFrameScheme.push_back(uncheckFrmState);	

	stFrameScheme checkFrmState(1);
	m_vecFrameScheme.push_back(checkFrmState);
	
	m_CurFrameScheme=0;
	
}

void CUICheck::OnSchemeFrame()
{

	if(IsChecked())
	{
		SetFrameScheme(1);
	}
	else 
	{
		SetFrameScheme(0);
	}

}

void CUICheck::OnCommand()
{
	
	if(m_ExtendStyle&UI_BTN_STYLE_RADIO)
		SelectSelf();
	else
		SetChecked(!IsChecked());
	
	CUIButton::OnCommand();	
	
}

int CUICheck::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	switch( msgtype )
	{

	case MSGTYPE_LEFTBTNDOWN:			
		
		m_State |= WGTSTATE_CLICKDOWN;

		if(!GetMsgStyle()) OnCommand();
				
		break;

	case MSGTYPE_LEFTBTNUP:
		if( m_State & WGTSTATE_CLICKDOWN )
		{						
			m_State &= ~WGTSTATE_CLICKDOWN;	

			if(GetMsgStyle()) OnCommand();
			
		}	
	
		
		break;

	default:
		return CUIButton::OnMessage( msgtype, wparam, lparam );
	}


	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;

}

//------------------------------------------------------------------------------------------
//----------------------------------class CUITabWin-----------------------------------------
//------------------------------------------------------------------------------------------
CUITabWin::CUITabWin()
{
	m_Type = WGTTYPE_TABWIN;
	
	m_pTabCover = NULL;

}

CUITabWin::~CUITabWin()
{
	SAFE_RELEASE(m_pTabCover);
}

void CUITabWin::ClearParameter()
{
	CUIWidget::ClearParameter();
	
	m_TabWinType=TABWIN_TYPE_DEFAULT;

	m_ExtendStyle=TABWIN_STYLE_NONE;

	m_TabRect.ox=0;
	m_TabRect.oy=0;
	m_TabRect.width=0;
	m_TabRect.height=0;

	SAFE_RELEASE(m_pTabCover);

}

void CUITabWin::SetTabCover( const char *path )
{
	SAFE_RELEASE( m_pTabCover );
	
	m_pTabCover = new CUIImage(path);
}

BOOL CUITabWin::IsPointInTabRect( int x, int y )
{
	stRect winRect=GetWinRect();

	if( x>winRect.ox+m_TabRect.ox && x<winRect.ox+m_TabRect.ox+m_TabRect.width &&
		y>winRect.oy+m_TabRect.oy && y<winRect.oy+m_TabRect.oy+m_TabRect.height )
		return TRUE;

	else return FALSE;


}

BOOL CUITabWin::IsPointIn( int x, int y )
{
	if(CUIWidget::IsPointIn(x,y))
	{
		return TRUE;
	}
	else 
	{		
		return IsPointInTabRect(x,y);		 
		
	}
	
}


void CUITabWin::SetChecked( bool bCheck )
{ 


	CUIWidget::SetChecked(bCheck);

	SetActived(bCheck);

	//�˴���Ҫ�Ľ�
	if( !bCheck )
	{
		if(IsFocused())
			g_pRootWgt->SetFocusWgt(NULL);
	}
	

}


void CUITabWin::DrawBarEdge()
{

	CUIWidget::DrawBarEdge();

	stColorQuad edgeColor=GetRenderColor(m_EdgeColor);
	stColorQuad backColor=GetRenderColor(m_BackColor);
	stColorQuad focusColor=GetRenderColor(m_FocusColor);

	if(IsChecked()) backColor=focusColor;

	//���Ʊ�����ɫ��ʵ�Ŀ�
	if( (m_DrawFlag&DRAWFLAG_WIN_FILL) && (backColor.a > 0) ) 
		g_pDisplay->DrawBar( m_WinRect.ox + m_TabRect.ox, m_WinRect.oy + m_TabRect.oy, m_TabRect.width, m_TabRect.height, backColor.c );
	
	//���Ʊ�����ɫ�ı�Ե
	if( (m_DrawFlag&DRAWFLAG_WIN_EDGE) && (edgeColor.a > 0) ) 
			g_pDisplay->DrawBox( m_WinRect.ox + m_TabRect.ox, m_WinRect.oy + m_TabRect.oy, m_TabRect.width, m_TabRect.height, edgeColor.c );
		
}

void CUITabWin::DrawDebugBarEdge()
{

	CUIWidget::DrawDebugBarEdge();


	
	g_pDisplay->PushClipState();
	g_pDisplay->EnableClipRect(false);


	g_pDisplay->DrawBox( m_WinRect.ox + m_TabRect.ox, m_WinRect.oy + m_TabRect.oy, m_TabRect.width, m_TabRect.height, stColorQuad(0,0,255,255).c  );


	g_pDisplay->PopClipState();


}


//ֻ���Լ�active��,�Ż����Լ��ı���
//ֻ���Լ�active��,���Ӳſɼ�
//�����������һ֡,����activeʱ����1֡,unactiveʱ,����0֡
//�������������һ֡,�����ǻ���0֡
void CUITabWin::RenderSelf()
{

	if(IsChecked())
	CUIWidget::RenderSelf();

	if(m_pTabCover)
	{			
		
		stColorQuad drawColor=GetRenderColor(stColorQuad(255,255,255,255));

		if(m_pTabCover->GetNumFrame()>1)
			m_pTabCover->Draw( m_WinRect.ox + m_TabRect.ox, m_WinRect.oy + m_TabRect.oy,(IsChecked())?1:0, fxTexMul, drawColor.c);
		else 
			m_pTabCover->Draw( m_WinRect.ox + m_TabRect.ox, m_WinRect.oy + m_TabRect.oy, 0, fxTexMul, drawColor.c );
	}

	DrawCaption();
	

}

//ֻ�е�ǰ�����ڱ�ѡ����,�Ż����Լ��ĺ���
void CUITabWin::RenderChild()
{
	if(IsChecked())
	CUIWidget::RenderChild();
}

void CUITabWin::SetTabRect( int ox, int oy, int width, int height )
{
	m_TabRect.ox = ox;
	m_TabRect.oy = oy;
	m_TabRect.width = width;
	m_TabRect.height = height;
}


void CUITabWin::DefaultTabRect()
{
	m_TabRect.ox = m_WinRect.ox;
	m_TabRect.oy = m_WinRect.oy;	
	m_TabRect.width = m_WinRect.width;
	m_TabRect.height = m_WinRect.height;	
	
}


stRect  CUITabWin::GetAbsTabRect()
{

	stRect winRect=GetWinRect();

	stRect absTabRect;
	absTabRect.ox=winRect.ox+m_TabRect.ox;
	absTabRect.oy=winRect.oy+m_TabRect.oy;
	absTabRect.width=m_TabRect.width;
	absTabRect.height=m_TabRect.height;

	return absTabRect;	

}

stRect  CUITabWin::GetHotWinRect()
{
	stRect winRect=GetWinRect();

	stRect absTabRect;
	absTabRect.ox=winRect.ox+m_TabRect.ox;
	absTabRect.oy=winRect.oy+m_TabRect.oy;
	absTabRect.width=m_TabRect.width;
	absTabRect.height=m_TabRect.height;

	return GetUnifyRect(absTabRect,winRect);

}

//���ñ�����(����ť���λ��)
//���ڲ��õ������㷨,ֻ���ڳ�ʼ�������ʱ������
void CUITabWin::SetTabWinType(int type)
{ 
	m_TabWinType=type; 
		
	if(m_TabWinType==TABWIN_TYPE_DOWN)
		m_TabRect.oy+=m_WinRect.height;
	else if(m_TabWinType==TABWIN_TYPE_UP)
		m_TabRect.oy-=m_TabRect.height;
	else if(m_TabWinType==TABWIN_TYPE_LEFT)
		m_TabRect.ox-=m_TabRect.width;
	else if(m_TabWinType==TABWIN_TYPE_RIGHT)
		m_TabRect.ox+=m_WinRect.width;
					
}

int  CUITabWin::GetTabWinType()
{ 
	return m_TabWinType; 
}

void CUITabWin::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;	

	if( (pelement=pvar->Element("tabcover")) ) SetTabCover( pelement->String() );

	
	if( (pelement=pvar->Element("tabrect")) )
	{
		int tmp[4];
		pelement->IntegerArray( tmp, 4 );
		SetTabRect( tmp[0], tmp[1], tmp[2], tmp[3] );
	}
	else 
		DefaultTabRect();

	//tabwintypeӦ��λ��TabRect֮��
	if( (pelement=pvar->Element("tabwintype")) ) SetTabWinType( pelement->Integer() );
	
		

}


int CUITabWin::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch(wparam) 
	{			
		
	case VK_TAB://tab
		{

			CUIWidget* pWgt=GetNextStopSibling();
			if(pWgt)
			{
				if(pWgt->IsVisible()&&pWgt->IsEnabled())
				{

					if(pWgt->GetExtendStyle()&TABWIN_STYLE_MANUAL_CHECK)
						pWgt->SelectSelf();
					
					g_pRootWgt->SetFocusWgt(pWgt);
				}
			}
		}
		break;
		
		
	default:
		
		if( m_pFather ) return m_pFather->OnKeyDown( wparam, lparam, bVirtual );
		else return 0;
	}
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}



int CUITabWin::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	switch( msgtype )
	{

	case MSGTYPE_GAINFOCUS:			
		{
			if((m_ExtendStyle&TABWIN_STYLE_MANUAL_CHECK)==0)
				SelectSelf();
		}
		break;


	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );
	}




	return 1;
}


const int DEFAULT_LABEL_MAX_WIDTH=800;
const int DEFAULT_LABEL_MIN_HEIGHT=0;

//------------------------------------------------------------------------------------------
//----------------------------------class CUILabel------------------------------------------
//------------------------------------------------------------------------------------------
CUILabel::CUILabel()
{
	m_Type = WGTTYPE_LABEL;
}

CUILabel::~CUILabel()
{

}

void CUILabel::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_FlashFlag  = 1;//Ĭ�Ͽɼ�,���ڲ���˸�ؼ�
	m_FlashTimer = 0;
	m_FlashLive  = 700;
	m_FlashDeath = 400;

	m_MaxWidth=DEFAULT_LABEL_MAX_WIDTH;
	m_MinHeight=DEFAULT_LABEL_MIN_HEIGHT;
	
	SetCaption("");
}


//ֻ�����߶ȣ����������
//����ֻ������ȣ��������߶�
//������ȫ���ɵ���
void CUILabel::AjustSelf()
{	

	
	if(!IsSizable()) return;//��ȸ߶ȶ������Ե���	
	
	
	int width=m_WinRect.width  - m_MarginH*2;
	int height=m_WinRect.height - m_MarginV*2;
	
	//init
	if(width<m_TextSize)  width =m_TextSize;
	if(height<m_TextSize) height=m_TextSize;
	

	if(IsSizableX()&&IsSizableY()) //��ȸ߶ȶ����Ե���,���д�ӡ
	{ 
		width=UI_MAX_INTEGER;
		height=UI_MAX_INTEGER;
		
		//�������ɴ�ӡ�Ŀ�ȸ߶�
		m_RichCaption.EstimateLine(width,height,m_TextSize,m_RowSpace);
		
		//��ȳ�����������
		if(width>m_MaxWidth)
		{
			width=m_MaxWidth;
			height=UI_MAX_INTEGER;
			
			//���Զ����ӡ�ĸ߶�
			m_RichCaption.EstimateLine(width,height,m_TextSize,m_RowSpace);
		}
		
	}
	else if(IsSizableY())//���߶ȿ��Ե���,���д�ӡ
	{	
		int widthtmp=width;
		height=UI_MAX_INTEGER;
		
		//���Զ����ӡ�ĸ߶�
		m_RichCaption.EstimateLine(widthtmp,height,m_TextSize,m_RowSpace);
	}

	if(height<m_MinHeight)
		height=m_MinHeight;
		

	//�ı䴰�ںͱ������ߴ�/////////////////////////////////////////////	

	m_CaptionRect=stRect(m_MarginH,m_MarginV,width,height);
	
	m_WinRect.width  = m_CaptionRect.width  + m_MarginH*2;
	m_WinRect.height = m_CaptionRect.height + m_MarginV*2;
	
	
	
	SetAjusted(true);
	
}

int CUILabel::ParseKeyChar(int x,int y,stKeyChar& keyChar)
{

	if(m_RichCaption.GetSize()>0)
	{
		int ox=m_WinRect.ox+m_CaptionRect.ox;
		int oy=m_WinRect.oy+m_CaptionRect.oy;
		int width=m_CaptionRect.width;
		int height=m_CaptionRect.height;
		
		return m_RichCaption.ParseKeyChar(keyChar,x,y,ox,oy,width,height,m_TextSize,m_RowSpace, m_TextStyle);
	}

	return RICHCHAR_CMD_NONE;

}


void CUILabel::RenderSelf()
{


	CUIWidget::RenderSelf();//���Ʊ����Ͳ�͸���ĺп�

	//������˸����ҵ�ǰ��˸��־Ϊ����
	if((m_Style&WGTSTYLE_FLASH)&&(!m_FlashFlag)) return;

	if(m_RichCaption.GetSize()>0)
	{
		stRect rect(m_WinRect.ox+m_CaptionRect.ox,
					m_WinRect.oy+m_CaptionRect.oy,
					m_CaptionRect.width,
					m_CaptionRect.height);

		stColorQuad magicColor=GetRenderColor(m_MagicColor);
		stColorQuad linkColor=GetRenderColor(m_LinkColor);

		stColorQuad drawColor;		
		if((m_DrawFlag&DRAWFLAG_HOTCAPTION)&&(IsMouseOver()))
		{
			drawColor=GetRenderColor(m_FocusColor);
		}
		else if((m_DrawFlag&DRAWFLAG_FOCUSCAPTION)&&(IsFocused()))
		{
			drawColor=GetRenderColor(m_FocusColor);
		}
		else 
		{
			drawColor=GetRenderColor(m_DrawColor);
		}



		int wgtState=UI_ITEM_STATE_FOCUSED;
		
		if(!IsEnabled())
			wgtState=UI_ITEM_STATE_DEFAULT;
		
		m_RichCaption.DrawRichText(rect,m_TextSize,m_RowSpace, drawColor.c, magicColor.c, linkColor.c, m_TextStyle, m_TextFont, wgtState);

	}
	
}



void CUILabel::UpdateSelf( float deltatime )
{
	
	//��˸
	if(m_Style&WGTSTYLE_FLASH)
	{
		int dtick = int(deltatime*1000);
		m_FlashTimer += dtick;
		if( (m_FlashFlag&&m_FlashTimer>m_FlashLive) || (!m_FlashFlag&&m_FlashTimer>m_FlashDeath) )
		{
			m_FlashFlag = 1-m_FlashFlag;//�л���ʾ״̬
			m_FlashTimer = 0;//���¿�ʼ��ʱ
		}
	}

	CUIWidget::UpdateSelf( deltatime );


}

void CUILabel::SetCaption( const char *pname )
{
	
	CUIWidget::SetCaption(pname);

	m_RichCaption.ParseTxt(pname,m_RichMode,m_RichFlag);

	AjustSelf();


}

void CUILabel::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
	if( (pelement=pvar->Element("flashlive")) )  m_FlashLive=pelement->Integer();
	if( (pelement=pvar->Element("flashdeath")) ) m_FlashDeath=pelement->Integer();

	
}


const int EDIT_MAX_CHAR_DEFAULT=255;
const int EDIT_MAX_VALUE_DEFAULT=2147483647;//2^31-1,�������

//-------------------------------------------------------------------------------------------
//---------------------------------------class CUILineEdit-----------------------------------
//-------------------------------------------------------------------------------------------
CUILineEdit::CUILineEdit()
{
	m_Type = WGTTYPE_EDIT;
}

CUILineEdit::~CUILineEdit()
{
}

void CUILineEdit::ClearParameter()
{
	CUIWidget::ClearParameter();
	
	m_EditMethod = UI_EDITMETHOD_ENGLISH|UI_EDITMETHOD_NUMBER|UI_EDITMETHOD_SYMBOL|UI_EDITMETHOD_CHINESE;

	m_TextStyle=dtLeft|dtVCenter;

	m_MaxChar = EDIT_MAX_CHAR_DEFAULT;
	m_CursorPos = 0;
	m_DispBegin = 0;

	m_SelBegin=0;

	m_CursorTimer = 0;
	m_CursorFlag = 1;
	m_CursorLive = 700;
	m_CursorDeath =400;

	m_NumHistory = 0;
	m_CurHistory = -1;

	//�ڵװ���
	m_BackColor = stColorQuad(0,0,0,255);
	m_DrawColor = stColorQuad(255,255,255,255);
	m_FocusColor = stColorQuad(255,255,255,255);
	m_EditSelColor=stColorQuad(0,0,255,128);
	m_GrayColor=stColorQuad(128,128,128,255);


	m_MaxValue=EDIT_MAX_VALUE_DEFAULT;
	m_MinValue=0.0;

	m_Upper=EDIT_MAX_VALUE_DEFAULT;
	m_Lower=0.0;

	m_Value=0.0;
	m_Step=1.0;

	m_Seperator="/";

	m_ExtendStyle=EDIT_STYLE_NONE;

	SetEditable(true);

	SetCaption("");

}

void CUILineEdit::AddChild( CUIWidget *pwidget )
{
	
	CUIWidget::AddChild(pwidget);
	
	if(pwidget->GetType()==WGTTYPE_SPIN) 
	{ 
		CUISpin* pSin=(CUISpin*)pwidget;		
		pSin->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",this);
	}
	
	
}

void CUILineEdit::DelChild( CUIWidget *pwidget )
{
	
	if(pwidget->GetType()==WGTTYPE_SPIN) 
	{ 
		CUISpin* pSin=(CUISpin*)pwidget;		
		pSin->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",0);
	}
	
	CUIWidget::DelChild(pwidget);
	
}


void CUILineEdit::CheckForReason()
{

	assert(m_SelBegin>=0);
	assert((m_CursorPos>=0)&&(m_CursorPos<=m_Caption.size()));
	assert(m_DispBegin>=0);
	
	//ȷ�����λ�ú���
	if(m_SelBegin<0)
		m_SelBegin=0;
	else if(m_SelBegin>m_Caption.size())
		m_SelBegin=m_Caption.size();

	if(m_CursorPos<0)
		m_CursorPos=0;
	else if(m_CursorPos>m_Caption.size())
		m_CursorPos=m_Caption.size();

	if(m_DispBegin<0)
		m_DispBegin=0;
	else if(m_DispBegin>m_CursorPos)
		m_DispBegin=m_CursorPos;

}

BOOL CUILineEdit::IsCursorVisible()
{

	
	//���ƹ��
	bool bCursorVis=false;		
	if(IsEnabled()&&(m_SelBegin==m_CursorPos))
	{
		if(g_pRootWgt->GetFocusWgt()==this)
		{	
			bCursorVis=true;	
		}
		else if(m_ExtendStyle&EDIT_STYLE_KEEPCURSOR)
		{			
			bCursorVis=true;			
		}
		else if(m_ExtendStyle&EDIT_STYLE_SMARTCURSOR)
		{	
			/*
			CUIWidget* pFocusWgt=g_pRootWgt->GetFocusWgt();
			
			if(pFocusWgt&&pFocusWgt->IsAcceptInput())
				bCursorVis=false;
			else
				bCursorVis=true;
			*/
			bCursorVis=(g_pRootWgt->GetHotWgt()==this);

		}

	}	

	if(bCursorVis&&m_CursorFlag) 
		return TRUE;
	else 
		return FALSE;

}

void CUILineEdit::DrawCaption()
{

	stRect CapRect=GetAbsoluteCaptionRect();

	stColorQuad drawColor=GetCaptionColor();
	stColorQuad selColor=GetRenderColor(m_EditSelColor);

	if(!IsEnabled()) 
	{
		drawColor=GetRenderColor(m_GrayColor);
	}
	
	if( m_DispBegin > m_CursorPos ) m_DispBegin = m_CursorPos;
	//if( m_DispBegin > m_SelEnd) m_DispBegin = m_SelEnd;
	
	int cursorx;//���ˮƽλ��
	int offset = UI_GetTextExtent(m_TextSize, m_Caption.c_str()+m_DispBegin, m_CursorPos-m_DispBegin, m_CaptionRect.width, cursorx );
	m_DispBegin += offset;
	
	if( !m_Caption.empty() )
	{
		static char buffer[UI_DEFAULT_BUFSIZE];
		const char *pstr = m_Caption.c_str()+m_DispBegin;
		
		int numchar = UI_GetNumPrintChar( pstr, m_CaptionRect.width, m_TextSize );//����ͻ�����������ʾ���ٸ��ַ�
		
		//add by lincoln [2007:4:1] [13:45]
		//{{{����ؼ��Ľ�
		/*
		static char buffer[UI_DEFAULT_BUFSIZE];
		if( m_EditMethod & UI_EDITMETHOD_CODER ) memset( buffer, '*', numchar );
		else memcpy( buffer, pstr, numchar );

		buffer[numchar] = 0;
		*/

		
		g_pDisplay->DrawTextEx( 
			m_WinRect.ox+m_CaptionRect.ox, 
			m_WinRect.oy+m_CaptionRect.oy, 
			m_CaptionRect.width, 
			m_CaptionRect.height,
			m_Caption.c_str(),
			m_TextSize,
			m_RowSpace, 
			drawColor.c, 
			m_TextStyle);

		const int* partial=UI_GetTextExtentPoint( m_Caption.c_str(), m_Caption.size(), m_TextSize );

		//ת��Ϊ�������ʾͷ��������
		int dispBeginX;
		int selBeginX;
		int selEndX;
		
		dispBeginX=(m_DispBegin>0)?partial[m_DispBegin-1]:0;
		
		selBeginX=(m_SelBegin>0)?partial[m_SelBegin-1]:0;
		selBeginX-=dispBeginX;


		selEndX=(m_CursorPos>0)?partial[m_CursorPos-1]:0;
		
		selEndX-=dispBeginX;
		
		

		if(selEndX<0) selEndX=0;
		else if(selEndX>m_CaptionRect.width) selEndX=m_CaptionRect.width; 

		if(selBeginX<0) selBeginX=0;
		else if(selBeginX>m_CaptionRect.width) selBeginX=m_CaptionRect.width; 

		int barBeginX=(selBeginX>selEndX)?selEndX:selBeginX;
		int barEndX=(selBeginX<selEndX)?selEndX:selBeginX;
		

		g_pDisplay->DrawBar(
			CapRect.ox+barBeginX,CapRect.oy,
			barEndX-barBeginX,m_TextSize,
			selColor.c);


	}

	//���ƹ��
	if(IsCursorVisible())
	{
		int cursorh = m_TextSize;
		
		if(m_TextStyle&dtVCenter)
		{
			g_pDisplay->DrawBar(CapRect.ox+cursorx,CapRect.oy+(CapRect.height-cursorh)*0.5,1,cursorh,drawColor.c );
		}
		else if(m_TextStyle&dtTop)
		{
			g_pDisplay->DrawBar(CapRect.ox+cursorx,CapRect.oy,1,cursorh,drawColor.c );
		}
		else//ȱʡ��ΪdtBottom
		{
			g_pDisplay->DrawBar(CapRect.ox+cursorx,CapRect.oy+CapRect.height-cursorh,1,cursorh,drawColor.c );
		}

	}



}


//message handlers
void CUILineEdit::RenderSelf()
{
	
	//���Ʊ����ͱ߿�
	CUIWidget::RenderSelf();

	DrawCaption();	
}

//��鳤�ȿ�λ
//ֻҪ��caption�ı�ĵط���Ӧ�õ��øú���
void CUILineEdit::UpdateInputFlag()
{	

	if(m_MaxChar-m_Caption.size()<1) 
		m_InputFlag=INPUT_FLAG_NONE;
	else if(m_MaxChar-m_Caption.size()<2 && !IsDBCSCharHead(m_Caption.c_str(),m_CursorPos-1)) 
		m_InputFlag=INPUT_FLAG_CHAR;
	else 
		m_InputFlag=INPUT_FLAG_WORD;
	
}

bool CUILineEdit::QueryInputFlag(char key)
{

	char buffer[2];
	buffer[0]=key;
	buffer[1]=0;

	UpdateInputFlag();

	//��鳤��
	if( m_InputFlag==INPUT_FLAG_NONE )
	{
		return false;
	}
	else if(m_InputFlag==INPUT_FLAG_CHAR)
	{
		if(IsDBCSChar(buffer,0) ) return false;//����
	}
	
	
	
	//mask����
	if(IsUpcaseChar(buffer[0])||IsLowcaseChar(buffer[0]))//Ӣ��
	{
		if((m_EditMethod&UI_EDITMETHOD_ENGLISH)==0)
			return false;
	}
	else if(IsNumberChar(buffer[0]))//����
	{
		if((m_EditMethod&UI_EDITMETHOD_NUMBER)==0)
			return false;
	}
	else if(IsSymbolChar(buffer[0]))//����
	{
		if((m_EditMethod&UI_EDITMETHOD_SYMBOL)==0)
			return false;
	}
	else if(IsDBCSChar(buffer,0))//����
	{
		if((m_EditMethod&UI_EDITMETHOD_CHINESE)==0)
			return false;	 
	}


	//value����
	if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0)
	{
		if(!IsNumberChar(buffer[0]))
			return false;
	}

	//pwd����
	if((m_EditMethod&UI_EDITMETHOD_CODER)!=0)
	{
		if(!IsPrintableWord(buffer[0]))
			return false;
	}
	

	return true;

}

bool CUILineEdit::CheckValueRange()
{
	bool bFixed=false;

	if(m_MaxValue<m_MinValue) 
	{
		m_MaxValue=m_MinValue;
		bFixed=true;
	}

	if(m_Value<m_MinValue) 
	{
		m_Value=m_MinValue;
		bFixed=true;
	}

	if(m_Value>m_MaxValue) 
	{
		m_Value=m_MaxValue;
		bFixed=true;
	}

	return bFixed;

}

void CUILineEdit::UpdateSelf( float deltatime )
{
	
	CheckForReason();

	//��˸���
	if((m_ExtendStyle&EDIT_STYLE_FLASHCURSOR)||(g_pRootWgt->GetFocusWgt() == this))
	{
		int dtick = int(deltatime*1000);
		m_CursorTimer += dtick;
		if( (m_CursorFlag&&m_CursorTimer>m_CursorLive) || (!m_CursorFlag&&m_CursorTimer>m_CursorDeath) )
		{
			m_CursorFlag = 1-m_CursorFlag;
			m_CursorTimer = 0;
		}
	}
	else 
	{
		m_CursorFlag=1;
		m_CursorTimer = 0;
	}

	if(IsUpdateDirty())
	{
		CallSafeScript(SCRIPT_FUNC_ON_VALUE_CHANGE);
		SetUpdateDirty(false);
	}

	
	CUIWidget::UpdateSelf( deltatime );
	
}

int CUILineEdit::GetEditMethod()
{
	return m_EditMethod;
}

void CUILineEdit::SetEditMethod( int method )
{
	m_EditMethod = method;
}

void CUILineEdit::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );

	if(!pvar) return;
	CVar *pelement=NULL;

	
	if( (pelement=pvar->Element("seperator")) )	m_Seperator=pelement->String();

	if( (pelement=pvar->Element("maxchar")) )	m_MaxChar=pelement->Integer();
			
	if( (pelement=pvar->Element("editmethod")) ) SetEditMethod( pelement->Integer() );

	if( (pelement=pvar->Element("maxvalue")) )	 m_MaxValue=pelement->Integer();
	if( (pelement=pvar->Element("minvalue")) )	 m_MinValue=pelement->Integer();

	if( (pelement=pvar->Element("upper")) )		m_Upper=pelement->Integer();
	if( (pelement=pvar->Element("lower")) )		m_Lower=pelement->Integer();


	//Ӧ�þ���range֮��
	if( (pelement=pvar->Element("value")) )		 SetVar( stUIVar(VALUE_CHANNEL_VALUE,0,pelement->Integer(),0));

	if( (pelement=pvar->Element("step")) )	     SetStep( pelement->Integer() );

	if( (pelement=pvar->Element("selcolor")) )
	{
		int color[4];
		pelement->IntegerArray( color, 4 );
		SetEditSelColor( stColorQuad(color[0], color[1], color[2], color[3]) );
		
	}	
	
	if( (pelement=pvar->Element("graycolor")) )
	{
		int color[4];
		pelement->IntegerArray( color, 4 );
		SetGrayColor( stColorQuad(color[0], color[1], color[2], color[3]) );
	}
	
}


//add by lincoln [2007:4:1] [13:45]
//{{{����ؼ��Ľ�
void CUILineEdit::Encrypt()
{
	/*
	assert(!m_bEncrypted);
	if(m_bEncrypted) return;
	m_bEncrypted=true;
	*/

	int len=m_Caption.length();

	stUIEncrypt uiVar;
	uiVar.pInBuf=(unsigned char*)m_Caption.c_str();
	uiVar.nInBufLen=len;

	uiVar.pOutBuf=m_EncryptCaption;
	uiVar.nOutBufLen=UI_DEFAULT_BUFSIZE;

	ExecCallBackFunc(UI_Callback_Encrypt,(void*)&uiVar);

	m_nSizeEncrypt=uiVar.nOutBufLen;	

	memset((void*)m_Caption.c_str(),'*',len);	

}

void CUILineEdit::Decrypt()
{

	/*
	assert(m_bEncrypted);
	if(!m_bEncrypted) return;
	m_bEncrypted=false;
	*/

	int len=m_Caption.length();


	stUIEncrypt uiVar;
	uiVar.pInBuf=m_EncryptCaption;
	uiVar.nInBufLen=m_nSizeEncrypt;

	unsigned char caption[UI_DEFAULT_BUFSIZE];	
	uiVar.pOutBuf=caption;
	uiVar.nOutBufLen=UI_DEFAULT_BUFSIZE;

	ExecCallBackFunc(UI_Callback_Decrypt,(void*)&uiVar);

	m_Caption=(const char*)uiVar.pOutBuf;

	for(int i=0;i<len;i++)
	{ 
		uiVar.pOutBuf[i]=0;
	}

	assert(len==m_Caption.length());

}
//}}}

void CUILineEdit::Clear()
{
	m_Caption.erase();

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	memset(m_EncryptCaption,0,UI_DEFAULT_BUFSIZE);
	m_nSizeEncrypt=0;
	//m_bEncrypted=true;
	//}}}

	m_CursorPos = 0;
	m_DispBegin = 0;		
	
	m_SelBegin=0;
	m_Value=0;	
	
}



//[2004:9:2] [15:32]
void CUILineEdit::SetStep(double step)
{
	
	m_Step=step;	
	
}

//�Ƿ������������
BOOL CUILineEdit::IsAcceptIme()
{
	if(m_EditMethod&UI_EDITMETHOD_CODER) return FALSE;

	return IsEnabled();

}

BOOL CUILineEdit::IsAcceptInput()
{
	return IsEnabled();
}

void CUILineEdit::SetCaption( const char *pname )
{

	Clear();	
	
	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	//}}}

	int len=strlen(pname);
	for(int i=0;i<len;i++)
	{ 

		char buffer[2];
		buffer[0] = pname[i];
		buffer[1] = 0;
		

		//�Ǵ�ӡ�ַ�
		if(!IsPrintableWord(buffer[0]))
			continue;
			
		if(!QueryInputFlag(buffer[0]))
		{
			continue;
		}
		
		m_Caption.insert( m_CursorPos, buffer );
		m_CursorPos++;
		m_SelBegin=m_CursorPos;

	}

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	//}}}

	if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
	{
		SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,atof(m_Caption.c_str()),0));
	}

	SetUpdateDirty(true);


}

void CUILineEdit::CaptionToValue()
{
	assert(m_EditMethod&UI_EDITMETHOD_CODER==0);

	double value=0.0;
	
	if(m_Style&WGTSTYLE_ADVANCE) //���ֵ����ĸ��ʾ
	{ 

		double maxvalue=0.0;

		std::string valueformat="%d"+m_Seperator+"%d";

		sscanf(m_Caption.c_str(),valueformat.c_str(),&value,&maxvalue);

		m_MaxValue=maxvalue;
		
		
	}
	else
	{
		value=atof(m_Caption.c_str());
	}

	//���ֵ�Ƿ�ı�
	if(m_Value!=value)
	{
		m_Value=value;	
	
		CheckValueRange();

		//add by lincoln [2007:4:1] [13:45]
		//{{{����ؼ��Ľ�
		SetUpdateDirty(true);
		/*
		char buf[UI_TEMPORARY_BUFSIZE];
		itoa(m_Value,buf,10);
		CallSafeScript(SCRIPT_FUNC_ON_VALUE_CHANGE,buf);
		*/
		//}}}
		
	}
	

}



void CUILineEdit::ValueToCaption()
{
	char buffer[UI_TEMPORARY_BUFSIZE];
	
	if(m_Style&WGTSTYLE_ADVANCE) //���ֵ����ĸ��ʾ
	{ 
		SetEditable(false);
		
		std::string valueformat="%d"+m_Seperator+"%d";

		sprintf(buffer,valueformat.c_str(),(int)m_Value,(int)m_MaxValue);
		m_Caption=buffer;
		m_CursorPos=m_Caption.size();
		m_DispBegin=0;
		m_SelBegin=m_CursorPos;
		
		
	}
	else
	{
		sprintf(buffer,"%d",(int)m_Value);
		m_Caption=buffer;
		m_CursorPos=m_Caption.size();
		m_DispBegin=0;
		m_SelBegin=m_CursorPos;
		
	}

}

void CUILineEdit::SetRange(double min,double max)
{
	m_MinValue=min;
	m_MaxValue=max;
}

void CUILineEdit::SetSeperator(const char* pChar)
{
	m_Seperator=pChar;
}


void CUILineEdit::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel) 
	{
		//add by lincoln [2007:4:1] [13:45]
		//{{{����ؼ��Ľ�
	case VALUE_CHANNEL_CAPTION:
		{
			if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
			uiVar.sValue = GetCaption();
			if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
		}
		break;

	case VALUE_CHANNEL_METHOD:
		uiVar.fValue=m_EditMethod;
		break;
		//}}}

	case VALUE_CHANNEL_VALUE:
		uiVar.fValue = m_Value;
		break;

	case VALUE_CHANNEL_MINVALUE:
		uiVar.fValue = m_MinValue;
		break;

	case VALUE_CHANNEL_MAXVALUE:
		uiVar.fValue = m_MaxValue;
		break;

	case VALUE_CHANNEL_RANGE_END:
		uiVar.fValue = m_CursorPos; 
		break;

	case VALUE_CHANNEL_RANGE_BEGIN:
		uiVar.fValue = m_SelBegin;
		break;

	case VALUE_CHANNEL_UPER_H:
		uiVar.fValue = m_Upper;
		break;

	case VALUE_CHANNEL_LOWER_H:
		uiVar.fValue = m_Lower;
		break;
	
	default:
		CUIWidget::GetVar(uiVar);
	
	}

}

//[2004:9:2] [15:32]
void CUILineEdit::SetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel) 
	{
		
	case VALUE_CHANNEL_VALUE:
		{		
			if(m_Value!=uiVar.fValue)
			{
				m_Value=uiVar.fValue;	
			
				CheckValueRange();

				//add by lincoln [2007:4:1] [13:45]
				//{{{����ؼ��Ľ�
				SetUpdateDirty(true);
				/*
				char buf[UI_TEMPORARY_BUFSIZE];
				itoa(m_Value,buf,10);
				CallSafeScript(SCRIPT_FUNC_ON_VALUE_CHANGE,buf);
				*/
				//}}}				
			}
			
			
			if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
			ValueToCaption();
			

		}
		break;

	//ֻ�ǲ�����OnValueChange�ű�����
	case VALUE_CHANNEL_POS:
		{		
			if(m_Value!=uiVar.fValue)
			{
				m_Value=uiVar.fValue;	
			
				CheckValueRange();
			}
			
			
			if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
			ValueToCaption();
			

		}
		break;

	case VALUE_CHANNEL_RANGE_END: 
		{
			m_CursorPos=uiVar.fValue; 
			if(m_CursorPos<0) m_CursorPos=0;
			else if(m_CursorPos>m_Caption.size()) m_CursorPos=m_Caption.size();
			
		}
		break;

	case VALUE_CHANNEL_RANGE_BEGIN:
		{
			m_SelBegin=uiVar.fValue;
			if(m_SelBegin<0) m_SelBegin=0;
			else if(m_SelBegin>m_Caption.size()) m_SelBegin=m_Caption.size();
		
		}
		break;

		//add by lincoln [2007:4:1] [13:45]
		//{{{����ؼ��Ľ�
	case VALUE_CHANNEL_METHOD:
		{
			m_EditMethod=uiVar.fValue;
		}
		break;
		//}}]

	case VALUE_CHANNEL_MINVALUE:
		m_MinValue=uiVar.fValue;
		break;

	case VALUE_CHANNEL_MAXVALUE:
		m_MaxValue=uiVar.fValue;
		break;

	case VALUE_CHANNEL_UPER_H:
		m_Upper=uiVar.fValue;
		break;

	case VALUE_CHANNEL_LOWER_H:
		m_Lower=uiVar.fValue;
		break;

	//text////////////////////////////////////////////////////////////////////////

	case VALUE_CHANNEL_EDITHISTORY:
		AddStringToHistory(uiVar.sValue.c_str());
		break;
	
	default:
		CUIWidget::SetVar(uiVar);
	}

}

void CUILineEdit::OnInit()
{
	SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,m_Value,0));
}

//bLeft:�Ƿ񽫹�꿿��
//bMove:�Ƿ��ƶ����
int CUILineEdit::CancelSel(bool bLeft,bool bMove)
{

	if(m_SelBegin!=m_CursorPos)
	{

		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		

		if(bMove)	
			m_CursorPos=(bLeft)?selBegin:selEnd;

		m_SelBegin=m_CursorPos;
		
		
		return 1;
	}
	else return 0;
	

}


void CUILineEdit::SelectAllText()
{
	SetSel(0,m_Caption.size());
}

//ѡ������
//����ֵ:0-ʧ��,1-�ɹ�
int CUILineEdit::SetSel(int start,int end)
{

	if(start<0) start=0;
	else if(start>m_Caption.size()) start=m_Caption.size();

	if(end<0) end=0;
	else if(end>m_Caption.size()) end=m_Caption.size();
	
	m_SelBegin=start;
	m_CursorPos=end;

	return 1;

}

//����1:֮ǰ��ѡ��
//����0:֮ǰû��ѡ��
int CUILineEdit::ClearSel()
{
	
	//��ʷ�б�ص���ʼ
	m_CurHistory = -1;	

	if(m_SelBegin!=m_CursorPos)
	{
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		
		m_Caption.erase(selBegin,selEnd-selBegin);

		if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
		{
			CaptionToValue();
		}
		
		m_CursorPos=selBegin;
		m_SelBegin =selBegin;

		SetUpdateDirty(true);
	
		return 1;
	}
	else return 0;
	

}


void CUILineEdit::OnInsert(const char* pText)
{

	if(!IsEditable()) return;

	if(strcmp(pText,"")==0) return;

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	//}}}


	int len=strlen(pText);
	for(int i=0;i<len;i++)
	{ 

		char buffer[2];
		buffer[0] = pText[i];
		buffer[1] = 0;
		

		//�Ǵ�ӡ�ַ�
		if(!IsPrintableWord(buffer[0]))
			continue;
			
		if(!QueryInputFlag(buffer[0]))
		{
			continue;
		}
		
		m_Caption.insert( m_CursorPos, buffer );
		m_CursorPos++;
		m_SelBegin=m_CursorPos;

	}

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	//}}}

	if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
	{
		SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,atof(m_Caption.c_str()),0));
	}

	SetUpdateDirty(true);

}

void CUILineEdit::OnPaste()
{
	if(!IsEditable()) return;

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	//}}}

	ClearSel();

	if( !::OpenClipboard(NULL) ) return;
	HANDLE handle = ::GetClipboardData(CF_TEXT);
	::CloseClipboard();
	if( !handle ) return;

	std::string  pasteTxt((const char*)handle);

	for(int i=0;i<pasteTxt.size();i++)
	{ 

		char buffer[2];
		buffer[0] = pasteTxt[i];
		buffer[1] = 0;
		

		//�Ǵ�ӡ�ַ�
		if(!IsPrintableWord(buffer[0]))
			continue;
			
		if(!QueryInputFlag(buffer[0]))
		{
			continue;
		}
		
		m_Caption.insert( m_CursorPos, buffer );
		m_CursorPos++;
		m_SelBegin=m_CursorPos;

	}

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	//}}}

	if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
	{
		SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,atof(m_Caption.c_str()),0));
	}

	SetUpdateDirty(true);

	
}

void CUILineEdit::OnCopy()
{

	if(m_SelBegin!=m_CursorPos)
	{
		//�õ�ѡ���ı�
		
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		


		std::string selText=m_Caption.substr(selBegin,selEnd-selBegin);
		if(selText.empty()) return;

		//add by lincoln [2007:4:1] [13:45]
		//{{{����ؼ��Ľ�
		/*
		if(m_EditMethod&UI_EDITMETHOD_CODER)
		{
		unsigned int num=selText.size();
		selText=std::string(num,'*');
		}
		*/


		//�õ������ڴ�
		HGLOBAL hGlobal=GlobalAlloc(GHND|GMEM_SHARE,selText.size()+1);
		if(!hGlobal) return;

		//��乲���ڴ�
		LPVOID pGlobal=GlobalLock(hGlobal);
		strcpy((char*)pGlobal,selText.c_str());
		GlobalUnlock(hGlobal);
		

		//���ü�����
		if( !::OpenClipboard(NULL) ) return;
		::EmptyClipboard();
		::SetClipboardData(CF_TEXT,hGlobal);
		::CloseClipboard();
		
	}		


}

void CUILineEdit::OnCut()
{

	OnCopy();

	if(!IsEditable()) return;

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	//}}}

	ClearSel();

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	//}}}

	SetUpdateDirty(true);

}


void CUILineEdit::AddStringToHistory( const char* text)
{
	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER )
	{
		return;
	}
	//}}}

	if(!text || !strcmp(text,"")) return;

	for( int i=UI_EDIT_HISTORY-1; i>0; i-- ) 
	m_History[i] = m_History[i-1];

	m_History[0] = text;
	if( m_NumHistory < UI_EDIT_HISTORY ) m_NumHistory++;
	m_CurHistory = -1;
}





int CUILineEdit::OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(!IsEditable()) return 1;

	if(HasScriptFunc(SCRIPT_FUNC_ONCHAR))
	{		
		//add by lincoln [2007:4:1] [13:45]
		//{{{����ؼ��Ľ�
		unsigned int param_w=wparam;
		unsigned int param_l=lparam;
		if( m_EditMethod & UI_EDITMETHOD_CODER )
		{
			param_w=0;
			param_l=0;
		}
		//}}}

		if(CallSafeScriptEx(SCRIPT_FUNC_ONCHAR,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}


	//�Ǵ�ӡ�ַ�
	if(!IsPrintableWord(wparam)) return 0;

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	//}}}

	ClearSel();
	SetUpdateDirty(true);

	char buffer[2];
	buffer[0] = (char)wparam;
	buffer[1] = 0;	
	
	if(QueryInputFlag(buffer[0]))
	{
		m_Caption.insert( m_CursorPos, buffer );
		m_CursorPos++;
		m_SelBegin=m_CursorPos;	

		if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
		{
			SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,atof(m_Caption.c_str()),0));
		}
	}

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	//}}}


	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;

	
}


int CUILineEdit::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		//add by lincoln [2007:4:1] [13:45]
		//{{{����ؼ��Ľ�
		unsigned int param_w=wparam;
		unsigned int param_l=lparam;
		if( m_EditMethod & UI_EDITMETHOD_CODER )
		{
			param_w=0;
			param_l=0;
		}
		//}}}

		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	SetUpdateDirty(true);

	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	//}}}

	bool bDefault=false;
	switch( wparam )
	{
		
	
	case VK_BACK:
		if(IsEditable())
		{
			if(!ClearSel())
			{
			
				if( m_CursorPos <= 0 ) break;
				
				//���ǰ�Ǻ�����,��Ϊ�϶�������������
				if(IsDBCSChar(m_Caption.c_str(), m_CursorPos-1) )
				{
					//add by jension [2007:1:19] [15:50]
					//{{{������ɫʱ ��ɫ��ֻ����������,�޷������˸񡱼������������
					if ( m_CursorPos == 1 
						|| (!IsDBCSChar(m_Caption.c_str(), m_CursorPos-2) || IsDBCSCharTail(m_Caption.c_str(), m_CursorPos-2)) )
					{
						if (m_DispBegin > 0) m_DispBegin -= 1;
						m_CursorPos -= 1;
						m_Caption.erase( m_CursorPos, 1 );
					}
					//}}}
					else 
					{

						if(m_DispBegin >= 2)
							m_DispBegin -= 2;

						if(m_CursorPos >= 2)
						{
							m_CursorPos -= 2;
							m_Caption.erase( m_CursorPos, 2 );
						}

					}
					
				}
				//���ǰ��E��
				else
				{
					if(m_DispBegin >= 1&&!IsDBCSCharTail(m_Caption.c_str(), m_DispBegin-1))	
						m_DispBegin -= 1;
					
					m_CursorPos -= 1;
					m_Caption.erase( m_CursorPos, 1 );
					
					
				}

				m_SelBegin=m_CursorPos;


				if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
				{
					CaptionToValue();
				}

			}
		}
		break;
	
	case VK_LEFT:

		//������ctrl��,ͬhome
		if((lparam&MK_CONTROL))
		{
			m_CursorPos = 0;
		}
		else 
		{
		
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			if(CancelSel(true))	break;
			

			if(m_CursorPos>=2&&IsDBCSCharHead(m_Caption.c_str(),m_CursorPos-2)) 
				m_CursorPos -= 2;
			else if(m_CursorPos>0) 
				m_CursorPos -= 1;

			assert( m_CursorPos <= m_Caption.size() );

			
		}

		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		m_SelBegin=m_CursorPos;

		break;
        
	case VK_RIGHT:

		//������ctrl��,ͬend
		if((lparam&MK_CONTROL))
		{
			m_CursorPos = m_Caption.size();
		}
		else 
		{
		 
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			if(CancelSel(false))	break;
			
			
			if( m_CursorPos<m_Caption.size()&&IsDBCSCharHead(m_Caption.c_str(), m_CursorPos) ) 
				m_CursorPos += 2;
			else if( m_CursorPos<m_Caption.size())
				m_CursorPos += 1;

			assert( m_CursorPos <= m_Caption.size() );

		}
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		m_SelBegin=m_CursorPos;
		
		break;
        
	case VK_HOME:

		m_CursorPos = 0;

		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		m_SelBegin=m_CursorPos;
		
		break;
        
	case VK_END:
	
		m_CursorPos = m_Caption.size();

		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		m_SelBegin=m_CursorPos;
		
		break;

	case VK_DELETE:
		if(IsEditable())
		{
			if(!ClearSel())
			{

				if( m_CursorPos == m_Caption.size() ) break;

				//modify+lincoln
				//[2005:5:28] [11:46]
				//{{{
				if( IsDBCSCharHead(m_Caption.c_str(), m_CursorPos) )
				{
					if( m_Caption.size() >= m_CursorPos+2 )
						m_Caption.erase( m_CursorPos, 2 );
					else if( m_Caption.size() >= m_CursorPos+1 )
						m_Caption.erase( m_CursorPos, 1 );
				}
				else
				{
					if( m_Caption.size() >= m_CursorPos+1 )
					m_Caption.erase( m_CursorPos, 1 );
				}		
				//}}}

				
				if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
				{
					CaptionToValue();
				}

			}
	
		}
		break;

	case VK_UP:
		if(IsEditable())
		{		
			if( m_CurHistory+1 < m_NumHistory )
			{
				m_CurHistory++;
				m_Caption = m_History[m_CurHistory];
				m_CursorPos = m_Caption.size();
				m_DispBegin = 0;
				m_SelBegin=m_CursorPos;

				if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
				{
					CaptionToValue();
				}	
				
			}
		}
		break;

	case VK_DOWN:
		if(IsEditable())
		{
		
			if( m_CurHistory > 0 )
			{
				m_CurHistory--;
				m_Caption = m_History[m_CurHistory];
				m_CursorPos = m_Caption.size();
				m_DispBegin = 0;
				m_SelBegin=m_CursorPos;

				if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
				{
					CaptionToValue();
				}

			}
		}
		break;

	default:
		bDefault=true;

	}
	//add by lincoln [2007:4:1] [13:45]
	//{{{����ؼ��Ľ�
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	//}}}

	if(bDefault) return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);


	return 1;
}

int  CUILineEdit::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{

	switch(msgtype)
	{
	
	case NTFTYPE_SPINARROW:
		if((int)(wparam) & WGTSTYLE_PRIMARY) 		
		SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,m_Value+m_Step,0));
		else 		
		SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,m_Value-m_Step,0));
	
		break;

	default:
		 return CUIWidget::OnNotify( msgtype, wparam, lparam );

	}

	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;


}

int CUILineEdit::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{

	switch(msgtype)
	{
	case MSGTYPE_CHAR:		 
	     return	OnChar( wparam, lparam );
		 break;

	case MSGTYPE_KEYDOWN:		 
		 return OnKeyDown( wparam, lparam );
		 break;

	case MSGTYPE_MOUSEMOVE:
		{
			 int x=LOWORD(lparam);
			 int y=HIWORD(lparam);

			 if( m_State & WGTSTATE_CLICKDOWN )
			 {	

				stRect abscaprect=GetAbsoluteCaptionRect();
				int width=x-abscaprect.ox;

				//Edit���ڵ��Զ�����
				if(width<0) 
				{
					if(m_CursorPos>=2&&IsDBCSCharHead(m_Caption.c_str(),m_CursorPos-2)) 
						m_CursorPos -= 2;
					else if(m_CursorPos>0) 
						m_CursorPos -= 1;					
				}
				else if(width>abscaprect.width) 
				{
				
					if( m_CursorPos<m_Caption.size()&&IsDBCSCharHead(m_Caption.c_str(), m_CursorPos) ) 
						m_CursorPos += 2;
					else if( m_CursorPos<m_Caption.size())
						m_CursorPos += 1;

				}
				else 
				{
					int numChar=UI_GetNumPrintChar(m_Caption.c_str()+m_DispBegin,width,m_TextSize);
				
					m_CursorPos= numChar+m_DispBegin;
				 
				}

				CheckForReason();					

			 } 
		}
		 break;

	case MSGTYPE_LEFTBTNUP:
		
		 if( m_State & WGTSTATE_CLICKDOWN )
		 {	
			m_State &= ~WGTSTATE_CLICKDOWN;	
			
			CallSafeScript(SCRIPT_FUNC_ON_CLICK);
					
		 }	
		 

		 break;


	case MSGTYPE_LBTNDBLCLK:
		{				
			SelectAllText();
			
			CallSafeScript(SCRIPT_FUNC_ON_DBCLICK);	
		}
		break;


	case MSGTYPE_LEFTBTNDOWN:
		{
			m_State |= WGTSTATE_CLICKDOWN;	

			stRect abscaprect=GetAbsoluteCaptionRect();
			int width=LOWORD(lparam)-abscaprect.ox;
			
			if(width<0) width=0;
			else if(width>abscaprect.width) width=abscaprect.width;
			
			int numChar=UI_GetNumPrintChar(m_Caption.c_str()+m_DispBegin,width,m_TextSize);
			
			m_CursorPos= numChar+m_DispBegin;

			if(m_CursorPos>m_Caption.size())
				m_CursorPos=m_Caption.size();
			
			m_SelBegin=m_CursorPos;

		}
		break;


	
	case MSGTYPE_LOSTFOCUS:
		{
			if(!(m_ExtendStyle&EDIT_STYLE_KEEPSEL))
				CancelSel(true,false);
		}
		break;	

	default:
		 return CUIWidget::OnMessage( msgtype, wparam, lparam );

	}

	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
}



//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIRichText---------------------------------------
//-------------------------------------------------------------------------------------------
CUIRichText::CUIRichText()
{
}


CUIRichText::~CUIRichText()
{

}

void CUIRichText::ParseTxt(const char* pBuf,int richMode,int richflag)
{

	m_RichChars.clear();
	
	for(int i=0;i<strlen(pBuf);i++)
	{
		unsigned char rawChar=pBuf[i];

		//�س����Ʒ���
		if(rawChar=='\n')
		{
			stRichChar richChar;
			richChar.type=RICH_CHAR_LF;
			richChar.keybuf[0]=rawChar;

			m_RichChars.push_back(richChar);
		}
		//Ӣ�Ļ����Ǻ��ֵĺ��ֽ�
		else if(rawChar<=128)
		{		
			
			//���ǰ���а�����֣���ӵ�һ����������ĺ���
			if(IsHalfDBCSPre(m_RichChars.size()))
			{
				m_RichChars[m_RichChars.size()-1].type=RICH_CHAR_WORD;
				m_RichChars[m_RichChars.size()-1].keybuf[1]=rawChar;
			}
			//���ǰ��û�а�����֣�ֱ�Ӳ���һ��Ӣ��
			else if(IsPrintableWord(rawChar))
			{
				stRichChar richChar;
				richChar.type=RICH_CHAR_CHAR;
				richChar.keybuf[0]=rawChar;		
				
				//////////////////////////////////////////////////////////////////////////
				stRichChar prevChar;
				
				//ǰһ���ַ�
				if(!m_RichChars.empty()) 
					prevChar=m_RichChars[m_RichChars.size()-1];

				if(UI_IsKeyCharHead(richChar,prevChar))
				{
					
					stKeyChar keyData;
					strncpy(keyData.key,pBuf+i,UI_KEYCHAR_MAX);
					keyData.flag=richflag;
					
					RawExecCallBack(UI_Callback_IsKeyChar,(void*)&keyData);


					//����ҵ���Ч�����ַ�����������һ��ϲ�Ϊһ��rich char
					if(UI_MaskRichMode(keyData,richMode))
					{
						
						richChar=stRichChar(keyData);							
						
						if ( keyData.type == RICH_CHAR_FIELD )
						{
							i += keyData.keynumeat - 1;
						}
						else
							i+=keyData.keynum-1;
						
					}
					
					
				}

				m_RichChars.push_back(richChar);				 

				//////////////////////////////////////////////////////////////////////////	


			}

		}
		//���֣���ǰ���Ƿ��а������
		else if(rawChar>128)
		{

			//���ǰ���а�����֣���ӵ�һ����������ĺ���
			if(IsHalfDBCSPre(m_RichChars.size()))
			{
				
				m_RichChars[m_RichChars.size()-1].type=RICH_CHAR_WORD;
				m_RichChars[m_RichChars.size()-1].keybuf[1]=rawChar;
				
			}
			//���ǰ��û�а�����֣�ֱ�Ӳ���������
			else 
			{
				
				stRichChar richChar;
				richChar.type=RICH_CHAR_HALF;
				richChar.keybuf[0]=rawChar;			
					
				m_RichChars.push_back(richChar);
				
			}
			
		}

	}	

}


//add+jension 2006-2-14 9:46
//{{{����Ƶ���û�����������
bool CUIRichText::RemoveCtrlChar(int flag)
{
	if (0 == m_RichChars.size())
		return false;

	bool bRemove =false;
	std::vector<stRichChar>::iterator iter = m_RichChars.begin();
	while(iter!=m_RichChars.end())
	{
		if ((*iter).type==flag)
		{
			iter = m_RichChars.erase(iter);
			bRemove = true;
		}
		else
		{
			iter++;
		}
	}

	if (bRemove)
		return true;
	else
		return false;
}
//}}}


int CUIRichText::GetSize()
{
	return m_RichChars.size();
}


bool CUIRichText::IsHalfDBCSPre(int pos)
{
	
	bool bHalfDBCS=false;
	
	if(pos>0&&pos<=m_RichChars.size())
	{
		if(	(m_RichChars[pos-1].type==RICH_CHAR_HALF) )
		bHalfDBCS=true;
	}
	
	return bHalfDBCS;
}


void CUIRichText::GetRawString(std::string& strBuf)
{

	strBuf="";

	for(int i=0;i<m_RichChars.size();i++)
	{ 
		strBuf+=m_RichChars[i].keybuf;
	}

}

int CUIRichText::GetTextExtent(int start,int length,int textSize)
{
	
	int end=start+length;

	
	if(start>m_RichChars.size()) start=m_RichChars.size();
	else if(start<0) end=0;
	
	if(end>m_RichChars.size()) end=m_RichChars.size();
	else if(end<0) end=0;

	int ext=0;
	for(int i=start;i<end;i++)
	{ 
		ext+=UI_GetCharSize(m_RichChars[i],textSize).x;
	}
	
	return ext;
	
}


int CUIRichText::ParseKeyChar(stKeyChar& keyChar, int px,int py,int sx,  int sy, int width,int height,int size,int space,int style)
{
	
	int xstart=0;
	int ystart=0;

	int printW=width;
	int printH=height;

	//���޶����ӡ,�õ�ʵ�ʴ�ӡ�Ŀ��,�߶�
	//ָ�������ܹ���ӡȫ���ַ�
    if(EstimateLine(printW,printH,size,space)>=m_RichChars.size())
	{
		//ʵ�ʴ�ӡ��Ҫ�Ŀ��С��ָ�����
		if(printW<width)
		{
			if(style&dtLeft) xstart=0;
			else if(style&dtRight) xstart=width-printW;
			else if(style&dtCenter) xstart=(width-printW)*0.5;
		}

		//ʵ�ʴ�ӡ��Ҫ�ĸ߶�С��ָ���߶�
		if(printH<height)
		{
			if(style&dtTop) ystart=0;
			else if(style&dtBottom) ystart=height-printH;
			else if(style&dtVCenter) ystart=(height-printH)*0.5;
		}

	}
	
	
	stRect absCapRct=stRect(sx+xstart,sy+ystart,printW,printH);

	if(!absCapRct.IsPointIn(px,py)) return RICHCHAR_CMD_NONE;

	//ת��Ϊ��Ա�����������
	Point2D pt(px-absCapRct.ox,py-absCapRct.oy);


	int dispHgt=0;
	for(int i=0;i<m_RichLines.size();i++)
	{ 
			
		const stRichLine& theLine=m_RichLines[i];


		//�����ײ�
		if(dispHgt>=absCapRct.height) break;

		//δ��������
		if(dispHgt+theLine.nHeight+space>=0)
		{

			//�ҵ�������
			if((pt.y>=dispHgt)&&(pt.y<=dispHgt+m_RichLines[i].nHeight))
			{

				//��ʾ���
				int dispWid=0;
				for(int j=theLine.nFirst;j<=theLine.nLast;j++)
				{	
					if(j>=m_RichChars.size()) break;
					
					Point2D charSize=UI_GetCharSize(m_RichChars[j],size);									

					//�ҵ�������
					if((pt.x>=dispWid)&&(pt.x<=dispWid+charSize.x))
					{

						for(int k=j;k>=0;k--)
						{ 
							
							if(m_RichChars[k].type==RICH_CHAR_HLINK)
							{
								if(m_RichChars[k].index<=DEFAULT_HLINK_BASE)
									return RICHCHAR_CMD_NONE;

								m_RichChars[k].GetKeyChar(keyChar);
								
								RawExecCallBack(UI_Callback_ValidKeyChar,(void*)&keyChar);

								return keyChar.cmdid;
							}

						}

						return RICHCHAR_CMD_NONE;
						
					}

					dispWid+=charSize.x;
				}

				return RICHCHAR_CMD_NONE;

			}
		
		
		}

		dispHgt+=theLine.nHeight+space;

	}
	

	return RICHCHAR_CMD_NONE;

}


//���ƶ�������
void CUIRichText::DrawRichText( stRect rect,int size,int space,DWORD dwColor,DWORD dwMagic,DWORD dwLink,int style,int font,int state)
{


	stUIPen uiPen;
	uiPen.dwdraw=dwColor;
	uiPen.dwmagic=dwMagic;
	uiPen.dwlink=dwLink;
	uiPen.font=font;
	uiPen.style=style;
	uiPen.size=size;
	uiPen.state=state;

	stUIPen defaultPen;
	defaultPen.dwdraw=dwColor;	
	defaultPen.font=font;	
	defaultPen.style=style;
	

	//���浱ǰ����
	int oldfont=g_pDisplay->SetFont(uiPen.font);	

	int xstart=0;
	int ystart=0;


	int printW=rect.width;
	int printH=rect.height;

	//���޶����ӡ,�õ�ʵ�ʴ�ӡ�Ŀ��,�߶�
	//ָ�������ܹ���ӡȫ���ַ�
    if(EstimateLine(printW,printH,size,space,true)>=m_RichChars.size())
	{
		//ʵ�ʴ�ӡ��Ҫ�Ŀ��С��ָ�����
		if(printW<rect.width)
		{
			if(style&dtLeft) xstart=0;
			else if(style&dtRight) xstart=rect.width-printW;
			else if(style&dtCenter) xstart=(rect.width-printW)*0.5;
		}

		//ʵ�ʴ�ӡ��Ҫ�ĸ߶�С��ָ���߶�
		if(printH<rect.height)
		{
			if(style&dtTop) ystart=0;
			else if(style&dtBottom) ystart=rect.height-printH;
			else if(style&dtVCenter) ystart=(rect.height-printH)*0.5;
		}

	}
			
	int dispHgt=ystart;
	for(int i=0;i<m_RichLines.size();i++)
	{ 
			
		const stRichLine& theLine=m_RichLines[i];

		//�����ײ�
		if(dispHgt>=(ystart+printH)) break;		

		//��ʾ���
		int dispWid=xstart;
		for(int j=theLine.nFirst;j<=theLine.nLast;j++)
		{	

			if(j>=m_RichChars.size()) break;

			Point2D charSize=UI_GetCharSize(m_RichChars[j],size);					
			
			int offset=(theLine.nHeight-charSize.y)/2;
			
			UI_DrawChar(dispWid+rect.ox,dispHgt+offset+rect.oy,m_RichChars[j],uiPen,defaultPen,true);

			dispWid+=charSize.x;
		}

		dispHgt+=theLine.nHeight+space;

	}


	//�ָ���ǰfont
	g_pDisplay->SetFont(oldfont);

}

//���д�ӡ��width���޴�
//�����ӡ��height���޴�
//���ɴ�ӡ��width��height���޴�
//���޶����ӡ, width,height����ʵ�ʴ�ӡ���Ĳ����ַ���ռ������
//����ֵ:ʵ�ʴ�ӡ���ַ�����
int  CUIRichText::EstimateLine( int& width,int& height,int size,int space,bool bCalcLine)
{

	//��ǰ�ַ��Ĵ�ӡλ��
	int sx=0;
	int sy=0;

	int ex=0;
	int ey=0;
	
	//��ǰ������
	int nRow=0;
	int nCol=0;

	//ǰһ�е��к�
	int preRow=-1;

	//���������Ϣ
	if(bCalcLine) m_RichLines.clear();

	//��ǰ�е��и�
	int lineHeight=0;

	//�����Ƶ�һ���ַ��Ľ���λ��
	int eymax=0;
	int exmax=0;

	//ʵ�ʴ�ӡ���ַ�����
	int printNum=0;	

	for(int i=0;i<=m_RichChars.size();i++)
	{ 

		Point2D charSize;
		
		if(i<m_RichChars.size())
			charSize=UI_GetCharSize(m_RichChars[i],size);
		else
			charSize=Point2D(0,size);
		
		bool bNeedReturn=false;
		
		//��һ���ַ������ǻ���
		if(i>0)
		{
			//ǰһ���ַ�Ϊ���п��Ʒ�,����
			if(m_RichChars[i-1].type==RICH_CHAR_LF)
				bNeedReturn=true;

			//������ӡ��ȣ�����
			if( (sx+charSize.x)>width)
				bNeedReturn=true;
		}


		if(bNeedReturn)
		{
			sx=0;
			sy+=(lineHeight+space);
			
			nRow++;
			nCol=0;
			lineHeight=charSize.y;
		}				

		if(charSize.y>lineHeight) lineHeight=charSize.y;

		//������򳬳�������(�ײ��������ַ�����Ȼ��ӡ)
		if(sy>height) break;


		//��¼��ӡ����λ��
		ex=sx+charSize.x;		
		ey=sy+lineHeight;

		printNum++;
		if(ey>eymax) eymax=ey;
		if(ex>exmax) exmax=ex;
		
		if(bCalcLine)
		{

			//ǰ��һ�к͵�ǰ�кŲ�ͬ,��������һ��
			if(preRow!=nRow)
			{
				m_RichLines.push_back(stRichLine(i));

				preRow=nRow;		
			}

			
			//ÿ��ӡһ���ַ�(sx,sy)(nRow,nCol)
			stRichLine* pLine=&m_RichLines[m_RichLines.size()-1];		
			pLine->nLast=i;
			pLine->nHeight=lineHeight;
		}
			
		//�ı��ӡ��ʼλ��
		sx+=charSize.x;
		nCol++;
		
	}	

	width=exmax;
	height=eymax;

	return printNum;


}

//add+jension 2006-02-20
//{{{���촰�����ױ�����ʾ����
//�õ���i�����ݵĸ߶�
int CUIRichText::GetRichLineHeight(int i)
{
	if ( i<0 || i>=m_RichLines.size() )
		return 0;
	
	int height = m_RichLines[i].nHeight;
	if (height<0)
	{
		height = 0;
	}

	return height;
}
//}}}

//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIRichEdit---------------------------------------
//-------------------------------------------------------------------------------------------
CUIRichEdit::CUIRichEdit()
{
	m_Type=WGTTYPE_RICHEDIT;
}

CUIRichEdit::~CUIRichEdit()
{

	
}

void CUIRichEdit::ClearParameter()
{
	CUIWidget::ClearParameter();


	m_ExtendStyle=EDIT_STYLE_NONE;

	m_MaxChar = EDIT_MAX_CHAR_DEFAULT;

	m_CursorPos=0;
	m_DispBegin=0;
	m_SelBegin=0;

	m_CursorTimer = 0;
	m_CursorFlag = 1;
	m_CursorLive = 700;
	m_CursorDeath =400;
	
	m_NumHistory = 0;
	m_CurHistory = -1;

	m_Advertise="";

	//�ڵװ���
	m_BackColor = stColorQuad(0,0,0,255);
	m_DrawColor = stColorQuad(255,255,255,255);
	m_FocusColor= stColorQuad(255,255,255,255);
	m_EditSelColor=stColorQuad(0,0,255,128);
	m_GrayColor=stColorQuad(128,128,128,255);

	m_RichMode=UI_RICHMODE_ALL;;

	SetEditable(true);

	SetCaption("");
	

}


BOOL CUIRichEdit::IsAcceptIme()
{
	return IsEnabled();
}
	
BOOL CUIRichEdit::IsAcceptInput()
{
	return IsEnabled();
}


const char *CUIRichEdit::GetCaption()
{

	GetRawString(m_Caption,0,m_RichCaption.size());

	return m_Caption.c_str();

}

//����ͨ�ַ���ת��Ϊ��ʾrich�ַ���
//ȷ������������ַ�����
//ע�⣺rich edit����ʾ�ı���m_Captionû���κι�ϵ
void CUIRichEdit::SetCaption( const char *pname )
{
	
	Clear();

	for(int i=0;i<strlen(pname);i++)
	{
		InputRawChar(pname[i]);
	}
	
	MoveDispBegin(0);
	MoveSelBegin(m_CursorPos);
		
	
}

void CUIRichEdit::DrawAdvertise()
{

	if( m_Advertise.empty() ) return;

	stRect captionRect=GetAbsoluteCaptionRect();

	stColorQuad drawColor=GetCaptionColor();

	stColorQuad selColor=GetRenderColor(m_EditSelColor);
	stColorQuad magicColor=GetRenderColor(m_MagicColor);
	stColorQuad linkColor=GetRenderColor(m_LinkColor);

	if(!IsEnabled()) 
	{
		drawColor=GetRenderColor(m_GrayColor);
		magicColor=drawColor;
	}

	Point2D captionOffset(0,0);

	if((m_CurFrameState>=0)&&(m_CurFrameState<FRAME_STATE_MAX))
	{
		captionOffset=m_CaptionOffset[m_CurFrameState];
	}

	g_pDisplay->DrawTextEx( 
		captionRect.ox+captionOffset.x, 
		captionRect.oy+captionOffset.y, 
		captionRect.width, 
		captionRect.height, 
		m_Advertise.c_str(), 
		m_TextSize,m_RowSpace, 
		drawColor.c, 
		m_TextStyle );	

}

BOOL CUIRichEdit::IsCursorVisible()
{

	bool bCursorVis=false;		
	if(IsEnabled()&&(m_SelBegin==m_CursorPos))
	{
		if(g_pRootWgt->GetFocusWgt()==this)
		{	
			bCursorVis=true;	
		}
		else if(m_ExtendStyle&EDIT_STYLE_KEEPCURSOR)
		{			
			bCursorVis=true;			
		}
		else if(m_ExtendStyle&EDIT_STYLE_SMARTCURSOR)
		{	
			/*
			CUIWidget* pFocusWgt=g_pRootWgt->GetFocusWgt();
			
			if(pFocusWgt&&pFocusWgt->IsAcceptInput())
				bCursorVis=false;
			else
				bCursorVis=true;
			*/
			bCursorVis=(g_pRootWgt->GetHotWgt()==this);
		}

	}
	
	if(bCursorVis&&m_CursorFlag)
		return TRUE;
	else
		return FALSE;

}

void CUIRichEdit::DrawCaption()
{

	
	stRect captionRect=GetAbsoluteCaptionRect();

	stColorQuad drawColor=GetCaptionColor();

	stColorQuad selColor=GetRenderColor(m_EditSelColor);
	stColorQuad magicColor=GetRenderColor(m_MagicColor);
	stColorQuad linkColor=GetRenderColor(m_LinkColor);

	int wgtState=UI_ITEM_STATE_FOCUSED;

	if(!IsEnabled()) 
	{
		drawColor=GetRenderColor(m_GrayColor);
		magicColor=drawColor;
		wgtState=UI_ITEM_STATE_DEFAULT;
	}
	
	stUIPen uiPen;
	uiPen.dwdraw=drawColor.c;
	uiPen.dwmagic=magicColor.c;
	uiPen.dwlink=linkColor.c;
	uiPen.font=m_TextFont;
	uiPen.style=m_TextStyle;
	uiPen.size=m_TextSize;
	uiPen.state=wgtState;

	stUIPen defaultPen;
	defaultPen.dwdraw=drawColor.c;	
	defaultPen.font=m_TextFont;	
	defaultPen.style=m_TextStyle;

	//���浱ǰ����
	int oldfont=g_pDisplay->SetFont(uiPen.font);
	

	//ѡ��Χ
	int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
	int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;


	//ǰһ��rich�ַ��Ĵ�ӡ����λ��
	int ex=0;

	//ǰһ��rich�ַ��Ĵ�ӡ����yλ��
	int sy=0;
	
	//���λ��
	Point2D cursorPos(0,0);
	
	
	for(int i=0;i<m_RichCaption.size();i++)
	{ 
		
		if(i<m_DispBegin)
		{
			UI_DrawChar(captionRect.ox+ex,captionRect.oy,m_RichCaption[i],uiPen,defaultPen,false);
			
			continue;
		}


		Point2D charSize=UI_GetCharSize(m_RichCaption[i],m_TextSize);

		//������ӡ���򣬽���
		if( (ex+charSize.x)>m_CaptionRect.width ) break;

		int offset=(m_TextSize-charSize.y)/2;


		UI_DrawChar(captionRect.ox+ex,captionRect.oy+offset,m_RichCaption[i],uiPen,defaultPen);

		
		if((i>=selBegin)&&(i<selEnd))
		{
			g_pDisplay->DrawBar(captionRect.ox+ex,captionRect.oy,charSize.x,m_TextSize,selColor.c);
		}

		//��¼���λ��
		if(m_CursorPos==i)
		{
			cursorPos.x=ex;
			cursorPos.y=sy;
		}


		//��¼��ӡ����λ��
		ex+=charSize.x;

	}


	if(m_CursorPos==m_RichCaption.size())
	{		
		cursorPos.x=ex;
		cursorPos.y=sy;
	}


	//���ƹ��
	if(IsCursorVisible())
		g_pDisplay->DrawBar(captionRect.ox+cursorPos.x,captionRect.oy+cursorPos.y,1,m_TextSize,drawColor.c );



	

	//�ָ���ǰ����
	g_pDisplay->SetFont(oldfont);	


}



void CUIRichEdit::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel) 
	{

	case VALUE_CHANNEL_RANGE_END:
		uiVar.fValue = m_CursorPos; 
		break;

	case VALUE_CHANNEL_RANGE_BEGIN:
		uiVar.fValue = m_SelBegin;
		break;

	default:
		CUIWidget::GetVar(uiVar);
	
	}

}

void  CUIRichEdit::SetVar(stUIVar& uiVar)
{
	
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_RANGE_END: 
		{
			m_CursorPos=uiVar.fValue; 
			if(m_CursorPos<0) m_CursorPos=0;
			SetUpdateDirty(true);
		}
		break;

	case VALUE_CHANNEL_RANGE_BEGIN:
		{
			m_SelBegin=uiVar.fValue; 
			if(m_SelBegin<0) m_SelBegin=0;
			SetUpdateDirty(true);
		}
		break;

	case VALUE_CHANNEL_EDITHISTORY:
		AddStringToHistory(uiVar.sValue.c_str());	
		break;

	default:
		CUIWidget::SetVar(uiVar);

	}
		
}


void CUIRichEdit::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
	
	if( (pelement=pvar->Element("maxchar")) )	 m_MaxChar=pelement->Integer();
	
	if( (pelement=pvar->Element("selcolor")) )
	{
		int color[4];
		pelement->IntegerArray( color, 4 );
		SetEditSelColor( stColorQuad(color[0], color[1], color[2], color[3]) );
		
	}	

	if( (pelement=pvar->Element("graycolor")) )
	{
		int color[4];
		pelement->IntegerArray( color, 4 );
		SetGrayColor( stColorQuad(color[0], color[1], color[2], color[3]) );
	}
	
	if( (pelement=pvar->Element("advertise")) )	 m_Advertise=pelement->String();
	
}

//�ӵڼ���rich char��ʼ���ݹ�Ľ�m_RichCaption�е��ַ�������֯���������λ�õ�
void CUIRichEdit::MergeRichChar(int start/*=0*/)
{

	
	for(int i=start;i<m_RichCaption.size();i++)
	{
		
		
		stRichChar currChar;		
		stRichChar prevChar;

		currChar=m_RichCaption[i];
		
		if(i>0) 
			prevChar=m_RichCaption[i-1];

		if(UI_IsKeyCharHead(currChar,prevChar))
		{

			stKeyChar keyData;
			
			std::string tmp;
			GetRawString(tmp,i,UI_KEYCHAR_MAX);//����ַ���ƫ��
			strncpy(keyData.key,tmp.c_str(),UI_KEYCHAR_MAX);
			keyData.flag=m_RichFlag;
			
			ExecCallBackFunc(UI_Callback_IsKeyChar,(void*)&keyData);
			

			//����ҵ���Ч�����ַ�����������һ��ϲ�Ϊһ��rich char
			if(UI_MaskRichMode(keyData,m_RichMode))
			{
				stRichChar rChar(keyData);

				while(keyData.keynum>0)
				{
					keyData.keynum-=EraseRichChar(i);
				}
				
				InsertRichChar(i,rChar);

				MoveSelBegin(m_CursorPos);

				MergeRichChar(i+1);

			}
					
			
		}		

	

	}





}

//posΪ�����captionrect������
//���ع��λ������
int CUIRichEdit::PosToChar(int pos)
{

	if(pos<=0) return 0;
	else if(pos>=m_CaptionRect.width) return m_RichCaption.size();
	else 
	{

		//pos�Ƿ���ǰһ��rich char�Ľ���λ�ú���
		bool bPre=true;
		
		//ǰһ��rich char�Ľ���λ��
		//��ǰrich char�Ŀ�ʼλ��
		int ext=0;

		int extpre=0;

		for(int i=m_DispBegin;i<m_RichCaption.size();i++)
		{ 

			//pos��ǰһ��rich char����ʼλ�ú���
			//����pos�ڵ�ǰrich char����ʼλ��ǰ��
			if( (pos<=ext) && bPre )
			{
				if(i==m_DispBegin)	return i;
				else if((pos-extpre)>(ext-pos)) return i;
				else return (i-1);
			}
			
			bPre=(pos>ext);

			extpre=ext;
			

			Point2D charSize=UI_GetCharSize(m_RichCaption[i],m_TextSize);
			ext+=charSize.x;

			

		}

		//�������λ��
		return i;



	}

}

int CUIRichEdit::GetTextExtent(int start,int length)
{

	int end=start+length;

	if(start>m_RichCaption.size()) start=m_RichCaption.size();
	if(start<0) start=0;

	if(end>m_RichCaption.size()) end=m_RichCaption.size();
	if(end<0) end=0;

	int ext=0;
	for(int i=start;i<end;i++)
	{ 
		Point2D charSize=UI_GetCharSize(m_RichCaption[i],m_TextSize);
		ext+=charSize.x;
	}
	
	return ext;


}

void CUIRichEdit::GetRawString(std::string& strBuf,int start,int length)
{

	if(start<0||start>=m_RichCaption.size()) start=0;

	if(length<0||(start+length)>m_RichCaption.size()) length=m_RichCaption.size()-start;

	strBuf="";

	for(int i=start;i<(start+length);i++)
	{ 
		strBuf+=m_RichCaption[i].keybuf;
	}

}


void CUIRichEdit::Clear()
{
	
	m_RichCaption.clear();

	MoveCursor(0);
	MoveDispBegin(0);	
	MoveSelBegin(0);

	
}



void CUIRichEdit::SelectAllText()
{

	SetSel(0,m_RichCaption.size());

}

int  CUIRichEdit::SetSel(int start,int end)
{
	if(start<0) start=0;
	else if(start>m_RichCaption.size()) start=m_RichCaption.size();
	
	if(end<0) end=0;
	else if(end>m_RichCaption.size()) end=m_RichCaption.size();
	
	MoveSelBegin(start);
	MoveCursor(end);
	
	
	return 1;

}

int  CUIRichEdit::ClearSel()
{

	//��ʷ�б�ص���ʼ
	m_CurHistory = -1;	

	if(m_SelBegin!=m_CursorPos)
	{
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		
		for(int i=selEnd-1;i>=selBegin;i--)
		EraseRichChar(i);
		
		MoveCursor(selBegin);
		MoveSelBegin(selBegin);
		
		SetUpdateDirty(true);

		return 1;
	}
	else return 0;

}


//bLeft:�Ƿ񽫹�꿿��
//bMove:�Ƿ��ƶ����
int  CUIRichEdit::CancelSel(bool bLeft,bool bMove)
{
	if(m_SelBegin!=m_CursorPos)
	{
		
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		
		if(bMove)
			MoveCursor((bLeft)?selBegin:selEnd);

		MoveSelBegin(m_CursorPos);
		
		
		return 1;
	}
	else return 0;

}


void CUIRichEdit::OnPaste()
{
	if(!IsEditable()) return;

	ClearSel();
	
	if( !::OpenClipboard(NULL) ) return;
	HANDLE handle = ::GetClipboardData(CF_TEXT);
	::CloseClipboard();
	if( !handle ) return;
	
	std::string  caption=(const char*)handle;
	if(caption.empty()) return;
			
	for(int i=0;i<caption.size();i++)
	{
		InputRawChar(caption[i]);
	}

	MoveSelBegin(m_CursorPos);

	SetUpdateDirty(true);

}

void CUIRichEdit::OnCopy()
{

	if(m_SelBegin!=m_CursorPos)
	{
		//�õ�ѡ���ı�
		
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		
		
		
		std::string selText;
		GetRawString(selText,selBegin,selEnd-selBegin);
		if(selText.empty()) return;
		
		
		//�õ������ڴ�
		HGLOBAL hGlobal=GlobalAlloc(GHND|GMEM_SHARE,selText.size()+1);
		if(!hGlobal) return;
		
		//��乲���ڴ�
		LPVOID pGlobal=GlobalLock(hGlobal);
		strcpy((char*)pGlobal,selText.c_str());
		GlobalUnlock(hGlobal);
		
		
		//���ü�����
		if( !::OpenClipboard(NULL) ) return;
		::EmptyClipboard();
		::SetClipboardData(CF_TEXT,hGlobal);
		::CloseClipboard();
		
	}		
	
	

}

void CUIRichEdit::OnCut()
{

	OnCopy();
	
	if(!IsEditable()) return;

	ClearSel();

	SetUpdateDirty(true);

}

void CUIRichEdit::OnInsert(const char* pText)
{

	if(!IsEditable()) return;

	if(strcmp(pText,""))
	{
		
		ClearSel();
		
		for(int i=0;i<strlen(pText);i++)
		{
			InputRawChar(pText[i]);
		}		
		
		MoveSelBegin(m_CursorPos);

		SetUpdateDirty(true);
	}

}


void CUIRichEdit::AddStringToHistory( const char* text)
{

	if(!text || !strcmp(text,"")) return;

	for( int i=UI_EDIT_HISTORY-1; i>0; i-- ) 
	m_History[i] = m_History[i-1];
	
	m_History[0] = text;
	if( m_NumHistory < UI_EDIT_HISTORY ) m_NumHistory++;
	m_CurHistory = -1;
}

//ѡ��ʼλ��pos��ʾ����һ����ѡ��rich�ַ�Ϊ��pos��rich�ַ�
//ע��selend����cursorpos����ʾ�����һ����ѡ���ַ�Ϊcusorpos-1
void CUIRichEdit::MoveSelBegin(int pos)
{
	if(pos<0) 
		pos=0;
	else if(pos>m_RichCaption.size()) 
		pos=m_RichCaption.size();
	

	m_SelBegin=pos;
}

//��ʾ��ʼλ��pos��ʾ����һ���ɼ�rich�ַ�Ϊ��pos��rich�ַ�
void CUIRichEdit::MoveDispBegin(int pos)
{

	if(pos<0) 
		pos=0;
	else if(pos>m_RichCaption.size()) 
		pos=m_RichCaption.size();
	
	m_DispBegin=pos;
	
}

//����λ��pos��ʾ������ڵ�pos��rich�ַ�ǰ��
void CUIRichEdit::MoveCursor(int pos)
{
	if(pos<0) 
		pos=0;
	else if(pos>m_RichCaption.size()) 
		pos=m_RichCaption.size();
	
	m_CursorPos=pos;

}


int CUIRichEdit::ParseMaxSize()
{
	int size=0;
		
	for(int i=0;i<m_RichCaption.size();i++)
	{ 
		size+=strlen(m_RichCaption[i].keybuf);
	}

	return size;
}

//��ָ��λ�ò���һ��rich char��ͬʱ���Զ��������
bool CUIRichEdit::InsertRichChar(int pos, stRichChar rChar)
{

	//��������ַ���Ŀ����
	if(ParseMaxSize()>=m_MaxChar) return false;
	
	
	if(pos>=0&&pos<=m_RichCaption.size())
	{

		if(m_CursorPos>=pos) m_CursorPos++;

		m_RichCaption.insert(m_RichCaption.begin()+pos,rChar);

		return true;
	}
	else 
	{
	 
		return false;
	}



}

//��ָ��λ��ɾ��һ��rich char��ͬʱ���Զ��������
//���ر�ɾ����rich char���ı��ֽ���Ŀ
int CUIRichEdit::EraseRichChar(int pos)
{
	
	if(pos>=0&&pos<m_RichCaption.size())
	{
		
		if(m_CursorPos>pos) m_CursorPos--;		
		
		int nResult=strlen(m_RichCaption[pos].keybuf);

		m_RichCaption.erase(m_RichCaption.begin()+pos);
		
		return nResult;
	}
	else 
	{
		
		return 0;
	}
	


}

bool CUIRichEdit::IsHalfDBCSPre(int pos)
{

	bool bHalfDBCS=false;
	
	if(pos>0&&pos<=m_RichCaption.size())
	{
		if(	(m_RichCaption[pos-1].type==RICH_CHAR_HALF)	)
		bHalfDBCS=true;
	}

	return bHalfDBCS;


}

//�ڹ��λ�ã�����rich char
//������������ַ������ǰ���Ƿ��ǰ������
//����У��ϲ�Ϊһ������
//���û�У���Ӱ���ַ�
//�Զ��������λ��
bool CUIRichEdit::InputRawChar(unsigned char rawChar)
{
	bool bNeedUpdate=false;

	//Ӣ�Ļ��ߺ��ֵĺ��ֽ�
	if(rawChar<=128)
	{

		//���ǰ���а�����֣���ӵ�һ����������ĺ���
		if(IsHalfDBCSPre(m_CursorPos))
		{

			m_RichCaption[m_CursorPos-1].type=RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]=rawChar;
			m_RichCaption[m_CursorPos-1].width=m_TextSize;

		}
		//���ǰ��û�а�����֣�ֱ�Ӳ���һ��Ӣ��
		else if(IsPrintableWord(rawChar))
		{

			stRichChar rChar;
			rChar.type=RICH_CHAR_CHAR;
			rChar.width=m_TextSize*0.5;
			rChar.keybuf[0]=rawChar;
			
			bNeedUpdate=InsertRichChar(m_CursorPos,rChar);
			
		}


	}
	//���֣���ǰ���Ƿ��а������
	else if(rawChar>128)
	{

		//���ǰ���а�����֣���ӵ�һ����������ĺ���
		if(IsHalfDBCSPre(m_CursorPos))
		{

			m_RichCaption[m_CursorPos-1].type=RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]=rawChar;
			m_RichCaption[m_CursorPos-1].width=m_TextSize;

		}
		//���ǰ��û�а�����֣�ֱ�Ӳ���������
		else 
		{

			stRichChar rChar;
			rChar.type=RICH_CHAR_HALF;
			rChar.width=m_TextSize*0.5;
			rChar.keybuf[0]=rawChar;
			
			bNeedUpdate=InsertRichChar(m_CursorPos,rChar);			
			
		}
 
	}



	return bNeedUpdate;


}


int CUIRichEdit::OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual)
{

	if(!IsEditable()) return 1;

	if(HasScriptFunc(SCRIPT_FUNC_ONCHAR))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONCHAR,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	//�Ǵ�ӡ�ַ�
	if(!IsPrintableWord(wparam)) return 0;

	
	ClearSel();
	SetUpdateDirty(true);

	//ȷ�������п�ʼ����ʱ�������ȷ�Ľṹ
	//����ֻ��Ҫparse����ͼ��Ϳ�����
	InputRawChar((unsigned char)wparam);

	MoveSelBegin(m_CursorPos);

		
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;

	
}


int CUIRichEdit::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{	

	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	SetUpdateDirty(true);

	switch( wparam )
	{
		

	case VK_BACK:
		if(IsEditable())
		{
			if(ClearSel()) break;
			EraseRichChar(m_CursorPos-1);
			if(m_DispBegin>0) MoveDispBegin(m_DispBegin-1);
			MoveSelBegin(m_CursorPos);

		}
		break;

	case VK_LEFT:
		
		//������ctrl��,ͬhome
		if((lparam&MK_CONTROL))
		{
			MoveCursor(0);
		}
		else 
		{
			
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			if(CancelSel(true))	break;
				
			MoveCursor(m_CursorPos-1);			
				
		}
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		MoveSelBegin(m_CursorPos);
		
		break;

	case VK_RIGHT:

		//������ctrl��,ͬend
		if((lparam&MK_CONTROL))
		{
			MoveCursor(m_RichCaption.size());
		}
		else 
		{
			
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			if(CancelSel(false))	break;
				
				
			MoveCursor(m_CursorPos+1);
				
		}
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		MoveSelBegin(m_CursorPos);

		
		break;

	case VK_HOME:

		MoveCursor(0);
		
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		MoveSelBegin(m_CursorPos);
		
		break;

	case VK_END:
		
		MoveCursor(m_RichCaption.size());
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		MoveSelBegin(m_CursorPos);
		
		break;

	case VK_DELETE:
		if(IsEditable())
		{
			if(ClearSel())	break;
			EraseRichChar(m_CursorPos);
		}
		break;

	case VK_UP:
		if(IsEditable())
		{
			if( m_CurHistory+1 < m_NumHistory )
			{
				m_CurHistory++;	
				
				SetCaption(m_History[m_CurHistory].c_str());				
			}
		}
		break;
		
	case VK_DOWN:
		if(IsEditable())
		{
			if( m_CurHistory > 0 )
			{
				m_CurHistory--;

				SetCaption(m_History[m_CurHistory].c_str());			
			}
		}
		break;

	default:

		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);

	}

//ȷ���༭�ռ���bug
#ifdef _DEBUG
	CheckForReason();
#endif

	return 1;
}

int CUIRichEdit::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{

	switch(msgtype)
	{
	case MSGTYPE_CHAR:		 
	     return	OnChar( wparam, lparam );
		 break;


	case MSGTYPE_KEYDOWN:		 
		 return OnKeyDown( wparam, lparam );
		 break;

	case MSGTYPE_MOUSEMOVE:
		{
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			if( m_State & WGTSTATE_CLICKDOWN )
			{	
				stRect abscaprect = GetAbsoluteCaptionRect();
				int width=LOWORD(lparam)-abscaprect.ox;

				int npos=0;				
				
				//Edit���ڵ��Զ�����
				if(width<0) 
				{
					npos=m_CursorPos-1;							
				}
				else if(width>abscaprect.width) 
				{
					npos=m_CursorPos+1;			
				}
				else 
				{
					npos=PosToChar(width);				
				}
				
				MoveCursor(npos);
				
				CheckForReason();
				AjustForReason();
				
			} 

		}
		break;
	
		
	case MSGTYPE_LBTNDBLCLK:	
		{				
			SelectAllText();
			
			CallSafeScript(SCRIPT_FUNC_ON_DBCLICK);	
		}
		break;

	case MSGTYPE_LEFTBTNUP:		
		if( m_State & WGTSTATE_CLICKDOWN )
		{	
			m_State &= ~WGTSTATE_CLICKDOWN;	
			
			CallSafeScript(SCRIPT_FUNC_ON_CLICK);
			
		}		
		break;	
		
	case MSGTYPE_LEFTBTNDOWN:		
		{
			m_State |= WGTSTATE_CLICKDOWN;	
			
			stRect abscaprect=GetAbsoluteCaptionRect();
			int width=LOWORD(lparam)-abscaprect.ox;
			
			if(width<0) width=0;
			else if(width>abscaprect.width) width=abscaprect.width;
			
			int npos=PosToChar(width);
			MoveCursor(npos);			
			MoveSelBegin(m_CursorPos);

		}
		break;


	case MSGTYPE_LOSTFOCUS:
		{
			if(!(m_ExtendStyle&EDIT_STYLE_KEEPSEL))
				CancelSel(true,false);
		}
		break;



	default:
		 return CUIWidget::OnMessage( msgtype, wparam, lparam );

	}

	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
}

//��������ָ��ķ�Χ
//ʹ���Ǳ����ں���ķ�Χ��
void CUIRichEdit::AjustForReason()
{

	if(m_CursorPos>m_RichCaption.size()) m_CursorPos=m_RichCaption.size();
	else if(m_CursorPos<0) m_CursorPos=0;
	
	if(m_SelBegin>m_RichCaption.size()) m_SelBegin=m_RichCaption.size();
	else if(m_SelBegin<0) m_SelBegin=0;
	
	if(m_DispBegin>m_RichCaption.size()) m_DispBegin=m_RichCaption.size();
	if(m_DispBegin<0) m_DispBegin=0;


	//����������ʾλ��֮ǰ��������ʾλ�ù��������λ��
	if( m_DispBegin > m_CursorPos ) MoveDispBegin(m_CursorPos);
	
	//�ӹ�굽��ʾ��ʼλ�ã���������ʾ���򣬿��ܴ�ӡ���ٸ�rich char
	while(GetTextExtent(m_DispBegin,m_CursorPos-m_DispBegin)>m_CaptionRect.width)
	{
		MoveDispBegin(m_DispBegin+1);
		
		if(m_DispBegin>m_RichCaption.size()) break;
	}


}


void CUIRichEdit::UpdateSelf( float deltatime )
{

	CheckForReason();

	//��˸���
	if((m_ExtendStyle&EDIT_STYLE_FLASHCURSOR)||(g_pRootWgt->GetFocusWgt() == this))
	{
		int dtick = int(deltatime*1000);
		m_CursorTimer += dtick;
		if( (m_CursorFlag&&m_CursorTimer>m_CursorLive) || (!m_CursorFlag&&m_CursorTimer>m_CursorDeath) )
		{
			m_CursorFlag = 1-m_CursorFlag;
			m_CursorTimer = 0;
		}
	}
	else 
	{
		m_CursorFlag=1;
		m_CursorTimer = 0;
	}



	//�ϲ���������ַ���
	if(IsUpdateDirty())
	{

		MergeRichChar();

		AjustForReason();

		SetUpdateDirty(false);

	}
		
	CUIWidget::UpdateSelf(deltatime);

}


void CUIRichEdit::CheckForReason()
{

	assert(m_SelBegin>=0);
	assert((m_CursorPos>=0)&&(m_CursorPos<=m_RichCaption.size()));
	assert(m_DispBegin>=0);
	
	//ȷ�����λ�ú���
	if(m_SelBegin<0)
		m_SelBegin=0;

	if(m_CursorPos<0)
		m_CursorPos=0;
	else if(m_CursorPos>m_RichCaption.size())
		m_CursorPos=m_RichCaption.size();

	if(m_DispBegin<0)
		m_DispBegin=0;
	else if(m_DispBegin>m_CursorPos)
		m_DispBegin=m_CursorPos;

}

int CUIRichEdit::ParseKeyChar(int x,int y,stKeyChar& keyChar)
{

		
	stRect captionRect=GetAbsoluteCaptionRect();

	//ǰһ��rich�ַ��Ĵ�ӡ����λ��
	int ex=0;

	//ǰһ��rich�ַ��Ĵ�ӡ����yλ��
	int sy=0;

	
	for(int i=m_DispBegin;i<m_RichCaption.size();i++)
	{ 
		stRichChar* pchar=&m_RichCaption[i];
		
		Point2D charSize=UI_GetCharSize(m_RichCaption[i],m_TextSize);
		
		//������ӡ���򣬽���
		if( (ex+charSize.x)>m_CaptionRect.width ) break;

			
		int offset=(m_TextSize-charSize.y)/2;
			
		stRect charRect(captionRect.ox+ex,captionRect.oy+offset,charSize.x,charSize.y);

	
		//�������ĳ��char����
		if(charRect.IsPointInside(x,y))
		{		

			for(int j=i;j>=0;j--)
			{ 
				
				if(m_RichCaption[j].type==RICH_CHAR_HLINK)
				{
					if(m_RichCaption[j].index<=DEFAULT_HLINK_BASE)
						return RICHCHAR_CMD_NONE;

					m_RichCaption[j].GetKeyChar(keyChar);

					ExecCallBackFunc(UI_Callback_ValidKeyChar,(void*)&keyChar);

					return keyChar.cmdid;
				}

			}

			return RICHCHAR_CMD_NONE;

		}


		//��¼��ӡ����λ��
		ex+=charSize.x;

	}

	return RICHCHAR_CMD_NONE;

}


//����edit����,�����ֻ��Ƶ�y����λ�ÿ�����captionrect������������
//��x����,���������
//���,edit���ڲ�֧��text align���
void CUIRichEdit::RenderSelf()
{

	CUIWidget::RenderSelf();


	if(m_RichCaption.empty()&&(!IsFocused())&&(!m_Advertise.empty()))
	{
		DrawAdvertise();
	}
	else
	{
		DrawCaption();
	}
	
}



//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIMultiEdit---------------------------------------
//-------------------------------------------------------------------------------------------
CUIMultiEdit::CUIMultiEdit()
{
	m_Type=WGTTYPE_MULTIEDIT;
}

CUIMultiEdit::~CUIMultiEdit()
{
	
}

void CUIMultiEdit::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_MaxChar = EDIT_MAX_CHAR_DEFAULT;

	m_ContentHeight=0;
	m_ContentPos=0;
	m_CurPage=0;

	m_CursorPos=0;	
	m_SelBegin=0;
	m_LinePos=0;

	m_CursorTimer = 0;
	m_CursorFlag = 1;
	m_CursorLive = 700;
	m_CursorDeath =400;
	
	//�ڵװ���
	m_BackColor = stColorQuad(0,0,0,255);
	m_DrawColor = stColorQuad(255,255,255,255);
	m_FocusColor = stColorQuad(255,255,255,255);
	m_EditSelColor=stColorQuad(0,0,255,128);
	m_GrayColor=stColorQuad(128,128,128,255);

	m_ReturnFlag=MEDIT_ENTER_STYLE_QQ;
	
	m_RichMode=UI_RICHMODE_ALL;

	m_Style  =  WGTSTYLE_EDITABLE|WGTSTYLE_CLIPSELF;
	m_ExtendStyle =EDIT_STYLE_KEEPSEL;

	SetEditable(true);

	SetCaption("");

}

BOOL CUIMultiEdit::IsAcceptIme()
{
	return IsEnabled();
}
	
BOOL CUIMultiEdit::IsAcceptInput()
{
	return IsEnabled();
}


//���scroll�Ǻ���,�Զ�ע��
void CUIMultiEdit::AddChild( CUIWidget *pwidget )
{
	
	CUIWidget::AddChild(pwidget);
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{

		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",this);
		
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",pScrollBar);

	}
	
	
}

void CUIMultiEdit::DelChild( CUIWidget *pwidget )
{
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{

		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",0);
		
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",0);

	}
	
	CUIWidget::DelChild(pwidget);
	
}

void CUIMultiEdit::OffsetContent(int offset)
{
	ScrollContent(m_ContentPos+offset);
}

//posΪ�����ԭʼλ�õ�ƫ��,ֻ���Ǹ�ֵ
void CUIMultiEdit::ScrollContent(int pos)
{		
	if(pos<(m_CaptionRect.height-m_ContentHeight)) 
		pos=(m_CaptionRect.height-m_ContentHeight);

	if(pos>0) 	pos=0;

	m_ContentPos=pos;//��ס��ǰƫ��

	m_CurPage=ceil(abs(m_ContentPos)/(m_CaptionRect.height*1.0));

	CUIScrollBar* pScrollBar=(CUIScrollBar*)GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pScrollBar) pScrollBar->SetScrollPos(m_ContentPos);
	
}


void CUIMultiEdit::UpdateScrollBar()
{

	CUIWidget* pBuddy;

	//���ݼ���ó����������߶���������ֱ������
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pBuddy;
			
		int nLower=m_CaptionRect.height-m_ContentHeight;
		if(nLower>0) nLower=0;
		
		pScrollBar->SetRange(nLower,0);
		
	}	
	
}

const char *CUIMultiEdit::GetCaption()
{

	GetRawString(m_Caption,0,m_RichCaption.size());

	return m_Caption.c_str();

}

//����ͨ�ַ���ת��Ϊ��ʾrich�ַ���
//ȷ������������ַ�����
//ע�⣺multi edit����ʾ�ı���m_Captionû���κι�ϵ
void CUIMultiEdit::SetCaption( const char *pname )
{
	
	Clear();

	for(int i=0;i<strlen(pname);i++)
	{
		InputRawChar(pname[i]);
	}	

	MoveSelBegin(m_CursorPos);

	SetUpdateDirty(true);		
	
}


void CUIMultiEdit::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel) 
	{

	case VALUE_CHANNEL_PAGE_V:
		uiVar.fValue = m_CurPage; 
		break;

	case VALUE_CHANNEL_RANGE_END:
		uiVar.fValue = m_CursorPos; 
		break;

	case VALUE_CHANNEL_RANGE_BEGIN:
		uiVar.fValue = m_SelBegin;
		break;

	default:
		CUIWidget::GetVar(uiVar);
	
	}

}

//[2004:9:2] [15:32]
void CUIMultiEdit::SetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel) 
	{

	case VALUE_CHANNEL_RANGE_END: 
		{
			m_CursorPos=uiVar.fValue; 
			if(m_CursorPos<0) m_CursorPos=0;
			SetUpdateDirty(true);
		}
		break;

	case VALUE_CHANNEL_RANGE_BEGIN:
		{
			m_SelBegin=uiVar.fValue; 
			if(m_SelBegin<0) m_SelBegin=0;
			SetUpdateDirty(true);
		}
		break;

	default:
		CUIWidget::SetVar(uiVar);
	}

}

void CUIMultiEdit::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
	if( (pelement=pvar->Element("maxchar")) )	 m_MaxChar=pelement->Integer();
	if( (pelement=pvar->Element("returnflag")) ) m_ReturnFlag=pelement->Integer();
	
	if( (pelement=pvar->Element("selcolor")) )
	{
		int color[4];
		pelement->IntegerArray( color, 4 );
		SetEditSelColor( stColorQuad(color[0], color[1], color[2], color[3]) );
		
	}	

	if( (pelement=pvar->Element("graycolor")) )
	{
		int color[4];
		pelement->IntegerArray( color, 4 );
		SetGrayColor( stColorQuad(color[0], color[1], color[2], color[3]) );
	}
	
	
}



int CUIMultiEdit::GetTextExtent(int start,int length)
{

	int end=start+length;

	if(start>m_RichCaption.size()) start=m_RichCaption.size();
	if(start<0) start=0;

	if(end>m_RichCaption.size()) end=m_RichCaption.size();
	if(end<0) end=0;

	int ext=0;
	for(int i=start;i<end;i++)
	{ 
		Point2D charSize=UI_GetCharSize(m_RichCaption[i],m_TextSize);
		ext+=charSize.x;
	}
	
	return ext;


}

void CUIMultiEdit::GetRawString(std::string& strBuf,int start,int length)
{

	if(start<0||start>=m_RichCaption.size()) start=0;

	if(length<0||(start+length)>m_RichCaption.size()) length=m_RichCaption.size()-start;

	strBuf="";

	for(int i=start;i<(start+length);i++)
	{ 
		strBuf+=m_RichCaption[i].keybuf;
	}

}


void CUIMultiEdit::Clear()
{
	
	m_RichCaption.clear();
	m_RichLines.clear();

	m_CursorPos=0;
	m_SelBegin=0;
	m_LinePos=0;
	
}



void CUIMultiEdit::SelectAllText()
{

	SetSel(0,m_RichCaption.size());

}

int  CUIMultiEdit::SetSel(int start,int end)
{
	if(start<0) start=0;
	else if(start>m_RichCaption.size()) start=m_RichCaption.size();
	
	if(end<0) end=0;
	else if(end>m_RichCaption.size()) end=m_RichCaption.size();
	
	MoveSelBegin(start);
	MoveCursor(end);
	
	
	return 1;

}

int  CUIMultiEdit::ClearSel()
{

	if(m_SelBegin!=m_CursorPos)
	{
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		
		for(int i=selEnd-1;i>=selBegin;i--)
		EraseRichChar(i);
		
		MoveCursor(selBegin);
		MoveSelBegin(selBegin);
		
		SetUpdateDirty(true);

		return 1;
	}
	else return 0;

}


//bLeft:�Ƿ񽫹�꿿��
//bMove:�Ƿ��ƶ����
int  CUIMultiEdit::CancelSel(bool bLeft,bool bMove)
{
	if(m_SelBegin!=m_CursorPos)
	{
		
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		
	
		if(bMove)
			MoveCursor((bLeft)?selBegin:selEnd);

		MoveSelBegin(m_CursorPos);
		
		
		return 1;
	}
	else return 0;

}


void CUIMultiEdit::OnPaste()
{

	if(!IsEditable()) return;

	ClearSel();
	
	if( !::OpenClipboard(NULL) ) return;
	HANDLE handle = ::GetClipboardData(CF_TEXT);
	::CloseClipboard();
	if( !handle ) return;
	
	std::string  caption=(const char*)handle;
			
	for(int i=0;i<caption.size();i++)
	{

		//���������������
		if(ParseMaxSize()>=m_MaxChar) 
		{
			CallSafeScriptEx(SCRIPT_FUNC_ON_ERROR,"%d",WGTERROR_MEDIT_INSERT);
			break;
		}

		InputRawChar(caption[i]);
	}

	MoveSelBegin(m_CursorPos);
	
	SetUpdateDirty(true);

}

void CUIMultiEdit::OnCopy()
{

	if(m_SelBegin!=m_CursorPos)
	{
		//�õ�ѡ���ı�
		
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		
		
		
		std::string selText;
		GetRawString(selText,selBegin,selEnd-selBegin);
		if(selText.empty()) return;
		
		//�õ������ڴ�
		HGLOBAL hGlobal=GlobalAlloc(GHND|GMEM_SHARE,selText.size()+1);
		if(!hGlobal) return;
		
		//��乲���ڴ�
		LPVOID pGlobal=GlobalLock(hGlobal);
		strcpy((char*)pGlobal,selText.c_str());
		GlobalUnlock(hGlobal);
		
		
		//���ü�����
		if( !::OpenClipboard(NULL) ) return;
		::EmptyClipboard();
		::SetClipboardData(CF_TEXT,hGlobal);
		::CloseClipboard();
		
	}		
	
	

}

void CUIMultiEdit::OnCut()
{
	OnCopy();
	
	if(!IsEditable()) return;
	
	ClearSel();
	
	SetUpdateDirty(true);

}

void CUIMultiEdit::OnInsert(const char* pText)
{
	if(!IsEditable()) return;

	if(strcmp(pText,""))
	{
		
		ClearSel();
		
		//�Ƿ���λ�ò����ַ�
		int margin=m_MaxChar-ParseMaxSize();
		int space=strlen(pText);
			
		if(margin>=space)
		{

			for(int i=0;i<strlen(pText);i++)
			{
				InputRawChar(pText[i]);
			}		
			
			MoveSelBegin(m_CursorPos);
		}
		else
		{
			CallSafeScriptEx(SCRIPT_FUNC_ON_ERROR,"%d",WGTERROR_MEDIT_INSERT);
		}


		SetUpdateDirty(true);
	}

}

//����ǰѡ������Ĳ�����������Ϊָ������
//1.�õ�selend��������a
//2.��selend����������a���Ʒ�
//3.��selend����selbegin������ɾ��������Ʒ�
//4.���selbegin�����岻����������b������������Ʒ�b
void CUIMultiEdit::OnSetFont(int font)
{

	if(!IsEditable()) return;

	if(m_SelBegin==m_CursorPos)
		return;
	
	if(font==-1)
		font=m_TextFont;
		
	int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
	int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;

	int endfont=GetFontPos(selEnd);
	int beginfont=GetFontPos(selBegin-1);	
	

	//�õ�����λ����������ַ���
	stKeyChar endKey;
	endKey.type=RICH_CHAR_FONT;
	endKey.index=endfont;
	ExecCallBackFunc(UI_Callback_GetKeyChar,(void*)&endKey);

	stRichChar endChar(endKey);


	//�õ���ʼλ����������ַ���
	stKeyChar beginKey;
	beginKey.type=RICH_CHAR_FONT;
	beginKey.index=font;
	ExecCallBackFunc(UI_Callback_GetKeyChar,(void*)&beginKey);

	stRichChar beginChar(beginKey);

	//�Ƿ���λ�ò�������ַ�
	int margin=m_MaxChar-ParseMaxSize();
	int space=strlen(beginKey.key)+strlen(endKey.key);
		
	if(margin>=space)	
	{

		if((font!=endfont))
			InsertRichChar(selEnd,endChar);

		for(int i=selEnd-1;i>=selBegin;i--)
		{ 
			if(m_RichCaption[i].type==RICH_CHAR_FONT)
			{
				m_RichCaption.erase(m_RichCaption.begin()+i);
				ExtendSelBackward(-1);
			}
		}		
		
		if((font!=beginfont))
			InsertRichChar(selBegin,beginChar);
		
	}
	else
	{
		CallSafeScriptEx(SCRIPT_FUNC_ON_ERROR,"%d",WGTERROR_MEDIT_FONT);
	}
	

	SetUpdateDirty(true);

}

//����ǰѡ������Ĳ�����������Ϊָ����ɫ
//1.�õ�selend������ɫa
//2.��selend��������ɫa���Ʒ�
//3.��selend����selbegin������ɾ����ɫ���Ʒ�
//4.��selbegin��������ɫ���Ʒ�b
void CUIMultiEdit::OnSetColor(int color)
{

	if(!IsEditable()) return;

	if(m_SelBegin==m_CursorPos)
		return;
		
	int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
	int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;

	int endcolor=GetColorPos(selEnd);
	int begincolor=GetColorPos(selBegin-1);

	//�õ�����λ����ɫ���Ʒ�
	stKeyChar endKey;
	endKey.type=RICH_CHAR_COLOR;
	endKey.index=endcolor;
	ExecCallBackFunc(UI_Callback_GetKeyChar,(void*)&endKey);

	stRichChar endChar(endKey);

	//�õ���ʼλ����ɫ���Ʒ�
	stKeyChar beginKey;
	beginKey.type=RICH_CHAR_COLOR;
	beginKey.index=color;
	ExecCallBackFunc(UI_Callback_GetKeyChar,(void*)&beginKey);

	stRichChar beginChar(beginKey);


	//�Ƿ���λ�ò�������ַ�
	int margin=m_MaxChar-ParseMaxSize();
	int space=strlen(beginKey.key)+strlen(endKey.key);
		
	if(margin>=space)	
	{
	
		if(color!=endcolor)
			InsertRichChar(selEnd,endChar);

		for(int i=selEnd-1;i>=selBegin;i--)
		{ 
			if(m_RichCaption[i].type==RICH_CHAR_COLOR)
			{
				m_RichCaption.erase(m_RichCaption.begin()+i);
				ExtendSelBackward(-1);
			}
		}

		if(color!=begincolor)
			InsertRichChar(selBegin,beginChar);		

	}
	else
	{
		CallSafeScriptEx(SCRIPT_FUNC_ON_ERROR,"%d",WGTERROR_MEDIT_COLOR);
	}

	SetUpdateDirty(true);

}

int CUIMultiEdit::GetColorPos()
{
	return GetColorPos((m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos);
}


int CUIMultiEdit::GetFontPos()
{
	return GetFontPos((m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos);
}

//�õ�ָ��λ���ַ�����ɫ
int CUIMultiEdit::GetColorPos(int pos)
{
	
	//Ĭ����ɫ
	DWORD color=DEFAULT_COLOR_BASE;

	if(pos>=m_RichCaption.size())
		pos=m_RichCaption.size()-1;

	for(int i=pos;i>=0;i--)
	{ 
		if(m_RichCaption[i].type==RICH_CHAR_COLOR)
		{
			color=m_RichCaption[i].index;
			break;
		}
	}

	return color;

}

//�õ�ָ��λ���ַ�������
int CUIMultiEdit::GetFontPos(int pos)
{
	//Ĭ������
	int font=DEFAULT_COLOR_BASE;

	if(pos>=m_RichCaption.size())
		pos=m_RichCaption.size()-1;

	for(int i=pos;i>=0;i--)
	{ 
		if(m_RichCaption[i].type==RICH_CHAR_FONT)
		{
			font=m_RichCaption[i].index;
			break;
		}
	}

	if(font==-1)
		font=m_TextFont;

	return font;
}


void CUIMultiEdit::ExtendSelBackward(int step)
{

	if(m_SelBegin<m_CursorPos)
	{
		MoveCursor(m_CursorPos+step);	 
	}
	else
	{
		MoveSelBegin(m_SelBegin+step);
	}
}

void CUIMultiEdit::ExtendSelForward(int step)
{
	if(m_SelBegin<m_CursorPos)
	{
		MoveSelBegin(m_SelBegin-step);
	}
	else
	{
		MoveCursor(m_CursorPos-step);	 
	}
}



//����λ��pos��ʾ������ڵ�pos��rich�ַ�ǰ��
void CUIMultiEdit::MoveCursor(int pos)
{
	if(pos<0) pos=0;
	else if(pos>m_RichCaption.size()) 
		pos=m_RichCaption.size();
	

	m_CursorPos=pos;

}


//ѡ��ʼλ��pos��ʾ����һ����ѡ��rich�ַ�Ϊ��pos��rich�ַ�
//ע��selend����cursorpos����ʾ�����һ����ѡ���ַ�Ϊcusorpos-1
void CUIMultiEdit::MoveSelBegin(int pos)
{
	if(pos<0) pos=0;
	else if(pos>m_RichCaption.size()) 
		pos=m_RichCaption.size();
	
		
	m_SelBegin=pos;

}


void CUIMultiEdit::MoveLinePos(int pos)
{
	
	if(pos>=m_RichLines.size()) 
		pos=m_RichLines.size()-1;

	if(pos<0) pos=0;
	
	m_LinePos=pos;

}

//�õ�raw string��size
int CUIMultiEdit::ParseMaxSize()
{
	
	int size=0;
		
	for(int i=0;i<m_RichCaption.size();i++)
	{ 
		size+=strlen(m_RichCaption[i].keybuf);
	}

	return size;
}


bool CUIMultiEdit::IsHalfDBCSPre(int pos)
{

	bool bHalfDBCS=false;
	
	if(pos>0&&pos<=m_RichCaption.size())
	{
		if(	(m_RichCaption[pos-1].type==RICH_CHAR_HALF)	)
		bHalfDBCS=true;
	}

	return bHalfDBCS;


}

//��ָ��λ�ò���һ��rich char��ͬʱ���Զ��������
bool CUIMultiEdit::InsertRichChar(int pos, stRichChar rChar)
{

	//��������ַ���Ŀ����
	if(ParseMaxSize()>=m_MaxChar) 
	{
		return false;
	}

	if(pos>=0&&pos<=m_RichCaption.size())
	{

		if(pos<=m_SelBegin)  m_SelBegin++;

		if(pos<=m_CursorPos) m_CursorPos++;

		m_RichCaption.insert(m_RichCaption.begin()+pos,rChar);

		return true;
	}
	else 
	{
		return false;
	}



}

//��ָ��λ��ɾ��һ��rich char��ͬʱ���Զ��������
//���ر�ɾ����rich char���ı��ֽ���Ŀ
int CUIMultiEdit::EraseRichChar(int pos)
{
	
	if(pos>=0&&pos<m_RichCaption.size())
	{
		
		if(m_CursorPos>pos) m_CursorPos--;		
		
		int nResult=strlen(m_RichCaption[pos].keybuf);

		m_RichCaption.erase(m_RichCaption.begin()+pos);
		
		return nResult;
	}
	else 
	{
		
		return 0;
	}

}

//�ڹ��λ�ã�����rich char
//������������ַ������ǰ���Ƿ��ǰ������
//����У��ϲ�Ϊһ������
//���û�У���Ӱ���ַ�
//�Զ��������λ��
bool CUIMultiEdit::InputRawChar(unsigned char rawChar)
{
	bool bNeedUpdate=false;

	//���з���
	if(rawChar=='\n')
	{

		stRichChar rChar;
		rChar.type=RICH_CHAR_LF;
		rChar.width=0;
		rChar.keybuf[0]=rawChar;
		
		bNeedUpdate=InsertRichChar(m_CursorPos,rChar);

	}
	//Ӣ�Ļ��ߺ��ֺ��ֽ�
	else if(rawChar<=128)
	{
		//���ǰ���а�����֣���ӵ�һ����������ĺ���
		if(IsHalfDBCSPre(m_CursorPos))
		{

			m_RichCaption[m_CursorPos-1].type=RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]=rawChar;
			m_RichCaption[m_CursorPos-1].width=m_TextSize;

		}
		//���ǰ��û�а�����֣�ֱ�Ӳ���Ӣ��
		else if(IsPrintableWord(rawChar))
		{
			stRichChar rChar;
			rChar.type=RICH_CHAR_CHAR;
			rChar.width=m_TextSize*0.5;
			rChar.keybuf[0]=rawChar;

			bNeedUpdate=InsertRichChar(m_CursorPos,rChar);			
		}
 

	}
	//���֣���ǰ���Ƿ��а������	
	else if(rawChar>128)
	{

		
		//���ǰ���а�����֣���ӵ�һ����������ĺ���
		if(IsHalfDBCSPre(m_CursorPos))
		{

			m_RichCaption[m_CursorPos-1].type=RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]=rawChar;
			m_RichCaption[m_CursorPos-1].width=m_TextSize;

		}
		//���ǰ��û�а�����֣�ֱ�Ӳ���������
		else 
		{

			stRichChar rChar;
			rChar.type=RICH_CHAR_HALF;
			rChar.width=m_TextSize*0.5;
			rChar.keybuf[0]=rawChar;
			
			bNeedUpdate=InsertRichChar(m_CursorPos,rChar);			
			
		}
 
	}



	return bNeedUpdate;


}


//�ӵڼ���rich char��ʼ���ݹ�Ľ�m_RichCaption�е��ַ�������֯���������λ�õ�
void CUIMultiEdit::MergeRichChar(int start/*=0*/)
{

	for(int i=start;i<m_RichCaption.size();i++)
	{
		
		
		stRichChar currChar;		
		stRichChar prevChar;

		currChar=m_RichCaption[i];
		
		if(i>0) 
			prevChar=m_RichCaption[i-1];

		if(UI_IsKeyCharHead(currChar,prevChar))
		{

			stKeyChar keyData;
			
			std::string tmp;
			GetRawString(tmp,i,UI_KEYCHAR_MAX);//����ַ���ƫ��
			strncpy(keyData.key,tmp.c_str(),UI_KEYCHAR_MAX);
			keyData.flag=m_RichFlag;

			ExecCallBackFunc(UI_Callback_IsKeyChar,(void*)&keyData);
		

			//����ҵ���Ч�����ַ�����������һ��ϲ�Ϊһ��rich char
			if(UI_MaskRichMode(keyData,m_RichMode))
			{
				stRichChar rChar(keyData);

				while(keyData.keynum>0)
				{
					keyData.keynum-=EraseRichChar(i);
				}
				
				InsertRichChar(i,rChar);
				
				MoveSelBegin(m_CursorPos);			

				MergeRichChar(i+1);

			}
					
			
		}		

	

	}





}

int CUIMultiEdit::OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual)
{

	if(!IsEditable()) return 1;

	if(HasScriptFunc(SCRIPT_FUNC_ONCHAR))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONCHAR,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	//�Ǵ�ӡ�ַ�
	if(!IsPrintableWord(wparam)) return 0;
	
	ClearSel();
	
	
	//ȷ�������п�ʼ����ʱ�������ȷ�Ľṹ
	//����ֻ��Ҫparse����ͼ��Ϳ�����
	//���������������
	if(ParseMaxSize()>=m_MaxChar) 
		CallSafeScriptEx(SCRIPT_FUNC_ON_ERROR,"%d",WGTERROR_MEDIT_INSERT);
	else
		InputRawChar((unsigned char)wparam);

	SetUpdateDirty(true);	

	MoveSelBegin(m_CursorPos);
		
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;

	
}


int CUIMultiEdit::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{

	case VK_RETURN:
		{
			if(m_ReturnFlag==MEDIT_ENTER_STYLE_QQ)
			{
				//������ctrl��
				if((lparam&MK_CONTROL))
				{				
					return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
				}
				else if(IsEditable())
				{

					ClearSel();

					InputRawChar('\n');					
					
					MoveSelBegin(m_CursorPos);

					SetUpdateDirty(true);	

				}

			}
			else if(m_ReturnFlag==MEDIT_ENTER_STYLE_MSN)
			{

				//������ctrl��
				if((lparam&MK_CONTROL))
				{	
										
					if(IsEditable())
					{
						ClearSel();

						InputRawChar('\n');
										
						MoveSelBegin(m_CursorPos);				

						SetUpdateDirty(true);	
					}
					
				}
				else
				{
					return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
				}

			}
			else if(m_ReturnFlag==MEDIT_ENTER_STYLE_EDIT)
			{
				if(IsEditable())
				{
					ClearSel();

					InputRawChar('\n');
				
					MoveSelBegin(m_CursorPos);

					SetUpdateDirty(true);	
					
				}
			}
			else if(m_ReturnFlag==MEDIT_ENTER_STYLE_RETURN)
			{
				return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
			}

		}
		break;

	case VK_BACK:
		if(IsEditable())
		{			
			if(!ClearSel())
			{
				EraseRichChar(m_CursorPos-1);
				MoveSelBegin(m_CursorPos);

				SetUpdateDirty(true);	
			}
		}
		break;

	case VK_DELETE:
		if(IsEditable())
		{		
			if(!ClearSel())
			{
				EraseRichChar(m_CursorPos);
				SetUpdateDirty(true);	
			}
		}
		break;

	case VK_LEFT:
		{
			
			//������ctrl��
			if((lparam&MK_CONTROL))
			{
				MoveCursorLineHome();
			}
			else 
			{
				
				//û�а���shift��
				if((lparam&MK_SHIFT)==0)
				if(CancelSel(true))	break;
					
				MoveCursorLeft();		
					
			}
			
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			MoveSelBegin(m_CursorPos);
			
		}
		break;

	case VK_RIGHT:

		//������ctrl��
		if((lparam&MK_CONTROL))
		{
			MoveCursorLineEnd();			
		}
		else 
		{
			
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			if(CancelSel(false))	break;
				
				
			MoveCursorRight();
				
		}
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		MoveSelBegin(m_CursorPos);

		
		break;

	case VK_HOME:

		//������ctrl��
		if((lparam&MK_CONTROL))
		{
			MoveCursorHome();
		}
		else 
		{
			
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			if(CancelSel(false))	break;

			MoveCursorLineHome();			
				
		}
		
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		MoveSelBegin(m_CursorPos);
		
		break;

	case VK_END:
		
		//������ctrl��
		if((lparam&MK_CONTROL))
		{
			MoveCursorEnd();
		}
		else 
		{
			
			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			if(CancelSel(false))	break;
				
				
			MoveCursorLineEnd();
			
		}
		
		//û�а���shift��
		if((lparam&MK_SHIFT)==0)
		MoveSelBegin(m_CursorPos);
		
		break;
	

	case VK_UP:
		{

			MoveCursorUp();

			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			MoveSelBegin(m_CursorPos);

		}
		break;
		
	case VK_DOWN:
		{
			
			MoveCursorDown();

			//û�а���shift��
			if((lparam&MK_SHIFT)==0)
			MoveSelBegin(m_CursorPos);
			
		}
		break;

	default:

		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);

	}

	return 1;
}


int  CUIMultiEdit::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_SCROLLBAR_POS:
		{

			CUIScrollBar* pBar=(CUIScrollBar*)lparam;
			if(pBar)	ScrollContent((int)wparam);	
		}
		break;
		
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}
int CUIMultiEdit::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{

	switch(msgtype)
	{
	case MSGTYPE_CHAR:		 
	     return	OnChar( wparam, lparam );
		 break;


	case MSGTYPE_KEYDOWN:
		 return OnKeyDown( wparam, lparam );
		 break;
		
	case MSGTYPE_LBTNDBLCLK:
		{				
			SelectAllText();
			
			CallSafeScript(SCRIPT_FUNC_ON_DBCLICK);	
		}		
		break;

	case MSGTYPE_LEFTBTNUP:		
		if( m_State & WGTSTATE_CLICKDOWN )
		{	
			m_State &= ~WGTSTATE_CLICKDOWN;	
			
			CallSafeScript(SCRIPT_FUNC_ON_CLICK);			
		}		
		break;	
		
	case MSGTYPE_LEFTBTNDOWN:		
		{
			m_State |= WGTSTATE_CLICKDOWN;	
			
			stRect capRect=GetAbsoluteCaptionRect();
				
			Point2D pos(LOWORD(lparam),HIWORD(lparam));
			
			if(capRect.IsPointInside(pos.x,pos.y))
			{
				pos.x-=capRect.ox;
				pos.y-=capRect.oy;
				
				MoveCursorOnPoint(pos);	
				
			}
			
			MoveSelBegin(m_CursorPos);
		}
		break;

	case MSGTYPE_MOUSEMOVE:
		{
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);		

			if( m_State & WGTSTATE_CLICKDOWN )
			{
				stRect capRect=GetAbsoluteCaptionRect();
				
				Point2D pos(LOWORD(lparam),HIWORD(lparam));

				if(capRect.IsPointInside(pos.x,pos.y))
				{
					pos.x-=capRect.ox;
					pos.y-=capRect.oy;

					MoveCursorOnPoint(pos);			
					
				}		
				//Edit���ڵ��Զ�����
				else
				{

					if(pos.x<capRect.ox)//���
					{
						pos.x=capRect.ox;
					}
					else if(pos.x>capRect.ox+capRect.width)
					{
						pos.x=capRect.ox+capRect.width;
					}

					if(pos.y<capRect.oy)//�ϱ�
					{
						pos.y=capRect.oy-m_TextSize;
					}
					else if(pos.y>capRect.oy+capRect.height)
					{
						pos.y=capRect.oy+capRect.height+m_TextSize;
					}

					pos.x-=capRect.ox;
					pos.y-=capRect.oy;

					MoveCursorOnPoint(pos);		
					
					CheckForReason();					
				}
				
				
			} 
		}
		break;
	
	case MSGTYPE_MOUSEWHEEL:
		{
			short offset=HIWORD(wparam);

			if(offset>0) OffsetContent(m_MouseDelta);	
			else OffsetContent(-m_MouseDelta);
		}
		break;

	case MSGTYPE_LOSTFOCUS:
		{
			if(!(m_ExtendStyle&EDIT_STYLE_KEEPSEL))
				CancelSel(true,false);
		}
		break;	


	default:
		 return CUIWidget::OnMessage( msgtype, wparam, lparam );

	}

	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
}

void  CUIMultiEdit::EnsureCursorVisible()
{
	//�õ����ʵ������
	Point2D charGrid=GetCursorGrid();

	int dispHgt=0;
	for(int row=0;row<m_RichLines.size();row++)
	{ 
		
		const stRichLine& theLine=m_RichLines[row];

		if(row==charGrid.x)
		{
			//��������
			if(dispHgt+m_ContentPos<0)
			{
				int offset=dispHgt+m_ContentPos;
				OffsetContent(-offset);
			}
			else if(dispHgt+theLine.nHeight+m_ContentPos>m_CaptionRect.height)
			{
				int offset=dispHgt+theLine.nHeight+m_ContentPos-m_CaptionRect.height;
				OffsetContent(-offset);
			}

			break;

		}

		dispHgt+=theLine.nHeight+m_RowSpace;

	}


}



Point2D CUIMultiEdit::GetCursorGrid()
{

	//�õ��������׵Ĺ������
	Point2D charGrid=PosToGrid(m_CursorPos);

	//���λ��������,���ڶ�����
	if(charGrid.y==0)
	{
		//����к͵�ǰ�в�ͬ
		if(m_LinePos<charGrid.x)
		{
			//ȡ��һ��
			if((charGrid.x>0)&&(charGrid.x<m_RichLines.size()))
			{
				charGrid.x--;
				
				const stRichLine& theLine=m_RichLines[charGrid.x];
				
				charGrid.y=(theLine.nLast-theLine.nFirst)+1;				

			}
		}

	}

	return charGrid;


}

//ptΪ�����captionrect������
void CUIMultiEdit::MoveCursorOnPoint(Point2D pt)
{

	//����ƶ�������һ��
	int nRow=HeightToRow(pt.y);
	if(nRow<0)
	{
		nRow=0;
		pt.x=0;
	}
	

	//�õ����ж�Ӧ��x���Ĺ������
	//����������һ���ַ����򷵻����һ����ͣ�����λ��
	int nIndex=WidthToChar(nRow,pt.x);

	if(nIndex>=0)
	{

		MoveCursor(nIndex);	

		MoveLinePos(nRow);

		CheckForReason();

		EnsureCursorVisible();

	}

}


void CUIMultiEdit::MoveCursorUp()
{

	//�õ����ʵ������
	Point2D charGrid=GetCursorGrid();

	int width=GridToWidth(charGrid.x,charGrid.y);
	if((width>=0)&&(charGrid.x>0)&&(charGrid.x<m_RichLines.size()))
	{
		int nIndex=WidthToChar(charGrid.x-1,width);

		if(nIndex>=0)
		{
			MoveCursor(nIndex);

			MoveLinePos(charGrid.x-1);

			CheckForReason();
		}

	}

	EnsureCursorVisible();

}

void CUIMultiEdit::MoveCursorDown()
{
	
	//�õ����ʵ������
	Point2D charGrid=GetCursorGrid();
			
	int width=GridToWidth(charGrid.x,charGrid.y);
	if((width>=0)&&(charGrid.x>=0)&&(charGrid.x<m_RichLines.size()-1))
	{
		int nIndex=WidthToChar(charGrid.x+1,width);

		if(nIndex>=0)
		{
			MoveCursor(nIndex);

			MoveLinePos(charGrid.x+1);

			CheckForReason();
		}

	}

	EnsureCursorVisible();

}

void CUIMultiEdit::MoveCursorLeft()
{

	//�õ����ʵ������
	Point2D charGrid=GetCursorGrid();

	stEditTick theTick(m_CursorPos-1,charGrid.x);

	//λ������
	if(charGrid.y==0)
	{
		//���滹��һ��
		if(charGrid.x>0)
		{
			theTick=GridToPos(charGrid.x-1,UI_MULTIEDIT_MAXCOL);			
		}

	}

	if((theTick.nPos>=0)&&(theTick.nRow>=0))
	{
		MoveCursor(theTick.nPos);

		MoveLinePos(theTick.nRow);	
		
		CheckForReason();
	}
	
	EnsureCursorVisible();

}

void CUIMultiEdit::MoveCursorRight()
{
	//�õ����ʵ������
	Point2D charGrid=GetCursorGrid();
	
	stEditTick theTick(m_CursorPos+1,charGrid.x);

	//�õ���������һ����ͣ��λ��
	stEditTick curTick=GridToPos(charGrid.x,UI_MULTIEDIT_MAXCOL);

	//λ����ĩ
	if(m_CursorPos==curTick.nPos)
	{
		//���滹��һ��
		if(charGrid.x<m_RichLines.size()-1)
		{
			theTick=GridToPos(charGrid.x+1,0);			
		}

	}

	if((theTick.nPos>=0)&&(theTick.nRow>=0))
	{
		MoveCursor(theTick.nPos);
		
		MoveLinePos(theTick.nRow);

		CheckForReason();
	}
	
	EnsureCursorVisible();

}

void CUIMultiEdit::MoveCursorLineHome()
{

	//�õ����ʵ������
	Point2D charGrid=GetCursorGrid();
	

	//�õ����������λ��
	stEditTick theTick=GridToPos(charGrid.x,0);

	MoveCursor(theTick.nPos);

	MoveLinePos(theTick.nRow);

	CheckForReason();

	EnsureCursorVisible();

}

void CUIMultiEdit::MoveCursorLineEnd()
{

	//�õ����ʵ������
	Point2D charGrid=GetCursorGrid();
	

	//�õ���������һ����ͣ��λ��
	stEditTick theTick=GridToPos(charGrid.x,UI_MULTIEDIT_MAXCOL);

	MoveCursor(theTick.nPos);
	
	MoveLinePos(theTick.nRow);
	
	CheckForReason();

	EnsureCursorVisible();
	

}

void CUIMultiEdit::MoveCursorHome()
{
	MoveCursor(0);

	MoveLinePos(0);

	CheckForReason();

	EnsureCursorVisible();
}

void CUIMultiEdit::MoveCursorEnd()
{

	MoveCursor(m_RichCaption.size());	

	MoveLinePos(m_RichLines.size()-1);	

	CheckForReason();

	EnsureCursorVisible();

}


//�������captionrect�ĸ߶�height,��������к�
int  CUIMultiEdit::HeightToRow(int height)
{
	
	//ת��Ϊ�����contentrect�ĸ߶�
	height-=m_ContentPos;

	int halfSpace=m_RowSpace*0.5;

	//��ʾ�߶�
	int dispHgt=0;
	if(height<dispHgt)	return -1;

	for(int nRow=0;nRow<m_RichLines.size();nRow++)
	{ 
		const stRichLine& theLine=m_RichLines[nRow];

		int ey=dispHgt+theLine.nHeight+halfSpace;

		if(height<=ey)	return nRow;

		dispHgt+=theLine.nHeight+m_RowSpace;

	}
	
	return m_RichLines.size()-1;	

}

//�õ����λ�õĿ��,-1��ʾ��Ч�Ĺ��λ��
int  CUIMultiEdit::GridToWidth(int row,int col)
{
	
	//�������
	if((row<0)||(row>=m_RichLines.size())) return -1;

	const stRichLine& theLine=m_RichLines[row];

	int dispWid=0;
	for(int j=theLine.nFirst;j<=theLine.nLast;j++)
	{	
		//���һ���ձ༭��
		if(j>=m_RichCaption.size()) break;

		if(j==theLine.nFirst+col) break;

		Point2D charSize=UI_GetCharSize(m_RichCaption[j],m_TextSize);					

		dispWid+=charSize.x;
	}

	return dispWid;


}

//�õ���row����,���captionrect���Ϊwidth�ĵ㴦,richchar������
//����������һ���ַ����򷵻����һ����ͣ�����λ��
int  CUIMultiEdit::WidthToChar(int row,int width)
{

	//�������
	if((row<0)||(row>=m_RichLines.size())) return -1;

	const stRichLine& theLine=m_RichLines[row];


	int dispWid=0;
	for(int j=theLine.nFirst;j<=theLine.nLast;j++)
	{	
		if((j<0)||(j>=m_RichCaption.size())) continue;

		Point2D charSize=UI_GetCharSize(m_RichCaption[j],m_TextSize);					
		
		if(width<=dispWid+charSize.x*0.5) 
		{
			return j;
		}

		dispWid+=charSize.x;
	}


	//���width�������е����һ���ַ�
	int nIndex=theLine.nLast;

	if(theLine.nLast<m_RichCaption.size())
	{
		if(m_RichCaption[theLine.nLast].type==RICH_CHAR_LF)
		{
			nIndex=theLine.nLast;
		}
		else 
		{
			nIndex=theLine.nLast+1;
		}

	}


	if(nIndex>m_RichCaption.size())
		nIndex=m_RichCaption.size();
	
	return nIndex;

}




//�õ���row��,��col�е��ַ�����
//����������һ���ַ����򷵻����һ����ͣ�����λ��
stEditTick CUIMultiEdit::GridToPos(int row,int col)
{
	//������
	if((row<0)||(row>=m_RichLines.size())) return stEditTick(-1,-1);

	const stRichLine& theLine=m_RichLines[row];

	int nIndex=theLine.nFirst+col;	
	
	//����������һ���ַ�
	if(nIndex>theLine.nLast)
	{
		if(theLine.nLast<m_RichCaption.size())
		{
			if(m_RichCaption[theLine.nLast].type==RICH_CHAR_LF)
			{
				nIndex=theLine.nLast;
			}
			else 
			{
				nIndex=theLine.nLast+1;
			}

		}
	}	
	
	if(nIndex>m_RichCaption.size())
		nIndex=m_RichCaption.size();
	
	return stEditTick(nIndex,row);

}

//�õ����λ�õ�����(row,col),�������׵�λ��
Point2D CUIMultiEdit::PosToGrid(int index)
{
	
	Point2D charGrid(0,0);

	if(index<0) index=0;
	else if(index>m_RichCaption.size()) index=m_RichCaption.size();

	for(int nRow=0;nRow<m_RichLines.size();nRow++)
	{ 
		const stRichLine& theLine=m_RichLines[nRow];

		//������
		if(index<theLine.nFirst)
		{
			charGrid.x=nRow;
			charGrid.y=0;
			break;
		}

		//λ�ڸ���
		if(index<=theLine.nLast)
		{
			charGrid.x=nRow;
			charGrid.y=index-theLine.nFirst;
			break;
		}	

	}

	return charGrid;
	
	
}

//����ÿ��richchar���ڵ�����,ÿ�е���ֹ
void CUIMultiEdit::CalcCharsLine()
{

	//��ǰ�ַ��Ĵ�ӡλ��
	int sx=0;
	int sy=0;

	int ex=0;
	int ey=0;
	
	//��ǰ������
	int nRow=0;
	int nCol=0;

	//ǰһ�е��к�
	int preRow=-1;

	//���������Ϣ
	m_RichLines.clear();

	//��ǰ�е��и�
	int lineHeight=0;
	

	for(int i=0;i<=m_RichCaption.size();i++)
	{ 

		Point2D charSize;
		
		if(i<m_RichCaption.size())
			charSize=UI_GetCharSize(m_RichCaption[i],m_TextSize);
		else
			charSize=Point2D(0,m_TextSize);
		
		bool bNeedReturn=false;
		
		//��һ���ַ������ǻ���
		if(i>0)
		{
			//ǰһ���ַ�Ϊ���п��Ʒ�,����
			if(m_RichCaption[i-1].type==RICH_CHAR_LF)
				bNeedReturn=true;

			//������ӡ��ȣ�����
			if( (sx+charSize.x)>m_CaptionRect.width)
				bNeedReturn=true;
		}


		if(bNeedReturn)
		{
			sx=0;
			sy+=(lineHeight+m_RowSpace);
			
			nRow++;
			nCol=0;
			lineHeight=charSize.y;
		}				

		if(charSize.y>lineHeight) lineHeight=charSize.y;

		
		//��¼��ӡ����λ��
		ex=sx+charSize.x;		
		ey=sy+lineHeight;
		

		//ǰ��һ�к͵�ǰ�кŲ�ͬ,��������һ��
		if(preRow!=nRow)
		{
			m_RichLines.push_back(stRichLine(i));

			preRow=nRow;		
		}

		
		//ÿ��ӡһ���ַ�(sx,sy)(nRow,nCol)
		stRichLine* pLine=&m_RichLines[m_RichLines.size()-1];		
		pLine->nLast=i;
		pLine->nHeight=lineHeight;
	
		
		//�ı��ӡ��ʼλ��
		sx+=charSize.x;
		nCol++;
		
	}
	

	m_ContentHeight=ey;


}

int CUIMultiEdit::ParseKeyChar(int x,int y,stKeyChar& keyChar)
{
	
	stRect absCapRct=GetAbsoluteCaptionRect();

	if(!absCapRct.IsPointIn(x,y)) return RICHCHAR_CMD_NONE;

	//ת��Ϊ��Ա�����������
	Point2D pt(x-absCapRct.ox,y-absCapRct.oy);


	int dispHgt=0;
	for(int i=0;i<m_RichLines.size();i++)
	{ 
			
		const stRichLine& theLine=m_RichLines[i];


		//�����ײ�
		if(dispHgt+m_ContentPos>=absCapRct.height) break;

		//δ��������
		if(dispHgt+m_ContentPos+theLine.nHeight+m_RowSpace>=0)
		{

			//�ҵ�������
			if((pt.y>=dispHgt+m_ContentPos)&&(pt.y<=dispHgt+m_ContentPos+m_RichLines[i].nHeight))
			{

				//��ʾ���
				int dispWid=0;
				for(int j=theLine.nFirst;j<=theLine.nLast;j++)
				{	
					if(j>=m_RichCaption.size()) break;
					
					Point2D charSize=UI_GetCharSize(m_RichCaption[j],m_TextSize);									

					//�ҵ�������
					if((pt.x>=dispWid)&&(pt.x<=dispWid+charSize.x))
					{

						for(int k=j;k>=0;k--)
						{ 
							
							if(m_RichCaption[k].type==RICH_CHAR_HLINK)
							{
								if(m_RichCaption[k].index<=DEFAULT_HLINK_BASE)
									return RICHCHAR_CMD_NONE;

								m_RichCaption[k].GetKeyChar(keyChar);
								
								ExecCallBackFunc(UI_Callback_ValidKeyChar,(void*)&keyChar);

								return keyChar.cmdid;
							}

						}

						return RICHCHAR_CMD_NONE;
						
					}

					dispWid+=charSize.x;
				}

				return RICHCHAR_CMD_NONE;

			}
		
		
		}

		dispHgt+=theLine.nHeight+m_RowSpace;

	}
	

	return RICHCHAR_CMD_NONE;

}

BOOL CUIMultiEdit::IsCursorVisible()
{

	bool bCursorVis=false;		
	if(IsEnabled()&&(m_SelBegin==m_CursorPos))
	{
		if(g_pRootWgt->GetFocusWgt()==this)
		{	
			bCursorVis=true;	
		}
		else if(m_ExtendStyle&EDIT_STYLE_KEEPCURSOR)
		{			
			bCursorVis=true;			
		}
		else if(m_ExtendStyle&EDIT_STYLE_SMARTCURSOR)
		{	
			/*
			CUIWidget* pFocusWgt=g_pRootWgt->GetFocusWgt();
			
			if(pFocusWgt&&pFocusWgt->IsAcceptInput())
				bCursorVis=false;
			else
				bCursorVis=true;
			*/
			bCursorVis=(g_pRootWgt->GetHotWgt()==this);
		}
	}

	if(bCursorVis&&m_CursorFlag)
		return TRUE;
	else 
		return FALSE;
}

void CUIMultiEdit::DrawCaption()
{

	stRect absCapRct=GetAbsoluteCaptionRect();

	stColorQuad drawColor=GetCaptionColor();

	stColorQuad selColor=GetRenderColor(m_EditSelColor);
	stColorQuad magicColor=GetRenderColor(m_MagicColor);
	stColorQuad linkColor=GetRenderColor(m_LinkColor);

	int wgtState=UI_ITEM_STATE_FOCUSED;

	if(!IsEnabled()) 
	{
		drawColor=GetRenderColor(m_GrayColor);
		magicColor=drawColor;
		wgtState=UI_ITEM_STATE_DEFAULT;
	}
	
	stUIPen uiPen;
	uiPen.dwdraw=drawColor.c;
	uiPen.dwmagic=magicColor.c;
	uiPen.dwlink=linkColor.c;
	uiPen.font=m_TextFont;
	uiPen.style=m_TextStyle;
	uiPen.size=m_TextSize;
	uiPen.state=wgtState;

	stUIPen defaultPen;
	defaultPen.dwdraw=drawColor.c;	
	defaultPen.font=m_TextFont;	
	defaultPen.style=m_TextStyle;


	//���浱ǰ����
	int oldfont=g_pDisplay->SetFont(uiPen.font);

	//ѡ��Χ
	int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
	int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;

	Point2D charGrid=GetCursorGrid();
	stRect cursorRct;

	int dispHgt=0;
	for(int i=0;i<m_RichLines.size();i++)
	{ 
			
		const stRichLine& theLine=m_RichLines[i];

		//�����ײ�
		if(dispHgt+m_ContentPos>=absCapRct.height) break;

		//δ��������
		bool bVisible=(dispHgt+theLine.nHeight+m_RowSpace+m_ContentPos>=0);

		int dispOrigin=absCapRct.oy+m_ContentPos+dispHgt;

		//��ʾ���
		int dispWid=absCapRct.ox;
		for(int j=theLine.nFirst;j<=theLine.nLast;j++)
		{	
			Point2D charSize=Point2D(0,m_TextSize);

			if(j<m_RichCaption.size())
				charSize=UI_GetCharSize(m_RichCaption[j],m_TextSize);					
			
			int offset=(theLine.nHeight-charSize.y)/2;
			
			if(j<m_RichCaption.size())
			{
				UI_DrawChar(dispWid,dispOrigin+offset,m_RichCaption[j],uiPen,defaultPen,bVisible);
			
				//����ѡ����
				if((j>=selBegin)&&(j<selEnd)&&bVisible)
				{
					g_pDisplay->DrawBar(dispWid,dispOrigin,charSize.x,theLine.nHeight+m_RowSpace,selColor.c);
				}

			}

			//��ǰ���ǹ����
			if(charGrid.x==i)
			{
				if(m_CursorPos==j)
					cursorRct=stRect(dispWid,dispOrigin+offset,1,charSize.y);		
				else if(m_CursorPos>j)
					cursorRct=stRect(dispWid+charSize.x,dispOrigin+offset,1,charSize.y);
			}

			dispWid+=charSize.x;
		}

		dispHgt+=theLine.nHeight+m_RowSpace;

	}

	
	//���ƹ��
	if(IsCursorVisible())
	{		
		g_pDisplay->DrawBar(cursorRct.ox,cursorRct.oy,cursorRct.width,cursorRct.height,drawColor.c );		
	}


	//�ָ���ǰ����
	g_pDisplay->SetFont(oldfont);


}

//����edit����,�����ֻ��Ƶ�y����λ�ÿ�����captionrect������������
//��x����,���������
//���,edit���ڲ�֧��text align���
void CUIMultiEdit::RenderSelf()
{

	CUIWidget::RenderSelf();	

	DrawCaption();	

}




void CUIMultiEdit::CheckForReason()
{
	
	//ȷ�����λ�ú���
	MoveCursor(m_CursorPos);
	MoveSelBegin(m_SelBegin);
	MoveLinePos(m_LinePos);

	//�õ��������ȵĹ��λ��
	Point2D charGrid=PosToGrid(m_CursorPos);

	if(m_LinePos>charGrid.x) MoveLinePos(charGrid.x);
	else if(m_LinePos<charGrid.x-1) MoveLinePos(charGrid.x-1);
	
	if((m_CursorPos>0)&&(m_CursorPos<=m_RichCaption.size()))
	{
		//���ǰ��һ���ַ��ǻ��з���
		if(m_RichCaption[m_CursorPos-1].type==RICH_CHAR_LF)
		{
			MoveLinePos(charGrid.x);
		}
	}

}

void CUIMultiEdit::ReparseText()
{

	MergeRichChar();

	CalcCharsLine();

	CheckForReason();
	
	UpdateScrollBar();
	
	EnsureCursorVisible();

}

void CUIMultiEdit::UpdateSelf( float deltatime )
{

	CheckForReason();

	//��˸���
	if((m_ExtendStyle&EDIT_STYLE_FLASHCURSOR)||(g_pRootWgt->GetFocusWgt() == this))
	{
		int dtick = int(deltatime*1000);
		m_CursorTimer += dtick;
		if( (m_CursorFlag&&m_CursorTimer>m_CursorLive) || (!m_CursorFlag&&m_CursorTimer>m_CursorDeath) )
		{
			m_CursorFlag = 1-m_CursorFlag;
			m_CursorTimer = 0;
		}
	}
	else 
	{
		m_CursorFlag=1;
		m_CursorTimer = 0;
	}



	//�ϲ���������ַ���
	if(IsUpdateDirty())
	{		

		ReparseText();

		SetUpdateDirty(false);

	}

	
	CUIWidget::UpdateSelf(deltatime);


}

//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIMenu---------------------------------------
//-------------------------------------------------------------------------------------------
CUIMenu::CUIMenu()
{
	m_Type = WGTTYPE_MENU;
}

CUIMenu::~CUIMenu()
{
}

void CUIMenu::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_Style=WGTSTYLE_POPUP|WGTSTYLE_TOPMOST;	

	m_EdgeCheck=EDGE_CHECK_SHOW|EDGE_CHECK_AUTO;

}

//����groupstop�������пɼ�����
//����ÿ�����ӵ�λ��
//�����Լ��ͻ�����С�ʹ��ڴ�С
void CUIMenu::AjustSelf()
{

	//�õ����пɼ����ӵ�����
	std::vector<CUIWidget*>vecVisChild;
	std::vector<CUIWidget*>::iterator iter;
	for(iter=m_Children.begin();iter!=m_Children.end();iter++)
	{ 
		if((*iter)->IsVisible())
			vecVisChild.push_back((*iter));
	}


	//����groupstop���пɼ�����
	std::sort(vecVisChild.begin(),vecVisChild.end(),SortWidgetByGroupStop);


	//����ÿ�����ӵ�λ��
	stRect clientRect=GetAbsoluteClientRect();
	int ey=0;
	for(iter=vecVisChild.begin();iter!=vecVisChild.end();iter++)
	{ 
		(*iter)->MoveToY(clientRect.oy+ey);
		ey+=(*iter)->GetHeight();		
	}

	//�����ͻ����ʹ��ڴ�С
	int offset=m_WinRect.height-m_ClientRect.height;
	m_ClientRect.height=ey;
	m_WinRect.height=ey+offset;

}

int  CUIMenu::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{

	case NTFTYPE_CHILD_SHOWHIDE:		
		{
			SetUpdateDirty(true);
		}		
		break;		
	
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}




void CUIMenu::PostUpdateSelf( float deltatime )
{

	if(IsUpdateDirty()) 
	{

		AjustSelf();
		
		SetUpdateDirty(false);
	}
	
	CUIWidget::PostUpdateSelf(deltatime);

}


//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIConsole------------------------------------
//-------------------------------------------------------------------------------------------
CUIConsole::CUIConsole()
{
	m_Type = WGTTYPE_CONSOLE;	
}

CUIConsole::~CUIConsole()
{
}

void CUIConsole::ClearParameter()
{
	CUIWidget::ClearParameter();
	
	//�ű�ģʽ
	m_CmdMode = CONSOLE_MODE_SCRIPT;

	m_NumInfo = 0;
}

void  CUIConsole::SetVar(stUIVar& uiVar)
{
	
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_ITEMTEXT:
		EnterString(uiVar.sValue.c_str());
		break;

	default:
		CUIWidget::SetVar(uiVar);

	}
	
}


void CUIConsole::RenderSelf()
{	

	CUIWidget::RenderSelf();//���Ʊ���ͼƬ�Ͱ�Χ��

	stColorQuad drawColor=GetRenderColor(m_DrawColor);
	
	//��ʾ������
	for( int i=0; i<m_NumInfo; i++ )
	{
		int ey = m_WinRect.oy+m_WinRect.height-EDIT_H-i*LINE_H;
		g_pDisplay->DrawTextEx( 
			m_WinRect.ox, ey-LINE_H, 
			m_WinRect.width, LINE_H, 
			m_AllInfo[i], 
			m_TextSize, 
			m_RowSpace,
			drawColor.c, 
			dtLeft|dtVCenter );
	}

}


//�س�����
//ѹ��һ���ַ���,��ʾ����,���Ҹ��ݵ�ǰ�ǽű�ģʽ�����ַ�ģʽ������Ӧ����
void CUIConsole::EnterString(const char *pstr)
{

	std::string buffer=">>";
	buffer+=pstr;		

	PushString( buffer.c_str() );//ѹ��һ���ַ���

	ExecString( pstr );//ִ��һ���ַ���

}

//ѹ��һ���ַ���,��ʾ����
void CUIConsole::PushString( const char *pstr )
{

	int maxnum = min(m_NumInfo-1, NUM_LINES-2);//ע�⣬�����и�[i+1],���ԣ��˴�ΪNUM_LINES-2��������NUM_LINES-1
	for( int i=maxnum; i>=0; i-- )
	strcpy( m_AllInfo[i+1], m_AllInfo[i] );//����������	

	if( m_NumInfo < NUM_LINES ) m_NumInfo++;//û�дﵽ������֮ǰ���ɼ���ѹ��

	if( strlen( pstr) < LINE_LEN)	
		strcpy( m_AllInfo[0], pstr );//δ�������ޣ���������0��
	else
	{
		//strncpy(m_AllInfo[0],pstr,LINE_LEN-1);//�������ޣ���ȡ(��bug)
		m_AllInfo[0][0] = 0;//�������ޣ�����
	}
	
}

void CUIConsole::ExecString( const char *pstr )
{

	
	if( strcmp(pstr , g_UICoreStyle.bufTalkCmd) == 0 ) m_CmdMode = CONSOLE_MODE_TALK;//�л����Ի�ģʽ������
	else if( strcmp(pstr , g_UICoreStyle.bufScriptCmd) == 0 ) m_CmdMode = CONSOLE_MODE_SCRIPT;//�л����ű�ģʽ������
	else if( m_CmdMode == CONSOLE_MODE_SCRIPT ) //�����˷�������ַ������ű����߶Ի���,��ǰ�������ڽű�ģʽ��
	{
		char buffer[UI_DEFAULT_BUFSIZE];
		buffer[0]= 0;
		
		strcpy( buffer, pstr );//��ǰΪ�ű�ģʽ
		
		if( buffer[0] )
		{
			g_pRootWgt->GetScriptEnv()->DoString( buffer );//ִ�нű�
			const char *perror = g_pRootWgt->GetScriptEnv()->GetLastError();//�õ����������
			if( perror ) PushString( perror );//ѹ����������
		}
		
	}
	

}


//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIScreen-------------------------------------
//-------------------------------------------------------------------------------------------
CUIScreen::CUIScreen()
{
	m_Type = WGTTYPE_SCREEN;
}

CUIScreen::~CUIScreen()
{

}

void CUIScreen::OnInit()
{

	
	CUIWidget* pWgt=NULL;
	
	
	//������Ļ��msgbox				
	pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_MSGBOX);
	if(pWgt) pWgt->ShowWidget(false);
	
	//���ع���msgbox
	pWgt=g_pRootWgt->GetSafetBuddyWgt(BUDDY_WIDGET_MSGBOX);
	if(pWgt) pWgt->ShowWidget(false);
	
	
	//������Ļ��tooltip
	pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_TIPWGT);
	if(pWgt) pWgt->ShowWidget(false);
	
	//���ع���tooltip
	pWgt=g_pRootWgt->GetSafetBuddyWgt(BUDDY_WIDGET_TIPWGT);
	if(pWgt) pWgt->ShowWidget(false);
	
	//������Ļ���Ҽ���Ϣinfotip
	pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_INFOWGT);
	if(pWgt) pWgt->ShowWidget(false);
	
	//���ع����Ҽ���Ϣinfotip
	pWgt=g_pRootWgt->GetSafetBuddyWgt(BUDDY_WIDGET_INFOWGT);
	if(pWgt) pWgt->ShowWidget(false);
	
	
	CUIWidget::OnInit();

}

void CUIScreen::OnDenit()
{
		
	g_pRootWgt->SetPopupWgt( NULL);
	g_pRootWgt->SetFocusWgt( NULL);
	g_pRootWgt->SetMouseWgt( NULL);
	g_pRootWgt->SetCaptureWgt( NULL);
	g_pRootWgt->SetDragWgt(  NULL);
	g_pRootWgt->SetAjustWgt( NULL);
	g_pRootWgt->SetHotWgt( NULL);

	CUIWidget::OnDenit();


}


void CUIScreen::SetParameter( CVar *pvar, CUIWidget *pfather )
{

	CUIWidget::SetParameter( pvar, pfather );

	//stRect windowRect=g_pRootWgt->GetWinRect();

	//SetWinRect(windowRect);
	

}


//ȷ���Ƿ����Ԥ����ű�����
void CUIScreen::ConfirmScripts( CVar *pvar )
{

	if(!pvar) return;

	m_ScriptFuncFlags.clear();
	for(int i=0;i<g_vecScriptFuncs.size();i++)
	{ 
		CVar* pFuncVar=pvar->Element(g_vecScriptFuncs[i].c_str());

		if(!pFuncVar) continue;
			
		if(pFuncVar->GetType()==VAR_FUNC)
			m_ScriptFuncFlags.push_back(stScriptFunc(i,""));
		else if(pFuncVar->GetType()==VAR_STRING)
			m_ScriptFuncFlags.push_back(stScriptFunc(i,pFuncVar->String()));

	}

}

int CUIScreen::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{
		
	case VK_TAB:
		{
			CUIWidget* pWgt=GetNextStopChild();
			if(pWgt&&pWgt->IsVisible())  g_pRootWgt->SetFocusWgt(pWgt);
		}
		break;
		
	default:
		
		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
		
		
	}
	
	return 1;
}



int CUIScreen::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	switch( msgtype )
	{

	case MSGTYPE_MOUSEIDLE:
		{			

			stGameCursor cursorData;
			ExecCallBackFunc(UI_Callback_GetGameCursor,(void*)&cursorData);
			
			//�����Ϸ��������Ƶ�ǰ������Ʋ�ͬ���л����
			if( (m_CursorName!=cursorData.name)&&(strcmp(cursorData.name,"")) )
			{
				m_CursorName=cursorData.name;
				
				g_pRootWgt->SetCurCursor(m_CursorName.c_str());
				
			}
			
		}
		break;		


	case MSGTYPE_LBTNDBLCLK:
	case MSGTYPE_LEFTBTNDOWN:	
	case MSGTYPE_LEFTBTNUP:	
	case MSGTYPE_RIGHTBTNDOWN:
	case MSGTYPE_RIGHTBTNUP:		
		
		//�����Ļʱ��,��ΪӦ���ɿͻ��˴���
		//������Щ��Ϣ�����ܽ���
	
		return 0;

		break;

	
	default:

		return CUIWidget::OnMessage(msgtype,wparam,lparam);
	
	}
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}


//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIDialog-------------------------------------
//-------------------------------------------------------------------------------------------
CUIDialog::CUIDialog()
{
	m_Type = WGTTYPE_DIALOG;	
}

CUIDialog::~CUIDialog()
{

}


void CUIDialog::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_EdgeCheck=EDGE_CHECK_SHOW|EDGE_CHECK_AUTO;
}




void CUIDialog::RenderSelf()
{

	CUIWidget::RenderSelf();//���Ʊ����Ͳ�͸���ĺп�
	
	DrawCaption();

}



int  CUIDialog::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_COMMAND_OK:	
		EndWidget(true);
		break;
	
	case NTFTYPE_COMMAND_CANCEL:		
		EndWidget(false);
		break;	
		
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}




//-------------------------------------------------------------------------------------------
//---------------------------------------class CUISystemDialog-------------------------------------
//-------------------------------------------------------------------------------------------

CUISystemDialog::CUISystemDialog()
{
	m_Type = WGTTYPE_SYSDIALOG;
}


CUISystemDialog::~CUISystemDialog()
{
		
}

//�ڷ��϶�������Ϊok
//�Ҽ�Ϊcancel
int CUISystemDialog::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	switch(msgtype)
	{

	case MSGTYPE_LEFTBTNUP:
		
		if( m_State & WGTSTATE_CLICKDOWN )
		{						
			m_State &= ~WGTSTATE_CLICKDOWN;	
			
			if(m_Style&WGTSTYLE_ADVANCE)
			{

				int x=LOWORD(lparam);
				int y=HIWORD(lparam);

				
				if(!IsPointInDragRect(x,y))
					OnCmdOk();
				
			}		

		}			

		break;

	case MSGTYPE_RIGHTBTNDOWN:
		OnCmdCancel();
		break;		

	default:
		return CUIDialog::OnMessage( msgtype, wparam, lparam );
		
	}
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}




//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIMessageBox-------------------------------------
//-------------------------------------------------------------------------------------------

CUIMessageBox::CUIMessageBox()
{
	m_Type = WGTTYPE_MESSAGEBOX;
}

CUIMessageBox::~CUIMessageBox()
{

}

void CUIMessageBox::ClearParameter()
{
	CUIDialog::ClearParameter();

	m_CallHandle=0;

	m_MsgId=0;

	m_MsgInfo="";

	EnableMove(true);	

	m_AlignType=ALIGNTYPE_WINRECT|ALIGNTYPE_FATHER;
	
	m_AlignStyle=ALIGNSTYLE_HCENTER|ALIGNSTYLE_VCENTER;

}


void CUIMessageBox::ShowMsgBox(Msgbox_Cmd nCmdShow,const char* pInfo,const char* pCaption/*="Message"*/,int nMsgId/*=UI_MSGID_DEFAULT*/,CUIWidget* pWgt/*=0*/,int nHandle/*=0*/)
{
	if(nCmdShow==UI_MSGBOX_CMDUPDATE)
	{
		if(m_MsgId!=nMsgId)
			return;

		if(!IsVisible()) 
			return;
	}

	RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",pWgt);

	m_Caption=pCaption;

	m_MsgInfo=pInfo;

	m_MsgId=nMsgId;	

	m_CallHandle=nHandle;	

	if(nCmdShow==UI_MSGBOX_CMDSHOW)
		ShowWidget(true);
	else if(nCmdShow==UI_MSGBOX_CMDHIDE)
		ShowWidget(false);


}



void CUIMessageBox::OnCmdOk()
{

	CUIWidget* pScriptWgt=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);

	if(pScriptWgt) 
	{		
		pScriptWgt->CallSafeScriptEx(SCRIPT_FUNC_ON_MSGBOX_OK,"%d,%d",m_MsgId,m_CallHandle);
	}
	else
	{
		CUIDialog::OnCmdOk();
	}
	
}

void CUIMessageBox::OnCmdCancel()
{

	CUIWidget* pScriptWgt=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);

	if(pScriptWgt) 
	{		
		pScriptWgt->CallSafeScriptEx(SCRIPT_FUNC_ON_MSGBOX_CANCEL,"%d,%d",m_MsgId,m_CallHandle);
	}
	else
	{		
		CUIDialog::OnCmdCancel();
	}
	
}

void    CUIMessageBox::SetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{

	case VALUE_CHANNEL_MSGBOX_ID:
		SetMsgId((int)uiVar.fValue); 
		break;

	case VALUE_CHANNEL_MSGBOX_INFO:
		SetMsgInfo(uiVar.sValue.c_str()); 
		break;

	default:
		CUIDialog::SetVar(uiVar);
	}

}

void  CUIMessageBox::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{

	case VALUE_CHANNEL_MSGBOX_ID:
		uiVar.fValue = GetMsgId(); 
		break;

	case VALUE_CHANNEL_MSGBOX_INFO:
		uiVar.sValue = GetMsgInfo(); 
		break;

	default:
		CUIDialog::GetVar(uiVar);
	}

}

void CUIMessageBox::RenderSelf()
{

	CUIDialog::RenderSelf();//���Ʊ����Ͳ�͸���ĺп�

}

//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIPicLabel-------------------------------------
//-------------------------------------------------------------------------------------------
CUIPicLabel::CUIPicLabel()
{
	m_Type = WGTTYPE_PICLABEL;

	m_pFontPic=NULL; 
}

CUIPicLabel::~CUIPicLabel()
{
	SAFE_RELEASE(m_pFontPic);
}

void CUIPicLabel::ClearParameter()
{
	CUIButton::ClearParameter();
	
	m_MaxWidth=DEFAULT_LABEL_MAX_WIDTH;
	m_MinHeight=DEFAULT_LABEL_MIN_HEIGHT;

	SAFE_RELEASE(m_pFontPic);
}

void CUIPicLabel::SetFontPic( CUIImage *psurface )
{
	if( psurface) psurface->IncRefCount();
	if( m_pFontPic ) m_pFontPic->Release();
	m_pFontPic = psurface;
}

void CUIPicLabel::SetFontPic( const char *filename )
{
	CUIImage *psurface = 0;
	if( filename) psurface = new CUIImage(filename);
	SetFontPic( psurface );
	if( psurface ) psurface->Release();
}


void CUIPicLabel::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIButton::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
	if( (pelement=pvar->Element("fontpic")) )
	SetFontPic( pelement->String());
	
}


void CUIPicLabel::SetCaption( const char *pname )
{

	CUIButton::SetCaption(pname);

	AjustSelf();

}

//ֻ�����߶ȣ����������
//����ֻ������ȣ��������߶�
//������ȫ���ɵ���
void CUIPicLabel::AjustSelf()
{	

	
	if(!IsSizable()) return;//��ȸ߶ȶ������Ե���	
	
	m_CaptionRect.ox	 = m_MarginH;
	m_CaptionRect.oy	 = m_MarginV;
	
	m_CaptionRect.width  = m_WinRect.width  - m_MarginH*2;
	m_CaptionRect.height = m_WinRect.height - m_MarginV*2;
	
	int width=m_CaptionRect.width;
	int height=m_CaptionRect.height;
	
	//init
	if(width<m_TextSize)  width =m_TextSize;
	if(height<m_TextSize) height=m_TextSize;
	

	if(IsSizableX()&&IsSizableY()) //��ȸ߶ȶ����Ե���,���д�ӡ
	{ 
		width=UI_MAX_INTEGER;
		height=UI_MAX_INTEGER;
		
		//�������ɴ�ӡ�Ŀ�ȸ߶�
		g_pDisplay->EstimateLine(m_Caption.c_str(),m_TextSize,m_RowSpace,width,height);
		
		//��ȳ�����������
		if(width>m_MaxWidth)
		{
			width=m_MaxWidth;
			height=UI_MAX_INTEGER;
			
			//���Զ����ӡ�ĸ߶�
			height=g_pDisplay->EstimateLine(m_Caption.c_str(),width,m_TextSize,m_RowSpace);
		}
		
	}
	else if(IsSizableY())//���߶ȿ��Ե���,���д�ӡ
	{	
		
		//���Զ����ӡ�ĸ߶�
		height=g_pDisplay->EstimateLine(m_Caption.c_str(),width,m_TextSize,m_RowSpace);
	}

	if(height<m_MinHeight)
		height=m_MinHeight;
		
	m_CaptionRect.width=width;
	m_CaptionRect.height=height;
	
	m_WinRect.width  = m_CaptionRect.width  + m_MarginH*2;
	m_WinRect.height = m_CaptionRect.height + m_MarginV*2;
	
	
	
	SetAjusted(true);
	
}

//ͼƬ���ֵĶ�Ӧ��ϵΪ:
/*

�������������������������������������������� 
���������������������������������������������� 
��������������"�������硡������������������ 
�������������������������������������������� 
�������������������������������������������� 
�����������������������¡��á��ġ��š��ơ��� 
�������ȡ��ɡ��ʡ��ˡ��̡��͡��Ρ��ϡ��С��� 
�������ҡ��ӡ��ԡ��ա��֡��ס��ء��١��ڡ��� 
�������ܡ��ݡ��ޡ��ߡ��ࡡ�ᡡ�⡡�㡡�䡡�� 
�������桡�硡�衡�顡�ꡡ�롡�졡����� 
�������𡡣񡡣򡡣󡡣��������������������� 
������������������������ 

*/
void CUIPicLabel::DrawCaption()
{

	if( !m_Caption.empty() && m_pFontPic)
	{
		stRect capRect=GetAbsoluteCaptionRect();

		Point2D captionOffset(0,0);

		if((m_CurFrameState>=0)&&(m_CurFrameState<FRAME_STATE_MAX))
		{
			captionOffset=m_CaptionOffset[m_CurFrameState];
		}

		capRect.ox+=captionOffset.x;
		capRect.oy+=captionOffset.y;

		//֧�ָ߿��ȵ�����
		int sx=0;
		int sy=0;
		int picWidth=m_pFontPic->GetWidth();
		int picHeight=m_pFontPic->GetHeight();
		
		//Ӣ���ַ��Ŀ��
		int charWidth=m_TextSize/2;

		//Ӣ���ַ��ĸ߶�
		int charHeight=m_TextSize;
				
		for ( int i=0; i<m_Caption.length(); i++)
		{
			char c = m_Caption[i];
			
			int frame=-1;

			if((c>=' ')&&(c<='~'))
			{
				frame =  c-' ';
			}
		
			//����֡����Χ,������
			if(frame>=m_pFontPic->GetNumFrame()) 
			{
				if ( c >= '0' && c <= '9' )
					frame = c-'0';
				if ( frame < 0 || frame>=m_pFontPic->GetNumFrame() )
					continue;
			}
					
			//��ӡˮƽ����,����
			if((sx+charWidth>capRect.width)||(c=='/n'))
			{	
				sy+=(charHeight+m_RowSpace);
				sx=0;
			}

			//��ӡ��ֱ����,�˳�
			if(sy+charHeight>capRect.height)
				return;

			m_pFontPic->Draw(capRect.ox+sx+(charWidth-picWidth)/2,capRect.oy+sy+(charHeight-picHeight)/2,frame);

			sx+=charWidth;
				
		}

	}
}





//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIChangeBar-------------------------------------
//-------------------------------------------------------------------------------------------

CUIChangeBar::CUIChangeBar()
{
	m_Type = WGTTYPE_CHANGEBAR;	
}

CUIChangeBar::~CUIChangeBar()
{
	m_StateString.clear();
}

void CUIChangeBar::ClearParameter()
{
	CUIButton::ClearParameter();

	//add by lincoln [2006:2:21] [10:45]
	//{{{changebar�Ľ�
	SetNumState(1);
	//}}}

	m_Value=0;

	m_ExtendStyle=UI_BTN_STYLE_POPUPMSG|UI_BTN_STYLE_AUTO_TOGGLE|UI_BTN_STYLE_AUTO_ROUND;
	

}

void CUIChangeBar::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
	
	case VALUE_CHANNEL_VALUE:
		uiVar.fValue = m_Value; 
		break;

	case VALUE_CHANNEL_ITEM_NUM:
		uiVar.fValue = GetNumState();
		break;

	case VALUE_CHANNEL_ITEMTEXT:
		uiVar.sValue=GetBarString(uiVar.iIndex);
		break;
	

	default:
		CUIButton::GetVar(uiVar);
	}

}

void  CUIChangeBar::SetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
	
	case VALUE_CHANNEL_VALUE:
		SetBarState((int)uiVar.fValue); 
		break;

	case VALUE_CHANNEL_ITEM_NUM:
		SetNumState((int)uiVar.fValue);
		break;

	case VALUE_CHANNEL_ITEMTEXT:
		SetBarString(uiVar.iIndex,uiVar.sValue.c_str());
		break;
	
	default:
		CUIButton::SetVar(uiVar);
	}
	
}


//Ĭ��һ��״̬һ֡
void CUIChangeBar::DefaultFrameScheme()
{
	
	m_vecFrameScheme.clear();

	int numState=GetNumState();
	for(int i=0;i<numState;i++)
	{ 

		stFrameScheme valueFrmState(i);
		m_vecFrameScheme.push_back(valueFrmState);
		
	}
	
	m_CurFrameScheme=0;
	
}

void CUIChangeBar::OnSchemeFrame()
{

	SetFrameScheme(m_Value);
	
}

void CUIChangeBar::SetBarState(int state)
{
	//�������ת
	//���state<0,���л������״̬
	//���state�������״̬,�л�������״̬

	int numState=GetNumState();
	if(m_ExtendStyle&UI_BTN_STYLE_AUTO_ROUND)
	{
		if(state>=numState) state=0;
		else if(state<0) state=numState-1;		
	}
	else 
	{
		if(state>=numState) state=numState-1;
		else if(state<0) state=0;
	}

	//ȷ�����ݺϷ�
	if(state<0) state=0;
	
	m_Value=state;

	if(m_Value<m_StateString.size()&&m_Value>=0)
	m_Caption=m_StateString[m_Value];

}


void CUIChangeBar::NextState(BOOL forward/*=true*/)
{
	
	if(forward)
		SetBarState(m_Value+1);
	else 
		SetBarState(m_Value-1);
	
}

void CUIChangeBar::SetNumState(int num)
{

	if(num>CHANGEBAR_MAX_STRING) num=CHANGEBAR_MAX_STRING;
	if(num<0) num=0;

	//add by lincoln [2006:2:21] [10:45]
	//{{{changebar�Ľ�
	m_StateString.clear();
	if(num>0)
	{
		m_StateString.resize(num,"");
	}
	//}}}

}

int  CUIChangeBar::GetNumState()
{
	return m_StateString.size();
}

void CUIChangeBar::SetBarString(int nIdx,const char* pstr)
{
	if(nIdx>=0&&nIdx<m_StateString.size())
	{
		m_StateString[nIdx]=pstr;
	}

}

const char* CUIChangeBar::GetBarString(int nIdx)
{
	if(nIdx>=0&&nIdx<m_StateString.size())
	{
		return m_StateString[nIdx].c_str();
	}

	return ("");

}

void CUIChangeBar::SetParameter( CVar *pvar, CUIWidget *pfather )
{

	
	if(!pvar) return;
	CVar *pelement=NULL;

	if( (pelement=pvar->Element("numstate")) )  
	{
		SetNumState(pelement->Integer());
	}
	else if( (pelement=pvar->Element("statestring")) )
	{
		const char* text[CHANGEBAR_MAX_STRING];
		int itemnum=CHANGEBAR_MAX_STRING;
		itemnum=pelement->StringArray(text,itemnum);				

		SetNumState(itemnum);		

		for(int i=0;i<itemnum;i++)
		{
			SetBarString(i,text[i]);
		}
	}


	CUIButton::SetParameter( pvar, pfather );
	
		
	if( (pelement=pvar->Element("value")) )  
	{
		SetBarState(pelement->Integer());
	}	
	
}


int  CUIChangeBar::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_SPINARROW:		
	
		NextState((int)(wparam) & WGTSTYLE_PRIMARY);		
		
		break;		
		
	default:
		return CUIButton::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}


int CUIChangeBar::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	switch( msgtype )
	{

	case MSGTYPE_LEFTBTNDOWN:			
		
		m_State |= WGTSTATE_CLICKDOWN;
		
		if(!GetMsgStyle())
		{
			if(m_ExtendStyle&UI_BTN_STYLE_AUTO_TOGGLE)
			NextState();	

			CallSafeScript(SCRIPT_FUNC_ON_CLICK);
			
			if(m_pFather) m_pFather->OnNotify(GetCommandId(),0,(unsigned int)this);
			
			
		}
		
		break;
		
	case MSGTYPE_LEFTBTNUP:
		if( m_State & WGTSTATE_CLICKDOWN )
		{	
			m_State &= ~WGTSTATE_CLICKDOWN;	
			
			if(GetMsgStyle())
			{
				if(m_ExtendStyle&UI_BTN_STYLE_AUTO_TOGGLE)
				NextState();	

				CallSafeScript(SCRIPT_FUNC_ON_CLICK);
				
				if(m_pFather) m_pFather->OnNotify(GetCommandId(),0,(unsigned int)this);
			}
			
		}	
		
		
		
		break;
		
		
	default:
		return CUIButton::OnMessage( msgtype, wparam, lparam );
	}
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;

}

//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIKeyEdit-------------------------------------
//-------------------------------------------------------------------------------------------

CUIKeyEdit::CUIKeyEdit()
{	
	m_Type = WGTTYPE_KEYEDIT;
}

CUIKeyEdit::~CUIKeyEdit()
{
	
}

void CUIKeyEdit::ClearParameter()
{
	CUIButton::ClearParameter();

	m_Value=-1;
	
	m_Caption="null";
	m_TextStyle=dtCenter|dtVCenter;

	m_CursorTimer = 0;
	m_CursorFlag = 1;
	m_CursorLive = 700;
	m_CursorDeath =400;
}

//�ܾ�������������
BOOL CUIKeyEdit::IsAcceptIme()
{
	return FALSE;
}

BOOL CUIKeyEdit::IsAcceptInput()
{
	return IsChecked()&&IsEnabled();
}

BOOL CUIKeyEdit::IsCursorVisible()
{
	return IsChecked()&&IsEnabled()&&m_CursorFlag;
}

void CUIKeyEdit::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIButton::SetParameter( pvar, pfather );
	
	if(!pvar) return;

	CVar *pelement=NULL;
	
	if( (pelement=pvar->Element("keymap")) )
	{
		CVarArray* pMapVar=(CVarArray*)pelement;

		m_KeyMap.clear();
		for(int i=0;i<pMapVar->GetNum();i++)
		{ 
			CVarArray* pKeyVar=(CVarArray*)(pMapVar->Element(i));
			
			int key=pKeyVar->Element(0)->Integer();
			const char* txt=pKeyVar->Element(1)->String();

			m_KeyMap[key]=txt;  

		}


	}
	
}


void CUIKeyEdit::SetVar(stUIVar& uiVar)
{

	switch(uiVar.iChannel)
	{
	
	case VALUE_CHANNEL_VALUE:
		m_Value=uiVar.fValue; 
		break;

	default:
		CUIButton::SetVar(uiVar);
	}

}

void CUIKeyEdit::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
	
	case VALUE_CHANNEL_VALUE:
		uiVar.fValue = m_Value; 
		break;
	
	default:
		CUIButton::GetVar(uiVar);	
	}
	
}


void CUIKeyEdit::DefaultFrameScheme()
{

	m_vecFrameScheme.clear();
	
	stFrameScheme editFrmState(0);
	m_vecFrameScheme.push_back(editFrmState);
	
	stFrameScheme uneditFrmState(1);
	m_vecFrameScheme.push_back(uneditFrmState);
	
	m_CurFrameScheme=0;

}

void CUIKeyEdit::OnSchemeFrame()
{
	
	if( IsChecked() )  
	SetFrameScheme(0);
	else    
	SetFrameScheme(1);	
	
}



void CUIKeyEdit::DrawCaption()
{
	
	
	if(IsCursorVisible())
	{   
		
		stColorQuad drawColor=GetCaptionColor();

		if(m_CursorFlag)
		g_pDisplay->DrawTextEx( 
		m_WinRect.ox+m_CaptionRect.ox, 
		m_WinRect.oy+m_CaptionRect.oy, 
		m_CaptionRect.width, 
		m_CaptionRect.height, 
		"-", 
		m_TextSize,
		m_RowSpace, 
		drawColor.c, 
		m_TextStyle );
	}
	else 
	{
		CUIButton::DrawCaption();
	}

}

void CUIKeyEdit::UpdateSelf( float deltatime )
{
		
	//��˸���
	if( IsChecked() )
	{
		int dtick = int(deltatime*1000);
		m_CursorTimer += dtick;
		if( (m_CursorFlag&&m_CursorTimer>m_CursorLive) || (!m_CursorFlag&&m_CursorTimer>m_CursorDeath) )
		{
			m_CursorFlag = 1-m_CursorFlag;
			m_CursorTimer = 0;
		}
	}
	
	CUIButton::UpdateSelf( deltatime);

}

//��ӳ���в���ָ���Ķ�Ӧ��ϵ��ӳ��
//�����Ƿ�ɹ�
BOOL CUIKeyEdit::OnMapKey(unsigned int key)
{

	//������˽��keymap
	if(!m_KeyMap.empty())
	{
		if(m_KeyMap.find(key)==m_KeyMap.end())
		return FALSE;

		m_Value=key;
		m_Caption=m_KeyMap[key];		

	}
	else 
	{
		if(g_UICoreStyle.mapVirtualKey.find(key)==g_UICoreStyle.mapVirtualKey.end())
		return FALSE;

		m_Value=key;
		m_Caption=g_UICoreStyle.mapVirtualKey[key];		
	 
	}


	return TRUE;

}

int CUIKeyEdit::OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual)
{

	if(HasScriptFunc(SCRIPT_FUNC_ONCHAR))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONCHAR,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	if(!IsChecked()) return 1;


	char key[2];
	key[0]=(char)wparam;
	key[1]=0;

	if(IsDBCSChar(key,0)) return 1;
	
	
	//��ӡ�ַ�
	if(IsPrintableWord(wparam))
	{
		
		m_Caption=key;

		m_Value=wparam;		

		SetChecked(false);
		
	}

	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;

	
}


int CUIKeyEdit::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}
	
	switch(wparam) 
	{
	
	//�л�����״̬
	case VK_RETURN:
		{
		
			if(IsChecked())
			{
				OnMapKey(wparam);
				
				SetChecked(false);
			}
			else
			{
				SetChecked(true);
			}	
		}
		break;		
	
	//ȡ������״̬
	case VK_ESCAPE:
		{
			OnMapKey(wparam);

			SetChecked(false);
		}
		break;		
		
	default:
		
		//��ǰ������״̬,����,Ȼ��ȡ������״̬
		if(IsChecked())	
		{			
			
			if(OnMapKey(wparam))
			{	
				SetChecked(false);
			}
			
		}		
		break;
		

	}
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
	
	
	
}


 
int CUIKeyEdit::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{

	//�༭״̬
	//����յ�����ת�ƣ��򲻸ı�ֵ���ı���ȡ���༭��
	//����յ����̻�����꣬��ı�ֵ���ı���ȡ���༭��

	//�Ǳ༭״ֻ̬����������,enter������༭
	switch( msgtype )
	{
	
	case MSGTYPE_CHAR:		
	     return	OnChar( wparam, lparam );
		 break;

	case MSGTYPE_KEYDOWN:
		return OnKeyDown(wparam,lparam);		
		break;
			
	case MSGTYPE_LEFTBTNDOWN:
		
		m_State |= WGTSTATE_CLICKDOWN;	
		
		break;	
		
		

	//�л�����״̬
	case MSGTYPE_LEFTBTNUP:
		if(IsChecked())
		{
			OnMapKey(VK_LBUTTON);

			SetChecked(false);
			
		}
		else 
			SetChecked(true);	 

		if( m_State & WGTSTATE_CLICKDOWN )
			m_State &= ~WGTSTATE_CLICKDOWN;	

		break;	

	//�л�����״̬
	case MSGTYPE_RIGHTBTNUP:
		if(IsChecked())
		{
			OnMapKey(VK_RBUTTON);
			
			SetChecked(false);
		}
		else 
			SetChecked(true);	 
		
		
		break;	

		
	case MSGTYPE_LOSTFOCUS:
		SetChecked(false);
		break;
		
	default:
		return CUIButton::OnMessage( msgtype, wparam, lparam );
	}	
		
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}





//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIProgressBar-------------------------------------
//-------------------------------------------------------------------------------------------

CUIProgressBar::CUIProgressBar()
{
	m_Type = WGTTYPE_PROGRESSBAR;	

	m_pProgressCover=NULL;
	m_pProgressLayer=NULL;
	m_pProgressAimer=NULL;
}

CUIProgressBar::~CUIProgressBar()
{
	SAFE_RELEASE(m_pProgressCover);
	SAFE_RELEASE(m_pProgressLayer);
	SAFE_RELEASE(m_pProgressAimer);
}

void CUIProgressBar::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_CurAimerFrame=0;

	m_nPos=0;
	m_nLower=0;
	m_nUpper=100;
	m_nStep=10;
	
	m_nVisPos=0;

	m_DelayTime=0.2;
	m_SlowTime=0.0;

	m_CurSlowTimer=0.0;
	m_CurDelayTimer=0.0;

	SAFE_RELEASE(m_pProgressCover);
	SAFE_RELEASE(m_pProgressLayer);
	SAFE_RELEASE(m_pProgressAimer);
}

void CUIProgressBar::SetProgressAimer( CUIImage *pimg )
{
	if( pimg) pimg->IncRefCount();
	if( m_pProgressAimer) m_pProgressAimer->Release();
	m_pProgressAimer = pimg;
}

void CUIProgressBar::SetProgressAimer( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetProgressAimer( pimg );
	if( pimg ) pimg->Release();
}


void CUIProgressBar::SetProgressCover( CUIImage *pimg )
{
	if( pimg) pimg->IncRefCount();
	if( m_pProgressCover) m_pProgressCover->Release();
	m_pProgressCover = pimg;
}

void CUIProgressBar::SetProgressCover( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetProgressCover( pimg );
	if( pimg ) pimg->Release();
}


void CUIProgressBar::SetProgressLayer( CUIImage *pimg )
{
	if( pimg) pimg->IncRefCount();
	if( m_pProgressLayer) m_pProgressLayer->Release();
	m_pProgressLayer = pimg;
}

void CUIProgressBar::SetProgressLayer( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetProgressLayer( pimg );
	if( pimg ) pimg->Release();
}



void CUIProgressBar::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;

	if( (pelement=pvar->Element("step")) )  
		SetStep( pelement->Integer() );

	if( (pelement=pvar->Element("range")) )
	{
		int tmp[2];
		pelement->IntegerArray( tmp, 2 );
		SetRange( tmp[0], tmp[1] );
	}

	if( (pelement=pvar->Element("pos")) )  
		SetPos( pelement->Integer() );	

	
	if( (pelement=pvar->Element("delaytime")) )  
		SetDelayTime( pelement->Float() );	

	if( (pelement=pvar->Element("slowtime")) )  
		SetSlowTime( pelement->Float() );	
	
	if( (pelement=pvar->Element("progressaimer")) )  
		SetProgressAimer( pelement->String() );	

	if( (pelement=pvar->Element("progresscover")) )  
		SetProgressCover( pelement->String() );	

	if( (pelement=pvar->Element("progresslayer")) )  
		SetProgressLayer( pelement->String() );	

}


int  CUIProgressBar::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_SPINARROW:			
		
		if((int)(wparam) & WGTSTYLE_PRIMARY) 
		{ 
			if((m_nPos+m_nStep)>=m_nLower&&(m_nPos+m_nStep)<=m_nUpper) 
				m_nPos+=m_nStep;
		}
		else 
		{
			if((m_nPos-m_nStep)>=m_nLower&&(m_nPos-m_nStep)<=m_nUpper) 
				m_nPos-=m_nStep;
		}
		
		break;		
			
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}


void CUIProgressBar::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_POS:
		uiVar.fValue = m_nPos; 
		break;

	case VALUE_CHANNEL_MINVALUE:	
		uiVar.fValue = m_nLower;
		break;

	case VALUE_CHANNEL_MAXVALUE:	
		uiVar.fValue = m_nUpper;
		break;

	case VALUE_CHANNEL_STEP:	
		uiVar.fValue = m_nStep;
		break;

	default:
		CUIWidget::GetVar(uiVar);

	}
	
	
}

void CUIProgressBar::SetVar(stUIVar& uiVar)
{
	
	switch(uiVar.iChannel)
	{

	case VALUE_CHANNEL_VALUE:
		{
			SetPos(uiVar.fValue);
			m_nVisPos=m_nPos;
		}
		break;

	case VALUE_CHANNEL_POS:
		SetPos(uiVar.fValue);
		break;

	case VALUE_CHANNEL_MINVALUE:	
		m_nLower=(int)uiVar.fValue;
		break;

	case VALUE_CHANNEL_MAXVALUE:	
		m_nUpper=(int)uiVar.fValue;
		break;

	case VALUE_CHANNEL_STEP:	
		m_nStep=(int)uiVar.fValue;
		break;

	default: 
		CUIWidget::SetVar(uiVar);

	}
	
	
}



void CUIProgressBar::UpdateSelf( float deltatime )
{


	if(m_CurDelayTimer<=0.0)
	{
		if(m_CurSlowTimer<=0.0)
		{

			if(m_nVisPos<m_nPos)
			{
				m_nVisPos+=m_nStep;
				
				if(m_nVisPos>m_nPos)
					m_nVisPos=m_nPos;		
			}
			else if(m_nVisPos>m_nPos)
			{
				m_nVisPos-=m_nStep;

				if(m_nVisPos<m_nPos)
					m_nVisPos=m_nPos;
			}
			
			m_CurSlowTimer=m_SlowTime;

		}
		else 
		{	
			m_CurSlowTimer-=deltatime;		 
		}
		
	}
	else 
	{	 
		m_CurDelayTimer-=deltatime;
	}
	
	CUIWidget::UpdateSelf(deltatime);

}

stRect CUIProgressBar::GetProgressRect(int pos)
{

	stRect rc;
	
	if(m_Style&WGTSTYLE_VERTICAL) 
	{

		rc.ox=m_WinRect.ox+m_ClientRect.ox;
		rc.oy=m_WinRect.oy+m_ClientRect.oy+m_ClientRect.height*(1.0-(pos-m_nLower)/((m_nUpper-m_nLower)*1.0));
		rc.width=m_ClientRect.width;
		rc.height=m_ClientRect.height*((pos-m_nLower)/((m_nUpper-m_nLower)*1.0));		
		
	}
	else 
	{
		rc.ox=m_WinRect.ox+m_ClientRect.ox; 
		rc.oy=m_WinRect.oy+m_ClientRect.oy;
		rc.width=m_ClientRect.width*((pos-m_nLower)/((m_nUpper-m_nLower)*1.0));
		rc.height=m_ClientRect.height;	 

	}

	return rc;
	
}

void CUIProgressBar::OnPlusFrame()
{

	CUIWidget::OnPlusFrame();
		
	m_CurAimerFrame++;
	
}


void CUIProgressBar::OnAffirmFrame()
{
	CUIWidget::OnAffirmFrame();

	//�����Ч֡�ķ�Χ
	if(m_pProgressAimer)
	{
		if(m_CurAimerFrame>=m_pProgressAimer->GetNumFrame())
		   m_CurAimerFrame=0;
	}
	
}


//������������Ԫ�����
//layer:һ����(����vispos)
//cover:һ����(pos)
//aimer:һ����׼��(����vispos)
void CUIProgressBar::RenderSelf()
{
	
	CUIWidget::RenderSelf();


	stColorQuad drawColor=GetRenderColor(m_DrawColor);

	
	if(m_pProgressLayer)
	{
		stRect rc=GetProgressRect(m_nVisPos);

		g_pDisplay->PushClipRect(rc.ox,rc.oy,rc.width,rc.height);
		m_pProgressLayer->Draw(rc.ox,rc.oy,m_CurFrame,fxTexMul,drawColor.c);
		g_pDisplay->PopClipState();
	}
	

	if(m_pProgressCover)
	{
		stRect rc=GetProgressRect(m_nPos);

		g_pDisplay->PushClipRect(rc.ox,rc.oy,rc.width,rc.height);
		m_pProgressCover->Draw(rc.ox,rc.oy,m_CurFrame,fxTexMul,drawColor.c);
		g_pDisplay->PopClipState();
	}

	if(m_pProgressAimer&&(m_nPos<m_nVisPos))
	{
		stRect rc=GetProgressRect(m_nPos);
		rc.oy=rc.oy+(rc.height-m_pProgressAimer->GetHeight())*0.5;
		rc.ox=rc.ox+rc.width-m_pProgressAimer->GetWidth()*0.5;
		rc.width=m_pProgressAimer->GetWidth();
		rc.height=m_pProgressAimer->GetHeight();

		g_pDisplay->PushClipRect(rc.ox,rc.oy,rc.width,rc.height);
		m_pProgressAimer->Draw(rc.ox,rc.oy,m_CurAimerFrame,fxTexMul,drawColor.c);
		g_pDisplay->PopClipState();
	}

	DrawCaption();
	
	
}

void CUIProgressBar::SetRange( int nLower,  int nUpper )
{

	m_nLower=nLower;
	m_nUpper=nUpper;

}

void CUIProgressBar::GetRange( int& nLower, int& nUpper )
{

	nLower=m_nLower;
	nUpper=m_nUpper;

}

int  CUIProgressBar::GetPos( )
{
	return m_nPos;
}

void CUIProgressBar::SetPos( int nPos )
{

	if(nPos<m_nLower)
		nPos=m_nLower;
	else if(nPos>m_nUpper)
		nPos=m_nUpper;
		
	m_nPos=nPos;


	//��ʾ��ǰλ�øı�,���赱ǰ����Ч��ʱ��
	m_CurDelayTimer=m_DelayTime;

}
	
void CUIProgressBar::OffsetPos( int nPos )
{
	SetPos(m_nPos+nPos);
}

void CUIProgressBar::SetStep( int nStep )
{
	if(fabs((float)nStep)<=fabs((float)(m_nUpper-m_nLower))) 
		m_nStep=nStep;
	else
		m_nStep=1;
}



//-------------------------------------------------------------------------------------------
//---------------------------------------class CUIScrollBar-------------------------------------
//-------------------------------------------------------------------------------------------

CUIScrollBar::CUIScrollBar()
{
	m_Type = WGTTYPE_SCROLLBAR;	
}

CUIScrollBar::~CUIScrollBar()
{
	
}

void CUIScrollBar::ClearParameter()
{
	CUIWidget::ClearParameter();
	
	m_nPos=0;
	m_nLower=0;
	m_nUpper=0;
	m_nPageSize=32;

	m_MinBtnSize=SCROLLBAR_BTN_MIN_SIZE;
	m_MaxBtnSize=SCROLLBAR_BTN_MAX_SIZE;

	m_AccelTimer=0;
	m_AccelThreshold=0.4f;

	m_ExtendStyle=SCROLLBAR_AUTO_DISABLE;

}

//�Լ����ߺ���clickdown��
BOOL CUIScrollBar::IsScrollLocked()
{
	
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		if((*iter)->IsClickDown())
			return TRUE;
	}

	return FALSE;


}

void CUIScrollBar::AddChild( CUIWidget *pwidget )
{

	CUIWidget::AddChild(pwidget);
	
	if(pwidget->GetType()==WGTTYPE_BUTTON) 
	{ 
		CUIButton* pButton=(CUIButton*)pwidget;
		this->RegisterBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON,BUDDY_STYLE_SELF,"",pButton);		
	}
	else if(pwidget->GetType()==WGTTYPE_SPIN) 
	{ 
		CUISpin* pSin=(CUISpin*)pwidget;		
		pSin->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",this);
	}


}

void CUIScrollBar::DelChild( CUIWidget *pwidget )
{
	
	if(pwidget->GetType()==WGTTYPE_BUTTON) 
	{ 
		this->RegisterBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON,BUDDY_STYLE_SELF,"",0);
	}
	else if(pwidget->GetType()==WGTTYPE_SPIN) 
	{ 
		CUISpin* pSin=(CUISpin*)pwidget;		
		pSin->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",0);
	}

	CUIWidget::DelChild(pwidget);

}


void CUIScrollBar::SetVar(stUIVar& uiVar)
{

	switch(uiVar.iChannel)
	{

	case VALUE_CHANNEL_VALUE:
	case VALUE_CHANNEL_POS:
		SetScrollPos(uiVar.fValue,true); 
		break;

	case VALUE_CHANNEL_MAXVALUE:
		m_nUpper=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_MINVALUE:
		m_nLower=uiVar.fValue; 
		break;

	default:
		CUIWidget::SetVar(uiVar);

	}
	

}

void CUIScrollBar::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_VALUE:
	case VALUE_CHANNEL_POS:
		uiVar.fValue = GetScrollPos(); 
		break;
		
	case VALUE_CHANNEL_MAXVALUE:
		uiVar.fValue=m_nUpper; 
		break;

	case VALUE_CHANNEL_MINVALUE:
		uiVar.fValue=m_nLower; 
		break;

	default:
		CUIWidget::GetVar(uiVar);

	}
	
	
}



int  CUIScrollBar::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_CHILD_DRAGGING:
		{
			OnBlockBtnMove();
		}
		break;		

	case NTFTYPE_SPINARROW:			
		{
		
			if((int)(wparam) & WGTSTYLE_PRIMARY) 
			{ 
				OffScrollPos(m_nPageSize);
			}
			else 
			{
				OffScrollPos(-m_nPageSize);
			}
		
		}
		break;	

	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}

int CUIScrollBar::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{ 
		
	case VK_NEXT:		
	case VK_PRIOR:
	case VK_RIGHT:
	case VK_LEFT:
	case VK_UP:
	case VK_DOWN:
		{
			//ת����Ϣ
			CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);
			CUIWidget * pSecondBuddy = GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY_SECOND);
			if (pBuddy && pSecondBuddy) 
			{
				return (pBuddy->OnKeyDown(wparam,lparam,bVirtual) || pSecondBuddy->OnKeyDown(wparam,lparam,bVirtual));
			}


			if(pBuddy)
				return pBuddy->OnKeyDown(wparam,lparam,bVirtual);
			else
				return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
		}
		break;

	default:
		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
		
	}
	
	return 1;

}



int CUIScrollBar::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	switch( msgtype )
	{	
		
	case MSGTYPE_MOUSEWHEEL:
		{
			short offset=HIWORD(wparam);

			if(offset>0) OffScrollPos(m_MouseDelta);	
			else OffScrollPos(-m_MouseDelta);
		}
		break;


	case MSGTYPE_MOUSEMOVE:
		{	
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			m_ClickDownPos.x=x;
			m_ClickDownPos.y=y;

			//�ƶ���ʱ����ʱparsetooltip
			if(m_IdleTime<0.0f)
			{
				int nIndex=ParseDragIndex(x,y,UI_PARSE_EXACT);
				ParseTooltip(nIndex,GetWinRect(),x,y);
			}

		}
		break;
	
		
	case MSGTYPE_LEFTBTNDOWN:
		{
			m_State |= WGTSTATE_CLICKDOWN;
		
			HitAction(LOWORD(lparam),HIWORD(lparam));							
		}
	
		break;

		
	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );
	}
	
	
	
	
	return 1;
}


void CUIScrollBar::RenderSelf()
{

	CUIWidget::RenderSelf();

	if(m_ExtendStyle&SCROLLBAR_RENDER_SLOT)
		DrawSlotBar();

}

void CUIScrollBar::DrawSlotBar()
{

	if(m_nUpper<=m_nLower) return;

	CUIButton* pBlockButton=(CUIButton*)GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);
	
	stRect slotRect;
	
	stColorQuad drawColor=GetRenderColor(m_DrawColor);

	//��ֱ������
	if(m_Style&WGTSTYLE_VERTICAL) 
	{ 		
		
		int scrollRange=m_ClientRect.height;
	
		if(pBlockButton) 
			scrollRange-=pBlockButton->GetHeight();		


		slotRect.ox=m_WinRect.ox+m_ClientRect.ox;
		slotRect.width=m_ClientRect.width;


		//�ߵ�����(�·�������,�·�Ϊupper)
		if(m_ExtendStyle&SCROLLBAR_DIRECTION_FLIP)
		{
			slotRect.oy=m_WinRect.oy+m_ClientRect.oy;
			slotRect.height=(m_nPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);				
		}
		//��������(�Ϸ�������,�Ϸ�Ϊupper)
		else 
		{
			//����ָ�������λ��(����Ķ���)				
			slotRect.height=(m_nPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);				
			slotRect.oy=m_WinRect.oy+m_ClientRect.oy+scrollRange-slotRect.height;
		}
		
	}
	//ˮƽ������
	else 
	{

		int scrollRange=m_ClientRect.width;
	
		if(pBlockButton) 
			scrollRange-=pBlockButton->GetWidth();	


		slotRect.oy=m_WinRect.oy+m_ClientRect.oy;
		slotRect.height=m_ClientRect.height;
		
		
		//�ߵ�����(��������,��Ϊupper)
		if(m_ExtendStyle&SCROLLBAR_DIRECTION_FLIP)
		{
			//����ָ�������λ��(�������)
			slotRect.width=(m_nPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);	
			slotRect.ox=m_WinRect.ox+m_ClientRect.ox+scrollRange-slotRect.width;
		}
		//��������(�ҷ�������,�ҷ�Ϊupper)
		else 
		{						
			//����ָ�������λ��(������Ҳ�)
			slotRect.ox=m_WinRect.ox+m_ClientRect.ox;
			slotRect.width=(m_nPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);
		}
		
	}

	if((slotRect.width>0)&&(slotRect.height>0)	)
	{
		g_pDisplay->DrawBar(slotRect.ox,slotRect.oy,slotRect.width,slotRect.height,drawColor.c);
	}
	
}

void CUIScrollBar::OnAlignDirty()
{

	//���¶������к���
	CUIWidget::OnAlignDirty();


	UpdateBlockBtnSize();

	UpdateBlockBtnPos();


}


void CUIScrollBar::UpdateSelf( float deltatime )
{

	CUIButton* pBlockButton=(CUIButton*)GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);
	if(pBlockButton) 
	{
		if(IsVertical())
			pBlockButton->ShowWidget(m_ClientRect.height>0);
		else
			pBlockButton->ShowWidget(m_ClientRect.width>0);
	}

	
	if(m_State& WGTSTATE_CLICKDOWN) 
	{ 
		m_AccelTimer+=deltatime;
		if(m_AccelTimer>m_AccelThreshold) 
		{ 		
			HitAction(m_ClickDownPos.x,m_ClickDownPos.y);				
		}
		
	}
	else 
	{
		m_AccelTimer=0;
	}
	
	
	CUIWidget::UpdateSelf( deltatime );
	
}


void CUIScrollBar::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;

	if( (pelement=pvar->Element("range")) )
	{
		int tmp[2];
		pelement->IntegerArray( tmp, 2 );
		SetRange( tmp[0], tmp[1] );
	}

	if( (pelement=pvar->Element("minbtnsize")) )
	SetMinBtnSize( pelement->Integer());

	if( (pelement=pvar->Element("maxbtnsize")) )
	SetMaxBtnSize( pelement->Integer());

	if( (pelement=pvar->Element("pagesize")) )
	SetPageSize( pelement->Integer());

	if( (pelement=pvar->Element("pos")) )
	SetScrollPos( pelement->Integer(),true );
	
	

}

//�����Ӵ��ڱ��϶�ʱ���Զ����øú���
//����ֵ��ʾ�Ƿ��޸���x��y
//��֤���Ӳ������Լ��Ŀͻ���
int  CUIScrollBar::ChildQueryMoveTo(int &x,int &y,CUIWidget* pChild)
{

	if(!pChild) return 0;


	//�˴���Ҫ�޸�
	if(x<(m_WinRect.ox+m_ClientRect.ox)) 
	x= (m_WinRect.ox+m_ClientRect.ox);

	if(y<(m_WinRect.oy+m_ClientRect.oy)) 
	y= (m_WinRect.oy+m_ClientRect.oy);

	if((x+pChild->GetWidth())>(m_WinRect.ox+m_ClientRect.ox+m_ClientRect.width) )
	x=m_WinRect.ox+m_ClientRect.ox+m_ClientRect.width-pChild->GetWidth();
	
	if((y+pChild->GetHeight())>(m_WinRect.oy+m_ClientRect.oy+m_ClientRect.height) )
	y=m_WinRect.oy+m_ClientRect.oy+m_ClientRect.height-pChild->GetHeight();
		
	return 1;//ȱʡ�޸�

}

void CUIScrollBar::SetPageSize( int nPageSize )
{
	
	m_nPageSize=nPageSize;
	
}

void CUIScrollBar::UpdateBlockBtnSize()
{


	CUIButton* pBlockButton=(CUIButton*)GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);
	
	if(!pBlockButton) return;

	//�����϶���ť�Ĵ�С
	if((m_ExtendStyle&SCROLLBAR_BLOCKBTN_AUTO_SIZE)==0)  return;

	//��ֱ������,�����߶�
	if(IsVertical())
	{

		int size=m_ClientRect.height-(m_nUpper-m_nLower);
	
		if(size<m_MinBtnSize) 
			size=m_MinBtnSize;
		else if(size>m_MaxBtnSize) 
			size=m_MaxBtnSize;
		
		pBlockButton->SetHeight(size);
		pBlockButton->DefaultCaptionRect();
		pBlockButton->DefaultClientRect();
		pBlockButton->DefaultDragRect();

	}
	//ˮƽ������,�������
	else 
	{
	 
		int size=m_ClientRect.width-(m_nUpper-m_nLower);
				
		if(size<m_MinBtnSize) 
			size=m_MinBtnSize;
		else if(size>m_MaxBtnSize) 
			size=m_MaxBtnSize;

		
		pBlockButton->SetWidth(size);
		pBlockButton->DefaultCaptionRect();
		pBlockButton->DefaultClientRect();
		pBlockButton->DefaultDragRect();

	}

}


void CUIScrollBar::SetMaxValue(int value)
{
	m_nUpper=value;
	
	if(m_nLower>m_nUpper)
		m_nLower=m_nUpper;
}

void CUIScrollBar::SetMinValue(int value)
{
	m_nLower=value;
	
	if(m_nLower>m_nUpper)
		m_nUpper=m_nLower;
}


void CUIScrollBar::SetRange( int nLower,  int nUpper )
{

	//ȷ����Χ����(lower<upper)
	if(m_ExtendStyle&SCROLLBAR_AUTO_DISABLE)
		EnableWidget(nLower<nUpper);

	if(m_ExtendStyle&SCROLLBAR_AUTO_HIDE)
		ShowWidget(nLower<nUpper);

	if(nLower<nUpper)
	{
		m_nLower=nLower;
		m_nUpper=nUpper;

		CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);
		if(pBuddy) pBuddy->OnNotify(NTFTYPE_SCROLLBAR_RANGE,m_nPos,(unsigned int)this);

		CUIWidget * pSecondBuddy = GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY_SECOND);
		if (pSecondBuddy) 
		{
			pSecondBuddy->OnNotify(NTFTYPE_SCROLLBAR_RANGE,m_nPos,(unsigned int)this);
		}
		//�����쳣ֵ
		if((m_nPos<m_nLower)||(m_nPos>m_nUpper))
		{
			SetScrollPos(m_nLower,true);			
		}
		
		UpdateBlockBtnSize();
	}
	
}

void CUIScrollBar::GetRange( int& nLower, int& nUpper )
{

	nLower=m_nLower;
	nUpper=m_nUpper;

}

int  CUIScrollBar::GetScrollPos( )
{
	return m_nPos;
}

//���ù������Ĺ���λ��,Ĭ�ϲ�֪ͨ��鴰��
int  CUIScrollBar::SetScrollPos(int nPos, bool bNotify/*= false*/)	
{

	if(nPos<m_nLower) nPos=m_nLower;
	else if(nPos>m_nUpper) nPos=m_nUpper;

	m_nPos=nPos;
	
	UpdateBlockBtnPos();

	//�ı��˹�����λ��
	if(bNotify)		
	{			
		CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);
		if(pBuddy) pBuddy->OnNotify(NTFTYPE_SCROLLBAR_POS,m_nPos,(unsigned int)this);
		CUIWidget * pSecondBuddy = GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY_SECOND);
		if (pSecondBuddy) 
		{
			pSecondBuddy->OnNotify(NTFTYPE_SCROLLBAR_POS,m_nPos,(unsigned int)this);
		}
		CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_nPos);					
	}
		
	return m_nPos;
}
	
int  CUIScrollBar::OffScrollPos( int nPos )
{
	return SetScrollPos( m_nPos + nPos ,true );
}


void CUIScrollBar::HitAction(int x,int y)
{
	if(!IsPointInClientRect(x,y) ) return;

	CUIButton* pBlockButton=(CUIButton*)GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);

	if(!pBlockButton) return;

	//���⻬����˸
	if(pBlockButton->IsPointIn(x,y)) return;

	int rPos=m_nPos;

	if(m_Style&WGTSTYLE_VERTICAL) 
	{ 
		int scrollRange=m_ClientRect.height;

		if(pBlockButton)
			scrollRange-=pBlockButton->GetHeight();

		rPos=ScreenToLogic(y-(m_WinRect.oy+m_ClientRect.oy),scrollRange);

	}
	else 
	{	
		int scrollRange=m_ClientRect.width;

		if(pBlockButton)
			scrollRange-=pBlockButton->GetWidth();

		rPos=ScreenToLogic(x-(m_WinRect.ox+m_ClientRect.ox),scrollRange);
	}		


	if(rPos>m_nPos) 
		OffScrollPos(m_nPageSize);
	else if(rPos<m_nPos) 
		OffScrollPos(-m_nPageSize);			


}



//����ָ������ڿͻ�����������꣬����m_pos��ֵ
int CUIScrollBar::ScreenToLogic(int cursorPos,int scrollRange)
{

	int rPos=0;	


	//��ֱ������
	if(m_Style&WGTSTYLE_VERTICAL) 
	{ 	

		//�ߵ�����(�·�������,�·�Ϊupper)
		if(m_ExtendStyle&SCROLLBAR_DIRECTION_FLIP)
		{			
			if(scrollRange>0)
			rPos=int(cursorPos*(m_nUpper-m_nLower)*1.0f/scrollRange);
		}
		//��������(�Ϸ�������,�Ϸ�Ϊupper)
		else 
		{
			if(scrollRange>0)
			rPos=int((scrollRange-cursorPos)*(m_nUpper-m_nLower)*1.0f/scrollRange);	
		}		
	}
	//ˮƽ������
	else 
	{
		
		//�ߵ�����(��������,��Ϊupper)
		if(m_ExtendStyle&SCROLLBAR_DIRECTION_FLIP)
		{				
			if(scrollRange>0)
			rPos=int((scrollRange-cursorPos)*(m_nUpper-m_nLower)*1.0f/scrollRange);				
		}
		//��������(�ҷ�������,�ҷ�Ϊupper)
		else 
		{						
			if(scrollRange>0)
			rPos=int(cursorPos*(m_nUpper-m_nLower)*1.0f/scrollRange);		
		}
	}

	return (rPos+m_nLower);
}

//����tickPos��ֵ�����㵱ǰblockbtn��λ��
//��������ڿͻ������������
int CUIScrollBar::LogicToScreen(int tickPos,int scrollRange)
{


	int rPos=0;	

	//��ֱ������
	if(m_Style&WGTSTYLE_VERTICAL) 
	{ 				

		//�ߵ�����(�·�������,�·�Ϊupper)
		if(m_ExtendStyle&SCROLLBAR_DIRECTION_FLIP)
		{						
			if(m_nUpper>m_nLower)
			{
				//����ָ�������λ��(����ĵײ�)
				rPos=(tickPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);				
				
			}
			else if(m_ExtendStyle&SCROLLBAR_BLOCKBTN_DOCK_FLIP)				
			{
				rPos=scrollRange;			
			}				
			
		}
		//��������(�Ϸ�������,�Ϸ�Ϊupper)
		else 
		{						
			if((m_nUpper>m_nLower))
			{
				//����ָ�������λ��(����Ķ���)
				rPos=scrollRange-(tickPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);
				
			}
			else if((m_ExtendStyle&SCROLLBAR_BLOCKBTN_DOCK_FLIP)==0)
			{
				rPos=scrollRange;	
			}
			
		}
		
	}
	//ˮƽ������
	else 
	{
		
		
		//�ߵ�����(��������,��Ϊupper)
		if(m_ExtendStyle&SCROLLBAR_DIRECTION_FLIP)
		{
			
			if(m_nUpper>m_nLower)
			{

				//����ָ�������λ��(�������)
				rPos=scrollRange-(tickPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);	
				
			}
			else if((m_ExtendStyle&SCROLLBAR_BLOCKBTN_DOCK_FLIP)==0)
			{
				rPos=scrollRange;	
			}
			
		}
		//��������(�ҷ�������,�ҷ�Ϊupper)
		else 
		{
						
			
			if(m_nUpper>m_nLower)
			{
				//����ָ�������λ��(������Ҳ�)
				rPos=(tickPos-m_nLower)*scrollRange*1.0f/(m_nUpper-m_nLower);

			}
			else if(m_ExtendStyle&SCROLLBAR_BLOCKBTN_DOCK_FLIP)				
			{
				rPos=scrollRange;						
			}
						
		}
		
	}


	return rPos;
}

//��m_nPos�ı�ʱ�������ı们���λ��
//�����λ����m_nPos���������λ�����
void CUIScrollBar::SetBlockBtnPos()
{

	CUIButton* pBlockButton=(CUIButton*)GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);
	if(!pBlockButton) return;

	//��ֱ������
	if(m_Style&WGTSTYLE_VERTICAL) 
	{ 
		int scrollRange=m_ClientRect.height-pBlockButton->GetHeight();

		int py=LogicToScreen(m_nPos,scrollRange);
		
		py+=m_WinRect.oy+m_ClientRect.oy;
		
		pBlockButton->MoveToY(py);		
			
	}
	//ˮƽ������
	else 
	{
		int scrollRange=m_ClientRect.width-pBlockButton->GetWidth();

		int px=LogicToScreen(m_nPos,scrollRange);
		
		px+=m_WinRect.ox+m_ClientRect.ox;
		
		
		pBlockButton->MoveToX(px);	
		
	}
}


//��m_nPos�ı�ʱ�������ı们���λ��
//�����λ����m_nPos���������λ�ÿ��Բ����
//������ݻ���ĵ�ǰλ�ü������m_nPosֵ����m_nPos��ǰֵ��ȣ�������ı们��λ��
void CUIScrollBar::UpdateBlockBtnPos()
{

	CUIButton* pBlockButton=(CUIButton*)GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);
	if(!pBlockButton) return;		

	//��ֱ������
	if(m_Style&WGTSTYLE_VERTICAL) 
	{ 
		int scrollRange=m_ClientRect.height-pBlockButton->GetHeight();		

		int cursorPos=pBlockButton->GetOriginY()-(m_WinRect.oy+m_ClientRect.oy);
		int yTick=ScreenToLogic(cursorPos,scrollRange);	

		if(yTick==m_nPos) return;

		int py=LogicToScreen(m_nPos,scrollRange);
		py+=m_WinRect.oy+m_ClientRect.oy;
	
		pBlockButton->MoveToY(py);		
			
	}
	//ˮƽ������
	else 
	{
		int scrollRange=m_ClientRect.width-pBlockButton->GetWidth();		

		int cursorPos=pBlockButton->GetOriginX()-(m_WinRect.ox+m_ClientRect.ox);
		int xTick=ScreenToLogic(cursorPos,scrollRange);	

		if(xTick==m_nPos) return;

		int px=LogicToScreen(m_nPos,scrollRange);
		px+=m_WinRect.ox+m_ClientRect.ox;
				
		pBlockButton->MoveToX(px);	
		
	}

}

//�������λ�øı�ʱ�������ı�m_nPos��ֵ
//ͬʱ֪ͨ��鴰��
void CUIScrollBar::OnBlockBtnMove()
{

	CUIButton* pBlockButton=(CUIButton*)GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);
	
	if(!pBlockButton) return;	 	
	

	//��ֱ������
	if(m_Style&WGTSTYLE_VERTICAL) 
	{ 
		int cursorPos=pBlockButton->GetOriginY()-(m_WinRect.oy+m_ClientRect.oy);
		int scrollRange=m_ClientRect.height-pBlockButton->GetHeight();		

		int yTick=ScreenToLogic(cursorPos,scrollRange);	

		SetScrollPos(yTick,true);	
		
	}
	//ˮƽ������
	else 
	{

		int cursorPos=pBlockButton->GetOriginX()-(m_WinRect.ox+m_ClientRect.ox);
		int scrollRange=m_ClientRect.width-pBlockButton->GetWidth();		

		int xTick=ScreenToLogic(cursorPos,scrollRange);	

		SetScrollPos(xTick,true);
		
	}
	
	

}


//------------------------------------------------------------------------------------------
//----------------------------------class CUISwitcher-----------------------------------------
//------------------------------------------------------------------------------------------


CUISwitcher::CUISwitcher()
{
	m_Type=WGTTYPE_SWITCHER;	
}

CUISwitcher::~CUISwitcher()
{

}

void CUISwitcher::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_CurGroupId=-1;
}
	
void CUISwitcher::SwitchTo(int grpid)
{

	for(int i=0;i<m_Children.size();i++)
	{ 
		//ֻ����������groupid���Ӵ���
		if(m_Children[i]->GetGroupId()>-1)
		m_Children[i]->ShowWidget((m_Children[i]->GetGroupId()==grpid));
	}

}

void CUISwitcher::OnInit()
{
	
	SwitchTo(m_CurGroupId);

	CUIWidget::OnInit();
}

void CUISwitcher::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
	if( (pelement=pvar->Element("switchto")) )
	m_CurGroupId=pelement->Integer();
	
}


void   CUISwitcher::SetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_SWITCH:
		SwitchTo((int)uiVar.fValue);
		break;

	default:
		CUIWidget::SetVar(uiVar);

	}


}



//------------------------------------------------------------------------------------------
//----------------------------------class CUIShaper-----------------------------------------
//------------------------------------------------------------------------------------------
CUIShaper::CUIShaper()
{
	m_Type=WGTTYPE_SHAPER;
}

CUIShaper::~CUIShaper()
{	
	
}

void CUIShaper::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_ShapeMode=UI_SHAPEMODE_NORMAL;

	m_DrawColor=stColorQuad(128,128,128,255);
	m_FocusColor=stColorQuad(255,0,0,255);

}


void CUIShaper::SetShapeMode(int mode)
{
	m_ShapeMode=mode;
}

void CUIShaper::AddLine(int x0,int y0,int x1,int y1,int color)
{
	m_Lines.push_back(stUIShape(x0,y0,x1,y1,color));
}

//����ڴ���ԭ�����
void CUIShaper::RenderLines()
{
	
	for(int i=0;i<m_Lines.size();i++)
	{
		g_pDisplay->DrawLine(m_Lines[i].sx+m_WinRect.ox,m_Lines[i].sy+m_WinRect.oy,m_Lines[i].ex+m_WinRect.ox,m_Lines[i].ey+m_WinRect.oy,m_Lines[i].color);
	}
}


void CUIShaper::RenderSelf()
{
	
	CUIWidget::RenderSelf();

	if(m_ShapeMode==UI_SHAPEMODE_NORMAL)
	RenderLines();
	

	
}

void CUIShaper::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
		
	if((pelement=pvar->Element("lines")))
	{
		
		CVarArray* pLine=(CVarArray*)pelement;
		
		int v[5];
		CVarArray* pPoint;
		CVar* pValue;
		
		for(int i=0;i<pLine->GetNum();i++)
		{ 
			
			pPoint=(CVarArray*)(pLine->Element(i));
			
			//Ĭ��ʹ�ô��ڻ�����ɫ
			v[4]=m_DrawColor.c;

			for(int j=0;j<pPoint->GetNum();j++)
			if(j<5)
			{				
				pValue=(CVar*)(pPoint->Element(j));
				v[j]=pValue->Integer();
			}
			
			AddLine(v[0],v[1],v[2],v[3],v[4]);
			
		}
		
	}
	
	
	
	
}

//------------------------------------------------------------------------------------------
//----------------------------------class CUITabList-----------------------------------------
//------------------------------------------------------------------------------------------
CUITabList::CUITabList()
{	
	m_Type = WGTTYPE_TABLIST;
}

CUITabList::~CUITabList()
{	
	
}

int  CUITabList::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
	
	case NTFTYPE_CHILD_SHOWHIDE:
	case NTFTYPE_COMMAND_CHECK:
		{
			SetUpdateDirty(true);
		}
		break;

	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}


void CUITabList::ArrangeContent()
{

	bool bDirty=false;

	//�õ�����groupstop���е����пɼ�����
	std::vector<CUIWidget*>vecVisChild;
	
	CUIWidget* pSelTab=NULL;
	stRect absClientRct=GetAbsoluteClientRect();
	int SAllH=0;//���пɼ����ӵĸ߶�֮��
	int SMinH=0;//����δѡ�еĿɼ����ӵ���С�߶�֮��
	int SMaxH=0;//ѡ�к��ӿ��ܵ����߶�
	int SShrinkH=0;//����Ϊѡ�пɼ�������Ҫ�����ĸ߶�֮��

	std::vector<CUIWidget*>::iterator iter;
	for(iter=m_Children.begin();iter!=m_Children.end();iter++)
	{ 
		CUIWidget* pChild=(*iter);
		
		if(!pChild->IsVisible()) continue;
		if(pChild->GetGroupId()<0) continue;
		if(pChild->GetGroupStop()<0) continue;
		
		int childH=pChild->GetHeight();
		int childMinH=pChild->GetMinHeight();

		SAllH+=childH;

		if(pChild->IsChecked()) 
			pSelTab=pChild;
		else
			SMinH+=childMinH;			
		
		vecVisChild.push_back(pChild);
	
	}

	SMaxH=absClientRct.height-SMinH;

	//���ڰ���groupstop����
	std::sort( vecVisChild.begin(),vecVisChild.end(), SortWidgetByGroupStop );

	//����ѡ�к��ӣ�������ܣ���������չ���ڳߴ磬��ȷ�����ڲ������������߶�SMaxH
	if(pSelTab)
	{
		//��סѡ�к��ӵĸ߶�
		int selH=pSelTab->GetHeight();

		//���ڻ�������չ����
		int extendH=SMaxH-selH;

		int plusH=m_PathSpeed;

		//�ͻ����пհף�����seltab�����
		if(absClientRct.height>SAllH)
			plusH+=absClientRct.height-SAllH;

		//�����������(extendH<0),ֱ��������ָ���ߴ�
		//���������չ�����ÿ�β�����m_PathSpeed
		if(plusH>extendH)
			plusH=extendH;
		
		//��չ����
		pSelTab->ExtendEdge(plusH,EDGE_FLAG_DOWN,pSelTab->GetAjustFlag());

		//ʵ����չ�ĸ߶�
		plusH=pSelTab->GetHeight()-selH;
		if(plusH!=0) bDirty=true;
		
		//�������пɼ��Ӵ��ڵĸ߶Ⱥ�
		SAllH+=plusH;
	}
		
	//���пɼ����ӵĸ߶Ⱥͳ����ͻ���
	if(SAllH>absClientRct.height)	
		SShrinkH=absClientRct.height-SAllH;
	
	//���������δѡ�д���
	int EndY=0;
	for(iter=vecVisChild.begin();iter!=vecVisChild.end();iter++)
	{ 
		CUIWidget* pChild=(*iter);

		pChild->MoveTo(absClientRct.ox,absClientRct.oy+EndY);

		//ѡ�к��Ӹ߶Ȳ��ᳬ��SMaxH
		if(!pChild->IsChecked()&&(SShrinkH<0))
		{	
			//��סѡ�к��ӵĸ߶�
			int childH=pChild->GetHeight();
			
			int plusH=SShrinkH;

			//��չ����
			pChild->ExtendEdge(plusH,EDGE_FLAG_DOWN,pSelTab->GetAjustFlag());

			plusH=childH-pChild->GetHeight();
			if(plusH!=0) bDirty=true;

			SShrinkH+=plusH;
			
			
		}

		EndY+=pChild->GetHeight();		

	}

	

	SetUpdateDirty(bDirty);

}

void CUITabList::UpdateSelf( float deltatime )
{

	if(IsUpdateDirty())
	{
		ArrangeContent();				
	}

	CUIWidget::UpdateSelf(deltatime);

}

//------------------------------------------------------------------------------------------
//----------------------------------class CUIContainer-----------------------------------------
//------------------------------------------------------------------------------------------
CUIContainer::CUIContainer()
{	
	m_Type = WGTTYPE_CONTAINER;	
}

CUIContainer::~CUIContainer()
{	
	
}

void CUIContainer::ClearParameter()
{
	CUIWidget::ClearParameter();	
	
	m_NumPage=Point2D(0,0);
	m_CurPage=Point2D(0,0);

	
	m_ItemMarginH=2;
	m_ItemMarginV=2;

	m_ContentRect=stRect(0,0,0,0);

	m_ExtendStyle=WGTLIST_STYLE_WHEEL;

}


void CUIContainer::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );

	if(!pvar) return;
	CVar *pelement=NULL;


	if( (pelement=pvar->Element("itemmarginh")) )
		SetItemMarginH( pelement->Integer());
	
	if( (pelement=pvar->Element("itemmarginv")) )
		SetItemMarginV( pelement->Integer());
	
}

void CUIContainer::PostUpdateSelf( float deltatime )
{	
	
	if(IsUpdateDirty()) 
	{
		UpdateScrollBar();	
		
		SetUpdateDirty(false);
	}
	
	CUIWidget::PostUpdateSelf(deltatime);
	
	
}

int CUIContainer::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{ 
		
	case VK_NEXT:
		
		GotoPageV(GetCurPageV()+1);
		
		break;
		
	case VK_PRIOR:
		
		GotoPageV(GetCurPageV()-1);
		
		break;
		

	case VK_RIGHT:
		
		//����ctrl��
		if(lparam&MK_CONTROL)
		GotoPageH(GetCurPageH()+1);
		else 
		OffsetContentH(-1);


		break;
		
	case VK_LEFT:
		
		if(lparam&MK_CONTROL)
		GotoPageH(GetCurPageH()-1);
		else 
		OffsetContentH(1);
		
		break;

		
	case VK_UP:
		
		//����ctrl��
		if(lparam&MK_CONTROL)
			GotoPageV(GetCurPageV()-1);
		else if(m_ExtendStyle&WGTLIST_STYLE_ENUM_ITEM)
		{	
			stUIVar uiVar(VALUE_CHANNEL_SEL,0,-1.0,0);
			GetVar(uiVar);

			int sel=uiVar.fValue;

			SetVar(stUIVar(VALUE_CHANNEL_SEL,0,sel-1,0));


		}
		else 
			OffsetContentV(1);
		
		break;
		
	
	case VK_DOWN:
		
		//����ctrl��
		if(lparam&MK_CONTROL)
			GotoPageV(GetCurPageV()+1);
		else if(m_ExtendStyle&WGTLIST_STYLE_ENUM_ITEM)
		{	
			stUIVar uiVar(VALUE_CHANNEL_SEL,0,-1.0,0);
			GetVar(uiVar);

			int sel=uiVar.fValue;

			SetVar(stUIVar(VALUE_CHANNEL_SEL,0,sel+1,0));

		}
		else
			OffsetContentV(-1);
		
		break;
	
		
	default:
		
		
		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);
		
	}
	
	return 1;
}


int  CUIContainer::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
	
	case NTFTYPE_COMMAND_CHECK:
		{
			CUIWidget* pWgt=(CUIWidget*)lparam;
			
			if(pWgt)
				EnsureItemVisible(pWgt->GetGroupStop());

		}
		break;

	case NTFTYPE_SCROLLBAR_POS:
		{
			CUIScrollBar* pBar=(CUIScrollBar*)lparam;
			if(pBar)
			{
				
				if(pBar->IsVertical())
				{					
					ScrollContentV((int)wparam);	
				}
				else 
				{
					ScrollContentH(-(int)wparam);	
				}
				
			}
			

		}
		break;
		
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}

int  CUIContainer::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch( msgtype )
	{
		
	case MSGTYPE_MOUSEWHEEL:
		{
			
			int nIndex=0;
			short offset=HIWORD(wparam);			
			
			//֧�������ֹ���
			if(m_ExtendStyle&WGTLIST_STYLE_WHEEL)
			{
				//������Ŀ����
				if(m_ExtendStyle&WGTLIST_STYLE_ENUM_ITEM)
				{
					//���ϳ��¹���
					if(offset>0)
						nIndex=-1;
					else
						nIndex=1;

					stUIVar uiVar(VALUE_CHANNEL_SEL,0,-1.0,0);
					GetVar(uiVar);

					int sel=uiVar.fValue+nIndex;
					SetVar(stUIVar(VALUE_CHANNEL_SEL,0,sel,0));



				}
				else //�������ع���
				{
					if(offset>0) OffsetContentV(m_MouseDelta);	
					else OffsetContentV(-m_MouseDelta);
				}
								
			}
			
		}
		
		
		break;	
	
		
		
	case MSGTYPE_KEYDOWN:
		return OnKeyDown( wparam, lparam );
		break;
		
		
		
	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );
	}
	
	
	
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}


void CUIContainer::AjustSelf()
{
	UpdateScrollBar();

	SetUpdateDirty(false);
}

void CUIContainer::ArrangeContent()
{

	
	m_ContentRect.width=0;
	m_ContentRect.height=0;

	//��������λ��
	int exmax=0;
	int eymax=0;

	int ex;
	int ey;

	for(int i=0;i<m_Children.size();i++)
	{ 
		if(!(m_Children[i]->IsVisible()))
			continue;

		ex=m_Children[i]->GetEndX();
		ey=m_Children[i]->GetEndY();
		
		if(ey>eymax) eymax=ey;
		if(ex>exmax) exmax=ex;
		
	}
	

	m_ContentRect.width=exmax-(m_WinRect.ox+m_ClientRect.ox);
	m_ContentRect.height=eymax-(m_WinRect.oy+m_ClientRect.oy);
	
	m_NumPage.x=ceil(m_ContentRect.width*1.0/m_ClientRect.width);
	m_NumPage.y=ceil(m_ContentRect.height*1.0/m_ClientRect.height);



}


void CUIContainer::UpdateScrollBar()
{	
	
	ArrangeContent();

	CUIWidget* pBuddy;

	//������ֱ����
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar* )pBuddy;
				
		int nLower=m_ClientRect.height-m_ContentRect.height;
		if(nLower>0) nLower=0;
	
		m_ContentRect.oy=0;
		m_CurPage.y=0;	

		pScrollBar->SetRange(nLower,0);
		pScrollBar->SetScrollPos(0);	
		
	}

	//����ˮƽ����
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar* )pBuddy;		
		
		int nUpper=m_ContentRect.width-m_ClientRect.width;
		if(nUpper<0) nUpper=0;
			
		m_ContentRect.ox=0;
		m_CurPage.x=0;	

		pScrollBar->SetRange(0,nUpper);
		pScrollBar->SetScrollPos(0);	

	}
	
}



void CUIContainer::OffsetContentH(int offset)
{
	
	if(offset<(m_ClientRect.width-m_ContentRect.width-m_ContentRect.ox)) 
	offset=(m_ClientRect.width-m_ContentRect.width-m_ContentRect.ox);

	if(offset>-m_ContentRect.ox) 	offset=-m_ContentRect.ox;
		
	
	for(int i=0;i<m_Children.size();i++)
	{
		m_Children[i]->Move(offset,0);
	}

	m_ContentRect.ox+=offset;
	
	m_CurPage.x=ceil(abs(m_ContentRect.ox)/(m_ClientRect.width*1.0));
	
	CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY);	
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar* )pBuddy;
		pScrollBar->SetScrollPos(-m_ContentRect.ox);
	}

}


void CUIContainer::OffsetContentV(int offset)
{
	
	if(offset<(m_ClientRect.height-m_ContentRect.height-m_ContentRect.oy)) 
	offset=(m_ClientRect.height-m_ContentRect.height-m_ContentRect.oy);

	if(offset>-m_ContentRect.oy) 	offset=-m_ContentRect.oy;
			
	for(int i=0;i<m_Children.size();i++)
	{
		m_Children[i]->Move(0,offset);
	}

	m_ContentRect.oy+=offset;	
		
	m_CurPage.y=ceil(abs(m_ContentRect.oy)/(m_ClientRect.height*1.0));
	
	CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);	
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar* )pBuddy;
		pScrollBar->SetScrollPos(m_ContentRect.oy);
	}
	
}

//posΪ�����ԭʼλ�õ�ƫ��,ֻ���Ǹ�ֵ
void CUIContainer::ScrollContentV(int pos)
{
	OffsetContentV(pos-m_ContentRect.oy);
}


//posΪ�����ԭʼλ�õ�ƫ��,ֻ���Ǹ�ֵ
void CUIContainer::ScrollContentH(int pos)
{
	OffsetContentH(pos-m_ContentRect.ox);
}



void CUIContainer::EnsureItemVisible(int index)
{

	for(int i=0;i<m_Children.size();i++)
	{ 			
		CUIWidget* pWgt=m_Children[i];

		if(!(pWgt->IsVisible()))
			continue;

		if(pWgt->GetGroupStop()==index)
		{
			stRect refRct=pWgt->GetWinRect();
			stRect absClntRct=GetAbsoluteClientRect();

			refRct.ox-=absClntRct.ox;
			refRct.oy-=absClntRct.oy;

			absClntRct.ox=0;
			absClntRct.oy=0;

			int visRelation=IsRectIntersect(refRct,absClntRct);							

			if((visRelation==RECT_TOTAL_INSIDE)||(visRelation==RECT_TOTAL_OUTSIDE)) return;

			Point2D offset(0,0);

			if((refRct.oy+refRct.height)>absClntRct.height)//item�ײ������ͻ����ײ�
			{

				offset.y=absClntRct.height-(refRct.oy+refRct.height);
				offset.y-=m_ItemMarginV;
			}
			else if(refRct.oy<0)//item���������ͻ�������
			{
				offset.y=-refRct.oy;
				offset.y+=m_ItemMarginV;
			}

			if((refRct.ox+refRct.width)>absClntRct.width)//item�ұ߳����ͻ����ұ�
			{
				
				offset.x=absClntRct.width-(refRct.ox+refRct.width);
				offset.x-=m_ItemMarginH;
			}
			else if(refRct.ox<0)//item��߳����ͻ������
			{
				offset.x=-refRct.ox;
				offset.x+=m_ItemMarginH;
			}

			OffsetContentH(offset.x);
			OffsetContentV(offset.y);

			return;
		}
			
	}
	
}


int  CUIContainer::GetCurPageV()
{
	return m_CurPage.y;
}

int  CUIContainer::GetCurPageH()
{
	return m_CurPage.x;
}

int  CUIContainer::GetNumPageV()
{
	return m_NumPage.y;
}


int  CUIContainer::GetNumPageH()
{
	return m_NumPage.x;
}

void CUIContainer::GotoPageV(int nPage)
{
	
	if(nPage>=m_NumPage.y) nPage=m_NumPage.y-1;

	if(nPage<0) nPage=0;
	
	int pos=-m_ClientRect.height*nPage;
	ScrollContentV(pos);

}

void CUIContainer::GotoPageH(int nPage)
{
	
	if(nPage>=m_NumPage.x) nPage=m_NumPage.x-1;
	
	if(nPage<0) nPage=0;
	
	int pos=-m_ClientRect.width*nPage;
	ScrollContentH(pos);
	
}



//------------------------------------------------------------------------------------------
//----------------------------------class CUITextList-----------------------------------------
//------------------------------------------------------------------------------------------
CUITextList::CUITextList()
{
	m_Type = WGTTYPE_TEXTLIST;

}
	
CUITextList::~CUITextList()
{	
	m_AllLines.clear();
}

void CUITextList::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_ExtendStyle=TEXTLIST_STYLE_AUTOSCROLL;

	m_LineMargin=4;

	m_NumPage=0;
	m_CurPage=0;
	m_CurScrollPos=0;

	m_MaxLine=200;
	
	m_ScrollSpeed=1;
	m_LockTime=4.0f;
	m_ScrollTimer=0.0f;

	m_AllLines.clear();

	m_TitleRect=stRect(0,0,0,0);

}

//���scroll�Ǻ���,�Զ�ע��
void CUITextList::AddChild( CUIWidget *pwidget )
{
	
	CUIWidget::AddChild(pwidget);
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{
		
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",this);

		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",pScrollBar);
		
		
	}
	
	
}

void CUITextList::DelChild( CUIWidget *pwidget )
{
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{
		
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",0);
		
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",0);
		
	}
	
	CUIWidget::DelChild(pwidget);
	
}



void CUITextList::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );
	
	int tmp[4];

	if(!pvar) return;
	CVar *pelement=NULL;
	
	if( (pelement=pvar->Element("scrollspeed")) )
	m_ScrollSpeed=pelement->Integer();

	if( (pelement=pvar->Element("locktime")) )
	m_LockTime=pelement->Double();

	if( (pelement=pvar->Element("maxline")) )
	m_MaxLine=pelement->Integer();
	
	if( (pelement=pvar->Element("linemargin")) )
	m_LineMargin=pelement->Integer();

	if( (pelement=pvar->Element("titlerect")) )
	{
		pelement->IntegerArray(tmp,4);
		m_TitleRect=stRect(tmp[0],tmp[1],tmp[2],tmp[3]);
	}

	
}



int	CUITextList::GetTable(int channel,lua_State *ls)
{
	if(channel==VALUE_CHANNEL_ITEMDATA)
	{
		if(lua_gettop(ls)<UI_TABLE_PARAM_START) 
			return 0;

		int index=lua_tonumber(ls,UI_TABLE_PARAM_START);

		const stUILineData* pdata=GetLineData(index);

		if(!pdata)	return 0;

		lua_newtable(ls);

		SetTableElement(ls,"intdata",pdata->intData);
		SetTableElement(ls,"strdata",pdata->strData);

		return 1;

	}
	else
		return CUIWidget::GetTable(channel,ls);	
}

int CUITextList::SetTable(int channel,lua_State *ls)
{

	if(channel==VALUE_CHANNEL_ITEMDATA)
	{
		if(lua_gettop(ls)<UI_TABLE_PARAM_START) 
			return 0;

		char pText[UI_DEFAULT_BUFSIZE];		
		GetTableElement(ls,"caption",pText,UI_DEFAULT_BUFSIZE,UI_TABLE_PARAM_START);
		unsigned int color=GetTableElementInt(ls,"color",UI_TABLE_PARAM_START);

		char strData[UI_LINEDATA_LEN];
		GetTableElement(ls,"strdata",strData,UI_LINEDATA_LEN,UI_TABLE_PARAM_START);
		unsigned int intData=GetTableElementInt(ls,"intdata",UI_TABLE_PARAM_START);

		stUILineData data(intData,strData);
		
		PushLine(pText,color,&data);
		
		return 0;

	}
	else
		return CUIWidget::SetTable(channel,ls);	

}


void CUITextList::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_SCROLLSPACE:
		uiVar.fValue = (m_CurScrollPos+m_ContentHeight-m_ClientRect.height);
		break;

	case VALUE_CHANNEL_POS:
		uiVar.fValue = m_CurScrollPos;
		break;

	case VALUE_CHANNEL_ITEM_NUM:
		uiVar.fValue =  m_AllLines.size();
		break;

	case VALUE_CHANNEL_ITEMTEXT:
		uiVar.sValue = GetLineText(uiVar.iIndex);
		break;

	default:
		CUIWidget::GetVar(uiVar);

	}


}

void  CUITextList::SetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_POS:
		{

			UpdateScrollBar();
				
			ScrollContent(int(uiVar.fValue));

			SetUpdateDirty(false);		
			
		}
		break;

	case VALUE_CHANNEL_ITEM_NUM:
		{	
			int eraseNum=m_AllLines.size()-int(uiVar.fValue);

			if(eraseNum>0)
				m_AllLines.erase(m_AllLines.begin(),m_AllLines.begin()+eraseNum);

			SetUpdateDirty(true);

		}
		break;

	//text////////////////////////////////////////////////////////////////////////

	case VALUE_CHANNEL_ITEMTEXT:
		PushLine(uiVar.sValue.c_str(),uiVar.iIndex);
		break;

	default:
		CUIWidget::SetVar(uiVar);

	}
	
}

int  CUITextList::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		

	case NTFTYPE_SCROLLBAR_POS:
		{	
			m_ScrollTimer=0.0f;
			ScrollContent((int)wparam);			
		}
		break;
		
		
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}

const char* CUITextList::GetLineText(int nIndex)
{
	static std::string strTmp;

	strTmp="";

	if((nIndex>=0)&&(nIndex<m_AllLines.size())) 
		m_AllLines[nIndex].richTxt.GetRawString(strTmp);

	return strTmp.c_str();

}


const stUILineData*  CUITextList::GetLineData(int nIndex)
{
	const stUILineData* pdata=NULL;
	
	if((nIndex>=0)&&(nIndex<m_AllLines.size())) 
		pdata=&(m_AllLines[nIndex].linedata);

	return pdata;

}


int  CUITextList::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch( msgtype )
	{

	case MSGTYPE_MOUSEWHEEL:
		{
			m_ScrollTimer=0.0f;
			
			short offset=HIWORD(wparam);

			if(offset>0) OffsetContent(m_MouseDelta);	
			else OffsetContent(-m_MouseDelta);
		}
		break;

	

	case MSGTYPE_KEYDOWN:
		return OnKeyDown( wparam, lparam );
		break;


    case MSGTYPE_LEFTBTNDOWN:	
		{
			m_ScrollTimer=0.0f;

			m_State |= WGTSTATE_CLICKDOWN;

			//���λ��
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);

			int nIndex=FindLineOnPoint(x,y);							

			CallSafeScriptEx(SCRIPT_FUNC_ON_CLICK,"%d,%d,%d",nIndex,wparam,lparam);
							
		}

		break;

	case MSGTYPE_LBTNDBLCLK:
		{
			m_ScrollTimer=0.0f;

			//���λ��
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);

			int nIndex=FindLineOnPoint(x,y);							

			CallSafeScriptEx(SCRIPT_FUNC_ON_DBCLICK,"%d,%d,%d",nIndex,wparam,lparam);

							
		}
		break;

	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );
	}
	
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}


int CUITextList::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{ 
	

	case VK_UP:
		{
			m_ScrollTimer=0.0f;
			OffsetContent(m_TextSize+m_RowSpace);
		}		
		break;

	case VK_DOWN:
		{				
			m_ScrollTimer=0.0f;
			OffsetContent(-m_TextSize-m_RowSpace);			
		}
		break;

	
	case VK_NEXT:

		m_ScrollTimer=0.0f;
		GotoPage(m_CurPage+1);

		break;

	case VK_PRIOR:
		
		m_ScrollTimer=0.0f;
		GotoPage(m_CurPage-1);
		
		break;
		

	default:

		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);

	}

	return 1;
}


void CUITextList::UpdateSelf( float deltatime )
{

	//�����ڳߴ�ı��ʱ�򣬴������λ��
	if((m_CurScrollPos<(m_ClientRect.height-m_ContentHeight))||(m_CurScrollPos>0))
	{
		SetUpdateDirty(true);
	}
	BOOL bBlockScroll = FALSE;
	bBlockScroll = (m_ExtendStyle & TEXTLIST_STYLE_BLOCKINMIDDLE) && (m_CurScrollPos != m_ClientRect.height-m_ContentHeight);
	if ( bBlockScroll && m_AllLines.size() > 0 )
	{
		stTextLine& lastline = m_AllLines.back();
		if ( m_ContentHeight - m_ClientRect.height <= lastline.height + m_RowSpace )
			bBlockScroll = FALSE;
	}
	if(IsUpdateDirty()) 
	{
		UpdateScrollBar();
		
		//�Զ�������������ӵ���
		if( (m_ExtendStyle&TEXTLIST_STYLE_AUTOSCROLL) && !bBlockScroll)
		{
			if(m_ExtendStyle&TEXTLIST_STYLE_REVERSE)
				ScrollToBegin();
			else
				ScrollToEnd();
		}		

		SetUpdateDirty(false);
	}

	CUIWidget* pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pBuddy;
		if(pScrollBar->IsScrollLocked())
			m_ScrollTimer=0.0f;
	}

	if(IsClickDown()) m_ScrollTimer=0.0f;

	if( (m_ExtendStyle&TEXTLIST_STYLE_DELAYSCROLL) && !bBlockScroll )
	{
		m_ScrollTimer+=deltatime;
		if(m_ScrollTimer>=m_LockTime)
		{
			m_ScrollTimer=m_LockTime;

			if(m_ExtendStyle&TEXTLIST_STYLE_REVERSE)
				OffsetContent(m_ScrollSpeed);
			else
				OffsetContent(-m_ScrollSpeed);
		}
	}	

	CUIWidget::UpdateSelf(deltatime);

}

int CUITextList::FindLineOnPoint(int x,int y)
{

	stRect absClntRct=GetAbsoluteClientRect(); 

	if(!absClntRct.IsPointIn(x,y)) 
		return(-1);
	
	//�������߶Ⱦ������
	int yStart=absClntRct.oy+m_CurScrollPos;	

	//��ʾ������
	for(int i=0;i<m_AllLines.size();i++)
	{ 

		//�ж��Ƿ�ɼ�
		int sy=yStart+m_AllLines[i].oy;

		if( ((sy+m_AllLines[i].height)>absClntRct.oy)&&(sy<(absClntRct.oy+absClntRct.height)) )
		{
			stRect lineRect(absClntRct.ox, sy, absClntRct.width, m_AllLines[i].height);

			if(lineRect.IsPointIn(x,y))
				return i;			
		}

	} 


	return(-1);
	
}

void CUITextList::RenderSelf()
{
	CUIWidget::RenderSelf();//���Ʊ���ͼƬ�Ͱ�Χ��

	
	int wgtState=UI_ITEM_STATE_FOCUSED;
		
	if(!IsEnabled())
		wgtState=UI_ITEM_STATE_DEFAULT;


	stRect absClntRct=GetAbsoluteClientRect(); 
	
	//�������߶Ⱦ������
	int yStart=absClntRct.oy+m_CurScrollPos;
	
	//��ʾ������
	for(int i=0;i<m_AllLines.size();i++)
	{ 
		
		//�ж��Ƿ�ɼ�
		int sy=yStart+m_AllLines[i].oy;

		
		stColorQuad drawColor(m_AllLines[i].color);
		stColorQuad magicColor=m_MagicColor;
		stColorQuad linkColor=m_LinkColor;
		
		if(m_ExtendStyle&TEXTLIST_STYLE_ALPHAFONT)
		{
			drawColor=GetRenderColor(drawColor);
			magicColor=GetRenderColor(magicColor);
			linkColor=GetRenderColor(linkColor);
		}


		if( ((sy+m_AllLines[i].height)>absClntRct.oy)&&(sy<(absClntRct.oy+absClntRct.height)) )
		{
			//add+jension 2006-02-20 11:20
			//{{{���촰�����ױ�����ʾ����
			/*
			stRect lineRct(absClntRct.ox, sy,absClntRct.width, m_AllLines[i].height);
			stRect titleRct(m_TitleRect.ox+absClntRct.ox, m_TitleRect.oy+sy,m_TitleRect.width, m_TitleRect.height);
			*/
			int lineheight = m_AllLines[i].richTxt.GetRichLineHeight(0);
			int yoffset = (lineheight - m_TitleRect.height)/2;
			stRect lineRct(absClntRct.ox, sy,absClntRct.width, m_AllLines[i].height);
			stRect titleRct(m_TitleRect.ox+absClntRct.ox, m_TitleRect.oy+sy+yoffset, m_TitleRect.width, m_TitleRect.height);
			//}}}

			m_AllLines[i].richTxt.DrawRichText(lineRct,
				m_TextSize,m_RowSpace,
				drawColor.c,magicColor.c,linkColor.c,
				m_TextStyle,m_TextFont,wgtState);

			if(strlen(m_AllLines[i].linedata.strData)>0)
			{
				CUIRichText richTxt;
				richTxt.ParseTxt(m_AllLines[i].linedata.strData,m_RichMode,m_RichFlag);

				richTxt.DrawRichText(titleRct,
					m_TextSize,m_RowSpace,
					drawColor.c,magicColor.c,linkColor.c,
					m_TextStyle,m_TextFont,wgtState);
			}
		}

	}

	
}

void CUITextList::ClearLines()
{

	m_AllLines.clear();

	SetUpdateDirty(true);
}

//����ײ�ѹ��һ��
void CUITextList::PushLine(const char* pText,unsigned int color/*=0*/,stUILineData* pdata/*=NULL*/)
{
	//ʹ��Ĭ�ϻ�����ɫ
	if(color<0) 	color=m_DrawColor.c;

	if(m_AllLines.size()>=m_MaxLine)
		m_AllLines.pop_front();

	stUILineData data;	
	
	if(pdata)	data=*pdata;	

	m_AllLines.push_back(stTextLine(pText,color,data,m_RichMode,m_RichFlag));	

	SetUpdateDirty(true);

}

int CUITextList::ParseKeyChar(int x,int y,stKeyChar& keyChar)
{

	stRect absClntRct=GetAbsoluteClientRect(); 
	
	//�������߶Ⱦ������
	int yStart=absClntRct.oy+m_CurScrollPos;
	
	//��ʾ������
	for(int i=0;i<m_AllLines.size();i++)
	{ 
		
		//�ж��Ƿ�ɼ�
		int sy=yStart+m_AllLines[i].oy;
		
		if( ((sy+m_AllLines[i].height)>absClntRct.oy)&&(sy<(absClntRct.oy+absClntRct.height)) )
		{

			stRect lineRct(absClntRct.ox, sy,absClntRct.width, m_AllLines[i].height);

			if(lineRct.IsPointIn(x,y))
			{
				return m_AllLines[i].richTxt.ParseKeyChar(keyChar,x,y,lineRct.ox,lineRct.oy,lineRct.width,lineRct.height,
						m_TextSize,m_RowSpace,m_TextStyle);
			}
		}

	}

	return RICHCHAR_CMD_NONE;
}


void CUITextList::OffsetContent(int offset)
{
	int pos=m_CurScrollPos+offset;
	
	ScrollContent(pos);
}

void CUITextList::ScrollToBegin()
{
	ScrollContent(0);
}

void CUITextList::ScrollToEnd()
{

	if(m_ExtendStyle&TEXTLIST_STYLE_SMARTSCROLL)
	{
		int lastH=0;

		if(!m_AllLines.empty())
		{
			lastH=m_AllLines[m_AllLines.size()-1].height;
		}

		if(lastH>m_ClientRect.height)
			ScrollContent(lastH-m_ContentHeight);
		else
			ScrollContent(m_ClientRect.height-m_ContentHeight);

	}
	else
	{
		ScrollContent(m_ClientRect.height-m_ContentHeight);
	}

}

void CUITextList::ScrollContent(int pos)
{
	if(pos<(m_ClientRect.height-m_ContentHeight)) 
		pos=(m_ClientRect.height-m_ContentHeight);
	
	if(pos>0) 	pos=0;
	
	
	m_CurScrollPos=pos;//��ס��ǰƫ��
	
	m_CurPage=ceil(abs(m_CurScrollPos)/(m_ClientRect.height*1.0));
	
	CUIScrollBar* pScrollBar=(CUIScrollBar*)GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pScrollBar) pScrollBar->SetScrollPos(m_CurScrollPos);
	
}

void CUITextList::GotoPage(int nPage)
{

	if(nPage>=m_NumPage) nPage=m_NumPage-1;
	
	if(nPage<0) nPage=0;
	
	int pos=m_ClientRect.height*nPage;
	ScrollContent(pos);

}

//�����������߶�
void CUITextList::ArrangeContent()
{

	m_ContentHeight=0;	
	if(m_ExtendStyle&TEXTLIST_STYLE_REVERSE)
	{

		for(int i=m_AllLines.size()-1;i>=0;i--)
		{ 
			int height=UI_MAX_INTEGER;
			int width=m_ClientRect.width;

			m_AllLines[i].richTxt.EstimateLine(width,height,m_TextSize,m_RowSpace);

			m_AllLines[i].height=height;
			m_AllLines[i].oy=m_ContentHeight+m_RowSpace;

			//��ǰ��Ŀ�ĵײ�
			m_ContentHeight=m_AllLines[i].oy+m_AllLines[i].height;
		}

	}
	else 
	{
	 
		for(int i=0;i<m_AllLines.size();i++)
		{ 
			int height=UI_MAX_INTEGER;
			int width=m_ClientRect.width;

			m_AllLines[i].richTxt.EstimateLine(width,height,m_TextSize,m_RowSpace);

			m_AllLines[i].height=height;
			m_AllLines[i].oy=m_ContentHeight+m_RowSpace;

			//��ǰ��Ŀ�ĵײ�
			m_ContentHeight=m_AllLines[i].oy+m_AllLines[i].height;
		}
	}

	m_ContentHeight+=m_RowSpace;

}

void CUITextList::UpdateScrollBar()
{

	//������������
	ArrangeContent();

	CUIWidget* pBuddy;
	
	//���ݼ���ó����������߶���������ֱ������
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pBuddy;
		
				
		int nLower=m_ClientRect.height-m_ContentHeight;
		if(nLower>0) nLower=0;
		
		pScrollBar->SetRange(nLower,0);
		pScrollBar->SetScrollPos(0);		
		
	}	
	
	ScrollContent(m_CurScrollPos);//�ظ����ϴι���λ��


}

//------------------------------------------------------------------------------------------
//----------------------------------class CTreeNode-----------------------------------------
//------------------------------------------------------------------------------------------


CTreeNode::CTreeNode()
{

	m_pMother=NULL;

	//Ψһ��ʶ��	
	m_Id=-1;


	//����λ��
	m_X=0;
	m_Y=0;

	m_Width=0;
	m_Height=0;

	//�ڵ㲼��ƫ�Ʋ���
	m_OffsetX=0;
	m_OffsetY=0;

	//�ڵ�ߴ����
	m_SizeX=0;
	m_SizeY=0;

	//�ӽڵ��Ƿ��۵�
	m_bCollapsed=TRUE;	

}
	
CTreeNode::~CTreeNode()
{

}


IWgtData* CTreeNode::GetHostWgt()
{
	CTreeNode* pNode=this;

	while(!pNode->IsRootNode()) 
	{ 
		if(!pNode->m_pMother) break;

		pNode=pNode->m_pMother;
	}

	return pNode->GetHostWgt();

}

bool CTreeNode::IsRootNode()
{
	return(!m_pMother);
}

bool CTreeNode::IsPointInIconRect(int x,int y)
{
	stRect absRect(m_X+m_IconRect.ox,m_Y+m_IconRect.oy,m_IconRect.width,m_IconRect.height);
	return absRect.IsPointIn(x,y);
}

//���Ƿ��ڽڵ��Լ��Ĺ̶�rect��
bool CTreeNode::IsPointInSelf(int x,int y)
{
	stRect absRect(m_X,m_Y,m_SizeX,m_SizeY);
	return absRect.IsPointIn(x,y);
}

//���Ƿ��ڽڵ������rect�л��¼��ڵ������rect��
bool CTreeNode::IsPointIn(int x,int y)
{
	stRect absRect(m_X,m_Y,m_Width,m_Height);
	return absRect.IsPointIn(x,y);
}

void CTreeNode::AddNode(int id,int offx,int offy,int sizex,int sizey,stRect iconRect)
{

	CTreeNode* pNode=new CTreeNode;

	pNode->m_Id=id;
	pNode->m_OffsetX=offx;
	pNode->m_OffsetY=offy;
	pNode->m_SizeX=sizex;
	pNode->m_SizeY=sizey;
	pNode->m_IconRect=iconRect;

	pNode->m_pMother=this;

	m_vecNodes.push_back(pNode);

}

void CTreeNode::ClearNodes()
{
	std::vector<CTreeNode*>::iterator iter;
	for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
	{	
		(*iter)->m_pMother=NULL;
		(*iter)->Release();		
	}

	m_vecNodes.clear();
}


void CTreeNode::UpdateNode()
{

	//��ʼ���ڵ���Ƴߴ�Ϊָ���ߴ�
	m_Width=m_SizeX;
	m_Height=m_SizeY;
	
	//���������ӽڵ���Ƴߴ�	
	if(!m_bCollapsed)
	{
		
		std::vector<CTreeNode*>::iterator iter;
		for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
		{
			CTreeNode* pNode=(*iter);

			//���㺢�ӵ�λ��
			pNode->m_X=m_X+pNode->m_OffsetX;
			pNode->m_Y=m_Y+m_Height+pNode->m_OffsetY;

			//���㺢�ӵĳߴ�
			pNode->UpdateNode();

			//�����Լ��ĳߴ�
			if(m_Width<(pNode->m_OffsetX+pNode->m_Width))
				m_Width=(pNode->m_OffsetX+pNode->m_Width);

			m_Height+=(pNode->m_OffsetY+pNode->m_Height);


		}

	}	


}


bool CTreeNode::ReQueryNode()
{

	IWgtData* pWgt=GetHostWgt();

	if(!pWgt) return FALSE;

	stQueryNode queryNode;
	queryNode.id=m_Id;
	RawExecCallBack(UI_Callback_QueryNode,(void*)&queryNode,pWgt);

	ClearNodes();
	for(int i=0;i<queryNode.numChild;i++)
	{
		AddNode(queryNode.idChild[i],queryNode.offx,queryNode.offy,queryNode.sizex,queryNode.sizey,queryNode.iconRect);
	}

	return (m_vecNodes.size()>0);	

}

bool CTreeNode::ExtendNode()
{
	//�����۵�״̬
	m_bCollapsed=false;

	return ReQueryNode();	
}

bool CTreeNode::CollapseNode()
{

	//root�ڵ���Զ�����۵�
	if(!IsRootNode())
	{
		//�����۵�״̬
		m_bCollapsed=true;
	}

	return (m_vecNodes.size()>0);
	
}

bool CTreeNode::ToggleNode()
{

	if(m_bCollapsed)
		return ExtendNode();
	else 
		return CollapseNode();	

}

CTreeNode* CTreeNode::GetNodeIconOnPoint(int x,int y)
{

	//û��������Ҫ���ӽڵ�������
	if(!m_bCollapsed)
	{
		std::vector<CTreeNode*>::iterator iter;
		for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
		{
			CTreeNode* pNode=(*iter)->GetNodeIconOnPoint(x,y);
			if(pNode)	return pNode;
		}

	}

	if(IsPointInIconRect(x,y)) return this;

	return NULL;


}

CTreeNode* CTreeNode::GetNodeOnPoint(int x,int y)
{

	//û��������Ҫ���ӽڵ�������
	if(!m_bCollapsed)
	{
		std::vector<CTreeNode*>::iterator iter;
		for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
		{
			CTreeNode* pNode=(*iter)->GetNodeOnPoint(x,y);
			if(pNode)	return pNode;
		}

	}

	if(IsPointIn(x,y)) return this;

	return NULL;

}


//path�ĸ�ʽ:id,motherid,grandmaid,...
stNodePath CTreeNode::GetNodePath()
{
		
	stNodePath nodePath;

	CTreeNode* pNode=this;
	do 
	{
		nodePath.idPath[nodePath.numPath]=pNode->m_Id;
		nodePath.numPath++;

		pNode=pNode->m_pMother;
		
	} while(pNode);
	
	nodePath.id=m_Id;

	return nodePath;

}

bool CTreeNode::ExtendNodeByPath(stNodePath path)
{
		
	if((path.numPath<=0)||(path.numPath>=UI_NODE_PATH_MAX)) return false;
		
	//���·��ָ��Ϊ�Լ�
	if(path.idPath[path.numPath-1]==m_Id)
	{
		path.numPath--;
		
		//���и����·������Ҫ���Լ��ĺ����в���
		if(path.numPath>0)
		{
			//����Լ��ĺ����۵���
			if(m_bCollapsed) ExtendNode();
			
			std::vector<CTreeNode*>::iterator iter;
			for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
			{
				if((*iter)->ExtendNodeByPath(path))
					return true;
			}
		}		
		
	}


	return false;


}

//path�ĸ�ʽ:id,motherid,grandmaid,...
CTreeNode* CTreeNode::GetNodeByPath(stNodePath path)
{
	if((path.numPath<=0)||(path.numPath>=UI_NODE_PATH_MAX)) return NULL;
		
	//���·��ָ��Ϊ�Լ�
	if(path.idPath[path.numPath-1]==m_Id)
	{
		path.numPath--;
		
		//���и����·������Ҫ���Լ��ĺ����в���
		if(path.numPath>0)
		{
			
			//����Լ��ĺ����۵���
			if(m_bCollapsed) ReQueryNode();
			
			std::vector<CTreeNode*>::iterator iter;
			for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
			{
				CTreeNode* pNode=(*iter)->GetNodeByPath(path);
				if(pNode)	return pNode;
			}
		}
		
		return this;
		
	}
	
	return NULL;
}


//bInCollapse:ǿ������collapsed�ӽڵ�
CTreeNode* CTreeNode::GetNodeById(int id,bool bInCollapse)
{
	if(m_Id==id) return this;
	
	//û��������Ҫ���ӽڵ�������
	if(bInCollapse||!m_bCollapsed)
	{
		std::vector<CTreeNode*>::iterator iter;
		for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
		{
			CTreeNode* pNode=(*iter)->GetNodeById(id,bInCollapse);
			if(pNode)	return pNode;
		}
		
	}
	
	return NULL;
	
}
  
//���ⲿֻ�ܴӸ��ڵ��е���
CTreeNode* CTreeNode::GetPrevNode(int id,bool bInCollapse)
{

	static CTreeNode* preNode=NULL;

	//���ⲿ�Ӹ��ڵ��е���ʱ����ʼ������֤������ؼ��˴˲������
	if(IsRootNode()) preNode=NULL;

	if(m_Id==id)
		return preNode;
	else 
		preNode=this;


	//û��������Ҫ���ӽڵ�������
	if(bInCollapse||!m_bCollapsed)
	{
		std::vector<CTreeNode*>::iterator iter;
		for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
		{
			CTreeNode* pNode=(*iter)->GetPrevNode(id,bInCollapse);
			if(pNode)	return pNode;
		}

	}

	return NULL;
	
}

//���ⲿֻ�ܴӸ��ڵ��е���
CTreeNode* CTreeNode::GetNextNode(int id,bool bInCollapse)
{
	static CTreeNode* preNode=NULL;

	//���ⲿ�Ӹ��ڵ��е���ʱ����ʼ������֤������ؼ��˴˲������
	if(IsRootNode()) preNode=NULL;

	if(preNode&&(preNode->m_Id==id))
		return this;
	else 
		preNode=this;
	
	//û��������Ҫ���ӽڵ�������
	if(bInCollapse||!m_bCollapsed)
	{
		std::vector<CTreeNode*>::iterator iter;
		for( iter=m_vecNodes.begin(); iter!=m_vecNodes.end(); iter++ )
		{
			CTreeNode* pNode=(*iter)->GetNextNode(id,bInCollapse);
			if(pNode)	return pNode;
		}

	}

	return NULL;	

}

//------------------------------------------------------------------------------------------
//----------------------------------class CUITreeCtrl-----------------------------------------
//------------------------------------------------------------------------------------------

CUITreeCtrl::CUITreeCtrl()
{
	m_Type = WGTTYPE_TREECTRL;

	m_RootNode.SetHostWgt(this);
}
	
CUITreeCtrl::~CUITreeCtrl()
{
	
}

void CUITreeCtrl::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_bSelDirty=false;

	m_FocusId=-1;

	m_RootNode.m_Id=-1;

	SetUpdateDirty(true);
}

void CUITreeCtrl::OnInit()
{

	CUIWidget::OnInit();

	m_RootNode.ExtendNode();
	SetUpdateDirty(true);

}

//ִ�нڵ���չ����Ч
void CUITreeCtrl::OnNodeExtend()
{
	stUISound sndData;
	sndData.id=m_SoundId[UI_SOUND_NODE_EXTEND];
	sndData.cmd=UI_SOUND_NODE_EXTEND;
	sndData.mode=0;
	ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);
}

//ִ�нڵ��۵�����Ч
void CUITreeCtrl::OnNodeCollapse()
{
	stUISound sndData;
	sndData.id=m_SoundId[UI_SOUND_NODE_COLLAPSE];
	sndData.cmd=UI_SOUND_NODE_COLLAPSE;
	sndData.mode=0;
	ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);
}


//���scroll�Ǻ���,�Զ�ע��
void CUITreeCtrl::AddChild( CUIWidget *pwidget )
{
	
	CUIWidget::AddChild(pwidget);
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{

		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",this);
		
		if(pScrollBar->IsVertical())
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",pScrollBar);
		else 
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY,BUDDY_STYLE_SELF,"",pScrollBar);
		

	}
	
	
}

void CUITreeCtrl::DelChild( CUIWidget *pwidget )
{
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{

		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",0);
		
		if(pScrollBar->IsVertical())
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",0);
		else 
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY,BUDDY_STYLE_SELF,"",0);
		

	}
	
	CUIWidget::DelChild(pwidget);
	
}

void CUITreeCtrl::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	
	CUIWidget::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
	if((pelement=pvar->Element("treeid")))
		m_RootNode.m_Id=pelement->Integer();   

}

void CUITreeCtrl::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{

	case VALUE_CHANNEL_TREEID:
		uiVar.fValue = m_RootNode.m_Id;
		break;
		
	case VALUE_CHANNEL_SEL:
		uiVar.fValue = m_SelectPath.id;
		break;

	case VALUE_CHANNEL_HOT:
		uiVar.fValue = m_FocusId;
		break;

	case VALUE_CHANNEL_PAGE_H:
		uiVar.fValue = m_CurPage.x; 	
		break;

	case VALUE_CHANNEL_PAGE_V:
		uiVar.fValue = m_CurPage.y; 	
		break;

	case VALUE_CHANNEL_PAGENUM_H:
		uiVar.fValue = m_NumPage.x; 
		break;

	case VALUE_CHANNEL_PAGENUM_V:
		uiVar.fValue = m_NumPage.y;	
		break;
	
	default:
		CUIWidget::GetVar(uiVar);

	}
	
	
}


void CUITreeCtrl::SetVar(stUIVar& uiVar)
{
	
	switch(uiVar.iChannel)
	{
	
	case VALUE_CHANNEL_TREEID:
		{

			m_RootNode.m_Id=int(uiVar.fValue);

			m_RootNode.ExtendNode();

			SetUpdateDirty(true);

		}
		break;

	case VALUE_CHANNEL_SEL:
		{

			//����ָ��ѡ��ڵ�id������·��
			stNodePath queryPath;
			queryPath.id=int(uiVar.fValue);
			ExecCallBackFunc(UI_Callback_QueryNodePath,(void*)&queryPath);			

			//·��Ϊ�գ��Ҳ���ָ���ڵ㣬ѡ��ԭ����·��
			if(queryPath.numPath<=0) queryPath=m_SelectPath;

			CTreeNode* pNode=m_RootNode.GetNodeByPath(queryPath);
			if(pNode)
			{				

				m_bSelDirty=true;
				m_SelectPath=pNode->GetNodePath();
				

				if(EnsureExtended(m_SelectPath))
				{
					ArrangeContent();	
		
					UpdateScrollBar();
				}

				Point2D offset;
				if(EnsureVisible(pNode,offset.x,offset.y))
				{
					
					OffsetContentH(offset.x);
					OffsetContentV(offset.y);
				}

			}

		}		
		break;

	case VALUE_CHANNEL_PAGE_H:
		GotoPageH((int)uiVar.fValue);
	
		break;

	case VALUE_CHANNEL_PAGE_V:
		GotoPageV((int)uiVar.fValue);	
		break;

	default: 
		CUIWidget::SetVar(uiVar);

	}
	
}



void CUITreeCtrl::OffsetContentH(int offset)
{
	
	int pos=m_CurScrollPos.x+offset;
	
	ScrollContentH(pos);
	
}


void CUITreeCtrl::OffsetContentV(int offset)
{
	
	int pos=m_CurScrollPos.y+offset;
	
	ScrollContentV(pos);
	
}

//posΪ�����ԭʼλ�õ�ƫ��,ֻ���Ǹ�ֵ
void CUITreeCtrl::ScrollContentH(int pos)
{


	if(pos<(m_WinRect.width-m_ContentRect.width)) 
		pos=(m_WinRect.width-m_ContentRect.width);

	if(pos>0) 	pos=0;
		
	
	m_CurScrollPos.x=pos;//��ס��ǰƫ��

	m_CurPage.x=ceil(abs(m_CurScrollPos.x)/(m_WinRect.width*1.0));

	CUIScrollBar* pScrollBar=(CUIScrollBar*)GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY);
	if(pScrollBar) pScrollBar->SetScrollPos(-m_CurScrollPos.x);

}



//posΪ�����ԭʼλ�õ�ƫ��,ֻ���Ǹ�ֵ
void CUITreeCtrl::ScrollContentV(int pos)
{
	
	
	if(pos<(m_WinRect.height-m_ContentRect.height)) 
		pos=(m_WinRect.height-m_ContentRect.height);
	
	if(pos>0) 	pos=0;
	
		
	m_CurScrollPos.y=pos;//��ס��ǰƫ��
	
	m_CurPage.y=ceil(abs(m_CurScrollPos.y)/(m_WinRect.height*1.0));
	
	CUIScrollBar* pScrollBar=(CUIScrollBar*)GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pScrollBar) pScrollBar->SetScrollPos(m_CurScrollPos.y);
	
}



void CUITreeCtrl::GotoPageH(int nPage)
{
	
	if(nPage>=m_NumPage.x) nPage=m_NumPage.x-1;

	if(nPage<0) nPage=0;
	
	int pos=-m_WinRect.width*nPage;
	ScrollContentH(pos);
	

}

void CUITreeCtrl::GotoPageV(int nPage)
{
	
	if(nPage>=m_NumPage.y) nPage=m_NumPage.y-1;
	
	if(nPage<0) nPage=0;
	
	int pos=-m_WinRect.height*nPage;
	ScrollContentV(pos);

	
}

//���и�����Ŀ(�򵥵ļ�����ռ�ݵ���������)
//���Ա���������
void CUITreeCtrl::ArrangeContent()
{


	m_ContentRect.ox=m_CurScrollPos.x;
	m_ContentRect.oy=m_CurScrollPos.y;

	m_RootNode.m_X=m_WinRect.ox+m_ContentRect.ox;
	m_RootNode.m_Y=m_WinRect.oy+m_ContentRect.oy;

	m_RootNode.UpdateNode();

	m_ContentRect.width=m_RootNode.m_Width+m_MarginH;
	m_ContentRect.height=m_RootNode.m_Height+m_MarginV;

	ComfortSelf();


}

void CUITreeCtrl::ComfortSelf()
{

	if(m_ExtendStyle&TREECTRL_STYLE_AUTO_COMFORT)
	{

		//�������
		if(IsSizableX())
		{
			int appendx=m_ContentRect.width-m_ClientRect.width;
			m_ClientRect.width+=appendx;
			m_WinRect.width+=appendx;
		}


		//�����߶�
		if(IsSizableY())
		{
			int appendy=m_ContentRect.height-m_ClientRect.height;
			m_ClientRect.height+=appendy;
			m_WinRect.height+=appendy;
		}
		
		SetUpdateDirty(true);

	}


}

void CUITreeCtrl::UpdateScrollBar()
{

	CUIWidget* pBuddy;

	//���ݼ���ó����������߶���������ֱ������
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pBuddy;

		
		int nLower=m_WinRect.height-m_ContentRect.height;
		if(nLower>0) nLower=0;
		
		pScrollBar->SetRange(nLower,0);		
		ScrollContentV(m_CurScrollPos.y);
				
		
	}	
	

	//���ݼ���ó������������������ˮƽ������
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pBuddy;

		
		int nUpper=m_ContentRect.width-m_WinRect.width;
		if(nUpper<0) nUpper=0;
		
		pScrollBar->SetRange(0,nUpper);		
		ScrollContentH(m_CurScrollPos.x);
		
	}	
	
}


//�ϷŴ���
bool CUITreeCtrl::ItemDrop(int x,int y)
{
	int index=ParseDragIndex(x,y,UI_PARSE_APPROX);
	
	stMoveItem data;

	stDragInfo info=g_pRootWgt->GetDragInfo();

	data.srcindex=info.index;
	data.srctype=info.type;
	data.srchostindex=info.hostindex;
	data.srchosttype=info.hosttype;

	data.itemnum=info.itemnum;

	data.dstindex=index;
	data.dsttype=m_DragType;	
	data.dsthostindex=m_HostIndex;
	data.dsthosttype=m_HostType;

	data.rect=info.rect;
	data.rect.ox=CUICursor::m_Pos.x-data.rect.width/2;
	data.rect.oy=CUICursor::m_Pos.y-data.rect.height/2;

	//��û���϶��뿪ԭ��λ�õ�ʱ��,���϶�
	if(!g_UICoreStyle.bOriginMove)
	{
		if(data.srctype==data.dsttype&&data.srcindex==data.dstindex) return false;
	}

	ExecCallBackFunc(UI_Callback_MoveItem,(void*)&data);

	
	return true;//���ܷ��µ���Ŀ

}


int  CUITreeCtrl::ParseDragIndex(int x,int y,int style)
{

	int index=-1;		
	
	if(style==UI_PARSE_EXACT)
	{
		CTreeNode* pNode=m_RootNode.GetNodeOnPoint(x,y);
		if(pNode)
		{
			if(pNode->IsPointInSelf(x,y))
			{
				index=pNode->m_Id;
			}				

		}
	}
	else if(style==UI_PARSE_NEAR)
	{		
		CTreeNode* pNode=m_RootNode.GetNodeOnPoint(x,y);
		if(pNode)
		{
			if(pNode->IsPointIn(x,y))
			{
				index=pNode->m_Id;
			}				

		}
		else 
		{
			index=m_RootNode.m_Id;		 
		}

	}
	else if(style==UI_PARSE_APPROX)
	{		
		CTreeNode* pNode=m_RootNode.GetNodeOnPoint(x,y);
		if(pNode)
		{
			if(pNode->IsPointIn(x,y))
			{
				index=pNode->m_Id;
			}				

		}	
	}	

	return index;
}

stDragInfo CUITreeCtrl::ParseDragInfo(int x,int y)
{
	stDragInfo infoDrag;

	if(!IsInItemDragRect(x,y)) return infoDrag;//���ڵ����϶�����,����Ҫ�����Ϸ�

	if(m_DragType==UI_DRAG_TYPE_NONE) 	return infoDrag;
	
	int index=ParseDragIndex(x,y,UI_PARSE_EXACT);
	CTreeNode* pNode=m_RootNode.GetNodeById(index,false);
	if(!pNode) return infoDrag;

	stRect rcNode=stRect(pNode->m_X,pNode->m_Y,pNode->m_Width,pNode->m_Height);
	
	stValidItem validItem;
	validItem.type=m_DragType;
	validItem.index=index;
	validItem.hosttype=m_HostType;
	validItem.hostindex=m_HostIndex;

	ExecCallBackFunc(UI_Callback_IsValidItem,(void*)&validItem);		
	if(!validItem.candrag)	return infoDrag;


	infoDrag.type=m_DragType;
	infoDrag.index=index;
	infoDrag.rect=rcNode;
	infoDrag.itemtype=validItem.itemtype;
	infoDrag.maxnum=validItem.nitem;
	infoDrag.bAttach=validItem.canattach;
	infoDrag.bAjust=validItem.canajust;
	infoDrag.pDragSrc=this;


	
	return infoDrag;
}

int  CUITreeCtrl::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_SCROLLBAR_POS:
		{

			CUIScrollBar* pBar=(CUIScrollBar*)lparam;
			if(pBar)
			{
				
				if(pBar->IsVertical())
				{					
					ScrollContentV((int)wparam);	
				}
				else 
				{
					ScrollContentH(-(int)wparam);	
				}
				
			}
			
		
		}
		break;
		
		
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}


int CUITreeCtrl::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{

	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{
	
	case VK_LEFT:
		{
			CTreeNode* pNode=m_RootNode.GetNodeById(m_SelectPath.id,false);
			if(pNode)
			{
				bool bAction=FALSE;

				//nodeû���۵������۵���node����ѡ���node
				if(!(pNode->m_bCollapsed))
				{
					m_bSelDirty=true;
					m_SelectPath=pNode->GetNodePath();
					bAction=pNode->CollapseNode();
					OnNodeCollapse();
				}

				if(bAction)
				{
					SetUpdateDirty(true);
				}
				else
				{

					//node�Ѿ��۵����������ǰ���һ��node
					pNode=m_RootNode.GetPrevNode(m_SelectPath.id,false);
					

					//�ҵ�ǰ��һ��node����ѡ���node����ȷ���ɼ�
					if(pNode&&(!pNode->IsRootNode()))
					{

						m_bSelDirty=true;
						m_SelectPath=pNode->GetNodePath();

						Point2D offset;
						if(EnsureVisible(pNode,offset.x,offset.y))
						{
							
							OffsetContentH(offset.x);
							OffsetContentV(offset.y);
						}

					}

				}


			}

		}
		break;
        
	case VK_RIGHT:
		{

			CTreeNode* pNode=m_RootNode.GetNodeById(m_SelectPath.id,false);
			if(pNode)
			{
				bool bAction=FALSE;

				if(pNode->m_bCollapsed)
				{
					m_bSelDirty=true;
					m_SelectPath=pNode->GetNodePath();
					bAction=pNode->ExtendNode();
					OnNodeExtend();


				}

				if(bAction)
				{
					ArrangeContent();
					
					Point2D offset;
					if(EnsureVisible(pNode,offset.x,offset.y,FALSE))
					{
						OffsetContentH(offset.x);
						OffsetContentV(offset.y);
					}


					SetUpdateDirty(true);
				}
				else 
				{

					pNode=m_RootNode.GetNextNode(m_SelectPath.id,false);
					
					if(pNode)
					{

						m_bSelDirty=true;
						m_SelectPath=pNode->GetNodePath();

						Point2D offset;
						if(EnsureVisible(pNode,offset.x,offset.y))
						{
							OffsetContentH(offset.x);
							OffsetContentV(offset.y);
						}

					}

				}


			}

		}		
		break;
        
	case VK_UP:
		{
			CTreeNode* pNode=m_RootNode.GetPrevNode(m_SelectPath.id,false);
			if(pNode&&(!pNode->IsRootNode()))
			{
				m_bSelDirty=true;
				m_SelectPath=pNode->GetNodePath();

				Point2D offset;
				if(EnsureVisible(pNode,offset.x,offset.y))
				{
					OffsetContentH(offset.x);
					OffsetContentV(offset.y);
				}
				
			}
			
		}
		break;

	case VK_DOWN:
		{
			
			CTreeNode* pNode=m_RootNode.GetNextNode(m_SelectPath.id,false);
			if(pNode)
			{
				m_bSelDirty=true;
				m_SelectPath=pNode->GetNodePath();

				Point2D offset;
				if(EnsureVisible(pNode,offset.x,offset.y))
				{
					OffsetContentH(offset.x);
					OffsetContentV(offset.y);
				}

			}

		}
		break;

	default:

		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);

	}

	return 1;
}

void CUITreeCtrl::OnClickNode(CTreeNode* pNode)
{

	if(!pNode) return;

	m_bSelDirty=true;
	m_SelectPath=pNode->GetNodePath();

	Point2D offset;
	if(EnsureVisible(pNode,offset.x,offset.y))
	{
		OffsetContentH(offset.x);
		OffsetContentV(offset.y);
	}


}

void CUITreeCtrl::OnDBClickNode(CTreeNode* pNode)
{

	m_bSelDirty=true;
	m_SelectPath=pNode->GetNodePath();

	if(pNode->m_bCollapsed) OnNodeExtend();
	else OnNodeCollapse();

	pNode->ToggleNode();

	Point2D offset;
	if(EnsureVisible(pNode,offset.x,offset.y))
	{
		OffsetContentH(offset.x);
		OffsetContentV(offset.y);
	}

	SetUpdateDirty(true);


}


int  CUITreeCtrl::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{

	switch( msgtype )
	{

	case MSGTYPE_KEYDOWN:
		return OnKeyDown(wparam,lparam);		
		break;

	case MSGTYPE_MOUSEWHEEL:
		{			
			short offset=HIWORD(wparam);

			if(offset>0) OffsetContentV(m_MouseDelta);	
			else OffsetContentV(-m_MouseDelta);	

		}
		break;

	//����ƶ����л�focus��select
	case MSGTYPE_MOUSEMOVE:
		{
			
			//���λ��
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			CTreeNode* pNode=m_RootNode.GetNodeOnPoint(x,y);
			if(pNode)
			{
				if(pNode->IsPointInSelf(x,y))
				{
					m_FocusId=pNode->m_Id;
				}				

			}

			//�ƶ���ʱ����ʱparsetooltip
			if(m_IdleTime<0.0f)
			{
				int nIndex=ParseDragIndex(x,y,UI_PARSE_EXACT);
				ParseTooltip(nIndex,GetWinRect(),x,y);
			}


		}		
		break;

	//����ѡ��
	case MSGTYPE_LEFTBTNDOWN:	
		{
			
			//���λ��
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			
			int id=m_RootNode.m_Id;

			CTreeNode* pNode=NULL;

			//����Ƿ����ĳ�����ڵ�iconrect��			
			if(pNode=m_RootNode.GetNodeIconOnPoint(x,y))
			{
				if(pNode->IsPointInIconRect(x,y))
				{
					OnDBClickNode(pNode);

					id=pNode->m_Id;
		
				}

			}
			else if(pNode=m_RootNode.GetNodeOnPoint(x,y))
			{
				if(pNode->IsPointInSelf(x,y))
				{
					OnClickNode(pNode);

					id=pNode->m_Id;
				}
				
			}

			if(m_DragType!=UI_DRAG_TYPE_NONE)
			{
				stClickItem useItem;
				useItem.type=m_DragType;
				useItem.index=id;
				useItem.hosttype=m_HostType;
				useItem.hostindex=m_HostIndex;
				
				ExecCallBackFunc(UI_Callback_ClickItem,(void*)&useItem);			
			}

			
			CallSafeScriptEx(SCRIPT_FUNC_ON_CLICK,"%d,%d,%d",id,wparam,lparam);
			
		}
		break;


	//˫���۵�
	case MSGTYPE_LBTNDBLCLK:			
		{
			
			//���λ��
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);

			int id=m_RootNode.m_Id;

			CTreeNode* pNode=NULL;
			//����Ƿ����ĳ�����ڵ�iconrect��			
			if(pNode=m_RootNode.GetNodeIconOnPoint(x,y))
			{

				if(pNode->IsPointInIconRect(x,y))
				{
					OnDBClickNode(pNode);
					id=pNode->m_Id;	
				}


			}
			else if(pNode=m_RootNode.GetNodeOnPoint(x,y))
			{
				
				if(pNode->IsPointInSelf(x,y))
				{	
					OnDBClickNode(pNode);
					id=pNode->m_Id;	
				}
				
				
			}

			if(m_DragType!=UI_DRAG_TYPE_NONE)
			{			
				stClickItem useItem;
				useItem.type=m_DragType;
				useItem.index=id;
				useItem.hosttype=m_HostType;
				useItem.hostindex=m_HostIndex;
				
				ExecCallBackFunc(UI_Callback_DClickItem,(void*)&useItem);
			}
			
			
			CallSafeScriptEx(SCRIPT_FUNC_ON_DBCLICK,"%d,%d,%d",id,wparam,lparam);

			
		}
		break;


	case MSGTYPE_RIGHTBTNDOWN:
		{
			
			//���λ��
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);

			int id=m_RootNode.m_Id;

			CTreeNode* pNode=m_RootNode.GetNodeOnPoint(x,y);
			if(pNode)
			{
				
				if(pNode->IsPointInSelf(x,y))
				{		
					id=pNode->m_Id;				
				}
				
				if(m_DragType!=UI_DRAG_TYPE_NONE)
				{			
					stClickItem useItem;
					useItem.type=m_DragType;
					useItem.index=id;
					useItem.hosttype=m_HostType;
					useItem.hostindex=m_HostIndex;
					
					ExecCallBackFunc(UI_Callback_RClickItem,(void*)&useItem);		
				}
		
				CallSafeScriptEx(SCRIPT_FUNC_ON_RCLICK,"%d",id);
			}
		
		}
		break;

	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );

	}

	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;

}



void CUITreeCtrl::UpdateSelf( float deltatime )
{
		
	ArrangeContent();	
		
	if(IsUpdateDirty()) 
	{	
		UpdateScrollBar();

		SetUpdateDirty(false);
	}

	if(m_bSelDirty)
	{
		CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d,%d",m_SelectPath.id,m_HistoryPath.id);

		m_HistoryPath=m_SelectPath;

		m_bSelDirty=false;
	}

	CUIWidget::UpdateSelf(deltatime);
	
}

void CUITreeCtrl::RenderSelf()
{

	//���Ʊ����Ͳ�͸���ĺп�
	CUIWidget::RenderSelf();

	RenderNodes(&m_RootNode);

}



void CUITreeCtrl::DrawGrid(CTreeNode* pNode)
{

	if(!pNode) return;
	
	if(pNode->m_bCollapsed) return;
	if(pNode->m_vecNodes.empty()) return;

	int cutOff=0;

	std::vector<CTreeNode*>::iterator iter;
	for( iter=pNode->m_vecNodes.begin(); iter!=pNode->m_vecNodes.end(); iter++ )
	{
		CTreeNode* pNode=(*iter);

		Point2D ptLeft=Point2D(pNode->m_X-pNode->m_OffsetX-pNode->m_OffsetX*0.5,pNode->m_Y+pNode->m_SizeY*0.5);
		Point2D ptRight=Point2D(pNode->m_X,pNode->m_Y+pNode->m_SizeY*0.5);
		g_pDisplay->DrawLine(ptLeft.x,ptLeft.y,ptRight.x,ptRight.y,stColorQuad(255,0,0).c);	

		cutOff=pNode->m_SizeY*0.5;		
	}

	
	Point2D ptUp=Point2D(pNode->m_X-pNode->m_OffsetX*0.5,pNode->m_Y+pNode->m_SizeY*0.5);
	Point2D ptDown=Point2D(pNode->m_X-pNode->m_OffsetX*0.5,pNode->m_Y+pNode->m_Height-cutOff);
	g_pDisplay->DrawLine(ptUp.x,ptUp.y,ptDown.x,ptDown.y,stColorQuad(255,0,0).c);


}

void CUITreeCtrl::DrawNode(CTreeNode* pNode)
{
	if(!pNode) return;

	stRect nodeRect=stRect(pNode->m_X,pNode->m_Y,pNode->m_SizeX,pNode->m_SizeY);

	stRect clipRect=GetAbsoluteClientRect();
	if(IsRectIntersect(nodeRect,clipRect)==RECT_NO_INTERSECT)	
		return;

	stDrawItem drawNode;
	drawNode.type=GetDragType();
	drawNode.index=pNode->m_Id;	
	drawNode.state=UI_ITEM_STATE_DEFAULT;
	drawNode.rect=nodeRect;

	if(m_SelectPath.id==pNode->m_Id)
		drawNode.state|=UI_ITEM_STATE_SELECTED;
	
	if(m_FocusId==pNode->m_Id)
		drawNode.state|=UI_ITEM_STATE_FOCUSED;

	if(pNode->m_bCollapsed) 
		drawNode.state|=UI_ITEM_STATE_COLLAPSED;
	
	ExecCallBackFunc(UI_Callback_DrawItem,(void*)&drawNode);


}

void CUITreeCtrl::RenderNodes(CTreeNode* pNode)
{
	if(!pNode) return;
		
	stRect wholeRect=stRect(pNode->m_X,pNode->m_Y,pNode->m_Width,pNode->m_Height);		

	stRect clipRect=GetAbsoluteClientRect();
	if(IsRectIntersect(wholeRect,clipRect)==RECT_NO_INTERSECT)	
		return;	
	
	if(m_ExtendStyle&TREECTRL_STYLE_DRAWGRID)
		DrawGrid(pNode);

	DrawNode(pNode);
	
	if(!pNode->m_bCollapsed)
	{
		std::vector<CTreeNode*>::iterator iter;
		for( iter=pNode->m_vecNodes.begin(); iter!=pNode->m_vecNodes.end(); iter++ )
		{
			RenderNodes(*iter);	
		}

	}

}

BOOL CUITreeCtrl::EnsureExtended(stNodePath path)
{

	m_RootNode.ExtendNodeByPath(path);

	return true;
}

BOOL CUITreeCtrl::EnsureVisible(CTreeNode* pNode,int& offx,int& offy,bool bSelf/*=TRUE*/)
{

	if(!pNode) 
		return FALSE;

	stRect nodeRect;
	if(bSelf)
	{
		nodeRect=stRect(pNode->m_X,pNode->m_Y,pNode->m_SizeX,pNode->m_SizeY);
	}
	else 
	{
		nodeRect=stRect(pNode->m_X,pNode->m_Y,pNode->m_Width,pNode->m_Height);
	}

	stRect clipRect=GetAbsoluteClientRect();
	int visRelation=IsRectIntersect(nodeRect,clipRect);
	

	if((visRelation==RECT_TOTAL_INSIDE)||(visRelation==RECT_TOTAL_OUTSIDE)) 
		return FALSE;

	//���ȱ�֤�����ɼ�
	if(nodeRect.oy<clipRect.oy)//item���������ͻ�������
	{
		offy=clipRect.oy-nodeRect.oy;
	}
	else if((nodeRect.oy+nodeRect.height)>(clipRect.oy+clipRect.height))//node�ײ������ͻ����ײ�
	{
		if(nodeRect.height>clipRect.height)
			offy=clipRect.oy-nodeRect.oy;
		else
			offy=(clipRect.oy+clipRect.height)-(nodeRect.oy+nodeRect.height);
	}
	
	
	//���ȱ�֤���ɼ�
	if(nodeRect.ox<clipRect.ox)//item��߳����ͻ������
	{
		offx=clipRect.ox-nodeRect.ox;
	}
	else if((nodeRect.ox+nodeRect.width)>(clipRect.ox+clipRect.width))//item�ұ߳����ͻ����ұ�
	{
		if(nodeRect.width>clipRect.width)
			offx=clipRect.ox-nodeRect.ox;
		else		
			offx=(clipRect.ox+clipRect.width)-(nodeRect.ox+nodeRect.width);
	}
	
	


	return TRUE;

}


//------------------------------------------------------------------------------------------
//----------------------------------class CUIWidgetList-----------------------------------------
//------------------------------------------------------------------------------------------
CUIWidgetList::CUIWidgetList()
{

	m_Type = WGTTYPE_WIDGETLIST;

}
	
CUIWidgetList::~CUIWidgetList()
{
	
}

void CUIWidgetList::ClearParameter()
{
	CUIContainer::ClearParameter();
	
	m_Style=WGTSTYLE_VERTICAL;//Ĭ�ϵ���

	m_ExtendStyle=WGTLIST_STYLE_WHEEL;

}


void CUIWidgetList::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{

	case VALUE_CHANNEL_ITEM_NUM:
		uiVar.fValue = GetVisChildNum();
		break;

	case VALUE_CHANNEL_SEL:
		uiVar.fValue = GetSelect(0);
		break;

	default:
		CUIContainer::GetVar(uiVar);

	}

}

void  CUIWidgetList::SetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
	
	case VALUE_CHANNEL_ITEM_NUM:
		SetVisChildNum((int)uiVar.fValue);
		break;

	case VALUE_CHANNEL_SEL:
		{
			int sel=uiVar.fValue;
			
			if(sel<0)
			{
				sel=0;
			}
			else 
			{
				int num=GetVisChildNum();
				
				if(sel>=num)
					sel=num-1;
			}

			SelectChild(0,sel);		
				
			EnsureItemVisible(sel);
		}	
		break;

	default:
		CUIContainer::SetVar(uiVar);

	}
	
}

//�õ���ǰ�ɼ����ӵ���Ŀ
int  CUIWidgetList::GetVisChildNum()
{
	int num=0;

	std::vector<CUIWidget*>::iterator iter;
	for(iter=m_Children.begin();iter!=m_Children.end();iter++)
	{
		if((*iter)->IsVisible())
			num++;
	}

	return num;

}

//�������õ�ǰ�ɼ����ӵ���Ŀ
//����ʵ�ʵĿɼ����ӵ���Ŀ
int CUIWidgetList::SetVisChildNum(int num)
{

	int visNum=0;
	
	if(m_ExtendStyle&WGTLIST_STYLE_DELETE_ITEM)
	{
		//ÿ�����ú�����Ŀ������ɾ�����к��ӣ�������Ӻ���
		ClearTemplates(0);
		for(int i=0;i<num;i++)
		{ 
			AddTemplate(0);
		}

		visNum=num;

	}
	else 
	{
		//ÿ�����ú�����Ŀ��������Ӳ�������ӣ��������������
		int curNum=GetTemplatesNum(0);
		while(curNum<num)
		{
			AddTemplate(0);
			curNum++;
		}

		
		//����groupstop����
		SortByGroupStop();


		std::vector<CUIWidget*>::iterator iter;
		for(iter=m_Children.begin();iter!=m_Children.end();iter++)
		{
			if(visNum<num)
			{
				(*iter)->ShowWidget(true);
				visNum++;
			}
			else 
			{
				(*iter)->ShowWidget(false);		 
			}
		}

	}	
	
	SetUpdateDirty(true);

	return visNum;

}

//�ú��������OnInit�е���
//���������Ϊ�ͻ������
//���������Ϊ�ͻ������
//��������ȸ��ݺ��ӵ��Զ����м���
void CUIWidgetList::ArrangeContent()
{

	//�ͻ������
	int absContentX=m_WinRect.ox+m_ClientRect.ox;
	int absContentY=m_WinRect.oy+m_ClientRect.oy;

	//widgetlist֧�ֺ������
	//{{{
	//m_ContentRect.width=m_ClientRect.width;
	m_ContentRect.width=0;
	//}}}

	m_ContentRect.height=0;
	
	
	//widget����ʼxλ��
	int sx=0;
	
	//widget����ʼyλ��
	int sy=m_ItemMarginV;
	
	//ǰһ��widget�Ľ���xλ��
	int ex=0;
	
	//ǰһ��widget�Ľ���yλ��
	int ey=0;
	
	//��������yλ��
	int eymax=0;

	//widgetlist֧�ֺ������
	//{{{
	int exmax=m_ClientRect.width;
	//}}}

	//����groupstop����
	SortByGroupStop();
	
	for(int i=0;i<m_Children.size();i++)
	{ 

		CUIWidget* pWgt=m_Children[i];

		if(!(m_Children[i]->IsVisible())) 
			continue;

		//vertical���ֱ�ӻ���
		BOOL bNeedReturn=IsVertical();
		if(!bNeedReturn)
		{
			int excur=m_Children[i]->GetWidth()+ex+m_ItemMarginH;
			
			//�ж��Ƿ�����
			if(excur>exmax)
			{
				//�����ȿɵ���
				if(IsSizableX())
				{	
					//��������ȣ�������,����
					if(excur>m_MaxWidth)
					{
						bNeedReturn=TRUE;
					}
					else
					{
						exmax=excur;
					}

				}
				else 
				{
					bNeedReturn=TRUE;				 
				}

			}	

		}

		if(bNeedReturn)
		{
			ex=0;
			sy=eymax+m_ItemMarginV;	
		}
		
		
		sx=ex+m_ItemMarginH;
		
		m_Children[i]->MoveToX(absContentX+sx);
		m_Children[i]->MoveToY(absContentY+sy);
			
		ex=sx+m_Children[i]->GetWidth()+m_ItemMarginH;
		ey=sy+m_Children[i]->GetHeight()+m_ItemMarginV;	
		
		if(ey>eymax) eymax=ey;

		//widgetlist֧�ֺ������
		//{{{
		if(ex>exmax) exmax=ex;
		//}}}
		
		
	}
	

	m_ContentRect.height=eymax;

	//widgetlist֧�ֺ������
	//{{{
	m_ContentRect.width=exmax;
	//}}}


	//widgetlist֧���Զ��������ڴ�С
	//{{{
	if(IsSizableX())
	{
		m_ClientRect.width=m_ContentRect.width;
		m_WinRect.width=m_ClientRect.ox+m_ClientRect.width+m_MarginH;
	}

	if(IsSizableY())
	{
		m_ClientRect.height=m_ContentRect.height;
		m_WinRect.height=m_ClientRect.oy+m_ClientRect.height+m_MarginV;
	}
	//}}}
	
	m_NumPage.x=ceil(m_ContentRect.width*1.0/m_ClientRect.width);
	m_NumPage.y=ceil(m_ContentRect.height*1.0/m_ClientRect.height);

}


//------------------------------------------------------------------------------------------
//----------------------------------class CUIListFile-----------------------------------------
//------------------------------------------------------------------------------------------
CUIListFile::CUIListFile()
{
	m_Type=WGTTYPE_LISTFILE;
}

CUIListFile::~CUIListFile()
{
	
}

void CUIListFile::ClearParameter()
{
	CUIListBox::ClearParameter();
	
	m_Directory="";

}

void CUIListFile::SetVar(stUIVar& uiVar)
{
	
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_DIRNAME:
		SetDirectory(uiVar.sValue.c_str());	
		break;

	default:
		CUIListBox::SetVar(uiVar);

	}
	
}


void CUIListFile::GetVar(stUIVar& uiVar)
{
	
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_PATHNAME:
		uiVar.sValue = GetPathName(); 	
		break;

	case VALUE_CHANNEL_DIRNAME:
		uiVar.sValue = GetDirectory();	
		break;

	default:
		CUIListBox::GetVar(uiVar);

	}
	
}

#include <direct.h> 
#include <io.h> 
//����б�
//���б���������е�ָ��Ŀ¼�µ���Ŀ
void CUIListFile::SetDirectory(  const char* name)
{ 
	if(name==0||name=="") return;
	
	if(m_ExtendStyle&LISTBOX_STYLE_DIRECTORY||m_ExtendStyle&LISTBOX_STYLE_FILE) 
	{
		
		DeleteAllItems();
		
		//���浱ǰĿ¼
		char oldPath[_MAX_PATH];
		_getcwd(oldPath,_MAX_PATH); 
		
		long handle; 
		struct _finddata_t filestruct;    
		
		std::vector<std::string> DirectoryNames;
		std::vector<std::string> FileNames;
		
		
		// ����Ҫ���ҵ�·����Ҳ��Ϊĳһ�����Ŀ¼�� 
		_chdir(name);
		
		//�õ���·��,��·���Զ�ȥ����..,.�ȶ���Ķ���
		char newPath[_MAX_PATH];
		_getcwd(newPath,_MAX_PATH); 
		
		m_Directory=newPath;		
		
		// ��ʼ���ҹ���, �ҵ���ǰĿ¼�µĵ�һ��ʵ��(�ļ�����Ŀ¼)�� 
		// �壪���ʾ�����κε��ļ�����Ŀ¼, filestructΪ���ҽ�� 
		handle = _findfirst("*", &filestruct); 
		
		// ���handleΪ��1, ��ʾ��ǰĿ¼Ϊ��, ��������Ҷ����� 
		if((handle == -1)) return; 
		
		// ����ҵ��ĵ�һ��ʵ���Ƿ���һ��Ŀ¼(filestruct.nameΪ������) 
		if( ::GetFileAttributes(filestruct.name) & FILE_ATTRIBUTE_DIRECTORY ) 
		{ 	  
			DirectoryNames.push_back(filestruct.name);			
		} 	
		else //��ʾ�ļ�
			if(m_ExtendStyle&LISTBOX_STYLE_FILE) 
				FileNames.push_back(filestruct.name);
			
			
			// �����Ե�ǰĿ¼�е���һ����Ŀ¼���ļ�����������ͬ���Ĳ��� 
			while(!(_findnext(handle,&filestruct))) 
			{ 
				
				if( ::GetFileAttributes(filestruct.name) & FILE_ATTRIBUTE_DIRECTORY ) 
				{ 						
					DirectoryNames.push_back(filestruct.name);				
				} 		
				else 
					if(m_ExtendStyle&LISTBOX_STYLE_FILE)
						FileNames.push_back(filestruct.name);
					
					
					
			} 
			
			//�����������ҹ��� 
			_findclose(handle); 
			
			//�ָ���ǰĿ¼
			_chdir(oldPath); 	
			
			for(int i=0;i<DirectoryNames.size();i++)
			{
				AddItem(DirectoryNames[i].c_str(),
					DirectoryNames[i].c_str(),
					m_ItemBkColor,
					LISTBOX_ITEM_TYPE_DIRECTORY,
					LISTBOX_ITEM_TYPE_DIRECTORY);
				
			}
			
			for(int j=0;j<FileNames.size();j++)
			{
				AddItem(FileNames[j].c_str(),
					FileNames[j].c_str(),
					m_ItemBkColor,
					LISTBOX_ITEM_TYPE_FILE,
					LISTBOX_ITEM_TYPE_FILE
					);
				
			}
			
			
	}
	
}

const char* CUIListFile::GetPathName()
{

	static std::string path;
	path=GetDirectory();
	
	//���һ���ַ�����б��
	if(path.find_last_of("\\")!=path.size()-1)
	path+="\\";
	
	std::string selText=GetSelText();
	
	if(selText!="."&&selText!="..")
	path+=selText;

	return path.c_str();

}

void CUIListFile::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIListBox::SetParameter( pvar, pfather );
	
	if(!pvar) return;
	CVar *pelement=NULL;
	
	if((pelement=pvar->Element("directory")))
		SetDirectory(pelement->String());   
	
}

int  CUIListFile::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch( msgtype )
	{

	case MSGTYPE_LBTNDBLCLK:
		{

			//Ŀ¼�ļ��б�		
			int nIndex=FindOnPoint(LOWORD(lparam),HIWORD(lparam));
			
			if(IsValidIndex(nIndex)) 
			{


				//˫����Ŀ¼
				if(GetItemData(nIndex)==LISTBOX_ITEM_TYPE_DIRECTORY)
				{
					//��ǰĿ¼·��
					std::string pathDir=GetDirectory();

					//��ǰĿ¼·�������һ�����ʽ���λ��
					int endPos=pathDir.find_last_not_of("\\");
				
					if(endPos==std::string::npos)//ȫ����б��
					endPos=-1;

					//subΪȥ�����һ������֮�������б���Ժ�õ����Ӵ�
					std::string sub=pathDir.substr(0,endPos+1);

					//��ǰĿ¼·�������һ�����ʵĿ�ʼλ��
					int startPos=sub.find_last_of('\\');
					
					if(startPos==std::string::npos)//���һ������ǰû��б��
						startPos=0;
					else
						startPos+=1;

					//��ǰĿ¼����·������
					std::string curDir=pathDir.substr(startPos,endPos-startPos+1);


					std::string newDir;
					newDir=GetDirectory();
					newDir+='\\';
					newDir+=GetItemText(nIndex);

					SetDirectory(newDir.c_str());

					UpdateScrollBar();

					//ѡ��ǰĿ¼����·������
					int idx=FindString(0,curDir.c_str());
					if(IsValidIndex(idx))
						SetSelected(idx,true);
					else 
						SetSelected(0,true);

					//��listbox��Ŀ¼ģʽ,����˫��Ŀ¼��Ŀ��ʱ��,�ŷ��͸ı�ѡ����Ϣ
					if(m_ExtendStyle&LISTBOX_STYLE_DIRECTORY)
					{
						
						CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);
						
						
					}
		
					
										
				}

				//˫�����ļ�
				else if(GetItemData(nIndex)==LISTBOX_ITEM_TYPE_FILE)					
				{
					SetSelected(nIndex,true);
					
					//ֻ�е�listbox���ļ�ģʽ,����˫���ļ���Ŀ,�ŷ��͸ı�ѡ����Ϣ
					if(m_ExtendStyle&LISTBOX_STYLE_FILE)
					{
						CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);						
						
					}

					
					
				}
				
				
				
			
			
			}
			
		}
			
		
		break;


		
	default:
		return CUIListBox::OnMessage( msgtype, wparam, lparam );
	}
	
	
	
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}



//------------------------------------------------------------------------------------------
//----------------------------------class CUIListBox-----------------------------------------
//------------------------------------------------------------------------------------------
CUIListBox::CUIListBox()
{
	m_Type=WGTTYPE_LISTBOX;
}

CUIListBox::~CUIListBox()
{
	
}


void CUIListBox::ArrangeContent()
{

	//���listbox��style��vertical����,��ʾ:������y����������Ŀ.������.
	//����,������x��������.������
	if(IsVertical()) ArrangeSingle();
	else ArrangeMulti();

}


//����:itemһ��������ǰһ��item���ұ�,�϶���;�������ContentRect���ұ�,������
//����contentrect�Ĵ�С
void CUIListBox::ArrangeMulti()
{

	//��������ʼ�ն���ͻ��������Ͻ�	
	m_ContentRect=stRect(0,0,0,0);

	//ÿ���׸�item����ʼxλ��
	int sx=0;

	//ǰһ��item����ʼyλ��
	int sy=m_ItemMarginV;

	//ǰһ��item�Ľ���xλ��,�����հ�
	int ex=0;

	//ǰһ��item�Ľ���yλ��
	int ey=0;

	//ǰ���item��y�����󵽴�λ��
	int eymax=0;
	int exmax=0;


	int col=-1;
	int row=0;

	m_nRows=0;
	m_nCols=0;



	for(int i=0;i<m_vectorItems.size();i++)
	{ 
		stRect& itemRect=m_vectorItems[i].ItemRect;

		itemRect=stRect(0,0,0,0);
		

//������///////////////////////////////////////
		
		//����,���ù̶����.
		itemRect.width=m_ItemWidth;	


//����߶�////////////////////////////////////////

		if(m_ExtendStyle&LISTBOX_STYLE_ITEMHEIGHT_FIX)//ʹ�ù̶��߶�
		{
			itemRect.height=m_ItemHeight;
		}				
		else //�Զ������߶�
		{

			//��Ӷ����հ�
			itemRect.height=m_ItemMarginV;
		
			//�������ָ߶�
			if(m_ExtendStyle&LISTBOX_STYLE_RICHTEXT)
			{
				int rWid=itemRect.width;
				int rHgt=UI_MAX_INTEGER;
				m_vectorItems[i].richText.EstimateLine(rWid,rHgt,m_TextSize,m_RowSpace);

				itemRect.height+=rHgt;
			}
			else 
			{
				itemRect.height+=g_pDisplay->EstimateLine(m_vectorItems[i].ItemText.c_str(),itemRect.width,m_TextSize,m_RowSpace);
			}
						
			//��ӵײ��հ�
			itemRect.height+=m_ItemMarginV;

		}


//����itemλ��/////////////////////////////////////////////////////
		

		//�ж��Ƿ�����
		if((itemRect.width+ex+m_ItemMarginH)>m_ContentWidth)
		{

			ex=0;
			sy=eymax+m_ItemMarginV;	

			col=0;
			row++;
		}
		else 
		{			
			col++;
		}

		sx=ex+m_ItemMarginH;
	

		itemRect.ox=sx;
		itemRect.oy=sy; 	
	
		ex=sx+itemRect.width+m_ItemMarginH;
		ey=sy+itemRect.height+m_ItemMarginV;

		if(ex>exmax) exmax=ex;

		m_vectorItems[i].ItemCol=col;
		m_vectorItems[i].ItemRow=row;

		if(row+1>m_nRows)		m_nRows=row+1;
		if(col+1>m_nCols)		m_nCols=col+1;

		//ʹ�ù̶��߶�,����Ҫ����ǰ��ͬ�еĸ߶�
		if( m_ExtendStyle&LISTBOX_STYLE_ITEMHEIGHT_FIX  )
		{
			if(ey>eymax) eymax=ey;

			continue;
		}
		
		//ǰ��ͬһ�еı��Լ���,��ǰ�����е�ͬһ��item������С
		if(ey>eymax)	
		{
			eymax=ey;
			
			for(int j=i-1;j>=0;j--)
			{ 
				if(m_vectorItems[j].ItemRow!=row) break;
				
				m_vectorItems[j].ItemRect.height=m_vectorItems[i].ItemRect.height;				
				
			}
			
		}
		//ǰ���item���Լ���,�϶���ͬһ��,���Լ�������С
		else if(i>0)
		{
			m_vectorItems[i].ItemRect.height=m_vectorItems[i-1].ItemRect.height;
			ey=itemRect.oy+itemRect.height+m_ItemMarginV;	
			
			if(ey>eymax) eymax=ey;
		}
		
	
	}

	m_ContentRect.width=exmax;
	m_ContentRect.height=eymax;


	//���й���
	m_NumPage.x=ceil(m_ContentRect.width*1.0/m_ClientRect.width);
	m_NumPage.y=ceil(m_ContentRect.height*1.0/m_ClientRect.height);
	

	

}

//����:itemֱ��������ǰһ��item������,�����
//����contentrect�Ĵ�С
void CUIListBox::ArrangeSingle()
{

	//��������ʼ�ն���ͻ��������Ͻ�	
	m_ContentRect=stRect(0,0,m_ClientRect.width,0);
	
	
	//ǰһ��item�Ľ���yλ��
	int ey=0;

	int row=-1;

	m_nRows=0;
	m_nCols=0;


	//��������
	//ÿ��item��߶���ͻ�����ߣ����Ͽհ�
	//ÿ��item�ұ߶���ͻ����ұߣ����Ͽհ�	
	int itemWidth=m_ClientRect.width-m_ItemMarginH*2;

	for(int i=0;i<m_vectorItems.size();i++)
	{ 
		stRect& itemRect=m_vectorItems[i].ItemRect;

		itemRect=stRect(m_ItemMarginH,0,itemWidth,0);

		//����߶�
		if(m_ExtendStyle&LISTBOX_STYLE_ITEMHEIGHT_FIX)//ʹ�ù̶��߶�
		{
			itemRect.height=m_ItemHeight;
		}
		else //�Զ������߶�
		{	

			//������ֶ����հ�
			itemRect.height=m_ItemMarginV;

			//�����ı��߶�
			if(m_ExtendStyle&LISTBOX_STYLE_RICHTEXT)
			{
				int rWid=itemRect.width;
				int rHgt=UI_MAX_INTEGER;
				m_vectorItems[i].richText.EstimateLine(rWid,rHgt,m_TextSize,m_RowSpace);

				itemRect.height+=rHgt;
			}
			else 
			{
				itemRect.height+=g_pDisplay->EstimateLine(m_vectorItems[i].ItemText.c_str(),itemRect.width,m_TextSize,m_RowSpace);
			}

			//������ֵײ��հ�
			itemRect.height+=m_ItemMarginV;			

		}


		//�������				
		itemRect.oy=ey+m_ItemMarginV;		
		ey=itemRect.oy+itemRect.height+m_ItemMarginV;

		//������Ҫ����
		row++;

		m_vectorItems[i].ItemCol=0;
		m_vectorItems[i].ItemRow=row;
		
		if(row+1>m_nRows)	m_nRows=row+1;
		

	}

	m_ContentRect.height=ey;

	
	//���й���
	m_NumPage.x=ceil(m_ContentRect.width*1.0/m_ClientRect.width);
	m_NumPage.y=ceil(m_ContentRect.height*1.0/m_ClientRect.height);

	
}


//------------------------------------------------------------------------------------------
//----------------------------------class CUIListBase-----------------------------------------
//------------------------------------------------------------------------------------------
CUIListBase::CUIListBase()
{
	
	m_Type = WGTTYPE_LISTBASE;

	m_pIconList=NULL;
	m_pItemDrawBack=NULL;
	m_pItemSelBack=NULL;
	m_pItemFocusBack=NULL;
	
}


CUIListBase::~CUIListBase()
{
	SAFE_RELEASE(m_pIconList);
	SAFE_RELEASE(m_pItemDrawBack);	
	SAFE_RELEASE(m_pItemSelBack);
	SAFE_RELEASE(m_pItemFocusBack);
}

void CUIListBase::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_ItemDrawFlag=LISTBOX_ITEM_FLAG_NONE;

	m_ItemDrawPos=stRect(0,0,-1,-1);	
	m_ItemSelPos=stRect(0,0,-1,-1);	
	m_ItemFocusPos=stRect(0,0,-1,-1);	
	
	m_BackColor    = stColorQuad(0,0,0,0);
	m_DrawColor  = stColorQuad(0,0,0,0);
	m_EdgeColor  = stColorQuad(0,0,0,0);
	m_FocusColor = stColorQuad(0,0,0,0);

	m_ItemDrawColor		= stColorQuad(255,255,255,255);//��͸����ɫ
	m_ItemBkColor		= stColorQuad(0,0,0,0);
	m_ItemEdgeColor		= stColorQuad(0,0,0,0);

	m_ItemSelColor		= stColorQuad(255,255,255,255);
	m_ItemSelBkColor	= stColorQuad(0,0,0,0);
	m_ItemSelEdgeColor	= stColorQuad(0,0,0,0);

	m_ItemFocusColor	= stColorQuad(255,255,255,255);
	m_ItemFocusBkColor	= stColorQuad(0,0,0,0);
	m_ItemFocusEdgeColor= stColorQuad(0,0,0,0);


	m_ItemWidth=m_TextSize;
	m_ItemHeight=m_TextSize;

	m_ItemMarginH=2;
	m_ItemMarginV=2;	

	m_Style=WGTSTYLE_VERTICAL ;//Ĭ�ϵ���	
	m_ItemTextStyle=dtCenter|dtVCenter;

	m_ContentWidth=0;

	m_IndexOffset=0;

	SAFE_RELEASE(m_pIconList);
	SAFE_RELEASE(m_pItemDrawBack);	
	SAFE_RELEASE(m_pItemSelBack);
	SAFE_RELEASE(m_pItemFocusBack);

	InitUpdate();

}

int	CUIListBase::GetTable(int channel,lua_State *ls)
{

	if(channel==VALUE_CHANNEL_SELTEXT)
	{

		int num=GetSelCount();
		
		//����ѡ����Ŀ
		lua_pushnumber(ls,num);
		
		//��ǰû��ѡ����Ŀ
		if(num<=0)
		{
			lua_pushnil(ls);			
		}
		else 
		{
		
			lua_newtable(ls);
			
			num=0;
			for(int i=0;i<m_vectorItems.size();i++)
			{ 
				const stListItem& item=m_vectorItems[i];
				if(item.ItemState&UI_ITEM_STATE_SELECTED)
				{
					SetTableElement(ls,num++,item.ItemText.c_str());					
				}			
			}
		}

		return 2;
	}
	else if(channel==VALUE_CHANNEL_ITEMTEXT)
	{
		int num=m_vectorItems.size();
		
		//������Ŀ
		lua_pushnumber(ls,num);
		
		//��ǰû����Ŀ
		if(num<=0)
		{
			lua_pushnil(ls);			
		}
		else 
		{
		
			lua_newtable(ls);
			
			for(int i=0;i<m_vectorItems.size();i++)
			{ 
				const stListItem& item=m_vectorItems[i];
				
				SetTableElement(ls,i,item.ItemText.c_str());

			}
		}

		return 2;
	}
	else
		return CUIWidget::GetTable(channel,ls);	

}


const char* CUIListBase::GetTipText(int nIndex)
{
	if(IsValidIndex(nIndex))
		return GetItemTip(nIndex);
	else 
		return m_TipText.c_str();

}


void CUIListBase::SetTipText( const char *pTip, int nIndex/*=-1*/)
{
	assert(pTip);

	if(IsValidIndex(nIndex))
		SetItemTip(nIndex,pTip);
	else 
		m_TipText=pTip;

}



//���scroll�Ǻ���,�Զ�ע��
void CUIListBase::AddChild( CUIWidget *pwidget )
{
	
	CUIWidget::AddChild(pwidget);
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{

		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",this);
		
		if(pScrollBar->IsVertical())
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",pScrollBar);
		else 
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY,BUDDY_STYLE_SELF,"",pScrollBar);
		

	}
	
	
}

void CUIListBase::DelChild( CUIWidget *pwidget )
{
	
	if(pwidget->GetType()==WGTTYPE_SCROLLBAR) 
	{

		CUIScrollBar* pScrollBar=(CUIScrollBar*)pwidget;
		pScrollBar->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",0);
		
		if(pScrollBar->IsVertical())
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,"",0);
		else 
		this->RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY,BUDDY_STYLE_SELF,"",0);
		

	}
	
	CUIWidget::DelChild(pwidget);
	
}


int CUIListBase::ParseKeyChar(int x,int y,stKeyChar& keyChar)
{
	for(int i=0;i<m_vectorItems.size();i++)		
	{
		//���⿽��
		stListItem* pItem= &m_vectorItems[i];

		stRect absItemRct(pItem->ItemRect.ox+m_ClientRect.ox+m_WinRect.ox,
						  pItem->ItemRect.oy+m_ClientRect.oy+m_WinRect.oy,
						  pItem->ItemRect.width,pItem->ItemRect.height);

		if(absItemRct.IsPointIn(x,y))
		{

			return (pItem->richText.ParseKeyChar(keyChar,x,y,
					absItemRct.ox,absItemRct.oy,absItemRct.width,absItemRct.height,
					m_TextSize,m_RowSpace,m_TextStyle));

		}

	}

	return RICHCHAR_CMD_NONE;
}


void CUIListBase::RenderSelf()
{

	CUIWidget::RenderSelf();
	
	for(int i=0;i<m_vectorItems.size();i++)		
	DrawItem(i);	
	
}

 
void CUIListBase::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );

	if(!pvar) return;
	CVar *pelement=NULL;

	int tmp[4];

	if( (pelement=pvar->Element("contentwidth")) )
	SetContentWidth( pelement->Integer());
	else//Ĭ��Ϊ�ͻ������ 
	SetContentWidth( m_ClientRect.width);


	if( (pelement=pvar->Element("itemdrawflag")) )
		SetItemDrawFlag(pelement->Integer());


	if( (pelement=pvar->Element("itemdrawpos")) )
	{
		pelement->IntegerArray(tmp,4);
		SetItemDrawPos(stRect(tmp[0],tmp[1],tmp[2],tmp[3]));
	}

	if( (pelement=pvar->Element("itemselpos")) )
	{
		pelement->IntegerArray(tmp,4);
		SetItemSelPos(stRect(tmp[0],tmp[1],tmp[2],tmp[3]));
	}

	if( (pelement=pvar->Element("itemfocuspos")) )
	{
		pelement->IntegerArray(tmp,4);
		SetItemFocusPos(stRect(tmp[0],tmp[1],tmp[2],tmp[3]));
	}


	if( (pelement=pvar->Element("itemdrawback")) )
		SetItemDrawBack( pelement->String());

	if( (pelement=pvar->Element("itemselback")) )
		SetItemSelBack( pelement->String());

	if( (pelement=pvar->Element("itemfocusback")) )
		SetItemFocusBack( pelement->String());
	
	if( (pelement=pvar->Element("iconlist")) )
		SetIconList( pelement->String());

	if( (pelement=pvar->Element("itemwidth")) )
		SetItemWidth( pelement->Integer());
	
	if( (pelement=pvar->Element("itemheight")) )
		SetItemHeight( pelement->Integer());
	
	if( (pelement=pvar->Element("itemmarginh")) )
		SetItemMarginH( pelement->Integer());
	
	if( (pelement=pvar->Element("itemmarginv")) )
		SetItemMarginV( pelement->Integer());

	if( (pelement=pvar->Element("itemtextstyle")) )
		SetItemTextStyle( pelement->Integer());

	//item��ɫ����//////////////////////////////////////

	if( (pelement=pvar->Element("itemdrawcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemDrawColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("itembkcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemBackColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("itemedgecolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemEdgeColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}


	if( (pelement=pvar->Element("itemfocuscolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemFocusColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("itemfocusbkcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemFocusBkColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("itemfocusedgecolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemFocusEdgeColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("itemselcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemSelColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("itemselbkcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemSelBkColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("itemseledgecolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetItemSelEdgeColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}


	////////////////////////ITEMS////////////////////////////////////////

	//���ȱʡ���Ե���Ŀ
	if( (pelement=pvar->Element("itemnum")) )
		SetItemCount(pelement->Integer());	
	
	if( (pelement=pvar->Element("itemrect")) )	
	{
		
		CVarArray* rectArray=(CVarArray*)pelement;
		
		int itemnum=rectArray->GetNum();
		
		//���ܳ�������
		if(itemnum> GetItemCount()) itemnum=GetItemCount();
		
		for(int i=0;i<itemnum;i++)
		{ 
			CVarArray *pRectVar=(CVarArray*)(rectArray->Element(i));
			
			stRect rect;
			pRectVar->Rect(rect);
			SetItemRect(i,rect);
			
		}
		
	}	
	
	if( (pelement=pvar->Element("itempos")) )	
	{
		
		CVarArray* posArray=(CVarArray*)pelement;
		
		int col=0;
		int row=0;

		int itemnum=posArray->GetNum();
		
		//���ܳ����ַ�������
		if(itemnum> GetItemCount()) itemnum=GetItemCount();
		
		for(int i=0;i<itemnum;i++)
		{ 
			CVarArray *pPosVar=(CVarArray*)(posArray->Element(i));
			
			if(pPosVar->GetNum()>1)
			{
				row=((CVar*)pPosVar->Element(0))->Integer();
				col=((CVar*)pPosVar->Element(1))->Integer();
			}
			
			SetItemRow(i,row);
			SetItemCol(i,col);

			char text[UI_TEMPORARY_BUFSIZE];
			sprintf(text,"%d %d",row,col);
			SetItemText(i,text);
			
		}
		
	}	

	//Ӧ�ý����text�ķ��������ΪҪʹ��ǰ��ĸ�������ֵ
	if( (pelement=pvar->Element("itemtext")) )
	{
		const char* text[LISTBOX_MAX_ITEMS];
		int itemnum=LISTBOX_MAX_ITEMS;

		itemnum=pelement->StringArray(text,itemnum);

		//���ܳ����ַ�������
		if(itemnum> GetItemCount()) itemnum=GetItemCount();
		
		for(int i=0;i<itemnum;i++)
		SetItemText(i,text[i]);
		
	}

	if( (pelement=pvar->Element("itemtip")) )
	{
		const char* tip[LISTBOX_MAX_ITEMS];
		int itemnum=LISTBOX_MAX_ITEMS;
		
		itemnum=pelement->StringArray(tip,itemnum);

		//���ܳ����ַ�������
		if(itemnum> GetItemCount()) itemnum=GetItemCount();

		
		for(int i=0;i<itemnum;i++)
		SetItemTip(i,tip[i]);//��ʹ��ȱʡ����ɫ
		
	}
	


	if( (pelement=pvar->Element("itemcolor")) )	
	{
		
		CVarArray* colorArray=(CVarArray*)pelement;
		
		int itemnum=colorArray->GetNum();
		
		//���ܳ����ַ�������
		if(itemnum> GetItemCount()) itemnum=GetItemCount();
		
		for(int i=0;i<itemnum;i++)
		{ 
			CVarArray *pColorVar=(CVarArray*)(colorArray->Element(i));
		
			stColorQuad color;
			pColorVar->ColorQuad(color);			
			SetItemColor(i,color);

		}
		
	}	
	


	if( (pelement=pvar->Element("itemicon")) )
	{
		int icons[LISTBOX_MAX_ITEMS];
		int itemnum=LISTBOX_MAX_ITEMS;
		
		itemnum=pelement->IntegerArray(icons,itemnum);
		
		//���ܳ�������
		if(itemnum> GetItemCount()) itemnum=GetItemCount();
		
		
		for(int i=0;i<itemnum;i++)
			SetItemIcon(i,icons[i]);
		
	}

	if( (pelement=pvar->Element("itemdata")) )
	{
		int datas[LISTBOX_MAX_ITEMS];
		int itemnum=LISTBOX_MAX_ITEMS;
		
		itemnum=pelement->IntegerArray(datas,itemnum);
		
		//���ܳ����ַ�������
		if(itemnum> GetItemCount()) itemnum=GetItemCount();
		
		
		for(int i=0;i<itemnum;i++)
		SetItemData(i,datas[i]);
		
	}
	
	
}


void CUIListBase::GetVar(stUIVar& uiVar)
{

	switch(uiVar.iChannel)
	{
	
	case VALUE_CHANNEL_INDEXOFFSET:
		uiVar.fValue=m_IndexOffset;
		break;

	case VALUE_CHANNEL_SEL:
		uiVar.fValue = m_CurSel; 	
		break;

	case VALUE_CHANNEL_HOT:
		uiVar.fValue = m_CurHot;
		break;

	case VALUE_CHANNEL_PAGE_H:
		uiVar.fValue = m_CurPage.x; 	
		break;

	case VALUE_CHANNEL_PAGE_V:
		uiVar.fValue = m_CurPage.y; 	
		break;

	case VALUE_CHANNEL_PAGENUM_H:
		uiVar.fValue = m_NumPage.x; 
		break;

	case VALUE_CHANNEL_PAGENUM_V:
		uiVar.fValue = m_NumPage.y;
		break;

	case VALUE_CHANNEL_ITEM_NUM:
		uiVar.fValue = GetItemCount();
		break;

	case VALUE_CHANNEL_ITEMCOLOR:
		uiVar.fValue = GetItemColor(uiVar.iIndex).c;
		break;

	//text////////////////////////////////////////////////////////////////////////

	case VALUE_CHANNEL_SELTEXT:
		uiVar.sValue = GetSelText();	
		break;

	case VALUE_CHANNEL_ITEMTEXT:
		uiVar.sValue = GetItemText(uiVar.iIndex);	
		break;
	
	default:
		CUIWidget::GetVar(uiVar);

	}
	
	
}

void CUIListBase::SetVar(stUIVar& uiVar)
{

	
	switch(uiVar.iChannel)
	{

	case VALUE_CHANNEL_INDEXOFFSET:
		SetIndexOffset(uiVar.fValue);
		break;

	case VALUE_CHANNEL_ITEM_NUM:
		SetItemCount(uiVar.fValue);
		break;

	case VALUE_CHANNEL_SEL:
		if(SetSelected((int)uiVar.fValue,true))
		{
			if(uiVar.iIndex>0)
			{
				CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);			
			}
		}
		break;

	case VALUE_CHANNEL_HOT:
		SetCurHot((int)uiVar.fValue);	
		break;

	case VALUE_CHANNEL_PAGE_H:
		GotoPageH((int)uiVar.fValue);
		break;

	case VALUE_CHANNEL_PAGE_V:
		GotoPageV((int)uiVar.fValue);
		break;

	case VALUE_CHANNEL_ITEMCOLOR:
		SetItemColor(uiVar.iIndex,stColorQuad(int(uiVar.fValue)));	
		break;

	//text////////////////////////////////////////////////////////////////////////

	case VALUE_CHANNEL_ITEMTEXT:
		SetItemText(uiVar.iIndex,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_ICONLIST:
		SetIconList(uiVar.sValue.c_str());
		break;

	default:
		CUIWidget::SetVar(uiVar);

	}
	
}


void CUIListBase::AjustSelf()
{
	UpdateScrollBar();

	SetUpdateDirty(false);
}

//���и�����Ŀ(�򵥵ļ�����ռ�ݵ���������)
//���Ա���������
void CUIListBase::ArrangeContent()
{

	//��������ʼ�ն���ͻ��������Ͻ�	
	m_ContentRect.ox=0;
	m_ContentRect.oy=0;
	
	//����֮ǰ,ָ������������Ϊ�ͻ������
	m_ContentRect.width=0;
	m_ContentRect.height=0;

	int ey=0;
	stRect rect;
	
	for(int i=0;i<m_vectorItems.size();i++)
	{ 
		rect=m_vectorItems[i].ItemRect;
		
		if((m_vectorItems[i].ItemRow+1)>m_nRows)		m_nRows=(m_vectorItems[i].ItemRow+1);
		if((m_vectorItems[i].ItemCol+1)>m_nCols)		m_nCols=(m_vectorItems[i].ItemCol+1);
		
		if((rect.oy+rect.height)>ey) 
			ey=(rect.oy+rect.height);
		
	}
	
	m_ContentRect.height=ey+m_ItemMarginV;

}


void CUIListBase::UpdateSelf( float deltatime )
{

	if(m_ExtendStyle&LISTBOX_STYLE_AUTO_COMFORT)
	{
		ComfortSelf();
	}

	CUIWidget::UpdateSelf(deltatime);
}

void CUIListBase::PostUpdateSelf( float deltatime )
{

	if(IsUpdateDirty()) 
	{
		UpdateScrollBar();
	
		SetUpdateDirty(false);
	}
	
	CUIWidget::PostUpdateSelf(deltatime);

}

void CUIListBase::UpdateScrollBar()
{

	//listbase֮bug�޸�
	Point2D curScrollPos(m_ContentRect.ox,m_ContentRect.oy);

	ArrangeContent();

	CUIWidget* pBuddy;

	//���ݼ���ó����������߶���������ֱ������
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pBuddy;
		
		int nLower=m_ClientRect.height-m_ContentRect.height;
		if(nLower>0) nLower=0;
		
		pScrollBar->SetRange(nLower,0);		
		
	}	
	

	//���ݼ���ó������������������ˮƽ������
	pBuddy=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY);
	if(pBuddy&&pBuddy->GetType()==WGTTYPE_SCROLLBAR)
	{
		CUIScrollBar* pScrollBar=(CUIScrollBar*)pBuddy;


		int nUpper=m_ContentRect.width-m_ClientRect.width;
		if(nUpper<0) nUpper=0;
		
		pScrollBar->SetRange(0,nUpper);		
		
	}	

	ScrollContentH(curScrollPos.x);
	ScrollContentV(curScrollPos.y);
	
}



void CUIListBase::OnInit()
{
	
	UpdateScrollBar();
	
	SetSelected(m_CurSel,true);
	
	CUIWidget::OnInit();
	
}


int  CUIListBase::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_SCROLLBAR_POS:
		{

			CUIScrollBar* pBar=(CUIScrollBar*)lparam;
			if(pBar)
			{
				
				if(pBar->IsVertical())
				{					
					ScrollContentV((int)wparam);	
				}
				else 
				{
					ScrollContentH(-(int)wparam);	
				}
				
				
			}
			
		
		}
		break;
		
		
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}

int  CUIListBase::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch( msgtype )
	{

	case MSGTYPE_LBTNDBLCLK:
		{
			
			//����Ƿ������ĳ����Ŀ��
			int nIndex=FindOnPoint(LOWORD(lparam),HIWORD(lparam));

			if(IsValidIndex(nIndex))
			{
							
				if(m_Style&WGTSTYLE_SYSTEMDRAW)
				{
					
					if(m_DragType!=UI_DRAG_TYPE_NONE)
					{			
						stClickItem useItem;
						useItem.type=m_DragType;
						useItem.index=nIndex;
						useItem.hosttype=m_HostType;
						useItem.hostindex=m_HostIndex;
						
						if(m_IndexOffset>0) useItem.index+=m_IndexOffset;


						ExecCallBackFunc(UI_Callback_DClickItem,(void*)&useItem);
					}
			
				}
				
				//modify+lincoln
				//[2005:4:13] [14:56]
				//�����ѡ��״̬����
				//{{{	
				if(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)
				{
					
					if(IsSelected(nIndex))
					{
						SetSelected(nIndex,false);
					}
					else
					{
						SetSelected(nIndex,true);
						CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);
					}
					
				}
				else
				{
					SetSelected(nIndex,true);
					
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);
				}
				//}}}
							
				
				CallSafeScriptEx(SCRIPT_FUNC_ON_DBCLICK,"%d,%d,%d",nIndex,wparam,lparam);

			}
			 
			
	
		}
		break;

	case MSGTYPE_LEFTBTNDOWN:	
		{

			m_State |= WGTSTATE_CLICKDOWN;

				
			int nIndex=FindOnPoint(LOWORD(lparam),HIWORD(lparam));

			if(IsValidIndex(nIndex)) 
			{
				if(m_Style&WGTSTYLE_SYSTEMDRAW)
				{					
					if(m_DragType!=UI_DRAG_TYPE_NONE)
					{
						stClickItem useItem;
						useItem.type=m_DragType;
						useItem.index=nIndex;
						useItem.hosttype=m_HostType;
						useItem.hostindex=m_HostIndex;
						
						if(m_IndexOffset>0) useItem.index+=m_IndexOffset;

						ExecCallBackFunc(UI_Callback_ClickItem,(void*)&useItem);			
					}

				}

				if(m_ExtendStyle&LISTBOX_STYLE_DBCLICK_SEL)
				{

					if(nIndex!=GetCurHot())
					SetCurHot(nIndex);						
					
				}
				else if(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)
				{
					
					if(IsSelected(nIndex))
					{
						SetSelected(nIndex,false);
					}
					else
					{
						SetSelected(nIndex,true);
						CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);
					}
					
				}
				else
				{
					SetSelected(nIndex,true);
					
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);
				}
				

			}

			CallSafeScriptEx(SCRIPT_FUNC_ON_CLICK,"%d,%d,%d",nIndex,wparam,lparam);

		}

		break;
		

	case MSGTYPE_RIGHTBTNDOWN:
		{
			
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			int nIndex=FindOnPoint(x,y);
			if(IsValidIndex(nIndex))
			{

				//modify+lincoln
				//[2005:4:13] [14:56]
				//�����ѡ��״̬����
				//{{{					
				SetCurHot(nIndex);	
				//}}}


				if(IsInItemDragRect(x,y))
				{					
					if(m_DragType!=UI_DRAG_TYPE_NONE)
					{
								
						stClickItem useItem;
						useItem.type=m_DragType;
						useItem.index=nIndex;
						useItem.hosttype=m_HostType;
						useItem.hostindex=m_HostIndex;
						
						if(m_IndexOffset>0) useItem.index+=m_IndexOffset;
						
						ExecCallBackFunc(UI_Callback_RClickItem,(void*)&useItem);			
					}

				}

				if(m_IndexOffset>0) nIndex+=m_IndexOffset;
				ParseInfotip(nIndex);
			}

			CallSafeScriptEx(SCRIPT_FUNC_ON_RCLICK,"%d",nIndex);
		}
		
		
		break;

	case MSGTYPE_MOUSEWHEEL:
		{
			int nIndex=-1;
			short offset=HIWORD(wparam);
			
			//֧�������ֹ���
			if(m_ExtendStyle&LISTBOX_STYLE_WHEEL)
			{
				//������Ŀ����
				if(m_ExtendStyle&LISTBOX_STYLE_ENUM_ITEM)
				{

					nIndex=GetCurSel();

					if(offset>0) nIndex++;
					else nIndex--;

					if(IsValidIndex(nIndex)) 
					{
					
						SetSelected(nIndex,true);
							
						CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);					
						
					}
					
					
				}
				else //�������ع���
				{
					if(offset>0) OffsetContentV(m_MouseDelta);	
					else OffsetContentV(-m_MouseDelta);
				}
								
				

			}
			
			
		}
		
		
		break;

	case MSGTYPE_MOUSEIDLE:
		{
			//֪ͨ����µĴ���:���ϴ����ֹͣ�ƶ�����,����������ͣ���˶��
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);	
			int nIndex=FindOnPoint(x,y);

			if(IsValidIndex(nIndex))
			{
				stRect itemRect=GetItemRect(nIndex);
				stRect winRect=GetWinRect();
				itemRect.ox+=winRect.ox;
				itemRect.oy+=winRect.oy;

				if(m_IndexOffset>0) nIndex+=m_IndexOffset;
			
				if(ParseTooltip(nIndex,itemRect,x,y)==UI_Cmd_SHOW)
				{
					AcceptIdle(false);
				}
			}

		}		
		break;


	case MSGTYPE_MOUSEMOVE:
		{
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			int nIndex;
			nIndex=FindOnPoint(x,y);

			if(IsValidIndex(nIndex)) 
			{
				if(nIndex!=GetCurHot())
				{
					SetCurHot(nIndex);				
				}

				if(m_CurAnchor!=nIndex)
				AcceptIdle(true);

			}
			else if(IsPointInClientRect(x,y))
			{
				SetTipText("");
				HideTooltip(false,x,y);
			}


			//��¼���λ��
			m_CurAnchor=nIndex;
			
		}
				
		break;	
	
	case MSGTYPE_MOUSEOUT:
		{		
			//��¼���λ��
			m_CurAnchor=-1;

			m_State &= ~WGTSTATE_MOUSEOVER;
			m_State &= ~WGTSTATE_CLICKDOWN;			
			
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);
			HideTooltip(true,x,y);
			
			CallSafeScript(SCRIPT_FUNC_ON_MOUSEOUT);
		}			
		break;


	case MSGTYPE_KEYDOWN:
		return OnKeyDown( wparam, lparam );
		break;
		
	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );
	}
	
	
	
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}

int CUIListBase::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch( wparam )
	{ 

	case VK_RETURN:
		{

			if(m_CurHot!=m_CurSel)
			{
				SetSelected(m_CurHot,true);
				CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);				
			}

		}
		break;

	
	case VK_UP:
		{

			int nIndex=(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)?m_CurHot:m_CurSel;
			int nRow=GetItemRow(nIndex)-1;
			int nCol=GetItemCol(nIndex);
			nIndex=ApproximateIndex(nRow,nCol);
			
			if(IsValidIndex(nIndex)) 
			{
				//modify+lincoln
				//[2005:4:13] [14:56]
				//�����ѡ��״̬����
				//{{{				
				if(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)
				{
					SetCurHot(nIndex);
				}
				else 
				{
					SetSelected(nIndex,true);									
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);					 
				}
				/*
				if(m_ExtendStyle&LISTBOX_STYLE_HOVER_SEL)
				{
					SetSelected(nIndex,true);
					
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);
					
				}
				else 
				{
					SetCurHot(nIndex);
				}
				*/
				//}}}
				
				
			}

		}
		
		break;

	case VK_DOWN:
		{
			
			
			int nIndex=(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)?m_CurHot:m_CurSel;
			int nRow=GetItemRow(nIndex)+1;
			int nCol=GetItemCol(nIndex);
			nIndex=ApproximateIndex(nRow,nCol);
			
			if(IsValidIndex(nIndex)) 
			{
				//modify+lincoln
				//[2005:4:13] [14:56]
				//�����ѡ��״̬����
				//{{{				
				if(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)
				{
					SetCurHot(nIndex);
				}
				else 
				{
					SetSelected(nIndex,true);									
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);					 
				}
				/*
				if(m_ExtendStyle&LISTBOX_STYLE_HOVER_SEL)
				{
					SetSelected(nIndex,true);
					
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);
				
				}
				else 
				{					
					SetCurHot(nIndex);
				}
				*/
				//}}}				
				
			}
			
		}
		break;

	case VK_LEFT:
		{
			
			
			int nIndex=(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)?m_CurHot:m_CurSel;
			int nRow=GetItemRow(nIndex);
			int nCol=GetItemCol(nIndex)-1;
			nIndex=ApproximateIndex(nRow,nCol,false);
			
			if(IsValidIndex(nIndex)) 
			{
				//modify+lincoln
				//[2005:4:13] [14:56]
				//�����ѡ��״̬����
				//{{{				
				if(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)
				{
					SetCurHot(nIndex);
				}
				else 
				{
					SetSelected(nIndex,true);									
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);					 
				}
				/*
				if(m_ExtendStyle&LISTBOX_STYLE_HOVER_SEL)
				{
					SetSelected(nIndex,true);
					
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);					
					
				}
				else 
				{
					SetCurHot(nIndex);
				}
				*/
				//}}}
				
				
			}
			
		}		
		break;


	case VK_RIGHT:
		{
			
			
			int nIndex=(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)?m_CurHot:m_CurSel;
			int nRow=GetItemRow(nIndex);
			int nCol=GetItemCol(nIndex)+1;
			nIndex=ApproximateIndex(nRow,nCol,false);
			
			if(IsValidIndex(nIndex)) 
			{
				
				//modify+lincoln
				//[2005:4:13] [14:56]
				//�����ѡ��״̬����
				//{{{				
				if(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)
				{
					SetCurHot(nIndex);
				}
				else 
				{
					SetSelected(nIndex,true);									
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);					 
				}
				/*
				if(m_ExtendStyle&LISTBOX_STYLE_HOVER_SEL)
				{
					SetSelected(nIndex,true);
					
					CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%d",m_CurSel);					
					
				}
				else 
				{					
					SetCurHot(nIndex);
				}
				*/
				//}}}
				
				
			}
			
		}		
		break;

		

	case VK_NEXT:

		GotoPageV(GetCurPageV()+1);

		break;

	case VK_PRIOR:
		
		GotoPageH(GetCurPageV()-1);
		
		break;
		

	default:


		return CUIWidget::OnKeyDown(wparam,lparam,bVirtual);

	}

	return 1;
}



void CUIListBase::GotoPageH(int nPage)
{
	
	if(nPage>=m_NumPage.x) nPage=m_NumPage.x-1;

	if(nPage<0) nPage=0;
	
	int pos=-m_ClientRect.width*nPage;
	ScrollContentH(pos);
	
	SetTipText("");
	
	HideTooltip(false,0,0);

}

void CUIListBase::GotoPageV(int nPage)
{
	
	if(nPage>=m_NumPage.y) nPage=m_NumPage.y-1;
	
	if(nPage<0) nPage=0;
	
	int pos=-m_ClientRect.height*nPage;
	ScrollContentV(pos);
	
	SetTipText("");
	
	HideTooltip(false,0,0);
	
	
}


int  CUIListBase::GetCurPageH()
{
	return m_CurPage.x;
}

int  CUIListBase::GetCurPageV()
{
	return m_CurPage.y;
}


int  CUIListBase::GetNumPageH()
{
	return m_NumPage.x;
}


int  CUIListBase::GetNumPageV()
{
	return m_NumPage.y;
}



int CUIListBase::GetItemCount()
{  
	return m_vectorItems.size();
}



int CUIListBase::GetCurSel()
{
	return m_CurSel;
}

int CUIListBase::GetCurHot()
{
	return m_CurHot;
}




BOOL CUIListBase::IsValidIndex(int nIndex)
{

	if( nIndex<0 || nIndex>=m_vectorItems.size() )
	return false;
	else 
	return true;
	
}

int CUIListBase::IsSelected(int nIndex)
{   
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return -1;//error
	
	if(m_vectorItems[nIndex].ItemState&UI_ITEM_STATE_SELECTED) 
		return 1;//selected
	else
		return 0;//not selected

}		

//���Ҿ�ȷƥ�������,�ҵ��ͷ���
//���û���ҵ�,�ٰ�ָ�������ȷ�ʽ,�ҵ�һ����ƥ�������
int  CUIListBase::ApproximateIndex(int nRow,int nCol,bool bRowPrior/*=true*/)
{

	int nIndex=-1;
	int nNear=m_vectorItems.size()*m_vectorItems.size();
	int curNear;
	
	for(int i=0;i<m_vectorItems.size();i++)
	{

		//�ҵ��˾�ȷƥ���
		if(m_vectorItems[i].ItemRow==nRow && m_vectorItems[i].ItemCol==nCol)
		return i;

		//��ģ��ƥ���
		if(bRowPrior)
		curNear=m_vectorItems.size()*abs(m_vectorItems[i].ItemRow-nRow)+abs(m_vectorItems[i].ItemCol-nCol);
		else 
		curNear=m_vectorItems.size()*abs(m_vectorItems[i].ItemCol-nCol)+abs(m_vectorItems[i].ItemRow-nRow);

		if(curNear<nNear)
		{
			nNear=curNear;
			nIndex=i;
		}

	}
	
	return nIndex;


}


//�ҵ�һ����ȷƥ�������
int CUIListBase::TranslateIndex(int nRow,int nCol)
{


	for(int i=0;i<m_vectorItems.size();i++)
	if(m_vectorItems[i].ItemRow==nRow&&m_vectorItems[i].ItemRow==nCol)
	return i;

	return -1;//û�ҵ�


}

void CUIListBase::SetCurHot(int nIndex)
{
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;
	
	if( m_CurHot>=0 && m_CurHot<m_vectorItems.size())
		m_vectorItems[m_CurHot].ItemState&=~UI_ITEM_STATE_FOCUSED;//ȡ��FOCUS
	
	m_CurHot=nIndex;
	
	m_vectorItems[nIndex].ItemState|=UI_ITEM_STATE_FOCUSED;//��FOCUS
		
	EnsureItemVisible(m_vectorItems[m_CurHot]);
	
}

BOOL CUIListBase::SetSelected(int nIndex, BOOL bSelect)
{   
	    
	    if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return FALSE;

		if(bSelect) 
		{	
			BOOL bDirty=FALSE;
			if(!(m_ExtendStyle&LISTBOX_STYLE_MULTI_SEL)) //��ѡ���
			{
				if(m_CurSel>=0&&m_CurSel<m_vectorItems.size()) 
				{
					m_vectorItems[m_CurSel].ItemState&=~UI_ITEM_STATE_SELECTED;//ȡ��ѡ��
				}

				bDirty=(m_CurSel!=nIndex);
			}
				
			m_vectorItems[nIndex].ItemState|=UI_ITEM_STATE_SELECTED;//��ѡ��

			m_CurSel=nIndex;
			SetCurHot(nIndex);	

			return bDirty;
			
		}
		else
		{
			m_vectorItems[nIndex].ItemState&=~UI_ITEM_STATE_SELECTED;//ȡ��ѡ��

			return FALSE;
		}

}



void CUIListBase::EnsureItemVisible(stListItem item)
{

	int visRelation=IsItemVisible(item);

	if((visRelation==RECT_TOTAL_INSIDE)||(visRelation==RECT_TOTAL_OUTSIDE)) return;

	Point2D offset(0,0);

	if((item.ItemRect.oy+item.ItemRect.height)>m_ClientRect.height)//item�ײ������ͻ����ײ�
	{

		offset.y=m_ClientRect.height-(item.ItemRect.oy+item.ItemRect.height);
		offset.y-=m_ItemMarginV;
	}
	else if(item.ItemRect.oy<0)//item���������ͻ�������
	{
		offset.y=-item.ItemRect.oy;
		offset.y+=m_ItemMarginV;
	}

	if((item.ItemRect.ox+item.ItemRect.width)>m_ClientRect.width)//item�ұ߳����ͻ����ұ�
	{
		
		offset.x=m_ClientRect.width-(item.ItemRect.ox+item.ItemRect.width);
		offset.x-=m_ItemMarginH;
	}
	else if(item.ItemRect.ox<0)//item��߳����ͻ������
	{
		offset.x=-item.ItemRect.ox;
		offset.x+=m_ItemMarginH;
	}

	OffsetContentH(offset.x);
	OffsetContentV(offset.y);

}

int CUIListBase::IsItemVisible(stListItem item)
{

	stRect dstRect;

	dstRect.ox=0;
	dstRect.oy=0;
	dstRect.width=m_ClientRect.width;
	dstRect.height=m_ClientRect.height;

	int result=IsRectIntersect(item.ItemRect,dstRect);

	return result;

}

int CUIListBase::GetSelCount() 
{   

	int count=0;
	for(int i=0;i<m_vectorItems.size();i++)
	{ 
		if(m_vectorItems[i].ItemState&UI_ITEM_STATE_SELECTED) 
		count++;
		
	}
	
	return count;

}

//����֮ǰ�������rgIndex�����ڴ�
//����ѡ�������
int CUIListBase::GetSelItems(int nMaxItems, int*rgIndex) 
{   

	int count=0;
	
	for(int i=0;i<m_vectorItems.size();i++)
	{ 
		if(m_vectorItems[i].ItemState&UI_ITEM_STATE_SELECTED) 
			rgIndex[count++]=i;
	
		if(count>=nMaxItems) break;
		
	}
	
	return count;	

}

//���������Ŀ,reset��listbox
//���count��ȱʡ��ɫ����Ŀ,�ı�����ʾ����.
void CUIListBase::SetItemCount(int count)
{
	
	
	InitUpdate();
	
	if(count>LISTBOX_MAX_ITEMS) count=LISTBOX_MAX_ITEMS;
	
	for(int i=0;i<count;i++)
	AddItem("","",m_ItemBkColor,i,i);//��ʹ��ȱʡ����ɫ,�ı�����ʾΪ��,��ͼ��,����Ϊ����
	
	UpdateScrollBar();
	
	
	
}

//Ӧ���ڴ˴���scrollbar��ʾ

void  CUIListBase::AddItem( const char* ptext,const char* ptip/*=0*/,
						  stColorQuad bkcolor/*=stColorQuad(0,0,0,0)*/,
						  int icon/*=-1*/,int data/*=-1*/,
						  int row/*=-1*/,int col/*=-1*/,
						  int state/*=UI_ITEM_STATE_DEFAULT*/)
{   

	stListItem item;
	
	item.ItemText=ptext;

	if(m_ExtendStyle&LISTBOX_STYLE_RICHTEXT)
	{
		item.richText.ParseTxt(ptext,m_RichMode,m_RichFlag);
	}

	if(ptip)  item.ItemTip=ptip;
	else	  item.ItemTip=ptext;
	
	item.ItemColor=bkcolor;
	

	item.ItemData=data;

	item.ItemIcon=icon;

	item.ItemState=state;

	//////////////////////////////////////////////////////////////////////////
	
	item.ItemRect=stRect(0,0,0,0);
	
	item.ItemRow=0;
	item.ItemCol=0;

	//////////////////////////////////////////////////////////////////////////
	
	
	m_vectorItems.push_back(item);	


}


void CUIListBase::DeleteAllItems()
{

	m_vectorItems.clear();		
	
	UpdateScrollBar();
	

}

//Ӧ���ڴ˴���scrollbar����
void  CUIListBase::DeleteItem(int nIndex)
{   

	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;

	
	m_vectorItems.erase(m_vectorItems.begin()+nIndex);		
	
	UpdateScrollBar();


}



int  CUIListBase::FindString(int nStartAfter,  const char* ptext) 
{   
	if( nStartAfter<0 || nStartAfter>=m_vectorItems.size() )
	nStartAfter = 0;
	
		for(int i=nStartAfter;i<m_vectorItems.size();i++)
		{
			if(m_vectorItems[i].ItemText==ptext) 
			return i;
		}

		return -1;//not found
	
}

//return 0:not valid string found
//return 1:selected
int  CUIListBase::SelectString(int nStartAfter,  const char* ptext) 
{   

	if( nStartAfter<0 || nStartAfter>=m_vectorItems.size() )
	nStartAfter=0;

	int nIndex=FindString(nStartAfter,ptext);
	
	if(nIndex>=0&&nIndex<m_vectorItems.size()) 	
	{ 
		SetSelected(nIndex,true);
		return 1;
	}
	else 
	{
	 
		return 0;
	}

}



void  CUIListBase::SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
{   

	for(int i=nFirstItem;i<=nLastItem;i++)
	SetSelected(i,bSelect);

}	


void CUIListBase::InitUpdate()
{
	m_CurSel=-1;
	m_CurHot=-1;
	m_CurAnchor=-1;
	
	m_CurPage.x=0;
	m_CurPage.y=0;
	
	m_NumPage.x=0;
	m_NumPage.y=0;

	m_nRows=0;
	m_nCols=0;

	m_ContentRect=stRect(0,0,0,0);

	m_vectorItems.clear();
	
}

//�����Լ��Ĵ�С,ʹ�����е�items��������ʾ�ڿͻ�����
void CUIListBase::ComfortSelf()
{

	//�������
	if(IsSizableX())
	{
		int appendx=m_ContentRect.width-m_ClientRect.width;
		m_ClientRect.width+=appendx;
		m_WinRect.width+=appendx;
	}


	//�����߶�
	if(IsSizableY())
	{
		int appendy=m_ContentRect.height-m_ClientRect.height;
		
		//����С����С�߶�
		if((m_WinRect.height+appendy)<m_MinHeight)
			appendy=m_MinHeight-m_WinRect.height;

		m_ClientRect.height+=appendy;
		m_WinRect.height+=appendy;
	}

	SetUpdateDirty(true);

}

void CUIListBase::OffsetContentH(int offset)
{
	
	//listbase֮bug�޸�
	/*
	int pos=m_CurScrollPos.x+offset;
	
	ScrollContentH(pos);
	*/

	if(offset<(m_ClientRect.width-m_ContentRect.width-m_ContentRect.ox)) 
		offset=(m_ClientRect.width-m_ContentRect.width-m_ContentRect.ox);

	if(offset>-m_ContentRect.ox) 	offset=-m_ContentRect.ox;

		
	for(int i=0;i<m_vectorItems.size();i++)
	{		
		m_vectorItems[i].ItemRect.ox+=offset;
	}

	m_ContentRect.ox+=offset;//��ס��ǰƫ��

	m_CurPage.x=ceil(abs(m_ContentRect.ox)/(m_ClientRect.width*1.0));

	CUIScrollBar* pScrollBar=(CUIScrollBar*)GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY);
	if(pScrollBar) pScrollBar->SetScrollPos(-m_ContentRect.ox);

	
}


void CUIListBase::OffsetContentV(int offset)
{

	if(offset<(m_ClientRect.height-m_ContentRect.height-m_ContentRect.oy)) 
		offset=(m_ClientRect.height-m_ContentRect.height-m_ContentRect.oy);
	
	if(offset>-m_ContentRect.oy) 	offset=-m_ContentRect.oy;
	
	
	for(int i=0;i<m_vectorItems.size();i++)
	{		
		m_vectorItems[i].ItemRect.oy+=offset;//����ƫ��
	}
	
	m_ContentRect.oy+=offset;//��ס��ǰƫ��
	
	m_CurPage.y=ceil(abs(m_ContentRect.oy)/(m_ClientRect.height*1.0));
	
	CUIScrollBar* pScrollBar=(CUIScrollBar*)GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
	if(pScrollBar) pScrollBar->SetScrollPos(m_ContentRect.oy);

	
}

//posΪ�����ԭʼλ�õ�ƫ��,ֻ���Ǹ�ֵ
void CUIListBase::ScrollContentH(int pos)
{

	OffsetContentH(pos-m_ContentRect.ox);


}



//posΪ�����ԭʼλ�õ�ƫ��,ֻ���Ǹ�ֵ
void CUIListBase::ScrollContentV(int pos)
{
	
	OffsetContentV(pos-m_ContentRect.oy);
	
}


//x,yΪ��������
stDragInfo CUIListBase::ParseDragInfo(int x,int y)
{
	stDragInfo infoDrag;

	if(!IsInItemDragRect(x,y)) return infoDrag;//���ڵ����϶�����,����Ҫ�����Ϸ�

	if(m_DragType==UI_DRAG_TYPE_NONE) 	return infoDrag;
	
	int index=FindOnPoint(x,y);

	if(!IsValidIndex(index)) return infoDrag;
	
	if(m_IndexOffset>0) index+=m_IndexOffset;

	stValidItem validItem;
	validItem.type=m_DragType;
	validItem.index=index;
	validItem.hosttype=m_HostType;
	validItem.hostindex=m_HostIndex;

	ExecCallBackFunc(UI_Callback_IsValidItem,(void*)&validItem);		
	if(!validItem.candrag)	return infoDrag;

	infoDrag.type=m_DragType;
	infoDrag.index=index;
	infoDrag.rect=GetAbsItemRect(index);
	infoDrag.itemtype=validItem.itemtype;
	infoDrag.maxnum=validItem.nitem;
	infoDrag.bAttach=validItem.canattach;
	infoDrag.bAjust=validItem.canajust;
	infoDrag.pDragSrc=this;


	
	return infoDrag;

}

int  CUIListBase::ParseDragIndex(int x,int y,int style)
{

	if(!IsInItemDragRect(x,y))
		return -1;

	int index=-1;
	
	if(style==UI_PARSE_EXACT)
	{
		index=FindOnPoint(x,y);
	}
	else if(style==UI_PARSE_NEAR)
	{		
		index=FindNearPoint(x,y);
	}
	else if(style==UI_PARSE_APPROX)
	{		
		index=FindApproxPoint(x,y);		
	}

	if(!IsValidIndex(index)) return -1;

	if(m_IndexOffset>0) index+=m_IndexOffset;

	return index;

}

bool CUIListBase::ItemDrop(int x,int y)
{

	int index=ParseDragIndex(x,y,UI_PARSE_APPROX);
	
	stMoveItem data;

	stDragInfo info=g_pRootWgt->GetDragInfo();

	data.srcindex=info.index;
	data.srctype=info.type;
	data.srchostindex=info.hostindex;
	data.srchosttype=info.hosttype;

	data.itemnum=info.itemnum;

	data.dstindex=index;
	data.dsttype=m_DragType;	
	data.dsthostindex=m_HostIndex;
	data.dsthosttype=m_HostType;

	data.rect=info.rect;
	data.rect.ox=CUICursor::m_Pos.x-data.rect.width/2;
	data.rect.oy=CUICursor::m_Pos.y-data.rect.height/2;
	
	//��û���϶��뿪ԭ��λ�õ�ʱ��,���϶�
	if(!g_UICoreStyle.bOriginMove)
	{
		if(data.srctype==data.dsttype&&data.srcindex==data.dstindex) return false;
	}

	ExecCallBackFunc(UI_Callback_MoveItem,(void*)&data);

	
	return true;//���ܷ��µ���Ŀ
	
}

//�����϶��еĵ���
//�Ȼ��Ʊ����ͱ߿�
//Ȼ�������ͨ״̬��ѡ��״̬������״̬�ı���ͼ
//�������ı�����ͨ�ı�����richtext�ı���
int CUIListBase::DrawDragItem(stDrawItem data)
{
		
	int x=data.rect.ox;
	int y=data.rect.oy;
	int w=data.rect.width;
	int h=data.rect.height;

	ExecCallBackFunc(UI_Callback_DrawItem,(void*)&data);

	return 0;

}


void  CUIListBase::DrawItem(int idx)
{
	
	//���⿽��
	stListItem* pItem= &m_vectorItems[idx];

	if(IsItemVisible(*pItem)==RECT_NO_INTERSECT) return;
	
	int x=pItem->ItemRect.ox+m_ClientRect.ox+m_WinRect.ox;
	int y=pItem->ItemRect.oy+m_ClientRect.oy+m_WinRect.oy;
	int w=pItem->ItemRect.width;
	int h=pItem->ItemRect.height;	
	
	stDrawItem drawitem;
	drawitem.type=m_DragType;	
	drawitem.index=idx;
	drawitem.hosttype=m_HostType;
	drawitem.hostindex=m_HostIndex;
	drawitem.rect=stRect(x,y,w,h);
	drawitem.state=pItem->ItemState;

	if(m_IndexOffset>0) drawitem.index+=m_IndexOffset;
	
	//���Ʊ����ͱ߿�//////////////////////////////////////////////////////////////////////////
	
	//�����б���ɫ����
	if(m_ExtendStyle&LISTBOX_STYLE_COLORLIST)
	{
		g_pDisplay->DrawBar( x,y,w,h, pItem->ItemColor.c );	
	}
	
	//��ͨ״̬
	g_pDisplay->DrawBar( x,y,w,h, m_ItemBkColor.c );
	g_pDisplay->DrawBox( x,y,w,h, m_ItemEdgeColor.c );
	drawitem.color=m_ItemDrawColor.c;

	//sel״̬
	if(drawitem.state&UI_ITEM_STATE_SELECTED) 
	{
		g_pDisplay->DrawBar( x,y,w,h, m_ItemSelBkColor.c );
		g_pDisplay->DrawBox( x,y,w,h, m_ItemSelEdgeColor.c );
		drawitem.color=m_ItemSelColor.c;
	}

	//hot״̬
	if(drawitem.state&UI_ITEM_STATE_FOCUSED) 
	{
		g_pDisplay->DrawBar( x,y,w,h, m_ItemFocusBkColor.c );
		g_pDisplay->DrawBox( x,y,w,h, m_ItemFocusEdgeColor.c );
		drawitem.color=m_ItemFocusColor.c;
	}	
	
	//������ͨ״̬ͼƬ
	if(m_pItemDrawBack)
	{
		int bw=m_pItemDrawBack->GetWidth();
		int bh=m_pItemDrawBack->GetHeight();

		int bx=x+m_ItemDrawPos.ox;
		int by=y+m_ItemDrawPos.oy;		

		//ˮƽ����
		if(m_ItemDrawPos.width<0)
			bx=x+(w-bw)*0.5;

		//��ֱ����
		if(m_ItemDrawPos.height<0)
			by=y+(h-bh)*0.5;

		m_pItemDrawBack->Draw(bx,by);			
		
	}

	//����ѡ��״̬ͼƬ
	if(m_pItemSelBack)
	{
		int bw=m_pItemSelBack->GetWidth();
		int bh=m_pItemSelBack->GetHeight();

		int bx=x+m_ItemSelPos.ox;
		int by=y+m_ItemSelPos.oy;		

		//ˮƽ����
		if(m_ItemSelPos.width<0)
			bx=x+(w-bw)*0.5;

		//��ֱ����
		if(m_ItemSelPos.height<0)
			by=y+(h-bh)*0.5;

		if(m_pItemSelBack->GetNumFrame()>1)
		{
			if(drawitem.state&UI_ITEM_STATE_SELECTED) 
				m_pItemSelBack->Draw(bx,by,1);
			else 
				m_pItemSelBack->Draw(bx,by,0);	
			
		}	
		else
		{		 
			if(drawitem.state&UI_ITEM_STATE_SELECTED) 
				m_pItemSelBack->Draw(bx,by);			
		}		
		
	}

	//���Ƹ���״̬ͼƬ
	if(m_pItemFocusBack)
	{
		int bw=m_pItemFocusBack->GetWidth();
		int bh=m_pItemFocusBack->GetHeight();

		int bx=x+m_ItemFocusPos.ox;
		int by=y+m_ItemFocusPos.oy;		

		//ˮƽ����
		if(m_ItemFocusPos.width<0)
			bx=x+(w-bw)*0.5;

		//��ֱ����
		if(m_ItemFocusPos.height<0)
			by=y+(h-bh)*0.5;

		if(m_pItemFocusBack->GetNumFrame()>1)
		{
			if(drawitem.state&UI_ITEM_STATE_FOCUSED) 
				m_pItemFocusBack->Draw(bx,by,1);
			else 
				m_pItemFocusBack->Draw(bx,by,0);	
			
		}	
		else
		{		 
			if(drawitem.state&UI_ITEM_STATE_FOCUSED) 
				m_pItemFocusBack->Draw(bx,by);			
		}		
		
	}


	//����ͼ���б���
	if(m_pIconList)
	{
		m_pIconList->Draw(x,y,pItem->ItemIcon);
	}


	//���ƿͻ�������
	ExecCallBackFunc(UI_Callback_DrawItem,(void*)&drawitem);

	
	//�����ı�
	if(m_ExtendStyle&LISTBOX_STYLE_RICHTEXT)
	{
		pItem->richText.DrawRichText(stRect(x,y,w,h),m_TextSize,m_RowSpace,drawitem.color,m_MagicColor.c,m_LinkColor.c,m_ItemTextStyle,m_TextFont,drawitem.state);				
	}
	else 
	{
		g_pDisplay->DrawTextEx(x,y,w,h,pItem->ItemText.c_str(),m_TextSize,m_RowSpace,drawitem.color,m_ItemTextStyle); 
	}
	

#ifdef _UIEDITMODE

	if(g_pRootWgt->GetEditMode()==UI_MODE_DEBUG)
		g_pDisplay->DrawBox(drawitem.rect.ox,drawitem.rect.oy,drawitem.rect.width,drawitem.rect.height,drawitem.color);

#endif

	
}



//x,yΪ��Կͻ���ԭ������
int CUIListBase::IsPointInItemRect(int x,int y,int nIndex)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return -1;//error
	
	stRect itemRect=m_vectorItems[nIndex].ItemRect;
	
	if( x>=itemRect.ox && x<=(itemRect.ox+itemRect.width) 
	  &&y>=itemRect.oy && y<=(itemRect.oy+itemRect.height) ) 
			return 1;//in 
		
		return 0;//out	
		
}

//x,yΪ��Կͻ�������
int CUIListBase::DisToItemCenter(int x,int y,int i)
{
	int dis=UI_MAX_INTEGER;

	if(IsValidIndex(i))
	{
		stRect rect=m_vectorItems[i].ItemRect;
		
		int cx=rect.ox+rect.width*0.5;
		int cy=rect.oy+rect.height*0.5;

		int dx=cx-x;
		int dy=cy-y;

		dis=sqrt((float)(dx*dx+dy*dy));
			
	}

	return dis;
}

//x,yΪ��������
//������ڿͻ�����
//���ر����е�item
int CUIListBase::FindOnPoint(int x,int y)
{
	
	//���ڿͻ����У��϶��Ҳ���
	if(!IsPointInClientRect(x,y)) return -1;
	
	//ת��Ϊ�������
	int px=x-(m_WinRect.ox+m_ClientRect.ox);
	int py=y-(m_WinRect.oy+m_ClientRect.oy);
	
	for(int i=0;i<m_vectorItems.size();i++)
		if(IsPointInItemRect(px,py,i)) return i;
		
	return -1;
		
		
}

//x,yΪ��������
//������ڿͻ�����
//���ؾ��������item
int CUIListBase::FindNearPoint(int x,int y)
{
	
	//���ڴ������У��϶��Ҳ���
	if(!IsPointInClientRect(x,y)) return -1;
	
	//ת��Ϊ��Կͻ�������
	int px=x-(m_WinRect.ox+m_ClientRect.ox);
	int py=y-(m_WinRect.oy+m_ClientRect.oy);


	//����ÿ��itemrect���ĵ��õ�ľ���
	int minDis=UI_MAX_INTEGER;
	int nearIndex=-1;
	int curDis;
	for(int i=0;i<m_vectorItems.size();i++)
	{
		curDis=DisToItemCenter(px,py,i);

		if(curDis<minDis) 
		{
			minDis=curDis;
			nearIndex=i;
		}

	}
		
	return nearIndex;
		
		
}

//x,yΪ��������
//������ڴ�������
//���ؾ��������item
int CUIListBase::FindApproxPoint(int x,int y)
{
	
	//���ڴ������У��϶��Ҳ���
	if(!IsPointInWinRect(x,y)) return -1;
	
	//ת��Ϊ��Կͻ�������
	int px=x-(m_WinRect.ox+m_ClientRect.ox);
	int py=y-(m_WinRect.oy+m_ClientRect.oy);


	//����ÿ��itemrect���ĵ��õ�ľ���
	int minDis=UI_MAX_INTEGER;
	int nearIndex=-1;
	int curDis;
	for(int i=0;i<m_vectorItems.size();i++)
	{
		curDis=DisToItemCenter(px,py,i);

		if(curDis<minDis) 
		{
			minDis=curDis;
			nearIndex=i;
		}

	}

	if(nearIndex<0)
		nearIndex=m_DragIndex;
		
	return nearIndex;		
		
}



stColorQuad CUIListBase::GetSelColor()
{
	int nIndex=GetCurSel();
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return stColorQuad(0,0,0);//error
	
	return m_vectorItems[nIndex].ItemColor;
	
}

int CUIListBase::GetSelCol()
{
	int nIndex=GetCurSel();
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return -1;//error
	
	return m_vectorItems[nIndex].ItemCol;
	
}


int CUIListBase::GetSelRow()
{
	int nIndex=GetCurSel();
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return -1;//error
	
	return m_vectorItems[nIndex].ItemRow;
	
}

const char* CUIListBase::GetSelText()
{
	int nIndex=GetCurSel();
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return ("");//error
	
	return m_vectorItems[nIndex].ItemText.c_str();

}

const char* CUIListBase::GetSelTip()
{
	int nIndex=GetCurSel();
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return ("");//error
	
	return m_vectorItems[nIndex].ItemTip.c_str();
	
}

int CUIListBase::GetItemData(int nIndex)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return 0;//error
	
	return m_vectorItems[nIndex].ItemData;
}


int CUIListBase::GetItemIcon(int nIndex)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return 0;//error
	
	return m_vectorItems[nIndex].ItemIcon;
}

const char* CUIListBase::GetItemTip(int nIndex)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return ("");//error
	
	return m_vectorItems[nIndex].ItemTip.c_str();
}

int CUIListBase::GetItemRow(int nIndex)
{
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return 0;//error
	
	return m_vectorItems[nIndex].ItemRow;
}

int CUIListBase::GetItemCol(int nIndex)
{

	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return 0;//error
	
	return m_vectorItems[nIndex].ItemCol;
}

const char* CUIListBase::GetItemText(int nIndex)
{

	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return ("");//error

	return m_vectorItems[nIndex].ItemText.c_str();

}

stColorQuad CUIListBase::GetItemColor(int nIndex)
{

	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return stColorQuad(0,0,0);//error
	
	return m_vectorItems[nIndex].ItemColor;
	

}


void CUIListBase::SetItemColor(int nIndex,stColorQuad color)
{
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemColor=color;
	
	
}


void CUIListBase::SetItemRow(int nIndex,int nRow)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemRow=nRow;
	
}

void CUIListBase::SetItemCol(int nIndex,int nCol)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemCol=nCol;
	
}

void CUIListBase::SetItemIcon(int nIndex,int nIcon)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemIcon=nIcon;
	
}

void CUIListBase::SetItemData(int nIndex,int nData)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemData=nData;
	
}

void CUIListBase::SetItemTip(int nIndex,const char* tip)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemTip=tip;
	
}

void CUIListBase::SetItemText(int nIndex,const char* text)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemText=text;

	if(m_ExtendStyle&LISTBOX_STYLE_RICHTEXT)
	{
		m_vectorItems[nIndex].richText.ParseTxt(text,m_RichMode,m_RichFlag);
	}

	
}

stRect CUIListBase::GetAbsItemRect(int nIndex)
{

	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return stRect(-1,-1,-1,-1);//error
	
	stRect rect;

	rect.ox=m_vectorItems[nIndex].ItemRect.ox+m_ClientRect.ox+m_WinRect.ox;
	rect.oy=m_vectorItems[nIndex].ItemRect.oy+m_ClientRect.oy+m_WinRect.oy;

	rect.width=m_vectorItems[nIndex].ItemRect.width;
	rect.height=m_vectorItems[nIndex].ItemRect.height;

	return (rect);

}

void CUIListBase::SetItemRect(int nIndex,stRect rect)
{
	
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return;//error
	
	m_vectorItems[nIndex].ItemRect=rect;
	
	
}


stRect CUIListBase::GetItemRect(int nIndex)
{
	if( nIndex<0 || nIndex>=m_vectorItems.size() )
		return stRect(-1,-1,-1,-1);//error
	
	return (m_vectorItems[nIndex].ItemRect);
	
}

//////////////////////////////////////////////////////////////////////////


void CUIListBase::SetItemDrawBack( const char *filename )
{
	CUIImage *psurface = 0;
	if( filename) psurface = new CUIImage(filename);
	SetItemDrawBack( psurface );
	if( psurface ) psurface->Release();
}


void CUIListBase::SetItemDrawBack( CUIImage *psurface )
{
	if( psurface) psurface->IncRefCount();
	if( m_pItemDrawBack ) m_pItemDrawBack->Release();
	m_pItemDrawBack = psurface;
}


void CUIListBase::SetItemSelBack( const char *filename )
{
	CUIImage *psurface = 0;
	if( filename) psurface = new CUIImage(filename);
	SetItemSelBack( psurface );
	if( psurface ) psurface->Release();
}


void CUIListBase::SetItemSelBack( CUIImage *psurface )
{
	if( psurface) psurface->IncRefCount();
	if( m_pItemSelBack ) m_pItemSelBack->Release();
	m_pItemSelBack = psurface;
}


void CUIListBase::SetItemFocusBack( const char *filename )
{
	CUIImage *psurface = 0;
	if( filename) psurface = new CUIImage(filename);
	SetItemFocusBack( psurface );
	if( psurface ) psurface->Release();
}


void CUIListBase::SetItemFocusBack( CUIImage *psurface )
{
	if( psurface) psurface->IncRefCount();
	if( m_pItemFocusBack ) m_pItemFocusBack->Release();
	m_pItemFocusBack = psurface;
}


//////////////////////////////////////////////////////////////////////////


void CUIListBase::SetIconList( const char *filename )
{
	CUIImage *psurface = 0;
	if( filename) psurface = new CUIImage(filename);
	SetIconList( psurface );
	if( psurface ) psurface->Release();
}


void CUIListBase::SetIconList( CUIImage *psurface )
{
	if( psurface) psurface->IncRefCount();
	if( m_pIconList ) m_pIconList->Release();
	m_pIconList = psurface;
}



//////////////////////////////////////////////////////////////////////////
/////////////////////////////CUIPicker/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CUIPicker::CUIPicker()
{
	m_Type = WGTTYPE_PICKER;

	m_pPosCursor= NULL;	

}

CUIPicker::~CUIPicker()
{
	SAFE_RELEASE(m_pPosCursor);	
}

void CUIPicker::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_ValueH=1.0;
	m_ValueV=0.0;

	m_ResultValue=0.0;	

	SAFE_RELEASE(m_pPosCursor);	
}


void CUIPicker::SetCursorPic( const char *path )
{
	SAFE_RELEASE( m_pPosCursor );
	
	m_pPosCursor= new CUIImage(path);
}

//�õ�����ڿͻ���������
Point2D CUIPicker::GetPickerPos()
{
	Point2D point(0,0);

	point.x = m_ValueH*m_ClientRect.width/1.0f+m_ClientRect.ox;
	point.y = m_ValueV*m_ClientRect.height/1.0f+m_ClientRect.oy;

	return point;

}

//x,y�����Կͻ�����λ��
void CUIPicker::SetPickerPos(int x,int y)
{
	
	//ת��Ϊ����ֵ
	m_ValueH=(x*1.0f/m_ClientRect.width);
	m_ValueV=(y*1.0f/m_ClientRect.height);
	
	ParseResultValue();

}

void CUIPicker::ParseResultValue()
{

	Point2D point=GetPickerPos();

	//wx,wy�����Դ�������λ��
	int wx=point.x+m_ClientRect.ox;
	int wy=point.y+m_ClientRect.oy;
	
	//wx,wy�����Ա���ͼƬԭ���λ��
	int rx=wx-m_BackPicPos.x;
	int ry=wy-m_BackPicPos.y;
	
	
	//ת������㴦�ĵ�ǰ֡��ͼƬ��ɫΪֵ
	if(m_pBackPic)
	{
		stColorQuad color=m_pBackPic->GetPointColor(rx,ry,m_CurFrame);
		SetResultValue(color.c);
	}

}

void CUIPicker::GetVar(stUIVar& uiVar)
{
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_VALUE:
		uiVar.fValue = m_ResultValue;
		break;

	case VALUE_CHANNEL_VALUE_H:
		uiVar.fValue =	m_ValueH;
		break;

	case VALUE_CHANNEL_VALUE_V:
		uiVar.fValue =	m_ValueV;
		break;

	default:
		CUIWidget::GetVar(uiVar);

	}
	
	
}

void CUIPicker::SetVar(stUIVar& uiVar)
{
	
	switch(uiVar.iChannel)
	{
		
	case VALUE_CHANNEL_VALUE:
		m_ResultValue=uiVar.fValue;	
		break;

	case VALUE_CHANNEL_VALUE_H:
		SetValueH(uiVar.fValue);
		break;

	case VALUE_CHANNEL_VALUE_V:
		SetValueV(uiVar.fValue);
		break;

	default: 
		CUIWidget::SetVar(uiVar);

	}
	
}


void CUIPicker::SetResultValue(double value)
{
	if(m_ResultValue!=value)
	{
		m_ResultValue=value;
		CallSafeScriptEx(SCRIPT_FUNC_ON_VALUE_CHANGE,"%10.4f",m_ResultValue);
	}
}


void CUIPicker::SetValueH(double valueH)
{
	if(m_ValueH<=1.0f&&m_ValueH>=0.0f)
	{
		m_ValueH=valueH;
		ParseResultValue();
	}
}

void CUIPicker::SetValueV(double valueV)
{
	if(m_ValueV<=1.0f&&m_ValueV>=0.0f)
	{
		m_ValueV=valueV;
		ParseResultValue();
	}
}


void CUIPicker::SetParameter( CVar *pvar, CUIWidget *pfather )
{
	CUIWidget::SetParameter( pvar, pfather );


	if(!pvar) return;
	CVar *pelement=NULL;

	if( (pelement=pvar->Element("cursorpic")) ) 
		SetCursorPic( pelement->String() );


}


int CUIPicker::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	
	switch( msgtype )
	{
		
	case MSGTYPE_LEFTBTNDOWN:			
		{

			m_State |= WGTSTATE_CLICKDOWN;

			//���λ��
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);

			//�����Կͻ�����λ��
			int rx=x-(m_WinRect.ox+m_ClientRect.ox);
			int ry=y-(m_WinRect.oy+m_ClientRect.oy);
			
			SetPickerPos(rx,ry);
			
		}
		
			
		CallSafeScript(SCRIPT_FUNC_ON_CLICK);
				
		break;
		
	case MSGTYPE_LEFTBTNUP:
		if( m_State & WGTSTATE_CLICKDOWN )
		{	
			m_State &= ~WGTSTATE_CLICKDOWN;	
		}
		break;


	case MSGTYPE_MOUSEMOVE:
		{
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);
			
			if( m_State & WGTSTATE_CLICKDOWN )
			{	
				
				//�����Կͻ�����λ��
				int rx=x-(m_WinRect.ox+m_ClientRect.ox);
				int ry=y-(m_WinRect.oy+m_ClientRect.oy);
			
				SetPickerPos(rx,ry);

				//�ƶ���ʱ����ʱparsetooltip
				if(m_IdleTime<0.0f)
				{
					int nIndex=ParseDragIndex(x,y,UI_PARSE_EXACT);
					ParseTooltip(nIndex,GetWinRect(),x,y);
				}

			}
		}
		break;	
		
		
	default:
		return CUIWidget::OnMessage( msgtype, wparam, lparam );
		
	}	
	
	
	return 1;
}

void CUIPicker::PostRenderSelf()
{

	//���Ƹ���ͼƬ
	CUIWidget::PostRenderSelf();
	
	//���ƹ��ͼƬ
	if(m_pPosCursor)
	{


		stRect absRc=GetAbsoluteClientRect();

		g_pDisplay->PushClipRect(absRc.ox,absRc.oy,absRc.width,absRc.height);

		Point2D point=GetPickerPos();
		
		int cx=point.x+m_WinRect.ox-0.5*m_pPosCursor->GetWidth();
		int cy=point.y+m_WinRect.oy-0.5*m_pPosCursor->GetHeight();

		m_pPosCursor->Draw(cx,cy);
		
		g_pDisplay->PopClipState();

	}
	
}




//////////////////////////////////////////////////////////////////////////
/////////////////////////////CUIComboBox////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CUIComboBox::CUIComboBox()
{
	m_Type=WGTTYPE_COMBOBOX;
}

CUIComboBox::~CUIComboBox()
{

}

void CUIComboBox::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_pDropWin=0;
	m_pToggleWin=0;
}

void CUIComboBox::AddChild( CUIWidget *pwidget )
{
	
	CUIWidget::AddChild(pwidget);
	

	if(pwidget->IsPopup()) 
		m_pDropWin=pwidget;
	else if(pwidget->GetType()==WGTTYPE_CHECK)
		m_pToggleWin=pwidget;

}

void CUIComboBox::DelChild( CUIWidget *pwidget )
{
	
	if(pwidget==m_pDropWin) 
		m_pDropWin=NULL;
	else if(pwidget==m_pToggleWin) 
		m_pToggleWin=NULL;
	
	
	CUIWidget::DelChild(pwidget);
	
}

void CUIComboBox::OnLoad()
{

	if(m_pToggleWin&&m_pDropWin)
		m_pDropWin->RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,"",m_pToggleWin);

	CUIWidget::OnLoad();
}


int  CUIComboBox::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	
	switch(msgtype)
	{
		
	case NTFTYPE_COMMAND_CHECK:		
		{
			if(m_pToggleWin==(CUIWidget*)lparam)
			if(m_pToggleWin&&m_pDropWin)
			{
				m_pDropWin->ShowWidget(m_pToggleWin->IsChecked());				
			}

		}		
		break;		

	case NTFTYPE_CHILD_SHOWHIDE:		
		{
			if(m_pDropWin==(CUIWidget*)lparam)
			if(m_pToggleWin&&m_pDropWin)
			{
				if( !(m_pDropWin->IsVisible()) && m_pToggleWin->IsChecked() )
				m_pToggleWin->SetChecked(false);				
			}

		}		
		break;		
	
	default:
		return CUIWidget::OnNotify( msgtype, wparam, lparam );
		
	}
	
	//����ָ��֪ͨ,�Լ�����,�����׷���	
	return 1;
	
	
}

