#ifndef __UISCRIPTMANAGER_I_H__
#define __UISCRIPTMANAGER_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IUIScriptManager
	{
	public:
		enum ENUMSCRIPTLANGUAGE
		{
			ESL_LUA = 0
		};

		virtual int GetScriptLanguage() = 0;

		virtual BOOL Initialize() = 0;
		virtual void UnInitialize() = 0;

		virtual BOOL LoadScriptFile(LPCTSTR lpszFileName) = 0;
		virtual BOOL LoadScriptBuffer(LPBYTE lpBuffer, UINT nLength) = 0;
		virtual BOOL ExecuteFunction(LPCTSTR lpszFunName, LPCTSTR lpszParams) = 0;
		
	};
} // Tenio

#endif // __UISCRIPTMANAGER_I_H__