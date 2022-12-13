// IDisplayD3D91.cpp: implementation of the IDisplayD3D9 class.
//
//////////////////////////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "IDisplayD3D91.h"
#include "Texture9.h"
//#include "q3d/gfx.h"

#include <string>
#include <vector>
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace std;
static vector<string> lines;

//o---------------------------------------------o
//|		CCtrl									|
//o---------------------------------------------o
enum TCtrlType{
	eCtrlColor	= 0,
	eCtrlFont
};

class CCtrl{
	static int	colorIdx_;
	static int	pos_[100];
	static int	data_[100];		//type | num
	static int	cnt_;
public:
	static void reset()						{ cnt_=0; colorIdx_=0; }
	static void setCnt( int i)				{ cnt_ = i; }
	static int	getColorIdx()				{ return colorIdx_; }
	//static void	setColorIdx( int colorIdx)	{ colorIdx_ = colorIdx; }
	static int	getPos( int nCtrl)			{ return pos_[nCtrl]; }
		
	static void doCtrl( int nCtrl);

	static void end()						{ pos_[cnt_] = -1; }
	static void addCtrl( TCtrlType ctrlType, const char* &str, int strPos);
};

int CCtrl::colorIdx_;
int	CCtrl::pos_[100];
int	CCtrl::data_[100];		//type | num
int	CCtrl::cnt_;

void CCtrl::doCtrl( int nCtrl){
	int ctrlType = data_[nCtrl] >>8;
	int n =	data_[nCtrl] & 0xff;
	
	switch( ctrlType) {
	case eCtrlColor:
		colorIdx_ = n;
		break;
	case eCtrlFont:
		display3D_->SetFont( n);
		break;
	default:
		assert(0);
	}
}




void CCtrl::addCtrl( TCtrlType ctrlType, const char* &str, int strPos){
	assert( str[2]>='0' && str[2]<='9');
	pos_[cnt_]		= strPos;
	data_[cnt_++]	= (ctrlType<<8) | (str[2]-'0');
	str +=3;
}

void parseCtrl( const char* str, char* nukeStr){
	int i=0; 
	CCtrl::reset();	
	while (*str){
		if ('~'==*str){					// ~~ : '~'
			switch( str[1] ) {
				case '~':
					*nukeStr++ = '~';
					str +=2;	i++;
					continue;
				case 'C':
					CCtrl::addCtrl( eCtrlColor, str, i);
					continue;	//not i++
				case 'F':
					CCtrl::addCtrl( eCtrlFont, str, i);
					continue;	//not i++
				default:
					assert(0);	//error contral format
			}
		}else{
			//if ( '\n' != *str )	保留 “回车”
			i++;
			*nukeStr++ = *str++;
		}
	}
	*nukeStr = 0;
	CCtrl::end();
}


IDisplayD3D9::IDisplayD3D9()
{
	fontType_ = 0;
	for(int i = 0; i < 10; i++)
	{
		ctrlTextColor_[i] = 0xffffffff;
	}
}

IDisplayD3D9::~IDisplayD3D9()
{

}

HRESULT	IDisplayD3D9::CreateFullScreenDisplay(HWND hWnd, int nScreenType)
{
	int width,height;
	
	switch(nScreenType) {
	case screen640x480  :
		{
			width = 640;
			height = 480;
		}
		break;
	case screen800x600  :
		{
			width = 800;
			height = 600;
		}
		break;
	case screen1024x768 :
		{
			width = 1024;
			height = 768; 
		}
		break;
	};

	HRESULT hr = _d3d9.CreateDisplay( );
	font10_[0].Init(0, 512, &GBK10);
	font10_[1].Init(0, 512, &GBK10Y);

 
	font12_[0].Init( 0, 512, &GBK12);
	font16_[0].Init( 0, 512, &GBK16);

	font12_[1].Init( 1, 512, &GBK12Y);
	font16_[1].Init( 1, 512, &GBK16Y);
 
	font24_[0].Init( 0,512,&GBK24);
	font24_[1].Init( 1,512,&GBK24Y);
	
	return hr;
}


//add+lincoln
//[2005:1:18] [16:34]
//增加裁减状态堆栈
//{{{
stClipState IDisplayD3D9::GetClipState( )
{
	stClipState state;

	GetClipRect(state.ox,state.oy,state.width,state.height);
	state.enable=IsClipEnable();

	return state;
}

