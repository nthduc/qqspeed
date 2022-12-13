#pragma once
#include <Windows.h>
#include <d3dx9.h>



namespace FBGUI
{
	class CGUIPoint ;
	class CGUIRect  ;
	//! CGUISize 类
	/*!
		CGUISize 比较类似 MFC CSize, 实现了相对坐标  mCX 和 mCY 成员是公开的，另外
		CGUISize 提供了维护这两个数据的成员函数
	*/
	class CGUISize
	{
	public:
		int	mCX;			//! 横坐标
		int	mCY;			//! 纵坐标

	public:
		//! CGUISize 构造函数, 默认构造一个 CGUISize( 0, 0 ) 对象
		CGUISize( ) : mCX( 0 ), mCY( 0 )						{	}

		//! CGUISize 构造函数, 通过两个整数构造一个 CGUISize 对象
		/*! \param int vCX 横坐标
			\param int vCY 纵坐标 */
		CGUISize( int vCX, int vCY ) : mCX( vCX ), mCY( vCY )	{	}

	public:
		//! CGUISize 析造函数
		~CGUISize( )											{	}

	public:
		//! != 操作符, 判断两个CGUISize是否不一样
		/*! \param const CGUISize& rSize  对象引用
			\return 如果该对象和rSize不一样返回true, 否则返回false */
		bool operator != ( const CGUISize& rSize ) const			
			{ return mCX != rSize.mCX || mCY != rSize.mCY; }

		//! == 操作符, 判断两个CGUISize是否一样
		/*! \param rSize CGUISize 对象引用
			\return 如果该对象和rSize一样返回true, 否则返回false */
		bool operator == ( const CGUISize& rSize ) const			
			{ return mCX == rSize.mCX && mCY == rSize.mCY; }

		//! -= 操作符, 该CGUISize对象减去一个CGUISize对象
		/*! \param rSize CGUISize 对象引用
			\return 没有返回值 */
		void operator -= ( const CGUISize& rSize )
			{ mCX -= rSize.mCX; mCY -= rSize.mCY; }

		//! += 操作符, 该CGUISize对象加上一个rSize对象
		/*! \param rSize CGUISize 对象引用
			\return 没有返回值 */
		void operator += ( const CGUISize& rSize )
			{ mCX += rSize.mCX; mCY += rSize.mCY; }

		//! - 操作符, 这个操作返回rPoint的值减去该CGUISize对象的值
		/*! \param rSize CGUIPoint 对象引用
			\return 返回rPoint的值减去该CGUISize对象的值 */
		CGUIPoint operator - ( const CGUIPoint& rPoint ) const;

		//! - 操作符, 这个操作返回rRect的值减去该CGUISize对象的值
		/*! \param rRect CGUIRect 对象引用
			\return 返回rRect的值减去该CGUISize对象后，构造的CGUIRect拷贝 */
		CGUIRect operator - ( const CGUIRect& rRect ) const;
			
		//! - 操作符, 这个操作返回该CGUISize对象的值减去CGUISize对象后，构造的CGUISize拷贝 */
		/*! \param rSize CGUISize 对象引用
			\return 返回该CGUISize对象的值减去 rSize的值 */
		CGUISize operator - ( const CGUISize& rSize ) const
			{ return CGUISize( mCX - rSize.mCX, mCY - rSize.mCY ); }

		//! - 操作符, 这个操作返回该CGUISize对象的取反后，构造的CGUISize拷贝
		/*! \return 返回该CGUISize对象取反后的值 */
		CGUISize operator - ( ) const
			{ return CGUISize( -mCX, -mCY ); }

		//! + 操作符, 这个操作返回rPoint的值加上该CGUISize对象的值
		/*! \param rPoint CGUIPoint 对象引用
			\return 返回rPoint的值加上该CGUISize对象后，构造的CGUIPoint拷贝 */
		CGUIPoint operator + ( const CGUIPoint& rPoint ) const;

		//! + 操作符, 这个操作返回rRect的值加上该CGUISize对象的值
		/*! \param rRect CGUIRect 对象引用
			\return 返回rRect的值加上该CGUISize对象后，构造的CGUIRect拷贝 */
		CGUIRect operator + ( const CGUIRect& rRect ) const;

		//! + 操作符, 这个操作返回该CGUISize对象的值加上rSize后，构造的CGUISize拷贝
		/*! \param rSize CGUISize 对象引用
			\return 返回该CGUISize对象加上一个rSize对象后，构造的CGUISize拷贝 */
		CGUISize operator + ( const CGUISize& rSize ) const
			{ return CGUISize( mCX + rSize.mCX, mCY + rSize.mCY ); }
	};







};