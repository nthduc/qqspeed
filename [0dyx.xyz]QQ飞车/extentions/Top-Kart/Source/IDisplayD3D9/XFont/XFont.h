//============================================================================
// �ļ�����XFont.h
// 
// ˵����  ������ʾ��������ʾ��Ӣ�ģ���Ҫ����� asc12 hzk12
//
//
//        
// ���ڣ�  2001/11/1
//============================================================================
#ifndef XFONT_H
#define XFONT_H

#include	"XfontTex.h"
#include	"XfontInf.h"

//============================================================================
// �ඨ��
//============================================================================

//������ʾ��
class XFont{
	int			fontType_;

	XFontTex*	pFontTex_;				//�ֿ����������Ϣ
	XFontLib	m_FontLib;				//�ֿ�
	XFontInf	m_FontInf;				//�ֿ�Information

    int			texWidth_;               // ������
    int			texHeight_;              // ����߶�
    FLOAT		m_fTextScale;
public:

    // 2D��ʽ��ʾ����,׼������
	// x,y�Ǵ����Ͻǿ�ʼ������
	// strText���ַ���ָ�룬��0����
	// dwColor����ɫ����
	// zoom�ǷŴ���
    void PrepareText2d( FLOAT x, FLOAT y, const char* strText, float zoom);

	// 3D��ʽ��ʾ����,׼������
	// x,y,z����3ά����ϵ
	// strText���ַ���ָ�룬��0����
	// r,g,b,a�ֱ������ɫֵ��alphaֵ�� ��Ҫע����Ǳ���1.0����������ɫֵ255, 0.5����128.......
	// zoom�ǷŴ���
	void Prepare3DText( float x, float y, float z, char* strText, float zoom);
	
	//��һ�����ֵ����꣺p1 ���Ͻ� p2 ���½ǣ� p3 ���Ͻ�, ��3��	//x1,y1,z1���Ͻ� x2,y2,z2���½�
	HRESULT Draw3DText( float *p1, float *p2, float *p3,DWORD dwColor,DWORD font,char *strText);
    
	void* getTex()				{ return pFontTex_->getTex(); }
public:
    XFont();		    // ���캯��
    ~XFont();			// ��������
    void	Release();

    // ��ʼ��
	//		fontType:	������
	//		nTexSize:	�����С�� nTexSize*nTexSize
	//		pFontInf:	ʹ�õ�Bmp�ֿ�
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
	TPoint p[4];		//4��������������ͼ���꣬˳ʱ�����
};

#endif


