#ifndef __IPGROUPDOWNLOADER_I_H__
#define __IPGROUPDOWNLOADER_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IIPGroupDownloader;

	TENIO_MAKE_TYPEID_MAP(IIPGroupDownloader, COMPONENTID_IPGROUPDOWNLOADER);

	class IIPGroupDownloader : public IComponent
	{
	public:
	};
};

#endif

// InterfaceName:IPGroupDownloader
// SubFolderName:DownloadEngine
