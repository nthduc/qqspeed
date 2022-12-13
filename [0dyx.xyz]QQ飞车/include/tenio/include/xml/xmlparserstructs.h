#ifndef __XMLPARSERSTRUCTS_H__
#define __XMLPARSERSTRUCTS_H__

//////////////////////////////////////////////////////////////////////////
// define some enums
//////////////////////////////////////////////////////////////////////////
enum MarkupDocFlags
{
	MDF_IGNORECASE = 8,
};
enum MarkupNodeFlags
{
	MNF_WITHCDATA      = 0x01,
	MNF_WITHNOLINES    = 0x02,
	MNF_WITHXHTMLSPACE = 0x04,
	MNF_WITHREFS       = 0x08,
	MNF_WITHNOEND      = 0x10,
	MNF_ESCAPEQUOTES  = 0x100,
	MNF_NONENDED   = 0x100000,
	MNF_ILLDATA    = 0x200000,
};
enum MarkupNodeType
{
	MNT_ELEMENT					= 1,  // 0x01
	MNT_TEXT					= 2,  // 0x02
	MNT_WHITESPACE				= 4,  // 0x04
	MNT_CDATA_SECTION			= 8,  // 0x08
	MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
	MNT_COMMENT					= 32, // 0x20
	MNT_DOCUMENT_TYPE			= 64, // 0x40
	MNT_EXCLUDE_WHITESPACE		= 123,// 0x7b
	MNT_LONE_END_TAG			= 128,// 0x80
	MNT_NODE_ERROR              = 32768 // 0x8000
};



#endif