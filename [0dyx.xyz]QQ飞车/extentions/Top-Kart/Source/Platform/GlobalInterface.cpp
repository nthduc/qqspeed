////////////////////////////////////////////////
//     FileName:    GlobalInterface.cpp
//     Author:      Fish
//     Date:        2006-12-8 10:20:55
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "GlobalInterface.h"

IGameApp*		GlobalInterface::m_pApp			= NULL;
IGUI*			GlobalInterface::m_pGUI			= NULL;
CMsgDataCache*	GlobalInterface::m_pDataCache	= NULL;
INetwork*		GlobalInterface::m_pNetwork		= NULL;
IQQTppp*		GlobalInterface::m_pQQTppp		= NULL;
CUIMiscMgr*		GlobalInterface::m_pUIMiscMgr	= NULL;
IGameSettings*	GlobalInterface::m_pSettings	= NULL;
IDisplay*		GlobalInterface::m_pDisplay		= NULL;
CScriptState*	GlobalInterface::m_pScriptState	= NULL;
CGPControl*		GlobalInterface::m_pGPControl	= NULL;
