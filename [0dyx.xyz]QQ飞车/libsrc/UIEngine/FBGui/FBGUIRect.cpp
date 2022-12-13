#include ".\fbguirect.h"
#include "FBGUIPoint.h"
#include "FBGUIDef.h"
namespace FBGUI
{
	// ************************************************************ //
	// CGUIRect
	// 2D ¾ØÐÎÇøÓò
	// ************************************************************ //

	bool CGUIRect::IntersectRect( const CGUIRect& rSrcRect1, const CGUIRect& rSrcRect2 )
	{
		if ( rSrcRect1.IsRectEmpty( ) || rSrcRect2.IsRectEmpty( ) )
			return false;

		int tLeft	= GUI_MAX( rSrcRect1.mTopLeft.mX, rSrcRect2.mTopLeft.mX );
		int tTop	= GUI_MAX( rSrcRect1.mTopLeft.mY, rSrcRect2.mTopLeft.mY );
		int tRight	= GUI_MIN( rSrcRect1.mBottomRight.mX, rSrcRect2.mBottomRight.mX );
		int tBottom	= GUI_MIN( rSrcRect1.mBottomRight.mY, rSrcRect2.mBottomRight.mY );

		if ( CGUIRect( tLeft, tTop, tRight, tBottom, false ).IsRectEmpty( ) )
		{
			SetRectEmpty( );
			return false;
		}

		mTopLeft.mX		= tLeft;
		mTopLeft.mY		= tTop;
		mBottomRight.mX = tRight;
		mBottomRight.mY = tBottom;
		return true;
	}

	void CGUIRect::NormalizeRect( )
	{
		mTopLeft.mX		= GUI_MIN( mTopLeft.mX, mBottomRight.mX );
		mTopLeft.mY		= GUI_MIN( mTopLeft.mY, mBottomRight.mY );
		mBottomRight.mX	= GUI_MAX( mTopLeft.mX, mBottomRight.mX );
		mBottomRight.mY	= GUI_MAX( mTopLeft.mY, mBottomRight.mY );
	}

	bool CGUIRect::PtInRect( const CGUIPoint& rPoint ) const
	{
		if ( IsRectEmpty( ) )
			return false;

		if ( rPoint.mX < mTopLeft.mX || rPoint.mX > mBottomRight.mX )
			return false;

		if ( rPoint.mY < mTopLeft.mY || rPoint.mY > mBottomRight.mY )
			return false;

		return true;
	}

	bool CGUIRect::UnionRect( const CGUIRect& rSrcRectl, const CGUIRect& rSrcRect2 )
	{
		int tLeft	= rSrcRectl.mTopLeft.mX < rSrcRect2.mTopLeft.mX ? rSrcRectl.mTopLeft.mX : rSrcRect2.mTopLeft.mX;
		int tTop	= rSrcRectl.mTopLeft.mY < rSrcRect2.mTopLeft.mY ? rSrcRectl.mTopLeft.mY : rSrcRect2.mTopLeft.mY;
		int tRight	= rSrcRectl.mBottomRight.mX > rSrcRect2.mBottomRight.mX ? rSrcRectl.mBottomRight.mX : rSrcRect2.mBottomRight.mX;
		int tBottom	= rSrcRectl.mBottomRight.mY > rSrcRect2.mBottomRight.mY ? rSrcRectl.mBottomRight.mY : rSrcRect2.mBottomRight.mY;

		mTopLeft.mX		= tLeft;
		mTopLeft.mY		= tTop;
		mBottomRight.mX = tRight;
		mBottomRight.mY = tBottom;
		return true;
	}
}