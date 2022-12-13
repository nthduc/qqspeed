// VFS.cpp : Defines the entry point for the DLL application.
//

#include "VFS.h"

#include <io.h>

#if defined(VFS_PORTABLE)

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

#endif

NAMESPACEBEGIN

static TCHAR space_ch[] = { _T((char)' '), _T((char)'\x09'), _T((char)'\x0A'), _T((char)'\x0D') };

extern int FixDirName(TCHAR* fixeddir)
{
	if (fixeddir==NULL)
	{
		return 0;
	}

	TrimSpace(fixeddir);
	for (int i=0; ; i++)
	{
		TCHAR& ch = fixeddir[i]; 
		if (ch==_T('\0')) break;
		if (ch==_T('/')) ch=_T('\\');
	}

	if (fixeddir[i-1]!=_T('\\')) { fixeddir[i++]=_T('\\'); fixeddir[i]=_T('\0'); }
	return i;
}

extern int FixPathName(TCHAR* fixedpath)
{
	if (fixedpath==NULL) 
	{
		return 0;
	}

	TrimSpace(fixedpath);
	for (int i=0; ; i++)
	{
		TCHAR& ch = fixedpath[i]; 
		if (ch==_T('\0')) break;
		if (ch==_T('/')) ch=_T('\\');
	}
	return i;
}

int FixDirName(const TCHAR* srcdir, TCHAR* fixeddir)
{
	if (srcdir==NULL || fixeddir==NULL)
	{
		return 0;
	}

	int pos, j;
	for ( pos=0; ; pos++ )
	{
		TCHAR ch = srcdir[pos];
		if (ch==_T('\0')) break;

        for (j=sizeof(space_ch)/sizeof(TCHAR)-1; j>=0; j--)
            if (ch==space_ch[j]) break;

		if (j<0)
		{	//no space
			if (ch!=_T('/')) fixeddir[pos] = ch;
			else fixeddir[pos] = _T('\\');
		}
	}

	if (pos<MAX_PATH-1)
	{
		if (pos>0 && fixeddir[pos-1]!=_T('\\'))
			fixeddir[pos++] = _T('\\');
		fixeddir[pos] = _T('\0');
	}
	else
	{
		fixeddir[MAX_PATH-1] = _T('\0');
		pos = -1;
	}

	return pos;
}

int FixPathName(const TCHAR* srcpath, TCHAR* fixedpath)
{
	if (srcpath==NULL || fixedpath==NULL)
	{
		return 0;
	}

	int pos, j;
	for ( pos=0; ; pos++ )
	{
		TCHAR ch = srcpath[pos];
		if (ch==_T('\0')) break;

        for (j=sizeof(space_ch)/sizeof(TCHAR)-1; j>=0; j--)
            if (ch==space_ch[j]) break;

		if (j<0)
		{	//no space
			if (ch!=_T('/')) fixedpath[pos] = ch;
			else fixedpath[pos] = _T('\\');
		}
	}

	if (pos<MAX_PATH)
	{
		fixedpath[pos] = _T('\0');
	}
	else
	{
		fixedpath[MAX_PATH-1] = _T('\0');
		pos = -1;
	}

	return pos;
}

int GetPathExtName(const TCHAR* pathname, TCHAR* extname)
{
	if (pathname==NULL || extname==NULL)
	{
		return 0;
	}

	int ret = 0;
	TCHAR* p = _tcsrchr(pathname,_T('.'));
	if (p && ( *++p!=_T('\\') && *p!=_T('/')) )
	{
		_tcscpy(extname,p);
		ret = 1;
	}
	else
	{
		extname[0] = _T('\0');
	}
	return ret;
}

int GetPathType(const TCHAR* path)
{
	if (path==NULL)
	{
		return VFS_PATH_NOTEXIST;
	}

	TCHAR fixedpath[MAX_PATH];
	int len = _tcslen(path);
	
	if (len==0) return VFS_PATH_NOTEXIST;

	if (path[len-1]==_T('\\') || path[len-1]==_T('/'))
	{
		_tcscpy(fixedpath,path);
		fixedpath[len-1]=_T('\0');
		path=fixedpath;
	}

	WIN32_FIND_DATA finddata;
	HANDLE hFind = FindFirstFile(path,&finddata);
	if (hFind==INVALID_HANDLE_VALUE)
		return VFS_PATH_NOTEXIST;
	FindClose(hFind);
	if (finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) return VFS_PATH_DIRECTORY;
	return VFS_PATH_FILE;
}

