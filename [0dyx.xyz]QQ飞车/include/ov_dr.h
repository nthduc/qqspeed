#ifndef OV_DR_H
#define	OV_DR_H

#include "ov_os.h"
#include "ov_err.h"

#define DR_MAGIC		0x4455
#define DR_BUILD		103
#define	DR_MAX_VERSION		0x7fffffff

#define DR_NAME_LEN		32
#define DR_DESC_LEN		32
#define DR_MACRO_LEN		32

#define DR_TYPE_MIN		0x00
#define DR_TYPE_UNION		DR_TYPE_MIN
#define DR_TYPE_STRUCT		0x01
#define	DR_TYPE_COMPOSITE	DR_TYPE_STRUCT
#define DR_TYPE_CHAR		0x02
#define DR_TYPE_UCHAR		0x03
#define DR_TYPE_SHORT		0x04
#define DR_TYPE_USHORT		0x05
#define DR_TYPE_INT		0x06
#define DR_TYPE_UINT		0x07
#define DR_TYPE_LONG		0x08
#define DR_TYPE_ULONG		0x09
#define DR_TYPE_LONGLONG	0x0a
#define DR_TYPE_ULONGLONG	0x0b
#define DR_TYPE_DATE		0x0c
#define DR_TYPE_TIME		0x0d
#define DR_TYPE_DATETIME	0x0e
#define DR_TYPE_MONEY		0x0f
#define DR_TYPE_FLOAT		0x10
#define DR_TYPE_DOUBLE		0x11
#define DR_TYPE_STRING		0x12
#define DR_TYPE_MAX		DR_TYPE_STRING

#define DR_NAME_UNION		"union"
#define DR_NAME_STRUCT		"struct"

#define DR_NAME_CHAR		"char"
#define DR_NAME_UCHAR		"uchar"
#define DR_NAME_BYTE		"byte"
#define DR_NAME_SHORT		"short"
#define DR_NAME_USHORT		"ushort"
#define DR_NAME_INT		"int"
#define DR_NAME_UINT		"uint"
#define DR_NAME_LONG		"long"
#define DR_NAME_ULONG		"ulong"
#define DR_NAME_LONGLONG	"longlong"
#define DR_NAME_ULONGLONG	"ulonglong"
#define DR_NAME_DATE		"date"
#define DR_NAME_TIME		"time"
#define DR_NAME_DATETIME	"datetime"
#define DR_NAME_MONEY		"money"
#define DR_NAME_FLOAT		"float"
#define DR_NAME_DOUBLE		"double"
#define DR_NAME_STRING		"string"

#define DR_TAG_TYPELIB		"typelib"
#define DR_TAG_TYPE		"type"
#define DR_TAG_MACROS		"macros"
#define DR_TAG_MACRO		"macro"
#define DR_TAG_ID		"id"
#define DR_TAG_NAME		"name"
#define DR_TAG_ITEM		"item"
#define DR_TAG_COUNT		"count"
#define DR_TAG_OFFSET		"offset"
#define DR_TAG_REFER		"refer"
#define DR_TAG_SELECT		"select"
#define DR_TAG_CLASS		"class"
#define DR_TAG_DESC		"desc"
#define DR_TAG_SIZETYPE		"sizetype"
#define DR_TAG_VERSION		"version"
#define DR_TAG_CONTENT		"content"
#define DR_TAG_TARGET		"target"
#define DR_TAG_PADDING		"padding"
#define DR_TAG_IO		"io"
#define DR_TAG_THIS		"this"
#define DR_TAG_DATASET		"dataset"

#define DR_TAG_STRUCT		DR_NAME_STRUCT
#define DR_TAG_UNION		DR_NAME_UNION

#define DR_IO_NOLIMIT		0x00
#define DR_IO_NOINPUT		0x01
#define DR_IO_NOOUTPUT		0x02
#define DR_IO_NOIO		0x03

#define DR_FLAG_NETORDER	0x01
#define DR_FLAG_COMPLEX		0x02

#define DR_FLAG_DIRECT		0x10	/* means use id as index */
#define DR_FLAG_UNION		0x20
#define DR_FLAG_FIXSIZE		0x40	/* a type that does not change size. */

