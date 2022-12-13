#ifndef __VERSIONREADER_I_H__
#define __VERSIONREADER_I_H__

#include "teniointerface.h"

#include "versionControler/versioncontroler_i.h"
#include "resourcestoragedef.h"

namespace Tenio
{
	class IVersionReader;

	TENIO_MAKE_TYPEID_MAP(IVersionReader, COMPONENTID_VERSIONREADER);

	class IVersionReader : public IComponent
	{
	public:
		virtual BOOL OpenVersionFileName(LPCTSTR lpszVersionFileName) = 0;
		virtual BOOL ReadFirstUpdateFileList(PRESOURCELIST pResourceList, int nFileCount) = 0; 
		virtual BOOL ReadNextUpdateFileList(PRESOURCELIST pResourceList, int nFileCount) = 0; 
		virtual BOOL IsFileEnd() = 0;
		virtual BOOL CloseVersionFileName() = 0;
	};
};

#endif

// InterfaceName:VersionReader
// SubFolderName:ResourceStorage
