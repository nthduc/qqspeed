#include "FBGUIBase.h"
#include "FBGUIPoint.h"

#include "FBGUISystem.h"
#include "Windows.h"
namespace FBGUI
{

	// ************************************************************ //
	// CGUISystem
	// ************************************************************ //
	template<> CGUISystem* CSingleton< CGUISystem >::spSingleton = NULL;

	CGUISystem::CGUISystem( CGUIRenderDriver*	pRenderDriver  ) 
	{
		mpRenderDriver = pRenderDriver ;
		mpFontManager  = new CGUIFontManager;
	}	

	CGUISystem::~CGUISystem( )
	{
		GUI_DELETEPTR( mpFontManager );
	}

}