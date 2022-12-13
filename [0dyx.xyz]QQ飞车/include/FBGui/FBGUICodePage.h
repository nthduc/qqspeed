#pragma once

#include <conio.h>

namespace FBGUI
{
	// ************************************************************ //
	// CGUICodePage
	// ************************************************************ //

	class CGUICodePage
	{
	public:
		virtual int     GetGlyphIndex( const char* pText ) const = 0;		// pText是一个多字节字符或是多字节中文， 返回索引
		virtual int     GetCodeSpace ()                    const = 0;		// 得到字符内码空间
		virtual wchar_t GetUnicode   ( int vGlphyIndex )   const = 0;		// 得到索引对应的Unicode编码
	};

	// ************************************************************ //
	// CGUIGB2312Page
	// ************************************************************ //

	class CGUIGB2312Page : public CGUICodePage
	{
	protected:
		enum CODE_SPACE
		{
			GB_MIN_1ST		= 0x00A1,
			GB_MAX_1ST		= 0x00FE,
			GB_MIN_2ND		= 0x00A1,
			GB_MAX_2ND		= 0x00FE,
		};

	public:
		virtual int     GetGlyphIndex( const char* pText ) const;		// pText是'\0'结束的字符串，只能是一个字符或是一个中文， 返回索引
		virtual int     GetCodeSpace ()                    const;		// 得到字符内码空间
		virtual wchar_t GetUnicode   ( int vGlphyIndex )   const;		// 得到索引对应的Unicode编码
	};
}