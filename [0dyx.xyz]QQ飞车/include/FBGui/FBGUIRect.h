#pragma once
#include "FBGUIPoint.h"


namespace FBGUI
{
	//! CGUIRect 类
	/*!
	CGUIRect 比较类似 MFC CGUIRect, 实现了相对矩形坐标  mTopLeft 和 mBottomRight 成员是公开的，另外
	CGUIRect 提供了维护这两个数据的成员函数, CGUIRect的构造函数默认将调用NormalizeRect, 该函数将使
	得一个CGUIRect对象正常化, 比如:mTopLeft( 10, 10 ) 和 mBottomRight( 20, 20 )是一个正常的矩形, 但
	一个mTopLeft( 20, 20 ) 和 mBottomRight( 10, 10 )是一个不正常的矩形, NormalizeRect将使得这样的矩
	形正常化，一个不正常的矩形在使用某些CGUIRect的成员函数时可能会发生错误！
	*/
	class CGUIRect
	{
	public:
		CGUIPoint	mTopLeft;					//! CGUIPoint 型,矩形的左上角
		CGUIPoint	mBottomRight;				//! CGUIPoint 型,矩形的右下角

	public:
		//! CGUIRect 构造函数, 默认构造一个 CGUIRect( 0, 0, 0, 0 ) 对象
		CGUIRect( )	{ NormalizeRect( ); }

		//! \brief CGUIRect 构造函数, 通过4个整数构造 CGUIRect 对象, 
		/*! \param vLeft int 型 左上角横坐标
		\param vTop int 型 左上角纵坐标
		\param vRight int 型 右下角横坐标
		\param vBottom int 型 右下角纵坐标
		\param vNormalize bool 型 是不是要正常化该 CGUIRect 对象 
		\sa CGUIRect */
		CGUIRect( int vLeft, int vTop, int vRight, int vBottom, bool vNormalize = true ) : mTopLeft( vLeft, vTop ), mBottomRight( vRight, vBottom )
		{ if ( vNormalize ) NormalizeRect( ); }

		//! CGUIRect 构造函数, 通过1个 CGUIPoint 和一个 CGUISize 构造 CGUIRect 对象, 
		/*! \param rPoint const CGUIPoint 型引用 左上角坐标
		\param rSize const CGUISize 型引用 CGUIRect尺寸
		\param vNormalize bool 型 是不是要正常化该 CGUIRect 对象 
		\sa CGUIRect */
		CGUIRect( const CGUIPoint& rPoint, const CGUISize& rSize, bool vNormalize = true ) : mTopLeft( rPoint ), mBottomRight( rPoint + rSize )
		{ if ( vNormalize ) NormalizeRect( ); }

		//! CGUIRect 构造函数, 通过2个 CGUIPoint 构造 CGUIRect 对象, 
		/*! \param rTopLeft const CGUIPoint 型引用 左上角坐标
		\param rBottomRight const CGUIPoint 型引用 右下角坐标
		\param vNormalize bool 型 是不是要正常化该 CGUIRect 对象 
		\sa CGUIRect */
		CGUIRect( const CGUIPoint& rTopLeft, const CGUIPoint& rBottomRight, bool vNormalize = true ) : mTopLeft( rTopLeft ), mBottomRight( rBottomRight )
		{ if ( vNormalize ) NormalizeRect( ); }
	public:
		//! CGUIRect 析造函数
		~CGUIRect( ) {	}

	public:
		//! 成员函数 BottomLeft, 
		/*! \return 返回一个CGUIPoint 的拷贝 */
		CGUIPoint BottomLeft( ) const
		{ return CGUIPoint( mTopLeft.mX, mBottomRight.mY );	}

		//! 成员函数 TopRight, 
		/*! \return 返回一个CGUIPoint 的拷贝 */
		CGUIPoint TopRight( ) const
		{ return CGUIPoint( mBottomRight.mX, mTopLeft.mY );	}

