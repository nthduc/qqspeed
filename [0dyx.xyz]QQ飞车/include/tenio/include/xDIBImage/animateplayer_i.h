#ifndef __ANIMATEPLAYER_I_H__
#define __ANIMATEPLAYER_I_H__

#include "../teniointerface.h"
#include "../Layer/imagebox_i.h"
#include "../June/image_i.h"


namespace Tenio
{
	class IAnimatePlayer : public IComponent
	{
	public:
		virtual int GetFormatId() = 0;
		virtual void Initialize(IJuneImageBox* pImageBox, IImage* pImage) = 0;
		virtual void StartAnimate() = 0;
		virtual void EndAnimate() = 0;
	};
};

#endif

// InterfaceName:AnimatePlayer
// SubFolderName:xDIBImage