void IDisplayD3D9::SetClipState( stClipState state )
{

	//设置当前状态
	SetClipRect(state.ox,state.oy,state.width,state.height);
	EnableClipRect(state.enable);

}

void IDisplayD3D9::PushClipState()
{
	//备份当前状态
	m_ClipMatrix.push(GetClipState());
}


//接口
void IDisplayD3D9::PopClipState()
{

	//恢复备份
	SetClipState(m_ClipMatrix.top());	

	//删除备份
	m_ClipMatrix.pop();

}



//add+lincoln
//[2005:1:18] [18:39]
//计算两个矩形的交集
static stClipState MergeClipState(stClipState src,stClipState dst)
{
	stClipState interRect;

	//完全不相交
	if( (dst.ox+dst.width  <=  src.ox) ||
		(dst.oy+dst.height <=  src.oy) ||
		(src.ox+src.width  <= dst.ox) ||
		(src.oy+src.height <= dst.oy) )
	{	
		interRect=stClipState(0,0,0,0);
	}

	//src在dst中
	else if( (dst.ox<=src.ox) && (dst.oy<=src.oy) && 
		(dst.ox+dst.width>=src.ox+src.width) &&
		(dst.oy+dst.height>=src.oy+src.height) )
	{

		interRect=src;
	}


	//dst在src中
	else if( (src.ox<=dst.ox) && (src.oy<=dst.oy) && 
		(src.ox+src.width>=dst.ox+dst.width)  &&
		(src.oy+src.height>=dst.oy+dst.height) )
	{
		interRect=dst;
	}

	//部分相交
	else
	{

		int xbegin=(dst.ox>src.ox)?dst.ox:src.ox;
		int ybegin=(dst.oy>src.oy)?dst.oy:src.oy;

		int xend=(dst.ox+dst.width<src.ox+src.width)?(dst.ox+dst.width):(src.ox+src.width);
		int yend=(dst.oy+dst.height<src.oy+src.height)?(dst.oy+dst.height):(src.oy+src.height);

		interRect=stClipState(xbegin,ybegin,xend-xbegin,yend-ybegin);

	}

	return interRect;

}

void IDisplayD3D9::PushClipRect(int left, int top, int width, int height)
{

	PushClipState();

	stClipState curState=GetClipState();	
	stClipState newState(left,top,width,height);	
	stClipState mergeState=MergeClipState(curState,newState);
	mergeState.enable=true;

	//设置新的cliprect
	SetClipState(mergeState);

}
//}}}


HRESULT IDisplayD3D9::CreateWindowedDisplay(HWND hWnd, int nScreenType)
{
	int width,height;
	
	switch(nScreenType) {
	case screen640x480  :
		{
			width = 640;
			height = 480;
		}
		break;
	case screen800x600  :
		{
			width = 800;
			height = 600;
		}
		break;
	case screen1024x768 :
		{
			width = 1024;
			height = 768; 
		}
		break;
	};

	RECT rt;
	::GetClientRect(hWnd,&rt);

	D3D9::m_Width = rt.right - rt.left;
	D3D9::m_Height = rt.bottom - rt.top;
//	D3D9::m_Width = width;
//	D3D9::m_Height = height;

	HRESULT hr = _d3d9.CreateDisplay( );

	font10_[0].Init(0, 512, &GBK10);
	font10_[1].Init(0, 512, &GBK10Y);

	font12_[0].Init( 0, 512, &GBK12);
	font16_[0].Init( 0, 512, &GBK16);

	font12_[1].Init( 1, 512, &GBK12Y);
	font16_[1].Init( 1, 512, &GBK16Y);

	font24_[0].Init( 0, 512, &GBK24);
	font24_[1].Init( 1, 512, &GBK24Y);

	return hr;

}
	
HRESULT	IDisplayD3D9::UpdateBounds()
{
	return E_NOTIMPL;
}

HRESULT	IDisplayD3D9::DestroyObjects()
{
	imageMgr9.Release();
//	font24_.Release();
	font12_[0].Release();
	font16_[0].Release();

	font12_[1].Release();
	font16_[1].Release();
	font24_[0].Release();
	font24_[1].Release();

	font10_[0].Release();
	font10_[1].Release();
	fontTexSet.Release();
	return _d3d9.Release();
}

