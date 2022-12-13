
/* zip.h -- IO for compress .zip files using zlib 
   Version 0.15 alpha, Mar 19th, 1998,

   Copyright (C) 1998 Gilles Vollant

   This unzip package allow creates .ZIP file, compatible with PKZip 2.04g
     WinZip, InfoZip tools and compatible.
   Encryption and multi volume ZipFile (span) are not supported.
   Old compressions used by old PKZip 1.x are not supported

  For uncompress .zip file, look at unzip.h

   THIS IS AN ALPHA VERSION. AT THIS STAGE OF DEVELOPPEMENT, SOMES API OR STRUCTURE
   CAN CHANGE IN FUTURE VERSION !!
   I WAIT FEEDBACK at mail info@winimage.com
   Visit also http://www.winimage.com/zLibDll/zip.htm for evolution

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
      ftp://ftp.pkware.com/probdesc.zip
*/

// modified by Tadeusz Dracz - 01.2000:
//		- added class' wrapers
//		- several bugs fixed
//		- several enhancements added
//		- MFC support added
//		- memory leaks eliminated when write error occured
//		- automaticaly free used memory on destruction or exception
//		- modern error notification using exceptions
// Copyright (C) Tadeusz Dracz  
// This notice may not be removed or altered from any source distribution.

// ZipFile.h: interface for the CZipFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZIPFILE_H__7F795F27_D6BD_11D3_B7C7_BBF03FA53147__INCLUDED_)
#define AFX_ZIPFILE_H__7F795F27_D6BD_11D3_B7C7_BBF03FA53147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZUBaseFile.h"

struct zip_fileinfo
{
	CTime		tmz_date;       /* date in understandable format           */
    uLong		dosDate;       /* if dos_date == 0, tmu_date is used      */
/*    uLong       flag;        */   /* general purpose bit flag        2 bytes */

    uLong       internal_fa;    /* internal file attributes        2 bytes */
    uLong       external_fa;    /* external file attributes        4 bytes */
public:
	uLong get_dos_date();
	zip_fileinfo();
};

struct curfile_info
{
	curfile_info();
	~curfile_info();
	z_stream stream;            /* zLib stream structure for inflate */
    int  stream_initialised;    /* 1 is stream is initialised */
    UINT pos_in_buffered_data;  /* last written byte in buffered_data */

    DWORD pos_local_header;     /* offset of the local header of the file 
                                     currenty writing */
    char* central_header;       /* central header data for the current file */
    DWORD size_centralheader;   /* size of the central header for cur file */
    uLong flag;                 /* flag of the file currently writing */

    int  method;                /* compression method of file currenty wr.*/
    Byte* buffered_data;/* buffer contain compressed data to be writ*/
    uLong dosDate;
    uLong crc32;
public:
	void free_central_header();
	void alloc_central_header();
};

class linkedlist_datablock_internal
{
public:
  linkedlist_datablock_internal();
  ~linkedlist_datablock_internal();
  class linkedlist_datablock_internal* next_datablock;
  uLong  avail_in_this_block;
  uLong  filled_in_this_block;
  uLong  unused; /* for future use and alignement */
  unsigned char* data;
	
};

class linkedlist_data
{
public:
	void add_data_in_datablock(char* buf, uLong len);
	linkedlist_data();
	~linkedlist_data();
	int write_datablock(CFile & f);
    linkedlist_datablock_internal* first_block;
    linkedlist_datablock_internal* last_block;
};

struct zip_internal
{
	zip_internal();
	~zip_internal();
    CFile filezip;
    linkedlist_data central_dir;/* datablock with central dir in construction*/
    int  in_opened_file_inzip;  /* 1 if a file in the zip is currently writ.*/
    struct curfile_info ci;            /* info on the file curretly writing */

    DWORD begin_pos;            /* position of the beginning of the zipfile */
    DWORD number_entry;
};


class CZipFile : public CZUBaseFile
{
public:
	CZipFile();
	void Open(LPCTSTR pathname, bool append);
/*
  Create a zipfile.
	 if the file pathname exist and append is true, the zip will be created at the end
	   of the file. (useful if the file contain a self extractor code)
*/

	CZipFile(LPCTSTR pathname, bool append = false);
/*
  Open a file in the ZIP for writing.
  filename : the filename in zip (if NULL, '-' without quote will be used
  *zipfi contain supplemental information
  if extrafield_local!=NULL and size_extrafield_local>0, extrafield_local
    contains the extrafield data the the local header
  if extrafield_global!=NULL and size_extrafield_global>0, extrafield_global
    contains the extrafield data the the local header
  if comment != NULL, comment contain the comment string
  method contain the compression method (0 for store, Z_DEFLATED for deflate)
  level contain the level of compression (can be Z_DEFAULT_COMPRESSION)
*/

	void OpenNewFileInZip (	CString filename, 
							zip_fileinfo& zipfi, 
							int level = Z_DEFAULT_COMPRESSION,
							CString comment = "", 
							const void* extrafield_local = NULL, 
							uInt size_extrafield_local = 0, 
							const void* extrafield_global = NULL, 
							uInt size_extrafield_global = 0, 
							int method = Z_DEFLATED);
/*
  Write data in the zipfile
*/

	void WriteInFileInZip(const void *buf, UINT len);
/*
  Close the current file in the zipfile
*/

	void CloseFileInZip();
/*
  Close the zipfile and the current file in the zipfile if is opened
*/
	void Close(CString global_comment = "");
/*
	Fill in zip_fileinfo fields (time an attributes) according to CFile data
*/
	void UpdateZipInfo(zip_fileinfo &zi, CFile &f);
	virtual ~CZipFile();

protected:
	void ziplocal_putValue_inmemory(Byte dest, uLong x, int nbByte);
	void ziplocal_putValue(uLong x, int nbByte);
	zip_internal zi;
};

#endif // !defined(AFX_ZIPFILE_H__7F795F27_D6BD_11D3_B7C7_BBF03FA53147__INCLUDED_)
