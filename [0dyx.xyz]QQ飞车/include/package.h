
#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include <windows.h>

const int STREAM_SET = 100;
const int STREAM_CUR = 101;
const int STREAM_END = 102;
//typedef unsigned int SIZE_T;


/**
 * 对指针指向内存的一段包装
 */
/*
class StreamPtr
{
public:
	StreamPtr() : m_pBase(NULL), m_Range(0), m_CurPos(0){}
	StreamPtr( void *ptr, SIZE_T datalen ) : m_pBase(ptr), m_Range(datalen), m_CurPos(0){}

	void Reset( void *ptr, SIZE_T datalen )
	{
		m_pBase = ptr;
		m_Range = datalen;
		m_CurPos = 0;
	}

	void Read( void * pbuf, SIZE_T datalen )
	{ 
		assert( m_CurPos+datalen <= m_Range );
		memcpy( pbuf, GetCurPtr(), datalen );
		m_CurPos += datalen;
	}

	void Write( const void * pbuf, SIZE_T datalen )
	{
		assert( m_CurPos+datalen <= m_Range );
		memcpy( GetCurPtr(), pbuf, datalen );
		m_CurPos += datalen;
	}
	SIZE_T Seek( int offset, int seekfrom )
	{
		switch( seekfrom )
		{
		case STREAM_SET: m_CurPos = offset; break;
		case STREAM_CUR: m_CurPos += offset; break;
		case STREAM_END: m_CurPos = m_Range+offset; break;
		default: assert(0); break;
		}
		assert( m_CurPos <= m_Range );
		return m_CurPos;
	}

	template<typename T>
	void   Read( T &data )
	{ 
		assert( m_CurPos+sizeof(T) <= m_Range );
		data = *(T *)GetCurPtr();
		m_CurPos += sizeof(T); 
	}

	template<typename T>
	void   Write( const T &data )
	{ 
		assert( m_CurPos+sizeof(T) <= m_Range );
		*(T *)GetCurPtr() = data;
		m_CurPos += sizeof(T);
	}

	SIZE_T TellPos(){ return m_CurPos; }
	SIZE_T GetStreamSize(){ return m_Range; }
	void *GetBasePtr(){ return m_pBase; }
	void *GetCurPtr(){ return (void *)((BYTE *)m_pBase + m_CurPos); }

private:
	void *m_pBase;
	SIZE_T m_Range;
	SIZE_T m_CurPos;
};
*/

struct stPkgFile
{
	std::string name;
	unsigned int flags;
	int offset;
	int originsize;
	int packsize;
};

struct stPkgHeader
{
	int version;
	int numfiles;
	int table_offset;
	int table_len;
};

//用于只读的package
class CFixPackage
{
public:
	~CFixPackage()
	{
		Close();
	}
	bool Open( const char *packpath );
	void Close();

	bool FindFirstFile( stPkgFile &obj );
	bool FindNextFile( stPkgFile &obj );

	bool GetFileData( void *pbuf, int originsize, int offset, int packsize );
	void *GetFilePackData( int offset, int packsize );
	int  GetNumFiles(){ return m_PkgDesc.numfiles; }
	int  GetFileSize(){ return m_FileSize; }

private:
	void  *m_pMemBuffer;
	unsigned int m_MemBufLen;

	FILE *m_FP;
	unsigned int m_FileSize;

	stPkgHeader m_PkgDesc;
	int    m_iCurFile;
	int    m_CurOffset;
};

//用于添加文件的的package
class CFlexPackage
{
public:
	bool Open( const char *packpath );
	bool Close();

	bool AddPackFile( const char *pfilepath, const void *pfiledata, int originsize, int packsize, unsigned int flags );

private:
	FILE *m_FP;
	std::vector<stPkgFile>m_FileTable;
	stPkgHeader m_PkgDesc;
};

#endif //__PACKAGE_H__