HRESULT	IDisplayD3D9::RestoreAllSurfaces()
{
	return E_NOTIMPL;
}

//extern IGfx* g_pGfx;

//Display methods
HRESULT	IDisplayD3D9::ClearScreen(DWORD dwColor)
{
	_d3d9.BeginSence();
	//g_pGfx->GetCurScene()->RenderUIName();
	return S_OK;
}

HRESULT	IDisplayD3D9::BeginScreen()
{
	return _d3d9.BeginSence();
}

HRESULT	IDisplayD3D9::UpdateScreen()
{
	return _d3d9.EndSence();
}


//Image utilities
HIMG IDisplayD3D9::SelectImage( TImage *pImage)
{
	return _d3d9.SelectImage(pImage,true);
}
	
HIMG IDisplayD3D9::GenImageHandle( int w, int h, int attrib)
{
	return _d3d9.GenImageHandle(w,h,attrib,true);
}

void IDisplayD3D9::ReleaseImageHandle( HIMG hImg)
{
	imageMgr9.ReleaseHandle( hImg);
}


void IDisplayD3D9::UpdateImage( HIMG hImg, int left, int top, TImage *pImage)
{
	assert(pImage);

	_d3d9.UpdateImage(hImg,0,0,pImage->w,pImage->h,pImage,0,0);
}

int	IDisplayD3D9::GetImageWidth(HIMG hImg)
{
	CImage9* pimg = imageMgr9.GetImage(hImg);

	if(pimg)
	{
		return pimg->GetImageWidth();
	}

	return 0;
}

int IDisplayD3D9::GetImageHeight(HIMG hImg)
{
	CImage9* pimg = imageMgr9.GetImage(hImg);

	if(pimg)
	{
		return pimg->GetImageHeight();
	}

	return 0;
}

void IDisplayD3D9::DrawImage( int left, int top, HIMG hImg, DWORD effect /*=0*/, 
							 DWORD dwColor /*=0xffffffff*/)
{
	_d3d9.DrawImage(left,top,hImg,effect,dwColor);
}
	
void  IDisplayD3D9::DrawImageArc(int left,int top,int angle,int rot,HIMG hImg,DWORD effect, DWORD dwColor)
{
	_d3d9.DrawImageArc(left,top,angle,rot,hImg,effect,dwColor);
}

void IDisplayD3D9::SetImageCenter( float centerX, float centerY)
{
	_d3d9.SetImageCenter(centerX,centerY);
}

void IDisplayD3D9::SetImageScale( float scaleX, float scaleY)
{
	_d3d9.SetImageScale(scaleX,scaleY);
}
	
void IDisplayD3D9::SetImageRotate( float degree)
{
	_d3d9.SetImageRotate(degree);
}

bool IDisplayD3D9::EnableTexLinear( bool flag)
{
	_d3d9.EnableTextureLinear(flag);
	return true;
}

HIMG IDisplayD3D9::SnapScreen( int left, int top, int width, int height)
{
	return 0;
}

TImage*  IDisplayD3D9::SnapScreen2( int left, int top, int width, int height)
{
	return NULL;
}
	
void IDisplayD3D9::SetClipRect( int left, int top, int width, int height)
{
	_d3d9.SetClipRect(left,top,width,height);
}

void IDisplayD3D9::GetClipRect( int&left, int&top, int&width, int&height)
{
	_d3d9.GetClipRect(left,top,width,height);
}
	
void IDisplayD3D9::EnableClipRect( bool flag)
{
	_d3d9.EnableClip(flag);
}

bool IDisplayD3D9::IsClipEnable()
{
	return _d3d9.IsClipEnable();
}

///??
void IDisplayD3D9::SetMaxTex( int size)
{
	return;
}

void IDisplayD3D9::DrawPixel(int x, int y, DWORD dwColor)
{
	_d3d9.DrawPixel(x,y,dwColor);
}

void IDisplayD3D9::DrawLine(int x1, int y1, int x2, int y2, DWORD dwColor)
{
	_d3d9.DrawLine(x1,y1,x2,y2,dwColor);
}