		//! 成员函数 BottomRight, 
		/*! \return 返回一个CGUIPoint 的拷贝 */
		CGUIPoint BottomRight( ) const
		{ return mBottomRight; }

		//! 成员函数 TopLeft, 
		/*! \return 返回一个CGUIPoint 的拷贝 */
		CGUIPoint TopLeft( ) const
		{ return mTopLeft; }

		//! 成员函数 CenterPoint, 
		/*! \return 返回该 CGUIRect 对象的中心点, 一个CGUIPoint 的拷贝 */
		CGUIPoint CenterPoint( ) const
		{ return CGUIPoint( ( mTopLeft.mX + mBottomRight.mX ) >> 1, ( mTopLeft.mY + mBottomRight.mY ) >> 1 ); }

		//! 成员函数 EqualRect, 判断两个对象是否相等
		/*! \param rRect const CGUIRect 型引用
		\return 返回该 CGUIRect 对象和rRect是否相等, 如果相等返回true, 否则返回false */
		bool EqualRect( const CGUIRect& rRect )	const
		{ return mTopLeft == rRect.mTopLeft && mBottomRight == rRect.mBottomRight; }

		//! 成员函数 DeflateRect, 缩小该 CGUIRect 对象, 缩小指的是像中心靠近
		/*! \param vX int 型 mTopLeft的横坐标增加和mBottomRight的横坐标减少量
		\param vY int 型 mTopLeft的纵坐标增加和mBottomRight的纵坐标减少量
		\return 没有返回值 */
		void DeflateRect( int vX, int vY )
		{ mTopLeft.mX += vX; mTopLeft.mY += vY; mBottomRight.mX -= vX; mBottomRight.mY -= vY; }

		//! 成员函数 DeflateRect, 缩小该 CGUIRect 对象, 缩小指的是像中心靠近
		/*! \param rSize const CGUISize 型 rSize的mCX指明了 mTopLeft的横坐标增加和mBottomRight的横坐标减少量
		rSize的mCY指明了 mTopLeft的纵坐标增加和mBottomRight的纵坐标减少量
		\return 没有返回值 */
		void DeflateRect( const CGUISize& rSize )
		{ 
			mTopLeft.mX		+= rSize.mCX; mTopLeft.mY		+= rSize.mCY; 
			mBottomRight.mX	-= rSize.mCX; mBottomRight.mY	-= rSize.mCY; 
		}

		//! 成员函数 DeflateRect, 缩小该 CGUIRect 对象, 缩小指的是像中心靠近
		/*! \param rRect const CGUIRect 型 rRect 指明了每一个边的缩小量
		\return 没有返回值 */
		void DeflateRect( const CGUIRect& rRect )
		{
			mTopLeft.mX		+= rRect.mTopLeft.mX;		mTopLeft.mY		+= rRect.mTopLeft.mY;		
			mBottomRight.mX -= rRect.mBottomRight.mX;	mBottomRight.mY -= rRect.mBottomRight.mY; 
		}

		//! 成员函数 DeflateRect, 缩小该 CGUIRect 对象, 缩小指的是像中心靠近
		/*! \param vLeft int 型 vLeft 指明了 左边 缩小量
		\param vTop int 型 vTop 指明了 上边 缩小量
		\param vRight int 型 vRight 指明了 右边 缩小量
		\param vBottom int 型 vBottom 指明了 下边 缩小量
		\return 没有返回值 */
		void DeflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX += vLeft; mTopLeft.mY += vTop; mBottomRight.mX -= vRight; mBottomRight.mY -= vBottom; }

		//! 成员函数 InflateRect, 扩大该 CGUIRect 对象, 扩大指的是像中心发散
		/*! \param vX int 型 mTopLeft的横坐标减少和mBottomRight的横坐标增加量
		\param vY int 型 mTopLeft的纵坐标减少和mBottomRight的纵坐标增加量
		\return 没有返回值 */
		void InflateRect( int vX, int vY )
		{ mTopLeft.mX -= vX; mTopLeft.mY -= vY; mBottomRight.mX += vX; mBottomRight.mY += vY; }