#define DR_FLAG_RESOVLED	0x100
#define DR_FLAG_VARIABLE	0x200
#define DR_FLAG_VERSIONED	0x400

#define	DR_FLAG_VALID		0x1000

#define DR_STACK_SIZE		32
#define DR_MIN_BSEARCH		16
#define DR_MIN_COPY		64

/* error code. */

#define DRE_MOD			0x02	/* @mod	2 */

#define DRE_UNIONINUNION	0x01	/* @error 1 Union can not in union */
#define DRE_COUNT		0x02	/* @error 2 Count can not be negitive */
#define DRE_NETBUFF		0x03	/* @error 3 Packed buffer too small */
#define DRE_HOSTBUFF		0x04	/* @error 4 Unpacked buffer too small */

typedef int DRPTR;
typedef int DRIDX;

struct tagDRTypeInfo
{
	char* pszName;
	char* pszCName;		/* name used in c/c++ head file. */
	char* pszSPrefix;	/* single prefix. */
	char* pszMPrefix;	/* multiple prefix(for array). */
	
	int iType;
	int iSize;
	unsigned int dwFlags;
};

typedef struct tagDRTypeInfo	DRTYPEINFO;
typedef struct tagDRTypeInfo	*LPDRTYPEINFO;

struct tagDRMacro
{
	char szMacro[DR_MACRO_LEN];
	int iID;
};

typedef struct tagDRMacro		DRMACRO;
typedef struct tagDRMacro		*LPDRMACRO;

union tagDRType
{
	char cValue;
	unsigned char byValue;
	short nValue;
	unsigned short wValue;
	int iValue;
	unsigned int dwValue;
	long lValue;
	unsigned long ulValue;
	longlong llValue;
	ulonglong ullValue;
	float fValue;
	double dValue;
	char szValue[1];
};

typedef union tagDRType		DRTYPE;
typedef union tagDRType		*LPDRTYPE;


/* this struct used to describe one member of a struct. */
/* if 0==iCount means that this member is a variable array. */
/* only one variable array is allowed. */
struct tagDRMetaEntry
{
	int iID;
	DRPTR ptrMeta; /* ptr is the offset of the meta-data itself. */
	int iIO;		/* the input/output control. */
	int idxIO;		/* the idx of input/output control macro. */

	int iVersion;
	DRIDX idxVersion;

	int iSize;  /* byte size. */
	int iOff;   /* used for align, -1 for unspecified. */
	int iCount; /* 1 means single, >1 means array, 0 means variable array */
	int iUnit;  /* the size of one element. */

	DRIDX idxCount; /* the macro's referred by count. */ 
	DRIDX idxID;  	/* the id macro's index. */
	DRIDX idxType; /* the index of the typeinfo. */
	int iType;   /* the typeid of this entry. */

	int iTargetOff;	/* the target's offset in data to this. */
	int iTargetUnit;/* the unit size of the target. */
	
	int iRealOff;	/* the real target's offset. */
	int iRealUnit;	/* the real target's unit. */

	int iReferOff;	/* the refer's offset in data. */
	int iReferUnit;	/* the unit size of the refer. */
	int iSelectOff; /* the selector's offset in data. */
	int iSelectUnit;/* the unit size of the selector. */

	char szName[DR_NAME_LEN];	/* used for generating c/c++ header. */

/*	char szType[DR_NAME_LEN];	*/

/*	this two member need not to resolve defer, so it isn't needed. */
/*
	char szRefer[DR_NAME_LEN];

	char szSelect[DR_NAME_LEN];
*/
	char szDesc[DR_DESC_LEN];	/* the description info. */
};

typedef struct tagDRMetaEntry	DRMETAENTRY;
typedef struct tagDRMetaEntry	*LPDRMETAENTRY;
typedef struct tagDRMetaEntry	*HMETAENTRY;

struct tagDRMeta
{
	unsigned int uFlags;
	DRIDX iIdx;

	int iSizeType;	/* the size of the packed struct. */
	int idxSize;

	int iID;
	int idxID;

	int iData;
	DRPTR ptrMeta;	/* offset of this meta from "data" member of head. */

