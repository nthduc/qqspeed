#include "FBGUICodePage.h"
#include "FBGUIDef.h"
#include <Windows.h>
namespace FBGUI
{

	// ************************************************************ //
	// CGUIGB2312Page
	// ************************************************************ //

	int CGUIGB2312Page::GetGlyphIndex( const char* pText ) const
	{
		int tGlphyIndex = 0;
		if ( pText[ 0 ] & 0x80 )
		{
			tGlphyIndex = ( (unsigned char) pText[ 0 ] - GB_MIN_1ST ) * ( GB_MAX_2ND + 1 - GB_MIN_2ND ) + (unsigned char) pText[ 1 ] - GB_MIN_2ND + 256;
		}
		else // 如果是英文
		{
			tGlphyIndex = (unsigned char) pText[ 0 ];
		}

		// 如果在内码空间里面
		if ( tGlphyIndex >= 0 && tGlphyIndex < GetCodeSpace( ) )
		{
			return tGlphyIndex;
		}

		// 否则返回错误码
		return GUI_ERR_OUTOFCODEPAGE;
	}

	int CGUIGB2312Page::GetCodeSpace( ) const
	{
		return ( GB_MAX_2ND + 1 - GB_MIN_2ND ) * ( GB_MAX_1ST + 1 - GB_MIN_1ST ) + 256;
	}

	wchar_t CGUIGB2312Page::GetUnicode( int vGlphyIndex ) const
	{
		char tText[ 3 ] = { 0 };
		if ( vGlphyIndex < 256 )
		{
			tText[ 0 ] = char( vGlphyIndex );
		}
		else
		{
			tText[ 0 ] = ( vGlphyIndex -  256 ) / ( GB_MAX_2ND + 1 - GB_MIN_2ND ) + GB_MIN_1ST;
			tText[ 1 ] = ( vGlphyIndex -  256 ) % ( GB_MAX_2ND + 1 - GB_MIN_2ND ) + GB_MIN_2ND;
		}

		// 得到Unicode编码
		wchar_t tUnicode;
		//CGUISystem::GetSingleton( ).GetPlatform( )->MbToWc( tText, int( ::strlen( tText ) ), &tUnicode, 1 );
		::MultiByteToWideChar( CP_ACP, 0, tText, int( ::strlen( tText ) ), &tUnicode, 1 );
		return tUnicode;
	}
}
