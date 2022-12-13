/*
 * Jpeg.h
 *
 * The Independent JPEG Group's JPEG software
 * ==========================================
 * Copyright (C) 1994-1998, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 * 
 * 
 * README for release 6b of 27-Mar-1998
 * ====================================
 *
 * This distribution contains the sixth public release of the Independent JPEG
 * Group's free JPEG software.  You are welcome to redistribute this software and
 * to use it for any purpose, subject to the conditions under LEGAL ISSUES, below.
 * 
 * Serious users of this software (particularly those incorporating it into
 * larger programs) should contact IJG at jpeg-info@uunet.uu.net to be added to
 * our electronic mailing list.  Mailing list members are notified of updates
 * and have a chance to participate in technical discussions, etc.
 * 
 * This software is the work of Tom Lane, Philip Gladstone, Jim Boucher,
 * Lee Crocker, Julian Minguillon, Luis Ortiz, George Phillips, Davide Rossi,
 * Guido Vollbeding, Ge' Weijers, and other members of the Independent JPEG
 * Group.
 *
 * IJG is not affiliated with the official ISO JPEG standards committee.
 *
 * ====================================================
 * 
 * Jim(LiuZiJian), 2003.6.25.
 *	Modified for make library "Jpeg".
 *
 */
#ifndef	_JPEG_H_
#define	_JPEG_H_

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus


//
// PS: quality is in [1,100], 1 for lowest qulity, 100 for highest quality.
//
extern  int  write_jpeg_file(char * filename, int quality, int w, int h, unsigned char *rgb_buffer);


//
// PS: you must free memory of rgb_buffer by call free() after use it.
//
extern  int  read_jpeg_file(char * filename, int *w, int *h, unsigned char **rgb_buffer);



#ifdef  __cplusplus
}
#endif//__cplusplus


#endif//_JPEG_H_
