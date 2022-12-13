// VFileSysImp.h : Declaration of the CVFileSysImp

#ifndef __VFILESYSIMP_H_
#define __VFILESYSIMP_H_

#include "resource.h"       // main symbols
#include "vfilesys.h"
#include "framework/vfs_i.h"
#include "framework/GameApp_i.h"

class CVFile;
/////////////////////////////////////////////////////////////////////////////
// CVFileSysImp
class ATL_NO_VTABLE CVFileSysImp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVFileSysImp, &CLSID_VFileSysImp>,
	public IFileSystem_V,
	public IGameService
{
public:
	CVFileSysImp()
	{
	}

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

	DECLARE_REGISTRY_RESOURCEID(IDR_VFILESYSIMP)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CVFileSysImp)
		COM_INTERFACE_ENTRY(IFileSystem_V)
		COM_INTERFACE_ENTRY(IGameService)
	END_COM_MAP()

protected:
	FILE_V* FileToHandle(CVFile*);
	CVFile* HandleToFile(FILE_V*);

public:
};

#endif //__VFILESYSIMP_H_
