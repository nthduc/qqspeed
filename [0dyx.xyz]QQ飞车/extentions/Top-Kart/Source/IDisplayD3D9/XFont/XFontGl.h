//============================================================================
// �ļ�����XFontD3D.h
// 
// ˵����  ������ʾ��������ʾ��Ӣ�ģ���Ҫ����� asc12 hzk12
//
// ��д�� �޸���Dx8�������d3dfont.h
//
//        
// ���ڣ�  2003/11/26	mike
//============================================================================
//����XFontTex��
//��Ҫ������ַ�д�������ϣ��������������Թ�render
//�ڲ�����ʹ������ķ�ʽ������"�������ʹ�á�Ƶ�ʶ�̬����
//���������Ժ󣬻��Զ�������õ��ַ��滻��
//��Ҫ����GetChar(), ������ڻ����У���ʹ��FontLib�õ��ַ�����

#ifndef XFONT_GL_H
#define XFONT_GL_H

#include	"../Display.h"
#include	"XFont.h"

class XFontD3D{
    
public:
	XFontD3D();
	~XFontD3D();

	HRESULT Init(	int fontType,			//����������͡��� 0-������1-��Բ
					int nTexSize,			//ʹ�õ������Դ��С��Ŀǰ������ 512! ����512*512��
					XFontInf *pFontInf);	//bmp���ֿ����Ϣ
	void	Release();

    // 2D��ʽ��ʾ����
	// x,y�Ǵ����Ͻǿ�ʼ������
	// strText���ַ���ָ�룬��0����
	// dwColor����ɫ����
	// zoom�ǷŴ���
    HRESULT DrawText2d( FLOAT x, FLOAT y, 
                      const char* strText, DWORD dwColor, float zoom, float z=0.9);

	// 3D��ʽ��ʾ���֡�δʵ��
	// x,y,z����3ά����ϵ
	// strText���ַ���ָ�룬��0����
	// r,g,b,a�ֱ������ɫֵ��alphaֵ�� ��Ҫע����Ǳ���1.0����������ɫֵ255, 0.5����128.......
	// zoom�ǷŴ���
    HRESULT Render3DText( float x, float y, float z, char* strText,
							   FLOAT r,FLOAT g,FLOAT b, FLOAT a,float zoom);
private:
	int		fontType_;			// ����������͡��� 0-������1-��Բ
	int		texSize_;			// �����С
	XFont	font_;				// ͨ������
};

#endif