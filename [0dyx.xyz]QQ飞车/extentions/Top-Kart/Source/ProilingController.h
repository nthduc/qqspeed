
#pragma once
#pragma warning(disable: 4530)

#include "NiColor.h"
#include "NiMemObject.h"
#include <vector>
#include "Profiling.h"
#include "ScreenTextManager.h"
#include "NiScreenElements.h"
#include "Profiling.h"

class CProfileIterator;

class ProfilingController 
	: public NiMemObject
{
public:
	ProfilingController(void);
	~ProfilingController(void);
	void   ProfileDraw(void);
	void   Profiling(const char *sCommand); 
	//void   SetProfilingPos(int x, int y);
	//void   Init(ScreenTextManager * pScreenTextManager);

protected:

    bool   m_bHelp;
	bool   m_bVisible;

	NiScreenElementsPtr m_spBackground;
	CProfileIterator * m_pProfileIterator;
	CProfileManager	 * m_pProfileManager;
	ScreenTextManager* m_pScreenTextManager;
};
