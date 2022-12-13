#pragma once
#include "FBGUIFontManager.h"
#include "FBGUIRenderDriver.h"

namespace FBGUI
{

/*!
\brief
	CGUISystem 类，CGUISystem 负责维护当前使用的渲染驱动，设备和逻辑坐标的变换
	CGUISystem负责接收消息并分派给当前的窗口Sheet,并且维护一些全局配置
*/
	class CGUISystem : public CSingleton< CGUISystem > 
	{
		
	protected:
	
		CGUIFontManager*			mpFontManager;		//! 字体管理器
		CGUIRenderDriver*			mpRenderDriver;		//! 当前使用的渲染驱动
		
	public:
		CGUISystem::CGUISystem( CGUIRenderDriver*	pRenderDriver ) ;
		CGUIRenderDriver* GetRenderDriver( ) const	{ return mpRenderDriver; }
		virtual ~CGUISystem( );
	};

};