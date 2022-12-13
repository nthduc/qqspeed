
#pragma once

#include <string>

extern void Log(const char* pacFmt, ...);

extern std::string PathGetFileName( const std::string &path ); //�ļ���
extern std::string PathGetExtName( const std::string &path ); //��չ��
extern std::string PathGetFileTitle( const std::string &path ); //�ļ�������������չ����
extern std::string PathGetFileDir( const std::string &path ); //�ļ�·��


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

