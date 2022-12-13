#ifndef __STREX_H__
#define __STREX_H__

inline char lcase(char c)
{
	if ( c >= 'A' && c <= 'Z' )
		return c - 'A' + 'a';
	return c;
}

inline char ucase(char c )
{
	if ( c >= 'a' && c <= 'z' )
		return c - 'a' + 'A';
	return c;
}

inline int strimatch(const char* src, const char* dst)
{
	int tmplen = min(strlen(src), strlen(dst));
	int i = 0;
	for ( ; i < tmplen; i++ )
	{
		if ( lcase(src[i]) != lcase(dst[i]) )
			break;
	}
	return i;
}

inline int strmatch(const char* src, const char* dst)
{
	int tmplen = min(strlen(src), strlen(dst));
	int i = 0;
	for ( ; i < tmplen; i++ )
	{
		if ( src[i] != dst[i] )
			break;
	}
	return i;
}

inline int striter(const char* src, char* dst, const char* filter="/\\")
{
	int i = 0;
	int len = strlen(src);
	while(i < len && strchr(filter, src[i]) )
		i++;
	if ( i == len )
		return i;
	int pos = strcspn(&src[i], filter);
	strncpy(dst, &src[i], pos);
	return pos+i;
}

inline int strsplit(const char* src, char* dst, const char* filter="/\\")
{
	int len = strlen(src) + 1;
	char* szTmp = new char[len];
	int i = 0;
	int c = 0;
	while(1)
	{
		memset(szTmp, 0, len);
		int pos = striter(&src[i], szTmp, filter);
		int tmplen = strlen(szTmp);
		if ( tmplen > 0 )
		{
			strcpy(&dst[i], szTmp);
			i += tmplen;
			i++;
			c++;
		}
		else
		{
			break;
		}
		if ( pos >= len - 1 )
			break;
	}
	dst[i] = 0;
	delete[] szTmp;
	return c;
}

#endif