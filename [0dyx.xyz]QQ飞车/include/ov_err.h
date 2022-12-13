/****************************************************************************
*
*	This file used to definition the error-code returned.
*
*	If you do not want to print the meaning of the error, then just
*
*	include this file. 
*	
*	If you do print the error, then you need the expat and scew lib.
*
*	Because the description of the error code use XML file.
*
*	steve, 2003-12-22, reserved.
*
*****************************************************************************/

#ifndef OV_ERR_H
#define OV_ERR_H

#include <stdio.h>

/* a errcode is consist by three part:
* error-level: 1byte.
* mod_id: 1byte.
* errno: 2byte. 0-1023 is used by system for general error.
* 0 means no error.
* <0 means error.
* >0 means success, but something happened.
*/

typedef int HERR;

#ifdef _REENTRANT
#define  overrno				*err_get_location()
#else
extern HERR g_overrno;
#define  overrno				g_overrno
#endif

#define ERR_LEVEL_MIN				0x00
#define ERR_LEVEL_DEBUG				0x01
#define ERR_LEVEL_WARN				0x02
#define ERR_LEVEL_ERROR				0x03
#define ERR_LEVEL_SEVERE			0x04
#define ERR_LEVEL_MAX				ERR_LEVEL_SEVERE

#define ERR_NOERR				0
#define ERR_ERROR				-1

/* the minimum available user error number */
#define ERR_MAX_ERRNO				1024
#define ERR_SYS_ERRNO				ERR_MAX_ERRNO

#define ERR_MAX_STR				4096

#define ERR_MAKE(level, mod, errno)		(0x80000000 | (level<<24) | (mod<<16) | (errno)) 

#define ERR_MAKE_DEBUG(mod, errno)		ERR_MAKE(ERR_LEVEL_DEBUG, mod, errno)
#define ERR_MAKE_WARN(mod, errno)		ERR_MAKE(ERR_LEVEL_WARN, mod, errno)
#define ERR_MAKE_ERROR(mod, errno)		ERR_MAKE(ERR_LEVEL_ERROR, mod, errno)
#define ERR_MAKE_SEVERE(mod, errno)		ERR_MAKE(ERR_LEVEL_SEVERE, mod, errno)

#define ERR_MAKE_CODE(mod, code)		( (mod)<<16 | (code) )

#define ERR_GET_MOD(error)			( ( error & 0x00ff0000 ) >> 16 )
#define ERR_GET_CODE(error)			( error & 0xffff )
#define ERR_GET_ERRNO(error)			( error & 0xffff )
#define ERR_GET_LEVEL(error)			( (error & 0x0f000000) >> 24 )

#define ERR_IS_ERROR(error)			( error & 0x80000000 )


#define ERR_NOMEM				0x01
#define ERR_BADARG				0x02
#define ERR_BADHANDLE				0x03
#define ERR_BUFF				0x04
#define ERR_SEARCH				0x05 
#define ERR_REPEAT				0x06 
#define ERR_NONEXIST				0x07 


#ifdef __cplusplus
extern "C"
{
#endif

HERR err_init(const char* pszPath, const char* pszLang, int iFlags);
void err_release(void);

HERR err_set_time_format(const char* pszFormat);
HERR err_set_output_format(const char* pszFormat);

HERR err_tostr(HERR hErr, char* pszErr, int* piLen);
HERR err_error(int iFile, HERR hErr);
HERR err_ferror(FILE* fp, HERR hErr);
void err_perror(HERR hErr);

HERR* err_get_location(void);

#ifdef __cplusplus
}
#endif

#endif /* OV_ERR_H */
