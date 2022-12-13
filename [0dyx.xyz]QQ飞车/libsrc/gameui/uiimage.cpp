
#include <windows.h>
#include "uiimage.h"
#include "display.h"
#include "imagelib.h"

#include "direct.h"
#include "io.h"

#include "vfilewrap.h"
#include "gameui.h"

extern IDisplay *g_pDisplay;


class CCoreUIImage : public CRefObject
{

public:

	CCoreUIImage( const std::string &path );

	//0:not changed,1:changed,-1:failed
	int CheckLoad();

	bool IsLoaded();
	void Unload();

	void Update();

public:

	std::string m_FilePath;
	
	std::vector<HIMG> m_ImgHandles;

	TImageAni* m_pImageAni;
	
	int   m_nBlockX, m_nBlockY;
	int   m_Width, m_Height;
	int   m_AniWidth, m_AniHeight;
	int   m_nFrame;

	int   m_LastUsedTick; //���һ��ʹ�õ�ʱ��
	int   m_CurUnloadHandle; //��ǰ�ͷŵ�m_ImgHandles��Ŀ

	static int m_CurTick;
	static int m_nUnloaded; //һ�������Ѿ��ж���image unloaded, ����һ��unloaded̫��

private:

	~CCoreUIImage();

};

const HIMG INVALID_IMG = (HIMG)0xffffffff;
const int MAX_UNLOADED = 1;
static std::vector<CCoreUIImage *>s_ImageArray;
int CCoreUIImage::m_CurTick = 0;
int CCoreUIImage::m_nUnloaded = 0;


CCoreUIImage::CCoreUIImage( const std::string &path) : m_FilePath(path)
{
	m_nBlockX=0;
	m_nBlockY=0;
	m_Width = 0;
	m_Height = 0;
	m_AniWidth = m_AniHeight = 0;
	m_nFrame=0;
	m_LastUsedTick = 0;
	m_pImageAni=NULL;
}

CCoreUIImage::~CCoreUIImage()
{
	Unload();
}

bool CCoreUIImage::IsLoaded()
{
	return (m_pImageAni!=NULL);
}


#ifdef _UI_RES_MORNITOR
#include "gameui.h"
#endif

//0:not changed,1:changed,-1:failed
int CCoreUIImage::CheckLoad()
{
	m_LastUsedTick = m_CurTick;

	if ( m_pImageAni != NULL )
		return 1;
	if( m_pImageAni == NULL )
	{
		m_pImageAni = LoadImageAni( (char *)m_FilePath.c_str() );
	}

#ifdef _UI_RES_MORNITOR
	GameUIConsoleInfo(m_FilePath.c_str());
#endif

	//����ʧ��
	if( !m_pImageAni ) return(-1);

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
					m_ImgHandles[index] = g_pDisplay->SelectImage( m_pImageAni->image[index] );
				}
			}
		}
	}
	DestroyImageAni( &m_pImageAni );
	m_pImageAni = (TImageAni*)1;
	m_CurUnloadHandle = 0;
	return(1);
}

void CCoreUIImage::Unload()
{
	while( m_CurUnloadHandle < m_ImgHandles.size() )
	{
		assert( m_ImgHandles[m_CurUnloadHandle] != INVALID_IMG );

		g_pDisplay->ReleaseImageHandle( m_ImgHandles[m_CurUnloadHandle] );
		m_ImgHandles[m_CurUnloadHandle] = INVALID_IMG;

		m_CurUnloadHandle++;
//		m_nUnloaded++;
//		if( m_nUnloaded >= MAX_UNLOADED ) return;
	}
	m_ImgHandles.resize(0);
	//DestroyImageAni( &m_pImageAni );
	m_pImageAni = NULL;
}

void CCoreUIImage::Update()
{
	if( IsLoaded() && m_nUnloaded<MAX_UNLOADED && m_CurTick-m_LastUsedTick>UI_IMAGE_CACHE_TICK )
	{
		Unload();
	}
}

CCoreUIImage *CreateCoreUIImage( const std::string &path )
{
	int i;
	for( i=0; i<s_ImageArray.size(); i++ )
	{
		CCoreUIImage *pimg = s_ImageArray[i];
		if( pimg && strcmp(pimg->m_FilePath.c_str(), path.c_str())==0 )
		{
			pimg->IncRefCount();
			return pimg;
		}
	}

	for( i=0; i<s_ImageArray.size(); i++ )
	{
		if( s_ImageArray[i] == NULL ) break;
	}

	if( i == s_ImageArray.size() ) s_ImageArray.resize(i+1);

	s_ImageArray[i] = new CCoreUIImage( path );
	return s_ImageArray[i];
}

void ReleaseCoreUIImage( CCoreUIImage *pimg )
{
	if( pimg->GetRefCount() > 1 )
	{
		pimg->Release();
		return;
	}

	for( int i=0; i<s_ImageArray.size(); i++ )
	{
		if( s_ImageArray[i] == pimg )
		{
			pimg->Release();
			s_ImageArray[i] = NULL;
			return;
		}
	}
	assert(0);
	return;
}

