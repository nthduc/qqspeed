#include "Top-KartPCH.h"
#include "bubblemgr.h"
#include "../libsrc/gameui/uicontrol.h"
#include "uiprotocol.h"
#include "txtCtrlr.h"
#include "fileop.h"

CBubbleMgr::CBubbleMgr(void):
m_pBubbleBgWidget(NULL),
m_pArrowImage(NULL)
{
	m_BubbleContainer.resize( 32 );
	for( int i=0; i<m_BubbleContainer.size(); i++ )
	{
		m_BubbleContainer[i].ID = 0;
		m_BubbleContainer[i].pRichText = new CUIRichText;
	}

	m_pBubbleBgWidget = new CUIWidget;
	m_pBubbleBgWidget->ClearParameter();

	char file[MAX_PATH] = "\0";
	_fullpath(file, "chatbb.txt", MAX_PATH);
	CtxtCtrlr txtCtrlr;
	if ( !txtCtrlr.Open(file) )
		return;

	CtxtRecordset* pRecordset = NULL;
/*	if ( !txtCtrlr.Next() )
		return;
	CtxtRecordset* pRecordset = txtCtrlr.GetRecordset();
	m_pArrowImage = new CImageEx;
	m_pArrowImage->Load(pRecordset->GetField(0)); //第一个条记录是箭头
*/
	int index = 0;	//剩下的是PatchPic
	while(index < PATCHID_NUMBER)
	{
		if ( !txtCtrlr.Next() )
			break;
		pRecordset = txtCtrlr.GetRecordset();
		if ( pRecordset->GetFieldCount() < 1 )
			continue;
		m_pBubbleBgWidget->SetPatchPic(pRecordset->GetField(0), index++);
	}
}

CBubbleMgr::~CBubbleMgr(void)
{
	for( int i=0; i<m_BubbleContainer.size(); i++ )
	{
		delete m_BubbleContainer[i].pRichText;
	}
	if ( m_pBubbleBgWidget ) delete m_pBubbleBgWidget;
	if ( m_pArrowImage ) delete m_pArrowImage;
}

BOOL CBubbleMgr::AddBubble(UINT id, LPCSTR text, Point2D pos, INT life, INT textsize, INT space, INT color, INT width, INT height, INT margin)
{
	
	int i;
	for( i=0; i<m_BubbleContainer.size(); i++ )
	{
		if( m_BubbleContainer[i].ID == id ) break;
	}
	if( i == m_BubbleContainer.size() )
	{
		for( i=0; i<m_BubbleContainer.size(); i++ )
		{
			if( m_BubbleContainer[i].ID == 0 ) break;
		}
		if( i == m_BubbleContainer.size() ) return FALSE;
	}

	BubbleItem &bubble = m_BubbleContainer[i];
	bubble.ID = id;
	bubble.Pos = pos;
	bubble.Life = life;
	bubble.TextSize = textsize;
	bubble.Space = space;
	bubble.Color = color;
	bubble.Age = 0;
	bubble.Width = width;
	bubble.Height = height;
	bubble.Margin = margin;
	bubble.pRichText->ParseTxt(text, UI_RICHMODE_ALL, UI_RICHFLAG_ICONNICKNAME);
	
	return TRUE;
}

void CBubbleMgr::Clear()
{
	for( int i=0; i<m_BubbleContainer.size(); i++ )
	{
		m_BubbleContainer[i].ID = 0;
	}
}

void CBubbleMgr::Update(int deltatick)
{
	static int t0 = GetTickCount();
	int t1 = GetTickCount();
	int dt = t1 - t0;
	t0 = t1;
	for ( BubbleContainer::iterator it = m_BubbleContainer.begin(); it != m_BubbleContainer.end(); it++ )
	{
		BubbleItem* bubble = &(*it);
		bubble->Age += dt;
		if ( bubble->Life > 0 && bubble->Age > bubble->Life )
			bubble->ID = 0;
		m_pBubbleBgWidget->UpdateSelf(dt);
	}
}

void CBubbleMgr::Render()
{
	for( int i=0; i<m_BubbleContainer.size(); i++ )
	{
		BubbleItem &bubble = m_BubbleContainer[i];
		if( bubble.ID == 0 ) continue;

		int ox = bubble.Pos.x, oy = bubble.Pos.y, height = INT_MAX, width = (bubble.Width > 0 ? bubble.Width : 100);
		int c = bubble.pRichText->EstimateLine(width, height, bubble.TextSize, 0);
		int margin = bubble.Margin;
		width < bubble.Width ? width = bubble.Width : 0;
		height < bubble.Height ? height = bubble.Height : 0;
		DrawBubbleBk( ox - margin, oy - margin, width + 2 * margin, height + 2 * margin);
		bubble.pRichText->DrawRichText( stRect(ox, oy, width, height), bubble.TextSize, bubble.Space, bubble.Color, 0x80808080, 0xff0000ff,  DT_CENTER|DT_VCENTER, 0 );
	}
}

void CBubbleMgr::DrawBubbleBk(int x, int y, int width, int height)
{
	m_pBubbleBgWidget->SetWinRect(x, y, width, height);
	m_pBubbleBgWidget->RenderSelf();
}