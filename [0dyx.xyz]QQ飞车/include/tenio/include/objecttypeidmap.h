#ifndef __TENIO_OBJECT_TYPE_ID_MAP_H__
#define __TENIO_OBJECT_TYPE_ID_MAP_H__

#include "componentid.h"

namespace Tenio
{
	#define TENIO_MAKE_TYPEID_MAP(interface, objectid)				  \
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
	template<>                                                    \
		class ID2Type<objectid>                                   \
		{                                                         \
		public:                                                   \
			typedef interface Type;                               \
		};                                                        \

	
	template<class InterfaceType>
		class Type2ID;

	template<DWORD dwObjectID>
		class ID2Type;

	TENIO_MAKE_TYPEID_MAP(void, 0xffffffff)
};

#endif