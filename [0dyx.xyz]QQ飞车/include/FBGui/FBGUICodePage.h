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
		virtual int     GetGlyphIndex( const char* pText ) const = 0;		// pText��һ�����ֽ��ַ����Ƕ��ֽ����ģ� ��������
		virtual int     GetCodeSpace ()                    const = 0;		// �õ��ַ�����ռ�
		virtual wchar_t GetUnicode   ( int vGlphyIndex )   const = 0;		// �õ�������Ӧ��Unicode����
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
		virtual int     GetGlyphIndex( const char* pText ) const;		// pText��'\0'�������ַ�����ֻ����һ���ַ�����һ�����ģ� ��������
		virtual int     GetCodeSpace ()                    const;		// �õ��ַ�����ռ�
		virtual wchar_t GetUnicode   ( int vGlphyIndex )   const;		// �õ�������Ӧ��Unicode����
	};
}