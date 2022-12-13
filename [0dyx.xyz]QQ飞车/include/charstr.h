
#ifndef __CHARSTR_H__
#define __CHARSTR_H__

/**
 * 判断buffer[pos] 是否是多子节字符的一部分
 **/
extern bool IsDBCSChar( const char *buffer, int pos );

/**
 * 判断buffer[pos] 是否是多子节字符的头字节
 **/
extern bool IsDBCSCharHead( const char *buffer, int pos );

/**
 * 判断buffer[pos] 是否是多子节字符的尾字节
 **/
extern bool IsDBCSCharTail( const char *buffer, int pos );


/**
 * 判断buffer[pos] 是否是数字
 **/
extern bool IsNumberChar( char c  );
extern bool IsSymbolChar( char c  );
extern bool IsUpcaseChar( char c  );
extern bool IsLowcaseChar( char c  );
extern bool IsPrintableChar(char c );
extern bool IsPrintableWord(char c );


extern bool IsGBKName( const char *pname );

extern bool IsLuaEscChar( char c );
extern void CheckStringForLua( char *buffer, int maxbuf, const char *psrc );

extern unsigned int Crc32Compute( const void *pdata, int datasize );


#endif