
vfilesysps.dll: dlldata.obj vfilesys_p.obj vfilesys_i.obj
	link /dll /out:vfilesysps.dll /def:vfilesysps.def /entry:DllMain dlldata.obj vfilesys_p.obj vfilesys_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del vfilesysps.dll
	@del vfilesysps.lib
	@del vfilesysps.exp
	@del dlldata.obj
	@del vfilesys_p.obj
	@del vfilesys_i.obj
