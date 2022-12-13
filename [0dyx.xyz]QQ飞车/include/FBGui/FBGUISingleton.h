#pragma once
#include "FBGUIBase.h"
#include <cassert>

namespace FBGUI
{

	template <typename T> class CSingleton
	{
	protected:
		static T* spSingleton;

	public:
		CSingleton( )
			{ assert( spSingleton == NULL ); spSingleton = static_cast<T*>( this ); }

	public:
	~CSingleton( )
			{ assert( spSingleton != NULL );  spSingleton = NULL; }

	public:
		static T& GetSingleton( )
			{ assert( spSingleton != NULL );  return ( *spSingleton ); }

		static T* GetSingletonPtr( )
			{ return ( spSingleton ); }
	};

};

