#ifndef __GIF_I_H__
#define __GIF_I_H__

#include "../teniointerface.h"
#include "imagedefine.h"

//////////////////////////////////////////////////////////////////////////
namespace Tenio
{
	class IGIF;
	
	TENIO_MAKE_TYPEID_MAP(IGIF, COMPONENTID_GIF)
		
	class IGIF : public IComponent
	{
	public:
		virtual BOOL LoadGifFile(LPCTSTR lpszFileName) = 0;
		virtual void UnLoad() = 0;
		virtual SIZE GetImageSize() = 0;
		virtual int  GetFrameCount() = 0;
		virtual LPGIFFRAME GetFrameInfo(int nFrameIndex) = 0;
	};
}

#endif //__GIF_I_H__