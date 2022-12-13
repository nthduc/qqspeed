// resourecestoragedef.h

#pragma once 
#include "TRPS\Downloader\downloaderdef.h"
#include "ziputility\ziputilitythread_i.h"
#include "TRPS/ResourceStorage/resourcestorage_i.h"
namespace Tenio
{

#define  TEMP_DOWNLOADFILENAME_SUFFIX    _T(".tdl")
#define  TEMP_COMPRESSFILENAME_SUFFIX    _T(".Folder")

#define  UPDATESTATUS_EXECUTE			0x00000001
#define  UPDATESTATUS_CANCEL			0x00000002
#define  UPDATESTATUS_CLEAR			    0x00000004

	typedef struct tagCompressFileInfo
	{
		int							   nTaskId;				//任务ID
		IZipUtilityThread*             pZipUtilityThread;          // 解压线程
		TCHAR                          szCompressFileName[MAX_PATH]; // 压缩文件名
		TCHAR                          szTemporaryPath[MAX_PATH];  // 临时解压路径
		TCHAR                          szDestPath[MAX_PATH];       // 解压目的路径
		FILELIST					   stCompressedResource;      // 压缩文件中的文件列表
	}
	COMPRESSFILEINFO, * PCOMPRESSFILEINFO;
}