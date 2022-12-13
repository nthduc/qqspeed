//============================================================================
// 文件名：XFont.h
// 
// 说明：  文字显示（可以显示中英文）需要点阵库 asc12 hzk12
//
//
//        
// 日期：  2001/11/1
//============================================================================
#ifndef XFONT_H
#define XFONT_H

#include	"XfontTex.h"
#include	"XfontInf.h"

//============================================================================
// 类定义
//============================================================================

//文字显示类
class XFont{
	int			fontType_;

	XFontTex*	pFontTex_;				//字库的纹理缓存信息
	XFontLib	m_FontLib;				//字库
	XFontInf	m_FontInf;				//字库Information

    int			texWidth_;               // 纹理宽度
    int			texHeight_;              // 纹理高度
    FLOAT		m_fTextScale;
public:

    // 2D方式显示文字,准备工作
	// x,y是从左上角开始的坐标
	// strText是字符串指针，以0结束
	// dwColor是颜色参数
	// zoom是放大倍数
    void PrepareText2d( FLOAT x, FLOAT y, const char* strText, float zoom);

	// 3D方式显示文字,准备工作
	// x,y,z代表3维坐标系
	// strText是字符串指针，以0结束
	// r,g,b,a分别代表颜色值和alpha值， 需要注意的是比如1.0代表整形颜色值255, 0.5代表128.......
	// zoom是放大倍数
	void Prepare3DText( float x, float y, float z, char* strText, float zoom);
	
	//第一个汉字的座标：p1 左上角 p2 坐下角， p3 右上角, 共3点	//x1,y1,z1右上角 x2,y2,z2右下角
	HRESULT Draw3DText( float *p1, float *p2, float *p3,DWORD dwColor,DWORD font,char *strText);
    
	void* getTex()				{ return pFontTex_->getTex(); }
public:
    XFont();		    // 构造函数
    ~XFont();			// 析构函数
    void	Release();

    // 初始化
	//		fontType:	本字体
	//		nTexSize:	纹理大小是 nTexSize*nTexSize
	//		pFontInf:	使用的Bmp字库
    HRESULT Init( int fontType, int nTexSize, XFontInf *pFontInf);
private:
	void calcCood2d( float sx, float sy, float w, float h, float HzPitch);
};

struct TOutHz{
	struct TPoint{
		float x, y, z, tx, ty;
		void set( float ax, float ay, float az, float atx, float aty){ 
			x=ax; y=ay; z=az;
			tx=atx; ty=aty; 
		}
	};
	TPoint p[4];		//4个顶点的坐标和贴图坐标，顺时针次序
};

#endif


