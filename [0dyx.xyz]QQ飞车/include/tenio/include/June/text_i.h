#ifndef __TEXT_I_H__
#define __TEXT_I_H__

#include "material_i.h"
#include "font_i.h"

namespace Tenio
{
	//! 记录文字/光标的位置，按字符计数
	struct LOCATION
	{
	public:
		LOCATION(int iLine = -1, int iCol = -1) : x(iLine), y(iCol){}

	public:
		int x; //! 行
		int y; //! 列
	};

	const int MAXSELECTIONSIZE = 1024;

	/**
	 * @ingroup tenuilib
	 * @brief 文字对象公共接口
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */	
	class IText : public IMaterial
	{
	public:
		virtual void SetText(LPCTSTR pText) = 0;
		virtual LPCTSTR GetText() = 0;
		virtual void AutoBreak(BOOL bAutoBreak, int nWidth) = 0;
		virtual SIZE GetTextSize() = 0;
		virtual void SetMultiLineMode(BOOL bIsMultiLine) = 0;
		virtual void SetNumberOnlyMode(BOOL bIsNumberOnly) = 0;
		virtual void SetPasswordMode(BOOL bIsPassword) = 0;
		virtual void SetMaxCharacters(int nCount) = 0;
		virtual LOCATION AdjustPosition(const POINT& ptSrc, POINT& ptDst) = 0;
		virtual LOCATION GetForwardCharPosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION GetBackwardCharPosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION GetForwardLinePosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION GetBackwardLinePosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION InsertChar(const LOCATION& location, POINT& ptDst, UINT uChar) = 0;
		virtual LOCATION DelForwardChar(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION DelBackwardChar(const LOCATION& location, POINT& ptDst) = 0;
		virtual void SetSel(const LOCATION& lcStart, const LOCATION& lcEnd) = 0;
		virtual LPTSTR GetSelText(LPTSTR pText) = 0;
		virtual LOCATION DelSelText(POINT& ptDst) = 0;
		virtual void SetFont(IFont* pFont) = 0;
		virtual int GetLineCount() = 0;
		virtual LPCTSTR GetTextByLineNumber(int nLineNum, int& nSelStart, int& nSelEnd) = 0;
	}; // Tenio
};

#endif // __TEXT_I_H__