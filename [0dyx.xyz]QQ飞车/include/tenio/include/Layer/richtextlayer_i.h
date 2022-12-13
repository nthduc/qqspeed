#ifndef __RICHTEXTLAYER_I_H__
#define __RICHTEXTLAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "../uilib/variableparam_i.h"
#include "../June/text_i.h"
#include "richeditdefine.h"

namespace Tenio
{
	class IRichTextLayer;

	TENIO_MAKE_TYPEID_MAP(IRichTextLayer, COMPONENTID_RICHTEXTLAYER);

	class IRichTextLayer : public IJuneLayer, public IComponent
	{
	public:
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
		virtual void InsertNode(CRichEditNode* pNode, const LOCATION& Location) = 0;
		virtual void InsertText(LPCTSTR pText, const LOGFONT* pLogFont, COLORREF Color,
			const LOCATION& Location) = 0;
		virtual void InsertHyperLinkText(LPCTSTR pText, const LOGFONT* pLogFont, COLORREF Color, 
			DWORD dwId, const LOCATION& Location) = 0;
		virtual int GetLineCount() = 0;
		virtual LOCATION AdjustPosition(const POINT& ptSrc, POINT& ptDst) = 0;
		virtual CRichEditNode* GetNode(const LOCATION& Location) = 0;
	};
};

#endif

// InterfaceName:RichTextLayer
// SubFolderName:Layer
