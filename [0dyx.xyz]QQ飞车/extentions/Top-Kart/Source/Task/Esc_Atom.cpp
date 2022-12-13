#include "Top-KartPCH.h"
#include "Esc_Atom.h"
#include "TaskNet.h"
#include "../InputManager.h"
#include "../Platform/GlobalInterface.h"
#include "framework/gui_i.h"


CEsc_Atom::CEsc_Atom():
m_eKey(NiInputKeyboard::KEY_ESCAPE)
{
	m_spKeyboard = InputManager::Get()->GetInputSystem()->GetKeyboard();
}

CEsc_Atom::~CEsc_Atom()
{

}

void CEsc_Atom::Update(float fTime)
{
	if(m_spKeyboard)
	{
		if(m_spKeyboard->KeyWasPressed(m_eKey))
		{
			GetMainApp()->ShowPointer();
			GlobalInterface::m_pScriptState->DoString("Win_ShowWidget('UI.taskescbox', true)");
			//CTaskNet::RequestReportCurrentOpt(NxPlayMgr::GetHostPlay()->GetPlayUin(), m_dwParam);
		}
		if(m_spKeyboard->KeyWasPressed(NiInputKeyboard::KEY_F4))
		{
			
			GetMainApp()->ShowPointer();
			GlobalInterface::m_pScriptState->DoString("Win_ShowWidget('UI.taskf4box', true)");
		}
	}
}