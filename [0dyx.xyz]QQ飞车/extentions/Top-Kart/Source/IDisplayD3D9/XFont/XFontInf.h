//XFontInf.h	by	czy
//�������ֿ���Ϣ�ṹ��ʹ��Bmp�ļ��洢�������Ρ�

#ifndef XFONTINF_H
#define XFONTINF_H

struct XCharSet {
	unsigned long	charSetID; 
	//������2���ֽ����
	unsigned char	c0First,c0Last;		//��1�ֽڵ���ʼֵ����ֵֹ
	unsigned char	c1First,c1Last;		//��2�ֽڵ���ʼֵ����ֵֹ
};

//���ֿ���Ϣ�ṹ
struct XFontInf{		
	char*	sFileName;		//Bmp�ļ���
	int		nSize;			//���ֵ���Ŀ�Ⱥ͸߶�
	XCharSet*	hzCode;		//���ֱ��뷽ʽ��GB2312,GBK, Unicode,Big5...
	//������2���ֽ����
//	unsigned char	c0First,c0Last;		//��1�ֽڵ���ʼֵ����ֵֹ
//	unsigned char	c1First,c1Last;		//��2�ֽڵ���ʼֵ����ֵֹ
};

/*extern XFontInf FontInf24;
extern XFontInf FontInf16;
extern XFontInf FontInf12;
*/
extern XFontInf GBK10, GBK12,	GBK16,	GBK24;
extern XFontInf GBK10Y, GBK12Y, GBK16Y, GBK24Y;


#endif


