#pragma once
#include "FBGUIPoint.h"


namespace FBGUI
{
	//! CGUIRect ��
	/*!
	CGUIRect �Ƚ����� MFC CGUIRect, ʵ������Ծ�������  mTopLeft �� mBottomRight ��Ա�ǹ����ģ�����
	CGUIRect �ṩ��ά�����������ݵĳ�Ա����, CGUIRect�Ĺ��캯��Ĭ�Ͻ�����NormalizeRect, �ú�����ʹ
	��һ��CGUIRect����������, ����:mTopLeft( 10, 10 ) �� mBottomRight( 20, 20 )��һ�������ľ���, ��
	һ��mTopLeft( 20, 20 ) �� mBottomRight( 10, 10 )��һ���������ľ���, NormalizeRect��ʹ�������ľ�
	����������һ���������ľ�����ʹ��ĳЩCGUIRect�ĳ�Ա����ʱ���ܻᷢ������
	*/
	class CGUIRect
	{
	public:
		CGUIPoint	mTopLeft;					//! CGUIPoint ��,���ε����Ͻ�
		CGUIPoint	mBottomRight;				//! CGUIPoint ��,���ε����½�

	public:
		//! CGUIRect ���캯��, Ĭ�Ϲ���һ�� CGUIRect( 0, 0, 0, 0 ) ����
		CGUIRect( )	{ NormalizeRect( ); }

		//! \brief CGUIRect ���캯��, ͨ��4���������� CGUIRect ����, 
		/*! \param vLeft int �� ���ϽǺ�����
		\param vTop int �� ���Ͻ�������
		\param vRight int �� ���½Ǻ�����
		\param vBottom int �� ���½�������
		\param vNormalize bool �� �ǲ���Ҫ�������� CGUIRect ���� 
		\sa CGUIRect */
		CGUIRect( int vLeft, int vTop, int vRight, int vBottom, bool vNormalize = true ) : mTopLeft( vLeft, vTop ), mBottomRight( vRight, vBottom )
		{ if ( vNormalize ) NormalizeRect( ); }

		//! CGUIRect ���캯��, ͨ��1�� CGUIPoint ��һ�� CGUISize ���� CGUIRect ����, 
		/*! \param rPoint const CGUIPoint ������ ���Ͻ�����
		\param rSize const CGUISize ������ CGUIRect�ߴ�
		\param vNormalize bool �� �ǲ���Ҫ�������� CGUIRect ���� 
		\sa CGUIRect */
		CGUIRect( const CGUIPoint& rPoint, const CGUISize& rSize, bool vNormalize = true ) : mTopLeft( rPoint ), mBottomRight( rPoint + rSize )
		{ if ( vNormalize ) NormalizeRect( ); }

		//! CGUIRect ���캯��, ͨ��2�� CGUIPoint ���� CGUIRect ����, 
		/*! \param rTopLeft const CGUIPoint ������ ���Ͻ�����
		\param rBottomRight const CGUIPoint ������ ���½�����
		\param vNormalize bool �� �ǲ���Ҫ�������� CGUIRect ���� 
		\sa CGUIRect */
		CGUIRect( const CGUIPoint& rTopLeft, const CGUIPoint& rBottomRight, bool vNormalize = true ) : mTopLeft( rTopLeft ), mBottomRight( rBottomRight )
		{ if ( vNormalize ) NormalizeRect( ); }
	public:
		//! CGUIRect ���캯��
		~CGUIRect( ) {	}

	public:
		//! ��Ա���� BottomLeft, 
		/*! \return ����һ��CGUIPoint �Ŀ��� */
		CGUIPoint BottomLeft( ) const
		{ return CGUIPoint( mTopLeft.mX, mBottomRight.mY );	}

		//! ��Ա���� TopRight, 
		/*! \return ����һ��CGUIPoint �Ŀ��� */
		CGUIPoint TopRight( ) const
		{ return CGUIPoint( mBottomRight.mX, mTopLeft.mY );	}