	int iMaxSubID;
	int iEntries;

	DRIDX idxType;
	int iType;
	
	DRIDX idxVersion;
	int iVersion;

	char szName[DR_NAME_LEN];

	DRMETAENTRY entries[1];
};

typedef struct tagDRMeta	DRMETA;
typedef struct tagDRMeta	*LPDRMETA;

struct tagDRMapEntry
{
	int iPtr;
	int iSize;
};

typedef struct tagDRMapEntry	DRMAPENTRY;
typedef struct tagDRMapEntry	*LPDRMAPENTRY;


struct tagDRIDEntry
{
	int iID;
	int iIdx;
};

typedef struct tagDRIDEntry	DRIDENTRY;
typedef struct tagDRIDEntry	*LPDRIDENTRY;


struct tagDRNameEntry
{
	char szName[DR_NAME_LEN];
	int iIdx;
};

typedef struct tagDRNameEntry	DRNAMEENTRY;
typedef struct tagDRNameEntry	*LPDRNAMEENTRY;

/*
	In order to refer the meta data fastly, We use two-level mapping. 
	First, A map entry for each meta data.
	Second, A index value for each map entry.
	There are two index array, one for id, one for name.
*/


struct tagDRMetaLib
{
	unsigned short wMagic;
	short nBuild;
	int iID;
	int iSize;
	int iMaxID;
	
	int checksum[4];	/* not really calculated. */

	int iVersion;
	int iPadding;
	
	int iMaxMetas;
	int iCurMetas;
	int iMaxMacros;
	int iCurMacros;
	
	DRPTR ptrCur;	/* all the offset is start from the 'data' member. */
	DRPTR ptrMacro;

	DRPTR ptrID;
	DRPTR ptrName;
	DRPTR ptrMap;
	DRPTR ptrMeta;
	
	char szName[DR_NAME_LEN];

	char data[1];		/* only used for reference data. */
};

typedef struct tagDRMetaLib	DRMETALIB;
typedef struct tagDRMetaLib	*LPDRMETALIB;


struct tagDRLibParam
{
	int iVersion;
	int iPadding;

	int iSize;		/* size for whole metalib struct. */
	int iID;
	int iMacros;
	int iMetas;
	
	char szName[DR_NAME_LEN];
};

typedef struct tagDRLibParam	DRLIBPARAM;
typedef struct tagDRLibParam	*LPDRLIBPARAM;

struct tagDRData
{
	char* pszBuff;
	int iBuff;		/* the size of the buffer. */
};

typedef struct tagDRData	DRDATA;
typedef struct tagDRData	*LPDRDATA;

struct tagDRStackItem
{
	LPDRMETA pstMeta;	/* the current meta data. */
	int iUnit;

	int iEntry;		/* entry index in meta. */
	int iCount;		/* array count. */

	char* pszHostBase;
	char* pszNetBase;
	char* pszTarget;
	int iOff;
};

typedef struct tagDRStackItem	DRSTACKITEM;
typedef struct tagDRStackItem	*LPDRSTACKITEM;

typedef DRSTACKITEM	DRSTACK[DR_STACK_SIZE];

/* some macro functions */

#define DR_IS_VALID(pstMeta)		((pstMeta)->uFlags & DR_FLAG_VALID )
#define DR_SET_VALID(pstMeta)		(pstMeta)->uFlags |= DR_FLAG_VALID
#define DR_CLR_VALID(pstMeta)		(pstMeta)->uFlags &= ~DR_FLAG_VALID

#define DR_IS_RESOLVED(pstMeta)		((pstMeta)->uFlags & DR_FLAG_RESOVLED )
#define DR_SET_RESOLVED(pstMeta)	(pstMeta)->uFlags |= DR_FLAG_RESOVLED
#define DR_CLR_RESOLVED(pstMeta)	(pstMeta)->uFlags &= ~DR_FLAG_RESOVLED

#define DR_IS_VARIABLE(pstMeta)		((pstMeta)->uFlags & DR_FLAG_VARIABLE )
#define DR_SET_VARIABLE(pstMeta)	(pstMeta)->uFlags |= DR_FLAG_VARIABLE
#define DR_CLR_VARIABLE(pstMeta)	(pstMeta)->uFlags &= ~DR_FLAG_VARIABLE

