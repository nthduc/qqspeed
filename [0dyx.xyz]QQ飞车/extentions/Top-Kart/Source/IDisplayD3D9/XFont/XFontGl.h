//============================================================================
// 文件名：XFontD3D.h
// 
// 说明：  文字显示（可以显示中英文）需要点阵库 asc12 hzk12
//
// 编写： 修改自Dx8例子里的d3dfont.h
//
//        
// 日期：  2003/11/26	mike
//============================================================================
//关于XFontTex，
//将要输出的字符写到纹理上，返回纹理坐标以供render
//内部缓冲使用链表的方式，根据"最近最少使用”频率动态调整
//缓冲满了以后，会自动将最不常用的字符替换掉
//主要函数GetChar(), 如果不在缓存中，将使用FontLib得到字符点阵

#ifndef XFONT_GL_H
#define XFONT_GL_H

#include	"../Display.h"
#include	"XFont.h"

class XFontD3D{
    
public:
	XFontD3D();
	~XFontD3D();

	HRESULT Init(	int fontType,			//本字体的类型。如 0-明流，1-粗圆
					int nTexSize,			//使用的纹理显存大小。目前必须用 512! 建议512*512。
					XFontInf *pFontInf);	//bmp汉字库的信息
	void	Release();

    // 2D方式显示文字
	// x,y是从左上角开始的坐标
	// strText是字符串指针，以0结束
	// dwColor是颜色参数
	// zoom是放大倍数
    HRESULT DrawText2d( FLOAT x, FLOAT y, 
                      const char* strText, DWORD dwColor, float zoom, float z=0.9);

	// 3D方式显示文字。未实现
	// x,y,z代表3维坐标系
	// strText是字符串指针，以0结束
	// r,g,b,a分别代表颜色值和alpha值， 需要注意的是比如1.0代表整形颜色值255, 0.5代表128.......
	// zoom是放大倍数
    HRESULT Render3DText( float x, float y, float z, char* strText,
							   FLOAT r,FLOAT g,FLOAT b, FLOAT a,float zoom);
private:
	int		fontType_;			// 本字体的类型。如 0-明流，1-粗圆
	int		texSize_;			// 纹理大小
	XFont	font_;				// 通用字体
};

#endif