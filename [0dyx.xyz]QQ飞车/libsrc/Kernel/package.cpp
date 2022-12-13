
#include "zlib.h"
#include <string>
#include <vector>
#include <assert.h>
#include "package.h"
#include "k_debug.h"

bool CFixPackage::Open( const char *packpath )
{
	m_MemBufLen = 1024*1024;
	m_pMemBuffer = malloc( m_MemBufLen );

	m_FP = fopen( packpath, "rb" );
	if( m_FP == NULL )
	{
		ReportError( "open package(%s) failed", packpath );
		return false;
	}

	fseek( m_FP, 0, SEEK_END );
	m_FileSize = ftell(m_FP);

	if( m_FileSize == 0 )
	{
		fclose(m_FP);
		m_FP = NULL;
		return false;
	}

	fseek( m_FP, 0, SEEK_SET );
	if( fread(&m_PkgDesc, sizeof(m_PkgDesc), 1, m_FP) != 1 )
	{
		fclose(m_FP);
		m_FP = NULL;
		return false;
	}

	return true;
}

void CFixPackage::Close()
{
	if( m_pMemBuffer ) free( m_pMemBuffer );
	if( m_FP ) fclose(m_FP);
}

bool CFixPackage::FindFirstFile( stPkgFile &obj )
{
	if( m_PkgDesc.numfiles == 0 ) return false;

	m_iCurFile = -1;
	m_CurOffset = m_PkgDesc.table_offset;
	return FindNextFile(obj);
}

bool CFixPackage::FindNextFile( stPkgFile &obj )
{
	m_iCurFile++;
	if( m_iCurFile == m_PkgDesc.numfiles ) return false;

	fseek( m_FP, m_CurOffset, SEEK_SET );

	short namelen;
	fread( &namelen, sizeof(short), 1, m_FP );
	char buffer[1024];
	assert( namelen < sizeof(buffer) );
	fread( buffer, namelen, 1, m_FP );
	buffer[namelen] = 0;
	obj.name = buffer;

	fread( &obj.flags, sizeof(obj.flags), 1, m_FP );
	fread( &obj.offset, sizeof(obj.offset), 1, m_FP );
	fread( &obj.originsize, sizeof(obj.originsize), 1, m_FP );
	fread( &obj.packsize, sizeof(obj.packsize), 1, m_FP );

	m_CurOffset = ftell(m_FP);
	return true;
}

bool CFixPackage::GetFileData( void *pbuf, int originsize, int offset, int packsize )
{
	fseek( m_FP, offset, SEEK_SET );

	void *pmembuf = m_pMemBuffer;
	if( packsize > m_MemBufLen ) pmembuf = malloc( packsize );

	bool bret = false;
	if( fread(pmembuf, packsize, 1, m_FP) == 1 )
	{
		uLongf destlen = originsize;
		bret = (uncompress((unsigned char *)pbuf, &destlen, (const unsigned char *)pmembuf, packsize) == Z_OK);
	}

	if( pmembuf != m_pMemBuffer ) free( pmembuf );
	return bret;
}

