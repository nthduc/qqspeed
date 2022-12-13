/* unzip.h -- IO for uncompress .zip files using zlib 
   Version 0.15 beta, Mar 19th, 1998,

   Copyright (C) 1998 Gilles Vollant

   This unzip package allow extract file from .ZIP file, compatible with PKZip 2.04g
     WinZip, InfoZip tools and compatible.
   Encryption and multi volume ZipFile (span) are not supported.
   Old compressions used by old PKZip 1.x are not supported

   THIS IS AN ALPHA VERSION. AT THIS STAGE OF DEVELOPPEMENT, SOMES API OR STRUCTURE
   CAN CHANGE IN FUTURE VERSION !!
   I WAIT FEEDBACK at mail info@winimage.com
   Visit also http://www.winimage.com/zLibDll/unzip.htm for evolution

   Condition of use and distribution are the same than zlib :

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.


*/
/* for more info about .ZIP format, see 
      ftp://ftp.cdrom.com/pub/infozip/doc/appnote-970311-iz.zip
   PkWare has also a specification at :
      ftp://ftp.pkware.com/probdesc.zip */

// modified by Tadeusz Dracz - 01.2000:
//		- added class' wrapers
//		- several bugs fixed
//		- several enhancements added
//		- MFC support added
//		- memory leaks eliminated when read error occured
//		- automaticaly free used memory on destruction or exception
//		- modern error notification using exceptions
// Copyright (C) 2000 Tadeusz Dracz  
// This notice may not be removed or altered from any source distribution.



// UnzipFile.h: interface for the CUnzipFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNZIPFILE_H__44D45340_D71F_11D3_B7C7_E77339672847__INCLUDED_)
#define AFX_UNZIPFILE_H__44D45340_D71F_11D3_B7C7_E77339672847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZUBaseFile.h"

/* unz_global_info structure contain global data about the ZIPfile
   These data comes from the end of central dir */
struct unz_global_info
{
	uLong number_entry;         /* total number of entries in
				       the central dir on this disk */
	uLong size_comment;         /* size of the global comment of the zipfile */
};

/* unz_file_info contain information about a file in the zipfile */
struct unz_file_info
{
    uLong version;              /* version made by                 2 bytes */
    uLong version_needed;       /* version needed to extract       2 bytes */
    uLong flag;                 /* general purpose bit flag        2 bytes */
    uLong compression_method;   /* compression method              2 bytes */
    uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
    uLong crc;                  /* crc-32                          4 bytes */
    uLong compressed_size;      /* compressed size                 4 bytes */ 
    uLong uncompressed_size;    /* uncompressed size               4 bytes */ 
    uLong size_filename;        /* filename length                 2 bytes */
    uLong size_file_extra;      /* extra field length              2 bytes */
    uLong size_file_comment;    /* file comment length             2 bytes */

    uLong disk_num_start;       /* disk number start               2 bytes */
    uLong internal_fa;          /* internal file attributes        2 bytes */
    uLong external_fa;          /* external file attributes        4 bytes */

    CTime tmu_date;
};


/* unz_file_info_interntal contain internal info about a file in zipfile*/
struct unz_file_info_internal
{
    uLong offset_curfile;/* relative offset of local header 4 bytes */
};


/* file_in_zip_read_info_s contain internal information about a file in zipfile,
    when reading and decompress it */
struct file_in_zip_read_info
{
	file_in_zip_read_info();
	~file_in_zip_read_info();
	char  *read_buffer;         /* internal buffer for compressed data */
	z_stream stream;            /* zLib stream structure for inflate */

	uLong pos_in_zipfile;       /* position in byte on the zipfile, for fseek*/
	uLong stream_initialised;   /* flag set if stream structure is initialised*/

	uLong offset_local_extrafield;/* offset of the local extra field */
	uInt  size_local_extrafield;/* size of the local extra field */
	uLong pos_local_extrafield;   /* position in the local extra field in read*/

	uLong crc32;                /* crc32 of all data uncompressed */
	uLong crc32_wait;           /* crc32 we must obtain after decompress all */
	uLong rest_read_compressed; /* number of byte to be decompressed */
	uLong rest_read_uncompressed;/*number of byte to be obtained after decomp*/
// 	CFile* file;                 /* io structore of the zipfile */
	uLong compression_method;   /* compression method (0==store) */
	uLong byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
};




/* unz_s contain internal information about the zipfile
*/
struct unz_s
{
	unz_s();
	~unz_s();
	CFile file;                 /* io structore of the zipfile */
	unz_global_info gi;       /* public global information */
	uLong byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
	uLong num_file;             /* number of the current file in the zipfile*/
	uLong pos_in_central_dir;   /* pos of the current file in the central dir*/
	uLong current_file_ok;      /* flag about the usability of the current file*/
	uLong central_pos;          /* position of the beginning of the central dir*/

