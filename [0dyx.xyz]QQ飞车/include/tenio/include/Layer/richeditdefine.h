#ifndef __RICHEDITDEFINE_H__
#define __RICHEDITDEFINE_H__

#include <windows.h>
#include "June/normalfont_i.h"

namespace Tenio
{
	class CRichEditNode
	{
	public:
		int nType;
		RECT rc;
	};

	class CBrNode : public CRichEditNode
	{
	};

	class CCharNode : public CRichEditNode
	{
	public:
		WORD wChar;
		IFont* pFont;
		COLORREF col;
	};

	class CHyperLinkCharNode : public CCharNode
	{
	public:
		DWORD dwId;
	};

	class CImgNode : public CRichEditNode
	{
	public:
		LPVOID lpImg;
	};
} // Tenio

#endif // __RICHEDITDEFINE_H__