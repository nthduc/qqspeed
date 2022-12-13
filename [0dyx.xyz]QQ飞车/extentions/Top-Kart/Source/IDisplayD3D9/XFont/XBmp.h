//:Bmp.h		by czy
//Bmp�ļ����ڴ�ӳ����Ϊ���ֿ�ʹ��	֧�ֺڰ׺�16ɫ���Ҷȣ�ģʽ��16ɫ���ڷ����


#ifndef XBMP_H
#define XBMP_H
#include "windows.h"
#include <cstdio>

#pragma pack( push, 1 )
struct BmpFileHead 
{		//Bmp�ļ�ͷ
	char	identifier[2];
	DWORD	file_size;
	DWORD	reserved;
	DWORD	bitmap_data_offset;
	DWORD	bitmap_header_size;
	DWORD	width;
	DWORD	height;
	WORD	planes;
	WORD	bits_per_pixel;
	DWORD	compression;
	DWORD	bitmap_data_size;
	DWORD	hresolution;
	DWORD	vresolution;
	DWORD	colors;
	DWORD	important_colors;
	DWORD	palette[2];
	//bitmap data
};
#pragma pack( pop )

//Bmp�ļ����ڴ�ӳ����Ϊ���ֿ�ʹ��	֧�ֺڰ׺�16ɫ���Ҷȣ�ģʽ��16ɫ���ڷ����
class XBmp
{
	int m_ColorCnt;					//ɫ��
	int m_nWidth, m_nHeight;		//��ȣ�
	int m_nPitch;

	BYTE *m_pData;					//��ɫ����
	BmpFileHead *m_pBmp;			//�ļ�ͷ

public:
	XBmp();
	~XBmp();

	void Release();

	//�Ӵ��̵���Bmp�ļ�
	void Load(char* sFileName);		

	//��Bmp�ļ���������ļ�
	void Save(char* sFileName);		

	//���ɿյ��ڴ�Bmp
	void CreateBlank(int ColorCnt, int nWidth, int nHeight);	

	int	 FileLen(){ return m_pBmp->file_size; }

	//2ɫ(�ڰ�)  ��  16ɫ���Ҷȣ�
	int	 ColorCnt(){ return m_ColorCnt; }

	int	 Width(){ return m_nWidth; }

	int	 Height(){ return m_nHeight; }

	//��Bmp��λ��x,y��ΪnColor
	void Set(int x, int y, int nColor);		

	//��Bmp��λ��x,yȡ����ɫֵ
	int  Get(int x, int y);
};

//Bmp�ļ����ڴ�ӳ����Ϊ���ֿ�ʹ��	֧�ֺڰ׺�16ɫ���Ҷȣ�ģʽ��16ɫ���ڷ����
class XBmpFile
{
	int m_ColorCnt;					//ɫ��
	int m_nWidth, m_nHeight;		//��ȣ�
	int m_nPitch;
	
//	BYTE *m_pData;					//��ɫ����
//	BmpFileHead *m_pBmp;			//�ļ�ͷ
	int dataOffset_;
	FILE* fp_;						//�ļ���������ڶ�ȡ
	
	BYTE	ReadByte( int pos);

public:
	XBmpFile();
	~XBmpFile();
	void Release();
	
	//modify+lincoln
	//[2004:12:23] [9:38]
	//���ӷ���ֵ������Ӵ��̵���Bmp�ļ�ʧ�ܵ����
	//{{{
	HRESULT Load(char* sFileName);		
	//}}}
	
	//���ɿյ��ڴ�Bmp
//	void CreateBlank(int ColorCnt, int nWidth, int nHeight);	
	
//	int	 FileLen(){ return m_pBmp->file_size; }
	//2ɫ(�ڰ�)  ��  16ɫ���Ҷȣ�
	int	 ColorCnt(){ return m_ColorCnt; }
	int	 Width(){ return m_nWidth; }
	int	 Height(){ return m_nHeight; }
	
	//��Bmp��λ��x,yȡ����ɫֵ
	int  Get(int x, int y);
};
#endif
