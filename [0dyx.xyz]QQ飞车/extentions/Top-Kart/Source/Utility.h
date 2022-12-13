
#pragma once
#include <string>
#include "stringex.h"//扩展了std::string 功能的StringEx

extern void Log(const char* pacFmt, ...);

extern std::string PathGetFileName( const std::string &path ); //文件名
extern std::string PathGetExtName( const std::string &path ); //扩展名
extern std::string PathGetFileTitle( const std::string &path ); //文件名（不包括扩展名）
extern std::string PathGetFileDir( const std::string &path ); //文件路径
//extern const char * MakeString(const char * fmt, ...);
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }


#define SAFE_NIDELETE(p)		{ if(p) { NiDelete (p);     (p)=NULL; } }
#define SAFE_NIDELETE_ARRAY(p)	{ if(p) { NiDelete[] (p);   (p)=NULL; } }