void *CFixPackage::GetFilePackData( int offset, int packsize )
{
	if( packsize > m_MemBufLen )
	{
		free( m_pMemBuffer );
		m_pMemBuffer = malloc( packsize );
		m_MemBufLen = packsize;
	}

	fseek( m_FP, offset, SEEK_SET );

	fread( m_pMemBuffer, packsize, 1, m_FP );
	return m_pMemBuffer;
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
bool CFlexPackage::Open( const char *packpath )
{
	m_FP = fopen( packpath, "rb");
	if( m_FP == NULL )
	{
		m_FP = fopen( packpath, "wb" );
		if( m_FP == NULL ) return false;

		m_PkgDesc.numfiles = 0;
		m_PkgDesc.version = 100;
		m_PkgDesc.table_offset = sizeof(m_PkgDesc);
		m_PkgDesc.table_len = 0;

		if( fwrite(&m_PkgDesc, sizeof(m_PkgDesc), 1, m_FP) != 1 )
		{
			fclose(m_FP);
			m_FP = NULL;
			return false;
		}
		else fclose(m_FP);
	}
	else
	{
		fread( &m_PkgDesc, sizeof(m_PkgDesc), 1, m_FP );
		fseek( m_FP, m_PkgDesc.table_offset, SEEK_SET );

		char buffer[4096];
		m_FileTable.resize(m_PkgDesc.numfiles);
		for( int i=0; i<m_PkgDesc.numfiles; i++ )
		{
			stPkgFile &obj = m_FileTable[i];
			short len;
			fread( &len, sizeof(short), 1, m_FP );
			assert( len < sizeof(buffer) );
			fread( buffer, len, 1, m_FP );
			buffer[len] = 0;
			obj.name = buffer;
			fread( &obj.flags, sizeof(int), 1, m_FP );
			fread( &obj.offset, sizeof(int), 1, m_FP );
			fread( &obj.originsize, sizeof(int), 1, m_FP );
			fread( &obj.packsize, sizeof(int), 1, m_FP );
		}
		fclose(m_FP);
	}

	m_FP = fopen( packpath, "r+b" );
	if( m_FP == NULL ) return false;

	return true;
}

static bool SafeWriteData( const void *pdata, int datalen, FILE *fp )
{
	if( fwrite(pdata, datalen, 1, fp) != 1 )
	{
		fclose(fp);
		return false;
	}
	else return true;
}

bool CFlexPackage::Close()
{
	if( m_FP == NULL ) return false;

	assert( m_FileTable.size() == m_PkgDesc.numfiles );
	if( m_PkgDesc.numfiles > 0 )
	{
		fseek( m_FP, m_PkgDesc.table_offset, SEEK_SET );

		for( int i=0; i<m_PkgDesc.numfiles; i++ )
		{
			stPkgFile &obj = m_FileTable[i];
			short len = obj.name.length();
			if( !SafeWriteData(&len, sizeof(short), m_FP) ) return false;
			if( !SafeWriteData(obj.name.c_str(), len, m_FP) ) return false;
			if( !SafeWriteData(&obj.flags, sizeof(int), m_FP) ) return false;
			if( !SafeWriteData(&obj.offset, sizeof(int), m_FP) ) return false;
			if( !SafeWriteData(&obj.originsize, sizeof(int), m_FP) ) return false;
			if( !SafeWriteData(&obj.packsize, sizeof(int), m_FP) ) return false;
		}

		m_PkgDesc.table_len = ftell(m_FP) - m_PkgDesc.table_offset;
	}

	fseek( m_FP, 0, SEEK_SET );
	if( !SafeWriteData(&m_PkgDesc, sizeof(m_PkgDesc), m_FP) ) return false;

	fclose(m_FP);
	return true;
}

bool CFlexPackage::AddPackFile( const char *pfilepath, const void *pfiledata, int originsize, int packsize, unsigned int flags )
{
	std::string path(pfilepath);

	int i;
	for( i=0; i<m_PkgDesc.numfiles; i++ )
	{
		stPkgFile &obj = m_FileTable[i];
		if( obj.name == path ) break;
	}

	if( i == m_PkgDesc.numfiles )
	{
		m_PkgDesc.numfiles++;
		m_FileTable.resize( m_PkgDesc.numfiles );
	}

	stPkgFile &obj = m_FileTable[i];
	obj.name = path;
	obj.flags = flags;
	obj.offset = m_PkgDesc.table_offset;
	obj.originsize = originsize;
	obj.packsize = packsize;

	fseek( m_FP, obj.offset, SEEK_SET );
	if( fwrite(pfiledata, packsize, 1, m_FP) != 1 ) return false;

	m_PkgDesc.table_offset += packsize;
	return true;
}

/*
int main()
{
	CFixPackage pkg;
	pkg.Open( "1.1.1.17.pkg" );

	stPkgFile obj;
	bool b = pkg.FindFirstFile( obj );
	while( b )
	{
		b = pkg.FindNextFile( obj );
	}

	//bool GetFileData( void *pbuf, int originsize, int offset, int packsize );

	pkg.Close();
	return 0;
}
*/