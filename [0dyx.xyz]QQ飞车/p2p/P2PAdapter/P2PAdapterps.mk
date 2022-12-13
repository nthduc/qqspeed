
P2PAdapterps.dll: dlldata.obj P2PAdapter_p.obj P2PAdapter_i.obj
	link /dll /out:P2PAdapterps.dll /def:P2PAdapterps.def /entry:DllMain dlldata.obj P2PAdapter_p.obj P2PAdapter_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del P2PAdapterps.dll
	@del P2PAdapterps.lib
	@del P2PAdapterps.exp
	@del dlldata.obj
	@del P2PAdapter_p.obj
	@del P2PAdapter_i.obj