		//! ��Ա���� BottomRight, 
		/*! \return ����һ��CGUIPoint �Ŀ��� */
		CGUIPoint BottomRight( ) const
		{ return mBottomRight; }

		//! ��Ա���� TopLeft, 
		/*! \return ����һ��CGUIPoint �Ŀ��� */
		CGUIPoint TopLeft( ) const
		{ return mTopLeft; }

		//! ��Ա���� CenterPoint, 
		/*! \return ���ظ� CGUIRect ��������ĵ�, һ��CGUIPoint �Ŀ��� */
		CGUIPoint CenterPoint( ) const
		{ return CGUIPoint( ( mTopLeft.mX + mBottomRight.mX ) >> 1, ( mTopLeft.mY + mBottomRight.mY ) >> 1 ); }

		//! ��Ա���� EqualRect, �ж����������Ƿ����
		/*! \param rRect const CGUIRect ������
		\return ���ظ� CGUIRect �����rRect�Ƿ����, �����ȷ���true, ���򷵻�false */
		bool EqualRect( const CGUIRect& rRect )	const
		{ return mTopLeft == rRect.mTopLeft && mBottomRight == rRect.mBottomRight; }

		//! ��Ա���� DeflateRect, ��С�� CGUIRect ����, ��Сָ���������Ŀ���
		/*! \param vX int �� mTopLeft�ĺ��������Ӻ�mBottomRight�ĺ����������
		\param vY int �� mTopLeft�����������Ӻ�mBottomRight�������������
		\return û�з���ֵ */
		void DeflateRect( int vX, int vY )
		{ mTopLeft.mX += vX; mTopLeft.mY += vY; mBottomRight.mX -= vX; mBottomRight.mY -= vY; }

		//! ��Ա���� DeflateRect, ��С�� CGUIRect ����, ��Сָ���������Ŀ���
		/*! \param rSize const CGUISize �� rSize��mCXָ���� mTopLeft�ĺ��������Ӻ�mBottomRight�ĺ����������
		rSize��mCYָ���� mTopLeft�����������Ӻ�mBottomRight�������������
		\return û�з���ֵ */
		void DeflateRect( const CGUISize& rSize )
		{ 
			mTopLeft.mX		+= rSize.mCX; mTopLeft.mY		+= rSize.mCY; 
			mBottomRight.mX	-= rSize.mCX; mBottomRight.mY	-= rSize.mCY; 
		}

		//! ��Ա���� DeflateRect, ��С�� CGUIRect ����, ��Сָ���������Ŀ���
		/*! \param rRect const CGUIRect �� rRect ָ����ÿһ���ߵ���С��
		\return û�з���ֵ */
		void DeflateRect( const CGUIRect& rRect )
		{
			mTopLeft.mX		+= rRect.mTopLeft.mX;		mTopLeft.mY		+= rRect.mTopLeft.mY;		
			mBottomRight.mX -= rRect.mBottomRight.mX;	mBottomRight.mY -= rRect.mBottomRight.mY; 
		}

		//! ��Ա���� DeflateRect, ��С�� CGUIRect ����, ��Сָ���������Ŀ���
		/*! \param vLeft int �� vLeft ָ���� ��� ��С��
		\param vTop int �� vTop ָ���� �ϱ� ��С��
		\param vRight int �� vRight ָ���� �ұ� ��С��
		\param vBottom int �� vBottom ָ���� �±� ��С��
		\return û�з���ֵ */
		void DeflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX += vLeft; mTopLeft.mY += vTop; mBottomRight.mX -= vRight; mBottomRight.mY -= vBottom; }

		//! ��Ա���� InflateRect, ����� CGUIRect ����, ����ָ���������ķ�ɢ
		/*! \param vX int �� mTopLeft�ĺ�������ٺ�mBottomRight�ĺ�����������
		\param vY int �� mTopLeft����������ٺ�mBottomRight��������������
		\return û�з���ֵ */
		void InflateRect( int vX, int vY )
		{ mTopLeft.mX -= vX; mTopLeft.mY -= vY; mBottomRight.mX += vX; mBottomRight.mY += vY; }

