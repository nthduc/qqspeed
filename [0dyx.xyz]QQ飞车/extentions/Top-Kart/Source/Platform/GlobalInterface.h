////////////////////////////////////////////////
//     FileName:    GlobalInterface.h
//     Author:      Fish
//     Date:        2006-12-8 10:16:15
//     Description: []
////////////////////////////////////////////////
#pragma once

interface	IGameApp;
interface	INetwork;
interface	IQQTppp;
interface	IGUI;
interface	IGameSettings;
class		IDisplay;
class		CUIMiscMgr;
class		CMsgDataCache;
class		CScriptState;
class		CGPControl;

struct GlobalInterface
{
	static IGameApp*		m_pApp;
	static INetwork*		m_pNetwork;
	static IQQTppp*			m_pQQTppp;
	static IGUI*			m_pGUI;
	static IDisplay*		m_pDisplay;
	static CUIMiscMgr*		m_pUIMiscMgr;
	static CMsgDataCache*	m_pDataCache;
	static IGameSettings*	m_pSettings;
	static CScriptState*	m_pScriptState;
	static CGPControl*		m_pGPControl;
};