//============================================================================
// �ļ�����XFont.cpp
// ˵����  ������ʾ��������ʾ��Ӣ�ģ���Ҫ����� asc12 hzk12 
// ��д��  �޸���Dx8�������d3dfont.cpp
//
// ���ڣ�  2001/11/1
//============================================================================
#define STRICT
//#include	"StdAfx.h"
#include "..\Engine.h"
#include	<stdio.h>
#include	<math.h>
#include	"Xfont.h"
#include <cassert>

//=============== global variable =================================================================
//max str length=1024/8;
static UINT			uText[256];				// ���ִ�ת����˫�ֽ��ַ���ʽ
static XFontNode*	tText[256];				// �ַ��������Դ��е���Ϣ��ʽ

TOutHz		outHzs[256];					//��������ַ���
int			numHz;							//�ַ�����
//================================================================================

// ���ִ�ת����˫�ֽ��ַ���ʽ��return �ַ�����
// ���ֺ�Ӣ�ĵı���ʹ��Windows�ĸ�ʽ��Ӣ�� = s[0], ���� = s[0]*256+s[1]
// ע�⣬�����λ��ĸ����෴��
int Str_Int( const char *str, UINT *hz){
	BYTE *s= (BYTE *)str;
	int i=0;
	while( *s )	{
		i++;	assert(i<255);
		if( *s < 0x81 ){	// Ascii�ַ�
			*hz= *s;
			s++;
			hz++;
		}else{			// ����
			if (0==s[1]){	//end at half hanzi
				*hz = '#';	//
				hz++;
				break;
			}
			*hz= s[0]*256L + s[1];		
			s+=2;
			hz++;
		}
	}
	*hz= 0;		//zero end
	numHz = i;
	return i;	//�ַ�������
}

//ȡ�ú��ִ�hz[]��ÿ���ֶ�Ӧ������������Ϣ���� p[]��
//	hz[]:	���ִ�
//	p[];	������Ϣ
void Text_Texture( XFontLib* pFontLib, XFontTex* pFontTex, UINT hz[], int fontType, XFontNode* p[]){
	for( int i=0; hz[i]; i++)
		p[i]= pFontTex->GetChar( pFontLib, hz[i], fontType);		//get Texture infomation of hz[i]
}

//===================================================================
//˵  �������������Ϣ��debug.txt����������
//===================================================================
void OutDebug(char *text,...){
	static DWORD num=0;
	FILE *fp;
	char buf[256];

	if(num==0)
		fp=fopen("debug.txt","w");
	else
		fp=fopen("debug.txt","a");

	num++;

	va_list ap;

	va_start(ap,text);
	_vsnprintf(buf,256,text,ap);
	va_end(ap);

	fprintf(fp,"%d:%s\n",num,buf);

	fclose(fp);
}

//===================================================================
// ����
//===================================================================
//const MAX_NUM_VERTICES=50*6;	//����������50������

//===========================================================================
// ���캯��
//===========================================================================
XFont::XFont(){
}

//===========================================================================
//  ��������
//===========================================================================
XFont::~XFont(){
    Release();
}

//===========================================================================
//  ��ʼ���� �����С nTexSize*nTexSize			(512x512)
//===========================================================================
HRESULT XFont::Init( int fontType, int nTexSize, XFontInf *pFontInf)
{
	

	//modify+lincoln
	//[2004:12:23] [10:03]
	//���Ӵ�����
	//��ʼ�����ֿ�
	//{{{
	if(m_FontLib.Init( pFontInf)==S_OK)
	{
		fontType_ = fontType;
		m_FontInf = *pFontInf;

		// Establish the font and texture size
		m_fTextScale  = 1.0f; // Draw fonts into texture without scaling
		
		//˳����������Ŀ�Ⱥ͸߶�
		pFontTex_ = fontTexSet.getFontTex( pFontInf->nSize, texWidth_, texHeight_);		//80

		
		return S_OK;

	}	
	else 
	{
		fontType_ = -1;
		
		XFontInf tmp={ 0,	0,	0 };
		m_FontInf = tmp;

		// Establish the font and texture size
		m_fTextScale  = 0.0f; // Draw fonts into texture without scaling
		

		return S_FALSE;	 
	}

	
	//}}}
	
	
	
}


//===========================================================================
//  �ͷ�
//===========================================================================
void XFont::Release(){
  m_FontLib.Release();	//�ͷ�Bmp���ֿ⡣���ͷ���ɣ�ϵͳ�Զ��ͷ�
}

