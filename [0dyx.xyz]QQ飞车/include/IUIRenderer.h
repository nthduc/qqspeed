

#ifndef __IUIINTERFACE_H__
#define __IUIINTERFACE_H__

#include "Interface.h"
#include "FBGui/FBGUIDef.h"
#include <windows.h>

typedef unsigned long DWORD;
typedef unsigned int IMGID;
typedef unsigned int FONTID;
#define INVALID_IMGID	0xffffffff
#define INVALID_FONTID  0xffffffff

#define ARGB(a,r,g,b)		((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define FONT_BOLD		0x00000001
#define FONT_ITALIC		0x00000002
#define FONT_UNDERLINE	0x00000004

struct TBitImage
{
	int  w;       //ͼ����
	int  h;       //ͼ��߶�
	int  pitch;    //ÿ��ͼ����Ҫ�������ֽ���
	int  attrib;    //ͼ������, 0��ʾ555ͼ��,1��ʾ4444ͼ��.
	unsigned char data[4];  //ͼ������
};

enum FillMode
{
	FILLMODE_SCALE, //����
	FILLMODE_TILE,  //ƽ��
};

class IActor;
class IUIRenderer : public ISimpleInterface
{
public:
	enum BlendMode
	{
		APPLY_REPLACE = 0,// Color = ����Color											Alpha = ����alpha
		APPLY_DECAL, //		 Color = ����color*����Alpha + ����color*(1 - ����Alpha)	Alpha = ����alpha
		APPLY_MODULATE, //	 Color = ����Color*����Color								Alpha = ����alpha*����alpha
	};



	//Image
	virtual IMGID CreateImage(const char *path) = 0;//װ��һ��ԭʼ�ļ�����

	virtual IMGID CreateImage(int uiWidth, int uiHeight,int attrib) = 0;//����һ���ڴ�����
	virtual IMGID CreateRenderImage(int uiWidth, int uiHeight) = 0;//����һ����Ⱦ����һ��

	virtual IMGID MakeGrayImage(IMGID ImageID) = 0;//��ָ����ԭʼ��ͼ����һ���Ҷ�ͼ

