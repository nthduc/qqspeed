#pragma once
#include <Windows.h>
#include <d3dx9.h>



namespace FBGUI
{
	class CGUIPoint ;
	class CGUIRect  ;
	//! CGUISize ��
	/*!
		CGUISize �Ƚ����� MFC CSize, ʵ�����������  mCX �� mCY ��Ա�ǹ����ģ�����
		CGUISize �ṩ��ά�����������ݵĳ�Ա����
	*/
	class CGUISize
	{
	public:
		int	mCX;			//! ������
		int	mCY;			//! ������

	public:
		//! CGUISize ���캯��, Ĭ�Ϲ���һ�� CGUISize( 0, 0 ) ����
		CGUISize( ) : mCX( 0 ), mCY( 0 )						{	}

		//! CGUISize ���캯��, ͨ��������������һ�� CGUISize ����
		/*! \param int vCX ������
			\param int vCY ������ */
		CGUISize( int vCX, int vCY ) : mCX( vCX ), mCY( vCY )	{	}

	public:
		//! CGUISize ���캯��
		~CGUISize( )											{	}

	public:
		//! != ������, �ж�����CGUISize�Ƿ�һ��
		/*! \param const CGUISize& rSize  ��������
			\return ����ö����rSize��һ������true, ���򷵻�false */
		bool operator != ( const CGUISize& rSize ) const			
			{ return mCX != rSize.mCX || mCY != rSize.mCY; }

		//! == ������, �ж�����CGUISize�Ƿ�һ��
		/*! \param rSize CGUISize ��������
			\return ����ö����rSizeһ������true, ���򷵻�false */
		bool operator == ( const CGUISize& rSize ) const			
			{ return mCX == rSize.mCX && mCY == rSize.mCY; }

		//! -= ������, ��CGUISize�����ȥһ��CGUISize����
		/*! \param rSize CGUISize ��������
			\return û�з���ֵ */
		void operator -= ( const CGUISize& rSize )
			{ mCX -= rSize.mCX; mCY -= rSize.mCY; }

		//! += ������, ��CGUISize�������һ��rSize����
		/*! \param rSize CGUISize ��������
			\return û�з���ֵ */
		void operator += ( const CGUISize& rSize )
			{ mCX += rSize.mCX; mCY += rSize.mCY; }

		//! - ������, �����������rPoint��ֵ��ȥ��CGUISize�����ֵ
		/*! \param rSize CGUIPoint ��������
			\return ����rPoint��ֵ��ȥ��CGUISize�����ֵ */
		CGUIPoint operator - ( const CGUIPoint& rPoint ) const;

		//! - ������, �����������rRect��ֵ��ȥ��CGUISize�����ֵ
		/*! \param rRect CGUIRect ��������
			\return ����rRect��ֵ��ȥ��CGUISize����󣬹����CGUIRect���� */
		CGUIRect operator - ( const CGUIRect& rRect ) const;
			
		//! - ������, ����������ظ�CGUISize�����ֵ��ȥCGUISize����󣬹����CGUISize���� */
		/*! \param rSize CGUISize ��������
			\return ���ظ�CGUISize�����ֵ��ȥ rSize��ֵ */
		CGUISize operator - ( const CGUISize& rSize ) const
			{ return CGUISize( mCX - rSize.mCX, mCY - rSize.mCY ); }

		//! - ������, ����������ظ�CGUISize�����ȡ���󣬹����CGUISize����
		/*! \return ���ظ�CGUISize����ȡ�����ֵ */
		CGUISize operator - ( ) const
			{ return CGUISize( -mCX, -mCY ); }

		//! + ������, �����������rPoint��ֵ���ϸ�CGUISize�����ֵ
		/*! \param rPoint CGUIPoint ��������
			\return ����rPoint��ֵ���ϸ�CGUISize����󣬹����CGUIPoint���� */
		CGUIPoint operator + ( const CGUIPoint& rPoint ) const;

		//! + ������, �����������rRect��ֵ���ϸ�CGUISize�����ֵ
		/*! \param rRect CGUIRect ��������
			\return ����rRect��ֵ���ϸ�CGUISize����󣬹����CGUIRect���� */
		CGUIRect operator + ( const CGUIRect& rRect ) const;

		//! + ������, ����������ظ�CGUISize�����ֵ����rSize�󣬹����CGUISize����
		/*! \param rSize CGUISize ��������
			\return ���ظ�CGUISize�������һ��rSize����󣬹����CGUISize���� */
		CGUISize operator + ( const CGUISize& rSize ) const
			{ return CGUISize( mCX + rSize.mCX, mCY + rSize.mCY ); }
	};







};