//===========================================================================
// 2D��ʽ��ʾ����, ׼������
// x,y�Ǵ����Ͻǿ�ʼ������
// strText���ַ���ָ�룬��0����
// zoom�ǷŴ���
//---------------------- return ------------------
// numHz �ַ�����
// outHzs[] ������ַ�������Ϣ
//===========================================================================
void XFont::PrepareText2d( FLOAT sx, FLOAT sy, const char* text, float zoom)
{
	float HzPitch = m_FontInf.nSize * zoom;		//���ּ��ˮƽ����
	int	  HzSize = (int)floor(HzPitch+0.5f);	//���ֵĴ�С,ȡ����		??�ƺ�Ӧ��0.1

	//����ʵ����ʾ�Ĵ�С
	float w,h;
	w = h = (float) HzSize;
	//��12*12����ģ��13*13,14*14
	if (12==m_FontInf.nSize && HzSize>=12 && HzSize<=14) {
		w=h=12.0f;
		if(HzSize>=13.0f){					//13*13,14*14?
//			sx+=1.0f;  
			sy+=1.0f;		//��12*12�����Ƶ�14*14���м�!!!
		}
	}

	numHz = Str_Int( text, uText);						// ������ת����Uint��ʽ
	Text_Texture( &m_FontLib, pFontTex_, uText, fontType_, tText);	// �����ֻ�������buffer 
	
	calcCood2d( sx, sy, w, h, HzPitch);
}

void XFont::calcCood2d( float sx, float sy, float w, float h, float HzPitch){
	int i;
	for(i=0; i<numHz; i++){
		UINT hz = uText[i];			//get a Hanzi
		XFontNode*	pTex= tText[i];	//get Texture Inf of the Hz
		
		//hz width height
		FLOAT Texw = (float) pTex->w / texWidth_;
		FLOAT Texh = (float) (pTex->h+0) / texHeight_;
		
		//hz Texture coordinate
		FLOAT tx1 = (float) pTex->left / texWidth_;
		FLOAT ty1 = (float) (pTex->top+0) / texWidth_;
		FLOAT tx2 = tx1+ Texw;
		FLOAT ty2 = ty1+ Texh;
		
		//���øú��ֵĶ�������
		outHzs[i].p[0].set( sx+0, sy+0, 0, tx1, ty1);
		outHzs[i].p[1].set( sx+w, sy+0, 0, tx2, ty1);
		outHzs[i].p[2].set( sx+w, sy+h, 0, tx2, ty2);
		outHzs[i].p[3].set( sx+0, sy+h, 0, tx1, ty2);
		
		// X��������
		if (hz<=0xff)	//ascii?
			sx+= HzPitch/2;
		else			//hz
			sx+= HzPitch;
	}
	
	assert( numHz == i);
}

//=============================================================================
// 3D��ʽ��ʾ����, ׼������
// x,y,z����3ά����ϵ
// strText���ַ���ָ�룬��0����
// zoom�ǷŴ���
//---------------------- return ------------------
// numHz �ַ�����
// outHzs[] ������ַ�������Ϣ
//=============================================================================
void XFont::Prepare3DText( float x, float y, float z, char* strText, float zoom)
{
	numHz = Str_Int( strText, uText);					// ������ת����Uint��ʽ
	Text_Texture(&m_FontLib, pFontTex_, uText, -1, tText);	// �����ֻ�������buffer

	for(int i=0; i<numHz; i++){
		UINT hz = uText[i];	//get a Hanzi
		XFontNode	*pTex= tText[i];	//get Texture Inf of the Hz

		//texture ���Ͻ� coordinate		
		FLOAT tx1 = (float) pTex->left / texWidth_;
	    FLOAT ty1 = (float) (pTex->top+0) / texWidth_;
		//texture width, height
		FLOAT w = (float) pTex->w  / texWidth_;
		FLOAT h = (float) (pTex->h+0) / texHeight_;
		//texture ���Ͻ� coordinate		
		FLOAT tx2 = tx1+ w;
	    FLOAT ty2 = ty1+ h;

		//����һ�����ֵľ���
		float m_fTextScale = 1.0f;	//czy???
		w = (tx2-tx1) * texWidth_  / ( 10.0f * m_fTextScale )*zoom;
		h = (ty2-ty1) * texHeight_ / ( 10.0f * m_fTextScale )*zoom;
		
		//���øú��ֵĶ�������
		outHzs[i].p[0].set( x,   y,   z,	tx1, ty2);
		outHzs[i].p[1].set( x+w, y,	  z,	tx2, ty2);
		outHzs[i].p[2].set( x+w, y+h, z,	tx2, ty1);
		outHzs[i].p[3].set( x,   y+h, z,	tx1, ty1);

		// X��������
		if (hz<=0xff)	//Asc?
			x+= w/2;
		else			//Hanzi
			x+= w;
	}
}

	//��һ�����ֵ����꣺p1 ���Ͻ� p2 ���½ǣ� p3 ���Ͻ�, ��3��	//x1,y1,z1���Ͻ� x2,y2,z2���½�
