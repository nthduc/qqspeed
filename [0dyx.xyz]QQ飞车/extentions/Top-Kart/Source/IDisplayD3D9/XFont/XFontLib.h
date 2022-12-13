//:XFontLib.h		by czy
//Bmp���ֿ�
#ifndef XFONTLIB_H
#define XFONTLIB_H

#include <windows.h>
#include "XBmp.h"
#include "XFontInf.h"

//����ĳ�ַ���λͼ��������Ϣ
struct CFontHz{
	UINT	c;							//�ַ���ASCII��(����˫�ֽ�)
	int		ColorCnt;					//2 or 16
	BYTE	*lpData;					//�ַ�λͼ��ָ��
//	DWORD	dwSize;						//�ַ�λͼ�ռ��С
	DWORD	w,h;						//�ַ�λͼ�Ŀ��,�߶�
	int		nextCharOffset;				//��һ���ַ���ƫ��
	DWORD   kx,ky;						//�ַ�λͼ��ԭ��֮��ľ���,ԭ���X,Y����
};

class XFontLib{
public:
	XFontLib()	{};
	~XFontLib()	{	Release();	};

	//����ָ����XFont���������Ӧ��Bmp�ļ�
	HRESULT Init( XFontInf* pFontInf);
	void Release();

	//��ȡָ���ַ���λͼ��Ϣ
	CFontHz *GetChar(UINT c);
private:
	//�ͷ��������Bmp�ļ�


	//��Bmp�ļ��л�õ���
	void GetHzMat(  UINT Hz,			//	Hz:	����
					BYTE* d);			//	d:	�����ε���	
	void GetHzMatFromBmp( UINT Hz, BYTE* d);

	XFontInf	m_FontInf;			//����Bmp�ֿ���Ϣ
//	XBmp		m_Bmp;				//����Bmp�ֿ���ڴ�Bmp�ļ�
	XBmpFile	m_Bmp;				//��Ϊʹ�ñ���Bmp�ֿ�����Bmp�ļ�!!!
	
	CFontHz		fontHz_;			//Ϊָ���ַ����ɵ�λͼ��Ϣ
	BYTE		m_HzMatrix[32*32];	//Ϊָ���ַ����ɵĺ��ֵ���, ���Ҫ����32*32

	BYTE*		cache_[65536];		//save Hanzi Mat which is used
};

#endif