		//! ��Ա���� InflateRect, ����� CGUIRect ����, ����ָ���������ķ�ɢ
		/*! \param rSize const CGUISize �� rSize��mCXָ���� mTopLeft�ĺ�������ٺ�mBottomRight�ĺ�����������
		rSize��mCYָ���� mTopLeft����������ٺ�mBottomRight��������������
		\return û�з���ֵ */
		void InflateRect( const CGUISize& rSize )
		{ 
			mTopLeft.mX		-= rSize.mCX;	mTopLeft.mY		-= rSize.mCY;			
			mBottomRight.mX += rSize.mCX;	mBottomRight.mY	+= rSize.mCY; 
		}

		//! ��Ա���� InflateRect, ����� CGUIRect ����, ����ָ���������ķ�ɢ
		/*! \param rRect const CGUIRect �� rRect ָ����ÿһ���ߵ�������
		\return û�з���ֵ */
		void InflateRect( const CGUIRect& rRect )
		{
			mTopLeft.mX		-= rRect.mTopLeft.mX;		mTopLeft.mY		-= rRect.mTopLeft.mY;	
			mBottomRight.mX += rRect.mBottomRight.mX;	mBottomRight.mY += rRect.mBottomRight.mY; 
		}

		//! ��Ա���� InflateRect, ����� CGUIRect ����, ����ָ���������ķ�ɢ
		/*! \param vLeft int �� vLeft ָ���� ��� ������
		\param vTop int �� vTop ָ���� �ϱ� ������
		\param vRight int �� vRight ָ���� �ұ� ������
		\param vBottom int �� vBottom ָ���� �±� ������
		\return û�з���ֵ */
		void InflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX -= vLeft; mTopLeft.mY -= vTop; mBottomRight.mX += vRight; mBottomRight.mY += vBottom; }

		//! ��Ա���� Height, ���ظ� CGUIRect ����ĸ߶�
		/*! \return ���ظ� CGUIRect ����ĸ߶� */
		int Height( ) const
		{ return mBottomRight.mY - mTopLeft.mY; }

		//! ��Ա���� Width, ���ظ� CGUIRect ����Ŀ��
		/*! \return ���ظ� CGUIRect ����Ŀ�� */
		int	Width( ) const	
		{ return mBottomRight.mX - mTopLeft.mX; }

		//! ��Ա���� IsRectEmpty, ���ظ� CGUIRect ����Ŀ�͸��ǲ���ȫ����0
		/*! \return ���ظ� CGUIRect �����Ƿ����� */
		bool IsRectEmpty( ) const
		{ return Width( ) <= 0 || Height( ) <= 0; }

		//! ��Ա���� IsRectNull, ���ظ� CGUIRect ������ĸ����Ƿ�ȫΪ0
		/*! \return ���ظ� CGUIRect �����Ƿ�Ϊ0 */
		bool IsRectNull( ) const
		{ return mTopLeft.mX == 0 && mTopLeft.mY == 0 && mBottomRight.mX == 0 && mBottomRight.mY == 0; }

		//! ��Ա���� MoveToX, ����� CGUIRect ����vX,�����겻��
		/*! \param vX int �� ָ������ĺ�����
		\return û�з���ֵ */
		void MoveToX( int vX )	
		{ mBottomRight.mX = vX + Width( ); mTopLeft.mX = vX; }

		//! ��Ա���� MoveToY, ����� CGUIRect ����vY,�����겻��
		/*! \param vY int �� ָ�������������
		\return û�з���ֵ */
		void MoveToY( int vY )	
		{ mBottomRight.mY = vY + Height( ); mTopLeft.mY = vY; }

		//! ��Ա���� MoveToXY, ����� CGUIRect ����vX, vY
		/*! \param vX int �� ָ������ĺ�����
		\param vY int �� ָ�������������
		\return û�з���ֵ */
		void MoveToXY( int vX, int vY )
		{
			mBottomRight.mX = vX + Width( );	mTopLeft.mX	= vX; 
			mBottomRight.mY = vY + Height( ); 	mTopLeft.mY	= vY;  
		}

		//! ��Ա���� MoveToXY, ����� CGUIRect ����rPoint
		/*! \param rPoint const CGUIPoint ������ ָ�����������
		\return û�з���ֵ */
		void MoveToXY( const CGUIPoint& rPoint )
		{
			mBottomRight.mX = rPoint.mX + Width( ); 	mTopLeft.mX = rPoint.mX; 
			mBottomRight.mY = rPoint.mY + Height( );	mTopLeft.mY = rPoint.mY; 
		}

		//! ��Ա���� OffsetRect, �ƶ��� CGUIRect ����
		/*! \param vX int �� ָ���������ƫ����
		\param vY int �� ָ���������ƫ����
		\return û�з���ֵ */
		void OffsetRect( int vX, int vY )					 
		{ *this += CGUIPoint( vX, vY ); }

		//! ��Ա���� OffsetRect, �ƶ��� CGUIRect ����
		/*! \param rPoint const CGUIPoint ������ ָ������ƫ����
		\return û�з���ֵ */
		void OffsetRect( const CGUIPoint& rPoint )
		{ *this += rPoint; }

		//! ��Ա���� OffsetRect, �ƶ��� CGUIRect ����
		/*! \param rSize const CGUISize ������ ָ������ƫ����
		\return û�з���ֵ */
		void OffsetRect( const CGUISize& rSize )				 
		{ *this += rSize; }	

		//! ��Ա���� SetRect, ���� CGUIRect ����
		/*! \param vLeft int �� CGUIRect��������ϽǺ�����
		\param vTop int �� CGUIRect��������Ͻ�������
		\param vRight int �� CGUIRect��������½Ǻ�����
		\param vBottom int �� CGUIRect��������½�������
		\return û�з���ֵ */
		void SetRect( int vLeft, int vTop, int vRight, int vBottom )
		{
			mTopLeft.mX		= vLeft;	mTopLeft.mY		= vTop; 
			mBottomRight.mX = vRight;	mBottomRight.mY = vBottom; 
		}

		//! ��Ա���� SetRect, ͨ��1�� CGUIPoint ��1�� CGUISize ���� CGUIRect ����, 
		/*! \param rPoint const CGUIPoint ������ ���Ͻ�����
		\param rSize const CGUISize ������ CGUIRect�ߴ�	
		\return û�з���ֵ */
		void SetRect( const CGUIPoint& rPoint, const CGUISize& rSize )
		{ mTopLeft = rPoint; mBottomRight = rPoint + rSize; }

		//! ��Ա���� SetRect, ͨ��2�� CGUIPoint ���� CGUIRect ����, 
		/*! \param rTopLeft const CGUIPoint ������ ���Ͻ�����
		\param rBottomRight const CGUIPoint ������ ���½�����
		\return û�з���ֵ */
		void SetRect( const CGUIPoint& rTopLeft, const CGUIPoint& rBottomRight )
		{ mTopLeft = rTopLeft; mBottomRight	= rBottomRight; }

		//! ��Ա���� SetRectEmpty, ���� CGUIRect ������0, 
		/*! \return û�з���ֵ */
		void SetRectEmpty( )
		{ mTopLeft.mX = 0; mTopLeft.mY = 0; mBottomRight.mX = 0; mBottomRight.mY = 0; }

		//! ��Ա���� Size, ���ظ� CGUIRect ����Ŀ�Ⱥ͸߶�
		/*! \return ���ظ� CGUIRect ����Ŀ�Ⱥ͸߶� */
		CGUISize Size( ) const
		{ return CGUISize( Width( ), Height( ) ); }

		//! ��Ա���� NormalizeRect, �������� CGUIRect ����
		/*! \return û�з���ֵ */
		void NormalizeRect( );

		//! ��Ա���� PtInRect, �ж�ָ�������Ƿ��ڸ� CGUIRect ������
		/*! \param rPoint const CGUIPoint������ ���жϵ�����
		\return ����ָ�������ǲ����ڸ� CGUIRect ������ */
		bool PtInRect( const CGUIPoint&	rPoint ) const;

		//! ��Ա���� IntersectRect, �������� CGUIRect ����Ľ���
		/*! \brief �����������ľ��β�����, ��ô�þ��ν�������
		\brief eg. CGUIRect( ).( CGUIRect( 0, 0, 10, 10 ), CGUIRect( 5, 5, 15, 15 ) ) == CGUIRect( 5, 5, 10, 10 )
		\param const CGUIRect& rSrcRect1 Դ����
		\param const CGUIRect& rSrcRect2 Դ����
		\return ������������Դ���λ��Ǽ��������ľ��β�������������false, ���ⷵ��true */
		bool IntersectRect( const CGUIRect& rSrcRect1, const CGUIRect& rSrcRect2 );

		//! ��Ա���� UnionRect, �������� CGUIRect ����Ĳ���
		/*! \brief ������������Դ������һ��������, ����������false
		\brief eg. CGUIRect( ).( CGUIRect( 0, 0, 10, 10 ), CGUIRect( 5, 5, 15, 15 ) ) == CGUIRect( 0, 0, 15, 15 )
		\param const CGUIRect& rSrcRect1 Դ����
		\param const CGUIRect& rSrcRect2 Դ����
		\return ������������Դ������һ��������, ����������false, ���ⷵ��true */
		bool UnionRect( const CGUIRect& rSrcRectl, const CGUIRect& rSrcRect2 );
	public:
		//! != ������, �ж����� CGUIRect �����Ƿ����
		/*! \param const CGUIRect& rRect 
		\return ���ظ� CGUIRect �����rRect�Ƿ����, �������ȷ���true, ���򷵻�false */
		bool operator != ( const CGUIRect& rRect ) const
		{ return !EqualRect( rRect ); }

		//! == ������, �ж����� CGUIRect �����Ƿ����
		/*! \param const CGUIRect& rRect 
		\return ���ظ� CGUIRect �����rRect�Ƿ����, �����ȷ���true, ���򷵻�false */
		bool operator == ( const CGUIRect& rRect ) const
		{ return EqualRect( rRect ); }

		//! &= ������, ����� CGUIRect ����� ָ�� CGUIRect ����Ľ���
		/*! \param const CGUIRect& rRect 
		\return û�з���ֵ */
		void operator &= ( const CGUIRect& rRect )
		{ IntersectRect( *this, rRect ); }

		//! |= ������, ����� CGUIRect ����� ָ�� CGUIRect ����Ĳ���
		/*! \param const CGUIRect& rRect 
		\return û�з���ֵ */
		void operator |= ( const CGUIRect& rRect )
		{ UnionRect( *this, rRect ); }

		//! & ������, ����� CGUIRect ����� ָ�� CGUIRect ����Ľ���
		/*! \param const CGUIRect& rRect 
		\return ����� CGUIRect ����� ָ�� CGUIRect ����Ĳ���, ���� CGUIRect �Ŀ���  */
		CGUIRect operator & ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult;
			tRectResult.IntersectRect( *this, rRect );
			return tRectResult;
		}

		//! | ������, ����� CGUIRect ����� ָ�� CGUIRect ����Ĳ���
		/*! \param const CGUIRect& rRect 
		\return ����� CGUIRect ����� ָ�� CGUIRect ����Ĳ���, ���� CGUIRect �Ŀ���  */
		CGUIRect operator | ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult;
			tRectResult.UnionRect( *this, rRect );
			return tRectResult;
		}

		//! - ������, �Ѹ� CGUIRect ����ƫ�� rPoint ����λ
		/*! \param const CGUIPoint& rPoint
		\return �Ѹ� CGUIRect ����ƫ�� rPoint ����λ, ���� CGUIRect �Ŀ���  */
		CGUIRect operator - ( const CGUIPoint& rPoint ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( -rPoint );
			return tRectResult;
		}

		//! - ������, �Ѹ� CGUIRect ����ƫ�� rSize ����λ
		/*! \param const CGUISize& rSize
		\return �Ѹ� CGUIRect ����ƫ�� rSize ����λ, ���� CGUIRect �Ŀ���  */
		CGUIRect operator - ( const CGUISize& rSize ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( -rSize );
			return tRectResult;
		}

		//! - ������, ��С�� CGUIRect ������С�ĳߴ��� rRect ָ��
		/*! \param const CGUIRect& rRect
		\return ��С�� CGUIRect ������С�ĳߴ��� rRect ָ��, ���� CGUIRect �Ŀ���  */
		CGUIRect operator - ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.DeflateRect( rRect );
			return tRectResult;
		}

		//! + ������, �Ѹ� CGUIRect ����ƫ�� rPoint ����λ
		/*! \param const CGUIPoint& rPoint
		\return �Ѹ� CGUIRect ����ƫ�� rPoint ����λ, ���� CGUIRect �Ŀ���  */
		CGUIRect operator + ( const CGUIPoint& rPoint ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( rPoint );
			return tRectResult;
		}

		//! + ������, �Ѹ� CGUIRect ����ƫ�� rSize ����λ
		/*! \param const CGUISize& rSize
		\return �Ѹ� CGUIRect ����ƫ�� rSize ����λ, ���� CGUIRect �Ŀ���  */
		CGUIRect operator + ( const CGUISize& rSize ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.OffsetRect( rSize );
			return tRectResult;
		}

		//! + ������, ����� CGUIRect ��������ĳߴ��� rRect ָ��
		/*! \param const CGUIRect& rRect
		\return ����� CGUIRect ��������ĳߴ��� rRect ָ��, ���� CGUIRect �Ŀ���  */
		CGUIRect operator + ( const CGUIRect& rRect ) const
		{
			CGUIRect tRectResult = *this;
			tRectResult.InflateRect( rRect );
			return tRectResult;
		}

		//! += ������, �Ѹ� CGUIRect ����ƫ�� rPoint ����λ
		/*! \param const CGUIPoint& rPoint
		\return û�з���ֵ  */
		void operator += ( const CGUIPoint& rPoint )
		{ mTopLeft += rPoint; mBottomRight += rPoint; }

		//! += ������, �Ѹ� CGUIRect ����ƫ�� rSize ����λ
		/*! \param const CGUISize& rSize
		\return û�з���ֵ  */
		void operator += ( const CGUISize& rSize )
		{ mTopLeft += rSize; mBottomRight += rSize; }

		//! += ������, ����� CGUIRect ��������ĳߴ��� rRect ָ��
		/*! \param const CGUIRect& rRect
		\return û�з���ֵ  */
		void operator += ( const CGUIRect& rRect )
		{ InflateRect( rRect );	}

		//! += ������, �Ѹ� CGUIRect ����ƫ�� rPoint ����λ
		/*! \param const CGUIPoint& rPoint
		\return û�з���ֵ  */
		void operator -= ( const CGUIPoint& rPoint )
		{ mTopLeft -= rPoint; mBottomRight -= rPoint; }

		//! += ������, �Ѹ� CGUIRect ����ƫ�� rSize ����λ
		/*! \param const CGUISize& rSize
		\return û�з���ֵ  */
		void operator -= ( const CGUISize& rSize )
		{ mTopLeft -= rSize; mBottomRight -= rSize;	}

		//! += ������, ��С�� CGUIRect ������С�ĳߴ��� rRect ָ��
		/*! \param const CGUIRect& rRect
		\return û�з���ֵ  */
		void operator -= ( const CGUIRect& rRect )
		{ DeflateRect( rRect );	}
	};
}