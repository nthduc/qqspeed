//============================================================================
// �ļ�����XFontD3D.cpp
// ˵����  openGl ������ʾ��������ʾ��Ӣ�ģ���Ҫ�����
// ��д��  �޸���Dx8�������d3dfont.cpp
//
// ���ڣ�  2003/11/26	mike
//============================================================================
#include  "..\\StdAfx.h"
#include  "..\\d3d9wrapper.h"
#include  "..\\Engine.h"
#include  "XFontGl.h"
#include  "math.h"
#include  "..\\Texture9.h"

extern IDisplay*	g_pDisplay;

const MAX_NUM_VERTICES=50*6;	//����������50������

XFontD3D::XFontD3D()
{
	//modify+lincoln
	//[2004:12:23] [10:28]
	//���Ӵ�����
	//{{{
	fontType_	= -1;
	texSize_	= 0;
	//}}}
}

XFontD3D::~XFontD3D(){
//	Release();
}

//===========================================================================
//  ��ʼ���� ��������. nTexSize*nTexSize			(512x512)
//===========================================================================
HRESULT XFontD3D::Init( int fontType, int texSize, XFontInf *pFontInf){
	
	
	
	//modify+lincoln
	//[2004:12:23] [10:09]
	//���Ӵ�����
	//{{{
	if(font_.Init( fontType, texSize, pFontInf)==S_OK)
	{

		fontType_	= fontType;
		texSize_	= texSize;

		return S_OK;

	}
	else 
	{
		fontType_	= -1;
		texSize_	= 0;

		return S_FALSE;	 
	}
	//}}}
    
}

void XFontD3D::Release(){
	font_.Release();
}

extern TOutHz outHzs[256];
extern int	numHz;

//===========================================================================
// 2D��ʽ��ʾ����
// x,y�Ǵ����Ͻǿ�ʼ������
// strText���ַ���ָ�룬��0����
// dwColor����ɫ����
// zoom�ǷŴ���
//===========================================================================
HRESULT XFontD3D::DrawText2d( FLOAT sx, FLOAT sy, 
                            const char* strText, DWORD dwColor,float zoom, float z)
{
    
	//modify+lincoln
	//[2004:12:23] [10:24]
	//���Ӵ�����
	//{{{
	if( (fontType_ < 0)||( texSize_ <= 0 )  )
	{
		return S_FALSE;
	}
	//}}}
	
	font_.PrepareText2d( sx, sy, strText, zoom);

	for ( int k = 0; k < numHz; k++)
	{
		TOutHz::TPoint* p = outHzs[k].p;
		for( int i=0; i<4; i++)
		{
			p[i].x = (float)ceil(p[i].x) + 0.5f;
			p[i].y = (float)ceil(p[i].y) + 0.5f;	//������λ�ã���ʾ�����!
			
			p[i].tx *= texSize_;
			p[i].ty *= texSize_;
		}

		
		texMgr9.GetTexture( (HTEX)font_.getTex())->Draw(
			(int)p[0].x, (int)p[0].y, (int)(p[2].x-p[0].x), (int)(p[2].y-p[0].y),
			p[0].tx, p[0].ty, p[2].tx-p[0].tx, p[2].ty-p[0].ty,dwColor);
	};

    return S_OK;
}

//=============================================================================
// 3D��ʽ��ʾ����
// x,y,z����3ά����ϵ
// strText���ַ���ָ�룬��0����
// r,g,b�ֱ������ɫֵ�� ��Ҫע����Ǳ���1.0����������ɫֵ255, 0.5����128.......
// zoom�ǷŴ���
//=============================================================================
HRESULT XFontD3D::Render3DText(float x, float y, float z,  char* strText,
							   FLOAT r,FLOAT g,FLOAT b, FLOAT a,float zoom)
{
/*	if( d3dDevice() == NULL )
        return E_FAIL;
	
	font_->Prepare3DText( x, y, z, strText, zoom);

	D3DMATERIAL8 mtrl;
	D3D8Text_InitMaterial( mtrl, r, g, b , a);
    d3dDevice()->SetMaterial( &mtrl );

    // Setup renderstate
    d3dDevice()->CaptureStateBlock( m_dwSavedStateBlock );
    d3dDevice()->ApplyStateBlock( m_dwDrawTextStateBlock );
    d3dDevice()->SetVertexShader( D3DFVF_FONT3DVERTEX );
    d3dDevice()->SetStreamSource( 0, m_pVB, sizeof(FONT3DVERTEX) );

    d3dDevice()->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    d3dDevice()->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

    // Turn off culling for two-sided text
    d3dDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Fill vertex buffer
    FONT3DVERTEX* pVertices;
	DWORD         dwNumTriangles = 0L;
    m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

	for(int i=0; i<numHz; i++){
		TOutHz::TPoint* p = outHzs[i].p;

		//���øú��ֵĶ�������
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3( p[0].x, p[0].y, p[0].z), D3DXVECTOR3(0,0,-1), p[0].tx, p[0].ty );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3( p[3].x, p[3].y, p[3].z), D3DXVECTOR3(0,0,-1), p[3].tx, p[3].ty );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3( p[1].x, p[1].y, p[1].z), D3DXVECTOR3(0,0,-1), p[1].tx, p[1].ty );

		*pVertices++ = InitFont3DVertex( D3DXVECTOR3( p[2].x, p[2].y, p[2].z), D3DXVECTOR3(0,0,-1), p[2].tx, p[2].ty );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3( p[1].x, p[1].y, p[1].z), D3DXVECTOR3(0,0,-1), p[1].tx, p[1].ty );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3( p[3].x, p[3].y, p[3].z), D3DXVECTOR3(0,0,-1), p[3].tx, p[3].ty );

		dwNumTriangles += 2;				//1 Hanzi use 2 triangles
		if( dwNumTriangles*3 > (MAX_NUM_VERTICES-6) )		{//too long?
			// Unlock, render, and relock the vertex buffer
			m_pVB->Unlock();
			d3dDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
			m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );
			dwNumTriangles = 0L;
		}
	}
    // Unlock and render the vertex buffer
    m_pVB->Unlock();

	// Draw
	if( dwNumTriangles > 0 )
		d3dDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    // Restore the modified renderstates
    d3dDevice()->ApplyStateBlock( m_dwSavedStateBlock );
*/
	return S_OK;
}

void* CreateFontTex( int texSize){
	return (void*) texMgr9.GenTextureHandle( texSize, texSize, imageAttrib4444,true);
}

void DrawChar2Tex( void* pTexture,DWORD x,DWORD y,WORD color,CFontHz *zfontdata){
	HTEX hTex = (HTEX) pTexture;
	TImage* img = CreateImage( zfontdata->w, zfontdata->h, imageAttrib4444);
	USHORT* pix = (USHORT*) img->data;

	for(int i=0;i<zfontdata->h;i++)	{
		for(int j=0;j<zfontdata->w;j++)	{
			int BmpColor=*(zfontdata->lpData+i*zfontdata->w+j);
			int TexColor;
			if (2==zfontdata->ColorCnt){ //2 color Bmp
				TexColor= BmpColor? color : 0;
			}else	//16 color Bmp
				TexColor= BmpColor? BmpColor<<12 | 0x0fff : 0;	//aRGB4444, alpha��Ϊ�Ҷ�ֵ

			pix[ (i-zfontdata->ky)*(zfontdata->w)+ j-zfontdata->kx]=TexColor;
		}
	}

	texMgr9.GetTexture( hTex)->UpdateImage(x,y,img->w,img->h,img,0,0);
	DestroyImage( &img);
}
 