static bool IsUIFileExist( const char* fname)
{
	FILE* fp = fopen( fname, "rb");
	if (0==fp)
		return false;
	fclose(fp);
	return true;
}


//����:ָ��һ��Ŀ¼srcpath���������ڴ��е�ǰ�������Դ�ļ�ɾ������ʣ�µ��ļ����Ƕ�����ļ�
//ɾ�������ļ�,����ļ��б�dump.txt�ļ�
void DumpImageInfo(const char* srcpath)
{
	if(strcmp(srcpath,"")==0) return;
	
	//���浱ǰĿ¼
	char oldPath[_MAX_PATH];
	_getcwd(oldPath,_MAX_PATH); 

	//�ı䵱ǰĿ¼������Ŀ¼��
	_chdir(srcpath);

	FILE* fp=NULL;
	fp=fopen("dump.txt","w+");

	std::vector<CCoreUIImage *>::iterator iter;
	for(iter=s_ImageArray.begin();iter!=s_ImageArray.end();iter++)
	{
		CCoreUIImage* pImg=(CCoreUIImage*)(*iter);
		
		if(!IsUIFileExist(pImg->m_FilePath.c_str()))
			fprintf(fp,"%d:%s-------------not found!\n",int(iter-s_ImageArray.begin()),pImg->m_FilePath.c_str());	
		else
			fprintf(fp,"%d:%s\n",int(iter-s_ImageArray.begin()),pImg->m_FilePath.c_str());		

		fflush(fp);

		//ע��,����ȷ���ļ���д
		DeleteFile(pImg->m_FilePath.c_str());
			
	}

	fclose(fp);
	
	//�ָ���ǰĿ¼
	_chdir(oldPath); 	

}

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
CUIImage::CUIImage( const char *path ,int prior/*=UI_IMG_PRIOR_DEFAULT*/)
{
	m_pCoreImg = CreateCoreUIImage( path );

	m_nImgPrior=prior;

	
	//���ָ���˴������룬������������Դ
	if(m_nImgPrior&UI_IMG_PRIOR_LOAD_INIT)
		m_pCoreImg->CheckLoad();


}

CUIImage::~CUIImage()
{
	ReleaseCoreUIImage( m_pCoreImg );
}




const char* CUIImage::GetFilePath()
{
	return m_pCoreImg->m_FilePath.c_str();
}

int CUIImage::GetWidth()
{
	m_pCoreImg->CheckLoad();
	return m_pCoreImg->m_Width;
}

int CUIImage::GetHeight()
{
	m_pCoreImg->CheckLoad();
	return m_pCoreImg->m_Height;
}

int CUIImage::GetNumFrame()
{
	m_pCoreImg->CheckLoad();
	return m_pCoreImg->m_nFrame;
}


//�õ�ָ��֡�е�ĳ����ɫ
//x,y���Ϊ�����ͼƬԭ�������
stColorQuad CUIImage::GetPointColor(int x,int y,int frame)
{

	stColorQuad color(0,0,0,0);
	
	TImageAni *pimageani = m_pCoreImg->m_pImageAni;

	if( pimageani
	  &&(frame>=0)&&(frame<GetNumFrame())
	  &&(x>=0)&&(x<GetWidth()) 
	  &&(y>=0)&&(y<GetHeight()) )
	{
		

		//�ж�(x,y)���Ǹ�block��
		int ix=x/pimageani->ani_width;
		int iy=y/pimageani->ani_height;
		int iBlcok=frame*pimageani->width_blocks*pimageani->height_blocks+iy*pimageani->width_blocks+ix;

		TImage* pImg=pimageani->image[iBlcok];

		if(pImg)
		{

			//���������blcok������λ��
			int px=x-ix*pimageani->ani_width;
			int py=y-iy*pimageani->ani_height;
			WORD value=((WORD*)pImg->data)[py*pimageani->ani_width+px];				

			//����ͼ���ʽ
			//imageAttrib1555=0,a1r5g5b5
			//imageAttrib4444=1,a4r4g4b4
			//imageAttrib8888=2,a8r8g8b8
			if(pImg->attrib==imageAttrib4444)
			{				
				ReduceHi4444(value,&color.a,&color.r,&color.g,&color.b);
			}			
			else if(pImg->attrib==imageAttrib1555)
			{				
				ReduceHi1555(value,&color.a,&color.r,&color.g,&color.b);
			}
			else if(pImg->attrib==imageAttrib8888)
			{			
				ReduceHi8888(value,&color.a,&color.r,&color.g,&color.b);
			}

					
		}

	}

	return color;


}



//���Ƿ���ָ��֡��
//͸������Ϊ��������
BOOL CUIImage::IsPointAlphaIn(int x,int y,int frame)
{

	if(!IsPointIn(x,y)) return false;
	else return (GetPointColor(x,y,frame).a!=0) ;
	
}


//���Ƿ���ͼƬ��
//������͸�����
BOOL CUIImage::IsPointIn(int x,int y)
{
	return ( (x>=0)&&(x<GetWidth())&&(y>=0)&&(y<GetHeight()) );
}


