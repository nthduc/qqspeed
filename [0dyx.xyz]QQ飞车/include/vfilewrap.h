////////////////////////////////////////////////
//     FileName:    vfilewrap.h
//     Author:      Fish
//     Date:        2006-9-25 10:48:20
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "framework/vfs_i.h"

#define DEFINE_FILESYSTEM_V IFileSystem_V* g_FileSystem_V;
extern DEFINE_FILESYSTEM_V;

inline void	vfinit( IFileSystem_V* pFileSystem )
{
	g_FileSystem_V = pFileSystem;
}

inline FILE_V*	vfopen( const char* filename, const char* mode )
{
	return g_FileSystem_V->vfopen(filename, mode);
}

inline int vfclose( FILE_V* stream )
{
	return g_FileSystem_V->vfclose(stream);
}

inline size_t vfread( void* buffer, size_t size, size_t count, FILE_V* stream )
{
	return g_FileSystem_V->vfread(buffer, size, count, stream);
}

inline size_t vfwrite( const void *buffer, size_t size, size_t count, FILE_V *stream )
{
	return g_FileSystem_V->vfwrite(buffer, size, count, stream);
}

inline int vfseek( FILE_V *stream, long offset, int origin )
{
	return g_FileSystem_V->vfseek(stream, offset, origin);
}

inline long vftell( FILE_V *stream )
{
	return g_FileSystem_V->vftell(stream);
}

inline int vfeof( FILE_V *stream )
{
	return g_FileSystem_V->vfeof(stream);
}

inline bool	vfexists(const char* filename)
{
	return	g_FileSystem_V->vfexists(filename);
}

inline size_t vfsize(FILE_V* stream)
{
	return g_FileSystem_V->vfsize(stream);
}