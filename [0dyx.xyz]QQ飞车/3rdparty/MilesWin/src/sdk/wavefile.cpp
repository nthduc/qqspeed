//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  WVFILE.C: Digital sound API module for digital sound file access      ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 9.0                  ##
//##                                                                        ##
//##  Version 1.00 of 15-Feb-95: Derived from WAILSFIL V1.00                ##
//##          1.10 of 11-May-97: Added IMA ADPCM support (Serge Plagnol)    ##
//##          1.20 of 10-May-02: Massive changes for new reverb, many       ##
//##                             functions moved from mssdig.cpp (JKR)      ##
//##                                                                        ##
//##  Author: John Miles and Jeff Roberts                                   ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include <stdio.h>
#include <string.h>

#include "mss.h"
#include "imssapi.h"

#ifndef IS_WIN32
#include <stdarg.h>
#endif

#include <math.h>

//
// Recognized file types
//

#define FTYP_VOC  0
#define FTYP_WAV  1
#define FTYP_ASI  2

#define  MY_PI  3.14159265F


//
// .WAV FACT chunk
//

typedef struct
{
  S8  FACT_string[ 4 ];
  U32 chunk_size;
  U32 samples;
}
FACT;

//
//
// .VOC terminator block
//

typedef struct
{
  U8 block_ID;
}
BLK_0;

//
// .VOC voice block
//

typedef struct
{
  U8 block_ID;
  U8 block_len[ 3 ];
  U8 time_constant;
  U8 pack_method;
}
BLK_1;

//
// .VOC continued voice block
//

typedef struct
{
   U8 block_ID;
   U8 block_len[ 3 ];
}
BLK_2;

//
// .VOC silence block
//

typedef struct
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U16 pause_period;
  U8  time_constant;
}
BLK_3;

//
// .VOC marker block
//

typedef struct
{
  U8  block_ID;
  U8  block_len[ 3 ];
  S16 marker;
}
BLK_4;

//
// .VOC ASCIIZ comment block
//

typedef struct
{
  U8 block_ID;
  U8 block_len[ 3 ];
  S8 string;
}
BLK_5;

//
// .VOC repeat loop block
//

typedef struct
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U16 repeat_count;
}
BLK_6;

//
// .VOC end-of-loop block
//

typedef struct
{
  U8 block_ID;
  U8 block_len[ 3 ];
}
BLK_7;

//
// .VOC extended attribute block
//
// (always followed by block 1)
//

typedef struct
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U16 time_constant;
  U8  pack_method;
  U8  voice_mode;
}
BLK_8;

//
// .VOC extended voice block
//
// (replaces blocks 1 and 8)
//

typedef struct
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U32 sample_rate;
  U8  bits_per_sample;
  U8  channels;
  U16 format;
  U8  reserved[ 4 ];
}
BLK_9;

//
// .WAV file headers
//

typedef struct
{
  S8  RIFF_string[ 4 ];
  U32 chunk_size;
  S8  ID_string[ 4 ];
  U8  data[ 1 ];
}
RIFF;

//
// .WAV PCM file format chunk
//

typedef struct
{
  S8   FMT_string[ 4 ];
  U32  chunk_size;

  S16  format_tag;
  S16  channels;
  S32  sample_rate;
  S32  average_data_rate;
  S16  alignment;
  S16  bits_per_sample;
  S16 extra;
  S16 samples_per_block;
}
FMT;

//
// .WAV file data chunk
//

typedef struct
{
  S8  DATA_string[ 4 ];
  U32 chunk_size;
  U8  data[ 1 ];
}
DATA;

extern C8 AIL_error[ 256 ];

#ifdef IS_DOS

//############################################################################
//##                                                                        ##
//## Locked code                                                            ##
//##                                                                        ##
//############################################################################

#define LOCK( x )   AIL_vmm_lock  ( &( x ), sizeof( x ) )
#define UNLOCK( x ) AIL_vmm_unlock( &( x ), sizeof( x ) )

static S32 locked = 0;

void AILSFILE_end( void );

void AILSFILE_start( void )
{
  if ( !locked )
  {
    AIL_vmm_lock_range( AILSFILE_start, AILSFILE_end );

    locked = 1;
  }
}

#define DOLOCK() AILSFILE_start()

#else

#define DOLOCK()

#endif


static void clear_last_sample(HSAMPLE S)
{
   S->left_val       =  0;
   S->right_val      =  0;
   S->last_decomp_left  =  65536;
   S->src_fract      =  0;
}

//############################################################################
//##                                                                        ##
//## Get length of .VOC block                                               ##
//##                                                                        ##
//############################################################################

static U32 AIL_VOC_block_len( void FAR *block )
{
  return( ( * (U32 FAR *) block ) >> 8 );
}


//#define CHECKDENORM
#ifdef CHECKDENORM

static void check_denorm( float * f )
{
  union
  {
    float *f;
    unsigned int *i;
  } both;

  both.f = f;

  if ( *f !=0.0f )
  {
    if ((( both.i[0] >>23 )&255) == 0 )
      MSSBreakPoint();
    if ((( both.i[0] >>23 )&255) == 255 )
      MSSBreakPoint();
  }
}

#else

#define check_denorm( ptr )

#endif

//############################################################################
//##                                                                        ##
//## Terminate playback of .VOC file                                        ##
//##                                                                        ##
//## Invoke application callback function, if any, and release the sample   ##
//## allocated to play this file                                            ##
//##                                                                        ##
//############################################################################

static void AIL_VOC_terminate(HSAMPLE sample)
{
   if (sample->system_data[SSD_EOD_CALLBACK] != (U32) NULL)
      {
      MSS_do_cb1( (AILSAMPLECB),
        (AILSAMPLECB)sample->system_data[SSD_EOD_CALLBACK], sample->driver->callingDS,
          sample);
      }

   if (sample->system_data[SSD_RELEASE] > 0)
      {
      AIL_release_sample_handle(sample);
      }

   sample->system_data[SSD_RELEASE] = -1;
}

//############################################################################
//##                                                                        ##
//## Process .VOC file block                                                ##
//##                                                                        ##
//## Called by .VOC initialization code and as end-of-sample callback       ##
//## function (interrupt-based)                                             ##
//##                                                                        ##
//## If play_flag clear, search for first block after desired marker (if    ##
//## any) and return without playing it                                     ##
//##                                                                        ##
//############################################################################

static void AIL_process_VOC_block(HSAMPLE sample, S32 play_flag)
{
   S32  voice_block;
   void FAR *b;

   voice_block = 0;

   //
   // Loop until voice block is found
   //

   while (!voice_block)
      {
      b = (void FAR *) sample->system_data[VOC_BLK_PTR];

      switch (*(U8 FAR *) b)
         {
         //
         // Terminator block
         //

         case 0:

            //
            // Terminate playback, then return without trying to advance
            // to next block
            //

            AIL_VOC_terminate(sample);

            return;

         //
         // Voice block
         //

         case 1:

            //
            // Skip block if desired marker has not been found
            //

            if (!sample->system_data[VOC_MARKER_FOUND])
               {
               break;
               }

            //
            // Set up sample data and start playback
            //

            AIL_set_sample_address(sample,
                                   AIL_ptr_add(b, sizeof(BLK_1)),
                                   AIL_VOC_block_len(b) - 2);

            AIL_set_sample_playback_rate(sample,(U32)
                             1000000L / (256 - ((BLK_1 FAR *) b)->time_constant));

            AIL_set_sample_type(sample,DIG_F_MONO_8,0);

            if (play_flag)
               AIL_start_sample(sample);

            voice_block = 1;
            break;

         //
         // Marker block
         //

         case 4:

            //
            // Ignore if entire file to be played
            //

            if (sample->system_data[VOC_MARKER] == -1)
               {
               break;
               }

            //
            // If this is the desired marker, set MARKER_FOUND flag --
            // otherwise, clear MARKER_FOUND flag to prevent playback
            // of future voice blocks
            //

            if (sample->system_data[VOC_MARKER] == (S32)
                                                   ((BLK_4 FAR *) b)->marker)
               {
               sample->system_data[VOC_MARKER_FOUND] = 1;
               }
            else
               {
               sample->system_data[VOC_MARKER_FOUND] = 0;
               }

            break;

         //
         // Repeat block
         //

         case 6:

            //
            // Log repeat count and starting address of repeat block
            //

            sample->system_data[VOC_REP_BLK] = (U32) b;

            sample->system_data[VOC_N_REPS]  = (U32)
                                             ((BLK_6 FAR *) b)->repeat_count;
            break;

         //
         // End repeat block
         //

         case 7:

            //
            // If finite repeat block active, check and decrement repeat
            // count
            //

            if (sample->system_data[VOC_N_REPS] != 0xffff)
               {
               if (sample->system_data[VOC_N_REPS]-- == 0)
                  {
                  break;
                  }
               }

            b = (void FAR *) sample->system_data[VOC_REP_BLK];
            break;

         //
         // Extended attribute block
         // (followed by block 1)
         //

         case 8:

            //
            // Skip block if desired marker has not been found
            //

            if (!sample->system_data[VOC_MARKER_FOUND])
               {
               break;
               }

            //
            // Set up sample data and start playback
            //

            if (((BLK_8 FAR *) b)->voice_mode)
               {
               AIL_set_sample_type(sample,DIG_F_STEREO_8,0);

               AIL_set_sample_playback_rate(sample,(U32)
                  128000000L / (65536L - ((BLK_8 FAR *) b)->time_constant));
               }
            else
               {
               AIL_set_sample_type(sample,DIG_F_MONO_8,0);

               AIL_set_sample_playback_rate(sample,(U32)
                  256000000L / (65536L - ((BLK_8 FAR *) b)->time_constant));
               }

            //
            // Advance to paired voice block (type 1) in .VOC image
            //

            b = (C8 FAR *)AIL_ptr_add(b, AIL_VOC_block_len(b) + 4);

            //
            // Set sample address and size, and start playback
            //

            AIL_set_sample_address(sample,
                                   AIL_ptr_add(b, sizeof(BLK_1)),
                                   AIL_VOC_block_len(b) - 2);

            if (play_flag)
               AIL_start_sample(sample);

            voice_block = 1;
            break;

         //
         // Extended voice block
         //

         case 9:

            //
            // Skip block if desired marker has not been found
            //

            if (!sample->system_data[VOC_MARKER_FOUND])
               {
               break;
               }

            //
            // Set up sample data and start playback
            //

            AIL_set_sample_address(sample,
                                   AIL_ptr_add(b, sizeof(BLK_9)),
                                   AIL_VOC_block_len(b) - 12);

            AIL_set_sample_playback_rate(sample, ((BLK_9 FAR *) b)->sample_rate);

            if ((((BLK_9 FAR *) b)->channels == 1) &&
                (((BLK_9 FAR *) b)->format   == 0))
               {
               AIL_set_sample_type(sample,DIG_F_MONO_8,0);
               }
            else if ((((BLK_9 FAR *) b)->channels == 2) &&
                     (((BLK_9 FAR *) b)->format   == 0))
               {
               AIL_set_sample_type(sample,DIG_F_STEREO_8,0);
               }
            else if ((((BLK_9 FAR *) b)->channels == 1) &&
                     (((BLK_9 FAR *) b)->format   == 4))
               {
               AIL_set_sample_type(sample,DIG_F_MONO_16,DIG_PCM_SIGN);
               }
            else if ((((BLK_9 FAR *) b)->channels == 2) &&
                     (((BLK_9 FAR *) b)->format   == 4))
               {
               AIL_set_sample_type(sample,DIG_F_STEREO_16,DIG_PCM_SIGN);
               }

            if (play_flag)
               AIL_start_sample(sample);

            voice_block = 1;
            break;
         }

      //
      // Advance pointer to next block in .VOC image
      //

      sample->system_data[VOC_BLK_PTR] = (U32)
                                         AIL_ptr_add(b, AIL_VOC_block_len(b) + 4);
      }
}

//############################################################################
//##                                                                        ##
//## End-of-sample callback handler for .VOC file playback                  ##
//##                                                                        ##
//############################################################################

static void AILLIBCALLBACK AIL_VOC_EOS( HSAMPLE sample )
{
  AIL_process_VOC_block( sample, 1 );
}


//############################################################################
//##                                                                        ##
//## Create sample instance by parsing .WAV file                            ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_WAV_info(void const FAR* data, AILSOUNDINFO FAR* info)
{
   void  FAR *end;
   FMT   FAR *f;
   DATA  FAR *d;
   FACT  FAR *fa;

   DOLOCK();

   if (data==0)
     goto error;

   end=AIL_ptr_add(data,LE_SWAP32(&((RIFF FAR *) data)->chunk_size));

   if (AIL_strnicmp((char FAR*)(((RIFF FAR *) data)->ID_string),"WAVE",4))
      {
     error:
      AIL_strcpy(AIL_error,"Not a WAV file.");
      return(0);
      }

   //
   // Find mandatory <fmt-ck>
   //

   f = (FMT FAR *) (((RIFF *) data)->data);

   while (AIL_strnicmp((char FAR*)f->FMT_string,"fmt ",4))
      {
      U32 chunk_size = LE_SWAP32( &f->chunk_size );
      f = (FMT FAR *) AIL_ptr_add(f, chunk_size + 8 + (chunk_size & 1));
      if (AIL_ptr_dif(f,end)>=0)
        goto error;
      }

   info->format= LE_SWAP16( &f->format_tag );
   info->rate= LE_SWAP32( &f->sample_rate );
   info->bits= LE_SWAP16( &f->bits_per_sample );
   info->channels= LE_SWAP16( &f->channels );
   info->block_size= LE_SWAP16( &f->alignment );

   //
   // Find mandatory <data-ck>
   //

   d = (DATA FAR *) (((RIFF *) data)->data);

   while (AIL_strnicmp((char FAR*)d->DATA_string,"data",4))
      {
      U32 chunk_size = LE_SWAP32( &d->chunk_size );
      d = (DATA FAR *) AIL_ptr_add( d, chunk_size + 8 + (chunk_size & 1));
      if (AIL_ptr_dif(d,end)>=0)
        goto error;
      }

   info->data_ptr=d->data;
   info->data_len= LE_SWAP32( &d->chunk_size );

   if ((info->format==WAVE_FORMAT_IMA_ADPCM) && (info->bits==4))
   {

     fa = (FACT FAR *) (((RIFF *) data)->data);

     while (AIL_strnicmp((char FAR*)fa->FACT_string,"fact",4))
        {
        U32 chunk_size = LE_SWAP32( &fa->chunk_size );
        fa = (FACT FAR *) AIL_ptr_add( fa, chunk_size + 8 + (chunk_size & 1));
        if (AIL_ptr_dif(fa,end)>=0)
          {

          U32 samples_per_block = 4 << (info->channels/2);
          samples_per_block = 1 + (info->block_size-samples_per_block)*8 / samples_per_block;
          info->samples=((info->data_len+info->block_size-1)/info->block_size)*samples_per_block;

          goto nofact;
          }
       }
       info->samples= LE_SWAP32( &fa->samples );
     nofact:;
   } else
     info->samples=(info->bits==0)?0:((info->data_len*8)/info->bits);

   info->initial_ptr=info->data_ptr;

   return(1);
}


//############################################################################
//##                                                                        ##
//## Create sample instance by parsing .WAV file                            ##
//##                                                                        ##
//############################################################################

void AIL_process_WAV_image( AILSOUNDINFO FAR const * info, HSAMPLE sample )
{
  S32 format=0;
  S32 sign=0;

  if ( info->channels == 2 )
  {
    format = DIG_F_STEREO_MASK ;
  }

#ifdef IS_XBOX
  if ( info->format == WAVE_FORMAT_XBOX_ADPCM )
  {
    format |= DIG_F_XBOX_ADPCM_MASK;
  }
  else
#endif
  {
    switch( info->bits )
    {
      case 4 :
        format |= DIG_F_ADPCM_MASK ;
      case 16:
        format |= DIG_F_16BITS_MASK ;
        sign   = DIG_PCM_SIGN ;
    }
  }

  AIL_set_sample_type( sample, format, sign );

  AIL_set_sample_playback_rate( sample, info->rate );

  //
  // Configure adpcm if required
  //

  if ( format & DIG_F_ADPCM_MASK )
  {
    AIL_set_sample_adpcm_block_size( sample, info->block_size );
  }

  //
  // Configure sample address and length based on DATA chunk
  //

  AIL_set_sample_address( sample, info->data_ptr, info->data_len );
}


//############################################################################
//##                                                                        ##
//## End-of-sample callback handler for .WAV file playback                  ##
//##                                                                        ##
//############################################################################

static void AILLIBCALLBACK AIL_WAV_EOS(HSAMPLE sample)
{
  if ( sample->system_data[ SSD_EOD_CALLBACK ] != (U32) NULL )
  {
    MSS_do_cb1( (AILSAMPLECB),
                (AILSAMPLECB) sample->system_data[ SSD_EOD_CALLBACK ], sample->driver->callingDS,
                sample );
  }

  if ( sample->system_data[ SSD_RELEASE ] > 0 )
  {
    AIL_release_sample_handle( sample );
  }

  sample->system_data[ SSD_RELEASE ] = -1;
}

//############################################################################
//##                                                                        ##
//## Play memory-resident file image                                        ##
//##                                                                        ##
//## Returns NULL on error, else handle to sample assigned to file          ##
//##                                                                        ##
//############################################################################

HSAMPLE AILCALL AIL_API_allocate_file_sample(HDIGDRIVER dig, void const FAR *file_image, S32 block)
{
   HSAMPLE sample;
   AILSOUNDINFO info;
   S32    type;
   char FAR * asifileext;

   DOLOCK();

   if (file_image==NULL)
     return(0);

   //
   // Identify file type
   //
   // Note: Currently only single-sample PCM .WAV files are supported, since
   // no known applications generate other formats for testing
   //

   if (!AIL_strnicmp((char FAR*)(((VOC FAR *) file_image)->ID_string),"Creative",8))
      {
      type = FTYP_VOC;
      }
   else if (AIL_WAV_info(file_image,&info))
      {
      if ( (info.format==WAVE_FORMAT_PCM)
           || ((info.format==WAVE_FORMAT_IMA_ADPCM) && (info.bits==4))
#ifdef IS_XBOX
           || ((info.format==WAVE_FORMAT_XBOX_ADPCM) && (info.bits==4))
#endif
         )
        type = FTYP_WAV;
      else
      {

        // handle asi wave files

        HPROVIDER ASI = RIB_find_file_dec_provider( "ASI codec",
                                                    "Input wave tag",
                                                    info.format,
                                                    "Output file types",
                                                    ".raw" );

        if (ASI==0)
        {
           asierr:
            AIL_set_error("Unsupported wave file format.");
            return NULL;
        }

        type = FTYP_ASI;

        PROVIDER_QUERY_ATTRIBUTE query_attribute = NULL;
        U32 token=0;

        RIB_request_interface_entry(ASI,
                                    "ASI codec",
                                    RIB_FUNCTION,
                                   "PROVIDER_query_attribute",
                        (U32 FAR *) &query_attribute);

        if (RIB_request_interface_entry(ASI,
                                        "ASI codec",
                                        RIB_ATTRIBUTE,
                                        "Input file types",
                                         &token) == RIB_NOERR)
        {
          asifileext = (char FAR*)query_attribute(token);

          //
          // try to find the extension
          //

          if (asifileext)
          {
            while ((asifileext[0]!=0) || (asifileext[1]!=0))
            {
              if ((asifileext[0]=='*') && (asifileext[1]=='.'))
                break;
              ++asifileext;
            }
          }

        }
        else
        {
          goto asierr;
        }

     }
   }
   else
      {
      AIL_set_error("Unknown file type.");
      return NULL;
      }

   //
   // Allocate and initialize sample for file
   //

   sample = AIL_allocate_sample_handle(dig);

   if (sample == NULL)
      {
      return NULL;
      }

   AIL_init_sample(sample);

   sample->system_data[SSD_EOD_CALLBACK] = (U32) NULL;

   //
   // Copy file attributes to sample
   //

   switch (type)
      {
      case FTYP_VOC:

         sample->system_data[VOC_BLK_PTR]      = (U32) AIL_ptr_add(file_image,
                                                 ((VOC FAR *) file_image)->
                                                 data_offset);

         sample->system_data[VOC_MARKER]       = block;
         sample->system_data[VOC_MARKER_FOUND] = (block == -1);
         sample->system_data[SSD_RELEASE]      = 1;

         AIL_register_EOS_callback(sample, AIL_VOC_EOS);

         AIL_process_VOC_block(sample,0);
         break;

      case FTYP_WAV:

         sample->system_data[SSD_RELEASE] = 1;

         AIL_register_EOS_callback(sample, AIL_WAV_EOS);

         AIL_process_WAV_image(&info,sample);
         break;

      case FTYP_ASI:
         sample->system_data[SSD_RELEASE] = 1;

         AIL_register_EOS_callback(sample, AIL_WAV_EOS);

         AIL_set_named_sample_file(sample,asifileext,info.data_ptr,info.data_len,block);
         break;

      }

   //
   // Return NULL if parser rejected sample file image, or handle if OK
   //

   if (sample->system_data[SSD_RELEASE] == -1)
      {
      AIL_set_error("Invalid or missing data block.");
      return NULL;
      }

   return sample;
}

