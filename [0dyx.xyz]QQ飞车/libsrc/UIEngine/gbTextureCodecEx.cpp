#include ".\gbtexturecodecex.h"

gbTextureCodecEx::gbTextureCodecEx(void)
:NiTextureCodec()
{
}

gbTextureCodecEx::~gbTextureCodecEx(void)
{
}

unsigned int gbTextureCodecEx::ExpandCompressedPixel(const NiPixelData& kSrc,unsigned int uiX,unsigned int uiY,unsigned int uiMipmapLevel ,unsigned int uiFace )
{
	unsigned int uiBlockStride = 0;//good name?
	switch (kSrc.GetPixelFormat().GetFormat())
	{
	case NiPixelFormat::FORMAT_DXT1:
		uiBlockStride = 8;
		break;
	case NiPixelFormat::FORMAT_DXT3:
		uiBlockStride = 16;
		break;
	case NiPixelFormat::FORMAT_DXT5:
		uiBlockStride = 16;
		break;
	default:
		return 0;
	};

	unsigned int uiMipmapLevels = kSrc.GetNumMipmapLevels();
	unsigned int uiNumFaces = kSrc.GetNumFaces();

	m_pucSrcPtr = kSrc.GetPixels(uiMipmapLevel, uiFace);
	unsigned int uiWidth = kSrc.GetWidth(uiMipmapLevel, uiFace);
	unsigned int uiHeight = kSrc.GetHeight(uiMipmapLevel, uiFace);

	if(uiWidth <= uiX||uiHeight <= uiY)
	{
		assert(!"取得像素位置时 坐标越界！");
		return 0;
	}

	unsigned int uiWholeBlockCols = uiWidth >> 2;
	unsigned int uiWholeExtraCols = uiWidth & 0x3;
	if(uiWholeExtraCols)
		uiWholeBlockCols ++;

	unsigned int uiBlockCols = uiX >> 2;
	unsigned int uiBlockRows = uiY >> 2;
	unsigned int uiExtraCols = uiX & 0x3;
	unsigned int uiExtraRows = uiY & 0x3;
	m_pucSrcPtr += (uiBlockRows*uiWholeBlockCols + uiBlockCols)*uiBlockStride;

	//NiPixelFormat::RGBA32
	unsigned char aucColorBlock[4 * 16];
	unsigned char* pucColor = aucColorBlock;
	// pucColor + 3 is the pointer to the alpha byte of the first pixel

	switch (kSrc.GetPixelFormat().GetFormat())
	{
	case NiPixelFormat::FORMAT_DXT1:
		NiTextureCodec::DecodeColorAlphaBlock(pucColor);
		break;
	case NiPixelFormat::FORMAT_DXT3:
		NiTextureCodec::DecodeT3AlphaBlock(pucColor+3);
		NiTextureCodec::DecodeColorBlock(pucColor);
		break;
	case NiPixelFormat::FORMAT_DXT5:
		NiTextureCodec::DecodeT5AlphaBlock(pucColor+3);
		NiTextureCodec::DecodeColorBlock(pucColor);
		break;
	default:
		return 0;
	};

	 pucColor = aucColorBlock;
	pucColor += (uiExtraRows*4 + uiExtraCols)*4;
	unsigned int* pReturn = (unsigned int*)(pucColor);
	return (*pReturn);
}