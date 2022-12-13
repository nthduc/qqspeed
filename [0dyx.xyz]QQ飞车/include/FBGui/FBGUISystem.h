#pragma once
#include "FBGUIFontManager.h"
#include "FBGUIRenderDriver.h"

namespace FBGUI
{

/*!
\brief
	CGUISystem �࣬CGUISystem ����ά����ǰʹ�õ���Ⱦ�������豸���߼�����ı任
	CGUISystem���������Ϣ�����ɸ���ǰ�Ĵ���Sheet,����ά��һЩȫ������
*/
	class CGUISystem : public CSingleton< CGUISystem > 
	{
		
	protected:
	
		CGUIFontManager*			mpFontManager;		//! ���������
		CGUIRenderDriver*			mpRenderDriver;		//! ��ǰʹ�õ���Ⱦ����
		
	public:
		CGUISystem::CGUISystem( CGUIRenderDriver*	pRenderDriver ) ;
		CGUIRenderDriver* GetRenderDriver( ) const	{ return mpRenderDriver; }
		virtual ~CGUISystem( );
	};

};