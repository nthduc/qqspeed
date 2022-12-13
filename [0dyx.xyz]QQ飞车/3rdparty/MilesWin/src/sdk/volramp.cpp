//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Volume Ramper                                 ##
//##                                                                        ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 5-Dec-04: Initial                                     ##
//##                                                                        ##
//##  Author: Jeff Roberts                                                  ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-322-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "mss.h"
#include "imssapi.h"


// global buffer
static   S32*   dwTempBuild=0;
static   U32    dwLastBytes=0;

//
// Attribute tokens
//

enum ATTRIB
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ attributes)
   //
   _FX_RAMP_TIME,
   _FX_RAMP_TO,
   _FX_RAMP_AT,
};

//
// Driver state descriptor
//
// One state descriptor is associated with each HDIGDRIVER
//

struct DRIVERSTATE
{
   //
   // Members common to all pipeline filter providers
   //

   HDIGDRIVER       dig;                  // Driver with which this descriptor is associated

   S32 FAR         *build_buffer;         // Location and size of driver's mixer output buffer
   S32              build_buffer_size;

   //
   // Members associated with specific filter provider
   //

   S32 rate;
};

//
// Per-sample filter state descriptor
//
// One state descriptor is associated with each HSAMPLE
//

struct SAMPLESTATE
{
   //
   // Members common to all pipeline filter providers
   //

   HSAMPLE          sample;   // HSAMPLE with which this descriptor is associated
   DRIVERSTATE FAR *driver;   // Driver with which this descriptor is associated

   //
   // Members associated with specific filter provider
   //
   F32    time;

   F32    current_level;
   F32    to_level;
};

//
// Globals
//

static S32 FLT_started = 0;

static C8 FLT_error_text[256];

//############################################################################
//#                                                                          #
//# Return floating-point type as unsigned long U32 (without actually      #
//# converting the value)                                                    #
//#                                                                          #
//############################################################################

static U32 float_as_long(F32 FP)
{
   static U32 val;

   *(F32 FAR *) (&val) = FP;

   return val;
}

//############################################################################
//#                                                                          #
//# Return signed long U32 as single-precision float (without actually     #
//# converting the value)                                                    #
//#                                                                          #
//############################################################################

