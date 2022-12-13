#ifndef __TENIODLLSTRUCT_H__
#define __TENIODLLSTRUCT_H__

#define TENIO_DLLMAXCOMPONENT	512

#include <windows.h>
#include "component_i.h"

#define DEFINE_COMPONENT_FACTORY(ClassName) static ClassName::_Creator _ObjectCreator;

typedef Tenio::IComponent* (*PCOMPONENTFACTORYMETHOD) ();

typedef struct _tagCOMPONENTCREATOR
{
	DWORD dwComponentID;
	PCOMPONENTFACTORYMETHOD pFactoryMethod;
}COMPONENTCREATOR, *PCOMPONENTCREATOR;

typedef struct _tagTENIODLLCREATORTABLE
{
	DWORD dwCount;
	COMPONENTCREATOR arFactoryMethods[TENIO_DLLMAXCOMPONENT];
}TENIODLLCREATORTABLE, *PTENIODLLCREATORTABLE;

typedef struct _tagCOMPONENTINFO
{
	DWORD dwComponentID;
	DWORD dwCreatorID;
	Tenio::IComponent* pComponent;
}COMPONENTINFO, *PCOMPONENTINFO;

#endif