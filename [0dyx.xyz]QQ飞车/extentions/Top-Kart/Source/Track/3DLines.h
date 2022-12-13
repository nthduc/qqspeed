#pragma once
#include "nimemobject.h"
#include "NiNode.h"
#include "NiTMap.h"
#include "NiLines.h"

class LineManager :
	public NiMemObject
{
	typedef NiTMap<unsigned int, NiLinesPtr> LineMap;
	LineManager(NiNodePtr spRoot);
	virtual ~LineManager(void);
public:	
	static LineManager * CreateLineManager(NiNodePtr spRoot);
	static void          DestroyLineManager();
	
	static LineManager * GetLineManager();

	unsigned int AddLine(const NiPoint3& kStart, const NiPoint3& kEnd, const NiColorA& kColor);
	void		 RemoveLine(unsigned int ID);
	void		 ResetLine(unsigned int ID, const NiPoint3& kStart, const NiPoint3& kEnd);

protected:
	bool		m_bDirtyFlag;
	LineMap		m_LinesMap;
	NiNodePtr   m_spLineRoot;

	static unsigned int  ms_uiLineID;
	static LineManager * ms_pLineMgr;
};
