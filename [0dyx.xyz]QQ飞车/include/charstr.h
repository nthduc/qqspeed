
#ifndef __CHARSTR_H__
#define __CHARSTR_H__

/**
 * �ж�buffer[pos] �Ƿ��Ƕ��ӽ��ַ���һ����
 **/
extern bool IsDBCSChar( const char *buffer, int pos );

/**
 * �ж�buffer[pos] �Ƿ��Ƕ��ӽ��ַ���ͷ�ֽ�
 **/
extern bool IsDBCSCharHead( const char *buffer, int pos );

/**
 * �ж�buffer[pos] �Ƿ��Ƕ��ӽ��ַ���β�ֽ�
 **/
extern bool IsDBCSCharTail( const char *buffer, int pos );


/**
 * �ж�buffer[pos] �Ƿ�������
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