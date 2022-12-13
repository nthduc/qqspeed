
#ifndef __KERNEL_H__
#define __KERNEL_H__

#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

#include "profile.h"

#include <vector>
#include <assert.h>
#include <math.h>

#define longlong      __int64
#define ulonglong     unsigned __int64

//#define GAMEVER_PUBLIC  //为外部版本，关闭调试功能
/*
#ifndef GAMEVER_PUBLIC
#undef assert
extern void dbgAssert( const char *srcfile, int line );
#define assert(op) do{ if(!(op)){ dbgAssert( __FILE__, __LINE__ ); __asm{ int 3 }; } }while(0)
#endif
*/
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) do{ if(p){ p->Release(); p=NULL; } }while(0)
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) do{ if(p){ delete p; p=NULL; } }while(0)
#endif


class CRefObject
{
public:
    CRefObject() : m_RefCount(1){}
    void IncRefCount(){ m_RefCount++; }
    int  GetRefCount(){ return m_RefCount; }
	void Release();

	//for debug use
	void DecRefCount(){ m_RefCount--; }

protected:
    virtual ~CRefObject(){;}

private:
	int m_RefCount;
};

struct stViewport
{
	int ox;
	int oy;
	int width;
	int height;
};


enum
{
	
	RECT_NO_INTERSECT	=0,
	RECT_TOTAL_INSIDE	=1,
	RECT_TOTAL_OUTSIDE	=2,
	RECT_PART_INTERSECT	=3,

		
};



struct stRect
{
	int ox;
	int oy;
	int width;
	int height;

	//add+wangqi
	//[2004:3:17] [13:42]
	stRect()
	{
		ox=0;
		oy=0;
		width=0;
		height=0;
		
	}

	//add+wangqi
	//[2004:3:17] [13:42]
	stRect(int x,int y,int w,int h)
	{
		ox=x;
		oy=y;
		width=w;
		height=h;

	}

	//add+wangqi
	//点是否在矩形中,不包含在边界上
	bool IsPointIn(int px,int py)
	{
		return((px>ox)&&(py>oy)&&(px<(ox+width))&&(py<(oy+height)));
	}

	//add+wangqi
	//点是否在矩形中,包含在边界上
	bool IsPointInside(int px,int py)
	{
		return((px>=ox)&&(py>=oy)&&(px<=(ox+width))&&(py<=(oy+height)));
	}

	//add+wangqi
	//[2004:5:8] [15:36]
	bool operator==( const stRect& src) const
	{
		return (ox==src.ox&&oy==src.oy&&width==src.width&&height==src.height);
	}

	//add+wangqi
	//[2004:5:8] [15:36]
	bool operator!=( const stRect& src) const
	{
		return !(ox==src.ox&&oy==src.oy&&width==src.width&&height==src.height);
	}

};


extern int IsRectIntersect(stRect src,stRect dst);
extern stRect GetUnifyRect(stRect src,stRect dst);
extern stRect GetIntersectRect(stRect src,stRect dst);

struct stColorQuad
{
	union
	{
		struct
		{
			unsigned char b;
			unsigned char g;
			unsigned char r;
			unsigned char a;
		};
		unsigned int c;
	};
	
	stColorQuad(){;}
	stColorQuad( unsigned char r1, unsigned char g1, unsigned char b1 )
	{
		r = r1, g = g1, b = b1, a = 255;
	}
	stColorQuad( unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1 )
	{
		r = r1, g = g1, b = b1, a = a1;
	}

	//add+wangqi
	//[2004:3:17] [13:42]
	stColorQuad( int color )
	{
		c=color;
	}

	//add+wangqi
	//[2004:3:26] [9:38]
	//透明度不变,颜色反色
	void Invert()
	{
		r=255-r;
		g=255-g;
		b=255-b;		
	}

	//add+wangqi
	//[2004:5:8] [15:37]
	bool operator==( const stColorQuad& src) const
	{
		return (c==src.c);
	}

	//add+wangqi
	//[2004:5:8] [15:37]
	bool operator!=( const stColorQuad& src) const
	{
		return !(c==src.c);
	}

	
	
};

class Point2D
{

public:

