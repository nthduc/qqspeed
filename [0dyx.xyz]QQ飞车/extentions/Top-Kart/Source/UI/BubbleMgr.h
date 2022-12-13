#pragma once

#include <vector>
#include "kernel.h"
#include "ImageEx.h"
#include "framework/gui_i.h"
using namespace std;

class CUIRichText;
class CUIWidget;
typedef struct tagBubbleItem
{
	UINT			ID;
	CUIRichText*	pRichText;
	INT				TextSize;
	INT				Space;
	INT				Color;
	Point2D			Pos;
	UINT			Life;
	UINT			Age;
	INT				Width;
	INT				Height;
	INT				Margin;
}BubbleItem;

typedef vector<BubbleItem>	BubbleContainer;

class CBubbleMgr : public IBubbleMgr
{
public:
	CBubbleMgr(void);
	~CBubbleMgr(void);

	BOOL AddBubble(UINT id, LPCSTR text, Point2D pos, INT life, INT textsize, INT space, INT color, INT width, INT height, INT margin);
	void Clear();

	void Update(int deltatick);
	void Render();

	void DrawBubbleBk(int x, int y, int width, int height);

private:
	BubbleContainer	m_BubbleContainer;
	CUIWidget*		m_pBubbleBgWidget;
	CImageEx*		m_pArrowImage;

};