#define DR_IS_VERSIONED(pstMeta)	((pstMeta)->uFlags & DR_FLAG_VERSIONED )
#define DR_SET_VERSIONED(pstMeta)	(pstMeta)->uFlags |= DR_FLAG_VERSIONED
#define DR_CLR_VERSIONED(pstMeta)	(pstMeta)->uFlags &= ~DR_FLAG_VERSIONED

#define DR_IS_FIXSIZE(pstMeta)		((pstMeta)->uFlags & DR_FLAG_FIXSIZE )
#define DR_SET_FIXSIZE(pstMeta)		(pstMeta)->uFlags |= DR_FLAG_FIXSIZE
#define DR_CLR_FIXSIZE(pstMeta)		(pstMeta)->uFlags &= ~DR_FLAG_FIXSIZE

#define DR_CALC_MACRO_PTR(pstParam)	0

#define DR_CALC_ID_PTR(pstParam)	( DR_CALC_MACRO_PTR(pstParam) + (pstParam)->iMacros*sizeof(DRMACRO) )

#define DR_CALC_NAME_PTR(pstParam)	( DR_CALC_ID_PTR(pstParam) + sizeof(DRIDENTRY)*(pstParam->iMetas) )

#define DR_CALC_MAP_PTR(pstParam)	( DR_CALC_NAME_PTR(pstParam) + sizeof(DRNAMEENTRY)*(pstParam->iMetas) )

#define DR_CALC_META_PTR(pstParam)	( DR_CALC_MAP_PTR(pstParam) + sizeof(DRMAPENTRY)*(pstParam->iMetas) )

#define DR_CALC_META_SIZE(iEntries)	( sizeof(DRMETAENTRY)*(iEntries) + offsetof(DRMETA, entries) )

#define DR_CALC_MIN_SIZE(iMetas, iMacros)	( sizeof(DRMACRO)*(iMacros) + \
(sizeof(DRIDENTRY) + sizeof(DRNAMEENTRY) + sizeof(DRMAPENTRY))*(iMetas) + offsetof(DRMETALIB, data) )

#define DR_CALC_FREE_SPACE(pstLib)	( (pstLib)->iSize - (pstLib)->ptrCur - offsetof(DRMETALIB, data) )

#define DR_GET_MACRO_TABLE(pstLib)		(LPDRMACRO)(pstLib->data + pstLib->ptrMacro) 

#define DR_GET_ID_TABLE(pstLib)		(LPDRIDENTRY)(pstLib->data + pstLib->ptrID) 

#define DR_GET_NAME_TABLE(pstLib)	(LPDRNAMEENTRY)(pstLib->data + pstLib->ptrName)

#define DR_GET_MAP_TABLE(pstLib)	(LPDRMAPENTRY)(pstLib->data + pstLib->ptrMap) 

#define DR_IDX_TO_META(pstLib, idx)	(LPDRMETA) (pstLib->data + ((LPDRMAPENTRY)(pstLib->data + pstLib->ptrMap))[idx].iPtr)

#define DR_PTR_TO_META(pstLib, ptr)	(LPDRMETA) (pstLib->data + (ptr))

#define DR_META_TO_LIB(pstMeta)		(LPDRMETALIB) ( ((int)(pstMeta)) - (pstMeta)->ptrMeta - offsetof(DRMETALIB, data) )

#define DR_CHECK_BUFF(pszBuff, iBuff, iSize, iError)			      \
	if( iSize>0 && iSize<iBuff)					      \
	{								      \
		pszBuff += iSize;					      \
		iBuff	-= iSize;					      \
	}								      \
	else								      \
	{								      \
		iBuff	= 0; 						      \
		iError	= -1;						      \
	}


#define DR_GET_INT(i, iSize, p)		switch(iSize)			      \
{									      \
	case 2:								      \
		i = (int)*(unsigned short*)(p);				      \
		break;							      \
	case 4:								      \
		i = (int)*(unsigned int*)(p);				      \
		break;							      \
	default:							      \
		i = (int)*(unsigned char*)(p);				      \
}

