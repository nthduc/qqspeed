
#include "charstr.h"
#include <assert.h>
#include <string.h>

//GBK内码的编码范围为：编码范围为：0x8140～0xFE7E，0x8180～0xFEFE

//0-asc,  1-dbchar head,  2-dbchar tail
static int GetDBCSCharState( const char *buffer, int pos )
{
    assert( pos < (int)strlen(buffer) );
    int state = 0; //0-asc,  1-dbchar head,  2-dbchar tail
    for( int i=0; i<=pos; i++ )
    {
        unsigned char c = (unsigned char)buffer[i];
        switch( state )
        {
        case 0:
            if( c>=0x81 && c<=0xfe ) state = 1;
            break;
        case 1:
            state = 2;
            break;
        case 2:
            if( c >= 128 ) state = 1;
            else state = 0;
            break;
        }
    }
    return state;
}

//是否是数字
bool IsNumberChar( char c )
{
	return( (c>='0')&&(c<='9') );
}

//是否是大写英文字母
bool IsUpcaseChar( char c  )
{
	return( (c>='A')&&(c<='Z') );
}

//是否是小写英文字母
bool IsLowcaseChar( char c  )
{
	return( (c>='a')&&(c<='z') );
}

//是否是英文符号
bool IsSymbolChar( char c  )
{
	//英文字符
	if((c>=' ')&&(c<='/')) return true;
	if((c>=':')&&(c<='@')) return true;
	if((c>='[')&&(c<='`')) return true;
	if((c>='{')&&(c<='~')) return true;

	return false;	
	
}

//是否可打印英文字符
bool IsPrintableChar(char c )
{
	return ((c>=' ')&&(c<='~'));
}

//是否可打印字符(英文或者汉字)
bool IsPrintableWord(char c )
{
	return (!((c>=0)&&(c<' ')||(c==127 )));
}

/**
 * 判断buffer[pos] 是否是多子节字符的一部分
 **/
bool IsDBCSChar( const char *buffer, int pos )
{
    if( GetDBCSCharState(buffer, pos) == 0 ) return false;
    else return true;
}

/**
 * 判断buffer[pos] 是否是多子节字符的头字节
 **/
bool IsDBCSCharHead( const char *buffer, int pos )
{
    if( GetDBCSCharState(buffer, pos) == 1 ) return true;
    else return false;
}

/**
 * 判断buffer[pos] 是否是多子节字符的尾字节
 **/
bool IsDBCSCharTail( const char *buffer, int pos )
{
    if( GetDBCSCharState(buffer, pos) == 2 ) return true;
    else return false;
}

bool IsGBKName( const char *pname )
{
	const char *pcur = pname;
    int state = 0;

	while( *pcur )
	{
		unsigned int c = (unsigned char)(*pcur++);
		switch(state)
		{
		case 0:
			if( c>=0x81 && c<=0xfe ) state = 1;
			else if( (c>='0' && c<='9') || (c>='A'&&c<='z') || c=='_' ) state = 0;
			else return false;
			break;

		case 1:
			if( (c>=0x40&&c<=0x7e) || (c>=0x80&&c<=0xfe) ) state = 0;
			else return false;
			break;
		}
	}
	return true;
}

bool IsLuaEscChar( char c )
{
	char escchars[] = { '\'', '\\', '\"', '\a', '\b', '\f', '\n', '\r', '\t', '\v' };
	for( int i=0; i<sizeof(escchars); i++ )
	{
		if( c == escchars[i] ) return true;
	}
	return false;
}

void CheckStringForLua( char *buffer, int maxbuf, const char *psrc )
{
	int count = 0;
	if(psrc)
	{
		const char *pcur = psrc;
		while( *pcur )
		{
			if( IsLuaEscChar(*pcur) ) buffer[count++] = '\\';
			buffer[count++] = *pcur++;
			if( count >= maxbuf-4 ) break;
		}
		assert( count < maxbuf );
	}
	buffer[count] = 0;
}


#define POLYNOMIAL 0x04c11db7L
static unsigned long crc_table[256];

class CrcTableInit
{
public:
	CrcTableInit();
};

static CrcTableInit s_CrcTableInit;

/* generate the table of CRC remainders for all possible bytes */
CrcTableInit::CrcTableInit()
{
	register int i, j;  
    register unsigned long crc_accum;
    for ( i = 0;  i < 256;  i++ )
    { 
        crc_accum = ( (unsigned long) i << 24 );
        for ( j = 0;  j < 8;  j++ )
        {
            if( crc_accum & 0x80000000L ) crc_accum = ( crc_accum << 1 ) ^ POLYNOMIAL;
            else crc_accum = ( crc_accum << 1 ); 
        }
        crc_table[i] = crc_accum; 
    }
}

unsigned int Crc32Compute( const void *pdata, int datasize )
{
    register int i;
    unsigned int result;

    if( datasize == 0 ) return 0;

	const unsigned char *data = (const unsigned char *)pdata; //对超过127得字符， 必须用unsigned char, 否则会出错
    result  = *data++ << 24;
    if( datasize > 1 )
    {
        result |= *data++ << 16;
        if( datasize > 2 )
        {
            result |= *data++ << 8;
            if( datasize > 3 ) result |= *data++;
        }
    }
    result = ~ result;
    datasize -= 4;
    
    for (i=0; i<datasize; i++)
    {
        result = (result << 8 | *data++) ^ crc_table[result >> 24];
    }
    
    return ~result;
}
