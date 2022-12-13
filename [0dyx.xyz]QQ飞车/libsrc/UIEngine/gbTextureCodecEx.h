
#ifndef GBTEXTURECODECEX_H
#define GBTEXTURECODECEX_H

#include "NiPixelData.h"
#include "nitexturecodec.h"

class gbTextureCodecEx : public NiTextureCodec
{
public:
	gbTextureCodecEx(void);
	~gbTextureCodecEx(void);

	unsigned int ExpandCompressedPixel(const NiPixelData& kSrc,unsigned int uiX,unsigned int uiY,
		unsigned int uiMipmapLevel = 0,unsigned int uiFace = 0);
};
#endif