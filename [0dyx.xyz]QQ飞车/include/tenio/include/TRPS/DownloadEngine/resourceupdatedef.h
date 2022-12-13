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
		int							   nTaskId;				//����ID
		IZipUtilityThread*             pZipUtilityThread;          // ��ѹ�߳�
		TCHAR                          szCompressFileName[MAX_PATH]; // ѹ���ļ���
		TCHAR                          szTemporaryPath[MAX_PATH];  // ��ʱ��ѹ·��
		TCHAR                          szDestPath[MAX_PATH];       // ��ѹĿ��·��
		FILELIST					   stCompressedResource;      // ѹ���ļ��е��ļ��б�
	}
	COMPRESSFILEINFO, * PCOMPRESSFILEINFO;
}