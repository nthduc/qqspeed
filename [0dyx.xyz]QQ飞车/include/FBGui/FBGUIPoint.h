#pragma once

#include "FBGUISize.h"
//IDirect3DDevice9::SetScissorRect

namespace FBGUI
{
	//! CGUIPoint 类
	/*!
	CGUIPoint 比较类似 MFC CPoint, 实现了相对坐标  mX 和 mY 成员是公开的，另外
	CGUIPoint 提供了维护这两个数据的成员函数
	*/
	class CGUIPoint
	{
	public:
		int		mX;
		int		mY;

	public:
		//! CGUIPoint 构造函数, 默认构造一个 CGUIPoint( 0, 0 ) 对象
		CGUIPoint( ) : mX( 0 ), mY( 0 )						{	}
		//! CGUIPoint 构造函数, 通过两个整数构造一个 CGUIPoint 对象
		/*! \param vX int 型, 横坐标
		\param vY int 型, 纵坐标 */
		CGUIPoint( int vX, int vY ) : mX( vX ), mY( vY )	{	}
	public:
		//! CGUIPoint 析造函数
		~CGUIPoint( )										{	}

	public:
		//! 成员函数 Offset, 横坐标和纵坐标分别加上vXOffset, vYOffset
		/*! \param vX int 型, 横坐标偏移量
		\param vY int 型, 纵坐标偏移量 */
		void Offset( int vXOffset, int vYOffset )
		{ mX += vXOffset; mY += vYOffset; }

		//! 成员函数 Offset, 横坐标和纵坐标分别加上rPoint.mX, rPoint.mY
		/*! \param rPoint const CGUIPoint型引用, 存储了横向和纵向的偏移量 */
		void Offset( const CGUIPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

		//! 成员函数 Offset, 横坐标和纵坐标分别偏移rSize.mX, rSize.mY */
		/*! \param rSize const CGUISize型引用, 存储了横向和纵向的偏移量 */
		void Offset( const CGUISize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }

	public:
		//! != 操作符, 判断两个CGUIPoint是否不一样 */
		/*! \param rPoint const CGUIPoint型引用
		\return 如果该对象和rPoint不一样返回true, 否则返回false */
		bool operator != ( const CGUIPoint& rPoint ) const
		{ return mX != rPoint.mX || mY != rPoint.mY; }

		//! == 操作符, 判断两个CGUIPoint是否一样 */
		/*! \param rPoint const CGUIPoint型引用
		\return 如果该对象和rPoint一样返回true, 否则返回false */
		bool operator == ( const CGUIPoint& rPoint ) const
		{ return mX == rPoint.mX && mY == rPoint.mY; }

		//! += 操作符, 该操作加上一个CGUISize对象 */
		/*! \param rSize const CGUISize型引用
		\return 没有返回值 */
		void operator += ( const CGUISize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }

		//! += 操作符, 该操作加上一个CGUIPoint对象 */
		/*! \param rPoint const CGUIPoint型引用
		\return 没有返回值 */
		void operator += ( const CGUIPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

		//! -= 操作符, 该操作减去一个CGUISize对象 */
		/*! \param rSize const CGUISize型引用
		\return 没有返回值 */
		void operator -= ( const CGUISize& rSize )
		{ mX -= rSize.mCX; mY -= rSize.mCY; }

		//! -= 操作符, 该操作减去一个CGUIPoint对象 */
		/*! \param rPoint const CGUIPoint型引用
		\return 没有返回值 */
		void operator -= ( const CGUIPoint& rPoint )
		{ mX -= rPoint.mX; mY -= rPoint.mY; }

		//! - 操作符, 该操作返回该CGUIPoint对象减去一个CGUIPoint对象后，构造的CGUIPoint拷贝 */
		/*! \param rPoint const CGUIPoint型引用
		\return 返回该CGUIPoint对象减去一个CGUIPoint对象后，构造的CGUIPoint拷贝 */
		CGUIPoint operator - ( const CGUIPoint& rPoint ) const
		{ return CGUIPoint( mX - rPoint.mX, mY - rPoint.mY ); }

		//! - 操作符, 该操作返回该CGUIPoint对象减去一个CGUISize对象后，构造的CGUIPoint拷贝
		/*! \param rSize const CGUISize型引用
		\return 返回该CGUIPoint对象减去一个CGUISize对象后，构造的CGUIPoint拷贝 */
		CGUIPoint operator - ( const CGUISize& rSize ) const
		{ return CGUIPoint( mX - rSize.mCX, mY - rSize.mCY ); }

		//! - 操作符, 该操作返回CGUIRect对象减去该CGUIPoint对象后，构造的CGUIRect拷贝
		/*! \param rRect const CGUIRect型引用
		\return 返回CGUIRect对象减去该CGUIPoint对象后，构造的CGUIRect拷贝 */
//		CGUIRect operator - ( const CGUIRect& rRect ) const;

		//! - 操作符, 该操作返回该CGUIPoint对象取反后，构造的CGUIPoint拷贝
		/*! \return 返回该CGUIPoint对象取反后，构造的CGUIPoint拷贝 */
		CGUIPoint operator - ( ) const
		{ return CGUIPoint( -mX, -mY ); }

		//! + 操作符, 该操作返回该CGUIPoint对象加上一个CGUIPoint对象后，构造的CGUIPoint拷贝
		/*! \param rPoint const CGUIPoint型引用
		\return 返回该CGUIPoint对象加上一个CGUIPoint对象后，构造的CGUIPoint拷贝 */
		CGUIPoint operator + ( const CGUIPoint& rPoint ) const
		{ return CGUIPoint( mX + rPoint.mX, mY + rPoint.mY ); }

		//! + 操作符, 该操作返回该CGUIPoint对象加上一个CGUISize对象后，构造的CGUIPoint拷贝
		/*! \param rSize const CGUISize型引用
		\return 返回该CGUIPoint对象加上一个CGUISize对象后，构造的CGUIPoint拷贝 */
		CGUIPoint operator + ( const CGUISize& rSize ) const
		{ return CGUIPoint( mX + rSize.mCX, mY + rSize.mCY ); }

		//! + 操作符, 该操作返回CGUIRect对象加上该CGUIPoint对象后，构造的CGUIRect拷贝
		/*! \param rRect const CGUIRect型引用
		\return 返回CGUIRect对象加上该CGUIPoint对象后，构造的CGUIRect拷贝 */
//		CGUIRect operator + ( const CGUIRect& rRect ) const;
	};
}

