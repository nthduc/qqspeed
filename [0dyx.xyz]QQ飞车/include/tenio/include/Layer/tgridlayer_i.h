#ifndef __TGRIDLAYER_I_H__
#define __TGRIDLAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"

namespace Tenio
{
	enum GRIDLINEDIRECTION
	{
		ENUM_GLD_HORI,
		ENUM_GLD_VERT	
	};

	enum GRIDDISPLAYMODE
	{
		ENUM_GDM_BOTH,
		ENUM_GDM_HORI,
		ENUM_GDM_VERT,
		ENUM_GDM_NONE
	};
	
	class ITGridLayer;

	TENIO_MAKE_TYPEID_MAP(ITGridLayer, COMPONENTID_TGRIDLAYER);

	class ITGridLayer : public IJuneLayer, public IComponent
	{
	public:
		virtual void InitGridLayer(int arHLineArray[], int arVLineArray[],
			UINT uHLineArrayLen, UINT uVLineArrayLen) = 0;
		virtual int InsertLine(int nPos, GRIDLINEDIRECTION gldDirection) = 0;
		virtual void RemoveLine(int nIndex, GRIDLINEDIRECTION gldDirection) = 0;
		virtual void RemoveAllLine(GRIDLINEDIRECTION gldDirection) = 0;
		virtual BOOL MoveLine(int nIndex, int nPos, GRIDLINEDIRECTION gldDirection) = 0;
		virtual BOOL OffsetLine(int nIndex, int nOffsetPos, GRIDLINEDIRECTION gldDirection) = 0;
		virtual BOOL OffsetAllLineAfterIndex(int nIndex, int nOffsetPos, GRIDLINEDIRECTION gldDirection) = 0;
		virtual BOOL OffsetAllLineBeforeIndex(int nIndex, int nOffsetPos, GRIDLINEDIRECTION gldDirection) = 0;
		virtual void SetDisplayMode(GRIDDISPLAYMODE gdmDisplayMode) = 0;
		virtual void SetPenInfo(PENINFO piPenInfo) = 0;
		virtual void SetSize(int nHeight, int nWidth) = 0;
		virtual int GetLinePos(int nIndex, GRIDLINEDIRECTION gldDirection) = 0;
		virtual int GetIndexByPos(int nPos, GRIDLINEDIRECTION gldDirection) = 0;
	};
};

#endif

// InterfaceName:TGridLayer
// SubFolderName:Layer
