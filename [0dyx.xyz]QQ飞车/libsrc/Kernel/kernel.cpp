
#include <windows.h>
#include "kernel.h"
#include "k_debug.h"


//add+wangqi
//[2004:11:3] [11:07]
//计算两个矩形的交集
stRect GetIntersectRect(stRect src,stRect dst)
{
	stRect interRect;

	//完全不相交
	if( (dst.ox+dst.width  <=  src.ox) ||
		(dst.oy+dst.height <=  src.oy) ||
		(src.ox+src.width  <= dst.ox) ||
		(src.oy+src.height <= dst.oy) )
	{	
		interRect=stRect(0,0,0,0);
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

		interRect=stRect(xbegin,ybegin,xend-xbegin,yend-ybegin);

	}

	return interRect;
	
}

//add+wangqi
//[2004:11:3] [11:07]
//计算两个矩形的并集
stRect GetUnifyRect(stRect src,stRect dst)
{

	int xbegin=(dst.ox<src.ox)?dst.ox:src.ox;
	int ybegin=(dst.oy<src.oy)?dst.oy:src.oy;

	int xend=(dst.ox+dst.width>src.ox+src.width)?(dst.ox+dst.width):(src.ox+src.width);
	int yend=(dst.oy+dst.height>src.oy+src.height)?(dst.oy+dst.height):(src.oy+src.height);
	
	return stRect(xbegin,ybegin,xend-xbegin,yend-ybegin);

}

//add+wangqi
//[2004:11:3] [11:07]
//计算src相对于dst的关系
int IsRectIntersect(stRect src,stRect dst)
{
	//完全不相交
	if( (dst.ox+dst.width  <=  src.ox) ||
		(dst.oy+dst.height <=  src.oy) ||
		(src.ox+src.width  <= dst.ox) ||
		(src.oy+src.height <= dst.oy) )
		return RECT_NO_INTERSECT;

	//src在dst中
	else if( (dst.ox<=src.ox) && (dst.oy<=src.oy) && 
			 (dst.ox+dst.width>=src.ox+src.width) &&
			 (dst.oy+dst.height>=src.oy+src.height) )
		return RECT_TOTAL_INSIDE;


	//dst在src中
	else if( (src.ox<=dst.ox) && (src.oy<=dst.oy) && 
			 (src.ox+src.width>=dst.ox+dst.width)  &&
			 (src.oy+src.height>=dst.oy+dst.height) )
		 return RECT_TOTAL_OUTSIDE;

	else 
		return RECT_PART_INTERSECT;				

}

void CRefObject::Release()
{
    m_RefCount--;
    assert( m_RefCount >= 0 );

    if( m_RefCount == 0 )
    {
        delete this;
    }
}

void dbgAssert( const char *srcfile, int line )
{
	char buffer[256];
	sprintf( buffer, "assert: file=%s, line=%d", srcfile, line );
	MessageBox( NULL, buffer, "error", MB_OK );
	ReportError( buffer );
}

CBufferQueue::CBufferQueue( int maxbuf ) : m_MaxBuf(maxbuf), m_BeginPos(0), m_EndPos(0)
{
	m_pBuffer = new unsigned char[m_MaxBuf];
}

CBufferQueue::~CBufferQueue()
{
	delete[] m_pBuffer;
}

int CBufferQueue::GetDataLen()  //得到push的数据长度
{
	int len;
	if( m_BeginPos <= m_EndPos ) len = m_EndPos-m_BeginPos;
	else len = m_MaxBuf-m_BeginPos + m_EndPos;
	return len;
}

int CBufferQueue::GetEmptyLen() //得到还可以push多少数据
{
	int capacity;
	if( m_EndPos < m_BeginPos ) capacity = m_BeginPos-1-m_EndPos;
	else capacity = m_MaxBuf-m_EndPos + m_BeginPos-1;

	return capacity;
}

bool CBufferQueue::CanPushIn( int datalen )
{
	return GetEmptyLen()>=datalen;
}

void CBufferQueue::WriteRawData( const void *pdata, int datalen )
{
	if( m_EndPos+datalen >= m_MaxBuf )
	{
		int mysize = m_MaxBuf - m_EndPos;
		int left = datalen - mysize;

		memcpy( &m_pBuffer[m_EndPos], pdata, mysize );
		if( left ) memcpy( &m_pBuffer[0], (unsigned char *)pdata+mysize, left );
		m_EndPos = left;
	}
	else
	{
		memcpy( &m_pBuffer[m_EndPos], pdata, datalen );
		m_EndPos += datalen;
	}
}

void CBufferQueue::ReadRawData( void *pdata, int datalen )
{
	assert( GetDataLen() >= datalen );
	if( m_BeginPos+datalen >= m_MaxBuf )
	{
		assert( m_EndPos < m_BeginPos );
		int mysize = m_MaxBuf - m_BeginPos;
		int left = datalen - mysize;

		memcpy( pdata, &m_pBuffer[m_BeginPos], mysize );
		if( left ) memcpy( (unsigned char *)pdata+mysize, &m_pBuffer[0], left );
		m_BeginPos = left;
		assert( m_BeginPos <= m_EndPos );
	}
	else
	{
		memcpy( pdata, &m_pBuffer[m_BeginPos], datalen );
		m_BeginPos += datalen;
	}
}

const int HEADLEN = sizeof(int);
//如果缓冲区满了， 返回false
bool CBufferQueue::PushData( const void *pdata, int datalen )
{
	if( !CanPushIn(datalen+HEADLEN) ) return false;

	WriteRawData( &datalen, HEADLEN );
	WriteRawData( pdata, datalen );
	return true;
}

int CBufferQueue::PullData( void *pdata, int maxdatalen )
{
	if( m_BeginPos == m_EndPos ) return -1;

	int datalen;
	ReadRawData( &datalen, HEADLEN );
	assert( datalen <= maxdatalen );
	ReadRawData( pdata, datalen );

	return datalen;
}

void CBufferQueue::Clear() //清除所有数据到空
{
	m_BeginPos = m_EndPos = 0;
}
