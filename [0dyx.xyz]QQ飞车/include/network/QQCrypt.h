/*
QQCrypt.h
	2002/09/25 pm23:28
	hyj@tencent.com




OIcqCrypt.h


OICQ加密
hyj@oicq.com
1999/12/24

  实现下列算法:
  Hash算法: MD5,已实现
  对称算法: DES,未实现
  非对称算法: RSA,未实现

*/

#ifndef _INCLUDED_QQCRYPT_H_
#define _INCLUDED_QQCRYPT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MD5_DIGEST_LENGTH	16
#define ENCRYPT_PADLEN		18
#define	CRYPT_KEY_SIZE		16

#ifdef _DEBUG
	BOOL Md5Test(); /*测试MD5函数是否按照rfc1321工作*/
#endif

/************************************************************************************************
	MD5数据结构
************************************************************************************************/

#define MD5_LBLOCK	16
typedef struct MD5state_st
	{
	unsigned long A,B,C,D;
	unsigned long Nl,Nh;
	unsigned long data[MD5_LBLOCK];
	int num;
	} MD5_CTX;

void MD5_Init(MD5_CTX *c);
void MD5_Update(MD5_CTX *c, const register unsigned char *data, unsigned long len);
void MD5_Final(unsigned char *md, MD5_CTX *c);


/************************************************************************************************
	Hash函数
************************************************************************************************/
/*
	输入const BYTE *inBuffer、int length
	输出BYTE *outBuffer
	其中length可为0,outBuffer的长度为MD5_DIGEST_LENGTH(16byte)
*/
void Md5HashBuffer( BYTE *outBuffer, const BYTE *inBuffer, int length);



/************************************************************************************************
	对称加密底层函数
************************************************************************************************/
//pOutBuffer、pInBuffer均为8byte, pKey为16byte
void TeaEncryptECB(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);
void TeaDecryptECB(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);
inline void TeaEncryptECB3(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);

#if 0
inline void TeaDecryptECB3(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);
#endif	// 0



/************************************************************************************************
	QQ对称加密第一代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);

/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
BOOL oi_symmetry_decrypt(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);







/************************************************************************************************
	QQ对称加密第二代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:nInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8byte的倍数);
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
int oi_symmetry_encrypt2_len(int nInBufLen);


/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);



/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
BOOL oi_symmetry_decrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);




/************************************************************************************************
	QQ对称加密第三代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:nInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8byte的倍数);
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
int qq_symmetry_encrypt3_len(int nInBufLen);


/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
//void qq_symmetry_encrypt3(const BYTE* pInBuf, int nInBufLen, DWORD dwUin, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);
//inline void qq_symmetry_encrypt3(const BYTE* pInBuf, int nInBufLen, DWORD dwUin, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);






#if 0
/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
inline BOOL qq_symmetry_decrypt3(const BYTE* pInBuf, int nInBufLen, BYTE chMainVer, BYTE chSubVer, DWORD dwUin, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);
#endif	// 0


#endif // #ifndef _INCLUDED_QQCRYPT_H_