#define DR_SET_INT(p, iSize, i)		switch(iSize)			      \
{									      \
	case 2:								      \
		*(unsigned short*)(p)	=	(unsigned short)(i);	      \
		break;							      \
	case 4:								      \
		*(unsigned int*)(p)	=	(unsigned int)(i);	      \
		break;							      \
	default:							      \
		*(unsigned char*)(p)	=	(unsigned char)(i);	      \
}

#define DR_GET_INT_NET(i, iSize, p)		switch(iSize)		      \
{									      \
	case 2:								      \
		i = (int)ntohs(*(unsigned short*)(p));			      \
		break;							      \
	case 4:								      \
		i = (int)ntohl(*(unsigned long*)(p));			      \
		break;							      \
	default:							      \
		i = (int)*(unsigned char*)(p);				      \
}

#define DR_SET_INT_NET(p, iSize, i)		switch(iSize)		      \
{									      \
	case 2:								      \
		*(unsigned short*)(p)	=	htons((unsigned short)(i));   \
		break;							      \
	case 4:								      \
		*(unsigned long*)(p)	=	htonl((unsigned long)(i));    \
		break;							      \
	default:							      \
		*(unsigned char*)(p)	=	(unsigned char)(i);	      \
}

#define DR_GET_SIZETYPE(p, iSize)	switch(iSize)			      \
{									      \
	case 1:								      \
		p = "uchar";						      \
		break;							      \
	case 2:								      \
		p = "ushort";						      \
		break;							      \
	case 4:								      \
		p = "int";						      \
		break;							      \
	default:							      \
		p = "";							      \
}


#define DR_GET_ENTRY(idx, entries, size, id)		{ 		      \
	int i;								      \
	int min;							      \
	int max;							      \
	int ret;							      \
	if( -1==entries[0].iID )					      \
		idx	=	0;					      \
	else								      \
		idx	=	-1;					      \
	i	=	id - entries[0].iID;				      \
	if( i>=0 && i<size && entries[i].iID==id )			      \
		idx	=	i;					      \
	else if( size<DR_MIN_BSEARCH )					      \
	{								      \
		for(i=0; i<size; i++)					      \
		{							      \
			if( entries[i].iID==id )		 	      \
			{						      \
				idx	=	i;			      \
				break;					      \
			}						      \
		}							      \
	}								      \
	else								      \
	{								      \
		min	=	0;					      \
		max	=	size - 1;				      \
		while(min<=max)						      \
		{							      \
			i	=	(min+max)>>1;			      \
			ret	=	id - entries[i].iID;		      \
			if( 0==ret )					      \
			{						      \
				idx	=	i;			      \
				break;					      \
			}						      \
			else if( ret<0 )				      \
				max	=	i - 1;			      \
			else						      \
				min	=	i + 1;			      \
		}							      \
	}								      \
}

#define DR_INC_UNION_ENTRY(pstTop, pstMeta, fChange)	{		      \
	pstTop->iCount--;						      \
	pstTop->pszHostBase +=	pstMeta->iData;				      \
	fChange	=	pstTop->iUnit;					      \
}

#define DR_INC_ENTRY(pstTop, pstMeta, fChange)		{		      \
	if( DR_TYPE_UNION==pstMeta->iType )				      \
	{								      \
		DR_INC_UNION_ENTRY(pstTop, pstMeta, fChange);		      \
	}								      \
	else								      \
	{								      \
		pstTop->iEntry++;					      \
		if( pstTop->iEntry >= pstMeta->iEntries )		      \
		{							      \
			pstTop->iEntry	=	0;			      \
			pstTop->iCount--;				      \
			pstTop->pszHostBase +=	pstMeta->iData;		      \
			fChange	=	pstTop->iUnit;			      \
		}							      \
	}								      \
}

#define DR_DEC_COUNT(pstTop, pstMeta, fChange)		{		      \
	pstTop->iCount--;						      \
	pstTop->pszHostBase +=	pstMeta->iData;				      \
	fChange	=	pstTop->iUnit;					      \
	if( DR_TYPE_STRUCT==pstMeta->iType )				      \
		pstTop->iEntry	=	0;				      \
}