void IDisplayD3D9::DrawPolygon( int numPoints, const float* xy, DWORD dwColor)
{
	_d3d9.DrawPolygon(numPoints,xy,dwColor);
}

void IDisplayD3D9::DrawPolygon( int numPoints, const float* xy, DWORD* dwColor)
{
	_d3d9.DrawPolygon(numPoints,xy,dwColor);
}


void IDisplayD3D9::DrawBar(int left, int top, int width, int height, DWORD dwColor)
{
	_d3d9.DrawBar(left,top,width,height,dwColor);
}

void IDisplayD3D9::DrawBox(int left, int top, int width, int height, DWORD dwColor)
{
	_d3d9.DrawBox(left,top,width,height,dwColor);
}
extern float g_fXUIScale;
extern float g_fYUIScale;

void IDisplayD3D9::DrawText( int sx, int sy, const char* strText, DWORD dwColor, int size,UINT nFlag)
{

	size *= g_fXUIScale;
	if (_d3d9.m_bOK)
	{
		if (size <= 10)
		{
			font10_[fontType_].DrawText2d( sx, sy, strText, dwColor, size/10.0f);
		}
		else if ( size < 16)
		{
			font12_[fontType_].DrawText2d( sx, sy, strText, dwColor, size/12.0f);
		}
		else 
			if ( 16 == size)	//只有大小刚好16的font16
			{
				font16_[fontType_].DrawText2d( sx, sy, strText, dwColor, size/16.0f);
			}
			else
			{
				font24_[fontType_].DrawText2d( sx, sy, strText, dwColor, size/24.0f);
			}
	}
}

void IDisplayD3D9::DrawTextEx( int left, int top, int width, int height, 
								const char* str, int size, int space, DWORD dwColor, UINT flags)
{
		//cut to mult lines, put in lines
	if ( 0 == EstimateLine( str, width, size, space) || !_d3d9.m_bOK)
	{
		return;
	}

	//real draw text
	int xSize = size, ySize = size+space;
	int dispLine = (height+space)/ySize;
	int numLine = min( lines.size(), dispLine);

	int topEmpty = 0; 
	if (flags &	dtVCenter){
		topEmpty = (height - (ySize*numLine-space)) / 2;
	}else if (flags & dtBottom){
		topEmpty = height - (ySize*numLine-space);
	}

	for(int i=0; i<numLine; i++){ 
		int y = topEmpty+i*ySize;
		if ( y<0 || y+size>height)		//clip, not ySize
	 	 	continue;
		int leftEmpty = 0;
		if (flags &	dtCenter){
			leftEmpty = (width - (size/2)*lines[i].length()) / 2;
		}else if (flags & dtRight){
			leftEmpty = width - (size/2)*lines[i].length();
		}
		DrawText( leftEmpty+left, y+top, (char*)lines[i].c_str(), dwColor, size);
	}
}
	
void IDisplayD3D9::DrawCtrlText( int left, int top, int width, int height, 
								  const char* str, int size, int space, UINT flags)
{
	if (!_d3d9.m_bOK) 
	{
		return;
	}
		//strip ctrl character
	static char nukeStr[999];
	parseCtrl( str, nukeStr);
	//cut to mult lines, put in lines
	if ( 0==EstimateLine( nukeStr, width, size, space))
		return;
	
	int saveFontType = fontType_;
	//caculate empty area
	int xSize = size, ySize = size+space;
	int dispLine = (height+space)/ySize;
	int numLine = min( lines.size(), dispLine);
	
	int topEmpty = 0; 
	if (flags &	dtVCenter){
		topEmpty = (height - (ySize*numLine-space)) / 2;
	}else if (flags & dtBottom){
		topEmpty = height - (ySize*numLine-space);
	}

	//real draw text
	//DWORD dwColor = ctrlTextColor_[0];
	//CCtrl::setColorIdx(0);
	int nChar = 0,  nCtrl=0;
	//	enableClipRect( 0!=(flags & dtClipRect));
	for(int i=0; i<numLine; i++){ 
		//caculate up/down empty
		bool isDraw = true;
		int y = topEmpty+i*ySize;
		if ( y<0 || y+size>height)		//clip, not ySize
			isDraw = false;

		//caculate left empty 
		int leftEmpty = 0;
		if (flags &	dtCenter){
			leftEmpty = (width - (size/2)*lines[i].length()) / 2;
		}else if (flags & dtRight){
			leftEmpty = width - (size/2)*lines[i].length();
		}
		
		string drawed="", phase="";
		while(1){
			if ( drawed.length() + phase.length() >= lines[i].length()){//line end
				if ( isDraw ){
					DWORD dwColor = ctrlTextColor_[CCtrl::getColorIdx()];
					DrawText( leftEmpty+left+drawed.length()*(size/2), y+top, (char*)phase.c_str(), dwColor, size);
				}
				break;
			}
			if ( '\n' == nukeStr[nChar]){
				nChar++;
				continue;
			}
			if ( nChar==CCtrl::getPos(nCtrl)){
				if ( isDraw ){
					DWORD dwColor = ctrlTextColor_[CCtrl::getColorIdx()];
					DrawText( leftEmpty+left+drawed.length()*(size/2), y+top, (char*)phase.c_str(), dwColor, size);
				}
				drawed += phase;
				phase = "";
				CCtrl::doCtrl( nCtrl);
				nCtrl++;	//next ctrl
			}else{
				phase += nukeStr[nChar++];
			}
		}
	}
	fontType_ = saveFontType;	//restore
}
	
