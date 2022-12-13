
#ifndef __S_VFILE_H__
#define __S_VFILE_H__

#include "kernel.h"
#include <map>
#include "package.h"
#pragma warning(disable:4018)

class CVFileMgr;
class CVFile
{
	friend class CVFileMgr;
public:
	CVFile();

	enum
	{
		NONE = 0,
		READ = 1,
		WRITE = 2,
		READWRITE = (READ|WRITE),
		TEXT = 4,   //if TEXT is not set, it is a binary
		SEEKBEGIN,
		SEEKCUR,
		SEEKEND
	};
	virtual void Close();
	virtual bool Read( void *pbuf, unsigned int buflen ) = 0;
	virtual bool Write( void *pbuf, unsigned int buflen ) = 0;
	virtual int  Seek( int offset, unsigned int seekfrom ) = 0;

	int  TellPos()
	{
		return Seek( 0, SEEKCUR );
	}
	int  GetFileSize()
	{
		return m_FileSize;
	}
	std::string GetFileName();
	std::string GetFileTitle();
	std::string GetFileDir();
	std::string GetFilePath();

protected:
	virtual ~CVFile();

	unsigned int m_OpenMode;
	std::string m_Path;
	int    m_FileSize;
};

enum
{
	VFILE_PKG = 1,
	VFILE_RAW = 2,
};

class CVFileMgr : public CSingleton<CVFileMgr>
{
public:
	CVFileMgr();
	~CVFileMgr();

	CVFile *OpenFile( const char *path, unsigned int openflag );
	bool IsFileExist( const char *path, unsigned int flags=VFILE_PKG|VFILE_RAW );

public:
	struct stPackage
	{
		char path[256];
		CFixPackage *pkg;
	};
	struct stPackFile
	{
		CFixPackage *pkg;
		unsigned int flags;
		unsigned int offset;
		unsigned int originsize;
		unsigned int packsize;
	};
	typedef std::pair<std::string, stPackFile> PackPair;

	bool AddPackage( const char *pkgpath );
	stPackage &GetPackage( int i )
	{ 
		assert( i>=0 && i<m_Pkgs.size() );
		return m_Pkgs[i];
	}
	CVFile *GetFreeFileCache( bool bpack );

private:
	std::vector<stPackage>m_Pkgs;
	std::map<std::string, stPackFile>m_PackFiles;
	std::vector<CVFile *>m_RawFileCache;
	std::vector<CVFile *>m_PackFileCache;

	int m_ErrorCode;
	std::string m_ErrorStr;
	CRITICAL_SECTION	m_CriticalSection;
};

inline CVFile *DarkOpenVFile( const char *path, unsigned int flags)
{
	return CVFileMgr::GetSingleton().OpenFile( path, flags );
}

inline CVFile *vfopen( const char *path, const char *mode)
{
	unsigned int flags = 0;
	if( strchr(mode, 'r') ) flags |= CVFile::READ;
	if( strchr(mode, 'w') ) flags |= CVFile::WRITE;
	if( strchr(mode, 't') ) flags |= CVFile::TEXT;

	return CVFileMgr::GetSingleton().OpenFile( path, flags );
}

inline void vfclose( CVFile *fp )
{
	fp->Close();
//	fp->Release();
}

inline int vfread( void *pbuffer, int size, int count, CVFile *fp )
{ 
	int cur = fp->TellPos();
	fp->Read(pbuffer, count*size);
	cur = fp->TellPos() - cur;
	return cur;
}

inline int vfwrite( void *pbuffer, int size, int count, CVFile *fp )
{
	if( fp->Write(pbuffer, size*count) ) return count*size;
	else return 0;
}

//add+wangqi
#include <stdarg.h>
inline int vfprintf( CVFile *fp, const char *format, ...)
{
	char		pbuffer[1024];
	
	//定义参数列表
	va_list		argptr;
	
	//定义参数列表从format之后开始
	va_start (argptr,format);

	//将参数列表按照format指定的方式格式化
	_vsnprintf(pbuffer,sizeof(pbuffer),format,argptr);
	
	//参数列表结束
	va_end (argptr);
	
	if( fp->Write(pbuffer, sizeof(pbuffer)) ) return sizeof(pbuffer);
	else return 0;
}


inline int vfseek( CVFile *fp, long offset, int origin )
{
	unsigned int seekfrom = ( origin==SEEK_SET ? CVFile::SEEKBEGIN : (origin==SEEK_CUR ? CVFile::SEEKCUR : CVFile::SEEKEND) );
	fp->Seek( offset, seekfrom );
	return 0;
}

inline int vftell( CVFile *fp )
{
	return fp->TellPos();
}

//add+lincoln
//[2004:11:8] [15:01]
inline int vfeof( CVFile *fp )
{
	return (fp->TellPos()==fp->GetFileSize());
}

extern std::string PathGetFileName( const std::string &path );
extern std::string PathGetFileTitle( const std::string &path );
extern std::string PathGetFileDir( const std::string &path );
extern std::string PathGetFileExten( const std::string &path );

extern void PathGetFileName( char *retbuf, const char *path );
extern void PathGetFileTitle( char *retbuf, const char *path );
extern void PathGetFileDir( char *retbuf, const char *path );
extern void PathGetFileExten( char *retbuf, const char *path );

extern void PrepareFileCreate( const char *path );
extern void AddPkgFile(const char* szPkgFile, const char* szPath, bool bIncludePath, const char* extfilter, bool bDelAfterPKG);

#endif