	uLong size_central_dir;     /* size of the central directory  */
	uLong offset_central_dir;   /* offset of start of central directory with
								   respect to the starting disk number */

	unz_file_info cur_file_info; /* public info about the current file in zip*/
	unz_file_info_internal cur_file_info_internal; /* private info about it*/
    file_in_zip_read_info* pfile_in_zip_read; /* structure about the current
	                                    file if we are decompressing it */
public:
	void free_pfile_in_zip_read();
	void alloc_pfile_in_zip_read();
};



class CUnzipFile : public CZUBaseFile
{
public:
/*
 fill in attibutes and time fields of the fileStatus with unz_file_info data
 WARNING: if the file is opened the function closes it
*/
	void UpdateFileStatus(CFile & f, unz_file_info &ui);
	CUnzipFile();
	void Open(LPCTSTR lpszPath);
/*
  Give the current position in uncompressed data
*/
	z_off_t tell();
/*
  return true if the end of file was reached, false elsewhere 
*/
	bool eof();
/*
  Get the global comment string of the ZipFile, in the szComment buffer.
  uSizeBuf is the size of the szComment buffer.
  return the number of byte copied
*/
	int GetGlobalComment (char* szComment, uLong uSizeBuf);
/*
  Read extra field from the current file (opened by OpenCurrentFile)
  This is the local-header version of the extra field (sometimes, there is
    more info in the local-header version than in the central-header)

  if buf==NULL, it return the size of the local extra field

  if buf!=NULL, len is the size of the buffer, the extra header is copied in
	buf.
  the return value is the number of bytes copied in buf
*/
	int GetLocalExtrafield (void* buf, UINT len);
/*
  Get Info about the current file
  if file_info structure will contain somes info about
	    the current file
  if szFileName!=NULL, the filemane string will be copied in szFileName
			(fileNameBufferSize is the size of the buffer)
  if extraField!=NULL, the extra field information will be copied in extraField
			(extraFieldBufferSize is the size of the buffer).
			This is the Central-header version of the extra field
  if szComment!=NULL, the comment string of the file will be copied in szComment
			(commentBufferSize is the size of the buffer)
*/
	void GetCurrentFileInfo ( unz_file_info* file_info, LPSTR szFileName = NULL, 
		uLong fileNameBufferSize = 0, void *extraField = NULL, uLong extraFieldBufferSize = 0,
		LPSTR szComment = NULL, uLong commentBufferSize = 0);
/*
  Write info about the ZipFile in the *pglobal_info structure.
  No preparation of the structure is needed
*/
	void GetGlobalInfo(unz_global_info & global_info);
	
/***************************************************************************/
/* Unzip package allow you browse the directory of the zipfile */

/*
  Set the current file of the zipfile to the first file.
*/
	void GoToFirstFile();
/*
  Set the current file of the zipfile to the next file.
  return true if there is no problem
  return false if the actual file was the latest.
*/
	bool GoToNextFile();
/*
  Try locate the file szFileName in the zipfile.
  For the iCaseSensitivity signification, see unzStringFileNameCompare

  return value :
  true if the file is found. It becomes the current file.
  false if the file is not found
*/
	bool LocateFile(CString szFileName, bool bCaseSensitive = false);
/*
  Open for reading data the current file in the zipfile.
*/
	void OpenCurrentFile();
/*
  Close the file in zip opened with OpenCurrentFile
  throw badCrc error if all the file was read but the CRC is not good
*/
	void CloseCurrentFile();
/*
  Read bytes from the current file (opened by OpenCurrentFile)
  buf contain buffer where data must be copied
  len the size of buf.

  return the number of byte copied if somes bytes are copied
  return 0 if the end of file was reached
*/
	int ReadCurrentFile(void* buf, UINT len);

	void Close();
	int StringFileNameCompare(CString fileName1, CString fileName2, bool caseSensitive = false);
	CUnzipFile(LPCTSTR lpszPath);
	virtual ~CUnzipFile();
protected:
	void unzlocal_GetCurrentFileInfoInternal( unz_file_info & file_info, 
		unz_file_info_internal & file_info_internal, LPSTR szFileName,
		uLong fileNameBufferSize, void *extraField, uLong extraFieldBufferSize,
		LPSTR szComment, uLong commentBufferSize);

	void unzlocal_DosDateToTmuDate(unz_file_info & file_info);
	void unzlocal_CheckCurrentFileCoherencyHeader (uInt & iSizeVar, uLong & offset_local_extrafield, uLong & size_local_extrafield);
	uLong unzlocal_SearchCentralDir();
	void unzlocal_getLong (uLong & pX);
	void unzlocal_getShort (uLong & pX);
	void unzlocal_getByte(int & pi);
	unz_s uf;

};

#endif // !defined(AFX_UNZIPFILE_H__44D45340_D71F_11D3_B7C7_E77339672847__INCLUDED_)
