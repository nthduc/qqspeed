/*
 *	Display.h
 *  Display functions header.
 *
 *  Jim, 2003.11.18.
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define _USEDLL_

#ifdef _USEDLL_
#define EXPORT_API __declspec(dllexport)
#define EXPORT_C  
#else
#define EXPORT_API __declspec(dllimport)
#define EXPORT_C
#endif //_USEDLL_


typedef unsigned long HIMG;


//��Ļ����
enum ScreenTypes
{
	screen640x480	=	0,
	screen800x600	=	1,
	screen1024x768	=	2,
};
	
	
//ͼ������
typedef	enum _ImageAttrib
{
    imageAttribBegin    =   0,
	imageAttrib1555		=	0,	// a1r5g5b5
	imageAttrib4444		=	1,	// a4r4g4b4
	imageAttrib8888		=	2,	// a8r8g8b8 add by qjb [2004-11-1] 
    imageAttribEnd      =   imageAttrib8888,
} ImageAttrib;


//ͼ��ṹ����
typedef	struct _TImage
{
	int  w;			//ͼ����
	int  h;			//ͼ��߶�
	int  pitch;		//ÿ��ͼ����Ҫ�������ֽ���
	int  attrib;    //ͼ������
	unsigned char data[4];	//ͼ������
} TImage;

//czy, added [12/16/2003]
//��Ч����
enum {
	fxTexMask		= 15,			//��ͼ����ɫ�Ĳ������ͣ� �������ٺͱ�������
	fxTexMul		= 0,			//��ͼ��ɫ * ������ɫ,  ��ͼalpha * ����alpha
	fxTexTex		= 1,			//ֻ����ͼ��ɫ
//	fxTexAdd		= 2,			//��ͼ��ɫ + ������ɫ    �ݲ�֧��

	fxBlendMask		= (15<<4),
	fxBlendAlpha	= 0<<4,         //��������ɫ�ͱ���alpha���
	fxBlendAdd		= 1<<4,         //��������ɫ�ͱ������
	fxBlendBlit		= 2<<4,			//��������ɫֱ�� blit��������
	fxBlendMax		= 3<<4,         //��ͼ��ɫ�ͱ�����ɫ,ȡ���ֵ
	
	fxGeoMirrorX	= 1<<8,			//ˮƽ����
	fxGeoMirrorY	= 1<<9,			//��ֱ����
	fxGeoScale		= 1<<10,		//���������ĵ�ͨ��״̬���ķ�ʽ�ṩ
	fxGeoRotate		= 1<<11,		//��ת����ת���ĵ�ͽǶ�ͨ��״̬���ķ�ʽ�ṩ

	//add+lincoln
	//[2005:2:16] [9:50]
    fxDrawCircle	= 1<<12,		//���ӻ���Բ����Ч�ӿ�

	fxGeo2DNoScale	= 1<<16,		//��2D�в�����,ֻ��2D��ʾ�ӿ�������
};

//add+lincoln
//[2004:11:18] [12:50]
//��������ķ�ɫ����						
#define DT_MAGICBACK    0x00100000
#define DT_MAGICEDGE    0x00200000
#define DT_MAGICLINE    0x00400000
#define DT_MAGICAXIS    0x00800000


//ԭ����displayģ��ֻ����drawchar
//���ⲿģ�飨client��ui���Լ������ı����ֵ�
enum {

	dtLeft		= DT_LEFT,		//default
	dtCenter	= DT_CENTER,
	dtRight		= DT_RIGHT,

	dtTop		= DT_TOP,		//default
	dtVCenter	= DT_VCENTER,
	dtBottom	= DT_BOTTOM,

	//add+lincoln
	//[2004:11:18] [12:50]
	//��������ķ�ɫ����
	dt_MagicBack	= DT_MAGICBACK,
	dt_MagicEdge	= DT_MAGICEDGE,
	dt_MagicLine	= DT_MAGICLINE,
	dt_MagicAxis	= DT_MAGICAXIS,

//	dtClipRect	= 0x00080000,		//ʹ�ü��о���
};

//delta add
const int MAX_TEXT_COLOR = 100; //0 is the default color


//////////////////////////////////////////////////////////////////////////////
// class CDisplay

class EXPORT_API IDisplay
{
public:
	//Status function
	virtual BOOL	IsWindowed() = 0;
	virtual WORD	GetDisplayWidth() = 0;
	virtual WORD	GetDisplayHeight() = 0;

	//Creation/destruction methods
	virtual HRESULT	CreateFullScreenDisplay(HWND hWnd, int nScreenType) = 0;
	virtual HRESULT CreateWindowedDisplay(HWND hWnd, int nScreenType) = 0;
	virtual HRESULT	UpdateBounds() = 0;
	virtual HRESULT	DestroyObjects() = 0;
	virtual HRESULT	RestoreAllSurfaces() = 0;

	//Display methods
	virtual HRESULT	ClearScreen(DWORD dwColor) = 0;
	virtual HRESULT	UpdateScreen() = 0;

	//Image utilities
public:
	virtual HIMG	SelectImage(TImage *pImage) = 0;
	virtual HIMG	GenImageHandle(int w, int h, int attrib) = 0;
	virtual void	UpdateImage(HIMG hImg, int left, int top, TImage *pImage) = 0;
//-	virtual void	DrawImage(int left, int top, HIMG hImg) = 0;
	virtual void	ReleaseImageHandle(HIMG hImg) = 0;
	
	//Jim, added [11/27/2003]
	//{{{
	virtual int		GetImageWidth(HIMG hImg) = 0;
	virtual int		GetImageHeight(HIMG hImg) = 0;
//-	virtual void	DrawImageColor(int left, int top, HIMG hImg, DWORD dwColor) = 0;
	//}}}

	//czy, added [12/16/2003]
	//image��Ч
	virtual void	DrawImage( int left, int top, HIMG hImg, DWORD effect=0, DWORD dwColor=0xffffffff) = 0;
	virtual void	SetImageCenter( float centerX, float centerY) = 0;
	virtual void	SetImageScale( float scaleX, float scaleY) = 0;
	virtual void	SetImageRotate( float degree) = 0;

	//czy, added [1/8/2004]
	//����Ļ�� Back �����ϵ�һ�����Σ�����һ���µ���ͼ
	// SnapScreen ��һ����ʱ�޷������bug: ���µߵ��ˣ�Ҫ��fxGeoMirrorY������
	virtual HIMG	SnapScreen( int left, int top, int width, int height)=0;
	virtual TImage*	SnapScreen2( int left, int top, int width, int height)=0;
	
	//czy, added [3/8/2004]
	//����: ��������ʱ����������˷�ʽ���Ƿ�ʹ�����Բ�ֵ������ֵ������ǰ��״̬
	virtual bool	EnableTexLinear( bool flag)=0;
	
	//czy, added [2/6/2004]
	//���ü��þ���.
	virtual void	SetClipRect( int left, int top, int width, int height)=0;
	virtual void	GetClipRect( int&left, int&top, int&width, int&height)=0;
	
	virtual void	EnableClipRect( bool flag)=0;	
	virtual bool	IsClipEnable()=0;
	
	//add+lincoln
	//[2005:1:18] [16:34]
	//���Ӳü�״̬��ջ	
	//{{{	
	virtual void	PushClipState(){}//���浱ǰ�ü�״̬
	virtual void	PopClipState(){}//�ָ�֮ǰ����Ĳü�״̬
	virtual void	PushClipRect(int left, int top, int width, int height){}//���浱ǰ״̬�������ü������òü�����Ϊ����ǰ�ü�rect��ָ��rect�Ľ���
	//}}}

	virtual void	SetMaxTex( int size)=0;		//512, 1024, 2048
public:
	virtual void	DrawPixel(int x, int y, DWORD dwColor) = 0;
	virtual void	DrawLine(int x1, int y1, int x2, int y2, DWORD dwColor) = 0;
	virtual void	DrawPolygon( int numPoints, const float* xy, DWORD dwColor) = 0;
	virtual void	DrawPolygon( int numPoints, const float* xy, DWORD* dwColor) = 0;
	
	virtual void	DrawBar(int left, int top, int width, int height, DWORD dwColor) = 0;
	virtual void	DrawBox(int left, int top, int width, int height, DWORD dwColor) = 0;
//	virtual void    DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect = 0,DWORD dwColor=0xffffffff)=0;
public:	
	//�����ı�
	virtual void	DrawText( int sx, int sy, const char* strText, DWORD dwColor, int size, UINT flags=0) = 0;
	//czy, added [12/16/2003]
	virtual void	DrawTextEx( int left, int top, int width, int height, 
							    const char* str, int size, int space=0, 
							    DWORD dwColor=0xffffffff, UINT flags=0) = 0;
	
	//��ʾ�����ַ����������ַ�Ϊ " ~C0", "~C1", ��"~C9"��
	virtual void	DrawCtrlText( int left, int top, int width, int height, 
								const char* str, int size, int space, UINT flags)=0;

	//���ÿ����ַ���ɫ��	colorIdxȡֵΪ0��MAX_TEXT_COLOR������ԭ����������Ӧ����ɫֵ
	virtual DWORD	SetCtrlTextColor( int colorIdx, DWORD dwColor )=0;

	//���õ�ǰʹ�õ����壺	0-���� 1-��Բ��	����ԭ��ʹ�õ���������
	virtual int		SetFont( int fontType)=0;

	//���ַ����еĿ����ַ�ȥ��
	virtual void	ClearCtrl( const char* ctrlStr, char* result)=0;
	
	//�����ַ�������ʾ�߶ȣ���λ�����أ�
	virtual int		EstimateLine( const char* str, int width, int size, int space=0) = 0;
	
	//���ݶ����ַ���str,���ַ��ߴ�size, �����ʾ��ռ���εĴ�С��
	//width��heightֵ�ֱ�Ϊ�ַ�������ȡ��ַ����߶ȣ���λ�����أ�
	//space���м��
	virtual void	EstimateLine( const char* str, int size, int space, int& width, int& height) = 0;

/*	//czy, deleted [3/15/2004]
	//�����о࣬����������ǰ���оࡣȱʡ�о�Ϊ0��
	virtual int		SetRowSpace( int) = 0;
	//���ص�ǰ���оࡣȱʡ�о�Ϊ0��
	virtual int		GetRowSpace() = 0;
*/

	//delta add[9/6/2004]
	virtual DWORD   GetCtrlTextColor( int colorIdx ) = 0;
	virtual void    DrawPixels( int ox, int oy, TImage *pimage ) = 0;

    //qjb add[12/29/2004]
    virtual HIMG    GenRotateImageHandle(TImage *pImage, int nArf, int nCenterx=0, int nCentery=0) = 0;
    virtual HIMG    GenScaleImageHandle(TImage *pImage, float fscalex, float fscaley) = 0;
    virtual bool    SaveGsoFile(HIMG hImg, char* szFileName) = 0;
};

extern EXPORT_C EXPORT_API IDisplay*    CreateDisplay3D();
extern EXPORT_C EXPORT_API void         DestroyDisplay3D( IDisplay*);

extern EXPORT_C EXPORT_API IDisplay*    CreateDisplay2D();
extern EXPORT_C EXPORT_API void         DestroyDisplay2D( IDisplay*);


//////////////////////////////////////////////////////////////////////////
// TImage utilities

extern EXPORT_C EXPORT_API TImage*  CreateImage(int w, int h, int attrib);
extern EXPORT_C EXPORT_API void     DestroyImage(TImage **pImage);
extern EXPORT_C EXPORT_API TImage*  DuplicateImage(TImage *pImage);
extern EXPORT_C EXPORT_API void     ClearImage(TImage *pImage);
extern EXPORT_C EXPORT_API void     BltImage(TImage *pDestImage, int left, int top, TImage *pSrcImage);


#endif //_DISPLAY_H_

