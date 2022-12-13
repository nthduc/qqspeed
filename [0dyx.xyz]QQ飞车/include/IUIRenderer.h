

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
	int  w;       //图像宽度
	int  h;       //图像高度
	int  pitch;    //每行图像需要的数据字节数
	int  attrib;    //图像属性, 0表示555图像,1表示4444图像.
	unsigned char data[4];  //图像数据
};

enum FillMode
{
	FILLMODE_SCALE, //拉伸
	FILLMODE_TILE,  //平铺
};

class IActor;
class IUIRenderer : public ISimpleInterface
{
public:
	enum BlendMode
	{
		APPLY_REPLACE = 0,// Color = 纹理Color											Alpha = 纹理alpha
		APPLY_DECAL, //		 Color = 纹理color*纹理Alpha + 顶点color*(1 - 纹理Alpha)	Alpha = 顶点alpha
		APPLY_MODULATE, //	 Color = 纹理Color*顶点Color								Alpha = 纹理alpha*顶点alpha
	};



	//Image
	virtual IMGID CreateImage(const char *path) = 0;//装载一个原始文件纹理

	virtual IMGID CreateImage(int uiWidth, int uiHeight,int attrib) = 0;//创建一个内存纹理
	virtual IMGID CreateRenderImage(int uiWidth, int uiHeight) = 0;//创建一个渲染纹理一次

	virtual IMGID MakeGrayImage(IMGID ImageID) = 0;//由指定的原始贴图创建一个灰度图