//############################################################################
//##                                                                        ##
//## Set parameters of existing HSAMPLE according to info struct            ##
//##                                                                        ##
//## Returns 0 on error, else 1                                             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_set_sample_info(HSAMPLE S, AILSOUNDINFO const FAR * info)
{
  S32 format=0;
  S32 sign=0;
  HPROVIDER HP = 0;

  DOLOCK();

   if ((S==NULL) || (info==NULL))
     return(0);

  AIL_init_sample( S );
  
  if ( info->channels == 2 )
  {
    format = DIG_F_STEREO_MASK;
  }

#ifdef IS_XBOX
  if ( info->format == WAVE_FORMAT_XBOX_ADPCM )
  {
    format |= DIG_F_XBOX_ADPCM_MASK;
  }
  else
#endif
  {
    if (info->format==WAVE_FORMAT_PCM)
    {
      if ( info->bits == 16 ) 
      {
        format |= DIG_F_16BITS_MASK;
        sign   = DIG_PCM_SIGN;
      }
      else if ( info->bits != 8 )
      {
        goto wave_err;
      }
    }
    else if ((info->format==WAVE_FORMAT_IMA_ADPCM) && (info->bits==4))
    {
      format |= DIG_F_ADPCM_MASK;
    }    
    else
    {
      // find asi
      HP = RIB_find_file_dec_provider( "ASI codec",
                                       "Input wave tag",
                                       info->format,
                                       "Output file types",
                                       ".raw" );

      if (!HP)
      {
       wave_err: 
        AIL_set_error("Unsupported wave file format.");
        return 0;
      }
    }
  }

  AIL_set_sample_type( S, format, sign );
  AIL_set_sample_playback_rate( S, info->rate );

  //
  // Configure adpcm if required
  //

  if ( format & DIG_F_ADPCM_MASK )
  {
    AIL_set_sample_adpcm_block_size( S, info->block_size );
  }

  //
  // Configure sample address and length based on DATA chunk
  //

  AIL_set_sample_address( S, info->data_ptr, info->data_len );

  //
  // Set up to use specified ASI codec to decode data for mixer
  //

  AIL_set_sample_processor(S,
                           DP_ASI_DECODER,
                           HP);

  return( 1 );                           
}

//############################################################################
//##                                                                        ##
//## Set parameters of existing HSAMPLE according to file data              ##
//##                                                                        ##
//## Returns 0 on error, else 1                                             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_set_sample_file(HSAMPLE S, void const FAR *file_image, S32 block)
{
   S32 type;
   AILSOUNDINFO info;

   DOLOCK();

   if ((S==NULL) || (file_image==NULL))
     return(0);

   clear_last_sample(S);

   S->adpcm.blocksize=  256;
   S->adpcm.blockleft = 0;
   S->adpcm.extrasamples = 0;

   //
   // Identify file type
   //
   //

   if (!AIL_strnicmp((char FAR*)(((VOC FAR *) file_image)->ID_string),"Creative",8))
      {
      type = FTYP_VOC;
      }
   else if (AIL_WAV_info(file_image,&info))
      {
      if ( (info.format==WAVE_FORMAT_PCM)
           || ((info.format==WAVE_FORMAT_IMA_ADPCM) && (info.bits==4))
#ifdef IS_XBOX
           || ((info.format==WAVE_FORMAT_XBOX_ADPCM) && (info.bits==4))
#endif
         )
        type = FTYP_WAV;
      else
        {

        // handle asi wave files

        HPROVIDER ASI = RIB_find_file_dec_provider( "ASI codec",
                                                    "Input wave tag",
                                                    info.format,
                                                    "Output file types",
                                                    ".raw" );
        if ( ASI )
        {
          //
          // set sample address and size for encoded data image
          //
  
          AIL_set_sample_address(S,info.data_ptr,info.data_len );
  
          //
          // Set up to use specified ASI codec to decode data for mixer
          //
  
          AIL_set_sample_processor(S,
                                   DP_ASI_DECODER,
                                   ASI);
          return( 1 );
        }

        AIL_set_error("Unsupported wave file format.");
        return 0;
        }
      }
   else
      {
      AIL_set_error("Unknown file type.");
      return 0;
      }

   //
   // Copy file attributes to sample
   //

   switch (type)
      {
      case FTYP_VOC:

         S->system_data[VOC_BLK_PTR]      = (U32) AIL_ptr_add(file_image,
                                                 ((VOC FAR *) file_image)->
                                                 data_offset);

         S->system_data[VOC_MARKER]       = block;
         S->system_data[VOC_MARKER_FOUND] = (block == -1);

         S->system_data[SSD_RELEASE]      = 0;

         AIL_process_VOC_block(S,0);
         break;

      case FTYP_WAV:

         S->system_data[SSD_RELEASE] = 0;

         AIL_process_WAV_image(&info,S);
         break;
      }

   //
   // Return NULL if parser rejected sample file image, or 1 if OK
   //

   if (S->system_data[SSD_RELEASE] == -1)
      {
      AIL_set_error("Invalid or missing data block.");
      return 0;
      }

   return 1;
}

//############################################################################
//##                                                                        ##
//## Set parameters of existing HSAMPLE according to file data, using       ##
//## the file suffix to specify the file type                               ##
//##                                                                        ##
//## Returns 0 on error, else 1                                             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_set_named_sample_file (HSAMPLE   S, //)
                                           C8   const FAR *file_type_suffix,
                                           void const FAR *file_image,
                                           S32       file_size,
                                           S32       block)
{
   DOLOCK();

   //
   // is this a wacky id3?
   //

   if ( ( ((U8 const FAR* )file_image)[ 0 ] == 0x49 ) && ( ((U8 const FAR* )file_image)[ 1 ] == 0x44 ) && ( ((U8 const FAR* )file_image)[ 2 ] == 0x33 ) &&
        ( ((U8 const FAR* )file_image)[ 3 ] < 0xff ) && ( ((U8 const FAR* )file_image)[ 4 ] < 0xff ) &&
        ( ((U8 const FAR* )file_image)[ 6 ] < 0x80 ) && ( ((U8 const FAR* )file_image)[ 7 ] < 0x80 ) && ( ((U8 const FAR* )file_image)[ 8 ] < 0x80 ) && ( ((U8 const FAR* )file_image)[ 9 ] < 0x80 ) )
   {
     U32 skip = 10 + ( (U32) ((U8 const FAR* )file_image)[ 9 ] ) | ( ( (U32) ((U8 const FAR* )file_image)[ 8 ] ) << 7 ) |
                   ( ( (U32) ((U8 const FAR* )file_image)[ 7 ] ) << 14 ) | ( ( (U32) ((U8 const FAR* )file_image)[ 6 ] ) << 21 );

     file_image = ( (U8 FAR*) file_image ) + skip;
     file_size -= skip;
     file_type_suffix = ".mp3";
   }

   //
   // If file type not specified, pass to .WAV / VOC handler
   //

   if ((file_type_suffix == NULL) ||
       (!AIL_strlen(file_type_suffix)))
      {
      return AIL_set_sample_file(S,
                                 file_image,
                                 block);
      }

   //
   // Search for ASI codec capable of processing this input file type
   //

   HPROVIDER HP = RIB_find_file_provider("ASI codec",
                                         "Input file types",
                                         file_type_suffix);

   //
   // If no specific provider available for this file type, try the default
   // .WAV/.VOC file handler
   //

   if (!HP)
      {
      if (AIL_strlen(file_type_suffix) >= 4)
         {
         if ((!AIL_stricmp(&file_type_suffix[AIL_strlen(file_type_suffix)-4],
                        ".WAV"))
               ||
            (!AIL_stricmp(&file_type_suffix[AIL_strlen(file_type_suffix)-4],
                        ".VOC")))
            {
            return AIL_set_sample_file(S,
                                       file_image,
                                       block);
            }
         }

      //
      // Not a .WAV or .VOC file, fail call
      //

      return 0;
      }


   // reset default filter values

   clear_last_sample(S);

   //
   // Otherwise, set sample address and size for encoded data image
   //

   AIL_set_sample_address(S,
                          file_image,
                          file_size);

   //
   // Set up to use specified ASI codec to decode data for mixer
   //

   AIL_set_sample_processor(S,
                            DP_ASI_DECODER,
                            HP);

   //
   // Return success
   //

   return 1;
}

//############################################################################
//##                                                                        ##
//## Set a RIB provider for a specified sample pipeline phase               ##
//##                                                                        ##
//############################################################################

HPROVIDER    AILCALL AIL_API_set_sample_processor  (HSAMPLE     S, //)
                                                    SAMPLESTAGE pipeline_stage,
                                                    HPROVIDER   provider)
{
   DOLOCK();

   S32 stage = pipeline_stage;

   if (stage == SAMPLE_ALL_STAGES)
      {
      stage = 0;
      }

   DPINFO FAR *DP;

   do
      {
      DP = &S->pipeline[stage];

      //
      // If existing stage is being replaced, shut it down first
      //

      if (DP->active)
         {
         switch (stage)
            {
            //
            // "MSS mixer services"
            //

            case DP_MERGE:
               break;

            //
            // "ASI codec stream"
            //

            case DP_ASI_DECODER:
               DP->TYPE.ASI.ASI_stream_close(DP->TYPE.ASI.stream);
               break;

            //
            // "MSS pipeline filter"
            //

            case DP_FILTER:

               #if defined(IS_WIN32) || defined(IS_MAC) || defined(IS_LINUX)
               DP->TYPE.FLT.provider->close_sample(DP->TYPE.FLT.sample_state);
               DP->TYPE.FLT.sample_state=0;
               #endif
               break;
            }

         DP->active = 0;
         }
      }
   while ((pipeline_stage == SAMPLE_ALL_STAGES) && (++stage < N_SAMPLE_STAGES));

   //
   // Set up to return previous provider
   //

   HPROVIDER previous = DP->provider;

   DP->provider = provider;

   //
   // If no new provider specified, exit normally after shutting down
   // existing provider
   //

   if (!provider)
      {
      return previous;
      }

   //
   // Query required attributes from provider interface
   //

   switch (stage)
      {
      //
      // "MSS mixer services"
      //

      case DP_MERGE:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         MIXSTAGE FAR *MIX = &DP->TYPE.MIX;

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_merge", (U32) &MIX->MSS_mixer_merge, RIB_NONE },
            };

         RIB_request(DP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         DP->active = 1;
         break;
         }

      //
      // "MSS pipeline filter"
      //

      case DP_FILTER:
         {

         #if defined(IS_WIN32) || defined(IS_MAC) || defined(IS_LINUX)

         //
         // open the filter if it ain't already opened
         //

         AIL_open_filter( provider, S->driver );

         //
         // Find DRIVERSTATE wrapper for this provider and driver
         //

         FLTPROVIDER FAR *F = FLT_find_provider_instance(provider, S->driver);

         DP->TYPE.FLT.provider = F;

         DP->TYPE.FLT.sample_state = 0;

         if ((F==NULL) || (F->open_sample==NULL))
           {
           return 0;
           }

         //
         // Create SAMPLESTATE structure for this HSAMPLE and HDRIVERSTATE
         //

         DP->TYPE.FLT.sample_state = F->open_sample(F->driver_state,
                                                    S);

         if (!DP->TYPE.FLT.sample_state)
           {
           return 0;
           }

         //
         // Indicate installation of filter module
         //

         DP->active = 1;

         #endif

         break;
         }

      //
      // "ASI codec stream"
      //

      case DP_ASI_DECODER:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         ASISTAGE FAR *ASI = &DP->TYPE.ASI;

         RIB_INTERFACE_ENTRY ASISTR[] =
            {
            { RIB_FUNCTION,   "ASI_stream_attribute",      (U32) &ASI->ASI_stream_attribute,      RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_open",           (U32) &ASI->ASI_stream_open,           RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_seek",           (U32) &ASI->ASI_stream_seek,           RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_close",          (U32) &ASI->ASI_stream_close,          RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_process",        (U32) &ASI->ASI_stream_process,        RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_set_preference", (U32) &ASI->ASI_stream_set_preference, RIB_NONE },
            { RIB_ATTRIBUTE,  "Input bit rate",            (U32) &ASI->INPUT_BIT_RATE,            RIB_NONE },
            { RIB_ATTRIBUTE,  "Input sample rate",         (U32) &ASI->INPUT_SAMPLE_RATE,         RIB_NONE },
            { RIB_ATTRIBUTE,  "Input sample width",        (U32) &ASI->INPUT_BITS,                RIB_NONE },
            { RIB_ATTRIBUTE,  "Input channels",            (U32) &ASI->INPUT_CHANNELS,            RIB_NONE },
            { RIB_ATTRIBUTE,  "Output bit rate",           (U32) &ASI->OUTPUT_BIT_RATE,           RIB_NONE },
            { RIB_ATTRIBUTE,  "Output sample rate",        (U32) &ASI->OUTPUT_SAMPLE_RATE,        RIB_NONE },
            { RIB_ATTRIBUTE,  "Output sample width",       (U32) &ASI->OUTPUT_BITS,               RIB_NONE },
            { RIB_ATTRIBUTE,  "Output channels",           (U32) &ASI->OUTPUT_CHANNELS,           RIB_NONE },
            { RIB_ATTRIBUTE,  "Position",                  (U32) &ASI->POSITION,                  RIB_NONE },
            { RIB_ATTRIBUTE,  "Percent done",              (U32) &ASI->PERCENT_DONE,              RIB_NONE },
            { RIB_ATTRIBUTE,  "Minimum input block size",  (U32) &ASI->MIN_INPUT_BLOCK_SIZE,      RIB_NONE },
            { RIB_PREFERENCE, "Raw source sample rate",    (U32) &ASI->RAW_RATE,                  RIB_NONE },
            { RIB_PREFERENCE, "Raw source sample width",   (U32) &ASI->RAW_BITS,                  RIB_NONE },
            { RIB_PREFERENCE, "Raw source channels",       (U32) &ASI->RAW_CHANNELS,              RIB_NONE },
            { RIB_PREFERENCE, "Requested sample rate",     (U32) &ASI->REQUESTED_RATE,            RIB_NONE },
            { RIB_PREFERENCE, "Requested sample width",    (U32) &ASI->REQUESTED_BITS,            RIB_NONE },
            { RIB_PREFERENCE, "Requested channels",        (U32) &ASI->REQUESTED_CHANS,           RIB_NONE }
            };

         RIB_request(DP->provider,"ASI stream",ASISTR);

         //
         // Open stream with codec, registering callback function
         //

         OutMilesMutex();
         ASI->stream = ASI->ASI_stream_open((U32) S,
                                                  DP_ASI_DECODER_callback,
                                                  S->len[0]);
         InMilesMutex();

         // jkr - exit on error
         if (ASI->stream==0)
         {
           DP->provider=0;
           return(previous);
         }

         //
         // Request codec output format which matches hardware format
         //

         ASI->ASI_stream_set_preference(ASI->stream,
                                        ASI->REQUESTED_RATE,
                                        &S->driver->DMA_rate);

         U32 achans=1 + ((S->driver->hw_format & DIG_F_STEREO_MASK) != 0);

         ASI->ASI_stream_set_preference(ASI->stream,
                                        ASI->REQUESTED_CHANS,
                                        &achans);

         U32 abits=8 + (8 * ((S->driver->hw_format & DIG_F_16BITS_MASK) != 0));

         ASI->ASI_stream_set_preference(ASI->stream,
                                        ASI->REQUESTED_BITS,
                                        &abits);

         //
         // Configure sample type and rate according to codec's actual output
         // format
         //

         U32 nch  = ASI->ASI_stream_attribute(ASI->stream, ASI->OUTPUT_CHANNELS);
         U32 rate = ASI->ASI_stream_attribute(ASI->stream, ASI->OUTPUT_SAMPLE_RATE);
         U32 bits = ASI->ASI_stream_attribute(ASI->stream, ASI->OUTPUT_BITS);

         AIL_set_sample_playback_rate(S, rate);

         if (nch == 2)
            {
            AIL_set_sample_type(S,
                               (bits == 16) ? DIG_F_STEREO_16 : DIG_F_STEREO_8,
                               (bits == 16) ? DIG_PCM_SIGN    : 0);
            }
         else
            {
            AIL_set_sample_type(S,
                               (bits == 16) ? DIG_F_MONO_16   : DIG_F_MONO_8,
                               (bits == 16) ? DIG_PCM_SIGN    : 0);
            }

#if 0
         AIL_debug_printf("SSP: %d channels, %d Hz, %d bits\n",nch,rate,bits);
#endif

         //
         // Indicate installation of decoder module
         //

         DP->active = 1;
         break;
         }
      }

   //
   // Return success
   //

   return previous;
}


//############################################################################
//##                                                                        ##
//## Set a RIB provider for a specified driver pipeline phase               ##
//##                                                                        ##
//############################################################################

HPROVIDER    AILCALL AIL_API_set_digital_driver_processor(HDIGDRIVER  dig, //)
                                                          DIGDRVSTAGE pipeline_stage,
                                                          HPROVIDER   provider)
{
   DOLOCK();

   if ( dig == 0 )
     return( 0 );

   S32 stage = pipeline_stage;

   if (stage == DIGDRV_ALL_STAGES)
      {
      stage = 0;
      }

   DPINFO FAR *DP;

   //
   // handle the "default" pipeline stages specially
   //

   if ((pipeline_stage==DP_DEFAULT_FILTER) || (pipeline_stage==DP_DEFAULT_FILTER))
      {
      HPROVIDER previous=dig->pipeline[pipeline_stage].provider;
      dig->pipeline[pipeline_stage].provider=provider;
      return(previous);
      }

   do
      {
      DP = &dig->pipeline[stage];

      //
      // If existing stage is being replaced, shut it down first
      //

      if (DP->active)
         {
         switch (stage)
            {
            //
            // "MSS mixer services"
            //

            case DP_FLUSH:
            case DP_COPY:
               break;
            }

         DP->active = 0;
         }
      }
   while ((pipeline_stage == DIGDRV_ALL_STAGES) && (++stage < N_DIGDRV_STAGES));

   //
   // If no new provider specified, exit normally after shutting down
   // existing provider
   //

   if (!provider)
      {
      return 0;
      }

   //
   // Set up to return previous provider
   //

   HPROVIDER previous = DP->provider;

   DP->provider = provider;

   //
   // Query required attributes from provider interface
   //

   switch (stage)
      {
      //
      // "MSS mixer services"
      //

      case DP_FLUSH:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         MIXSTAGE FAR *MIX = &DP->TYPE.MIX;

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_flush", (U32) &MIX->MSS_mixer_flush, RIB_NONE },
            };

         RIB_request(DP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         DP->active = 1;
         break;
         }

      case DP_COPY:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         MIXSTAGE FAR *MIX = &DP->TYPE.MIX;

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_copy",  (U32) &MIX->MSS_mixer_copy,  RIB_NONE }
            };

         RIB_request(DP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         DP->active = 1;
         break;
         }
      }

   //
   // Return success
   //

   return previous;
}

//############################################################################
//##                                                                        ##
//## Install end-of-file callback handler                                   ##
//##                                                                        ##
//############################################################################

