#pragma once

#include "FBGUISize.h"
//IDirect3DDevice9::SetScissorRect

namespace FBGUI
{
	//! CGUIPoint ��
	/*!
	CGUIPoint �Ƚ����� MFC CPoint, ʵ�����������  mX �� mY ��Ա�ǹ����ģ�����
	CGUIPoint �ṩ��ά�����������ݵĳ�Ա����
	*/
	class CGUIPoint
	{
	public:
		int		mX;
		int		mY;

	public:
		//! CGUIPoint ���캯��, Ĭ�Ϲ���һ�� CGUIPoint( 0, 0 ) ����
		CGUIPoint( ) : mX( 0 ), mY( 0 )						{	}
		//! CGUIPoint ���캯��, ͨ��������������һ�� CGUIPoint ����
		/*! \param vX int ��, ������
		\param vY int ��, ������ */
		CGUIPoint( int vX, int vY ) : mX( vX ), mY( vY )	{	}
	public:
		//! CGUIPoint ���캯��
		~CGUIPoint( )										{	}

	public:
		//! ��Ա���� Offset, �������������ֱ����vXOffset, vYOffset
		/*! \param vX int ��, ������ƫ����
		\param vY int ��, ������ƫ���� */
		void Offset( int vXOffset, int vYOffset )
		{ mX += vXOffset; mY += vYOffset; }

		//! ��Ա���� Offset, �������������ֱ����rPoint.mX, rPoint.mY
		/*! \param rPoint const CGUIPoint������, �洢�˺���������ƫ���� */
		void Offset( const CGUIPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

		//! ��Ա���� Offset, �������������ֱ�ƫ��rSize.mX, rSize.mY */
		/*! \param rSize const CGUISize������, �洢�˺���������ƫ���� */
		void Offset( const CGUISize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }

	public:
		//! != ������, �ж�����CGUIPoint�Ƿ�һ�� */
		/*! \param rPoint const CGUIPoint������
		\return ����ö����rPoint��һ������true, ���򷵻�false */
		bool operator != ( const CGUIPoint& rPoint ) const
		{ return mX != rPoint.mX || mY != rPoint.mY; }

		//! == ������, �ж�����CGUIPoint�Ƿ�һ�� */
		/*! \param rPoint const CGUIPoint������
		\return ����ö����rPointһ������true, ���򷵻�false */
		bool operator == ( const CGUIPoint& rPoint ) const
		{ return mX == rPoint.mX && mY == rPoint.mY; }

		//! += ������, �ò�������һ��CGUISize���� */
		/*! \param rSize const CGUISize������
		\return û�з���ֵ */
		void operator += ( const CGUISize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }

		//! += ������, �ò�������һ��CGUIPoint���� */
		/*! \param rPoint const CGUIPoint������
		\return û�з���ֵ */
		void operator += ( const CGUIPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

		//! -= ������, �ò�����ȥһ��CGUISize���� */
		/*! \param rSize const CGUISize������
		\return û�з���ֵ */
		void operator -= ( const CGUISize& rSize )
		{ mX -= rSize.mCX; mY -= rSize.mCY; }

		//! -= ������, �ò�����ȥһ��CGUIPoint���� */
		/*! \param rPoint const CGUIPoint������
		\return û�з���ֵ */
		void operator -= ( const CGUIPoint& rPoint )
		{ mX -= rPoint.mX; mY -= rPoint.mY; }

		//! - ������, �ò������ظ�CGUIPoint�����ȥһ��CGUIPoint����󣬹����CGUIPoint���� */
		/*! \param rPoint const CGUIPoint������
		\return ���ظ�CGUIPoint�����ȥһ��CGUIPoint����󣬹����CGUIPoint���� */
		CGUIPoint operator - ( const CGUIPoint& rPoint ) const
		{ return CGUIPoint( mX - rPoint.mX, mY - rPoint.mY ); }

		//! - ������, �ò������ظ�CGUIPoint�����ȥһ��CGUISize����󣬹����CGUIPoint����
		/*! \param rSize const CGUISize������
		\return ���ظ�CGUIPoint�����ȥһ��CGUISize����󣬹����CGUIPoint���� */
		CGUIPoint operator - ( const CGUISize& rSize ) const
		{ return CGUIPoint( mX - rSize.mCX, mY - rSize.mCY ); }

		//! - ������, �ò�������CGUIRect�����ȥ��CGUIPoint����󣬹����CGUIRect����
		/*! \param rRect const CGUIRect������
		\return ����CGUIRect�����ȥ��CGUIPoint����󣬹����CGUIRect���� */
//		CGUIRect operator - ( const CGUIRect& rRect ) const;

		//! - ������, �ò������ظ�CGUIPoint����ȡ���󣬹����CGUIPoint����
		/*! \return ���ظ�CGUIPoint����ȡ���󣬹����CGUIPoint���� */
		CGUIPoint operator - ( ) const
		{ return CGUIPoint( -mX, -mY ); }

		//! + ������, �ò������ظ�CGUIPoint�������һ��CGUIPoint����󣬹����CGUIPoint����
		/*! \param rPoint const CGUIPoint������
		\return ���ظ�CGUIPoint�������һ��CGUIPoint����󣬹����CGUIPoint���� */
		CGUIPoint operator + ( const CGUIPoint& rPoint ) const
		{ return CGUIPoint( mX + rPoint.mX, mY + rPoint.mY ); }

		//! + ������, �ò������ظ�CGUIPoint�������һ��CGUISize����󣬹����CGUIPoint����
		/*! \param rSize const CGUISize������
		\return ���ظ�CGUIPoint�������һ��CGUISize����󣬹����CGUIPoint���� */
		CGUIPoint operator + ( const CGUISize& rSize ) const
		{ return CGUIPoint( mX + rSize.mCX, mY + rSize.mCY ); }

		//! + ������, �ò�������CGUIRect������ϸ�CGUIPoint����󣬹����CGUIRect����
		/*! \param rRect const CGUIRect������
		\return ����CGUIRect������ϸ�CGUIPoint����󣬹����CGUIRect���� */
//		CGUIRect operator + ( const CGUIRect& rRect ) const;
	};
}

