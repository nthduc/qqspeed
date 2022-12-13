#pragma once

#include "componentid.h"

namespace Tenio
{
	#define TENIO_MAKE_TYPE2ID_MAP(interface, objectid)		      \
	template<>                                                    \
		class Type2ID<interface>								  \
		{                                                         \
		public:                                                   \
			inline static const DWORD ComponentID()               \
			{                                                     \
				static DWORD dwObjectID = objectid;               \
				return dwObjectID;                                \
			}                                                     \
			inline static const LPCTSTR InterfaceName()           \
			{                                                     \
				static LPCTSTR lpszInterfaceName = TEXT(#interface);\
				return lpszInterfaceName;                         \
			}                                                     \
		};                                                        \
		
		template<class InterfaceType>
		class Type2ID;
};
