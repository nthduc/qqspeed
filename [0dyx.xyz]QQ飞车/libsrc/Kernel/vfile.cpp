
#include <stdio.h>
#include <windows.h>
#include "vfile.h"
#include <string>
#include "k_debug.h"
#include "package.h"
#include "zlib.h"
#include "kernel.h"

#pragma comment(lib,"zdll.lib")

static bool IsPathExist( const char *path )
{
	WIN32_FIND_DATA find; 

	HANDLE hsearch = FindFirstFile( path, &find );
	if( hsearch == INVALID_HANDLE_VALUE )
	{
		return false;
	}
	else
	{
		FindClose(hsearch);
		return true;
	}
}
//------------------------------------------------------------------------------------------
//--------------------------------------class CVFile----------------------------------------
//------------------------------------------------------------------------------------------
CVFile::CVFile() : m_OpenMode(NONE)
{
}

CVFile::~CVFile()
{
}

void CVFile::Close()
{
	m_OpenMode = NONE;
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
std::string CVFile::GetFileName()
{
	return m_Path.substr( GetFileNamePos(m_Path) );
}

std::string CVFile::GetFileTitle()
{
	return PathGetFileTitle( m_Path );
}

std::string CVFile::GetFileDir()
{
	return m_Path.substr( 0, GetFileNamePos(m_Path) );
}

std::string CVFile::GetFilePath()
{
	return m_Path;
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

std::string PathGetFileExten( const std::string &path )
{
	STR_POS pos = path.find_last_of( '.' );
	return path.substr(pos);
}

static const char *GetFileNamePos( const char *path )
{
	int index = strlen(path) - 1;
	while( index >= 0 )
	{
		if( path[index]=='/' || path[index]=='\\' ) break;
		index--;
	}
	return &path[index+1];
}

static const char *GetFileExtenPos( const char *path )
{
	const char *p = strrchr( path, '.' );
	if( p == NULL ) p = &path[strlen(path)];
	return p;
}

static void GetSubString( char *retbuf, const char *pstart, const char *pend )
{
	int len = pend - pstart;
	if( len < 0 ) len = 0;
	if( len ) memcpy( retbuf, pstart, len );
	retbuf[len] = 0;
}

void PathGetFileName( char *retbuf, const char *path )
{
	GetSubString( retbuf, GetFileNamePos(path), &path[strlen(path)] );
}

void PathGetFileTitle( char *retbuf, const char *path )
{
	GetSubString( retbuf, GetFileNamePos(path), GetFileExtenPos(path) );
}

void PathGetFileDir( char *retbuf, const char *path )
{
	GetSubString( retbuf, path, GetFileNamePos(path) );
}

void PathGetFileExten( char *retbuf, const char *path )
{
	GetSubString( retbuf, GetFileExtenPos(path)+1, &path[strlen(path)] );
}

//------------------------------------------------------------------------------------------
//--------------------------------------class CVDiskFile------------------------------------
//------------------------------------------------------------------------------------------
class CVDiskFile : public CVFile
{
	friend class CVFileMgr;
public:
	CVDiskFile();
	virtual ~CVDiskFile();
	bool Open( const char *path, unsigned int openflag );
	
	virtual void Close();
	virtual bool Read( void *pbuf, unsigned int buflen );
	virtual bool Write( void *pbuf, unsigned int buflen );
	virtual int  Seek( int offset, unsigned int seekfrom );

protected:
	FILE *m_FP;
};

CVDiskFile::CVDiskFile()
{
	m_FP = 0;
}

CVDiskFile::~CVDiskFile()
{
	if( m_FP ) fclose( m_FP );
}

void CVDiskFile::Close()
{
	if( m_FP )
	{
		fclose( m_FP );
		m_FP = 0;
	}
	CVFile::Close();
}

bool CVDiskFile::Open( const char *path, unsigned int openflag )
{
	char openstr[3] = "rb";
	if( openflag & CVFile::WRITE ) openstr[0] = 'w';
	if( openflag & CVFile::TEXT  ) openstr[1] = 't';
	FILE *fp = fopen( path, openstr );
	if( fp == NULL )
	{
		//s_ReportError( "%s----不存在", fullpath );
		return false;
	}

	m_FP = fp;
	m_OpenMode = openflag;
	fseek( fp, 0, SEEK_END );
	m_FileSize = ftell(fp);
	fseek( fp, 0, SEEK_SET );

	return true;
}

bool CVDiskFile::Read( void *pbuf, unsigned int buflen )
{
//	OutputDebugString(m_szPath);

	if( fread(pbuf, 1, buflen, m_FP) == buflen ) return true;
	else return false;
}

bool CVDiskFile::Write( void *pbuf, unsigned int buflen )
{
	if( fwrite(pbuf, 1, buflen, m_FP) == buflen )
	{
		fflush( m_FP );
		return true;
	}
	else return false;


}

int  CVDiskFile::Seek( int offset, unsigned int seekfrom )
{
	int origin = (seekfrom==SEEKBEGIN) ? SEEK_SET : (seekfrom==SEEKCUR ? SEEK_CUR : SEEK_END );
	fseek( m_FP, offset, origin );
	return ftell( m_FP );
}

//------------------------------------------------------------------------------------------
//--------------------------------------class CVPackage-------------------------------------
//------------------------------------------------------------------------------------------
unsigned int HashString(const char *string)
{
	unsigned int result = 0;
	int place = 0;
	while (*string)
	{
		char string_value = isupper(*string) ? tolower(*string) : *string;
		result += (string_value << (place * 8));
		string++;
		place++;
		if (place > 3) place = 0;
	}
	return result;
}

//------------------------------------------------------------------------------------------
//--------------------------------------class CVPackFile------------------------------------
//------------------------------------------------------------------------------------------
class CVPackFile : public CVFile
{
	friend class CVFileMgr;
public:
	CVPackFile();
	virtual ~CVPackFile();

	bool Open( CVFileMgr::stPackFile &packobj, unsigned int flag );

	virtual void Close();
	virtual bool Read( void *pbuf, unsigned int buflen );
	virtual bool Write( void *pbuf, unsigned int buflen );
	virtual int  Seek( int offset, unsigned int seekfrom );

protected:
	void CheckMemImage( int datalen );

	unsigned int m_Offset; //relative to file beginning
	void *m_pMemImage;
	int   m_MemImgSize;
};

CVPackFile::CVPackFile() : m_pMemImage(NULL), m_MemImgSize(0)
{
}

CVPackFile::~CVPackFile()
{
	if( m_pMemImage ) free( m_pMemImage );
}

void CVPackFile::CheckMemImage( int datalen )
{
	if( m_MemImgSize < datalen )
	{
		if( m_pMemImage ) free( m_pMemImage );
		m_pMemImage = malloc( datalen );
		m_MemImgSize = datalen;
	}
}

bool CVPackFile::Open( CVFileMgr::stPackFile &packobj, unsigned int flag )
{
	CheckMemImage( packobj.originsize );
	packobj.pkg->GetFileData( m_pMemImage, packobj.originsize, packobj.offset, packobj.packsize );

	m_FileSize = packobj.originsize;
	m_OpenMode = flag;
	m_Offset = 0;

	return true;
}

void CVPackFile::Close()
{
	if( m_MemImgSize > 1024*1024 ) //释放太大的内存
	{
		free( m_pMemImage );
		m_pMemImage = NULL;
		m_MemImgSize = 0;
	}

	CVFile::Close();
}

bool CVPackFile::Read( void *pbuf, unsigned int buflen )
{
	if( m_Offset+buflen > m_FileSize )
	{
		buflen = m_FileSize - m_Offset;
	}

	memcpy( pbuf, (unsigned char *)m_pMemImage+m_Offset, buflen );
	m_Offset += buflen;

	return true;
}

bool CVPackFile::Write( void *pbuf, unsigned int buflen )
{
	assert(0);
	return false;
}

int  CVPackFile::Seek( int offset, unsigned int seekfrom )
{
	if( seekfrom == SEEKBEGIN ) m_Offset = offset;
	else if( seekfrom == SEEKCUR ) m_Offset += offset;
	else
	{
		assert( seekfrom == SEEKEND );
		m_Offset = m_FileSize - offset;
	}
	assert( m_Offset>=0 && m_Offset<=m_FileSize );
	return m_Offset;
}

//------------------------------------------------------------------------------------------
//---------------------------------------class CVFileMgr------------------------------------
//------------------------------------------------------------------------------------------
CRITICAL_SECTION s_CriticalSection;

CVFileMgr::CVFileMgr()
{
	InitializeCriticalSection( &s_CriticalSection );

	FILE *fp = fopen( "data\\package.cfg", "rt" );
	if( fp != NULL )
	{
		char buffer[256], filename[256], path[256];
		while( fgets(buffer, sizeof(buffer), fp) )
		{
			sscanf( buffer, "%s", filename );
			if( filename[0] == 0 ) continue;

			sprintf( path, "data\\%s", filename );
			if( IsPathExist(path) && !AddPackage(path) )
			{
//				ReportError( "Open package error: %s", path );
			}
		}

		fclose(fp);
	}
  
	int i;
	m_RawFileCache.resize(10);
	for( i=0; i<m_RawFileCache.size(); i++ ) m_RawFileCache[i] = new CVDiskFile;

	m_PackFileCache.resize(10);
	for( i=0; i<m_PackFileCache.size(); i++ ) m_PackFileCache[i] = new CVPackFile;
}

CVFileMgr::~CVFileMgr()
{
	int i;

	for( i=0; i<m_RawFileCache.size(); i++ )
	{
		assert( m_RawFileCache[i]->m_OpenMode == CVFile::NONE );
		delete m_RawFileCache[i];
	}
	for( i=0; i<m_PackFileCache.size(); i++ )
	{
		assert( m_PackFileCache[i]->m_OpenMode == CVFile::NONE );
		delete m_PackFileCache[i];
	}

	for( i=0; i<m_Pkgs.size(); i++ )
	{
		m_Pkgs[i].pkg->Close();
		delete m_Pkgs[i].pkg;
	}

	DeleteCriticalSection( &s_CriticalSection );
}

static void GetCleanPath( std::string &str, const char *psrc )
{
	char buffer[1024];
	const char *pcur = psrc;
	char *pdst = buffer;
	while( *pcur )
	{
		int c = *pcur;
		if( c == '/' ) *pdst = '\\';
		else if( c>='A' && c<='Z' ) *pdst = c-'A' + 'a';
		else *pdst = *pcur;

		pdst++;
		pcur++;
	}
	*pdst = 0;
	assert( pdst-buffer < sizeof(buffer) );
	str = buffer;
}

bool CVFileMgr::AddPackage( const char *pkgpath )
{
	CFixPackage *pkg = new CFixPackage;
	if( !pkg->Open(pkgpath) )
	{
		delete pkg;
		return false;
	}

	std::string cleanpath;
	stPkgFile fileobj;
	bool bfind = pkg->FindFirstFile(fileobj);
	while( bfind )
	{
		stPackFile packfile;

		packfile.pkg = pkg;
		packfile.flags = fileobj.flags;
		packfile.offset = fileobj.offset;
		packfile.originsize = fileobj.originsize;
		packfile.packsize = fileobj.packsize;

		GetCleanPath( cleanpath, fileobj.name.c_str() );
		m_PackFiles[cleanpath] = packfile;

		bfind = pkg->FindNextFile(fileobj);
	}

	stPackage pkgobj;
	strcpy( pkgobj.path, pkgpath );
	pkgobj.pkg = pkg;
	m_Pkgs.push_back( pkgobj );

	return true;
}

CVFile *CVFileMgr::GetFreeFileCache( bool bpack )
{
	std::vector<CVFile *> &array = bpack ? m_PackFileCache : m_RawFileCache;

	std::vector<CVFile *>::iterator iter = array.begin();
	for(; iter!=array.end(); iter++ )
	{
		if( (*iter)->m_OpenMode == CVFile::NONE ) return *iter;
	}

	assert(0);
	return NULL;
}

inline char *DKstrlwr( char *str )
{
	return _strlwr( str );
}
inline char *DKstrlwr( char *pdest, const char *psrc )
{
	strcpy( pdest, psrc );
	return _strlwr( pdest );
}

static void MyCleanFileName( char *pdest, const char *psrc )
{
	DKstrlwr( pdest, psrc );
	int i;
	int namelen = strlen(pdest);
	for( i=0; i<namelen; i++ )
	{
		if( pdest[i] == '/' ) pdest[i] = '\\';
	}
}

class MyFileMutex
{
public:
	MyFileMutex()
	{
		EnterCriticalSection( &s_CriticalSection );
	}
	~MyFileMutex()
	{
		LeaveCriticalSection( &s_CriticalSection );
	}
};

CVFile *CVFileMgr::OpenFile( const char *path, unsigned int openflag )
{
	MyFileMutex mymutex;

#ifdef GAMEVER_PUBLIC
	if( (openflag&CVFile::WRITE) == 0 )
	{
		std::string newpath;
		GetCleanPath( newpath, path );

		std::map<std::string, stPackFile>::iterator iter = m_PackFiles.find(newpath);
		if( iter != m_PackFiles.end() )
		{
			CVPackFile *pfile = (CVPackFile *)GetFreeFileCache( true );

			if( pfile->Open(iter->second, openflag) )
			{
				pfile->m_Path = newpath;
				return pfile;
			}
			else return NULL;
		}
	}

	//open as current directory harddisk file
	CVDiskFile *pfile = (CVDiskFile *)GetFreeFileCache( false );
	if( pfile->Open(path, openflag) )
	{
		pfile->m_Path = path;
		return pfile;
	}
#else

	//open as current directory harddisk file
	CVDiskFile *pfile = (CVDiskFile *)GetFreeFileCache( false );
	if( pfile->Open(path, openflag) )
	{
		pfile->m_Path = path;
		return pfile;
	}

	if( (openflag&CVFile::WRITE) == 0 )
	{
		std::string newpath;
		GetCleanPath( newpath, path );

		std::map<std::string, stPackFile>::iterator iter = m_PackFiles.find(newpath);
		if( iter != m_PackFiles.end() )
		{
			CVPackFile *pfile = (CVPackFile *)GetFreeFileCache( true );

			if( pfile->Open(iter->second, openflag) )
			{
				pfile->m_Path = newpath;
				return pfile;
			}
			else return NULL;
		}
	}
#endif

	return NULL;
}

bool CVFileMgr::IsFileExist( const char *path, unsigned int flags )
{
	MyFileMutex mymutex;

	if( flags & VFILE_PKG )
	{
		std::string newpath;
		GetCleanPath( newpath, path );

		std::map<std::string, stPackFile>::iterator iter = m_PackFiles.find(newpath);
		if( iter != m_PackFiles.end() ) return true;
	}

	if( flags & VFILE_RAW )
	{
		if( IsPathExist(path) ) return true;
	}

	return false;
}

void PrepareFileCreate( const char *path )
{
	char dir[256];
	const char *pcur = path;
	while( *pcur )
	{
		if( *pcur=='/' || *pcur=='\\' )
		{
			int len = pcur - path;
			memcpy( dir, path, len );
			dir[len] = 0;

			if( !IsPathExist(dir) ) CreateDirectory( dir, NULL );
		}
		pcur++;
	}

	if( IsPathExist(path) ) SetFileAttributes( path, FILE_ATTRIBUTE_ARCHIVE );
}


void AddPkgRecursiveFile(const char* szRoot, const char* szPath, CFlexPackage& pkg, bool bIncludePath, bool bDelAfterPKG)
{

	// 读写文件

	FILE *pFile;
	char szFile[MAX_PATH];
	//	sprintf(szFile, "%s\\%s", szRoot, szPath);
	_makepath(szFile, NULL, szRoot, szPath, NULL);

	pFile = fopen(szFile, "rb");

	fseek(pFile, 0, SEEK_END);
	int nSrcLen = 0;
	nSrcLen = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	unsigned char* pFileSrc = new unsigned char[nSrcLen];

	fread(pFileSrc, nSrcLen, 1, pFile);

	unsigned long nDstLen;
	nDstLen = compressBound(nSrcLen);
	unsigned char* pFileDst = new unsigned char[nDstLen];
	if(Z_OK != compress(pFileDst, &nDstLen, pFileSrc, nSrcLen))
	{
		//				TRACE("ERROR ==============================\n");
	}
	else
	{
		char szPathName[MAX_PATH] = "\0";
		if ( bIncludePath )
		{
			char driver[4] = "\0", fpath[MAX_PATH] = "\0", fsubpath[MAX_PATH] = "\0", ext[10] = "\0";
			_splitpath(szRoot, driver, fpath, fsubpath, ext);
			//			if ( strlen(fsubpath) > 0 )
			{
				sprintf(szPathName, "%s\\%s", fsubpath, szPath);
			}
		}
		else
		{
			strcpy(szPathName, szPath);
		}
		pkg.AddPackFile(szPathName, pFileDst, nSrcLen, nDstLen, 0);

	}

	fclose(pFile);
	delete pFileSrc;	

	if ( bDelAfterPKG )
	{
		SetFileAttributes(szFile, FILE_ATTRIBUTE_NORMAL);
		DeleteFile(szFile);
	}
}

void AddPkgRecursivePath(const char* szRoot, const char* szPath, CFlexPackage& pkg, bool bIncludePath, const char* extfilter, bool bDelAfterPKG)
{
	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 
	char szFind[MAX_PATH];

	sprintf(szFind, "%s\\%s\\*.*", szRoot, szPath);

	hSearch = FindFirstFile(szFind,  &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		return;
	} 
	BOOL fFinished = FALSE; 

	while (!fFinished) 
	{ 
		if(strcmp(FileData.cFileName, ".") == 0 || strcmp(FileData.cFileName, "..") == 0)
		{
		}
		else
		{
			char szFile[MAX_PATH];
			sprintf(szFile, "%s\\%s", szPath, FileData.cFileName);
			if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				AddPkgRecursivePath(szRoot, szFile, pkg, bIncludePath, extfilter, bDelAfterPKG);
			}
			else
			{
				char ext[10] = "\0";
				_splitpath(FileData.cFileName, NULL, NULL, NULL, ext);
				if ( !extfilter || strlen(extfilter) == 0 || !stricmp(extfilter, ext) )
					AddPkgRecursiveFile(szRoot, szFile, pkg, bIncludePath, bDelAfterPKG);
			}
		}


		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
			} 
		}
	} 	
}