//��frame<0ʱ��,������
//��frame>=UI_IMAGE_MAX_FRAME����frame>=m_pImageAni->framesʱ��,�������֡
//���֡��������UI_IMAGE_MAX_FRAME=10000
void CUIImage::Draw( int left, int top, int frame/*=0*/,DWORD dwEffect/*=0*/,DWORD dwColor/*=0xffffffff*/)
{	
	//���frameС��0,�򲻻���
	if( frame<0 ) return;

	//����ʧ��
	if(m_pCoreImg->CheckLoad()<0) return;
	
	int nBlockX=m_pCoreImg->m_nBlockX;
	int nBlockY=m_pCoreImg->m_nBlockY;

	
	//�ж�frame�Ƿ񳬽�,������磬��������һ֡
	if(frame>=m_pCoreImg->m_nFrame) 
		frame=m_pCoreImg->m_nFrame-1;

	for( int j=0; j<nBlockY; j++ )
	{
		for( int i=0; i<nBlockX; i++ )
		{
			int index = frame*nBlockX*nBlockY + j*nBlockX+i;
			g_pDisplay->DrawImage( left+i*m_pCoreImg->m_AniWidth, top+j*m_pCoreImg->m_AniHeight, m_pCoreImg->m_ImgHandles[index],dwEffect,dwColor );
		}
	}
}


//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
Point2D CUICursor::m_Pos(0,0);
bool    CUICursor::m_bShow = true;//��Զֻ��һ��cursor��������Ϊ��ľ�̬����

CUICursor::CUICursor( const char *name, const char *path )
		 : CUIImage(path,UI_IMG_PRIOR_LOAD_INIT), m_Name(name), m_Life(0), m_AnimStep(0.8f), m_HotSpot(0,0), m_HoldFrame(0), m_RightFrame(0)
{
#ifdef _WINDOWS_CURSOR_
		m_hCursor = LoadCursorFromFile(path);
		char driver[4], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
		_splitpath(path, driver, dir, fname, ext);
		char lpath[MAX_PATH];
		char lfname[MAX_PATH] = "\0";
		sprintf(lfname, "%sl", fname);
		_makepath(lpath, driver, dir, lfname, ext);
		char rpath[MAX_PATH];
		char rfname[MAX_PATH] = "\0";
		sprintf(rfname, "%sr", fname);
		_makepath(rpath, driver, dir, rfname, ext);
		m_hCursorL = LoadCursorFromFile(lpath);
		m_hCursorR = LoadCursorFromFile(rpath);
#endif
}

CUICursor::~CUICursor()
{
}


void CUICursor::Update( float deltatime )
{
	m_Life += deltatime;
}

void CUICursor::SetActive()
{
#ifdef _WINDOWS_CURSOR_
	if( (GetKeyState(VK_LBUTTON) & 0xff00) )
	{
		SetCursor(m_hCursorL ? m_hCursorL : m_hCursor);
	}
	else if( (GetKeyState(VK_RBUTTON) & 0xff00) )
	{
		SetCursor(m_hCursorR ? m_hCursorR : m_hCursor);
	}
	else
	{
		SetCursor(m_hCursor);
	}
#endif
}
//cursor�����Լ�������ֵ�������Լ�
//frame����Ϊ����ͨ֡���������֡holdframe���Ҽ�����֡rightframe��
void CUICursor::Render()
{
	int nframe = GetNumFrame();

	//ͼ�񲻴���
	if(nframe<=0) return;
		
	int start, end;

	if(m_RightFrame<m_HoldFrame)
		m_RightFrame=m_HoldFrame;


	//�������
	if( (GetKeyState(VK_LBUTTON) & 0xff00) )
	{
		start = m_HoldFrame;

		if(m_RightFrame>m_HoldFrame)
			end = m_RightFrame - 1;
		else 
			end = nframe - 1;
	}
	//�Ҽ�����
	else if( (GetKeyState(VK_RBUTTON) & 0xff00) )
	{
		start = m_RightFrame;

		end = nframe-1;
	}
	//��ͨ״̬
	else 
	{
		start = 0;

		if(m_HoldFrame>0)
			end = m_HoldFrame - 1;
		else if(m_RightFrame>m_HoldFrame)
			end = m_RightFrame - 1;
		else 
			end = nframe - 1;	 
	}


	
	nframe = end - start + 1;
	if(nframe<=0) nframe=1;	

	int frame = int(m_Life*nframe/m_AnimStep) % nframe;
	if( frame < 0 ) frame = 0;
	if( frame >= nframe ) frame = nframe-1;

	frame += start;

	Draw( m_Pos.x-m_HotSpot.x, m_Pos.y-m_HotSpot.y, frame );
}

void UpdateUIImage()
{
	CCoreUIImage::m_CurTick++;
	CCoreUIImage::m_nUnloaded = 0;

	for( int i=0; i<s_ImageArray.size(); i++ )
	{
		CCoreUIImage *pimg = s_ImageArray[i];
		
		if(pimg)
			pimg->Update();
	}
}