AILSAMPLECB AILCALL AIL_API_register_EOF_callback(HSAMPLE S, AILSAMPLECB EOFILE)
{
   AILSAMPLECB old;

   DOLOCK();

   if (S == NULL)
      {
      return NULL;
      }

   old = (AILSAMPLECB) S->system_data[SSD_EOD_CALLBACK];

   S->system_data[SSD_EOD_CALLBACK] = (U32) EOFILE;

   return old;
}

S32 AILCALL AIL_API_digital_CPU_percent(HDIGDRIVER dig)
{
  S32 time;
  U32 diff;

  if (dig==0)
    return(0);

  time=AIL_ms_count();

  diff=time-dig->last_ms_polled;
  if (diff<150)
    return(dig->last_percent);

  dig->last_ms_polled=time;

  dig->ms_count+=(dig->us_count/1000);
  dig->us_count=dig->us_count%1000;

  time=(diff)?((dig->ms_count*100)/diff):0;
  dig->ms_count=0;

  dig->last_percent=time;

  if (time>100)
    time=100;

  return(time);
}

#define min(a,b)  (((a) < (b)) ? (a) : (b))

static S32 get_operation(U32 dest_rate,U32 dest_format,U32 src_rate,U32 src_bits, U32 src_chans)
{
  //
  // Set mixer operation code
  //

  S32 op = 0;

  if (dest_format & DIG_F_STEREO_MASK)
     {
     op |= M_DEST_STEREO;
     }

  if (src_chans==2)
     {
     op |= M_SRC_STEREO;
     }

  if (src_bits!=8)
     {
     op |= M_SRC_16;
     }

  //
  // Set M_SRC_SCALE
  //
  // Force M_SRC_SCALE if stereo source mixed into mono output
  //
  // (This routine assumes no volume scaling in use)
  //

  if ((op & M_SRC_STEREO) && (!(op & M_DEST_STEREO)))
     {
     op |= M_VOL_SCALING;
     }

  if (src_rate != dest_rate)
     {
     op |= M_RESAMPLE;
     }

  //
  // Enable filtering if preference set
  //

  if ((AIL_get_preference(DIG_ENABLE_RESAMPLE_FILTER)) && ((src_rate) != dest_rate))
     {
     op |= M_FILTER;
     }

  return(op);
}

//############################################################################
//##                                                                        ##
//## Return how much memory a conversion will require                       ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_size_processed_digital_audio(
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO const FAR* src)
{
   if ((src==0) || (num_srcs==0))
     return(0);

   // a point is one sample in mono, or two samples in stereo -
   // it is one sample point in time

   S32 i;
   S32 max_src_points=0;

   //calculate the sample points for each input
   for(i=0;i<num_srcs;i++) {
     S32 points=src[i].Info.data_len;
     if (src[i].Info.format==WAVE_FORMAT_IMA_ADPCM)
       points<<=1;    //2 adpcm samples in a byte
     else
       if (src[i].Info.bits!=8)
         points>>=1;  //2 bytes in a 16-bit sample
     if (src[i].Info.channels==2)
       points>>=1;    //2 samples in a stereo point

     // adjust number of points for sample rate differences
     points=(S32)(((F64)points*(F64)dest_rate)/((F64)src[i].Info.rate));

     // keep track of the greatest number of points
     if (points>max_src_points)
       max_src_points=points;
   }

   // figure number of dest points
   S32 dest_point_size=((dest_format&DIG_F_STEREO_MASK)?2:1)*((dest_format&DIG_F_16BITS_MASK)?2:1);

   return( (dest_point_size*max_src_points)+256 );  // 256 for slop

}

//############################################################################
//##                                                                        ##
//## Ask MSS to reset the ASI at the end of the specified buffer            ##
//##                                                                        ##
//############################################################################

DXDEC  void     AILCALL AIL_request_EOB_ASI_reset   (HSAMPLE S,
                                                     U32     buff_num)
{
  if (S)
  {
    S->reset_ASI[buff_num]=1;
  }
}


//############################################################################
//##                                                                        ##
//## Convert data from one sample rate and format to another                ##
//##                                                                        ##
//############################################################################

#define NUMBUILDSAMPLES 2048

S32 AILCALL AIL_API_process_digital_audio(
                                 void FAR       *dest_buffer,
                                 S32             dest_buffer_size,
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO FAR* src)
{

   DOLOCK();
   U32 nbytes_written=0;

   if ((src==0) || (dest_buffer==0) || (num_srcs==0))
     return(0);

   S32 adpcm_size=0;
   S16 FAR*adpcm_temp_buffer=0;

   S32 dest_chan=((dest_format&DIG_F_STEREO_MASK)?2:1);

   //
   // Get driver mixer providers
   //

   MIXER_FLUSH MIXER_flush;
   MIXER_MERGE MIXER_merge;
   MIXER_COPY  MIXER_copy;

   HPROVIDER HP;


   RIB_enumerate_providers("MSS mixer services",
                            NULL,
                           &HP);

   RIB_INTERFACE_ENTRY MIXER[] =
      {
      FN(MIXER_merge),
      FN(MIXER_flush),
      FN(MIXER_copy)
      };

   RIB_request(HP,"MSS mixer services",MIXER);


   S32 i;

   //calculate all of the operations
   S32 operations[256];
   for(i=0;i<num_srcs;i++)
     operations[i]=get_operation(dest_rate,dest_format,src[i].Info.rate,src[i].Info.bits,src[i].Info.channels);

  keepconverting:

   // a point is one sample in mono, or two samples in stereo -
   // it is one sample point in time

   S32 max_src_points=0;

   //calculate the sample points for each input
   for(i=0;i<num_srcs;i++) {
     S32 points=src[i].Info.data_len;
     if (src[i].Info.format==WAVE_FORMAT_IMA_ADPCM)
       points<<=1; //2 adpcm samples in a byte
     else
       if (src[i].Info.bits!=8)
         points>>=1;  //2 bytes in a 16-bit sample
     if (src[i].Info.channels==2)
       points>>=1;  //2 samples in a stereo point

     // adjust number of points for sample rate differences
     points=(S32)(((F64)points*(F64)dest_rate)/((F64)src[i].Info.rate));

     // keep track of the greatest number of points
     if (points>max_src_points)
       max_src_points=points;
   }

   // figure number of dest points
   S32 dest_point_size=dest_chan*((dest_format&DIG_F_16BITS_MASK)?2:1);
   S32 dest_points=dest_buffer_size/dest_point_size;

   //limit our outputs to the maximum number of inputs
   if (max_src_points<dest_points)
     dest_points=max_src_points;

   //just exit if we don't have any mixing to do
   if (dest_points==0)
     return(nbytes_written);


   // Init persistent variables for all passes
   for(i=0;i<num_srcs;i++) {
     if ((src[i].mss_adpcm.blocksize!=src[i].Info.block_size) || (src[i].Info.initial_ptr==src[i].Info.data_ptr)) {
       src[i].mss_adpcm.blocksize    = src[i].Info.block_size;
       src[i].mss_adpcm.blockleft    = 0;
       src[i].mss_adpcm.extrasamples = 0;
       src[i].src_fract=0;
       src[i].left_val=0;
       src[i].right_val=0;
     }
   }

   //ok, get down to the mixing
   U8 FAR* dest=(U8 FAR*)dest_buffer;

   S32 build_buffer[NUMBUILDSAMPLES];
   S32 build_points=NUMBUILDSAMPLES/dest_chan;

   while (dest_points) {

     //figure out how many points we're going to do this pass
     S32 points=build_points;
     if (points>dest_points)
       points=dest_points;

     S32 build_size=points*sizeof(S32)*dest_chan;

     //clear the output buffer
     MIXER_flush( build_buffer,
                  build_size
#ifdef IS_X86
                  ,AIL_get_preference(AIL_ENABLE_MMX_SUPPORT)
#endif
                  );

     S32 max_built_size=0;

     //now loop through the srcs mixing in a piece of each src's buffer
     for(i=0;i<num_srcs;i++) {

       //do we have anything to mix?
       if (src[i].Info.data_len==0)
         continue;

       void const FAR* mix_ptr;
       S32 mix_size;

       // if we're dealing with ADPCM, we have to decompress into a temp buffer first
       if (src[i].Info.format==WAVE_FORMAT_IMA_ADPCM) {

         void const FAR* src_orig=src[i].Info.data_ptr;

         mix_size=2*((S32)(((((F64)points)*((F64)src[i].Info.rate))/(F64)dest_rate)+0.5F))*src[i].Info.channels;

         //make sure the buffer is big enough
         if (mix_size>adpcm_size) {
           if (adpcm_temp_buffer)
             AIL_mem_free_lock(adpcm_temp_buffer);
           adpcm_size=mix_size;
           adpcm_temp_buffer=(S16 FAR*)AIL_mem_alloc_lock(adpcm_size);
           if (adpcm_temp_buffer==0)
             return(0);
         }

         S16 FAR* temp=adpcm_temp_buffer;

         if (src[i].Info.channels==2)
            {
            DecodeADPCM_STEREO(&temp, &src[i].Info.data_ptr, mix_size, src[i].Info.data_len, &src[i].mss_adpcm);
            }
         else
            {
            DecodeADPCM_MONO(&temp, &src[i].Info.data_ptr, mix_size, src[i].Info.data_len, &src[i].mss_adpcm);
            }

         U32 src_moved=AIL_ptr_dif(src[i].Info.data_ptr,src_orig);
         src[i].Info.data_len=(src[i].Info.data_len<=src_moved)?0:(src[i].Info.data_len-src_moved);

         mix_ptr=adpcm_temp_buffer;

         //don't worry about the fractional bit with adpcm
         src[i].src_fract=0;

       } else {

         mix_ptr=src[i].Info.data_ptr;
         mix_size=src[i].Info.data_len;

       }


       //
       // Call mixer provider
       //

       #ifdef IS_32

         void const FAR* mix_orig=mix_ptr;
         S32 FAR* build=build_buffer;

         InMilesMutex();

         MIXER_merge(&mix_ptr,
                     &src[i].src_fract,
                      AIL_ptr_add( mix_ptr, mix_size),
                     &build,
                      AIL_ptr_add( build, build_size),
                     &src[i].left_val,
                     &src[i].right_val,
                      (S32)((((F64) src[i].Info.rate) * 65536.0F) / (F64) dest_rate),
                      2048,
                      2048,
                      operations[i]
#ifdef IS_X86
                      ,AIL_get_preference(AIL_ENABLE_MMX_SUPPORT)
#endif
                      );

         OutMilesMutex();

         S32 built_size=AIL_ptr_dif(build,build_buffer);
         U32 src_moved=AIL_ptr_dif(mix_ptr,mix_orig);

       #else

         U32 src_offset  = LOWORD(mix_ptr);
         U32 dest_offset = LOWORD(build_buffer);

         MIXER_merge(HIWORD(mix_ptr),
                     HIWORD(build_buffer),
                    &src[i].src_fract,
                    &src_offset,
                    &dest_offset,
                     src_offset+mix_size,
                     dest_offset+build_size,
                    &src[i].left_val,
                    &src[i].right_val,
                     (S32)((((F64) src[i].Info.rate) * 65536.0F) / (F64) dest_rate),
                     (2048 << 16) | 2048,
                     operations[i]);

         U32 src_moved=(src_offset - LOWORD(mix_ptr));
         S32 built_size=(dest_offset - LOWORD(build_buffer));

       #endif

       // don't adjust the pointers for adpcm (already done)
       if (!(src[i].Info.format==WAVE_FORMAT_IMA_ADPCM)) {
         src[i].Info.data_ptr = AIL_ptr_add(src[i].Info.data_ptr, src_moved);
         src[i].Info.data_len=(src[i].Info.data_len<=src_moved)?0:(src[i].Info.data_len-src_moved);
       }

       if (built_size>max_built_size)
         max_built_size=built_size;

     }

     if (max_built_size==0)
       break;

     //copy from the build buffer into dest
     MIXER_copy( build_buffer,
                 max_built_size,
                 dest,
                 dest_format,
#ifdef IS_X86
                 AIL_get_preference(AIL_ENABLE_MMX_SUPPORT)
#else
                 0
#endif
                 );

     dest=(U8 FAR*)AIL_ptr_add(dest,points*dest_point_size);
     dest_points-=points;
   }

   S32 written=AIL_ptr_dif(dest,dest_buffer);
   nbytes_written+=written;
   dest_buffer=dest;
   dest_buffer_size-=written;

   if (dest_buffer_size)
     goto keepconverting;

   if (adpcm_temp_buffer)
     AIL_mem_free_lock(adpcm_temp_buffer);

   return(nbytes_written);
}

//
// ASCII alternate redist directory
//

char AIL_redist_directory[260]="";

//############################################################################
//##                                                                        ##
//## Set the alternate MSS driver directory                                 ##
//##                                                                        ##
//############################################################################

extern "C"
char FAR*  AILEXPORT AIL_set_redist_directory(char const FAR* dir)
{
   S32 len;

   if ( ( dir == NULL ) || ( ( dir[ 0 ] == '.' ) && ( dir[ 1 ] == 0 ) ) )
      *AIL_redist_directory=0;
   else
      {
#ifdef IS_MAC
      AIL_strcpy(AIL_redist_directory,dir);

      len=AIL_strlen(AIL_redist_directory);
      if (len)
      {
        if (AIL_redist_directory[len-1]!=':')
        {
          AIL_redist_directory[len]=':';
          AIL_redist_directory[len+1]=0;
        }
      }
#else
      AIL_strcpy(AIL_redist_directory,dir);
      len=AIL_strlen(AIL_redist_directory);
      if (len)
         {
         if ((AIL_redist_directory[len-1]!='/') && (AIL_redist_directory[len-1]!='\\') && (AIL_redist_directory[len-1]!=':'))
            {
            AIL_redist_directory[len]='/';
            AIL_redist_directory[len+1]=0;
            }
         }
#endif
      }

   return (char FAR*) (AIL_redist_directory);
}


//############################################################################
//##                                                                        ##
//## Return number of actively playing samples for given driver             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_active_sample_count( HDIGDRIVER dig )
{
#ifdef IS_XBOX
  return( dig->n_active_samples );
#else
  S32 i,n;

  if ( dig == 0 )
  {
    return( 0 );
  }

  n = 0;

  for ( i = 0 ; i < dig->n_samples ; i++ )
  {
    if ( ( dig->samples[ i ].status == SMP_PLAYING ) || ( dig->samples[ i ].status == SMP_PLAYINGBUTRELEASED ) )
    {
      ++n;
    }
  }

  return( n );
#endif
}

//############################################################################
//##                                                                        ##
//## Allocate a SAMPLE structure for use with a given driver                ##
//##                                                                        ##
//############################################################################

HSAMPLE AILCALL AIL_API_allocate_sample_handle( HDIGDRIVER dig )
{
  S32    i;
  HSAMPLE S;

  //
  // Exit if driver invalid
  //

  if ( dig == NULL )
  {
    return( 0 );
  }

  //
  // Lock timer services to prevent reentry
  //

  AIL_lock();

  //
  // Look for an unallocated sample structure
  //

  for ( i = 0 ; i < dig->n_samples ; i++ )
  {
    if ( dig->samples[ i ].status == SMP_FREE )
      break;
  }

  //
  // If all structures in use, return NULL
  //

  if ( i == dig->n_samples )
  {
    AIL_set_error( "Out of sample handles." );

    AIL_unlock();
    return( 0 );
  }

  S = &dig->samples[ i ];

  //
  // Initialize sample to default (SMP_DONE) status with nominal
  // sample attributes
  //

  AIL_init_sample( S );

  AIL_unlock();
  return( S );
}

//############################################################################
//##                                                                        ##
//## Free a SAMPLE structure for later allocation                           ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_release_sample_handle( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  hardware_stop( S );

  S->status = SMP_FREE;

  //
  // Shut down any installed pipeline providers
  //

  AIL_set_sample_processor( S,
                            SAMPLE_ALL_STAGES,
                            0 );

  hardware_spin_until_stopped( S );

  #ifdef IS_XBOX
  if ( S->ds_staging_buffer )
  {
    S->ds_staging_buffer->next = S->driver->staging_buffer_ring;
    S->driver->staging_buffer_ring = S->ds_staging_buffer;
    S->ds_staging_buffer = 0;
  }
  #endif
}

//############################################################################
//##                                                                        ##
//## Initialize a SAMPLE structure to baseline values                       ##
//##                                                                        ##
//## Sample is allocated (not free), done playing, and stopped              ##
//##                                                                        ##
//## By default, sample source data is assumed to be 11.025 kHz 8-bit mono  ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_init_sample( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  hardware_stop( S );

  //
  // Set status to FREE while manipulating vars, to keep callback thread
  // from reading invalid data
  //

  S->status = SMP_FREE;

  //
  // Shut down any previously-active pipeline providers
  //   and init pipeline stages
  //

  AIL_set_sample_processor( S,
                            SAMPLE_ALL_STAGES,
                            0 );

  //
  // Initialize sample vars
  //

  S->start[ 0 ]     =  NULL;
  S->len  [ 0 ]     =  0;
  S->pos  [ 0 ]     =  0;
  S->done [ 0 ]     =  0;
  S->reset_ASI[ 0 ] =  0;

  S->start[ 1 ]     =  NULL;
  S->len  [ 1 ]     =  0;
  S->pos  [ 1 ]     =  0;
  S->done [ 1 ]     =  1;
  S->reset_ASI[ 1 ] =  0;

  clear_last_sample(S);

  S->current_buffer =  0;
  S->last_buffer    = -2;
  S->starved        =  1;

  S->loop_count     =  1;
  S->loop_start     =  0;
  S->loop_end       = -1;

  #ifndef IS_XBOX
  S->format         =  DIG_F_MONO_8;
  S->flags          =  0;
  S->playback_rate  =  11025;
  #endif

  //
  // Mark as single-buffered sample
  //
  
  S->service_type = 1;

  // 50% of the default volume to each speaker
  AIL_API_set_sample_volume_pan( S, 1.0f, 0.5f );

  S->dry_level = 1.0F;
  S->wet_level = 0.0F;

  S->lp.on = 0;

  // Set default ADPCM data
  S->adpcm.blocksize    =  256;
  S->adpcm.blockleft    = 0;
  S->adpcm.extrasamples = 0;

  S->SOB =  NULL;
  S->EOB =  NULL;
  S->EOS =  NULL;
  S->dosob = 0;
  S->doeob = 0;
  S->doeos = 0;

  //
  // Init pipeline stages
  //

  for (S32 i = 0 ; i < N_SAMPLE_STAGES ; i++ )
  {
    S->pipeline[ i ].active = 0;
  }

  //
  // Select default mixer merge provider
  //

  AIL_set_sample_processor( S,
                            DP_MERGE,
                            S->driver->pipeline[ DP_DEFAULT_MERGE ].provider );

  AIL_set_sample_processor( S,
                            DP_FILTER,
                            S->driver->pipeline[ DP_DEFAULT_FILTER ].provider );

  //
  // Mark sample initialized
  //

  S->status =  SMP_DONE;

  #ifdef IS_XBOX
  S->direct_mode = 1;

  hardware_spin_until_stopped( S );
  hardware_3d_init( S );

  #endif
}

//############################################################################
//##                                                                        ##
//## Get status of sample                                                   ##
//##                                                                        ##
//############################################################################

U32 AILCALL AIL_API_sample_status( HSAMPLE S )
{
  if ( S == NULL )
  {
    return( 0 );
  }

  return( S->status );
}

//############################################################################
//##                                                                        ##
//## Set adpcm sample block size                                            ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_adpcm_block_size( HSAMPLE S, U32 blocksize )
{
  if ( S )
  {
    S->adpcm.blocksize = blocksize;
  }
}

//############################################################################
//##                                                                        ##
//## Set starting address and length of sample                              ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_address( HSAMPLE S, void const FAR *start, U32 len )
{
  if ( S == NULL )
  {
    return;
  }

  S->start[ 0 ] = start;
  S->len[ 0 ]   = len;

  S->start[ 1 ] = NULL;
  S->len[ 1 ]   = 0;

  //
  // Mark as single-buffered sample
  //
  
  S->service_type = 1;
}