#define DR_CALC_TARGET(pstTop, pszNet)			{		      \
	if( pstTop->iOff<0 )						      \
	{								      \
		pstTop->pszTarget =	pszNet;				      \
		pszNet		-=	pstTop->iOff;			      \
		pstTop->pszNetBase=	pszNet;				      \
	}								      \
	else								      \
	{								      \
		pstTop->pszNetBase=	pszNet;				      \
		pstTop->pszTarget =	(pstTop-1)->pszNetBase + pstTop->iOff;\
	}								      \
}

#define DR_CHECK_POINTER(s, size, e)		if( (s) + (size)>(e) ) return -1

/* macro functions end. */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public functions. */

int dr_typename_to_idx(const char* pszName);
LPDRTYPEINFO dr_idx_to_typeinfo(int iIdx);
int dr_typeid_to_idx(int iTypeID);

int dr_macro_by_name(DRMETALIB* pstLib, const char* pszName, int* piID);

int dr_create_metalib(LPDRMETALIB* ppstLib, const char* pszXml, int iXml);
int dr_create_metalib_fp(LPDRMETALIB* ppstLib, FILE* fp);
int dr_create_metalib_file(LPDRMETALIB* ppstLib, const char* pszFile);

int dr_init_metalib(DRMETALIB* pstLib, DRLIBPARAM* pstParam);

void dr_free_metalib(LPDRMETALIB pstLib);

int dr_save_metalib(LPDRMETALIB pstLib, char* pszXml, int* piXml);
int dr_save_metalib_fp(LPDRMETALIB pstLib, FILE* fp);
int dr_save_metalib_file(LPDRMETALIB pstLib, const char* pszFile);

int dr_load_bin_file(LPDRMETALIB* ppstLib, const char* pszFile);
int dr_load_bin_fp(LPDRMETALIB* ppstLib, FILE* fp);
int dr_save_bin_file(LPDRMETALIB pstLib, const char* pszFile);
int dr_save_bin_fp(LPDRMETALIB pstLib, FILE* fp);

int dr_add_meta(DRMETALIB* pstLib, const char* pszXml, int iXml);
int dr_remove_meta(DRMETALIB* pstLib, const char* pszName);

int dr_obfusticate(DRMETALIB* pstLib);

int dr_meta_by_name(DRMETALIB* pstLib, const char* pszName, LPDRMETA* ppstMeta);
int dr_meta_by_id(DRMETALIB* pstLib, int iID, LPDRMETA* ppstMeta);

int dr_entry_by_name(DRMETA* pstMeta, const char* pszName, int* piIdx);
int dr_entry_by_id(DRMETA* pstMeta, int iID, int* piIdx);

int dr_input(DRMETA* pstMeta, DRDATA* pstHost, DRDATA* pstXml, int iVersion);
int dr_input_fp(DRMETA* pstMeta, DRDATA* pstHost, FILE* fp, int iVersion);
int dr_input_file(DRMETA* pstMeta, DRDATA* pstHost, const char* pszPath, int iVersion);
int dr_output(DRMETA* pstMeta, DRDATA* pstXml, DRDATA* pstHost, int iVersion);
int dr_output_fp(DRMETA* pstMeta, FILE* fp, DRDATA* pstHost, int iVersion);
int dr_output_file(DRMETA* pstMeta, const char* pszFile, DRDATA* pstHost, int iVersion);

int dr_fprintf(DRMETA* pstMeta, FILE* fp, DRDATA* pstHost, int iVersion);
int dr_sprintf(DRMETA* pstMeta, DRDATA* pstOut, DRDATA* pstHost, int iVersion);

int dr_hton(DRMETA* pstMeta, DRDATA* pstNet, DRDATA* pstHost, int iVersion);
int dr_ntoh(DRMETA* pstMeta, DRDATA* pstHost, DRDATA* pstNet, int iVersion);

void dr_dump(FILE* fp, LPDRMETALIB pstLib);
int dr_metalib_to_hpp(DRMETALIB* pstLib, const char* pszFile, const char* pszLeading);

#ifdef __cplusplus
}
#endif

#endif	/* OV_DR_H */