	Point2D(){x=0;y=0;}
	Point2D( int x1, int y1 ) : x(x1), y(y1){}
	Point2D( const Point2D &rhs ) : x(rhs.x), y(rhs.y){}
	Point2D &operator =( const Point2D &rhs ){ x=rhs.x, y=rhs.y; return *this; }
	Point2D operator +( const Point2D &rhs ) const
	{
		return Point2D(x+rhs.x, y+rhs.y);
	}
	Point2D operator -( const Point2D &rhs ) const
	{
		return Point2D(x-rhs.x, y-rhs.y);
	}
	Point2D operator *( const Point2D &rhs ) const
	{
		return Point2D(x*rhs.x, y*rhs.y);
	}
	Point2D operator /( const Point2D &rhs ) const
	{
		return Point2D(x/rhs.x, y/rhs.y);
	}

	Point2D operator *( int scale ) const
	{
		return Point2D(x*scale, y*scale);
	}

	Point2D operator /( int scale ) const
	{
		return Point2D(x/scale, y/scale);
	}

	bool operator==( const Point2D &rhs ) const
	{
		return x==rhs.x && y==rhs.y;
	}

	bool operator!=( const Point2D &rhs ) const
	{
		return x!=rhs.x || y!=rhs.y;
	}

	Point2D &operator +=( const Point2D &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Point2D &operator -=( const Point2D &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Point2D &operator *=( float scale )
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Point2D &operator /=( float scale )
	{
		x /= scale;
		y /= scale;
		return *this;
	}

	int LengthSq(){ return x*x+y*y; }
	int Length(){ return (int)sqrt((float)(x*x+y*y)); }

public:

	int x;
	int y;

};

//add+lincoln
enum
{
	SHAPE_TYPE_RECT		=0,//ox,oy,width,height,type,flag(4+2个参数)
	SHAPE_TYPE_CIRCLE	=1,//ox,oy,width,height,type,flag(4+2个参数)
	SHAPE_TYPE_TRIANGLE	=2,//x0,y0,x1,y1,x2,y2,type,flag(6+2个参数)
	
	//最大的shape参数个数
	SHAPE_PARAM_MAX		=6,

};

struct stShape
{
	bool bFlag;
	int nType;
	int vecParams[SHAPE_PARAM_MAX];

	//add+wangqi
	//[2004:3:17] [13:42]
	stShape()
	{
		bFlag=true;
		nType=SHAPE_TYPE_RECT;

		for(int i=0;i<SHAPE_PARAM_MAX;i++)
		{
			vecParams[i]=0;
		}
		
	}

	stShape(int type,bool flag,int iVars[])
	{

		nType=type;
		bFlag=flag;

		for(int i=0;i<SHAPE_PARAM_MAX;i++)
		{
			vecParams[i]=iVars[i];
		}

	}

	//add+wangqi
	//点是否在矩形中,不包含在边界上
	bool IsPointIn(int px,int py)
	{		

		switch(nType) 
		{
		case SHAPE_TYPE_RECT:
			{
				return ((px>vecParams[0])&&(py>vecParams[1])&&(px<(vecParams[0]+vecParams[2]))&&(py<(vecParams[1]+vecParams[3])));
			}
			break;

		case SHAPE_TYPE_TRIANGLE:
			{
				Point2D n01(vecParams[2]-vecParams[0],vecParams[3]-vecParams[1]);
				Point2D n12(vecParams[4]-vecParams[2],vecParams[5]-vecParams[3]);
				Point2D n20(vecParams[0]-vecParams[4],vecParams[1]-vecParams[5]);

				Point2D n0p(px-vecParams[0],py-vecParams[1]);
				Point2D n1p(px-vecParams[2],py-vecParams[3]);
				Point2D n2p(px-vecParams[4],py-vecParams[5]);

				int na=(n01.x*n0p.y) - (n01.y*n0p.x); 
				int nb=(n12.x*n1p.y) - (n12.y*n1p.x); 
				int nc=(n20.x*n2p.y) - (n20.y*n2p.x); 				

				if(na*nb<0) return false;
				else if(nb*nc<0) return false;
				else if(nc*na<0) return false;
				else  return true;

			}
			break;

		case SHAPE_TYPE_CIRCLE:
			{
				int cx=vecParams[0]+vecParams[2]/2;
				int cy=vecParams[1]+vecParams[3]/2;
				int dx=px-cx;
				int dy=py-cy;
				float an=vecParams[2]*vecParams[2]/4.0f;
				float bn=vecParams[3]*vecParams[3]/4.0f;
				if(an<=0) an=1;
				if(bn<=0) bn=1;
				
				return ((dx*dx/an+dy*dy/bn)<1.0f);
			}
			break;

		default:
			return false;
		}

	}

	//add+wangqi
	//点是否在矩形中,包含在边界上
	bool IsPointInside(int px,int py)
	{

		switch(nType) 
		{
		case SHAPE_TYPE_RECT:
			{
				return ((px>=vecParams[0])&&(py>=vecParams[1])&&(px<=(vecParams[0]+vecParams[2]))&&(py<=(vecParams[1]+vecParams[3])));
			}
			break;
		
		case SHAPE_TYPE_TRIANGLE:
			{
				Point2D n01(vecParams[2]-vecParams[0],vecParams[3]-vecParams[1]);
				Point2D n12(vecParams[4]-vecParams[2],vecParams[5]-vecParams[3]);
				Point2D n20(vecParams[0]-vecParams[4],vecParams[1]-vecParams[5]);

				Point2D n0p(px-vecParams[0],py-vecParams[1]);
				Point2D n1p(px-vecParams[2],py-vecParams[3]);
				Point2D n2p(px-vecParams[4],py-vecParams[5]);

				int na=(n01.x*n0p.y) - (n01.y*n0p.x); 
				int nb=(n12.x*n1p.y) - (n12.y*n1p.x); 
				int nc=(n20.x*n2p.y) - (n20.y*n2p.x); 				

				if(na*nb<0) return false;
				else if(nb*nc<0) return false;
				else if(nc*na<0) return false;
				else return true;

			}
			break;

		case SHAPE_TYPE_CIRCLE:
			{
				int cx=vecParams[0]+vecParams[2]/2;
				int cy=vecParams[1]+vecParams[3]/2;
				int dx=px-cx;
				int dy=py-cy;
				float an=vecParams[2]*vecParams[2]/4.0f;
				float bn=vecParams[3]*vecParams[3]/4.0f;
				if(an<=0) an=1;
				if(bn<=0) bn=1;

				return ((dx*dx/an+dy*dy/bn)<=1.0f);
			}
			break;

		default:
			return false;
		}
	}


};

//add+lincoln
//[2005:5:11] [19:07]
//增加折线段处理功能
class LineSeg2D
{

public:

	std::vector<Point2D> m_vecPoints;


public:

	LineSeg2D(){;}	

	LineSeg2D( const LineSeg2D &src )
	{
		m_vecPoints=src.m_vecPoints;			
	}

	LineSeg2D &operator =( const LineSeg2D &src )
	{ 
		m_vecPoints=src.m_vecPoints;
		return *this; 
	}

	LineSeg2D operator +( const LineSeg2D &src ) const
	{
		LineSeg2D dst(*this);
		dst.m_vecPoints.insert(dst.m_vecPoints.begin(),src.m_vecPoints.begin(),src.m_vecPoints.end());
		return dst;
	}

	LineSeg2D &operator +=( const LineSeg2D &src )
	{
		m_vecPoints.insert(m_vecPoints.begin(),src.m_vecPoints.begin(),src.m_vecPoints.end());
		
		return *this;
	}

	bool operator==( const LineSeg2D &src ) const
	{
		return(m_vecPoints==src.m_vecPoints);
	}

	bool operator!=( const LineSeg2D &src ) const
	{
		return(m_vecPoints!=src.m_vecPoints);
	}

	//pt到seg的水平距离，返回值<0表示没有合理距离
	//假定点pt到折线的水平射线与折线只有一个交点
	int PointSegDisH(Point2D pt)
	{
		
		int dis=-1;

		if(m_vecPoints.size()<2)
			return(dis);

		int x=pt.x;
		int y=pt.y;
		
		for(int i=1;i<m_vecPoints.size();i++)
		{ 
			int x0=m_vecPoints[i-1].x;
			int y0=m_vecPoints[i-1].y;

			int x1=m_vecPoints[i].x;
			int y1=m_vecPoints[i].y;

			
			if((y-y0)*(y1-y)>=0)
			{

				if(y1==y0)
					dis=fabs((float)(x1-x));
				else
					dis=fabs(float((x1-x0)*(y-y0)/(y1-y0)+(x0-x)));

				break;

			}
			

		}


		return dis;

	}

	//pt到seg的垂直距离，返回值<0表示没有合理距离
	//假定点pt到折线的垂直射线与折线只有一个交点
	int PointSegDisV(Point2D pt)
	{

		int dis=-1;

		if(m_vecPoints.size()<2)
			return(dis);

		
		int x=pt.x;
		int y=pt.y;
		
		for(int i=1;i<m_vecPoints.size();i++)
		{ 
			int x0=m_vecPoints[i-1].x;
			int y0=m_vecPoints[i-1].y;

			int x1=m_vecPoints[i].x;
			int y1=m_vecPoints[i].y;

			
			if((x-x0)*(x1-x)>=0)
			{

				if(x1==x0)
					dis=fabs(float(y1-y));
				else
					dis=fabs(float((y1-y0)*(x-x0)/(x1-x0)+(y0-y)));

				break;

			}
			

		}


		return dis;

	}



};


class BitArray1D
{
public:
	BitArray1D( int len ) : m_NumBits(len)
	{
		m_BufLen = (m_NumBits+31)/32;
		m_pData = new unsigned int[m_BufLen];
	}
	~BitArray1D()
	{
		delete[] m_pData;
	}
	void Clear()
	{
		memset( m_pData, 0, m_BufLen*4 );
	}
	int GetBit( int i )
	{
		return (m_pData[i>>5] >> (i&31)) & 1;
	}
	void SetBit( int i, int bit )
	{
		unsigned int &data = m_pData[i>>5];
		int ibit = i&31;
		data = (data & ~(1<<ibit)) | (bit<<ibit);
	}

private:
	unsigned int *m_pData;
	int m_NumBits;
	int m_BufLen;
};

class BitArray2D
{
public:
	BitArray2D( int nbitx, int nbity ) : m_nBitX(nbitx), m_nBitY(nbity), m_BitArray(nbitx*nbity)
	{
	}
	~BitArray2D()
	{
	}
	void Clear()
	{
		m_BitArray.Clear();
	}
	int GetBit( int x, int y )
	{
		return m_BitArray.GetBit( y*m_nBitX+x );
	}
	void SetBit( int x, int y, int bit )
	{
		m_BitArray.SetBit( y*m_nBitX+x, bit );
	}

	int GetWidth(){ return m_nBitX; }
	int GetHeight(){ return m_nBitY; }

private:
	BitArray1D m_BitArray;
	int m_nBitX, m_nBitY;
};

template <typename T> class CSingleton
{
	static T *m_pSingleton;

public:
    CSingleton()
    {
		assert( !m_pSingleton );
        int offset = (int)(T *)1 - (int)(CSingleton<T> *)(T *)1;
        m_pSingleton = (T *)((int)this + offset);
    }

    ~CSingleton()
    {
        assert( m_pSingleton );
        m_pSingleton = 0;
    }

    static T &GetSingleton(void)
    { 
        assert( m_pSingleton );
        return (*m_pSingleton); 
    }

    static T *GetSingletonPtr(void)
    { 
        return (m_pSingleton);
    }
};
template <typename T> T *CSingleton<T>::m_pSingleton = 0;

//元素为大小不等的buffer
class CBufferQueue
{
public:
	CBufferQueue( int maxbuf );
	~CBufferQueue();

	void Clear(); //清除所有数据到空

	//如果缓冲区满了， 返回false
	bool PushData( const void *pdata, int datalen );

	//如果缓冲区空，返回-1, 否则返回数据长度
	int  PullData( void *pdata, int maxdatalen );

private:
	unsigned char *m_pBuffer;
	int m_MaxBuf;
	int m_BeginPos;
	int m_EndPos;

	int GetDataLen();  //得到push的数据长度
	int GetEmptyLen(); //得到还可以push多少数据
	//push datalen这么长的数据，是否会超过缓冲区大小
	bool CanPushIn( int datalen );

	//往缓冲区里面读写数据，必须保证这些操作能成功执行
	void WriteRawData( const void *pdata, int datalen );
	void ReadRawData( void *pdata, int datalen );
};

template<typename T>
inline void Swap( T &t1, T &t2 )
{
	T tmp(t1);
	t1 = t2;
	t2 = tmp;
}

template<typename T>
inline T Max( T t1, T t2 )
{
	return t1>t2 ? t1 : t2;
}

template<typename T>
inline T Min( T t1, T t2 )
{
	return t1<t2 ? t1 : t2;
}

template<typename T>
inline T Clamp( T t1, T t2, T t )
{
	return t<t1 ? t1 : (t>t2? t2:t);
}

struct ChainListNode
{
	ChainListNode *m_pNextNode;
	ChainListNode *m_pPrevNode;
};

template<class T>
class ChainList
{
public:
	ChainList(){ Clear(); }

	//得到当前加入的node数目
	unsigned int GetNumNodes(){ return m_nNodes; }
	bool IsEmpty(){ return m_nNodes==0; }
	void Clear()
	{ 
		m_EndNode.m_pNextNode = m_EndNode.m_pPrevNode = &m_EndNode;
		m_nNodes = 0;
	}

	void PushFront( ChainListNode *pnode ){ InsertAfter(pnode, &m_EndNode); }
	void PushBack( ChainListNode *pnode ){ InsertBefore(pnode, &m_EndNode); }
	void PopFront()
	{
		assert( !IsEmpty() );
		Remove( m_EndNode.m_pNextNode );
	}
	void PopBack()
	{
		assert( !IsEmpty() );
		Remove( m_EndNode.m_pPrevNode );
	}

	bool IsEndNode( ChainListNode *pnode )
	{
		return pnode==&m_EndNode;
	}

	void InsertBefore( ChainListNode *pnode, ChainListNode *pref )
	{
		assert( pnode && pref );

		//这四行的顺序非常重要
		pnode->m_pNextNode = pref;
		pnode->m_pPrevNode = pref->m_pPrevNode;

		pref->m_pPrevNode->m_pNextNode = pnode;
		pref->m_pPrevNode = pnode;

		m_nNodes++;
	}

	void InsertAfter( ChainListNode *pnode, ChainListNode *pref )
	{
		assert( pnode && pref );

		pnode->m_pPrevNode = pref;
		pnode->m_pNextNode = pref->m_pNextNode;

		pref->m_pNextNode->m_pPrevNode = pnode;
		pref->m_pNextNode = pnode;

		m_nNodes++;
	}

	void Remove( ChainListNode *pnode )
	{
		assert( pnode && pnode!=&m_EndNode );

		pnode->m_pNextNode->m_pPrevNode = pnode->m_pPrevNode;
		pnode->m_pPrevNode->m_pNextNode = pnode->m_pNextNode;

		pnode->m_pNextNode = pnode->m_pPrevNode = NULL;

		m_nNodes--;
	}

	bool IsNodeInList( ChainListNode *pnode )
	{
		ChainListNode *pcur = m_EndNode.m_pNextNode;
		while( pcur != &m_EndNode )
		{
			if( pcur == pnode ) return true;
			pcur = pcur->m_pNextNode;
		}
		return false;
	}

	void DeleteAllNodes()
	{
		ChainListNode *pcur = m_EndNode.m_pNextNode;
		while( pcur != &m_EndNode )
		{
			ChainListNode *pnext = pcur->m_pNextNode;
			delete static_cast<T *>(pcur);
			pcur = pnext;
		}
		Clear();
	}
    
	T *BeginIterate(){ return (T *)m_EndNode.m_pNextNode; }
	T *Iterate( ChainListNode *pnode ){ return (T *)pnode->m_pNextNode; }


private:
	ChainListNode m_EndNode;
	unsigned int m_nNodes;
};

// T Y P E D E F S ///////////////////////////////////////////////////////////////////////////////////////////////////
typedef signed char        SCHR;
typedef unsigned char      UCHR;
typedef signed char        SSTR;
typedef unsigned char      USTR;
typedef signed short int   SHINT;
typedef unsigned short int UHINT;
typedef signed long int    SLONG;
typedef signed __int64     SHYPER;
typedef unsigned __int64   UHYPER;

#endif //__KERNEL_H__