	virtual void SetImageUsedRect(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth)= 0;//设置当前纹理的描画范围
	virtual void SetImageColor(IMGID ImageID,DWORD color)= 0;//设置绘制图片的顶点颜色
	virtual void UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg) = 0;//更新一个纹理
	virtual void UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg , RECT *rcRegion) = 0;//局部更新一个纹理
	virtual void ReleaseImage(IMGID ImageID) = 0;//卸载一个纹理
	virtual void ClearImage(IMGID ImageID , DWORD dwColor) = 0 ; //clear the image and fill it with the specified color
    virtual bool HaveAlphaChannels(IMGID ImageID) = 0; //判断该图片是否有alpha通道

	virtual int GetHeight(IMGID ImageID) = 0;//得到纹理的高度
	virtual int GetWidth(IMGID ImageID) = 0;//得到纹理的宽度
	virtual float GetPixelAlpha(IMGID ImageID,unsigned int uiX,unsigned int uiY) = 0;//获得某个像素点的alpha透明度(0.0f~1.0f)
    virtual DWORD GetPixelColor(IMGID ImageID, unsigned int uiX, unsigned int uiY) = 0 ; //获取某个象素点的颜色，返回是（r,g,b,a）形式

	//Font
	//-------------------------------------------------------------------------------------------------------
	//!!!           以下接口是基于freetype的字体绘制接口，作者:maplefan, 整理:puccaliang
	//              是独立的接口，请慎与本文件内的其他有关字体的接口混用
	//-------------------------------------------------------------------------------------------------------
	//！！！以下的粗体参数bool bBold不可用，只是留下接口做日后扩展。

   //创建不带阴影不带描边的字体
	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing )=0 ;
    //创建带阴影不带描边的字体
	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     int nShadowOffsetX,    int nShadowOffsetY, DWORD dwShadowColor) =0;
   //创建带描边不带阴影的字体
	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     unsigned int nEdgeWidth, DWORD EdgeColor ) =0;
   //创建带阴影和带描边的字体
	virtual void CreateFont( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     int nShadowOffsetX,   int nShadowOffsetY, DWORD dwShadowColor, unsigned int nEdgeWidth,   
				             DWORD EdgeColor) =0;
	
	//设置当前使用的字体，Fontname相当于Font的ID
	virtual void SetCurrentFontName(const char* pFontName)=0;
	
	//得到当前的字体名称
	virtual const char* GetCurrentFontName()=0;
	
	//销毁一个已经创建的字体
	virtual void DestroyFont(const char* pFontName) =0;
    
	//设置or得到当前字体的文本颜色
	virtual void  SetCurrentTextFrontColor(DWORD dwColor)=0;
	virtual DWORD GetCurrentTextFrontColor()=0;

	//设置or得到当前字体的背景色
	virtual void  SetCurrentTextBackColor(DWORD dwColor) =0;
	virtual DWORD GetCurrentTextBackColor() =0;

	//设置or得到当前字体的阴影颜色
	virtual void  SetCurrentTextShadowColor(DWORD dwColor)=0;
	virtual DWORD GetCurrentTextShadowColor()=0;

	//设置or得到当前字体的描边颜色
	virtual void  SetCurrentTextEdgeColor(DWORD dwColor)=0;
	virtual DWORD GetCurrentTextEdgeColor()=0;

	//设置or得到当前字体的alpha值。
	virtual void SetCurrentTextFrontAndBackAlpha(unsigned int nAlpha) = 0 ;
	virtual DWORD GetCurrentTextAlpha() = 0 ;

	//绘制字体，参数分别是左上角的坐标、所画的字串以及裁剪框,如果不需要裁减，把rcClip设为NULL
	virtual void DrawText(unsigned int nDestLeft, unsigned int nDestTop, const char* szText, RECT *rcClip) =0;
	virtual void DrawText( const char* pText, const RECT& rDestRect, RECT *rcClip, unsigned int vFormat ) =0;

	//传入一个字串，得到这个字串将绘在屏幕上的总宽度，参数分别是字串和字串长度
	virtual int  GetTextWidth( const char* pText, int nTextCount ) =0 ;

	//得到当前这种字体在屏幕上绘制的高度，所有字都是统一高度的。
    virtual int  GetTextHeight() =0;

    //在绘制字体前，准备好一切需要的渲染状态，必须在DrawText前调用
	virtual void BeginRenderText() = 0 ;

    //绘制字体完毕后，恢复原来的渲染状态。
	virtual void EndRenderText() = 0;
    //---------------------------------------------------------------------------------------------------------



	//下面的注册函数应该在 BeginRender()之前调用
	virtual void Reg3DImage(IMGID ImageID,int iHeight,int iWidth,float fX,float fY,float fZ)= 0;//注册这一贞需要描画的3D贴图

	virtual void EstimateCharacter(FONTID fontID,unsigned int uiCharacter,int& iWidth,int& iHeight) = 0;//得到一个字型绑定矩形框
	virtual void EstimateString(FONTID fontID,const char* pcString,int& iWidth,int& iHeight) = 0;//得到字符串矩形框，根据是否限定宽度
	virtual void GetCutPoint(FONTID fontID,const char* pcString,int iWidth,int& iPos) = 0;//得到换行位置

	//clip
	virtual void SetClipRect( int iTop, int iLeft, int iHeight, int iWidth)= 0;//设置剪切框
	virtual void GetClipRect( int&iTop, int&iLeft, int&iHeight, int&iWidth)= 0;//得到剪切框
	virtual void EnableClipRect(bool flag)= 0; //开关剪切
	virtual bool IsClipEnable()= 0; //判断剪切

	//draw
	virtual void SetDrawFlags(BlendMode flags)= 0;//设置混合方式
	virtual void DrawLine(int iXTop,int iXLeft,int iYTop,int iYLeft,DWORD color)= 0;//绘制直线
	virtual void DrawCharacter(unsigned int Glyph,int iTop,int iLeft)= 0;//绘制一个字形
	virtual void DrawString(const char* pcText,int iTop,int iLeft)= 0;//绘制字符串,根据限定宽度换行
	virtual void DrawRect(int iTop,int iLeft,int iHeight,int iWidth,DWORD color,bool bNeedFill)= 0;//绘制方块
	virtual void DrawPolygon(int* pVertex,int iNumVertices,DWORD color,bool bNeedFill)= 0;//绘制多边形


	//可通过拉伸和平铺来绘制图片，要画的图片的区域用rcScr表示，屏幕的目标区域用rcDest表示，byImageAlphaSet用来表示该图片的绘制时所显示的alpha值
	//成功调用时返回true，出错返回false
	virtual bool DrawImage(IMGID ImageID, FillMode enFillMode, RECT* rcScr, RECT* rcDest, unsigned char byImageAlphaSet) =0 ;
	virtual void DrawImage(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth)= 0;//绘制图片

	virtual void DrawImageWithMask(IMGID ImageID, IMGID ImageIDMask,
		int iLeftDes,int iTopDes, int iWidthDes, int iHeightDes )= 0;//绘制带蒙板的图片

	virtual void BeginUI() = 0 ;
	virtual void EndUI() = 0 ;

	// Dirty Rectangle solution for User Interface Rendering
	virtual void CreateUIScreenBlock(int nScreenWidth, int nScreenHeight) = 0 ;
	virtual void UpdateUIScreen( RECT *pDirtyRectList, int nRectCount, TBitImage *pScreenBuffer) = 0 ;
};

IUIRenderer *GetUIRenderer();
void SetUIRenderer(IUIRenderer *pUIRenderer);

#endif //__IUIINTERFACE_H__