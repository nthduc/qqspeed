#pragma once

namespace Tenio
{
    // Local version file name
#define LOCAL_VERSION_FILE_DIR						_T("config\\LocalVersionFile.xml")

    // Update version file name
#define UPDATE_VERSION_FILE_DIR						_T("config\\UpdateVersionFile.xml")

#define VERSION_FILE_TITLE					        _T("xml version=\"1.0\"")
#define FILE_LIST					     	        _T("filelist")
#define FILE_LIST_FILE					     	    _T("file")
#define FILE_NAME									_T("name") 
#define FILE_VERSION					     	    _T("version")
#define FILE_PREFIX					     	        _T("file_")
#define TYPEANDFILENAME_SEPARATOR					_T('@')
#define MAX_VERSION_LENGTH                        20
#define INVALID_VERSION                             -1
#define MAX_FILENAME_LENGTH                         (MAX_PATH + 10)	//////////////////////////////////////////////////////////////////////////
	// Data struct
// 	typedef struct tagResourceList
// 	{
// 		int   nFileCount;
// 		TCHAR szFileNameList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
// 	}
// 	RESOURCELIST, * PRESOURCELIST;
}