DWORD IDisplayD3D9::SetCtrlTextColor( int colorIdx, DWORD dwColor)
{
	assert( colorIdx >= 0 && colorIdx < MAX_TEXT_COLOR);
	DWORD oldColor = ctrlTextColor_[colorIdx];
	ctrlTextColor_[colorIdx] = dwColor;
	return oldColor;
}

int	IDisplayD3D9::SetFont( int fontType)
{
	assert( fontType >= 0 && fontType <= 1);
	int save = fontType_;
	fontType_ = fontType;
	return save;

}

void IDisplayD3D9::ClearCtrl( const char* ctrlStr, char* result)
{
	parseCtrl( ctrlStr, result);
}

int	IDisplayD3D9::EstimateLine( const char* str, int width, int size, int space)
{
	int wid = width / (size / 2);
	
	if ( wid < 2 ) 
	{
		return 0;
	}

	lines.clear();
	
	string	ln, hz;
	char* p = (char*) str;
	
	for( ; *p; )
	{
		if ( '\n' == *p) 
		{
			lines.push_back( ln);
			ln = "";
			p++;
			continue;
		}
		if ( *p < 0 )
		{	//汉字
			hz = p[0];	
			hz += p[1];
			p += 2;
		}
		else
		{
			hz = *p++;
		}
		
		if ( (ln.length() + hz.length()) > wid) 
		{
			lines.push_back( ln);
			ln = "";
		}			

		ln += hz;
	}

	if (ln.length()>0)
	{
		lines.push_back(ln);
	}

	return lines.size()*size + (lines.size()-1)*space;
}
	
void IDisplayD3D9::EstimateLine( const char* str, int size, int space, int& width, int& height)
{
	height = EstimateLine( str, 9999, size, space);	//use a huge width!
	
	int wid = lines[0].length();
	
	for(int i = 1; i < lines.size(); i++)
	{
		wid = max( wid, lines[i].length());
	}
	
	width = (wid * size) / 2;
}


DWORD IDisplayD3D9::GetCtrlTextColor( int colorIdx )
{
	assert( colorIdx >= 0 && colorIdx < MAX_TEXT_COLOR);
	return ctrlTextColor_[colorIdx];
}

void IDisplayD3D9::DrawPixels( int ox, int oy, TImage *pimage )
{
	//NO_IMPL
}



//o---------------------------------------------o
//|		create interface						|
//o---------------------------------------------o
IDisplayD3D9* display3D_;
IDisplay* g_pDisplay;

IDisplay* CreateDisplay3D(){
	display3D_ = new IDisplayD3D9;
	g_pDisplay = (IDisplay*) display3D_;
	return display3D_;
}

void DestroyDisplay3D( IDisplay* pDisplay){
	if (pDisplay)
	{
		assert( pDisplay == display3D_);
		display3D_->DestroyObjects();
		delete display3D_;
		display3D_ = NULL;
	}
}


