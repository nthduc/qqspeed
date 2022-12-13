#pragma once
#include <string>
#include <d3d9.h>
namespace FBGUI
{
	// ************************************************************ //
	// CGUITexture
	// @remarks
	//    ���ඨ���������ʽ�ĳ�����
	// @par
	// ************************************************************ //

	class CGUITexture
	{
	public:
		CGUITexture( )			{}
	public:
		virtual ~CGUITexture( )	{}

	public:
		int GetTexWidth( ){return mTexWidth;}	
		int GetTexHeight( ){return mTexHeight;}	
		const std::string& GetTexName( ){return mTexName;}
	public:
		LPDIRECT3DTEXTURE9  m_TextCasheTexture;
		int					mTexWidth;
		int					mTexHeight;
		const std::string   mTexName;
	};

}