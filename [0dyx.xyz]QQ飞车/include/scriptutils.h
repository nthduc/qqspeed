////////////////////////////////////////////////
//     FileName:    scriptutils.h
//     Author:      Fish
//     Date:        2006-10-12 15:44:19
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "variant.h"
#include "script.h"

#define UI_SCRIPT_BUFSIZE	200

inline int CallScript(CScriptState *pscript, LPCSTR pfunc,LPCSTR arg = "")
{
	CVarFloat retvalue( "tmpfunc_ret" );
	char buffer[4096] = "\0";	
	if(strlen(arg)>0)
		sprintf( buffer, "tmpfunc = %s; tmpfunc_ret = -1; if tmpfunc then tmpfunc_ret = tmpfunc(%s) end;", pfunc,arg );//如果该函数存在，调用该函数
	else
		sprintf( buffer, "tmpfunc = %s; tmpfunc_ret = -1; if tmpfunc then tmpfunc_ret = tmpfunc() end;", pfunc );//如果该函数存在，调用该函数
	pscript->DoString( buffer);//执行lua代码，调用lua函数
	retvalue.GetFromScript(pscript);
	return retvalue.Integer();
}

inline int CallScriptEx(CScriptState *pscript, LPCSTR pfunc,LPCSTR fmt, ...)
{
	char	msg[UI_SCRIPT_BUFSIZE];
	va_list	argptr;
	va_start(argptr,fmt);
	vsprintf(msg,fmt,argptr);
	va_end(argptr);
	return CallScript(pscript, pfunc,msg);	
}


inline int TriggleScriptEvent(CScriptState *pscript, LPCSTR pfunc, LPCSTR arg = "")
{
	char szfunc[200] = "\0";
	sprintf(szfunc, "UI.children.AppEventHandle.%s", pfunc);
	return CallScript(pscript, szfunc, arg);
}

inline int TriggleScriptEventEx(CScriptState *pscript, LPCSTR pfunc,LPCSTR fmt, ...)
{
	char	msg[UI_SCRIPT_BUFSIZE];
	va_list	argptr;
	va_start(argptr,fmt);
	vsprintf(msg,fmt,argptr);
	va_end(argptr);
	return TriggleScriptEvent(pscript, pfunc, msg);	
}