bool IsDirectory(const char* szPath)
{
	WIN32_FIND_DATA fData;
	HANDLE hFind = FindFirstFile(szPath, &fData);
	if ( hFind == INVALID_HANDLE_VALUE )
		return false;
	return fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

void AddPkgFile(const char* szPkgFile, const char* szPath, bool bIncludePath, const char* extfilter, bool bDelAfterPKG)
{
	CFlexPackage pkg;
	if ( !pkg.Open(szPkgFile) )
		return;

	if ( !IsDirectory(szPath) )
	{
		char driver[4] = "\0", path[MAX_PATH] = "\0", fname[MAX_PATH] = "\0", ext[10] = "\0";
		_splitpath(szPath, driver, path, fname, ext);
		if ( !extfilter || strlen(extfilter) == 0 || !stricmp(extfilter, ext) )
		{
			char pathnew[MAX_PATH] = "\0";
			_makepath(pathnew, driver, path, NULL, NULL);
			char fnamenew[MAX_PATH] = "\0";
			_makepath(fnamenew, NULL, NULL, fname, ext);
			AddPkgRecursiveFile(pathnew, fnamenew, pkg, bIncludePath, bDelAfterPKG);
			pkg.Close();
		}
		return;
	}

	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 
	char szFind[MAX_PATH];

	sprintf(szFind, "%s\\*.*", szPath);

	hSearch = FindFirstFile(szFind,  &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		return;
	} 
	BOOL fFinished = FALSE; 

	while (!fFinished) 
	{ 
		if(strcmp(FileData.cFileName, ".") == 0 || strcmp(FileData.cFileName, "..") == 0)
		{
		}
		else
		{
			if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				AddPkgRecursivePath(szPath, FileData.cFileName, pkg, bIncludePath, extfilter, bDelAfterPKG);
			}
			else
			{
				char ext[10] = "\0";
				_splitpath(FileData.cFileName, NULL, NULL, NULL, ext);
				if ( !extfilter || strlen(extfilter) == 0 || !stricmp(extfilter, ext) )
				{
					AddPkgRecursiveFile(szPath, FileData.cFileName, pkg, bIncludePath, bDelAfterPKG);
				}
			}
		}


		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				//				AfxMessageBox("找完了");
				fFinished = TRUE; 
			} 
			else 
			{ 
				//				AfxMessageBox("出错");
			} 
		}
	} 	

	pkg.Close();

}