	virtual void SetImageUsedRect(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth)= 0;//���õ�ǰ������軭��Χ
	virtual void SetImageColor(IMGID ImageID,DWORD color)= 0;//���û���ͼƬ�Ķ�����ɫ
	virtual void UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg) = 0;//����һ������
	virtual void UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg , RECT *rcRegion) = 0;//�ֲ�����һ������
	virtual void ReleaseImage(IMGID ImageID) = 0;//ж��һ������
	virtual void ClearImage(IMGID ImageID , DWORD dwColor) = 0 ; //clear the image and fill it with the specified color
    virtual bool HaveAlphaChannels(IMGID ImageID) = 0; //�жϸ�ͼƬ�Ƿ���alphaͨ��

	virtual int GetHeight(IMGID ImageID) = 0;//�õ�����ĸ߶�
	virtual int GetWidth(IMGID ImageID) = 0;//�õ�����Ŀ��
	virtual float GetPixelAlpha(IMGID ImageID,unsigned int uiX,unsigned int uiY) = 0;//���ĳ�����ص��alpha͸����(0.0f~1.0f)
    virtual DWORD GetPixelColor(IMGID ImageID, unsigned int uiX, unsigned int uiY) = 0 ; //��ȡĳ�����ص����ɫ�������ǣ�r,g,b,a����ʽ

	//Font
	//-------------------------------------------------------------------------------------------------------
	//!!!           ���½ӿ��ǻ���freetype��������ƽӿڣ�����:maplefan, ����:puccaliang
	//              �Ƕ����Ľӿڣ������뱾�ļ��ڵ������й�����Ľӿڻ���
	//-------------------------------------------------------------------------------------------------------
	//���������µĴ������bool bBold�����ã�ֻ�����½ӿ����պ���չ��

   //����������Ӱ������ߵ�����
	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing )=0 ;
    //��������Ӱ������ߵ�����
	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     int nShadowOffsetX,    int nShadowOffsetY, DWORD dwShadowColor) =0;
   //��������߲�����Ӱ������
	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     unsigned int nEdgeWidth, DWORD EdgeColor ) =0;
   //��������Ӱ�ʹ���ߵ�����
	virtual void CreateFont( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     int nShadowOffsetX,   int nShadowOffsetY, DWORD dwShadowColor, unsigned int nEdgeWidth,   
				             DWORD EdgeColor) =0;
	
	//���õ�ǰʹ�õ����壬Fontname�൱��Font��ID
	virtual void SetCurrentFontName(const char* pFontName)=0;
	
	//�õ���ǰ����������
	virtual const char* GetCurrentFontName()=0;
	
	//����һ���Ѿ�����������
	virtual void DestroyFont(const char* pFontName) =0;
    
	//����or�õ���ǰ������ı���ɫ
	virtual void  SetCurrentTextFrontColor(DWORD dwColor)=0;
	virtual DWORD GetCurrentTextFrontColor()=0;

	//����or�õ���ǰ����ı���ɫ
	virtual void  SetCurrentTextBackColor(DWORD dwColor) =0;
	virtual DWORD GetCurrentTextBackColor() =0;

	//����or�õ���ǰ�������Ӱ��ɫ
	virtual void  SetCurrentTextShadowColor(DWORD dwColor)=0;
	virtual DWORD GetCurrentTextShadowColor()=0;

	//����or�õ���ǰ����������ɫ
	virtual void  SetCurrentTextEdgeColor(DWORD dwColor)=0;
	virtual DWORD GetCurrentTextEdgeColor()=0;

	//����or�õ���ǰ�����alphaֵ��
	virtual void SetCurrentTextFrontAndBackAlpha(unsigned int nAlpha) = 0 ;
	virtual DWORD GetCurrentTextAlpha() = 0 ;

	//�������壬�����ֱ������Ͻǵ����ꡢ�������ִ��Լ��ü���,�������Ҫ�ü�����rcClip��ΪNULL
	virtual void DrawText(unsigned int nDestLeft, unsigned int nDestTop, const char* szText, RECT *rcClip) =0;
	virtual void DrawText( const char* pText, const RECT& rDestRect, RECT *rcClip, unsigned int vFormat ) =0;

	//����һ���ִ����õ�����ִ���������Ļ�ϵ��ܿ�ȣ������ֱ����ִ����ִ�����
	virtual int  GetTextWidth( const char* pText, int nTextCount ) =0 ;

	//�õ���ǰ������������Ļ�ϻ��Ƶĸ߶ȣ������ֶ���ͳһ�߶ȵġ�
    virtual int  GetTextHeight() =0;

    //�ڻ�������ǰ��׼����һ����Ҫ����Ⱦ״̬��������DrawTextǰ����
	virtual void BeginRenderText() = 0 ;

    //����������Ϻ󣬻ָ�ԭ������Ⱦ״̬��
	virtual void EndRenderText() = 0;
    //---------------------------------------------------------------------------------------------------------



	//�����ע�ắ��Ӧ���� BeginRender()֮ǰ����
	virtual void Reg3DImage(IMGID ImageID,int iHeight,int iWidth,float fX,float fY,float fZ)= 0;//ע����һ����Ҫ�軭��3D��ͼ

	virtual void EstimateCharacter(FONTID fontID,unsigned int uiCharacter,int& iWidth,int& iHeight) = 0;//�õ�һ�����Ͱ󶨾��ο�
	virtual void EstimateString(FONTID fontID,const char* pcString,int& iWidth,int& iHeight) = 0;//�õ��ַ������ο򣬸����Ƿ��޶����
	virtual void GetCutPoint(FONTID fontID,const char* pcString,int iWidth,int& iPos) = 0;//�õ�����λ��

	//clip
	virtual void SetClipRect( int iTop, int iLeft, int iHeight, int iWidth)= 0;//���ü��п�
	virtual void GetClipRect( int&iTop, int&iLeft, int&iHeight, int&iWidth)= 0;//�õ����п�
	virtual void EnableClipRect(bool flag)= 0; //���ؼ���
	virtual bool IsClipEnable()= 0; //�жϼ���

	//draw
	virtual void SetDrawFlags(BlendMode flags)= 0;//���û�Ϸ�ʽ
	virtual void DrawLine(int iXTop,int iXLeft,int iYTop,int iYLeft,DWORD color)= 0;//����ֱ��
	virtual void DrawCharacter(unsigned int Glyph,int iTop,int iLeft)= 0;//����һ������
	virtual void DrawString(const char* pcText,int iTop,int iLeft)= 0;//�����ַ���,�����޶���Ȼ���
	virtual void DrawRect(int iTop,int iLeft,int iHeight,int iWidth,DWORD color,bool bNeedFill)= 0;//���Ʒ���
	virtual void DrawPolygon(int* pVertex,int iNumVertices,DWORD color,bool bNeedFill)= 0;//���ƶ����


	//��ͨ�������ƽ��������ͼƬ��Ҫ����ͼƬ��������rcScr��ʾ����Ļ��Ŀ��������rcDest��ʾ��byImageAlphaSet������ʾ��ͼƬ�Ļ���ʱ����ʾ��alphaֵ
	//�ɹ�����ʱ����true��������false
	virtual bool DrawImage(IMGID ImageID, FillMode enFillMode, RECT* rcScr, RECT* rcDest, unsigned char byImageAlphaSet) =0 ;
	virtual void DrawImage(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth)= 0;//����ͼƬ

	virtual void DrawImageWithMask(IMGID ImageID, IMGID ImageIDMask,
		int iLeftDes,int iTopDes, int iWidthDes, int iHeightDes )= 0;//���ƴ��ɰ��ͼƬ

	virtual void BeginUI() = 0 ;
	virtual void EndUI() = 0 ;

	// Dirty Rectangle solution for User Interface Rendering
	virtual void CreateUIScreenBlock(int nScreenWidth, int nScreenHeight) = 0 ;
	virtual void UpdateUIScreen( RECT *pDirtyRectList, int nRectCount, TBitImage *pScreenBuffer) = 0 ;
};

IUIRenderer *GetUIRenderer();
void SetUIRenderer(IUIRenderer *pUIRenderer);

#endif //__IUIINTERFACE_H__