		//! 成员函数 InflateRect, 扩大该 CGUIRect 对象, 扩大指的是像中心发散
		/*! \param rSize const CGUISize 型 rSize的mCX指明了 mTopLeft的横坐标减少和mBottomRight的横坐标增加量
		rSize的mCY指明了 mTopLeft的纵坐标减少和mBottomRight的纵坐标增加量
		\return 没有返回值 */
		void InflateRect( const CGUISize& rSize )
		{ 
			mTopLeft.mX		-= rSize.mCX;	mTopLeft.mY		-= rSize.mCY;			
			mBottomRight.mX += rSize.mCX;	mBottomRight.mY	+= rSize.mCY; 
		}

		//! 成员函数 InflateRect, 扩大该 CGUIRect 对象, 扩大指的是像中心发散
		/*! \param rRect const CGUIRect 型 rRect 指明了每一个边的扩大量
		\return 没有返回值 */
		void InflateRect( const CGUIRect& rRect )
		{
			mTopLeft.mX		-= rRect.mTopLeft.mX;		mTopLeft.mY		-= rRect.mTopLeft.mY;	
			mBottomRight.mX += rRect.mBottomRight.mX;	mBottomRight.mY += rRect.mBottomRight.mY; 
		}

		//! 成员函数 InflateRect, 扩大该 CGUIRect 对象, 扩大指的是像中心发散
		/*! \param vLeft int 型 vLeft 指明了 左边 扩大量
		\param vTop int 型 vTop 指明了 上边 扩大量
		\param vRight int 型 vRight 指明了 右边 扩大量
		\param vBottom int 型 vBottom 指明了 下边 扩大量
		\return 没有返回值 */
		void InflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX -= vLeft; mTopLeft.mY -= vTop; mBottomRight.mX += vRight; mBottomRight.mY += vBottom; }

		//! 成员函数 Height, 返回该 CGUIRect 对象的高度
		/*! \return 返回该 CGUIRect 对象的高度 */
		int Height( ) const
		{ return mBottomRight.mY - mTopLeft.mY; }

		//! 成员函数 Width, 返回该 CGUIRect 对象的宽度
		/*! \return 返回该 CGUIRect 对象的宽度 */
		int	Width( ) const	
		{ return mBottomRight.mX - mTopLeft.mX; }

		//! 成员函数 IsRectEmpty, 返回该 CGUIRect 对象的宽和高是不是全大于0
		/*! \return 返回该 CGUIRect 对象是否正常 */
		bool IsRectEmpty( ) const
		{ return Width( ) <= 0 || Height( ) <= 0; }

		//! 成员函数 IsRectNull, 返回该 CGUIRect 对象的四个边是否全为0
		/*! \return 返回该 CGUIRect 对象是否为0 */
		bool IsRectNull( ) const
		{ return mTopLeft.mX == 0 && mTopLeft.mY == 0 && mBottomRight.mX == 0 && mBottomRight.mY == 0; }

		//! 成员函数 MoveToX, 对其该 CGUIRect 对象到vX,纵坐标不变
		/*! \param vX int 型 指明对齐的横坐标
		\return 没有返回值 */
		void MoveToX( int vX )	
		{ mBottomRight.mX = vX + Width( ); mTopLeft.mX = vX; }

		//! 成员函数 MoveToY, 对其该 CGUIRect 对象到vY,横坐标不变
		/*! \param vY int 型 指明对齐的纵坐标
		\return 没有返回值 */
		void MoveToY( int vY )	
		{ mBottomRight.mY = vY + Height( ); mTopLeft.mY = vY; }

		//! 成员函数 MoveToXY, 对其该 CGUIRect 对象到vX, vY
		/*! \param vX int 型 指明对齐的横坐标
		\param vY int 型 指明对齐的纵坐标
		\return 没有返回值 */
		void MoveToXY( int vX, int vY )
		{
			mBottomRight.mX = vX + Width( );	mTopLeft.mX	= vX; 
			mBottomRight.mY = vY + Height( ); 	mTopLeft.mY	= vY;  
		}

		//! 成员函数 MoveToXY, 对其该 CGUIRect 对象到rPoint
		/*! \param rPoint const CGUIPoint 型引用 指明对齐的坐标
		\return 没有返回值 */
		void MoveToXY( const CGUIPoint& rPoint )
		{
			mBottomRight.mX = rPoint.mX + Width( ); 	mTopLeft.mX = rPoint.mX; 
			mBottomRight.mY = rPoint.mY + Height( );	mTopLeft.mY = rPoint.mY; 
		}

		//! 成员函数 OffsetRect, 移动该 CGUIRect 对象
		/*! \param vX int 型 指明横坐标的偏移量
		\param vY int 型 指明纵坐标的偏移量
		\return 没有返回值 */
		void OffsetRect( int vX, int vY )					 
		{ *this += CGUIPoint( vX, vY ); }

		//! 成员函数 OffsetRect, 移动该 CGUIRect 对象
		/*! \param rPoint const CGUIPoint 型引用 指明坐标偏移量
		\return 没有返回值 */
		void OffsetRect( const CGUIPoint& rPoint )
		{ *this += rPoint; }

		//! 成员函数 OffsetRect, 移动该 CGUIRect 对象
		/*! \param rSize const CGUISize 型引用 指明坐标偏移量
		\return 没有返回值 */
		void OffsetRect( const CGUISize& rSize )				 
		{ *this += rSize; }	

		//! 成员函数 SetRect, 设置 CGUIRect 对象
		/*! \param vLeft int 型 CGUIRect对象的左上角横坐标
		\param vTop int 型 CGUIRect对象的左上角纵坐标
		\param vRight int 型 CGUIRect对象的右下角横坐标
		\param vBottom int 型 CGUIRect对象的右下角纵坐标
		\return 没有返回值 */
		void SetRect( int vLeft, int vTop, int vRight, int vBottom )
		{
			mTopLeft.mX		= vLeft;	mTopLeft.mY		= vTop; 
			mBottomRight.mX = vRight;	mBottomRight.mY = vBottom; 
		}

		//! 成员函数 SetRect, 通过1个 CGUIPoint 和1个 CGUISize 设置 CGUIRect 对象, 
		/*! \param rPoint const CGUIPoint 型引用 左上角坐标
		\param rSize const CGUISize 型引用 CGUIRect尺寸	
		\return 没有返回值 */
		void SetRect( const CGUIPoint& rPoint, const CGUISize& rSize )
		{ mTopLeft = rPoint; mBottomRight = rPoint + rSize; }

		//! 成员函数 SetRect, 通过2个 CGUIPoint 设置 CGUIRect 对象, 
		/*! \param rTopLeft const CGUIPoint 型引用 左上角坐标
		\param rBottomRight const CGUIPoint 型引用 右下角坐标
		\return 没有返回值 */
		void SetRect( const CGUIPoint& rTopLeft, const CGUIPoint& rBottomRight )
		{ mTopLeft = rTopLeft; mBottomRight	= rBottomRight; }

		//! 成员函数 SetRectEmpty, 将该 CGUIRect 对象置0, 
		/*! \return 没有返回值 */
		void SetRectEmpty( )
		{ mTopLeft.mX = 0; mTopLeft.mY = 0; mBottomRight.mX = 0; mBottomRight.mY = 0; }

		//! 成员函数 Size, 返回该 CGUIRect 对象的宽度和高度
		/*! \return 返回该 CGUIRect 对象的宽度和高度 */
		CGUISize Size( ) const
		{ return CGUISize( Width( ), Height( ) ); }

		//! 成员函数 NormalizeRect, 正常化该 CGUIRect 对象
		/*! \return 没有返回值 */
		void NormalizeRect( );

		//! 成员函数 PtInRect, 判断指定坐标是否在该 CGUIRect 对象中
		/*! \param rPoint const CGUIPoint型引用 待判断的坐标
		\return 返回指定坐标是不是在该 CGUIRect 对象中 */
		bool PtInRect( const CGUIPoint&	rPoint ) const;

		//! 成员函数 IntersectRect, 计算两个 CGUIRect 对象的交集
		/*! \brief 如果计算产生的矩形不正常, 那么该矩形将被清零
		\brief eg. CGUIRect( ).( CGUIRect( 0, 0, 10, 10 ), CGUIRect( 5, 5, 15, 15 ) ) == CGUIRect( 5, 5, 10, 10 )
		\param const CGUIRect& rSrcRect1 源矩形
		\param const CGUIRect& rSrcRect2 源矩形
		\return 如果传入的两个源矩形或是计算后产生的矩形不正常，将返回false, 否这返回true */
		bool IntersectRect( const CGUIRect& rSrcRect1, const CGUIRect& rSrcRect2 );

		//! 成员函数 UnionRect, 计算两个 CGUIRect 对象的并集
		/*! \brief 如果传入的两个源矩形有一个不正常, 函数将返回false
		\brief eg. CGUIRect( ).( CGUIRect( 0, 0, 10, 10 ), CGUIRect( 5, 5, 15, 15 ) ) == CGUIRect( 0, 0, 15, 15 )
		\param const CGUIRect& rSrcRect1 源矩形
		\param const CGUIRect& rSrcRect2 源矩形
		\return 如果传入的两个源矩形有一个不正常, 函数将返回false, 否这返回true */
		bool UnionRect( const CGUIRect& rSrcRectl, const CGUIRect& rSrcRect2 );
	public:
		//! != 操作符, 判断两个 CGUIRect 对象是否不相等
		/*! \param const CGUIRect& rRect 
		\return 返回该 CGUIRect 对象和rRect是否相等, 如果不相等返回true, 否则返回false */
		bool operator != ( const CGUIRect& rRect ) const
		{ return !EqualRect( rRect ); }

		//! == 操作符, 判断两个 CGUIRect 对象是否相等
		/*! \param const CGUIRect& rRect 
		\return 返回该 CGUIRect 对象和rRect是否相等, 如果相等返回true, 否则返回false */
		bool operator == ( const CGUIRect& rRect ) const
		{ return EqualRect( rRect ); }

		//! &= 操作符, 计算该 CGUIRect 对象和 指定 CGUIRect 对象的交集
		/*! \param const CGUIRect& rRect 
		\return 没有返回值 */
		void operator &= ( const CGUIRect& rRect )
		{ IntersectRect( *this, rRect ); }

		//! |= 操作符, 计算该 CGUIRect 对象和 指定 CGUIRect 对象的并集
		/*! \param const CGUIRect& rRect 
		\return 没有返回值 */
		void operator |= ( const CGUIRect& rRect )
		{ UnionRect( *this, rRect ); }

		//! & 操作符, 计算该 CGUIRect 对象和 指定 CGUIRect 对象的交集
		/*! \param const CGUIRect& rRect 
		\return 计算该 CGUIRect 对象和 指定 CGUIRect 对象的并集, 返回 CGUIRect 的拷贝  */
		CGUIRect operator & ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult;
			tRectResult.IntersectRect( *this, rRect );
			return tRectResult;
		}

		//! | 操作符, 计算该 CGUIRect 对象和 指定 CGUIRect 对象的并集
		/*! \param const CGUIRect& rRect 
		\return 计算该 CGUIRect 对象和 指定 CGUIRect 对象的并集, 返回 CGUIRect 的拷贝  */
		CGUIRect operator | ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult;
			tRectResult.UnionRect( *this, rRect );
			return tRectResult;
		}

		//! - 操作符, 把该 CGUIRect 反向偏移 rPoint 个单位
		/*! \param const CGUIPoint& rPoint
		\return 把该 CGUIRect 反向偏移 rPoint 个单位, 返回 CGUIRect 的拷贝  */
		CGUIRect operator - ( const CGUIPoint& rPoint ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( -rPoint );
			return tRectResult;
		}

		//! - 操作符, 把该 CGUIRect 反向偏移 rSize 个单位
		/*! \param const CGUISize& rSize
		\return 把该 CGUIRect 反向偏移 rSize 个单位, 返回 CGUIRect 的拷贝  */
		CGUIRect operator - ( const CGUISize& rSize ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( -rSize );
			return tRectResult;
		}

		//! - 操作符, 缩小该 CGUIRect 对象，缩小的尺寸由 rRect 指定
		/*! \param const CGUIRect& rRect
		\return 缩小该 CGUIRect 对象，缩小的尺寸由 rRect 指定, 返回 CGUIRect 的拷贝  */
		CGUIRect operator - ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.DeflateRect( rRect );
			return tRectResult;
		}

		//! + 操作符, 把该 CGUIRect 正向偏移 rPoint 个单位
		/*! \param const CGUIPoint& rPoint
		\return 把该 CGUIRect 正向偏移 rPoint 个单位, 返回 CGUIRect 的拷贝  */
		CGUIRect operator + ( const CGUIPoint& rPoint ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( rPoint );
			return tRectResult;
		}

		//! + 操作符, 把该 CGUIRect 正向偏移 rSize 个单位
		/*! \param const CGUISize& rSize
		\return 把该 CGUIRect 正向偏移 rSize 个单位, 返回 CGUIRect 的拷贝  */
		CGUIRect operator + ( const CGUISize& rSize ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( rSize );
			return tRectResult;
		}

		//! + 操作符, 扩大该 CGUIRect 对象，扩大的尺寸由 rRect 指定
		/*! \param const CGUIRect& rRect
		\return 扩大该 CGUIRect 对象，扩大的尺寸由 rRect 指定, 返回 CGUIRect 的拷贝  */
		CGUIRect operator + ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.InflateRect( rRect );
			return tRectResult;
		}

		//! += 操作符, 把该 CGUIRect 正向偏移 rPoint 个单位
		/*! \param const CGUIPoint& rPoint
		\return 没有返回值  */
		void operator += ( const CGUIPoint& rPoint )
		{ mTopLeft += rPoint; mBottomRight += rPoint; }

		//! += 操作符, 把该 CGUIRect 正向偏移 rSize 个单位
		/*! \param const CGUISize& rSize
		\return 没有返回值  */
		void operator += ( const CGUISize& rSize )
		{ mTopLeft += rSize; mBottomRight += rSize; }

		//! += 操作符, 扩大该 CGUIRect 对象，扩大的尺寸由 rRect 指定
		/*! \param const CGUIRect& rRect
		\return 没有返回值  */
		void operator += ( const CGUIRect& rRect )
		{ InflateRect( rRect );	}

		//! += 操作符, 把该 CGUIRect 反向偏移 rPoint 个单位
		/*! \param const CGUIPoint& rPoint
		\return 没有返回值  */
		void operator -= ( const CGUIPoint& rPoint )
		{ mTopLeft -= rPoint; mBottomRight -= rPoint; }

		//! += 操作符, 把该 CGUIRect 反向偏移 rSize 个单位
		/*! \param const CGUISize& rSize
		\return 没有返回值  */
		void operator -= ( const CGUISize& rSize )
		{ mTopLeft -= rSize; mBottomRight -= rSize;	}

		//! += 操作符, 缩小该 CGUIRect 对象，缩小的尺寸由 rRect 指定
		/*! \param const CGUIRect& rRect
		\return 没有返回值  */
		void operator -= ( const CGUIRect& rRect )
		{ DeflateRect( rRect );	}
	};
}