HRESULT XFont::Draw3DText( float *p1, float *p2, float *p3,DWORD dwColor,DWORD font,char *strText)
{
/*	float zoom=1.0f;
	float sx1=p1[0], sy1=p1[1], sz1=p1[2];
	float sx2=p2[0], sy2=p2[1], sz2=p2[2];

    if( d3dDevice() == NULL )
        return E_FAIL;

	// �����ֻ�������buffer 
	Str_Int(strText, s_sHz);
	Text_Texture(&m_FontLib, &m_FontTex, s_sHz, s_FontTex);
//	Text_Texture(&m_FontTex, s_sHz, s_FontTex);
	

    // Setup renderstate
/*    d3dDevice()->CaptureStateBlock( m_dwSavedStateBlock );
    d3dDevice()->ApplyStateBlock( m_dwDrawTextStateBlock );
    d3dDevice()->SetVertexShader( D3DFVF_FONT2DVERTEX );
    d3dDevice()->SetStreamSource( 0, m_pVB, sizeof(FONT2DVERTEX) );

    // Set filter states
    d3dDevice()->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    d3dDevice()->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );


    // Fill vertex buffer
    int	dwNumTriangles = 0;
	FONT2DVERTEX* pVertices = NULL;
    m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

	for(int i=0; s_sHz[i]; i++){
		UINT hz=s_sHz[i];
		XFontTexNode *pTex= s_FontTex[i];

		//hz width height
		FLOAT w = (float) pTex->w / texWidth_;
		FLOAT h = (float) (pTex->h+0) / texHeight_;

		//hz Texture coordinate
		FLOAT tx1 = (float) pTex->left / texWidth_;
	    FLOAT ty1 = (float) (pTex->top+0) / texWidth_;
		FLOAT tx2 = tx1+ w;
	    FLOAT ty2 = ty1+ h;

		
		float dx= (p3[0]-p1[0])*2;
		float dy= (p3[1]-p1[1])*2;

/ *		const zz=0.9f;	
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,zz,1.0f), dwColor, tx1, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,zz,1.0f), dwColor, tx1, ty1 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,zz,1.0f), dwColor, tx2, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,zz,1.0f), dwColor, tx2, ty1 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,zz,1.0f), dwColor, tx2, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,zz,1.0f), dwColor, tx1, ty1 );
* /
//		float	x1,x2;
//		float	y1,y2;
		float	z1,z2;

//	x1=(p3[0]-p1[0])*len+p1[0];		//sx
//	y1=(p3[1]-p1[1])*len+p1[1];		//sy
	z1=p1[2];						

//	x2=p2[0]-p1[0]+x1;
//	y2=p2[1]-p1[1]+y1;
	z2=p2[2];

	//p1 ���Ͻ� p2 ���½ǣ� x1,y1,z1���Ͻ� x2,y2,z2���½�
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx1, sy1,p1[2],1.0f), dwColor, tx1, ty1 );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx1+dx, sy1+dy,z1,1.0f), dwColor, tx2, ty1 );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx2, sy2, p2[2],1.0f), dwColor, tx1, ty2 );

	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx1+dx, sy1+dy, z1,1.0f), dwColor, tx2, ty1 );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx2+dx, sy2+dy, z2,1.0f), dwColor, tx2, ty2 );
	*pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx2, sy2, p2[2],1.0f), dwColor, tx1, ty2 );

		// X��������
		if (hz<=0xff){	//ascii?
			sx1+=dx/2;
			sy1+=dy/2;
			sx2+=dx/2;
			sy2+=dy/2;
		}else{			//Hanzi
			sx1+=dx;
			sy1+=dy;
			sx2+=dx;
			sy2+=dy;
		}
		dwNumTriangles += 2;
		// ���Ҫ����Ķ�������������,��������Ⱦ
		if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )	{
			// ���㻺�����,��Ⱦ,����������
			m_pVB->Unlock();
			d3dDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
			pVertices = NULL;
			m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );
			dwNumTriangles = 0L;
		}
/ *		// X��������
		if (hz<=0xff)	//ascii?
			sx+= w/2;
		else			//hz
			sx+= w;
* /
	}

    // Unlock and render the vertex buffer
    m_pVB->Unlock();
   
	// Draw
	if(dwNumTriangles)	d3dDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles);

    // Restore the modified renderstates
    d3dDevice()->ApplyStateBlock( m_dwSavedStateBlock );
*/
    return S_OK;
}
