////////////////////////////////////////////////
//     FileName:    VFileSysImp2.h
//     Author:      Fish
//     Date:        2006-12-13 10:32:43
//     Description: []
////////////////////////////////////////////////

#pragma once

#include "resource.h"       // main symbols
#include "vfilesys.h"
#include "framework/GameApp_i.h"
#include "framework/vfs_i.h"
#include "vector"
#include "VFSHelper.h"

class ATL_NO_VTABLE CVFileSysImp2:
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVFileSysImp2, &CLSID_VFileSysImp>,
	public IFileSystem_V,
	public IGameService

{
public:
	CVFileSysImp2(void);
	~CVFileSysImp2(void);

	//IFileSystem_V
	virtual FILE_V*	vfopen	( const char* filename, const char* mode )	;
	virtual int		vfclose	( FILE_V* stream )							;
	virtual size_t	vfread	( void* buffer, size_t size, size_t count, FILE_V* stream )		;
	virtual size_t	vfwrite	( const void *buffer, size_t size, size_t count, FILE_V *stream )	;
	virtual int		vfseek	( FILE_V *stream, long offset, int origin )	;
	virtual long	vftell	( FILE_V *stream )			;
	virtual int		vfeof	( FILE_V *stream )			;
	virtual bool	vfexists	(const char* filename)		;
	virtual size_t	vfsize	(FILE_V* stream)			;

	//IGameService
	STDMETHOD(AppInit)(IGameApp*);
	STDMETHOD(AppUnInit)();

	DECLARE_REGISTRY_RESOURCEID(IDR_VFILESYSIMP2)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CVFileSysImp2)
		COM_INTERFACE_ENTRY(IFileSystem_V)
		COM_INTERFACE_ENTRY(IGameService)
	END_COM_MAP()

private:
	CVFSHelper	m_VFSHelper;
	std::vector<VROOT_h> m_vecRoot;
};