bool MkDirEx( const TCHAR* _path )
{
	if (_path==NULL)
	{
		return false;
	}

	TCHAR path[MAX_PATH+1];
	assert(_tcslen(_path)<MAX_PATH);

	_tcscpy(path,_path);

	TCHAR * pstr;

	for (pstr=path;;)
	{
		pstr = _tcschr(pstr,_T('\\'));
		if (pstr)
		{
			*pstr = 0;
			if (_taccess(path,0)==-1)
			{
				_tmkdir(path);
			}
			*pstr++ = _T('\\');
		}
		else
		{
			break;
		}
	}

    return true;
}

/*
int GetRelativeTrgFileName(int type, const TCHAR* relativesrcpath, const TCHAR* trgpath, TCHAR* newtrgpath)
{
	if (relativesrcpath==NULL || trgpath==NULL || newtrgpath==NULL)
	{
		return 0;
	}

	int trglen = _tcslen(trgpath);
	if (type==VFS_PATH_NOTEXIST || type==VFS_PATH_DIRECTORY)
	{
		TCHAR lastchar = (trglen>0)?trgpath[trglen-1]:0;

		_tcscpy(newtrgpath,trgpath);

		if (type==VFS_PATH_DIRECTORY)
		{
			if (trglen>0 && !(lastchar==_T('\\') || lastchar==_T('/')))
			{
				newtrgpath[trglen++]=_T('\\');
				newtrgpath[trglen++]=_T('\0');
				lastchar=_T('\\');
			}
		}
		
		if (trglen==0 || lastchar==_T('\\') || lastchar==_T('/'))
		{
			_tcscat(newtrgpath,relativesrcpath);
		}
	}
	else
	{
		_tcscpy(newtrgpath,trgpath);
	}
	TrimSpace(newtrgpath);
	return 1;
}
*/

int GetTrgFileName(int type, const TCHAR* srcpath, const TCHAR* trgpath, TCHAR* newtrgpath)
{
	if (srcpath==NULL || trgpath==NULL || newtrgpath==NULL)
	{
		return 0;
	}

	int trglen = _tcslen(trgpath);
	if (type==VFS_PATH_NOTEXIST || type==VFS_PATH_DIRECTORY)
	{
		TCHAR lastchar = (trglen>0)?trgpath[trglen-1]:0;

		_tcscpy(newtrgpath,trgpath);

		if (type==VFS_PATH_DIRECTORY)
		{
			if (trglen>0 && !(lastchar==_T('\\') || lastchar==_T('/')))
			{
				newtrgpath[trglen++]=_T('\\');
				newtrgpath[trglen++]=_T('\0');
				lastchar=_T('\\');
			}
		}
		
		if (trglen==0 || lastchar==_T('\\') || lastchar==_T('/'))
		{
			TCHAR* p = _tcsrchr(srcpath,_T('\\'));
			if (p!=NULL && *++p!=_T('\0'))
			{
				_tcscpy(newtrgpath,p);
			}
			else
			{
				_tcscat(newtrgpath,srcpath);
			}
		}
	}
	else
	{
		_tcscpy(newtrgpath,trgpath);
	}
	TrimSpace(newtrgpath);
	return 1;
}

int TrimSpace(TCHAR* str)
{
	if (str==NULL)
	{
		return 0;
	}

    int len = _tcslen(str);

    int i, j, s, e;

    for (i=0; i<len; i++)
    {
        for (j=sizeof(space_ch)/sizeof(TCHAR)-1; j>=0; j--)
            if (str[i]==space_ch[j]) break;

        if (j<0) //no space
            break;
    }
    s=i;

    for (i=len-1; i>s; i--)
    {
        for (j=sizeof(space_ch)-1; j>=0; j--)
            if (str[i]==space_ch[j]) break;

        if (j<0) //no space
            break;
    }
    e=i;

    e=e+1-s;

    if (s>0)
    {
        memmove ( str, &str[s], e*sizeof(TCHAR) );
    }

    str[e] = _T('\0');

    return len-e;
}

void StringToLower(TCHAR* str)
{
	int pos = 0;
	for (;;)
	{
		TCHAR& ch = str[pos++];
		
		if (ch==_T('\0')) break;

		if (ch>=_T('A') && ch<=_T('Z'))
		{
			ch = ch - _T('A') + _T('a');
		}
	}
}

NAMESPACEEND