//############################################################################
//##                                                                        ##
//## Set sample data format and flags                                       ##
//##                                                                        ##
//## Available formats:                                                     ##
//##                                                                        ##
//##   DIG_F_MONO_8                                                         ##
//##   DIG_F_MONO_16                                                        ##
//##   DIG_F_STEREO_8                                                       ##
//##   DIG_F_STEREO_16                                                      ##
//##   DIG_F_ADPCM_MONO_16                                                  ##
//##   DIG_F_ADPCM_STEREO_16                                                ##
//##                                                                        ##
//## Available flags:                                                       ##
//##                                                                        ##
//##   DIG_PCM_SIGN                                                         ##
//##   DIG_PCM_ORDER (stereo formats only)                                  ##
//##                                                                        ##
//## Note: 16-bit sample data must be in Intel byte order                   ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_type( HSAMPLE S, S32 format, U32 flags )
{
  if ( S == NULL )
  {
    return;
  }

  if ( ( format != S->format ) ||
       ( flags  != S->flags ) )
  {
    S->format = format;
    S->flags  = flags;

    // Reset ADPCM offset to the end of decode buffer
    // to force a decode buffer refill
    if ( format & DIG_F_ADPCM_MASK )
    {
      S->adpcm.blockleft = 0;
      S->adpcm.extrasamples = 0;
    }
  }
}

//############################################################################
//##                                                                        ##
//## Get sample playback rate in hertz                                      ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_playback_rate( HSAMPLE S )
{
  if ( S == NULL)
  {
    return 0;
  }

  return( S->playback_rate );
}

//############################################################################
//##                                                                        ##
//## Set sample playback rate in hertz                                      ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_playback_rate( HSAMPLE S, S32 playback_rate )
{
  if ( S )
  {
    if ( playback_rate > 0 )
    {
      S->playback_rate = playback_rate;
      set_hardware_rate( S );
    }
  }
}

//############################################################################
//##                                                                        ##
//## Get sample volume levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_volume_levels( HSAMPLE S, F32 * left_level, F32 * right_level )
{
  if (S == NULL)
  {
    return;
  }

  if ( left_level )
  {
    *left_level = S->left_volume;
  }
  if ( right_level )
  {
    *right_level = S->right_volume;
  }
}

//############################################################################
//##                                                                        ##
//## Set sample volume levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_volume_levels( HSAMPLE S, F32 left_level, F32 right_level )
{
  if ( S == NULL )
  {
    return;
  }

  S->left_volume = left_level;
  S->right_volume = right_level;

  // build some reasonable values for reporting
  if ( left_level > 0.0001f )
  {
    F32 ratio = (F32) pow( right_level / left_level, 10.0f / 3.0f );
    S->save_pan = ratio / ( ratio + 1 );
    if ( S->save_pan < 0.0001f )
      S->save_volume = left_level;
    else
      S->save_volume = right_level * (F32) pow( S->save_pan, -0.3f );
  }
  else
  {
    if ( right_level > 0.0001f )
    {
      S->save_pan = 1.0f;
      S->save_volume = right_level;
    }
    else
    {
      S->save_pan = 0.5f;
      S->save_volume = 0.0f;
    }
  }

  #ifdef IS_XBOX
  S->leftb_volume = S->left_volume;
  S->rightb_volume = S->rightb_volume;
  S->center_volume = S->save_volume;
  S->low_volume = S->save_volume;
  S->save_fb_pan = 0.5f;
  #endif

  set_hardware_volume( S );
}

//############################################################################
//##                                                                        ##
//## Get sample reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_reverb_levels( HSAMPLE S, F32 * dry_level, F32 * wet_level )
{
  if ( S == NULL )
  {
    if ( dry_level )
    {
      *dry_level = 0.0;
    }
    if ( wet_level )
    {
      *wet_level = 0.0;
    }
    return;
  }

  if ( dry_level )
  {
    *dry_level = S->dry_level;
  }
  if ( wet_level )
  {
    *wet_level = S->wet_level;
  }
}

//############################################################################
//##                                                                        ##
//## Set sample reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_reverb_levels( HSAMPLE S, F32 dry_level, F32 wet_level )
{
  if ( S == NULL )
  {
    return;
  }

  S->dry_level = dry_level;
  S->wet_level = wet_level;

  if ( ( S->status == SMP_PLAYING) && ( S->wet_level > 0.0001f ) && ( S->driver->master_wet > 0.0001f ) && ( S->driver->reverb_time > 0.0001f ) )
  {
    S->driver->reverb_off_time = AIL_ms_count() + S->driver->reverb_duration + 100;
    S->driver->reverb_on = 1;
  }

  set_hardware_volume( S );
}


//############################################################################
//##                                                                        ##
//## Set sample volume and panpot / stereo balance (0=left, 1=right)        ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_volume_pan( HSAMPLE S, F32 volume, F32 pan )
{
  if ( S == NULL )
  {
    return;
  }

  S->save_volume = (F32) pow( volume, 10.0f / 6.0f );
  S->save_pan = pan;

  if ( pan == 0.5f )
  {
    S->left_volume = S->save_volume * 0.812252196f;
    S->right_volume = S->save_volume * 0.812252196f;
  }
  else
  {
    S->left_volume = S->save_volume * (F32) pow( 1.0f - pan, 0.3f );
    S->right_volume = S->save_volume * (F32) pow( pan, 0.3f );
  }

  #ifdef IS_XBOX
    // 0.812252196f is the front/back perfectly balance pan
  S->left_volume = S->left_volume * 0.812252196f;
  S->right_volume = S->right_volume * 0.812252196f;

  S->leftb_volume = S->left_volume;
  S->rightb_volume = S->rightb_volume;
  S->center_volume = S->save_volume;
  S->low_volume = S->save_volume;

  S->save_fb_pan = 0.5f;
  S->save_center = 1.0f;
  S->save_low = 1.0f;
  #endif

  set_hardware_volume( S );
}


//############################################################################
//##                                                                        ##
//## Get sample volume and panpot / stereo balance (0=left, 1=right)        ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_volume_pan( HSAMPLE S, F32 FAR * volume, F32 FAR * pan )
{
  if ( S == NULL )
  {
    return;
  }

  if (volume)
  {
    *volume = (F32) pow( S->save_volume, 6.0f / 10.0f );
  }

  if (pan)
  {
    *pan = S->save_pan;
  }
}

#ifdef IS_XBOX

//############################################################################
//##                                                                        ##
//## Set sample 5.1 volume and panpot / stereo balance (0=left, 1=right)    ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_51_volume_pan( HSAMPLE S, F32 volume, F32 pan, F32 fb_pan, F32 center_level, F32 sub_level )
{
  if ( S == NULL )
  {
    return;
  }

  F32 left, right, front, back;

  S->save_volume = (F32) pow( volume, 10.0f / 6.0f );
  S->save_pan = pan;
  S->save_fb_pan = fb_pan;
  S->save_center = center_level;
  S->save_low = sub_level;

  if ( fb_pan == 0.5f )
  {
    front = 0.812252196f;
    back = 0.812252196f;
  }
  else
  {
    front = (F32) pow( 1.0f - fb_pan, 0.3f );
    back = (F32) pow( fb_pan, 0.3f );
  }

  if ( pan != fb_pan )
  {
    if ( pan == 0.5f )
    {
      left = S->save_volume * 0.812252196f;
      right = S->save_volume * 0.812252196f;
    }
    else
    {
      left = S->save_volume * (F32) pow( 1.0f - pan, 0.3f );
      right = S->save_volume * (F32) pow( pan, 0.3f );
    }
  }
  else
  {
    left = S->save_volume * front;
    right = S->save_volume * back;
  }

  S->left_volume = left * front;
  S->right_volume = right * front;

  S->leftb_volume = left * back;
  S->rightb_volume = right * back;

  S->center_volume = S->save_volume * center_level;
  S->low_volume = S->save_volume * sub_level;

  set_hardware_volume( S );
}


//############################################################################
//##                                                                        ##
//## Get sample 5.1 volume and panpot / stereo balance (0=left, 1=right)    ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_51_volume_pan( HSAMPLE S, F32 FAR * volume, F32 FAR * lr_pan, F32 FAR * fb_pan, F32 FAR * center_level, F32 FAR * sub_level )
{
  if ( S == NULL )
  {
    return;
  }

  if ( volume )
  {
    *volume = (F32) pow( S->save_volume, 6.0f / 10.0f );
  }

  if ( lr_pan )
  {
    *lr_pan = S->save_pan;
  }

  if ( fb_pan )
  {
    *fb_pan = S->save_fb_pan;
  }

  if ( center_level )
  {
    *center_level = S->save_center;
  }

  if ( sub_level )
  {
    *sub_level = S->save_low;
  }
}

//############################################################################
//##                                                                        ##
//## Set sample volume levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_51_volume_levels( HSAMPLE S, F32 f_left_level, F32 f_right_level, F32 b_left_level, F32 b_right_level, F32 center_level, F32 sub_level )
{
  if ( S == NULL )
  {
    return;
  }

  S->left_volume = f_left_level;
  S->right_volume = f_right_level;

  S->leftb_volume = b_left_level;
  S->rightb_volume = b_right_level;
  S->center_volume = center_level;
  S->low_volume = sub_level;

  // build some reasonable values for reporting
  if ( f_left_level > 0.0001f )
  {
    F32 ratio = (F32) pow( f_right_level / f_left_level, 10.0f / 3.0f );
    S->save_pan = ratio / ( ratio + 1 );

    ratio = (F32) pow( b_left_level / f_left_level, 10.0f / 3.0f );
    S->save_fb_pan = ratio / ( ratio + 1 );

    S->save_volume = ( S->save_pan > 0.0001f ) ?
                       ( ( S->save_fb_pan > 0.0001f ) ?
                         ( b_right_level * (F32) pow( S->save_pan * S->save_fb_pan, -0.3f ) ) :
                         ( f_right_level * (F32) pow( S->save_pan, -0.3f ) )
                        ) :
                        ( ( S->save_fb_pan > 0.0001f ) ?
                          ( b_left_level * (F32) pow( S->save_fb_pan, -0.3f ) ) :
                          f_left_level
                        );
  }
  else
  {
    if ( b_left_level > 0.0001f )
    {
      F32 ratio = (F32) pow( b_right_level / b_left_level, 10.0f / 3.0f );
      S->save_pan = ratio / ( ratio + 1 );

      ratio = (F32) pow( f_left_level / b_left_level, 10.0f / 3.0f );
      S->save_fb_pan = 1.0f - ( ratio / ( ratio + 1 ) );

      S->save_volume = ( S->save_pan > 0.0001f ) ?
                         ( ( S->save_fb_pan > 0.0001f ) ?
                           ( b_right_level * (F32) pow( S->save_pan * S->save_fb_pan, -0.3f ) ) :
                           ( f_right_level * (F32) pow( S->save_pan, -0.3f ) )
                          ) :
                          ( ( S->save_fb_pan > 0.0001f ) ?
                            ( b_left_level * (F32) pow( S->save_fb_pan, -0.3f ) ) :
                            f_left_level
                          );
    }
    else
    {
      if ( f_right_level > 0.0001f )
      {
        F32 ratio = (F32) pow( b_right_level / f_right_level, 10.0f / 3.0f );
        S->save_fb_pan = ratio / ( ratio + 1 );
        S->save_pan = 1.0f;

        S->save_volume = ( S->save_fb_pan > 0.0001f ) ?
                         ( b_right_level * (F32) pow( S->save_fb_pan, -0.3f ) ) :
                         f_right_level;
      }
      else
      {
        if ( b_right_level > 0.0001f )
        {
          S->save_pan = 1.0f;
          S->save_fb_pan = 1.0f;
          S->save_volume = b_right_level;
        }
        else
        {
          S->save_pan = 0.5f;
          S->save_fb_pan = 0.5f;
          S->save_volume = 0;
        }
      }
    }
  }

  if ( S->save_volume > 0.0001f )
  {
    S->save_center = center_level / S->save_volume;
    S->save_low = sub_level / S->save_volume;
  }
  else
  {
    S->save_center = 0.0f;
    S->save_low = 0.0f;
  }

  set_hardware_volume( S );
}

//############################################################################
//##                                                                        ##
//## Get sample 5.1 volume and panpot / stereo balance (0=left, 1=right)    ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_51_volume_levels( HSAMPLE S, F32 FAR * f_left_level, F32 FAR * f_right_level, F32 FAR * b_left_level, F32 FAR * b_right_level, F32 FAR * center_level, F32 FAR * sub_level )
{
  if ( S == NULL )
  {
    return;
  }

  if ( f_left_level )
  {
    *f_left_level = S->left_volume;
  }

  if ( f_right_level )
  {
    *f_right_level = S->right_volume;
  }

  if ( b_left_level )
  {
    *b_left_level = S->leftb_volume;
  }

  if ( b_right_level )
  {
    *b_right_level = S->rightb_volume;
  }

  if ( center_level )
  {
    *center_level = S->center_volume;
  }

  if ( sub_level )
  {
    *sub_level = S->low_volume;
  }
}

#endif

//############################################################################
//##                                                                        ##
//## Return # of bytes per sample                                           ##
//##                                                                        ##
//############################################################################

U32 SS_granularity( U32 format )
{
  switch ( format )
  {
    case DIG_F_MONO_16:
    case DIG_F_STEREO_8:
      return( 2 );

    case DIG_F_STEREO_16:
      return( 4 );

    default:
      return( 1 );
  }
}

//############################################################################
//##                                                                        ##
//## Get the granularity of sample type (1=8/m,2=8/s,16/m,4=16/m, or adpcm) ##
//##                                                                        ##
//## Value is returned in bytes                                             ##
//##                                                                        ##
//############################################################################

U32 AILCALL AIL_API_sample_granularity( HSAMPLE S )
{
  if ( S == NULL )
  {
    return( 0 );
  }

  if ( S->pipeline[ DP_ASI_DECODER ].active )
  {
    //
    // All ASI compressed formats are assumed to have 1-byte granularity --
    // the ASI layer will handle any granularity issues associated with
    // seeks, etc. internally
    //

    return( 1 );
  }

#ifdef IS_XBOX
  if ( S->format & DIG_F_XBOX_ADPCM_MASK )
  {
    return( 36 * ( ( S->format & DIG_F_STEREO_MASK ) ? 2 : 1 ) );
  }
#endif

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    return( S->adpcm.blocksize );
  }

  return( SS_granularity( S->format ) );
}

//############################################################################
//##                                                                        ##
//## Get position of next sample block to be transferred                    ##
//##                                                                        ##
//## Value is returned in bytes relative to start of sample, and may be     ##
//## treated as the sample's approximate playback position                  ##
//##                                                                        ##
//############################################################################

U32 AILCALL AIL_API_sample_position( HSAMPLE S )
{
  if ( S == NULL )
  {
    return( 0 );
  }

  check_hardware_buffer( S, 1 );

  return( S->pos[ S->current_buffer ] );
}

//############################################################################
//##                                                                        ##
//## Set position of next sample block to be transferred                    ##
//##                                                                        ##
//## Value is specified in bytes relative to start of sample, and will      ##
//## determine the precise location of the next block of data which is      ##
//## fetched from the sample                                                ##
//##                                                                        ##
//## AIL_set_sample_position(0) "rewinds" the sample to its beginning       ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_position( HSAMPLE S, U32 pos )
{
  U32 gran;

  if ( S == NULL )
  {
    return;
  }

  gran = AIL_sample_granularity( S );
  #ifdef IS_XBOX
  gran = ( gran + 3 ) & ~3;
  #endif

  pos = ( ( pos + ( gran / 2 ) ) / gran ) * gran;

  // Reset ADPCM offset to the end of decode buffer
  // to force a decode buffer refill

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    S->adpcm.blockleft = 0;
    S->adpcm.extrasamples = 0;
  }

  //
  // If this is an ASI-compressed format, perform a seek with offset -1
  // to allow the decoder to reinitialize its state
  //
  // We could also implement this by seeking to the specified offset, but
  // we need to change the position here anyway so it will be reported
  // correctly in subsequent AIL_set_sample_position() calls.  The ASI
  // decoder itself does not care about the seek position -- it only needs
  // to be informed when we change it
  //

  if ( S->pipeline[ DP_ASI_DECODER ].active )
  {
    ASISTAGE *ASI = &S->pipeline[ DP_ASI_DECODER ].TYPE.ASI;

    ASI->ASI_stream_seek( ASI->stream, -1 );
  }

  if ( S->status == SMP_PLAYING )
  {
    check_hardware_buffer( S, 0 );
  }

  S->pos[ S->current_buffer ] = pos;

  set_hardware_position( S );
}

//############################################################################
//##                                                                        ##
//## Get number of sample loops remaining                                   ##
//##                                                                        ##
//## 1 indicates that the sample is on its last iteration                   ##
//## 0 indicates that the sample is looping indefinitely                    ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_loop_count( HSAMPLE S )
{
  if ( S == NULL )
  {
    return -1;
  }

  return( S->loop_count );
}

//############################################################################
//##                                                                        ##
//## Set sample loop count                                                  ##
//##                                                                        ##
//##  1: Single iteration, no looping                                       ##
//##  0: Loop indefinitely                                                  ##
//##  n: Play sample n times                                                ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_loop_count(HSAMPLE S, S32 loop_count)
{
  if (S == NULL)
  {
    return;
  }

  S->loop_count = loop_count;
}

//############################################################################
//##                                                                        ##
//## Set optional subblock for looping                                      ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_loop_block( HSAMPLE S, //)
                                            S32     loop_start_offset,
                                            S32     loop_end_offset )
{
  U32 gran;

  if ( S == NULL )
  {
    return;
  }

  gran = AIL_sample_granularity( S );
  #ifdef IS_XBOX
  gran = ( gran + 3 ) & ~3;
  #endif

  S->loop_start = ( ( loop_start_offset + ( gran / 2 ) ) / gran ) * gran;

  if ( loop_end_offset == -1 )
    S->loop_end = -1;
  else
    S->loop_end = ( ( loop_end_offset + ( gran / 2 ) ) / gran ) * gran;

  set_hardware_loop_region( S );
}


//############################################################################
//##                                                                        ##
//## Set master volume for all samples                                      ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_digital_master_volume_level( HDIGDRIVER dig, F32 master_volume )
{
  if ( dig == 0 )
    return;

  dig->master_volume = master_volume;

  set_master_hardware_volume( dig );
}

//############################################################################
//##                                                                        ##
//## Return current master digital volume setting                           ##
//##                                                                        ##
//############################################################################

F32 AILCALL AIL_API_digital_master_volume_level(HDIGDRIVER dig)
{
  if ( dig == 0 )
    return( 0.0f );
    
  return( dig->master_volume );
}


//############################################################################
//##                                                                        ##
//## Get driver reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_digital_master_reverb_levels( HDIGDRIVER dig, F32 FAR* dry_level, F32 FAR * wet_level )
{
  if (dig == NULL)
  {
    return;
  }

  if ( dry_level )
  {
    *dry_level = dig->master_dry;
  }
  if ( wet_level )
  {
    *wet_level = dig->master_wet;
  }
}

//############################################################################
//##                                                                        ##
//## Set driver reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_digital_master_reverb_levels( HDIGDRIVER dig, F32 dry_level, F32 wet_level )
{
  if (dig == NULL)
  {
    return;
  }

  dig->master_dry = dry_level;
  dig->master_wet = wet_level;

#ifndef IS_XBOX

  if ( ( dig->master_wet > 0.0001f ) && ( dig->reverb_time > 0.0001f ) )
  {
    dig->reverb_off_time = AIL_ms_count() + dig->reverb_duration + 100;
    dig->reverb_on = 1;
  }
#endif

  set_master_hardware_volume( dig );
}

#ifndef IS_XBOX

#define REVERB_DELAYS 6

static const F32 ReverbDelayTimes[ REVERB_DELAYS ] = { 0.0297F, 0.0371F, 0.0411F, 0.0437F, 0.005F, 0.0017F };
static const F32 ReverbAllPass[ 2 ] = { 0.0968535f, 0.032924f };

