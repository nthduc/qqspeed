////////////////////////////////////////////////
//     FileName:    vfs_i.h
//     Author:      Fish
//     Date:        2006-10-10 16:08:43
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "wtypes.h"
#include "framework/comdef.h"

typedef void FILE_V;
DECL_IID(IFileSystem_V, "313A97EA-329E-4B88-8505-197A5422DCF2") 
struct DECLSPEC_NOVTABLE IFileSystem_V : IUnknown
{
	virtual FILE_V*	vfopen	( const char* filename, const char* mode )	PURE;
	virtual int		vfclose	( FILE_V* stream )							PURE;
	virtual size_t	vfread	( void* buffer, size_t size, size_t count, FILE_V* stream )			PURE;
	virtual size_t	vfwrite	( const void *buffer, size_t size, size_t count, FILE_V *stream )	PURE;
	virtual int		vfseek	( FILE_V *stream, long offset, int origin )	PURE;
	virtual long	vftell	( FILE_V *stream )			PURE;
	virtual int		vfeof	( FILE_V *stream )			PURE;
	virtual bool	vfexists	(const char* filename)		PURE;
	virtual size_t	vfsize	(FILE_V* stream)			PURE;
};

EXTERN_C const CLSID CLSID_VFileSysImp;
EXTERN_C const CLSID CLSID_VFileSysImp2;