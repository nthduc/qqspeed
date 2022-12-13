
#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "kernel.h"
#include "display.h"

extern void DumpImageInfo(const char* srcpath);

const int UI_IMAGE_MAX_FRAME=10000;

//�ж���֡û�л��ƾ��ͷ�
const int UI_IMAGE_CACHE_TICK=100;

//ͼ������ͻ��Ʒ�ʽ
enum UIImgPrior
{
	
	UI_IMG_PRIOR_DEFAULT		=0,//Ĭ�ϣ�����ʱ���룬������ʽ���ƣ�
	UI_IMG_PRIOR_LOAD_INIT		=1,//������ʱ���룩
	UI_IMG_PRIOR_DRAW_PIXEL		=2,//����Blt��ʽ���ƣ�

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
	//�õ�ָ��֡��ĳ����ɫ
	stColorQuad GetPointColor(int x,int y,int frame);
	

	//���Ƿ���ָ��֡��
	//͸������Ϊ��������
	BOOL IsPointAlphaIn(int x,int y,int frame);
	
	//���Ƿ���ͼƬ��
	//������͸�����
	BOOL IsPointIn(int x,int y);

	void Draw( int left, int top, int frame=0, DWORD dwEffect=0, DWORD dwColor=0xffffffff);

protected:

	~CUIImage();

	CCoreUIImage *m_pCoreImg;

	int   m_nImgPrior; 


};

#define _WINDOWS_CURSOR_

//cursor���κ���������֮�ϣ����ԣ�û�м̳�CUIWidget
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

	int m_HoldFrame;  //��ס�����������ʼ��, 0��ʾ��ס��û�а�ס�޲��
	int m_RightFrame; //��ס����Ҽ������ʼ��, 0��ʾ��ס�Ҽ�������޲��

#ifdef	_WINDOWS_CURSOR_
	HCURSOR	m_hCursor;
	HCURSOR	m_hCursorL;
	HCURSOR	m_hCursorR;
#endif
};

extern void UpdateUIImage();

#endif //__UIIMAGE_H__