S32 AILCALL AIL_allocate_reverb_buffers( HDIGDRIVER dig )
{
  U32 mem;
  U32 del_sizes[ REVERB_DELAYS ];
  U32 del_sizes_pad[ REVERB_DELAYS ];
  U32 i;
  F32 rate = (F32) dig->DMA_rate;

  // calculate the size of the reverb and predelay buffer sizes

  dig->reverb_build_size = ( dig->build_size / dig->channels_per_sample );

  i = (U32) ( ( ( ( (F32) AIL_preference[DIG_MAX_PREDELAY_MS] ) / 1000.0f ) * rate ) * 4.0f );

  i = ( ( i + dig->reverb_build_size - 1 ) / dig->reverb_build_size );

  dig->reverb_buffer_size = i * dig->reverb_build_size;

  mem = ( dig->reverb_buffer_size + 15 ) & ~15;

  // calculate the sizes for the delay lines

  for( i = 0 ; i < REVERB_DELAYS ; i++ )
  {
    del_sizes[ i ] = ( (U32) ( ( rate * ReverbDelayTimes[ i ] ) /*+ 0.9999f*/ ) ) * 4;
    del_sizes_pad[ i ] = ( del_sizes[ i ] + 15 ) & ~15;
    mem += del_sizes_pad[ i ];
  }

  // now allocate the entire buffer
  dig->reverb_build_buffer = (S32 FAR *) AIL_mem_alloc_lock( mem );

  if ( dig->reverb_build_buffer == 0 )
  {
    return( 0 );
  }

  // clear everything to zeros
  AIL_memset( dig->reverb_build_buffer, 0, mem );

  // set the two reverb offsets
  dig->reverb_into = 0;
  dig->reverb_outof = 0;

  // set all of the delay line pointers
  dig->ri.c.start0 = (F32 FAR*) ( ( (U8 FAR*) dig->reverb_build_buffer ) + ( ( dig->reverb_buffer_size + 15 ) & ~15 ) );
  dig->ri.c.start1 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start0 ) + del_sizes_pad[ 0 ] );
  dig->ri.c.start2 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start1 ) + del_sizes_pad[ 1 ] );
  dig->ri.c.start3 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start2 ) + del_sizes_pad[ 2 ] );
  dig->ri.c.start4 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start3 ) + del_sizes_pad[ 3 ] );
  dig->ri.c.start5 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start4 ) + del_sizes_pad[ 4 ] );

  dig->ri.c.end0 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start0 ) + del_sizes[ 0 ] );
  dig->ri.c.end1 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start1 ) + del_sizes[ 1 ] );
  dig->ri.c.end2 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start2 ) + del_sizes[ 2 ] );
  dig->ri.c.end3 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start3 ) + del_sizes[ 3 ] );
  dig->ri.c.end4 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start4 ) + del_sizes[ 4 ] );
  dig->ri.c.end5 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start5 ) + del_sizes[ 5 ] );

  dig->ri.u.address0 = dig->ri.c.start0;
  dig->ri.u.address1 = dig->ri.c.start1;
  dig->ri.u.address2 = dig->ri.c.start2;
  dig->ri.u.address3 = dig->ri.c.start3;
  dig->ri.u.address4 = dig->ri.c.start4;
  dig->ri.u.address5 = dig->ri.c.start5;

  dig->ri.u.X0 = 0;
  dig->ri.u.X1 = 0;
  dig->ri.u.Y0 = 0;
  dig->ri.u.Y1 = 0;

  return( 1 );
}


static void calc_constants( REVERB_INFO FAR * ri, F32 time, F32 damping )
{
  F32 C;

  ri->c.C0 = (F32) pow( 0.001f, ReverbDelayTimes[ 0 ] /  time );
  ri->c.C1 = (F32) pow( 0.001f, ReverbDelayTimes[ 1 ] /  time );
  ri->c.C2 = (F32) pow( 0.001f, ReverbDelayTimes[ 2 ] /  time );
  ri->c.C3 = (F32) pow( 0.001f, ReverbDelayTimes[ 3 ] /  time );
  ri->c.C4 = (F32) pow( 0.001f, ReverbDelayTimes[ 4 ] /  ReverbAllPass[ 0 ] );
  ri->c.C5 = (F32) pow( 0.001f, ReverbDelayTimes[ 5 ] /  ReverbAllPass[ 1 ] );

  // calculate new coeffs
  if ( damping < 0.0001f )
  {
    ri->c.A = 0.0f;
    ri->c.B0 = 0.0f;
    ri->c.B1 = 0.0f;
  }
  else
  {
    C =  1.0f / (F32)tan( MY_PI * damping * 0.125f );

    // calculate coefficients
    ri->c.A =  1.0f / ( 1.0f + (F32) sqrt( 2.0f ) * C + C * C );

    // calculate coefficients
    ri->c.B0 = 2.0f * ( 1.0f - C * C ) * ri->c.A;
    ri->c.B1 = ( 1.0f - (F32) sqrt( 2.0 ) * C + C * C ) * ri->c.A;
  }

  check_denorm( &ri->c.C0 );
  check_denorm( &ri->c.C1 );
  check_denorm( &ri->c.C2 );
  check_denorm( &ri->c.C3 );
  check_denorm( &ri->c.C4 );
  check_denorm( &ri->c.C5 );
  check_denorm( &ri->c.A );
  check_denorm( &ri->c.B0 );
  check_denorm( &ri->c.B1 );
}


#ifdef NOQIFIST
static S32 __inline f2i(F32 val)
{
  S32 retval;
  __asm
  {
    fld [val]
    fistp [retval]
  }
  return(retval);
}
#else
#define f2i(val) ((S32)val)
#endif


static void FAR * do_reverb( HDIGDRIVER dig, void FAR * dest, void FAR * src, U32 bytes )
{
  U32 num = bytes / 4;
  S32 FAR * s = (S32 FAR *) src;
  S32 FAR * d = (S32 FAR *) dest;
  S32 stereo = ( dig->hw_format & DIG_F_STEREO_MASK );

  REVERB_INFO ri = dig->ri;

  while (num--)
  {
    F32 temp, U1, U2;
    F32 comb_sum;
    F32 samp, lp_samp;
    F32 left, right;

    samp = ( (F32) *s++ ) * ri.c.A;
    check_denorm( &samp );

    // calculate sample
    lp_samp = samp + ri.u.X0 + ri.u.X0 + ri.u.X1
              - ri.c.B0 * ri.u.Y0
              - ri.c.B1 * ri.u.Y1 + ( 1.192092896e-06F * 2.0f );

    check_denorm( &lp_samp );

    // save sample history
    ri.u.X1 = ri.u.X0;
    ri.u.X0 = samp;
    ri.u.Y1 = ri.u.Y0;
    ri.u.Y0 = lp_samp;

    // process sum of comb filters
    comb_sum = *ri.u.address0 + \
               *ri.u.address1 + \
               *ri.u.address2 + \
               *ri.u.address3;

    check_denorm( &comb_sum );

    // process coef for each comb filter
    *ri.u.address0 = ri.c.C0 * ( *ri.u.address0 ) + lp_samp;
    *ri.u.address1 = ri.c.C1 * ( *ri.u.address1 ) + lp_samp;
    *ri.u.address2 = ri.c.C2 * ( *ri.u.address2 ) + lp_samp;
    *ri.u.address3 = ri.c.C3 * ( *ri.u.address3 ) + lp_samp;

    check_denorm( ri.u.address0 );
    check_denorm( ri.u.address1 );
    check_denorm( ri.u.address2 );
    check_denorm( ri.u.address3 );

    // advance inputs
    ++ri.u.address0;
    ++ri.u.address1;
    ++ri.u.address2;
    ++ri.u.address3;

    // process clip for buffer wrap
    if ( ri.u.address0 >= ri.c.end0 )
      ri.u.address0 = ri.c.start0;

    if ( ri.u.address1 >= ri.c.end1 )
      ri.u.address1 = ri.c.start1;

    if ( ri.u.address2 >= ri.c.end2 )
      ri.u.address2 = ri.c.start2;

    if ( ri.u.address3 >= ri.c.end3 )
      ri.u.address3 = ri.c.start3;


    // get allpass value
    U1 = *ri.u.address4;
    check_denorm(ri.u.address4);
    check_denorm(ri.u.address5);

    // all pass
    temp = ri.c.C4 * U1 + comb_sum;
    *ri.u.address4 = temp;
    ++ri.u.address4;

    check_denorm(ri.u.address4);

    // adjust
    U1 -= ri.c.C4 * temp;
    U2 = *ri.u.address5;

    // all pass
    temp = ri.c.C5 * U2 + ( U1 * 0.25f );
    *ri.u.address5 = temp;
    ++ri.u.address5;

    check_denorm(ri.u.address5);

    // compute left
    left = U2 - ri.c.C5 * temp;
    check_denorm(&left);

    // process clip for buffer wrap
    if ( ri.u.address4 >= ri.c.end4 )
      ri.u.address4 = ri.c.start4;

    if ( ri.u.address5 >= ri.c.end5 )
      ri.u.address5 = ri.c.start5;

    // write out the new sample
    if ( stereo )
    {
      // compute right
      right = U2 - ri.c.C5 * ( ri.c.C5 * U2 + ( U1 * 0.15f ) );
      check_denorm(&right);

      d[ 0 ] += f2i( left );
      d[ 1 ] += f2i( right );
      d += 2;
    }
    else
    {
      d[ 0 ] += f2i( left );
      ++d;
    }
  }

  dig->ri.u = ri.u;

  return( d );
}

void AILCALL AIL_apply_reverb( HDIGDRIVER dig )
{
  S32 left;

  left = dig->reverb_buffer_size - dig->reverb_outof;

  if ( dig->reverb_build_size >= left )
  {
    void * new_dest;

    // do the last bit of the src buffer
    new_dest = do_reverb( dig, dig->build_buffer, ( (U8*) dig->reverb_build_buffer ) + dig->reverb_outof, left );

    // now do the first part of the src buffer
    left = dig->reverb_build_size - left;
    do_reverb( dig, new_dest, dig->reverb_build_buffer, left );

    dig->reverb_outof = left;
  }
  else
  {
    // do the entire src buffer
    do_reverb( dig, dig->build_buffer, ( (U8*) dig->reverb_build_buffer ) + dig->reverb_outof, dig->reverb_build_size );
    dig->reverb_outof += dig->reverb_build_size;
  }

  dig->reverb_into += dig->reverb_build_size;
  if ( dig->reverb_into >= (U32) dig->reverb_buffer_size )
    dig->reverb_into = 0;
}

//############################################################################
//##                                                                        ##
//## Set driver reverb settings                                             ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_digital_master_reverb(HDIGDRIVER dig, F32 reverb_time, F32 reverb_predelay, F32 reverb_damping)
{
  if (dig == NULL)
  {
    return;
  }

  dig->reverb_time = reverb_time;
  dig->reverb_duration = (S32) ( ( reverb_time + reverb_predelay ) * 1000.0f );
  dig->reverb_predelay = reverb_predelay;
  dig->reverb_damping = reverb_damping;

  if ( ( dig->master_wet > 0.0001f ) && ( dig->reverb_time > 0.0001f ) )
  {
    dig->reverb_off_time = AIL_ms_count() + dig->reverb_duration + 100;
    dig->reverb_on = 1;

    U32 bytes = ( (U32) ( reverb_predelay * (F32) dig->DMA_rate ) ) * 4;
    if ( bytes > dig->reverb_into )
    {
      dig->reverb_outof = dig->reverb_buffer_size - ( bytes - dig->reverb_into );
    }
    else
    {
      dig->reverb_outof = dig->reverb_into - bytes;
    }
  }

  calc_constants( &dig->ri, dig->reverb_time, dig->reverb_damping );
}


//############################################################################
//##                                                                        ##
//## Get driver reverb settings                                             ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_digital_master_reverb( HDIGDRIVER dig, F32 FAR * reverb_time, F32 FAR* reverb_predelay, F32 FAR* reverb_damping )
{
  if ( dig == NULL )
  {
    return;
  }

  if ( reverb_time )
  {
    *reverb_time = dig->reverb_time;
  }
  if ( reverb_predelay )
  {
    *reverb_predelay = dig->reverb_predelay;
  }
  if ( reverb_damping )
  {
    *reverb_damping = dig->reverb_damping;
  }
}


typedef struct ROOM_PARAMS
{
  F32 level;
  F32 time;
  F32 damping;
  F32 predelay;
} ROOM_PARAMS;


static ROOM_PARAMS rooms[EAX_ENVIRONMENT_COUNT]  =
{
  { /*EAX_ENVIRONMENT_GENERIC,*/ 0.0F,1.493F,0.5F, 0.011f },
  { /*EAX_ENVIRONMENT_PADDEDCELL,*/ 0.25F,0.1F,0.1F, 0.002f  },
  { /*EAX_ENVIRONMENT_ROOM,*/ 0.417F,0.4F,0.666F, 0.003f },
  { /*EAX_ENVIRONMENT_BATHROOM,*/ 0.75F,1.499F,0.166F, 0.011f },
  { /*EAX_ENVIRONMENT_LIVINGROOM,*/ 0.208F,0.478F,0.1F, 0.004f },
  { /*EAX_ENVIRONMENT_STONEROOM,*/ 0.3F,2.309F,0.68F, 0.017f },
  { /*EAX_ENVIRONMENT_AUDITORIUM,*/ 0.403F,4.279F,0.5F, 0.03f },
  { /*EAX_ENVIRONMENT_CONCERTHALL,*/ 0.5F,3.961F,0.5F, 0.029f },
  { /*EAX_ENVIRONMENT_CAVE,*/ 0.5F,2.886F,1.304F, 0.022f },
  { /*EAX_ENVIRONMENT_ARENA,*/ 0.361F,7.284F,0.332F, 0.03f },
  { /*EAX_ENVIRONMENT_HANGAR,*/ 0.5F,10.0F,0.3F, 0.03f },
  { /*EAX_ENVIRONMENT_CARPETEDHALLWAY,*/ 0.153F,0.259F,0.55F, 0.03f },
  { /*EAX_ENVIRONMENT_HALLWAY,*/ 0.361F,1.493F,0.50F, 0.011f },
  { /*EAX_ENVIRONMENT_STONECORRIDOR,*/ 0.444F,2.697F,0.638F, 0.02f },
  { /*EAX_ENVIRONMENT_ALLEY,*/ 0.25F,1.752F,0.776F, 0.011f },
  { /*EAX_ENVIRONMENT_FOREST,*/ 0.111F,3.145F,0.472F, 0.088f },
  { /*EAX_ENVIRONMENT_CITY,*/ 0.111F,2.767F,0.224F, 0.011f },
  { /*EAX_ENVIRONMENT_MOUNTAINS,*/ 0.2F,0.8F,0.2F, 0.300f },
  { /*EAX_ENVIRONMENT_QUARRY,*/ 0.55F,3.00F,0.55F, 0.043f },
  { /*EAX_ENVIRONMENT_PLAIN,*/ 0.12F,0.8F,0.18F, 0.100f },
  { /*EAX_ENVIRONMENT_PARKINGLOT,*/ 0.208F,1.652F,1.5F, 0.012f },
  { /*EAX_ENVIRONMENT_SEWERPIPE,*/ 0.652F,2.886F,0.25F, 0.021f },
  { /*EAX_ENVIRONMENT_UNDERWATER,*/ 1.0F,1.499F,0.1F, 0.011f },
  { /*EAX_ENVIRONMENT_DRUGGED,*/ 0.875F,8.392F,1.388F, 0.011f },
  { /*EAX_ENVIRONMENT_DIZZY,*/ 0.139F,10.00F,1.5F, 0.011f },
  { /*EAX_ENVIRONMENT_PSYCHOTIC,*/ 0.486F,7.563F,2.5F, 0.011f }
};


//############################################################################
//##                                                                        ##
//## Set driver reverb settings                                             ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_digital_master_room_type( HDIGDRIVER dig, S32 room_type )
{
  if ( dig == 0 )
    return;

  dig->last_room_type = room_type;
  AIL_set_digital_master_reverb( dig, rooms[ room_type ].time, rooms[ room_type ].predelay, rooms[ room_type ].damping );
  AIL_set_digital_master_reverb_levels( dig, 1.0f, rooms[ room_type ].level );
}


//############################################################################
//##                                                                        ##
//## Lowpass helper functions                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_apply_lowpass( void FAR* dest, void const FAR* src, void const FAR* src_end, LOWPASS_INFO FAR* lp_in, S32 op )
{
  LOWPASS_INFO lp = *lp_in;

  if ( op & M_SRC_16 )
  {
    S16 FAR * d = (S16 FAR*) dest;
    S16 const FAR * s = (S16 const FAR*) src;

    if ( op & M_SRC_STEREO )
    {
      do
      {
        register S32 in_s, out_s;

        in_s = ( (S32) *s++ ) * lp.c.A;

        out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                  - lp.c.B0 * lp.u.YL0
                  - lp.c.B1 * lp.u.YL1 ) >> 12;

        lp.u.XL1 = lp.u.XL0;
        lp.u.XL0 = in_s;
        lp.u.YL1 = lp.u.YL0;
        lp.u.YL0 = out_s;

        if ( out_s < -32768 )
          *d++ = -32768;
        else if ( out_s > 32767 )
          *d++ = 32767;
        else
          *d++ = (S16) out_s;


        in_s = ( (S32) *s++ ) * lp.c.A;

        out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                  - lp.c.B0 * lp.u.YL0
                  - lp.c.B1 * lp.u.YL1 ) >> 12;

        lp.u.XR1 = lp.u.XR0;
        lp.u.XR0 = in_s;
        lp.u.YR1 = lp.u.YR0;
        lp.u.YR0 = out_s;

        if ( out_s < -32768 )
          *d++ = -32768;
        else if ( out_s > 32767 )
          *d++ = 32767;
        else
          *d++ = (S16) out_s;

      } while ( (void FAR*)s < src_end );
    }
    else
    {
      do
      {
        register S32 in_s, out_s;

        in_s = ( (S32) *s++ ) * lp.c.A;

        out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                  - lp.c.B0 * lp.u.YL0
                  - lp.c.B1 * lp.u.YL1 ) >> 12;

        lp.u.XL1 = lp.u.XL0;
        lp.u.XL0 = in_s;
        lp.u.YL1 = lp.u.YL0;
        lp.u.YL0 = out_s;

        if ( out_s < -32768 )
          *d++ = -32768;
        else if ( out_s > 32767 )
          *d++ = 32767;
        else
          *d++ = (S16) out_s;

      } while ( (void FAR*)s < src_end );
    }
  }
  else
  {
    U8 FAR * d = (U8 FAR*) dest;
    U8 const FAR * s = (U8 const FAR*) src;

    if ( op & M_SRC_STEREO )
    {
      do
      {
        register S32 in_s, out_s;

        in_s = ( ( (S32) *s++ ) - 128 ) * lp.c.A;

        out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                  - lp.c.B0 * lp.u.YL0
                  - lp.c.B1 * lp.u.YL1 ) >> 12;

        lp.u.XL1 = lp.u.XL0;
        lp.u.XL0 = in_s;
        lp.u.YL1 = lp.u.YL0;
        lp.u.YL0 = out_s;

        if ( out_s < -32768 )
          *d++ = 0;
        else if ( out_s > 32767 )
          *d++ = 255;
        else
          *d++ = (U8) ( out_s + 128 );


        in_s = ( ( (S32) *s++ ) - 128 ) * lp.c.A;

        out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                  - lp.c.B0 * lp.u.YL0
                  - lp.c.B1 * lp.u.YL1 ) >> 12;

        lp.u.XR1 = lp.u.XR0;
        lp.u.XR0 = in_s;
        lp.u.YR1 = lp.u.YR0;
        lp.u.YR0 = out_s;

        if ( out_s < -32768 )
          *d++ = 0;
        else if ( out_s > 32767 )
          *d++ = 255;
        else
          *d++ = (U8) ( out_s + 128 );

      } while ( (void FAR*)s < src_end );
    }
    else
    {
      do
      {
        register S32 in_s, out_s;

        in_s = ( ( (S32) *s++ ) - 128 ) * lp.c.A;

        out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                  - lp.c.B0 * lp.u.YL0
                  - lp.c.B1 * lp.u.YL1 ) >> 12;

        lp.u.XL1 = lp.u.XL0;
        lp.u.XL0 = in_s;
        lp.u.YL1 = lp.u.YL0;
        lp.u.YL0 = out_s;

        if ( out_s < -32768 )
          *d++ = 0;
        else if ( out_s > 32767 )
          *d++ = 255;
        else
          *d++ = (U8) ( out_s + 128 );

      } while ( (void FAR*)s < src_end );
    }
  }

  lp_in->u = lp.u;
}

#endif

