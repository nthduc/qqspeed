////////////////////////////////////////////////
//     FileName:    ImageEx.cpp
//     Author:      Fish
//     Date:        2006-10-11 15:47:57
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "ImageEx.h"

#define INVALID_IMG		-1

CImageEx::CImageEx():
m_pDisplay(NULL),
m_pImageAni(NULL)
{

}

CImageEx::~CImageEx()
{
	DestroyImageAni( &m_pImageAni );
	m_pImageAni = NULL;
	for ( vector<HIMG>::iterator it = m_ImgHandles.begin(); it != m_ImgHandles.end(); it++ )
	{
		m_pDisplay->ReleaseImageHandle(*it);
	}
}

void CImageEx::Init(IDisplay* Display)
{
	m_pDisplay = Display;
}

BOOL CImageEx::Load(LPCSTR file)
{
	if( m_pImageAni != NULL )
		return FALSE;
	m_pImageAni = LoadImageAni((char*)file);
	//ÔØÈëÊ§°Ü
	if( !m_pImageAni ) 
		return FALSE;

	m_nBlockX   = m_pImageAni->width_blocks;
	m_nBlockY   = m_pImageAni->height_blocks;
	m_Width     = m_pImageAni->plane_width;
	m_Height    = m_pImageAni->plane_height;
	m_nFrame    = m_pImageAni->frames;
	m_AniWidth  = m_pImageAni->ani_width;
	m_AniHeight = m_pImageAni->ani_height;

	if( m_ImgHandles.size() == 0 )
	{
		m_ImgHandles.resize( m_nBlockX*m_nBlockY*m_nFrame, INVALID_IMG );
	}

	for( int iframe=0; iframe<m_nFrame; iframe++ )
	{
		for( int j=0; j<m_nBlockY; j++ )
		{
			for( int i=0; i<m_nBlockX; i++ )
			{
				int index = iframe*m_nBlockX*m_nBlockY + j*m_nBlockX + i;
				if( m_ImgHandles[index] == INVALID_IMG )
				{
					m_ImgHandles[index] = m_pDisplay->SelectImage( m_pImageAni->image[index] );
				}
			}
		}
	}

	return TRUE;
}

void CImageEx::Draw(int frame, int x, int y)
{
	if ( frame >= m_nFrame )
		frame = frame % m_nFrame;
	if ( frame < 0 )
		frame = 0;
	m_pDisplay->DrawImage(x, y, m_ImgHandles[frame]);
}
