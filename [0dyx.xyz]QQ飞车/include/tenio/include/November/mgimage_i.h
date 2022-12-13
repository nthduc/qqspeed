#ifndef __MGIMAGE_I_H__
#define __MGIMAGE_I_H__

#include "../teniointerface.h"
#include "../june/image_i.h"
#include "novdefine.h"

namespace Tenio
{
	class IMGImage;

	TENIO_MAKE_TYPEID_MAP(IMGImage, COMPONENTID_MGIMAGE);

	class IMGImage : 
		public IImage,
		public IComponent
	{
	public:
		virtual BOOL CreateImageFromFile(
			LPCTSTR lpszFileName, 
			PSELECTPARAM pSelectParam, 
			ENMCREATEFLAG eCreateFlag) = 0;

		virtual BOOL CreateEmptyImage(
			int nWidth, 
			int nHeight, 
			int nFrameNum = 1, 
			BOOL bHaveAlphaChannels = TRUE, 
			UINT unCreateFlag = 0) = 0;

		virtual BOOL Draw(int x, int y, UINT nFrame = 0, PDRAW_PARA pDP = NULL) = 0;
		virtual BOOL Draw(
			HDC hdc, 
			int nDstX, 
			int nDstY, 
			int nSrcX, 
			int nSrcY, 
			int nWidth, 
			int nHeight, 
			UINT nFrame = 0) = 0;
	};
};

#endif

// InterfaceName:MGImage
// SubFolderName:November