void AILCALL AIL_API_set_sample_low_pass_cut_off( HSAMPLE S,
                                                  F32     cutoff )
{
  if ( S )
  {
    if ( ( cutoff < 0.0f ) || ( cutoff >= 0.999f ) )
    {
      S->lp.on = 0;
      S->lp.cutoff = 1.0f;
    }
    else
    {
#ifndef IS_XBOX
      F32 A;

      if ( cutoff < 0.0001f )
      {
        A = 0;
        S->lp.c.A =  0;
        S->lp.c.B0 = 0;
        S->lp.c.B1 = 0;
      }
      else
      {
        // calculate new coeffs
        F32 C =  1.0F / (F32)tan( MY_PI * cutoff * 0.5f );

        // calculate coefficients
        A = 4096.0F / ( 1.0F + ( (F32)sqrt( 2.0F ) * C ) + C * C );
        S->lp.c.A =  (S32) A;

        // calculate coefficients
        S->lp.c.B0 =  (S32)( 2.0F * ( 1.0F - C * C ) * A );
        S->lp.c.B1 =  (S32)( ( 1.0F - ( (F32)sqrt( 2.0F ) * C ) + C * C ) * A );
      }

      if ( S->lp.on == 0 )
      {
        S->lp.u.XL0 = 0;
        S->lp.u.XL1 = 0;
        S->lp.u.YL0 = 0;
        S->lp.u.YL1 = 0;
        S->lp.u.XR0 = 0;
        S->lp.u.XR1 = 0;
        S->lp.u.YR0 = 0;
        S->lp.u.YR1 = 0;
      }
      else
      {
        S->lp.u.XL0 = (S32) ( ( (F32) S->lp.u.YL0 ) * A );
        S->lp.u.XL1 = (S32) ( ( (F32) S->lp.u.YL1 ) * A );
        S->lp.u.XR0 = (S32) ( ( (F32) S->lp.u.YR0 ) * A );
        S->lp.u.XR1 = (S32) ( ( (F32) S->lp.u.YR1 ) * A );
      }

#endif
      S->lp.cutoff = cutoff;
      S->lp.on = 1;
    }

    set_hardware_low_pass( S );
  }
}

F32 AILCALL AIL_API_sample_low_pass_cut_off(HSAMPLE S)
{
  if ( ( S ) && ( S->lp.on ) )
  {
    return( S->lp.cutoff );
  }
  else
  {
    return( 1.0f );
  }
}

//############################################################################
//##                                                                        ##
//## Start playback of sample from beginning                                ##
//##                                                                        ##
//## At a minimum, sample must first have been initialized with prior calls ##
//## to AIL_init_sample() and AIL_set_sample_address()                      ##
//##                                                                        ##
//## Playback will begin at the next DMA half-buffer transition             ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_start_sample( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  //
  // Make sure sample has been allocated
  //

  if ( S->status == SMP_FREE )
  {
    return;
  }

  //
  // Make sure valid sample data exists
  //

  if ( ( S->len  [ S->current_buffer ] == 0 ) ||
       ( S->start[ S->current_buffer ] == NULL ) )
  {
    return;
  }

  // Clear residuals
  clear_last_sample(S);

  // Reset ADPCM data
  S->adpcm.blockleft = 0;
  S->adpcm.extrasamples = 0;

  //
  // Rewind sample to beginning
  //

  S->pos[ S->current_buffer ] = 0;

  //
  // Mark as single-buffered sample
  //

  S->service_type = 1;


  #if !defined(IS_DOS) && !defined(IS_XBOX)
  if ( S->driver->released )
  {
     S->status = SMP_PLAYINGBUTRELEASED;
  }
  else
  #endif
  {
    //
    // start reverb if we need to
    //

    if ( ( S->wet_level > 0.0001f ) && ( S->driver->master_wet > 0.0001f ) && ( S->driver->reverb_time > 0.0001f ) )
    {
      S->driver->reverb_off_time = AIL_ms_count() + S->driver->reverb_duration + 100;
      S->driver->reverb_on = 1;
    }

    //
    // Set 'playing' status
    //

    S->status = SMP_PLAYING;

    //
    // If sample's driver is not already transmitting data, start it
    //

    SS_start_DIG_driver_playback( S );
  }
}

//############################################################################
//##                                                                        ##
//## Stop playback of sample                                                ##
//##                                                                        ##
//## Sample playback may be resumed with AIL_resume_sample(), or restarted  ##
//## from the beginning with AIL_start_sample()                             ##
//##                                                                        ##
//## Playback will stop at the next DMA half-buffer transition              ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_stop_sample( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  #ifdef IS_XBOX

  if ( S->status == SMP_PLAYING )
  {
    check_hardware_buffer( S, 1 );
  }

  hardware_stop( S );
  #endif

  //
  // Make sure sample is currently playing
  //

  if ( S->status != SMP_PLAYING )
  {
    // on the xbox, if the sound is stopped and they call stop again, do the spin
    hardware_spin_until_stopped( S );
    return;
  }

  //
  // Mask 'playing' status
  //

  S->status = SMP_STOPPED;
}

//############################################################################
//##                                                                        ##
//## Resume playback of sample from current position                        ##
//##                                                                        ##
//## Playback will resume at the next DMA half-buffer transition            ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_resume_sample( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  //
  // See if sample has been previously stopped
  //

  if ( S->status == SMP_STOPPED )
  {
    goto startsound;
  }
  else
  {
    if ( S->status == SMP_DONE )
    {
      //
      // this means they called resume before start (good for non-zero
      //   start offsets)
      //

      //
      // Make sure valid sample data exists
      //

      if ( ( S->len  [ S->current_buffer ] == 0 ) ||
           ( S->start[ S->current_buffer ] == NULL ) )
      {
        return;
      }

     startsound:

      // Clear residuals
      clear_last_sample(S);

      #if !defined(IS_DOS) && !defined(IS_XBOX)
      if ( S->driver->released )
      {
        S->status = SMP_PLAYINGBUTRELEASED;
      }
      else
      #endif
      {
        if ( ( S->service_type == 2) && ( S->last_buffer < 0 ) )
          return;

        //
        // start reverb if we need to
        //

        if ( ( S->wet_level > 0.0001f ) && ( S->driver->master_wet > 0.0001f ) && ( S->driver->reverb_time > 0.0001f ) )
        {
          S->driver->reverb_off_time = AIL_ms_count() + S->driver->reverb_duration + 100;
          S->driver->reverb_on = 1;
        }

        //
        // Set 'playing' status
        //

        S->status = SMP_PLAYING;

        //
        // If sample's driver is not already transmitting data, start it
        //

        SS_start_DIG_driver_playback( S );
      }
    }
  }
}

//############################################################################
//##                                                                        ##
//## Terminate playback of sample, setting sample status to SMP_DONE        ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_end_sample(HSAMPLE S)
{
  if ( S == NULL )
  {
    return;
  }

  hardware_stop( S );

  //
  // Make sure sample has been allocated
  //

  if ( S->status == SMP_FREE )
  {
    return;
  }

  hardware_spin_until_stopped( S );

  //
  // If sample is not already done, halt it and invoke its end-of-buffer
  // and end-of-sample callback functions
  //

  if ( S->status != SMP_DONE )
  {
    //
    // Set 'done' status
    //

    S->status = SMP_DONE;
    S->starved = 1;

    //
    // Call EOB and EOS functions
    //

    if ( S->EOB != NULL )
    {
      MSS_do_cb1( (AILSAMPLECB) ,S->EOB,S->driver->callingDS,
                   S);
    }

    if ( S->EOS != NULL )
    {
      MSS_do_cb1( (AILSAMPLECB) ,S->EOS,S->driver->callingDS,
                  S);
    }
  }
}


//############################################################################
//##                                                                        ##
//## Set start-of-block (SOB) callback function for sample                  ##
//##                                                                        ##
//## Callback function will be invoked prior to playback of each sample     ##
//## data block, whose position within the sample can be determined by      ##
//## calling AIL_sample_position()                                          ##
//##                                                                        ##
//## The rate at which this function is called is determined by the DMA     ##
//## half-buffer transition period for the driver in use; e.g., a 1K half-  ##
//## buffer being played at 22 kHz will trigger start-of-block callbacks    ##
//## at a rate of 22 per second                                             ##
//##                                                                        ##
//## This function returns the sample's previous SOB callback handler       ##
//## address, or NULL if no SOB callback handler was registered             ##
//##                                                                        ##
//############################################################################

AILSAMPLECB AILCALL AIL_API_register_SOB_callback( HSAMPLE S, AILSAMPLECB SOB )
{
  AILSAMPLECB old;

  if ( S == NULL )
  {
    return( 0 );
  }

  old = S->SOB;

  S->SOB = SOB;

  return( old );
}

//############################################################################
//##                                                                        ##
//## Set end-of-buffer (EOB) callback function for sample                   ##
//##                                                                        ##
//## Callback function will be invoked when either of two sample buffers    ##
//## finish playing                                                         ##
//##                                                                        ##
//## When not double-buffering, the end-of-sample (EOS) callback, if any,   ##
//## will be triggered immediately after the end of buffer 0                ##
//##                                                                        ##
//## This function returns the sample's previous EOB callback handler       ##
//## address, or NULL if no EOB callback handler was registered             ##
//##                                                                        ##
//############################################################################

AILSAMPLECB AILCALL AIL_API_register_EOB_callback( HSAMPLE S, AILSAMPLECB EOB )
{
  AILSAMPLECB old;

  if ( S == NULL )
  {
    return( 0 );
  }

  old = S->EOB;

  S->EOB = EOB;

  return( old );
}

//############################################################################
//##                                                                        ##
//## Set end-of-sample (EOS) callback function for sample                   ##
//##                                                                        ##
//## Callback function will be invoked when all valid sample buffers have   ##
//## finished playing                                                       ##
//##                                                                        ##
//## When not double-buffering, the end-of-sample (EOS) callback will be    ##
//## triggered immediately after the end of buffer 0                        ##
//##                                                                        ##
//## This function returns the sample's previous EOS callback handler       ##
//## address, or NULL if no EOS callback handler was registered             ##
//##                                                                        ##
//############################################################################

AILSAMPLECB AILCALL AIL_API_register_EOS_callback( HSAMPLE S, AILSAMPLECB EOS )
{
  AILSAMPLECB old;

  if ( S == NULL )
  {
    return( 0 );
  }

  old = S->EOS;

  S->EOS = EOS;

  return( old );
}

//############################################################################
//##                                                                        ##
//## Set sample user data value at specified index                          ##
//##                                                                        ##
//## Any desired 32-bit value may be stored at one of eight user data words ##
//## associated with a given SAMPLE                                         ##
//##                                                                        ##
//## Callback functions may access the user data array at interrupt time    ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_user_data( HSAMPLE S, U32 index, S32 value )
{
  if ( S == NULL )
  {
    return;
  }

  S->user_data[ index ] = value;
}

//############################################################################
//##                                                                        ##
//## Get sample user data value at specified index                          ##
//##                                                                        ##
//## Any desired 32-bit value may be stored at one of eight user data words ##
//## associated with a given SAMPLE                                         ##
//##                                                                        ##
//## Callback functions may access the user data array at interrupt time    ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_user_data( HSAMPLE S, U32 index )
{
  if ( S == NULL )
  {
    return 0;
  }

  return( S->user_data[ index ] );
}

static S32 nibbles_per_sample( S32 format )
{
  switch ( format )
  {
    case DIG_F_ADPCM_MONO_16:
      return( 1 );
    case DIG_F_ADPCM_STEREO_16:
    case DIG_F_MONO_8:
      return( 2 );
    case DIG_F_STEREO_8:
    case DIG_F_MONO_16:
      return( 4 );
    default:
      return( 8 );
  }
}


//############################################################################
//##                                                                        ##
//## Return minimum buffer size for dual-buffer playback                    ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_minimum_sample_buffer_size( HDIGDRIVER dig, //)
                                                S32       playback_rate,
                                                S32       format )
{
#ifdef IS_XBOX

  return( dig->build_size * 2 );

#else

  S32  app_nibbles_per_sample;
  S32  hw_nibbles_per_sample;
  S32  n;

  //
  // Get # of application nibbles per sample unit
  //

  app_nibbles_per_sample = nibbles_per_sample( format );

  //
  // Get # of hardware nibbles per sample unit
  //

  hw_nibbles_per_sample = dig->channels_per_sample * dig->bytes_per_channel * 2;

  //
  // Multiply size of hardware half-buffer by ratio of logical-to-physical
  // sample size
  //

  n = dig->buffer_size * app_nibbles_per_sample / hw_nibbles_per_sample ;

  //
  // Scale n by resampling ratio
  //

  n = n * playback_rate / dig->DMA_rate;

  //
  // Scale n by 2X resampling tolerance to provide safety margin
  //

  n = n + ( ( n * AIL_preference[ DIG_RESAMPLING_TOLERANCE ] ) / 32768 );

  //
  // If DMA rate is not 1X, 2X, or 4X times playback rate, round buffer
  // size up 1 sample to avoid possible truncation errors
  //

  if ( ( dig->DMA_rate != ( 1 * playback_rate ) ) &&
       ( dig->DMA_rate != ( 2 * playback_rate ) ) &&
       ( dig->DMA_rate != ( 4 * playback_rate ) ) )
  {
    n += 4;
  }

  //
  // Round n up to nearest multiple of 256 bytes
  //

  n = ( n + 255 ) & ~255;

  //
  // Return size *3 (*4 in Win32) to ensure enough data is
  // available for initial buffer fill
  //

#ifndef IS_DOS

#if defined(IS_WIN32)
  if ( AIL_preference[ DIG_USE_WAVEOUT ] )
  {
    n = n * 4;
  }
  else
#endif
#if defined(IS_WIN32)
  {
    // get the number of fragments to mix ahead
    S32 DS_frag_mix_ahead = AIL_preference[ DIG_DS_MIX_FRAGMENT_CNT ];
    if ( DS_frag_mix_ahead >= dig->DS_frag_cnt )
    {
      DS_frag_mix_ahead = dig->DS_frag_cnt - 1;
    }

    n = n * DS_frag_mix_ahead;
  }
#else
#if defined(IS_MAC)
  n = n * 2;
#else
  n = n * 3;
#endif
#endif
#endif

 return( n );
#endif
}

//############################################################################
//##                                                                        ##
//## Set address and length for one of two double-buffered sample buffers   ##
//##                                                                        ##
//## Start playback of sample if not already in progress                    ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_load_sample_buffer(HSAMPLE S, S32 buff_num, void const FAR *buffer, U32 len)
{
  if (S == NULL)
  {
    return;
  }

#ifdef IS_XBOX
  S->direct_mode = 0;
#endif

  S->done [buff_num] = (len == 0);
  S->start[buff_num] = buffer;
  S->len  [buff_num] = len;
  S->pos  [buff_num] = 0;

  S->starved = 0;

  if (len)
  {
    if (S->status != SMP_PLAYING)
    {
      //
      // Mark as double-buffered sample
      //

      S->service_type = 2;

#if 0
      //
      // Don't start driver playback until both buffers primed with
      // stream data
      //

      if (S->last_buffer < 0)
      {
        return;
      }
#endif

      //
      // Set 'playing' status
      //

      #if !defined(IS_DOS) && !defined(IS_XBOX)
      if (S->driver->released)
      {
        S->status = SMP_PLAYINGBUTRELEASED;
      }
      else
      #endif
      {
        //
        // start reverb if we need to
        //

        if ( ( S->wet_level > 0.0001f ) && ( S->driver->master_wet > 0.0001f ) && ( S->driver->reverb_time > 0.0001f ) )
        {
          S->driver->reverb_off_time = AIL_ms_count() + S->driver->reverb_duration + 100;
          S->driver->reverb_on = 1;
        }

        S->status = SMP_PLAYING;

        //
        // Start driver playback
        //

        SS_start_DIG_driver_playback(S);
      }
    }
  }
}

//############################################################################
//##                                                                        ##
//## Get double-buffer playback status for sample                           ##
//##                                                                        ##
//##  0: Buffer 0 is ready to be filled (buffer 1 or neither buffer playing)##
//##  1: Buffer 1 is ready to be filled (buffer 0 playing)                  ##
//## -1: Both buffers are already full                                      ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_buffer_ready( HSAMPLE S )
{
  if ( S == NULL )
  {
    return -1;
  }

  //
  // For first two calls after sample initialization, return 0 and 1,
  // respectively
  //
  // This allows the application to "prime" the buffers for continued
  // playback
  //

  switch ( S->last_buffer )
  {
    case -2:
      //
      // First call after AIL_init_sample() must clear second buffer's
      // "done" flag to permit buffer-switching
      //

      S->done[ 1 ] = 0;

      //
      // Set up to load buffer 0 this call, and "bootstrap" buffer 1 at
      // next call
      //

      S->last_buffer = -1;
      return( 0 );

    case -1:
      //
      // Return 1 to force load of second buffer immediately
      // Subsequent calls should operate on alternating buffers
      //

      S->last_buffer = S->current_buffer;
      return( 1 );
  }

  //
  // If buffer has not switched since last call, return -1
  //

  if ( S->last_buffer == S->current_buffer )
  {
    return( -1 );
  }

  //
  // New current_buffer exists -- set last_buffer equal to
  // current_buffer and return exhausted buffer
  //

  S->last_buffer = S->current_buffer;

  return( S->current_buffer ^ 1 );
}


//############################################################################
//##                                                                        ##
//## Get information about status of streaming buffer pair                  ##
//##                                                                        ##
//############################################################################

S32  AILCALL AIL_API_sample_buffer_info( HSAMPLE    S, //)
                                         U32     FAR* pos0,
                                         U32     FAR* len0,
                                         U32     FAR* pos1,
                                         U32     FAR* len1 )
{
  if ( S == 0 )
  {
    if ( pos0 ) *pos0 = 0;
    if ( len0 ) *len0 = 0;
    if ( pos1 ) *pos1 = 0;
    if ( len1 ) *len1 = 0;
    return( 0 );
  }
  
  if ( pos0 != NULL )
  {
    *pos0 = S->pos[ 0 ];
  }

  if ( pos1 != NULL )
  {
    *pos1 = S->pos[ 1 ];
  }

  if ( len0 != NULL )
  {
    *len0 = S->len[ 0 ];
  }

  if ( len1 != NULL )
  {
    *len1 = S->len[ 1 ];
  }

  return( S->starved );
}

//############################################################################
//##                                                                        ##
//## Get size and current position of sample in milliseconds                ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_ms_position( HSAMPLE  S, //)
                                         S32 FAR *total_milliseconds,
                                         S32 FAR *current_milliseconds )
{
  U32 datarate;

  if ( S == NULL )
  {
    return;
  }

  //
  // Get data rate in bytes per second
  //

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    //
    // ADPCM compression
    //

    U32 samples_per_block = 4 << ( ( S->format & DIG_F_STEREO_MASK ) ? 1 : 0 );
    samples_per_block = 1 + ( S->adpcm.blocksize - samples_per_block ) * 8 / samples_per_block;

    datarate = ( S->playback_rate * S->adpcm.blocksize ) / samples_per_block;
  }
  else
  {
    if ( S->pipeline[ DP_ASI_DECODER ].active )
    {
      //
      // ASI compression
      //

      ASISTAGE *ASI = &S->pipeline[ DP_ASI_DECODER ].TYPE.ASI;

      datarate = ASI->ASI_stream_attribute( ASI->stream,
                                            ASI->INPUT_BIT_RATE ) / 8;
    }
    else
    {
      //
      // Straight PCM
      //

      datarate = ( S->playback_rate * nibbles_per_sample( S->format ) ) / 2;
    }
  }

  //
  // Return requested values
  //

  if ( total_milliseconds )
  {
    *total_milliseconds = (S32) ( ( (float) S->len[ S->current_buffer ] * 1000.0 ) / (float) datarate );
  }

  if ( current_milliseconds )
  {
    *current_milliseconds = (S32) ( ( (float) S->pos[ S->current_buffer ] * 1000.0 ) / (float) datarate );
  }
}

