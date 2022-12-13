#ifndef __INISTRUCT_H__
#define __INISTRUCT_H__

namespace Tenio
{
	enum
	{
		MAX_INIKEYNAMELEN  = 128,
		MAX_INIKEYVALUELEN = 512,
		MAX_INISECTIONNAMELEN = 128
	};

	typedef struct _tagINIKEYINFO
	{
		TCHAR szKeyName[MAX_INIKEYNAMELEN];
		TCHAR szKeyValue[MAX_INIKEYVALUELEN];
	}INIKEYINFO;
	
	typedef struct _tagINISectionInfo
	{
		TCHAR szSectionName[MAX_INISECTIONNAMELEN];
	}SECTIONINFO;
};

#endif