static F32 long_as_float(S32 integer)
{
   static F32 val;

   *(S32 FAR *) (&val) = integer;

   return val;
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
#define f2i(val) ((S32)(val))
#endif

//############################################################################
//#                                                                          #
//# Retrieve a standard RIB provider attribute by index                      #
//#                                                                          #
//############################################################################

static U32 AILEXPORT PROVIDER_query_attribute (HATTRIB index)
{
   switch ((ATTRIB) index)
      {
      case PROVIDER_NAME:      return (U32) "Volume Ramp Filter";
      case PROVIDER_VERSION:   return 0x100;
      }

   return 0;
}

//############################################################################
//#                                                                          #
//# Set provider preference value, returning previous setting                #
//#                                                                          #
//############################################################################

static S32 AILEXPORT FLT_set_provider_preference (HATTRIB    preference, //)
                                           void FAR*  value)
{
   S32 prev = -1;

//   switch ((ATTRIB) preference)
      {
      //
      // Preferences
      //
      }

   return prev;
}

//############################################################################
//#                                                                          #
//# Return FLT filter error text                                             #
//#                                                                          #
//############################################################################

static C8 FAR *       AILEXPORT FLT_error       (void)
{
   if (!AIL_strlen(FLT_error_text))
      {
      return 0;
      }

   return FLT_error_text;
}

//############################################################################
//#                                                                          #
//# Initialize FLT sample filter                                             #
//#                                                                          #
//############################################################################

static FLTRESULT AILEXPORT FLT_startup     (void)
{
   if (FLT_started++)
      {
      strcpy(FLT_error_text,"Already started");
      return FLT_ALREADY_STARTED;
      }

   //
   // Init static prefs/attributes
   //

   FLT_error_text[0]            = 0;

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//# Shut down FLT sample filter                                              #
//#                                                                          #
//############################################################################

static FLTRESULT      AILEXPORT FLT_shutdown    (void)
{
   if (!FLT_started)
      {
      strcpy(FLT_error_text,"Not initialized");
      return FLT_NOT_INIT;
      }

   --FLT_started;

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//# Allocate driver-specific descriptor                                      #
//#                                                                          #
//############################################################################

static HDRIVERSTATE AILEXPORT FLT_open_driver (HDIGDRIVER dig, //)
                                        S32 FAR   *build_buffer,
                                        S32        build_buffer_size)

{
   if (dwTempBuild==0)
   {
     dwTempBuild=(S32*)AIL_mem_alloc_lock(128000*sizeof(S32));
     if (dwTempBuild==0)
       return(0);
     dwLastBytes = 128000*sizeof(S32); // clear the entire buffer
   }

   DRIVERSTATE FAR *DRV = (DRIVERSTATE *) AIL_mem_alloc_lock(sizeof(DRIVERSTATE));

   if (DRV == NULL)
      {
      strcpy(FLT_error_text,"Out of memory");
      return 0;
      }

   AIL_memset(DRV,
              0,
              sizeof(DRIVERSTATE));

   //
   // Initialize generic members
   //

   DRV->dig               = dig;
   DRV->build_buffer      = build_buffer;
   DRV->build_buffer_size = build_buffer_size;

   AIL_digital_configuration(dig,&DRV->rate,0,0);

   //
   // Return descriptor address cast to handle
   //

   return (HSAMPLESTATE) DRV;
}

//############################################################################
//#                                                                          #
//# Close filter driver instance                                             #
//#                                                                          #
//############################################################################

static FLTRESULT     AILEXPORT FLT_close_driver (HDRIVERSTATE state)
{
   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) state;

   AIL_mem_free_lock(DRV);

   if ( dwTempBuild )
   {
     AIL_mem_free_lock( dwTempBuild );
     dwTempBuild = 0;
   }

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//#  Perform any needed processing before per-sample mixing begins           #
//#                                                                          #
//#  Called after the build buffer has been flushed prior to the mixing      #
//#  phase, but before any samples have been mixed into it                   #
//#                                                                          #
//############################################################################

static void AILEXPORT FLT_premix_process (HDRIVERSTATE driver)
{
}

//############################################################################
//#                                                                          #
//#  Process data after mixing                                               #
//#                                                                          #
//#  Called after all samples have been mixed into the 32-bit build buffer,  #
//#  prior to copying the build-buffer contents to the driver's output       #
//#  buffer                                                                  #
//#                                                                          #
//############################################################################

static void AILEXPORT FLT_postmix_process (HDRIVERSTATE driver)
{
}

//############################################################################
//#                                                                          #
//# Assign filter to specified sample                                        #
//#                                                                          #
//############################################################################

static HSAMPLESTATE AILEXPORT FLTSMP_open_sample (HDRIVERSTATE driver, //)
                                           HSAMPLE      S)
{
   //
   // Allocate SAMPLESTATE descriptor
   //

   SAMPLESTATE FAR *SS = (SAMPLESTATE FAR *) AIL_mem_alloc_lock(sizeof(SAMPLESTATE));

   if (SS == NULL)
      {
      strcpy(FLT_error_text,"Out of memory");
      return 0;
      }

   AIL_memset(SS,
              0,
              sizeof(SAMPLESTATE));

   //
   // Initialize generic members
   //

   SS->driver = (DRIVERSTATE FAR *) driver;
   SS->sample = S;

   //
   // Initialize provider-specific members to their default values
   //


   // init autopan default values
   SS->time   =  0.0f;
   SS->current_level = 1.0f;
   SS->to_level = 1.0f;

   //
   // Return descriptor address cast to handle
   //

   return (HSAMPLESTATE) SS;
}

//############################################################################
//#                                                                          #
//# Close filter sample instance                                             #
//#                                                                          #
//############################################################################

static FLTRESULT     AILEXPORT FLTSMP_close_sample (HSAMPLESTATE state)
{
   SAMPLESTATE FAR *SS = (SAMPLESTATE FAR *) state;

   AIL_mem_free_lock(SS);

   return FLT_NOERR;
}


//############################################################################
//#                                                                          #
//#  Process sample data just prior to mixing                                #
//#                                                                          #
//#  Return 0 to force MSS to skip mixing stage and update returned pointers #
//#  with stored values (orig_src, orig_src_fract, build_dest, left_val, and #
//#  right_val).  Return 1 to allow mixer to operate normally on original    #
//#  source and dest pointers, ignoring any pointer values written back by   #
//#  the filter.                                                             #
//#                                                                          #
//#  Parameters:                                                             #
//#                                                                          #
//#    state is the sample descriptor.  You can retrieve the HSAMPLE via     #
//#    the state.sample member, if needed.                                   #
//#                                                                          #
//#    orig_src is the pointer to the data being submitted to the mixer      #
//#    pipeline.  This is normally in the application's own data space.  The #
//#    filter should advance this pointer if it is going to return 0 to      #
//#    inhibit mixing.                                                       #
//#                                                                          #
//#    orig_src_fract is the fractional (16:16) source address.  Normally    #
//#    the integer part of this fraction is 0, unless the mixer's smoothing  #
//#    algorithm is undersampling the data.  The fractional source address   #
//#    is not usually especially critical, except for DLS synthesis and      #
//#    similar applications with tight subblock loops.  Filters which do not #
//#    maintain fractional source addresses do not need to update this       #
//#    pointer.                                                              #
//#                                                                          #
//#    orig_src_end is the end of the source data + 1 byte.                  #
//#                                                                          #
//#    build_dest is the destination of the current mixing operation in the  #
//#    build buffer.  The build buffer consists of 32-bit DWORDS which are   #
//#    16-bit samples shifted left 11 times (*2048).  The lower 11 bits are  #
//#    used for volume scaling, while the upper 5 bits provide headroom for  #
//#    clipping.  Like orig_src, the filter should advance this pointer if   #
//#    mixing will be inhibited.                                             #
//#                                                                          #
//#    build_dest_end is the end of the destination data area + 1 byte.      #
//#    Due to resampling and format conversion performed by the mixer, the   #
//#    amount of data to be processed in a given frame may be limited by     #
//#    EITHER build_dest_end OR orig_src_end.  The only assumption you can   #
//#    make regarding the amount of data that may be passed to this routine  #
//#    is that it will not exceed the driver's build buffer size.  As little #
//#    as a single mono sample may be passed to this routine.                #
//#                                                                          #
//#    left_val and right_val are the left and right amplitude values from   #
//#    the end of the last mixing operation, respectively.  They are needed  #
//#    by the mixer's smoothing algorithm.  Filters which do not require     #
//#    these values do not need to update them.                              #
//#                                                                          #
//#    playback_ratio is the fixed-point resampling constant, given at 16:16 #
//#    precision.  A value of 0x10000 indicates no resampling.               #
//#                                                                          #
//#    left_scale and right_scale are the 11-bit left and right channel      #
//#    amplitude scalars, ranging from 0 to 2047.                            #
//#                                                                          #
//#    mixer_provider is provided for the convenience of filters which need  #
//#    a general-purpose mixer kernel (such as the reverb example).  This is #
//#    the same mixer provider that will be called immediately after         #
//#    FLTSMP_sample_process() returns with a nonzero value.                 #
//#                                                                          #
//#    mixer_operation is the operation code for the current mixing phase.   #
//#    The operation flags are defined as follows:                           #
//#                                                                          #
//#       M_DEST_STEREO 1  // Set to enable stereo mixer output              #
//#       M_SRC_16      2  // Set to enable mixing of 16-bit samples         #
//#       M_FILTER      4  // Set to enable smoothing when resampling        #
//#       M_SRC_STEREO  8  // Set to enable mixing of stereo input samples   #
//#       M_VOL_SCALING 16 // Set to enable volume scalars other than 2047   #
//#       M_RESAMPLE    32 // Set to enable playback ratios other than 65536 #
//#       M_ORDER       64 // Set to reverse L/R stereo order for sample     #
//#                                                                          #
//############################################################################

static S32 AILEXPORT FLTSMP_sample_process(HSAMPLESTATE    state,  //)
                                    void const FAR * FAR *orig_src,
                                    U32        FAR *orig_src_fract,
                                    void const FAR *orig_src_end,
                                    S32  FAR * FAR *build_dest,
                                    void FAR       *build_dest_end,
                                    S32        FAR *left_val,
                                    S32        FAR *right_val,
                                    S32             playback_ratio,
                                    S32             left_scale,
                                    S32             right_scale,
                                    MIXSTAGE   FAR *mixer_provider,
                                    U32             mixer_operation)
{
   SAMPLESTATE FAR *SS           = (SAMPLESTATE FAR *) state;
   HSAMPLE          S            = SS->sample;
   DRIVERSTATE FAR *DRV          = SS->driver;
   F32    current, to;

   current = SS->current_level;
   to = SS->to_level;

   if ( SS->time <= 0.00001f )
   {
     current = to;
     SS->current_level = to;
     SS->time = 0.0f;
   }
     
   if ( to == current )
   {
     // mix into directly into the output buffer
     mixer_provider->MSS_mixer_merge( orig_src,
                                      orig_src_fract,
                                      orig_src_end,
                                      build_dest,
                                      build_dest_end,
                                      left_val,
                                      right_val,
                                      playback_ratio,
                                      (S32) ( ( (F32) left_scale ) * current ),
                                      (S32) ( ( (F32) right_scale ) * current ),
                                      mixer_operation
  #ifdef IS_X86
                                      ,AIL_MMX_available()
  #endif
                                      );
   }
   else
   {
     void  const FAR   *  lpOrigSrc=  *orig_src;
     S32   FAR   *  lpBuildDest    =  *build_dest;
     S32   FAR   *  lpBuildTemp;
     void  FAR   *  lpBuildTempEnd;
  
     U32    dwBuildBytes   =  AIL_ptr_dif( build_dest_end, *build_dest );
     U32    dwIndex;
     U32    dwTemp;

     // clear buffer
     AIL_memset( dwTempBuild, 0, dwLastBytes );
  
     // get pointer to build buffer
     lpBuildTemp    =  dwTempBuild;
     lpBuildTempEnd =  ( char FAR * )lpBuildTemp + dwBuildBytes;
  
     // mix into temp buffer
     mixer_provider->MSS_mixer_merge( orig_src,
                                      orig_src_fract,
                                      orig_src_end,
                   (S32 FAR * FAR *)  &lpBuildTemp,
                                      lpBuildTempEnd,
                                      left_val,
                                      right_val,
                                      playback_ratio,
                                      left_scale,
                                      right_scale,
                                      mixer_operation
  #ifdef IS_X86
                                      ,AIL_MMX_available()
  #endif
                                      );
  
     F32 adj;

     adj = ( ( to - current ) * 1000.0f ) / ( SS->time * (F32)DRV->rate );

     // calculate the amount the mixer mixed into the build buffer
     // and convert to samples i.e. divide by sample size
     dwLastBytes = AIL_ptr_dif( lpBuildTemp, dwTempBuild );
     dwTemp      = dwLastBytes / 4;
  
     // check if mono or stereo
     if ( mixer_operation & M_DEST_STEREO )
     {
        if ( adj < 0.0f )
        {
          // mix into build buffer
          for( dwIndex = 0; dwIndex < dwTemp; dwIndex += 2 )
          {
            current += adj;  if ( current < to ) current = to;
            // get input samples
            lpBuildDest[ dwIndex ]  += f2i( ( (F32) dwTempBuild[ dwIndex ] ) * current );
            lpBuildDest[ dwIndex + 1 ]  += f2i( ( (F32) dwTempBuild[ dwIndex + 1 ] ) * current );
          }
        }
        else
        {
          // mix into build buffer
          for( dwIndex = 0; dwIndex < dwTemp; dwIndex += 2 )
          {
            current += adj;  if ( current > to ) current = to;
            // get input samples
            lpBuildDest[ dwIndex ]  += f2i( ( (F32) dwTempBuild[ dwIndex ] ) * current );
            lpBuildDest[ dwIndex + 1 ]  += f2i( ( (F32) dwTempBuild[ dwIndex + 1 ] ) * current );
          }
        }
        SS->time -= ( dwTemp * 500 ) / DRV->rate;
     }
     else
     {
        if ( adj < 0.0f )
        {
          // mix into build buffer
          for( dwIndex = 0; dwIndex < dwTemp; dwIndex += 1 )
          {
            current += adj;  if ( current < to ) current = to;
            // get input samples
            lpBuildDest[ dwIndex ]  += f2i( ( (F32) dwTempBuild[ dwIndex ] ) * current );
          }
        }
        else
        {
          // mix into build buffer
          for( dwIndex = 0; dwIndex < dwTemp; dwIndex += 1 )
          {
            current += adj;  if ( current > to ) current = to;
            // get input samples
            lpBuildDest[ dwIndex ]  += f2i( ( (F32) dwTempBuild[ dwIndex ] ) * current );
          }
        }        
        SS->time -= ( dwTemp * 1000 ) / DRV->rate;
     }
     
     if ( SS->time < 0.0f )
       SS->time = 0.0f;

     if ( SS->current_level < to )
     {
       if ( current > to )
         SS->current_level = to;
       else
         SS->current_level = current;     
     }
     else
     {
       if ( current < to )
         SS->current_level = to;
       else
         SS->current_level = current;     
     }

     // advance pointer
     *build_dest  += dwTemp;
   }

   //
   // Return 1 to allow mixing process to run normally
   //

   return 0;
}

//############################################################################
//#                                                                          #
//# Retrieve an FLT sample attribute or preference value by index            #
//#                                                                          #
//############################################################################

static S32     AILEXPORT FLTSMP_sample_attribute (HSAMPLESTATE state, //)
                                           HATTRIB      attribute)
{
   SAMPLESTATE FAR *SS = (SAMPLESTATE FAR *) state;

   switch ((ATTRIB) attribute)
      {
         case  _FX_RAMP_TIME   :
               return( (S32) float_as_long( SS->time ) );

         case  _FX_RAMP_AT   :
               return( (S32) float_as_long( SS->current_level ) );
               
         case  _FX_RAMP_TO   :
               return( (S32) float_as_long( SS->to_level ) );

      }

   return -1;
}

//############################################################################
//#                                                                          #
//# Set sample preference value, returning previous setting                  #
//#                                                                          #
//############################################################################

static S32 AILEXPORT FLTSMP_set_sample_preference (HSAMPLESTATE state, //)
                                            HATTRIB      preference,
                                            void FAR*    value)
{
   SAMPLESTATE FAR *SS  = (SAMPLESTATE FAR *) state;
   HSAMPLE          S   = SS->sample;

   F32 val;
   S32 prev = -1;

   // determine preference
   switch ((ATTRIB) preference)
      {
      //
      // Preferences
      //
      case  _FX_RAMP_TIME  :

            // get previous value and set new
            prev        = (S32) float_as_long( SS->time );
            SS->time   = *(F32 FAR*)value;

            if ( SS->time <= 0.00001f )
              SS->time = 0.0f;
            break;

      case  _FX_RAMP_TO   :

            // get previous value and set new
            prev = (S32) float_as_long( SS->to_level );
            val = *(F32 FAR*)value;

            SS->to_level = val;
            if ( SS->to_level > 1.0f )
              SS->to_level = 1.0f;
            else if ( SS->to_level < 0.0f )
              SS->to_level = 0.0f;

            if ( SS->time <= 0.00001f )
              SS->current_level = SS->to_level;

            break;

      }

   return prev;
}

extern "C" S32 RampMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 RampMain( HPROVIDER provider_handle, U32 up_down )
{
   const RIB_INTERFACE_ENTRY FLT[] =
      {
      REG_AT("Name",                     PROVIDER_NAME,        RIB_STRING),
      REG_AT("Version",                  PROVIDER_VERSION,     RIB_HEX),

      REG_FN(PROVIDER_query_attribute),

      REG_FN(FLT_startup),
      REG_FN(FLT_error),
      REG_FN(FLT_shutdown),

      REG_FN(FLT_open_driver),
      REG_FN(FLT_close_driver),

      REG_FN(FLT_premix_process),
      REG_FN(FLT_postmix_process),

      REG_FN(FLT_set_provider_preference),

      };

   const RIB_INTERFACE_ENTRY FLTSMP[] =
      {
      REG_FN(FLTSMP_open_sample),
      REG_FN(FLTSMP_close_sample),

      REG_FN(FLTSMP_sample_process),

      REG_FN(FLTSMP_sample_attribute),
      REG_FN(FLTSMP_set_sample_preference),

      REG_AT("Ramp Time",             _FX_RAMP_TIME,         RIB_FLOAT),
      REG_PR("Ramp Time",             _FX_RAMP_TIME,         RIB_FLOAT),
      REG_AT("Ramp At",               _FX_RAMP_AT,           RIB_FLOAT),
      REG_AT("Ramp To",               _FX_RAMP_TO,           RIB_FLOAT),
      REG_PR("Ramp To",               _FX_RAMP_TO,           RIB_FLOAT),
      };

   if (up_down)
      {
         RIB_register(provider_handle,
                     "MSS pipeline filter",
                      FLT);

         RIB_register(provider_handle,
                     "Pipeline filter sample services",
                      FLTSMP);
      }
   else
      {
         RIB_unregister_all(provider_handle);
      }

   return TRUE;
}