//############################################################################
//##                                                                        ##
//## Seek to a specified millisecond within a sample                        ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_ms_position( HSAMPLE S, //)
                                             S32     milliseconds )
{
  U32 datarate;

  if ( S == NULL )
  {
    return;
  }

  //
  // Get data rate in bytes per second
  //

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    //
    // ADPCM compression
    //

    U32 samples_per_block = 4 << ( ( S->format & DIG_F_STEREO_MASK ) ? 1 : 0 );
    samples_per_block = 1 + ( S->adpcm.blocksize - samples_per_block ) * 8 / samples_per_block;

    datarate = ( S->playback_rate * S->adpcm.blocksize ) / samples_per_block;
  }
  else
  {
    if ( S->pipeline[ DP_ASI_DECODER ].active )
    {
      //
      // ASI compression
      //

      ASISTAGE *ASI = &S->pipeline[ DP_ASI_DECODER ].TYPE.ASI;

      datarate = ASI->ASI_stream_attribute( ASI->stream,
                                            ASI->INPUT_BIT_RATE ) / 8;
    }
    else
    {
      //
      // Straight PCM
      //

      datarate = ( S->playback_rate * nibbles_per_sample( S->format ) ) / 2;
    }
  }

  //
  // Set requested position
  //

  AIL_set_sample_position( S, (S32) ( ( (float) datarate * (float) milliseconds ) / 1000.0 ) );
}


//############################################################################
//##                                                                        ##
//## ASI callback routine to fetch encoded source data from sample          ##
//##                                                                        ##
//## Used by both MSSWO.CPP and MSSDS.CPP                                   ##
//##                                                                        ##
//############################################################################

S32 AILCALLBACK DP_ASI_DECODER_callback(U32       user, //)
                                        void FAR *dest,
                                        S32       bytes_requested,
                                        S32       offset)
{
   HSAMPLE S = (HSAMPLE) user;

   S32 total = 0;
   S32 orig = bytes_requested;

   S32 n = S->current_buffer;

   if (offset != -1)
      {
      S->pos[n] = offset;
      }

   //
   // Fulfill as much of request as possible from current buffer
   //

   S32 amount = bytes_requested;

   if ((S->pos[n] + amount) > S->len[n])
      {
      amount = S->len[n] - S->pos[n];
      }

   AIL_memcpy(dest,
              AIL_ptr_add(S->start[n], S->pos[n]),
              amount);

   dest = AIL_ptr_add(dest, amount);

   S->pos[n] += amount;

   total += amount;
   bytes_requested -= amount;

   //
   // If end of buffer reached with samples left to fetch, try to
   // switch buffers
   //

   if (bytes_requested > 0)
      {
      if (S->EOB != NULL)
         ++S->doeob;

      if (S->service_type == 2)
         {

         // reset the ASI, if requested
         if (S->reset_ASI[n])
         {
           // return without switching buffers if we were requested to do a reset
           return(total);
         }

         n ^= 1;

         //
         // If explicit 0-length buffer was posted, fall through to allow
         // the sample to terminate
         //

         if (S->done[n])
            {
            return total;
            }
         else
            {
            //
            // If alternate buffer not yet initialized, break out of loop
            // to allow application to supply data as soon as it can
            //

            if (!S->len[n])
               {
               S->starved = 1;
               return total;
               }

            //
            // If alternate buffer already played, break out of loop to
            // allow application to refresh it as soon as possible
            //

            if (S->pos[n])
               {
               S->starved = 1;
               return total;
               }

            //
            // Otherwise, alternate buffer is ready to play -- switch to
            // it and keep filling output buffer
            //

            S->current_buffer = n;

            amount = bytes_requested;

            if ((S->pos[n] + amount) > S->len[n])
               {
               amount = S->len[n] - S->pos[n];
               }

            AIL_memcpy(dest,
                       AIL_ptr_add(S->start[n], S->pos[n]),
                       amount);

            S->pos[n] += amount;

            total += amount;
            }
         }
      }

   return total;
}

//############################################################################
//##                                                                        ##
//## Flush mixer buffer                                                     ##
//##                                                                        ##
//############################################################################

#ifndef IS_XBOX

void AILCALL SS_flush (HDIGDRIVER dig)
{
   //
   // Initialize the build buffer by flushing with 0
   //

   dig->pipeline[DP_FLUSH].TYPE.MIX.MSS_mixer_flush(dig->build_buffer,
                                                    dig->build_size
#ifdef IS_X86
                                                    ,dig->use_MMX
#endif
                                                    );

   if ( dig->reverb_on )
   {
     if ( dig->reverb_off_time < AIL_ms_count() )
     {
       dig->reverb_on = 0;
     }
     else
     {
       // clear the reverb buffer
       dig->pipeline[DP_FLUSH].TYPE.MIX.MSS_mixer_flush((S32 FAR *)AIL_ptr_add( dig->reverb_build_buffer, dig->reverb_into ),
                                                        dig->reverb_build_size
#ifdef IS_X86
                                                        ,dig->use_MMX
#endif
                                                        );
     }
   }

#if defined(IS_WIN32) || defined(IS_MAC) || defined(IS_LINUX)

   //
   // Invoke installable filters
   //

   FLT_call_premix_processors(dig);

#endif
}


//############################################################################
//##                                                                        ##
//## Merge source data into output buffer by calling processor and mixer    ##
//## handlers                                                               ##
//##                                                                        ##
//############################################################################

S32  SS_merge (HSAMPLE S, //)
               S32     in_len,
               S32     out_pos)
{
  //
  // Set mixer operation code
  //

  S32 op = 0;
  S32 extra_op = 0;
  S32 src_sample_shift = 0;
  S32 dest_sample_shift = 2;

  if (S->driver->hw_format & DIG_F_STEREO_MASK)
  {

    extra_op |= M_DEST_STEREO;
    dest_sample_shift += 1;

    //
    // Reverse left/right channels if DIG_PCM_ORDER specified
    //

    if (S->flags & DIG_PCM_ORDER)
    {
      op |= M_ORDER;
    }
  }

  if (S->format & DIG_F_STEREO_MASK)
  {
    op |= M_SRC_STEREO;
    src_sample_shift += 1;
  }

  if ((S->format & DIG_F_16BITS_MASK) || (S->format & DIG_F_ADPCM_MASK) || (S->pipeline[DP_ASI_DECODER].active) )
  {
    op |= M_SRC_16;
    src_sample_shift += 1;
  }

  F32 wet_level = S->wet_level * S->driver->master_wet;
  F32 dry_level = S->dry_level * S->driver->master_dry;

  // convert 1.0 to 0 to 2048 to 0

  S32 left_scale = (U32) ( 2048.0 * S->left_volume * S->driver->master_volume * dry_level );
  S32 right_scale = (U32) ( 2048.0 * S->right_volume * S->driver->master_volume * dry_level );
  S32 reverb_scale = (U32) ( 2048.0 * S->save_volume * S->driver->master_volume * wet_level );

  if ( left_scale > 2048 )
    left_scale = 2048;
  if ( right_scale > 2048 )
    right_scale = 2048;
  if ( reverb_scale > 2048 )
    reverb_scale = 2048;

  if ((left_scale != 2048) || (right_scale != 2048))
  {
    extra_op |= M_VOL_SCALING;
  }

  S32 reverb_extra_op = (reverb_scale != 2048)? M_VOL_SCALING : 0;

  //
  // Calculate sampling fraction
  //

  S32 playback_ratio = (S32) ((((F32) S->playback_rate) * 65536.0F) /
                                (F32) S->driver->DMA_rate);

  if ((playback_ratio > (65536 + AIL_preference[DIG_RESAMPLING_TOLERANCE])) ||
      (playback_ratio < (65536 - AIL_preference[DIG_RESAMPLING_TOLERANCE])))
  {
    op |= M_RESAMPLE;

    //
    // Enable filtering if preference set
    //

    if ( AIL_preference[ DIG_ENABLE_RESAMPLE_FILTER ] )
    {
      extra_op |= M_FILTER;
    }
  }
  else
  {
    playback_ratio = 65536;
    S->src_fract = 0;
  }

  //
  // Get dest address range
  //

  void FAR *dest = AIL_ptr_add(S->driver->build_buffer,
                               out_pos);

  void FAR *dest_end = AIL_ptr_add(S->driver->build_buffer,
                                   S->driver->build_size);

  void FAR *reverb_dest = AIL_ptr_add(S->driver->reverb_build_buffer,
                                      out_pos + S->driver->reverb_into );

  void FAR *reverb_dest_end = AIL_ptr_add(S->driver->reverb_build_buffer,
                                           S->driver->reverb_into + S->driver->reverb_build_size);

  //
  // Calculate # of bytes needed for this loop
  //
  // This calculation takes resampling into account, as well as the
  // difference in sample sizes between the output of the decode stage
  // ("source sample") and the mixer build buffer.  The resulting sample
  // count reflects the amount of data needed to fill the build buffer
  // from its current out_pos write position.
  //

  S32 needed_dest = AIL_ptr_dif( dest_end, dest );

  S32 needed_src = mult64addandshift( needed_dest >> dest_sample_shift, playback_ratio,
                                      S->src_fract & 0xffff, 16
                                    ) << src_sample_shift;

  S32 using_process = 0;
  U8 process_buffer_start[ DIG_PROCESS_BUFFER_SIZE + 15 ];

  //
  // align our temp pointer
  //

  U8 FAR * process_buffer = (U8 FAR*) ( ( ( (U32) ( &process_buffer_start[ 0 ] ) ) + 15 ) & ~15 );

  //
  // If ASI or ADPCM compression is in use, unpack data into
  // temporary buffer
  //

  void const FAR *src;
  void FAR *src_end;

  if (S->pipeline[DP_ASI_DECODER].active)
  {
    //
    // ASI decoder in use -- source data must be obtained from
    // DP_ASI_DECODER_callback() via ASI_stream_process()
    //


    // ok, if we have a sample left over from the last decompressed buffer, 
    //   then we need to use it first
    if ( S->last_decomp_left != 65536 )
    {
     handle_zero_compressed:
      needed_src = ( 1 << src_sample_shift );
      // we don't store these specifically little endian because we are
      //   just storing exactly what we read earlier (that is, on big endian
      //   machines, last_decomp_left/right are byte swapped in the value)   
      ( ( S16 FAR* ) process_buffer)[ 0 ] = (S16)S->last_decomp_left;
      ( ( S16 FAR* ) process_buffer)[ 1 ] = (S16)S->last_decomp_right;
      goto no_decomp;
    }

    if ( needed_src == 0 )
    {
      // always decompress one sample
      needed_src = ( 1 << src_sample_shift );
    }

    needed_src = S->pipeline[DP_ASI_DECODER].TYPE.ASI.
                   ASI_stream_process(S->pipeline[DP_ASI_DECODER].TYPE.ASI.stream,
                                      process_buffer,
                                      ( needed_src > DIG_PROCESS_BUFFER_SIZE ) ? DIG_PROCESS_BUFFER_SIZE : needed_src );

    // if no data was output and if we have to reset the ASI,
    //   don't advance the output pointer, because we'll just call this
    //   routine again immediately (after the ASI has been reset out at
    //   the upper layer).
    
    if ( ( needed_src == 0 ) && (S->reset_ASI[S->current_buffer]) )
    {
      return 0;
    }

   no_decomp:
    using_process = 1;
    src = process_buffer;
    src_end = AIL_ptr_add( src, needed_src );
  }
  else
  {
    //
    // No ASI decoder in use -- source data is available in its entirety
    // from the current buffer
    //

    src = AIL_ptr_add(S->start[S->current_buffer],
                      S->pos  [S->current_buffer]);

    src_end = AIL_ptr_add(src,
                          in_len);

    //
    // If ADPCM compression in use, decompress the data into the decode
    // buffer
    //

    if (S->format & DIG_F_ADPCM_MASK)
    {

      // ok, if we have a sample left over from the last decompressed buffer, 
      //   then we need to use it first
      if ( S->last_decomp_left != 65536 )
      {
        goto handle_zero_compressed;
      }
      
      if ( needed_src == 0 )
      {
        needed_src = ( 1 << src_sample_shift );
      }

      void const FAR *in  = src;
      void FAR *out = process_buffer;

      //
      // Decode block of data from source sample to decode buffer
      //
      // Size of block to decode is determined by decoder
      // given smaller amount of available and needed data
      //

      if (S->format & DIG_F_STEREO_MASK)
      {
        DecodeADPCM_STEREO(&out, &in,
                           ( needed_src > DIG_PROCESS_BUFFER_SIZE ) ? DIG_PROCESS_BUFFER_SIZE : needed_src,
                           in_len, &S->adpcm);
      }
      else
      {
        DecodeADPCM_MONO(&out, &in,
                         ( needed_src > DIG_PROCESS_BUFFER_SIZE ) ? DIG_PROCESS_BUFFER_SIZE : needed_src,
                         in_len, &S->adpcm);
      }

      //
      // Update source sample position index
      //
      // Note: In Win16, *in and *out are normalized upon return from
      // the ADPCM decoder.  Their selectors may not be the same as the
      // original selectors passed to *in and *out.  Since we are writing
      // to the (small) decode buffer, *out will not be renormalized in
      // this application -- but *in often will.
      //

      using_process = 1;
      src = process_buffer;
      src_end = out;

      needed_src = AIL_ptr_dif( src_end, src );

      S->pos[S->current_buffer] = AIL_ptr_dif(in,
                                              S->start[S->current_buffer]);
    }
    else
    {
      if ( needed_src == 0 )
      {
        //
        // we always need at least one input sample, but only *advance* the
        // source pointer if we are about to wrap around to the next one
        //

        needed_src = ( 1 << src_sample_shift );
        if ( ( S->src_fract & 0xffff ) + playback_ratio >= 65536 )
          goto uncomp_advance;
      }
      else
      {
       uncomp_advance:

        //
        // clamp to the end of the buffer
        //

        if ( needed_src > in_len )
          needed_src = in_len;

        //
        // advance the source pointer
        //

        if ( S->lp.on )
        {
          if ( needed_src > DIG_PROCESS_BUFFER_SIZE )
          {
            needed_src = DIG_PROCESS_BUFFER_SIZE;
          }
        }

        S->pos[ S->current_buffer ] += needed_src;
      }

      src_end = AIL_ptr_add( src, needed_src );
    }
  }

  //
  // If empty source or dest range, return build buffer full
  //

  if ( ( needed_src == 0 ) || ( needed_dest == 0 ) )
  {
    // we return buffer full so that the filling routine that calls
    //   ss_merge exits (otherwise it just keeps calling)
    return S->driver->build_size;
  }

  //
  // do the low pass if necessary
  //

  if ( S->lp.on )
  {
    AIL_apply_lowpass( process_buffer, src, src_end, &S->lp, op );

    src = process_buffer;
    src_end = AIL_ptr_add( src, needed_src );
  }

  //
  // now calc new needed_dest based on our src size (which may have changed
  //   due to input buffer size and/or compression
  //

  S32 dest_from_src = shift64addsubanddiv( needed_src >> src_sample_shift, 16,
                                           playback_ratio - 1, S->src_fract & 0xffff,
                                           playback_ratio ) << dest_sample_shift;

  if ( dest_from_src < needed_dest )
    needed_dest = ( dest_from_src ) ? dest_from_src : ( 1 << dest_sample_shift );


#if defined( IS_WIN32 ) || defined( IS_MAC ) || defined(IS_LINUX)

  //
  // Call reverb processor, if appropriate
  //

  if ( ( S->driver->reverb_on ) && ( reverb_scale != 0 ) )
  {
    U32 timer = AIL_ms_count();

    // advance the time when the reverb should turn off
    if ( ( (U32) ( S->driver->reverb_off_time - timer ) ) < ( S->driver->reverb_duration + 100 ) )
    {
      S->driver->reverb_off_time = timer + S->driver->reverb_duration + 100;
    }

    // mix the audio samples into the reverb buffer

    void const FAR *temp_src = src;
    U32       temp_src_fract = S->src_fract;
    void FAR *temp_dest      = reverb_dest;
    S32       temp_left_val  = S->left_val;
    S32       temp_right_val = S->right_val;
    S32 call_mixer = 1;

    if (S->pipeline[DP_FILTER].active)
    {
      FLTPROVIDER FAR *F = S->pipeline[DP_FILTER].TYPE.FLT.provider;

      call_mixer = F->sample_process(S->pipeline[DP_FILTER].TYPE.FLT.sample_state,
                                     &temp_src,
                                     &temp_src_fract,
                                      src_end,
                   (S32 FAR * FAR *) &temp_dest,
                                      reverb_dest_end,
                                     &temp_left_val,
                                     &temp_right_val,
                                      playback_ratio,
                                      reverb_scale,
                                      reverb_scale,
                                     &S->pipeline[DP_MERGE].TYPE.MIX,
                                      op | reverb_extra_op );
    }

    if (call_mixer)
    {
      //
      // Execute mixer call normally with original src and dest pointers,
      // discarding pointers returned by filter
      //

      temp_src       = src;
      temp_src_fract = S->src_fract;
      temp_dest      = reverb_dest;
      temp_left_val  = S->left_val;
      temp_right_val = S->right_val;

      S->pipeline[DP_MERGE].TYPE.MIX.MSS_mixer_merge(&temp_src,
                                                     &temp_src_fract,
                                                      src_end,
                                   (S32 FAR * FAR *) &temp_dest,
                                                      reverb_dest_end,
                                                     &temp_left_val,
                                                     &temp_right_val,
                                                      playback_ratio,
                                                      reverb_scale,
                                                      reverb_scale,
                                                      op | reverb_extra_op
#ifdef IS_X86
                                                      ,S->driver->use_MMX
#endif
                                                      );
    }
  }

#endif

  //
  // Call mixer provider
  //

#ifdef IS_32

  //
  // Call installable filter first, skipping mixer call altogether if
  // requested
  //

  #ifndef IS_DOS

  S32 call_mixer = 1;

  void const FAR *filter_src       = src;
  U32       filter_src_fract = S->src_fract;
  void FAR *filter_dest      = dest;
  S32       filter_left_val  = S->left_val;
  S32       filter_right_val = S->right_val;

  if ( (S->pipeline[DP_FILTER].active) && ( ( left_scale != 0 ) || ( right_scale != 0 ) ) )
  {
    FLTPROVIDER FAR *F = S->pipeline[DP_FILTER].TYPE.FLT.provider;

    call_mixer = F->sample_process(S->pipeline[DP_FILTER].TYPE.FLT.sample_state,
                                   &filter_src,
                                   &filter_src_fract,
                                    src_end,
                 (S32 FAR * FAR *) &filter_dest,
                                    dest_end,
                                   &filter_left_val,
                                   &filter_right_val,
                                    playback_ratio,
                                    left_scale,
                                    right_scale,
                                   &S->pipeline[DP_MERGE].TYPE.MIX,
                                    op | extra_op );
  }

  if (call_mixer)
  #endif
  {
    if ( ( left_scale == 0 ) && ( right_scale == 0 ) )
    {
      //
      // volume is zero, so just advance the dest pointer (don't mix)
      //

      clear_last_sample(S);

      dest = AIL_ptr_add( dest, needed_dest );
    }
    else
    {
      //
      // Execute mixer call normally with original src and dest pointers,
      // discarding pointers returned by filter
      //

      S->pipeline[DP_MERGE].TYPE.MIX.MSS_mixer_merge(&src,
                                                     &S->src_fract,
                                                     src_end,
                                   (S32 FAR * FAR *) &dest,
                                                     dest_end,
                                                     &S->left_val,
                                                     &S->right_val,
                                                     playback_ratio,
                                                     left_scale,
                                                     right_scale,
                                                     op | extra_op
#ifdef IS_X86
                                                     ,S->driver->use_MMX
#endif
                                                     );

    }
  }
  #ifndef IS_DOS
  else
  {
    //
    // Skip mixer call, and update source and dest pointers based on
    // values returned from filter
    //

    src = filter_src;
    S->src_fract = filter_src_fract;
    dest         = filter_dest;
    S->left_val  = filter_left_val;
    S->right_val = filter_right_val;
  }
  #endif

#else

  U32 src_offset  = LOWORD(src);
  U32 dest_offset = LOWORD(dest);

  U32 src_end_offset  = src_offset  + AIL_ptr_dif(src_end, src);
  U32 dest_end_offset = dest_offset + AIL_ptr_dif(dest_end,dest);

  S->pipeline[DP_MERGE].TYPE.MIX.MSS_mixer_merge(HIWORD(src),
                                                 HIWORD(dest),
                                                &S->src_fract,
                                                &src_offset,
                                                &dest_offset,
                                                 src_end_offset,
                                                 dest_end_offset,
                                                &S->left_val,
                                                &S->right_val,
                                                 playback_ratio,
                                                 (left_scale << 16) | right_scale,
                                                 op|extra_op);
  
  dest = (S32 FAR *) AIL_ptr_add(dest, dest_offset - LOWORD(dest));

#endif

  // if we are using the process buffer, either we used all of it, or
  //   we didn't use the very last sample.  if we didn't use the last
  //   one, then save it (since the process_buffer is only on the stack)
  if ( using_process )
  {
    if ( src != src_end )
    {
      // we don't store these specifically little endian because we are
      //   just storing exactly what we read earlier (that is, on big endian
      //   machines, last_decomp_left/right are byte swapped in the value)   
      if ( S->format & DIG_F_STEREO_MASK )
      {
        if (S->flags & DIG_PCM_ORDER)
        {
          S->last_decomp_left  =  ( (S16 FAR*) src )[ 1 ];
          S->last_decomp_right  =  ( (S16 FAR*) src )[ 0 ];
        }
        else
        {
          S->last_decomp_left  =  ( (S16 FAR*) src )[ 0 ];
          S->last_decomp_right  =  ( (S16 FAR*) src )[ 1 ];
        }
      }
      else
      {
        S->last_decomp_left  =  ( (S16 FAR*) src )[ 0 ];
        S->last_decomp_right =  S->last_decomp_left;
      }
    }
    else
    {
      S->last_decomp_left  =  65536;
    }
  }

  //
  // Return updated build buffer output offset
  //

  return AIL_ptr_dif(dest,
                     S->driver->build_buffer);
}

