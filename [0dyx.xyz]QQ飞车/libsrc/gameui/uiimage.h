
#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "kernel.h"
#include "display.h"

extern void DumpImageInfo(const char* srcpath);

const int UI_IMAGE_MAX_FRAME=10000;

//有多少帧没有绘制就释放
const int UI_IMAGE_CACHE_TICK=100;

//图像载入和绘制方式
enum UIImgPrior
{
	
	UI_IMG_PRIOR_DEFAULT		=0,//默认（绘制时载入，以纹理方式绘制）
	UI_IMG_PRIOR_LOAD_INIT		=1,//（创建时载入）
	UI_IMG_PRIOR_DRAW_PIXEL		=2,//（以Blt方式绘制）

};

class CCoreUIImage;
class CUIImage : public CRefObject
{
	
public:

	CUIImage( const char *path ,int prior=UI_IMG_PRIOR_DEFAULT);

	const char* GetFilePath();

	int  GetWidth();
	int  GetHeight();
	int  GetNumFrame();

	//color querry
	//得到指定帧中某点颜色
	stColorQuad GetPointColor(int x,int y,int frame);
	

	//点是否在指定帧中
	//透明就认为不在其中
	BOOL IsPointAlphaIn(int x,int y,int frame);
	
	//点是否在图片中
	//不考虑透明情况
	BOOL IsPointIn(int x,int y);

	void Draw( int left, int top, int frame=0, DWORD dwEffect=0, DWORD dwColor=0xffffffff);

protected:

	~CUIImage();

	CCoreUIImage *m_pCoreImg;

	int   m_nImgPrior; 


};

#define _WINDOWS_CURSOR_

//cursor在任何其他对象之上，所以，没有继承CUIWidget
class CUICursor : public CUIImage
{
public:
	CUICursor( const char *name, const char *path );
	~CUICursor();

	const char *GetName(){ return m_Name.c_str(); }
	void SetHotSpot( const Point2D &pos ){ m_HotSpot = pos; }
	void SetAnimStep( float t ){ m_AnimStep = t; }
	void SetHoldFrame( int i ) { m_HoldFrame = i; }
	void SetRightFrame( int i ) { m_RightFrame = i; }
	
	float GetAnimStep(){ return m_AnimStep; }
	Point2D GetHotSpot(){ return m_HotSpot; };

	void ShowCursor( bool b ){ m_bShow = b; }
	void Update( float deltatime );
	void Render();

	void SetActive();
	static Point2D m_Pos;
	static bool    m_bShow;

private:
	std::string m_Name;
	float m_Life;
	float m_AnimStep;
	Point2D m_HotSpot;

	int m_HoldFrame;  //按住鼠标左键后的起始桢, 0表示按住和没有按住无差别
	int m_RightFrame; //按住鼠标右键后的起始桢, 0表示按住右键和左键无差别

#ifdef	_WINDOWS_CURSOR_
	HCURSOR	m_hCursor;
	HCURSOR	m_hCursorL;
	HCURSOR	m_hCursorR;
#endif
};

extern void UpdateUIImage();

#endif //__UIIMAGE_H__