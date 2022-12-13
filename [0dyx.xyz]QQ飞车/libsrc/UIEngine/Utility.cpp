#include "nimain.h"
#include "Utility.h"

void Log(const char* pacFmt, ...)
{
#if defined(_DEBUG)
	char acTemp[2048];

	va_list args;
	va_start(args, pacFmt);
	vsprintf(acTemp, pacFmt, args);
	va_end(args);

	NiOutputDebugString(acTemp);
	printf(acTemp);
#endif  
}

typedef std::string::size_type STR_POS;
static int GetFileNamePos( const std::string &path )
{
	STR_POS pos = path.find_last_of( '/' );
	STR_POS pos1 = path.find_last_of( '\\' );

	if( pos==path.npos || (pos1!=path.npos&&pos1>pos) ) pos = pos1;
	if( pos != path.npos ) return pos+1;
	else return 0;
}

std::string PathGetFileName( const std::string &path )
{
	return path.substr( GetFileNamePos(path) );
}

std::string PathGetFileTitle( const std::string &path )
{
	STR_POS pos = GetFileNamePos( path );
	STR_POS pos1 = path.find_last_of( '.' );
	return path.substr( pos, pos1-pos );
}

std::string PathGetFileDir( const std::string &path )
{
	std::string dir = path.substr( 0, GetFileNamePos(path) );
	if( dir.size() == 0 ) dir = ".\\";
	return dir;
}

std::string PathGetExtName( const std::string &path )
{
	STR_POS pos = path.find_last_of( '.' );
	if( pos != path.npos ) pos++;
	return path.substr(pos);
}