//############################################################################
//##                                                                        ##
//## Copy mixer buffer to DMA buffer                                        ##
//##                                                                        ##
//############################################################################

void AILCALL SS_copy (HDIGDRIVER dig, void FAR *lpWaveAddr)
{
#ifdef IS_32

  //
  // Add reverb buffer contents, if any, to build buffer
  //

  if ( dig->reverb_on )
  {
    AIL_apply_reverb( dig );
  }

#endif

#if defined(IS_WIN32) || defined(IS_MAC) || defined(IS_LINUX)

  //
  // Invoke installable filters
  //

  FLT_call_postmix_processors(dig);

#endif

#ifdef IS_DOS
  //
  // If DIG_DUAL_DMA or DIG_PCM_SPLIT modes are in use for legacy cards
  // like the RAP-10 and GUS, we must use the old AILSSA_DMA_copy routine
  // instead of the new RIB-based provider.  These modes will not be
  // supported in future RIB providers.
  //

  if (dig->hw_mode_flags & (DIG_PCM_SPLIT | DIG_DUAL_DMA))
  {
     AILSSA_DMA_copy( ( lpWaveAddr == dig->DMA[ 0 ] ) ? 0 : 1,
                    &dig->DMA[0],
                     dig->hw_mode_flags,
                     dig->hw_format,
                     dig->n_active_samples,
                     dig->build_buffer,
                     dig->samples_per_buffer,
                     dig->buffer_size);
  }
  else
#endif
  {
    //
    // Copy build buffer contents to output buffer
    //

    dig->pipeline[DP_COPY].TYPE.MIX.MSS_mixer_copy(dig->build_buffer,
                                                   dig->build_size,
                                                   lpWaveAddr,
                                                   dig->hw_format
#ifdef IS_MAC
                                                   ,1
#else
#ifdef IS_X86
                                                   ,dig ->use_MMX
#endif
#endif
                                                   );
  }
}


#endif

//############################################################################
//##                                                                        ##
//## Copy data from source sample to build buffer, with mixing and ASI      ##
//## decompression                                                          ##
//##                                                                        ##
//############################################################################

void AILCALL SS_stream_to_buffer(HSAMPLE S, S32 size)
{
  S32 out_pos;
  S32 in_len;
  S32 next_buffer;

  S->dosob=0;
  S->doeob=0;
  S->doeos=0;

  //
  // Bail if source stream ended in previous call
  //

  if (S->status != SMP_PLAYING)
  {
    return;
  }

  //
  // Init output buffer position to beginning of build buffer
  //            

  out_pos = 0;

  //
  // Merge source data into output buffer until source exhausted or output full
  //
  // Loop can be exited under the following conditions:
  //
  // 1) Output buffer full (normal condition)
  //
  // 2) Source sample ended (normal condition)
  //
  // 3) Source stream "starved" (abnormal condition)
  //

  while (out_pos < size)
  {
    //
    // Set input len = size of source block to merge, based on
    // size and playback position of source sample
    //

    in_len = S->len[S->current_buffer] - S->pos[S->current_buffer];

    //
    // Initial block may terminate before end of source buffer, if loop
    // endpoint has been declared
    //

    if (S->loop_count != 1)
    {
      if ((S->loop_end != -1) && ((U32) S->loop_end < S->len[S->current_buffer]))
      {
        in_len = S->loop_end - S->pos[S->current_buffer];
      }
    }

    //
    // If no input data left, check for buffer switching and loop iteration
    //
    // (Note: in_len may fall below 0 if source data is undersampled!)
    //

    if (in_len <= 0)
    {
      //
      // Set end-of-buffer function, if any, to signal end of loop block
      // or buffer
      //
      // Note that this function will be called repeatedly if buffer
      // starvation occurs
      //

      if (S->EOB != NULL)
        ++S->doeob;

      // reset the ASI, if requested
      if (S->reset_ASI[S->current_buffer])
      {
        // if requested, do a reset now that the ASI decoder has the data
        if ( S->pipeline[DP_ASI_DECODER].active )
        {
          ASISTAGE *ASI = &S->pipeline[DP_ASI_DECODER].TYPE.ASI;
          ASI->ASI_stream_seek(ASI->stream, -2);
        }
        S->reset_ASI[S->current_buffer]=0;
      }

      //
      // If looping active, go back to beginning of loop to fetch next
      // source data block
      //

      if (S->loop_count != 1)
      {
        //
        // Reset source sample position to beginning of loop
        //

        S->pos[S->current_buffer] = S->loop_start;

        //
        // Reset ADPCM offset to the end of decode buffer
        // to force a decode buffer refill
        //

        if (S->format & DIG_F_ADPCM_MASK)
        {
          S->adpcm.blockleft = 0;
          S->adpcm.extrasamples = 0;
        }

        //
        // Reset ASI state, if applicable
        //

        if (S->pipeline[DP_ASI_DECODER].active)
        {
          ASISTAGE *ASI = &S->pipeline[DP_ASI_DECODER].TYPE.ASI;
          ASI->ASI_stream_seek(ASI->stream, -2);
        }

        //
        // Decrement loop count if not infinite
        //

        if (S->loop_count != 0)
        {
          --S->loop_count;
        }

        //
        // Recalculate size and address of source block
        //

        continue;
      }


      //
      // If streaming sample and alternate buffer available,
      // set up to fetch next source data block from it
      //
      // Typically, buffer-switching takes place in the ASI fetch
      // callback function if an ASI decoder is in use.  We must
      // still be able to switch here, though, in case the callback
      // function returns at the end of its current buffer.
      //

      if (S->service_type == 2)
      {

        next_buffer = S->current_buffer ^ 1;

        //
        // If explicit 0-length buffer was posted, fall through to allow
        // the sample to terminate
        //

        if (!S->done[next_buffer])
        {
          //
          // If alternate buffer not yet initialized, break out of loop
          // to allow application to supply data as soon as it can
          //

          if (!S->len[next_buffer])
          {
            goto starved;
          }

          //
          // If alternate buffer already played, break out of loop to
          // allow application to refresh it as soon as possible
          //

          if (S->pos[next_buffer])
          {
           starved:
            S->starved=1;

#ifdef IS_XBOX
            // once we starve, we fill the rest of this buffer with silence
            //   and then we need to clear the next two as well
            if ( out_pos == 0 )
              ++S->cleared_bufs;

            if ( S->cleared_bufs <= 2 )
            {
              // fill rest to zero
              AIL_memset( AIL_ptr_add( S->ds_staging_buffer->data,
                                      ( size & -S->waiting_for_which_half ) +
                                      out_pos ),
                          ( S->format & DIG_F_16BITS_MASK ) ? 0 : 0x80,
                          size - out_pos );
            }
#endif
            goto docbs;
          }

          //
          // Otherwise, alternate buffer is ready to play -- switch to
          // it and keep filling output buffer
          //

          S->current_buffer = next_buffer;
          continue;
        }
      }

      //
      // If new input data is still not available after looping
      // and/or switching buffers, terminate the sample
      //

#ifdef IS_XBOX
      // once we're done, we fill the rest of this buffer with silence
      //   and then we need to clear the next two as well
      if ( out_pos == 0 )
        ++S->cleared_bufs;

      // fill rest to zero
      AIL_memset( AIL_ptr_add( S->ds_staging_buffer->data,
                              ( size & -S->waiting_for_which_half ) +
                              out_pos ),
                  ( S->format & DIG_F_16BITS_MASK ) ? 0 : 0x80,
                  size - out_pos );

      if ( S->cleared_bufs < 2 )
        goto docbs;
#endif

      S->status = SMP_DONE;
      S->starved = 1;

      if (S->EOS != NULL)
         ++S->doeos;

      goto docbs;
    }

    //
    // Set to perform start-of-block callback
    //

    if (S->SOB != NULL)
       ++S->dosob;

    //
    // Merge source block with contents of output buffer, updating output
    // buffer position counter
    //
    // Note that buffer switching may also take place within this routine if
    // an ASI decoder is in use!
    //

#ifdef IS_XBOX
    S->cleared_bufs = 0;
#endif

    out_pos = SS_merge(S,         // sample handle
                       in_len,    // max len of source block to copy
                       out_pos);  // dest position in build buffer

  }

docbs:

  //
  // make the callbacks after everything's been merged
  //

  while (S->dosob--)
  {
    MSS_do_cb1( (AILSAMPLECB) ,S->SOB,S->driver->callingDS,
    S);
  }

  while (S->doeob--)
  {
    MSS_do_cb1( (AILSAMPLECB) ,S->EOB,S->driver->callingDS,
    S);
  }

  while (S->doeos--)
  {
    MSS_do_cb1( (AILSAMPLECB) ,S->EOS,S->driver->callingDS,
    S);
  }
}

DXDEF S32 AILEXPORT MIX_RIB_MAIN( HPROVIDER provider_handle, U32 up_down )
{
  if (up_down)
  {
    //
    // Initialize default internal mixer RIB
    //

    const RIB_INTERFACE_ENTRY MIXER[] =
       {
       { RIB_FUNCTION, "MIXER_startup",  (U32) &MSS_mixer_startup,  RIB_NONE },
       { RIB_FUNCTION, "MIXER_shutdown", (U32) &MSS_mixer_shutdown, RIB_NONE },
       { RIB_FUNCTION, "MIXER_flush",    (U32) &MSS_mixer_flush,    RIB_NONE },
       { RIB_FUNCTION, "MIXER_merge",    (U32) &MSS_mixer_merge,    RIB_NONE },
       { RIB_FUNCTION, "MIXER_copy",     (U32) &MSS_mixer_copy,     RIB_NONE }
       };

    RIB_register(provider_handle,
                "MSS mixer services",
                 MIXER);
  }
  else
  {
    RIB_unregister_all(provider_handle);
  }

  return( TRUE );
}


/*

Not currently used...

//############################################################################
//##                                                                        ##
//## REVERB_process()                                                       ##
//##                                                                        ##
//############################################################################

void REFLECT_process(HSAMPLE   S, //)
                     void const FAR *orig_src,
                     U32       orig_src_fract,
                     void const FAR *orig_src_end,
                     S32  FAR *build_dest,
                     void FAR *build_dest_end,
                     S32       playback_ratio,
                     U32       op)
{
   //
   // Reverb algorithm details:
   //
   // Apply reflections to contents of this buffer segment, until
   // either (a) the volume scalar has dropped below 60 dB down (< 2
   // on a 2048-unit linear square); or (b) the end of the reverb buffer
   // is reached via wraparound to the reverb buffer's write cursor.
   //
   // The period of each reflection is determined by the reverb voice's
   // reflection time.
   //
   // The initial volume is determined by scaling the base sample volume
   // by the reverb voice's level.
   //
   // Prior to each reflection, the 0-2048 volume is dropped by a factor
   // which will reduce it to 2 in the number of reflection intervals which
   // equals the period specified by the reverb buffer's decay time.
   //
   // The initial dest buffer offset for the first reflection is determined
   // by adding the reflection time to the reverb buffer's current_offset.
   // Subsequent reflection offsets are determined by adding the reflection
   // period to the last reflection's offset.
   //
   // The reverb buffer's write cursor, however, is advanced only when the
   // reverb buffer data is added to the build buffer contents in the postmix
   // stage.  In the postmix stage, the contents of the reverb buffer are
   // added into the output build buffer, and the buffer's write cursor
   // is advanced accordingly.
   //

   HDIGDRIVER dig = S->driver;

   //
   // Fail if reverb parms invalid or disabled
   //

   if ((S->reverb_reflect_time < 0.0001F) ||
       (S->reverb_level        < 0.001F))
      {
      return;
      }

   //
   // Get initial destination offset
   //
   // The size of the reverb buffer is a multiple of the build buffer size,
   // and this position is guaranteed to be aligned to a
   // build-buffer-size boundary
   //

   S32 dest_offset = dig->reverb_buffer_position;

   //
   // Adjust destination offset by difference between build_dest pointer and
   // beginning of build buffer
   //
   // This handles the cases where buffer-switching, looping, etc.
   // require multiple calls to the mixer to fill the build buffer
   // completely
   //

   dest_offset += AIL_ptr_dif(build_dest,
                              dig->build_buffer);

   S32 FAR *write_cursor = (S32 FAR *) AIL_ptr_add(dig->reverb_buffer,
                                                   dest_offset);

   //
   // Calculate # of bytes in reverb buffer that correspond to
   // reflection time interval
   //

   F32 adv = F32(S->playback_rate) * S->reverb_reflect_time;

   S32 reflection_advance = ((S32) adv) *
                           ((op & M_DEST_STEREO) ? 8 : 4);

   //
   // Calculate factor by which to decrease amplitude each reflection
   //
   // We want the volume to fall to 2/2048 (1/1024=-60 dB) after
   // reverb_decay_time seconds elapses.  Each reflection interval is
   // reverb_reflect_time seconds long, so the d/r quotient is the total
   // number of reflection periods over which the volume decay should occur.
   //

   S32 decay_intervals = (S32) (S->reverb_decay_time / S->reverb_reflect_time);

   if (decay_intervals == 0)
      {
      decay_intervals = 1;
      }

   S32 volume_decay = (S->base_scale - 2) / decay_intervals;

   if (volume_decay == 0)
      {
      volume_decay = 1;
      }

   //
   // Get (potential) amount of destination data to write to reverb
   // buffer for each reflection interval
   //

   S32 max_dest_bytes = AIL_ptr_dif(build_dest_end, build_dest);

   S32 FAR *reverb_end = (S32 FAR *) AIL_ptr_add(dig->reverb_buffer,
                                                 dig->reverb_buffer_size);

   //
   // Modify mixer operation code to support reverb (always use volume
   // scaling, never do filtering)
   //

   op |=  M_VOL_SCALING;
   op &= ~M_FILTER;

   //
   // Generate reflections in reverb buffer until buffer full or sound
   // dies out
   //

   S32 left_scale  = (S32) (((F32) S->left_scale)  * S->reverb_level);
   S32 right_scale = (S32) (((F32) S->right_scale) * S->reverb_level);
   S32 base_scale  = (S32) (((F32) S->base_scale)  * S->reverb_level);

   S32  FAR *dest;
   void FAR *dest_end;

   S32 wrapped = 0;

   while (1)
      {
#if 0
      S32 tolerance = reflection_advance / 1000;

      dest_offset -= tolerance;

      dest_offset += (rand() % (2 * tolerance));

      dest_offset &= ~7;
#endif

      //
      // Add one reflection period to dest buffer offset
      //

      dest_offset += reflection_advance;

      if (dest_offset >= dig->reverb_buffer_size)
         {
         dest_offset -= dig->reverb_buffer_size;
         }

      //
      // Decrease amplitude by decay value; exit if -60 dB point reached
      //

      base_scale  = max(base_scale  - volume_decay, 0);
      left_scale  = max(left_scale  - volume_decay, 0);
      right_scale = max(right_scale - volume_decay, 0);

      if (base_scale <= 2)
         {
         break;
         }

      --decay_intervals;

      //
      // Add reflection to buffer, handling buffer wrap as needed
      //

      S32 left_val  = S->left_val;
      S32 right_val = S->right_val;

      dest     = (S32 FAR *) AIL_ptr_add(dig->reverb_buffer, dest_offset);
      dest_end =             AIL_ptr_add(dest,               max_dest_bytes);

      void const FAR *src        = orig_src;
      U32       src_fract  = orig_src_fract;
      void const FAR *src_end    = orig_src_end;

      while (1)
         {
         if (AIL_ptr_dif(dest, reverb_end) == 0)
            {
            S32 size = AIL_ptr_dif(dest_end, dest);
            dest     = dig->reverb_buffer;
            dest_end = AIL_ptr_add(dest, size);
#if 0
            AIL_debug_printf("!");
#endif
            continue;
            }

         S32 remnant = AIL_ptr_dif(dest_end, reverb_end);

         if (remnant > 0)
            {
#if 0
            AIL_debug_printf("W");
#endif
            dest_end = reverb_end;
            }

         if ((AIL_ptr_dif(write_cursor, dest)     > 0) &&
             (AIL_ptr_dif(dest_end, write_cursor) > 0))
            {
            dest_end = write_cursor;
            remnant  = 0;
#if 0
            AIL_debug_printf("T");  // truncated write because of wrap
#endif
            }

         S->pipeline[DP_MERGE].TYPE.MIX.MSS_mixer_merge(&src,
                                                        &src_fract,
                                                         src_end,
                                      (S32 FAR * FAR *) &dest,
                                                         dest_end,
                                                        &left_val,
                                                        &right_val,
                                                         playback_ratio,
                                                         left_scale,
                                                         right_scale,
                                                         op,
                                                         S->driver->use_MMX);

         if (remnant > 0)
            {
            dest     = dig->reverb_buffer;
            dest_end = AIL_ptr_add(dest, remnant);
            }
         else
            {
            break;
            }
         }
      }
}

void AIL_allocate_reflection_buffer( HDIGDRVR dig )
{
   //
   // Allocate reverb buffer, if desired
   //

   dig->reverb_buffer_size     = AIL_preference[DIG_REVERB_BUFFER_SIZE];
   dig->reverb_buffer_position = 0;

   if (dig->reverb_buffer_size == 0)
      {
      dig->reverb_buffer = NULL;
      }
   else
      {
      //
      // Reverb buffer size must be a multiple of build buffer size
      //

      S32 s = (dig->reverb_buffer_size + (dig->build_size - 1)) / dig->build_size;

      dig->reverb_buffer_size = s * dig->build_size;

      dig->reverb_buffer = (S32 FAR *) AIL_mem_alloc_lock(dig->reverb_buffer_size);

      if (dig->reverb_buffer == NULL)
         {
         AIL_set_error("Could not allocate reverb buffer.");

         if (drvr->PM_ISR != -1)
            {
            AIL_restore_USE16_ISR(drvr->PM_ISR);
            }

         AIL_call_driver(dig->drvr, DRV_SHUTDOWN_DEV, NULL, NULL);

         dig->drvr->initialized = 0;

         AIL_mem_free_DOS  (dig->DMA_buf, dig->DMA_seg, dig->DMA_sel);
         AIL_mem_free_lock (dig->build_buffer);
         AIL_mem_free_lock (dig->decode_buffer);
         AIL_mem_free_lock (dig);

         return NULL;
         }

      //
      // Wipe reverb buffer
      //

      AIL_memset(dig->reverb_buffer,
                 0,
                 dig->reverb_buffer_size);
      }

}
*/

#ifdef IS_DOS

//############################################################################
//##                                                                        ##
//## End of locked code                                                     ##
//##                                                                        ##
//############################################################################

void AILSFILE_end(void)
{
   if (locked)
      {
      AIL_vmm_unlock_range(AILSFILE_start, AILSFILE_end);

      locked = 0;
      }
}

#endif

