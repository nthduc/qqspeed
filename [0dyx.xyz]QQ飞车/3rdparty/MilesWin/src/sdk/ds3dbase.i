//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: M3D module for DirectSound 3D, Aureal A3D, and EAX providers ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 07-Sep-98: Initial                                    ##
//##          1.01 of 21-Oct-98: Added sample attributes, sample rate calls ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#ifdef EAX3
#define INITGUID
#endif

#define diag_printf // AIL_debug_printf

#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <process.h>

#include <dsound.h>

#ifdef AUREAL
#include "ia3d.h"
#endif

#ifdef EAX3D

#ifdef EAX3

#include "eax3.h"

#ifdef EAX4
#include "eax4.h"
#endif

DEFINE_GUID(DSPROPSETID_EAX20_ListenerProperties,
    0x306a6a8,
    0xb224,
    0x11d2,
    0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22);

DEFINE_GUID(DSPROPSETID_EAX_ReverbProperties,
    0x4a4e6fc1,
    0xc341,
    0x11d1,
    0xb7, 0x3a, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

DEFINE_GUID(DSPROPSETID_EAXBUFFER_ReverbProperties,
    0x4a4e6fc0,
    0xc341,
    0x11d1,
    0xb7, 0x3a, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

DEFINE_GUID(DSPROPSETID_EAX20_BufferProperties,
    0x306a6a7,
    0xb224,
    0x11d2,
    0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22);

#define EAXPROP DSPROPSETID_EAX_ListenerProperties
#define EAXBUFPROP DSPROPSETID_EAX_BufferProperties

#elif EAX2

#include "eax2.h"
#define EAXPROP DSPROPSETID_EAX_ListenerProperties
#define EAXBUFPROP DSPROPSETID_EAX_BufferProperties

#else

#include "eax.h"
#define EAXPROP DSPROPSETID_EAX_ReverbProperties
#define EAXBUFPROP DSPROPSETID_EAXBUFFER_ReverbProperties

#endif

#define PSET_SETGET (KSPROPERTY_SUPPORT_GET | KSPROPERTY_SUPPORT_SET)
#define CREVERB_INVALID_VALUE -1.0E+30F
#define CREVERBBUFFER_SETGET (KSPROPERTY_SUPPORT_GET|KSPROPERTY_SUPPORT_SET)
#define CREVERBBUFFER_INVALID_VALUE -1.0E+30F

#endif

#include "mss.h"

#include <objbase.h>


#define DSBCAPS_MUTE3DATMAXDISTANCE 0x00020000

static S32 M3D_started = 0;
static S32 mute_at_max = 1;

static C8 M3D_error_text[256];

//
// Additional attributes and preferences
//

enum ATTRIB
{
   //
   // Provider attribs
   //

   MAX_SUPPORTED_SAMPLES,

   MUTE_AT_MAX,

#ifdef EAX3D
   EAX_ENVIRONMENT,
   EAX_EFFECT_VOLUME,
   EAX_DECAY_TIME,
   EAX_DAMPING,
   EAX_ALL_PARAMETERS,
   EAX_PROPERTY_SET,

#ifdef EAX2
   EAX_ENVIRONMENT_SIZE,
   EAX_ENVIRONMENT_DIFFUSION,
   EAX_ROOM,
   EAX_ROOM_HF,
   EAX_DECAY_HF_RATIO,
   EAX_REFLECTIONS,
   EAX_REFLECTIONS_DELAY,
   EAX_REVERB,
   EAX_REVERB_DELAY,
   EAX_ROOM_ROLLOFF,
   EAX_AIR_ABSORPTION,
   EAX_FLAGS,

#ifdef EAX3
   EAX_ROOM_LF,
   EAX_DECAY_LF_RATIO,
   EAX_REFLECTIONS_PAN,
   EAX_REVERB_PAN,
   EAX_ECHO_TIME,
   EAX_ECHO_DEPTH,
   EAX_MODULATION_TIME,
   EAX_MODULATION_DEPTH,
   EAX_HF_REFERENCE,
   EAX_LF_REFERENCE,
#endif

#ifdef EAX4
  EAX_AUTO_GAIN_P,
  EAX_AUTOWAH_P,
  EAX_CHORUS_P,
  EAX_DISTORTION_P,
  EAX_ECHO_P,
  EAX_EQUALIZER_P,
  EAX_FLANGER_P,
  EAX_FSHIFTER_P,
  EAX_VMORPHER_P,
  EAX_PSHIFTER_P,
  EAX_RMODULATOR_P,
  EAX_REVERB_P,

  EAX_AUTO_GAIN_S_P,
  EAX_AUTOWAH_S_P,
  EAX_CHORUS_S_P,
  EAX_DISTORTION_S_P,
  EAX_ECHO_S_P,
  EAX_EQUALIZER_S_P,
  EAX_FLANGER_S_P,
  EAX_FSHIFTER_S_P,
  EAX_VMORPHER_S_P,
  EAX_PSHIFTER_S_P,
  EAX_RMODULATOR_S_P,
  EAX_REVERB_S_P,
#endif

#endif
#endif

   //
   // Voice attribs for "MSS 3D sample services"
   //

#ifdef EAX3D
   EAX_EFFECT_REVERBMIX,
   EAX_EFFECT_ALL_PARAMETERS,
   EAX_SAMPLE_PROPERTY_SET,

#ifdef EAX2
   EAX_SAMPLE_DIRECT,
   EAX_SAMPLE_DIRECT_HF,
   EAX_SAMPLE_ROOM,
   EAX_SAMPLE_ROOM_HF,
   EAX_SAMPLE_OBSTRUCTION,
   EAX_SAMPLE_OBSTRUCTION_LF_RATIO,
   EAX_SAMPLE_OCCLUSION,
   EAX_SAMPLE_OCCLUSION_LF_RATIO,
   EAX_SAMPLE_OCCLUSION_ROOM_RATIO,
   EAX_SAMPLE_ROOM_ROLLOFF,
   EAX_SAMPLE_AIR_ABSORPTION,
   EAX_SAMPLE_OUTSIDE_VOLUME_HF,
   EAX_SAMPLE_FLAGS,

#ifdef EAX3
   EAX_SAMPLE_OCCLUSION_DIRECT_RATIO,
   EAX_SAMPLE_EXCLUSION,
   EAX_SAMPLE_EXCLUSION_LF_RATIO,
   EAX_SAMPLE_DOPPLER,
#endif

#ifdef EAX4
  EAX_SAMPLE_SLOT_VOLUMES_P,
  EAX_SAMPLE_SLOT_VOLUMES_S_P,
#endif

#endif

#endif

   DIRECTSOUND_SAMPLE_BUFFER, 
};

//
// Use 16K half-buffers by default
//

#define BUFF_SIZE 16384

//
// Service object positions every 100 milliseconds
//

#define SERVICE_MSECS 100

//
// Epsilon value used for FP comparisons with 0
//

#define EPSILON 0.0001F

//
// Object types
//

enum OBJTYPE
{
   IS_SAMPLE,
   IS_LISTENER,
   IS_OBJECT
};

struct DS3VECTOR3D
{
   D3DVALUE x;
   D3DVALUE y;
   D3DVALUE z;
};

//
// Hardware voice structure
//

struct DSVOICE
{
   S32 n_chans;                        // Normally 1
   S32 n_bits;                         // Bits per sample 

   LPDIRECTSOUNDBUFFER   lpdsb;        // Base DirectSound buffer
   LPDIRECTSOUND3DBUFFER lpds3db;      // Extended DirectSound 3D buffer

#ifdef EAX3D
   LPKSPROPERTYSET lpPropertySet;      // Property set interface for secondary buffer
#endif

   struct SAMPLE3D FAR *assigned_to_sample;
   DSVOICE *next_voice;                // Pointer to next in global list
};

//
// Sample descriptor
//

struct SAMPLE3D
{
   OBJTYPE  type;                // Runtime type of object

   U32      index;               // which sample is this?

   U32      status;              // SMP_ flags: _FREE, _DONE, _PLAYING

   void const FAR *start;        // Sample buffer address (W)
   U32       len  ;              // Sample buffer size in bytes (W)
   U32       pos  ;              // Index to next byte (R/W)
   U32       done ;              // Nonzero if buffer with len=0 sent by app

   S32      loop_count;          // # of cycles-1 (1=one-shot, 0=indefinite)
   S32      loop_start;          // Starting offset of loop block (0=SOF)
   S32      loop_end;            // End offset of loop block (-1=EOF)

   F32      volume;              // Sample volume 0-1.0
   S32      playback_rate;       // Playback rate in samples/sec
   S32      chans;               // # of channels

   S32      bytes_per_sample;    // 1 or 2 for 8- or 16-bit samples

   S32      buffers_past_end;    // # of silent buffers appended

   //
   // Positioning
   //

   DS3VECTOR3D position;         // 3D position
   DS3VECTOR3D face;             // 3D orientation
   DS3VECTOR3D up;               // 3D up-vector
   DS3VECTOR3D velocity;         // 3D velocity
   S32         auto_update;      // TRUE to automatically update in background

   F32         max_dist;         // Sample distances
   F32         min_dist;

   //
   // Provider-specific fields
   //

   DSVOICE *voice;                     // Hardware voice currently assigned to sample (NULL if none)

   U32 previous_position;              // Previous play cursor position
   U32 previous_write_position;        // Previous write cursor position
   U32 last_move_time;                 // Last time that the clock moved

   U32 lastblockdone;                  // estimated time when last mix will be done

   F32 obstruction;
   F32 occlusion;
   F32 exclusion;

   F32 inner_angle;
   F32 outer_angle;
   F32 outer_volume;

   volatile S32 noints;          // no interrupt servicing please

   AIL3DSAMPLECB eos;
   H3DSAMPLE clientH3D;
};

//
// define provider name

#ifdef DX7SN
  #define PROVIDER_NAME_STR "DirectSound3D 7+ Software - Pan and Volume"
#else
#ifdef DX7SL
  #define PROVIDER_NAME_STR "DirectSound3D 7+ Software - Light HRTF"
#else
#ifdef DX7SH
  #define PROVIDER_NAME_STR "DirectSound3D 7+ Software - Full HRTF"
#else
#ifdef AUREAL
  #define PROVIDER_NAME_STR "Aureal A3D Interactive (TM)"
#else
#ifdef EAX4
  #define PROVIDER_NAME_STR "Creative Labs EAX 4 (TM)"
#else
#ifdef EAX3
  #define PROVIDER_NAME_STR "Creative Labs EAX 3 (TM)"
#else
#ifdef EAX2
  #define PROVIDER_NAME_STR "Creative Labs EAX 2 (TM)"
#else
#ifdef EAX3D
  #define PROVIDER_NAME_STR "Creative Labs EAX (TM)"
#else
#ifdef HWARE
  #define PROVIDER_NAME_STR "DirectSound3D Hardware Support"
#else
  #define PROVIDER_NAME_STR "DirectSound3D Software Emulation"
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif


//
// Support up to 8 samples (arbitrary limit) with default DS3D software
// provider
//
// With Aureal, we allocate 64 but later set the limit to the actual # of
// streaming buffers available in hardware
//

#ifdef AUREAL
   //
   // Aureal: limit to 96 voices, or max supported, whichever is lower
   //

   #define N_SAMPLES 96
#else
#ifdef DX7SN
   // fast software
   #define N_SAMPLES 64
#else
#ifdef DX7SL
   // light software
   #define N_SAMPLES 32
#else
#ifdef DX7SH
   // slower software
   #define N_SAMPLES 32
#else
#ifdef SWARE
   //
   // Old Software: limit to 16 voices
   //

   #define N_SAMPLES 16
#else
   //
   // Hardware: limit to 96 voices, or max supported, whichever is lower
   //

   #define N_SAMPLES 96

#endif
#endif
#endif
#endif
#endif

//
// Globals
//

static DSVOICE *first_voice = NULL;

static SAMPLE3D samples[N_SAMPLES];
static S32 avail_samples = 0;

static S32 active = 0;

static LPDIRECTSOUND           lpDS;
static LPDIRECTSOUNDBUFFER     lpDSPRIM;
static LPDIRECTSOUND3DLISTENER lp3DLISTENER;
static DSCAPS                  DSCaps;

static DS3VECTOR3D listen_position;
static DS3VECTOR3D listen_face;
static DS3VECTOR3D listen_up;
static DS3VECTOR3D listen_velocity;
static S32         listen_auto_update = 0;

static HTIMER      service_timer;
static HTIMER      buffer_timer;

#ifdef EAX3D
static S32         room_type;                // values defined by EAX.H

static LPDIRECTSOUNDBUFFER   lpSecondaryBuffer = NULL; // Secondary buffer
static LPDIRECTSOUND3DBUFFER lpDs3dBuffer      = NULL; // 3D interface for secondary buffer
static LPKSPROPERTYSET       lpPropertySet     = NULL; // Property set interface for secondary buffer

#endif

static void set_M3D_defaults(H3DSAMPLE samp);

static void API_lock(void)
{
   if (AIL_get_preference(AIL_LOCK_PROTECTION))
     AIL_lock();
}

static void API_unlock(void)
{
   if (AIL_get_preference(AIL_LOCK_PROTECTION))
   AIL_unlock();
}

static S32 incnoints(SAMPLE3D FAR *S)
{
   volatile S32 FAR* noi=&S->noints;
   __asm
   {
     mov edx,[noi]
     mov eax,1
     lock xadd [edx],eax
   }
}

static void decnoints(SAMPLE3D FAR *S)
{
   volatile S32 FAR* noi=&S->noints;
   __asm
   {
     mov eax,[noi]
     lock dec dword ptr [eax]
   }
}

//############################################################################
//##                                                                        ##
//## Convert linear loudness level to 20log10 (dB) level for volume and pan ##
//## functions                                                              ##
//##                                                                        ##
//## Returns negative dB * 100 down from linear_max                         ##
//##                                                                        ##
//############################################################################

static F32 linear_to_dB(F32 linear_level)
{
  //
  // Ensure extreme values return max/min results
  //

  if (linear_level <= 0.001f)
    return -10000.0F;

  if (linear_level >= (1.0f-EPSILON))
    return 0.0F;

  return( (F32) ( ( 20000.0f / 6.0f ) * log10(linear_level) ) );
}

static F32 dB_to_linear(F32 dB)
{
  if ((dB+EPSILON) > 0.0F)
    return 1.0F;
  if ((dB-EPSILON) < -10000.0F)
    return 0.0F;

  return (F32) pow( 10.0f, ( dB / ( 20000.F / 6.0f ) ));
}

//############################################################################
//##                                                                        ##
//## Set volume level of secondary buffer                                   ##
//##                                                                        ##
//############################################################################

static void DS_set_volume(SAMPLE3D FAR *S)
{
   F32 v;

   v = S->volume;

   //
   // Set secondary buffer volume
   //

   if (S->voice != NULL)
      {
      F32 net_volume = linear_to_dB( v
#ifndef EAX3
                       *(1.0F-S->exclusion)
#endif
#ifndef EAX2
                       *(1.0F-S->occlusion)
                       *(1.0F-S->obstruction)
#endif
                       );

      API_lock();
      S->voice->lpdsb->SetVolume((S32)net_volume);
      API_unlock();
      }
}


//############################################################################
//##                                                                        ##
//## Set cone settings of secondary buffer                                  ##
//##                                                                        ##
//############################################################################

static void DS_set_cone(SAMPLE3D FAR *S)
{
   F32 v;

   v = S->outer_volume; // convert to linear

   //
   // Set secondary buffer volume
   //

   if (S->voice != NULL)
      {
      F32 net_volume = linear_to_dB(v);

      API_lock();

      S->voice->lpds3db->SetConeAngles((S32)S->inner_angle,(S32)S->outer_angle,DS3D_DEFERRED );
      S->voice->lpds3db->SetConeOutsideVolume((S32)net_volume,DS3D_DEFERRED );

      lp3DLISTENER->CommitDeferredSettings();

      API_unlock();
      }
}


//############################################################################
//##                                                                        ##
//## Set playback rate of secondary buffer                                  ##
//##                                                                        ##
//############################################################################

static void DS_set_frequency(SAMPLE3D FAR *S)
{
   //
   // Set new frequency
   //

   if (S->voice != NULL)
      {
      API_lock();
      HRESULT result= S->voice->lpdsb->SetFrequency(S->playback_rate);
      API_unlock();
      }
}

//############################################################################
//##                                                                        ##
//## Lock region of secondary buffer, returning write cursor information    ##
//##                                                                        ##
//############################################################################

static S32 DS_lock_secondary_region(SAMPLE3D FAR *S, //)
                                    S32           offset,
                                    S32           size,
                                    void        **p1,
                                    U32          *l1,
                                    void        **p2,
                                    U32          *l2)
{
   HRESULT result;

   if (S->voice == NULL)
      {
      return 0;
      }

   //
   // Lock the buffer, returning 0 on failure
   //

   do
      {
      API_lock();

      result = S->voice->lpdsb->Lock(offset,
                              size,
                              p1,
                              (DWORD*)l1,
                              p2,
                              (DWORD*)l2,
                              0);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         S->voice->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);

   if (result != DS_OK)
      {
      return 0;
      }

   if ((*l1) + (*l2) != (U32) size)
      {
      return 0;
      }

   return 1;
}

//############################################################################
//##                                                                        ##
//## Unlock region of secondary buffer                                      ##
//##                                                                        ##
//############################################################################

static void DS_unlock_secondary_region(SAMPLE3D FAR *S, //)
                                       void         *p1,
                                       U32           l1,
                                       void         *p2,
                                       U32           l2)
{
   HRESULT result;

   if (S->voice == NULL)
      {
      return;
      }

   //
   // Unlock the buffer, returning 0 on failure
   //

   do
      {
      API_lock();

      result = S->voice->lpdsb->Unlock(p1,
                                l1,
                                p2,
                                l2);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         S->voice->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);
}

//############################################################################
//##                                                                        ##
//## Start playback of secondary buffer at beginning                        ##
//##                                                                        ##
//############################################################################

static void DS_start_secondary(SAMPLE3D FAR *S)
{
   HRESULT result;

   if (S->voice == NULL)
      {
      return;
      }

   do
      {
      API_lock();

      S->previous_position = -1;
      S->buffers_past_end = 0;

      result = S->voice->lpdsb->Play(0,
                              0,
                              DSBPLAY_LOOPING);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         S->voice->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);
}

//############################################################################
//##                                                                        ##
//## Stop playback of secondary buffer                                      ##
//##                                                                        ##
//############################################################################

static void DS_stop_secondary(DSVOICE FAR *V)
{
   HRESULT result;

   if (V == NULL)
      {
      return;
      }

   do
      {
      API_lock();

      result = V->lpdsb->Stop();

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         V->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);

}

//############################################################################
//##                                                                        ##
//## Rewind secondary buffer to beginning                                   ##
//##                                                                        ##
//############################################################################

static void DS_rewind_secondary(DSVOICE FAR *V)
{
   HRESULT result;

   if (V == NULL)
      {
      return;
      }

   do
      {
      API_lock();

      result = V->lpdsb->SetCurrentPosition(0);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         V->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);
}

//############################################################################
//##                                                                        ##
//## Flush sample's secondary buffer with silence                           ##
//##                                                                        ##
//############################################################################

static void DS_flush_secondary(DSVOICE FAR *V)
{
   DWORD     dwDummy;
   void   *lpDummy;
   DWORD     cnt;
   void   *dest;
   U32     silence;
   HRESULT result;

   //
   // Request lock on entire buffer
   //

   API_lock();

   do
      {
      result = V->lpdsb->Lock(0,
                              BUFF_SIZE * 2,
                             &dest,
                             &cnt,
                             &lpDummy,
                             &dwDummy,
                              0);

      if (result == DSERR_BUFFERLOST)
         {
         V->lpdsb->Restore();
         }
      }
   while (result == DSERR_BUFFERLOST);

   if (result != DS_OK)
      {
      API_unlock();
      return;
      }

   if (cnt != (U32) BUFF_SIZE * 2)
      {
      API_unlock();
      return;
      }

   //
   // Flush with silence
   //

   silence = (V->n_bits == 16) ? 0 : 0x80808080;

   memset(dest,
          silence,
          cnt);

   //
   // Release lock
   //

   V->lpdsb->Unlock(dest,
                    cnt,
                    lpDummy,
                    dwDummy);

   API_unlock();
}

//############################################################################
//##                                                                        ##
//## Release voice resources                                                ##
//##                                                                        ##
//############################################################################

static void DS_release_voice_resources(DSVOICE FAR *V)
{
   API_lock();

   if (V->lpds3db != NULL)
      {
      V->lpds3db->Release();
      V->lpds3db = NULL;
      }

   if (V->lpdsb != NULL)
      {
      V->lpdsb->Release();
      V->lpdsb = NULL;
      }

#ifdef EAX3D
   if (V->lpPropertySet != NULL)
      {
      V->lpPropertySet->Release();
      V->lpPropertySet = NULL;
      }
#endif

   API_unlock();
}

//############################################################################
//##                                                                        ##
//## Try to find a hardware voice for this sample, if it doesn't already    ##
//## have one                                                               ##
//##                                                                        ##
//## Sample must have valid chans, playback_rate, and bytes_per_sample.     ##
//## A nonzero result means the sample's voice field is valid.  No other    ##
//## sample fields are used by this routine                                 ##
//##                                                                        ##
//############################################################################

static S32 DS_assign_voice_to_sample(SAMPLE3D FAR *S)
{
   //
   // Return values:
   //
   // 0: Voice could not be allocated
   // 1: Existing voice was reused
   // 2: New voice was allocated
   //

   API_lock();

   //
   // Keep sample's current voice if it qualifies
   //

   if (S->voice != NULL)
      {
      if (((S->bytes_per_sample * 8) == S->voice->n_bits) &&
           (S->chans                 == S->voice->n_chans))
         {
         API_unlock();
         return 1;
         }

      //
      // Sample has a voice, but it's the wrong format.  Deallocate it now so it will be 
      // reallocated in the desired format
      //
      // We assume that deallocating the current voice is guaranteed to make a new
      // one available to the allocator below.  In theory, another app could grab the voice,
      // so the caller must check the return value and fail gracefully.  In reality, this
      // should be vanishingly unlikely
      //

      DS_release_voice_resources(S->voice);

      S->voice->assigned_to_sample = NULL;
      S->voice = NULL;
      }

   //
   // Find a compatible hardware voice associated with a SAMPLE3D that's not playing
   // Failing that, try to allocate a new voice from DirectSound
   // Failing that, return 0
   //
   // (Currently, voices are allocated with the H3DSAMPLE itself, and cannot be reclaimed 
   // from inactive samples)
   //

   S32 result = 1;

   DSVOICE FAR *V = NULL;
   
#if 0
   V = first_voice;

   while (V != NULL)
      {
      SAMPLE3D FAR *cur = V->assigned_to_sample;

      if (cur == NULL)
         {
         //
         // Voice is not assigned to any sample -- it's free to use
         //

         break;
         }

      if ((cur->status & 255) != SMP_PLAYING)
         {
         //
         // Current sample using this voice is not playing
         //

         if (((S->bytes_per_sample * 8) == V->n_bits) &&
              (S->chans                 == V->n_chans))
            {
            //
            // Voice matches desired format, so use it
            //

            break;
            }
         }

      V = V->next_voice;
      }
#endif

   //
   // Try to allocate a new voice in the specified format
   //

   if (V == NULL)
      {
      V = (DSVOICE FAR *) AIL_mem_alloc_lock(sizeof(DSVOICE));

      memset(V, 0, sizeof(*V));

      V->lpdsb         = NULL;
      V->lpds3db       = NULL;
#ifdef EAX3D
      V->lpPropertySet = NULL;
#endif

      V->n_chans = S->chans;
      V->n_bits  = S->bytes_per_sample * 8;

      //
      // Set up WAVEFORMATEX structure
      //

      WAVEFORMATEX wf;

      wf.wFormatTag      = WAVE_FORMAT_PCM;
      wf.nChannels       = (S16) S->chans;
      wf.nSamplesPerSec  = S->playback_rate;
      wf.nAvgBytesPerSec = S->playback_rate * S->chans * S->bytes_per_sample;
      wf.nBlockAlign     = (U16) (S->chans * S->bytes_per_sample);
      wf.wBitsPerSample  = (S16) S->bytes_per_sample * 8;
      wf.cbSize          = 0;

      //
      // Set up DSBUFFERDESC structure
      //

      DSBUFFERDESC dsbdesc;

      memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));

      dsbdesc.dwSize           =  sizeof(DSBUFFERDESC);

      dsbdesc.dwFlags          =  DSBCAPS_GLOBALFOCUS   |
                                  DSBCAPS_CTRL3D |
                                  DSBCAPS_GETCURRENTPOSITION2 |
                                  ( ( mute_at_max ) ? DSBCAPS_MUTE3DATMAXDISTANCE : 0 ) |
#ifdef SWARE
                                  DSBCAPS_LOCSOFTWARE |
#else
#ifdef HWARE
                                  DSBCAPS_LOCHARDWARE |
#else
#ifdef EAX3D
                                  DSBCAPS_LOCHARDWARE |
#endif
#endif
#endif
                                  DSBCAPS_CTRLVOLUME |
                                  DSBCAPS_CTRLFREQUENCY;

#ifdef DX7SN
      dsbdesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
#else
#ifdef DX7SL
      dsbdesc.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;
#else
#ifdef DX7SH
      dsbdesc.guid3DAlgorithm = DS3DALG_HRTF_FULL;
#endif
#endif
#endif

      dsbdesc.dwBufferBytes    =  (BUFF_SIZE * 2);
      dsbdesc.lpwfxFormat      = &wf;

      //
      // Create DirectSound buffer (streaming emitter) object
      //

      if (!(SUCCEEDED (lpDS->CreateSoundBuffer(&dsbdesc,
                                               &V->lpdsb,
                                                NULL))))

         {
         AIL_set_error("Could not create secondary DS buffer");

         AIL_mem_free_lock(V);

         API_unlock();
         return FALSE;
         }

      //
      // Get the DirectSound3D interface for this buffer
      //

      HRESULT res = V->lpdsb->QueryInterface(IID_IDirectSound3DBuffer,
                                    (LPVOID *) &V->lpds3db);

      if (!(SUCCEEDED(res)))
         {
         AIL_set_error("Could not obtain IDirectSound3DBuffer interface");

         V->lpdsb->Release();
         V->lpdsb = NULL;

         AIL_mem_free_lock(V);

         API_unlock();
         return FALSE;
         }

#ifdef EAX3D

      //
      // Get the sound-source property set for this buffer, and make sure
      // it supports the reverb-mix property
      //

      if (FAILED(V->lpds3db->QueryInterface(IID_IKsPropertySet,
                                 (void **) &V->lpPropertySet)))
         {
         V->lpPropertySet = NULL;
         }
      else
         {
         DWORD support = 0;

#ifdef EAX2
#define BUFFERPROPSNEEDED DSPROPERTY_EAXBUFFER_ALLPARAMETERS
#else
#define BUFFERPROPSNEEDED DSPROPERTY_EAXBUFFER_REVERBMIX
#endif
         if (FAILED(V->lpPropertySet->QuerySupport(EAXBUFPROP,
                                                   BUFFERPROPSNEEDED,
                                                  &support))
                    ||
                    ((support & CREVERBBUFFER_SETGET) != CREVERBBUFFER_SETGET))
            {
            V->lpPropertySet->Release();
            V->lpPropertySet = NULL;
            }
          }
#endif

      //
      // Allocation succeeded -- link new voice into global list
      //

      V->next_voice = first_voice;
      first_voice   = V;

      //
      // Flush newly-allocated voice with silence
      //

      DS_flush_secondary(V);

      result = 2;
      }

   //
   // Assign voice to sample
   // 

   if (V->assigned_to_sample != NULL)
      {
      V->assigned_to_sample->voice = NULL;
      }

   V->assigned_to_sample = S;

   S->voice = V;

   //
   // Return success
   //

   API_unlock();
   return result;
}

//############################################################################
//#                                                                          #
//# Flush sample buffers                                                     #
//#                                                                          #
//############################################################################

static void reset_voice_state(SAMPLE3D FAR *S, //)
                              BOOL          flush_with_silence)
{
   if (S->voice != NULL)
      {
      DS_stop_secondary(S->voice);

      if (flush_with_silence)
         {
         DS_flush_secondary(S->voice);
         }

      DS_rewind_secondary(S->voice);
      }

   S->previous_position = -1;
   S->buffers_past_end  = 0;
}

//############################################################################
//#                                                                          #
//# Re-enable event processing                                               #
//#                                                                          #
//############################################################################

static void wake_sample(SAMPLE3D FAR *S)
{
  decnoints(S);
}

//############################################################################
//##                                                                        ##
//## Copy data from source sample to target secondary buffer                ##
//##                                                                        ##
//## Backfill target secondary buffer with silence to end of source data    ##
//##                                                                        ##
//############################################################################

static S32 DS3D_stream_to_buffer(SAMPLE3D FAR *S, S32 half, S32 len)
{
   void *out;
   U32   out_len;
   void *in;
   U32   in_len;
   U32   copy_len;
   void *dest1;
   U32   len1;
   void *dest2;
   U32   len2;
   U32   amtmixed=0;
   S32   doclear=0;
   S32   docb=0;

   //
   // Lock segment to fill
   //

   if (!DS_lock_secondary_region(S,
                                 half * len,
                                 len,
                                 &dest1,
                                 &len1,
                                 &dest2,
                                 &len2))
      {
      return 0;
      }

   //
   // Init output pointer to beginning of secondary buffer segment
   //

   out     = dest1;
   out_len = len1;

   //
   // Copy source data to output buffer until source exhausted or output full
   //
   // Loop can be exited under the following conditions:
   //
   // 1) Output buffer full (normal condition)
   //
   // 2) Source sample ended (normal condition)
   //
   // 3) Source stream "starved" (abnormal condition)
   //
   // If source stream ended in previous call, simply flush segment with
   // silence and return
   //

   while (1)
      {
      //
      // Exit if output buffer full
      //

      if (out_len == 0)
         {
         break;
         }

      //
      // Set input pointer and len = initial source block to copy, based on
      // size and playback position of source sample
      //

      in      = (U8 *) S->start + S->pos;
      in_len  =        S->len   - S->pos;

      //
      // Initial block may terminate before end of source buffer, if loop
      // endpoint has been declared
      //

      if (S->loop_count != 1)
         {
         if ((S->loop_end != -1) && ((U32) S->loop_end < S->len))
            {
            in_len = S->loop_end - S->pos;
            }
         }

      //
      // If no input data left, check for buffer switching and loop iteration
      //

      if (in_len == 0)
         {
         //
         // If looping active, go back to beginning of loop to fetch next
         // source data block
         //

         if (S->loop_count != 1)
            {
            //
            // Reset source sample position to beginning of loop
            //

            S->pos = S->loop_start;

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
         // If new input data is still not available after looping,
         // set up to terminate sample after last two valid buffers have
         // been completely processed
         //

         if (S->buffers_past_end++ > 2)
         {
            U32 oldstatus=S->status;

            diag_printf("%X DONE\n",S);

            S->status = SMP_DONE;
            doclear=1;

            if (oldstatus==SMP_PLAYING)
              docb=1;
            break;
         }

         break;
         }

      //
      // Copy block of data directly from source sample to buffer
      //
      // Size of block to copy is determined by smaller amount
      // of available or needed data
      //

      copy_len = min(out_len, in_len);

      memcpy(out, in, copy_len);

      //
      // Update source sample position index
      //

      S->pos += copy_len;

      amtmixed += copy_len;

      out_len -= copy_len;
      out      = (U8 *) out + copy_len;
      }

   //
   // If insufficient source data was available to fill output buffer,
   // flush remainder of buffer with silence
   //

   if (out_len > 0)
      {
      memset(out,
                (S->bytes_per_sample == 2) ? 0 : 0x80808080,
                 out_len);

      out_len = 0;
      }


   // setup to monitor the "SMP_DONE" time
   if (amtmixed)
   {
     U32 timer=AIL_ms_count();

     // if not enough time has past for the last block assume this block will start after it
     if (S->lastblockdone>timer)
       timer=S->lastblockdone;

     S->lastblockdone=timer+((amtmixed*1000)/(S->playback_rate*S->bytes_per_sample));
   }

   //
   // Unlock the previously-locked buffer segment
   //

   DS_unlock_secondary_region(S,
                              dest1,
                              len1,
                              dest2,
                              len2);

   if (doclear)
     reset_voice_state(S, FALSE);

   return(docb);
}


#ifdef EAX3D

//----------------------------------------------------------
// void SetEnvironment(unsigned long envId)
//
// DESCRIPTION: Selects a new environment, from the list
//               in EAX.H.  e.g. EAX_ENVIRONMENT_CAVE
//
// PARAMETERS: Environment ID
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetEnvironment(unsigned long envId)
{
   API_lock();

   lpPropertySet->Set( EAXPROP,
#ifdef EAX2
                      DSPROPERTY_EAXLISTENER_ENVIRONMENT,
#else
                      DSPROPERTY_EAX_ENVIRONMENT,
#endif
                     NULL,0,
                     &envId,sizeof(unsigned long));

   API_unlock();
}

#ifdef EAX2

static void EAX2_Set_int(DSPROPERTY_EAX_LISTENERPROPERTY prop,U32 val)
{
   API_lock();

   lpPropertySet->Set( EAXPROP,
                       prop,
                       NULL,0,
                      &val,sizeof(U32));

   API_unlock();
}

static void EAX2_Set_float(DSPROPERTY_EAX_LISTENERPROPERTY prop,F32 val)
{
   API_lock();

   lpPropertySet->Set( EAXPROP,
                       prop,
                       NULL,0,
                      &val,sizeof(F32));

   API_unlock();
}

static U32 EAX2_Get_int(DSPROPERTY_EAX_LISTENERPROPERTY prop)
{
   U32 val=0xffffffff;
   unsigned long r;

   API_lock();

   lpPropertySet->Get(EAXPROP,
                      prop,
                      NULL,0,
                     &val,sizeof(U32),&r);

   API_unlock();

   return val;
}

static F32 EAX2_Get_float(DSPROPERTY_EAX_LISTENERPROPERTY prop)
{
   F32 val=-1.0;
   unsigned long r;

   API_lock();

   lpPropertySet->Get(EAXPROP,
                      prop,
                      NULL,0,
                     &val,sizeof(F32),&r);

   API_unlock();

   return val;
}


static U32 EAX2_GetSet_int(DSPROPERTY_EAX_LISTENERPROPERTY prop,U32 val)
{
  U32 ret=EAX2_Get_int(prop);
  EAX2_Set_int(prop,val);
  return(ret);
}


static F32 EAX2_GetSet_float(DSPROPERTY_EAX_LISTENERPROPERTY prop,F32 val)
{
  F32 ret=EAX2_Get_float(prop);
  EAX2_Set_float(prop,val);
  return(ret);
}


static void EAX2_Sample_Set_int(SAMPLE3D FAR *S,DSPROPERTY_EAX_BUFFERPROPERTY prop,U32 val)
{
   API_lock();

   S->voice->lpPropertySet->Set(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(U32));

   API_unlock();
}

static void EAX2_Sample_Set_float(SAMPLE3D FAR *S,DSPROPERTY_EAX_BUFFERPROPERTY prop,F32 val)
{
   API_lock();

   S->voice->lpPropertySet->Set(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(F32));

   API_unlock();
}

static U32 EAX2_Sample_Get_int(SAMPLE3D FAR *S,DSPROPERTY_EAX_BUFFERPROPERTY prop)
{
   U32 val=0xffffffff;
   unsigned long r;

   API_lock();

   S->voice->lpPropertySet->Get(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(U32),
                         &r);

   API_unlock();

   return val;
}

static F32 EAX2_Sample_Get_float(SAMPLE3D FAR *S,DSPROPERTY_EAX_BUFFERPROPERTY prop)
{
   F32 val=-1.0f;
   unsigned long r;

   API_lock();

   S->voice->lpPropertySet->Get(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(F32),
                         &r);

   API_unlock();

   return val;
}


static U32 EAX2_Sample_GetSet_int(SAMPLE3D FAR *S,DSPROPERTY_EAX_BUFFERPROPERTY prop,U32 val)
{
  U32 ret=EAX2_Sample_Get_int(S,prop);
  EAX2_Sample_Set_int(S,prop,val);
  return(ret);
}

static F32 EAX2_Sample_GetSet_float(SAMPLE3D FAR *S,DSPROPERTY_EAX_BUFFERPROPERTY prop,F32 val)
{
  F32 ret=EAX2_Sample_Get_float(S,prop);
  EAX2_Sample_Set_float(S,prop,val);
  return(ret);
}

#endif

//----------------------------------------------------------
// void SetVolume(float volume)
//
// DESCRIPTION: Changes the reverb volume
//
// PARAMETERS: volume value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetVolume(float volume)
{
   API_lock();

#ifdef EAX2
   LONG vol=(LONG)linear_to_dB(volume);
   lpPropertySet->Set(EAXPROP,
                      DSPROPERTY_EAXLISTENER_ROOM,
                      NULL,0,
                     &vol,sizeof(LONG));
   lpPropertySet->Set(EAXPROP,
                      DSPROPERTY_EAXLISTENER_ROOMHF,
                      NULL,0,
                     &vol,sizeof(LONG));
#else
   lpPropertySet->Set(EAXPROP,
                      DSPROPERTY_EAX_VOLUME,
                      NULL,0,
                     &volume,sizeof(float));
#endif
   API_unlock();
}

//----------------------------------------------------------
// void SetDecayTime(float time)
//
// DESCRIPTION: Changes the reverb decay time
//
// PARAMETERS: decay time value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetDecayTime(float time)
{
   API_lock();

   lpPropertySet->Set(EAXPROP,
#ifdef EAX2
                      DSPROPERTY_EAXLISTENER_DECAYTIME,
#else
                      DSPROPERTY_EAX_DECAYTIME,
#endif
                      NULL,0,
                     &time,sizeof(float));

   API_unlock();
}

//----------------------------------------------------------
// void SetDamping(float damping)
//
// DESCRIPTION: Changes the reverb damping
//
// PARAMETERS: damping value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetDamping(float damping)
{
   API_lock();

   lpPropertySet->Set(EAXPROP,
#ifdef EAX2
                      DSPROPERTY_EAXLISTENER_DECAYHFRATIO,
#else
                      DSPROPERTY_EAX_DAMPING,
#endif
                      NULL,0,
                     &damping,sizeof(float));

   API_unlock();
}

//----------------------------------------------------------
// unsigned long GetEnvironment()
//
// DESCRIPTION: Gets the current reverb environment
//
// PARAMETERS: none
//
// RETURNS: Environment ID
//
//----------------------------------------------------------

static unsigned long EAX_GetEnvironment()
{
   unsigned long envId=EAX_ENVIRONMENT_GENERIC;
   unsigned long r;

   API_lock();

   lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                      DSPROPERTY_EAXLISTENER_ENVIRONMENT,
#else
                      DSPROPERTY_EAX_ENVIRONMENT,
#endif
                      NULL,0,
                     &envId,sizeof(unsigned long),&r);

   API_unlock();

   return envId;
}

//----------------------------------------------------------
// float GetVolume()
//
// DESCRIPTION: Gets the current reverb volume
//
// PARAMETERS: none
//
// RETURNS: volume value
//
//----------------------------------------------------------

static float EAX_GetVolume()
{
   float volume=CREVERB_INVALID_VALUE;
   unsigned long r;

   API_lock();

#ifdef EAX2
   LONG room=20000;
   lpPropertySet->Get(EAXPROP,
                      DSPROPERTY_EAXLISTENER_ROOM,
                      NULL,0,
                     &room,sizeof(LONG),&r);

   if (room!=20000)
     volume=dB_to_linear((F32)room);
#else
   lpPropertySet->Get(EAXPROP,
                      DSPROPERTY_EAX_VOLUME,
                      NULL,0,
                     &volume,sizeof(float),&r);
#endif

   API_unlock();

   return volume;
}

//----------------------------------------------------------
// float GetDecayTime()
//
// DESCRIPTION: Gets the current reverb decay time
//
// PARAMETERS: none
//
// RETURNS: decay time value
//
//----------------------------------------------------------

static float EAX_GetDecayTime()
{
   float time=CREVERB_INVALID_VALUE;
   unsigned long r;

   API_lock();

   lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                      DSPROPERTY_EAXLISTENER_DECAYTIME,
#else
                      DSPROPERTY_EAX_DECAYTIME,
#endif
                      NULL,0,
                     &time,sizeof(float),&r);

   API_unlock();

   return time;
}

//----------------------------------------------------------
// float GetDamping()
//
// DESCRIPTION: Gets the current reverb damping
//
// PARAMETERS: none
//
// RETURNS: damping value
//
//----------------------------------------------------------

static float EAX_GetDamping()
{
   float damping=CREVERB_INVALID_VALUE;
   unsigned long r;

   API_lock();

   lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                      DSPROPERTY_EAXLISTENER_DECAYHFRATIO,
#else
                      DSPROPERTY_EAX_DAMPING,
#endif
                      NULL,0,
                     &damping,sizeof(float),&r);

   API_unlock();

   return damping;
}

//----------------------------------------------------------
// float SetAll()
//
// DESCRIPTION: Sets all parameters of the current reverb
//
// PARAMETERS: pointer to structure
//
// RETURNS: none
//
//----------------------------------------------------------

static void EAX_SetAll(void FAR *allparameters)
{
   API_lock();

#ifdef EAX2
   lpPropertySet->Set(EAXPROP,
                      DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
                      NULL,0,
                      allparameters,sizeof(EAXLISTENERPROPERTIES));
#else
   lpPropertySet->Set(EAXPROP,
                      DSPROPERTY_EAX_ALL,
                      NULL,0,
                      allparameters,sizeof(EAX_REVERBPROPERTIES));
#endif

   API_unlock();
}

//----------------------------------------------------------
// float GetAll()
//
// DESCRIPTION: Gets all parameters of the current reverb
//
// PARAMETERS: none
//
// RETURNS: pointer to structure
//
//----------------------------------------------------------

static void FAR * EAX_GetAll()
{
#ifdef EAX2
   static EAXLISTENERPROPERTIES allparameters;
#else
   static EAX_REVERBPROPERTIES allparameters;
#endif
   unsigned long r;

   API_lock();

   lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                      DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
#else
                      DSPROPERTY_EAX_ALL,
#endif
                      NULL,0,
                     &allparameters,sizeof(float),&r);

   API_unlock();

   return &allparameters;
}

//----------------------------------------------------------
// void SetReverbMix(float mix)
//
// DESCRIPTION: Changes the reverb mix value
//
// PARAMETERS: mix value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetReverbMix(SAMPLE3D FAR *S, float mix)
{
  API_lock();

#ifdef EAX2
  LONG auto_mode;

  if ( fabs( mix + 1.0F ) < EPSILON )
  {
    // turn on auto
    auto_mode = 1;
    mix = 1.0f;
  }
  else
  {
    // turn off auto
    auto_mode = 0;
  }

  S->voice->lpPropertySet->Set( EAXBUFPROP,
                         EAXBUFFERFLAGS_ROOMAUTO,
                         NULL,
                         0,
                        &auto_mode,
                         sizeof( LONG ) );

  S->voice->lpPropertySet->Set( EAXBUFPROP,
                         EAXBUFFERFLAGS_ROOMHFAUTO,
                         NULL,
                         0,
                        &auto_mode,
                         sizeof( LONG ) );

  LONG vol = (LONG) linear_to_dB( mix );
  S->voice->lpPropertySet->Set( EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_ROOMHF,
                         NULL,
                         0,
                        &vol,
                         sizeof( LONG ) );

  S->voice->lpPropertySet->Set( EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_ROOM,
                         NULL,
                         0,
                        &vol,
                         sizeof( LONG ) );
#else

  // make EAX 1 behave like EAX 2 (in EAX 2, the mix only controls wet,
  //                               in EAX 1, the mix controls only wet from 
  //                                  0 to 0.5, and both from 0.5 to 1)
  // so, we just scale the mix to from 0-1 to 0-0.5 and it matches
   
  if ( mix > 0.0f )
  {
    mix /= 2.0f;
  }

  S->voice->lpPropertySet->Set( EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_REVERBMIX,
                         NULL,
                         0,
                        &mix,
                         sizeof( float ) );
#endif

   API_unlock();
}

//----------------------------------------------------------
// float GetReverbMix()
//
// DESCRIPTION: Gets the current reverb mix
//
// PARAMETERS: none
//
// RETURNS: mix value
//
//----------------------------------------------------------

static float EAX_GetReverbMix(SAMPLE3D FAR *S)
{
   float mix=CREVERBBUFFER_INVALID_VALUE;
   unsigned long r;

   API_lock();

#ifdef EAX2
   LONG room=20000;
   S->voice->lpPropertySet->Get(EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_ROOM,
                         NULL,
                         0,
                        &room,
                         sizeof(LONG),
                         &r);
   if (room!=20000)
     mix=dB_to_linear((F32)room);

#else
   S->voice->lpPropertySet->Get(EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_REVERBMIX,
                         NULL,
                         0,
                        &mix,
                         sizeof(float),
                         &r);
#endif

   API_unlock();

   return mix;
}

//----------------------------------------------------------
// float SetSampleAll()
//
// DESCRIPTION: Sets all parameters of the current sample
//
// PARAMETERS: pointer to structure
//
// RETURNS: none
//
//----------------------------------------------------------

static void EAX_SetSampleAll(SAMPLE3D FAR *S,void FAR *allparameters)
{
   API_lock();

#ifdef EAX2
   S->voice->lpPropertySet->Set(EAXPROP,
                                                 DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
                                                 NULL,0,
                                                 allparameters,sizeof(EAXBUFFERPROPERTIES));
#else
   S->voice->lpPropertySet->Set(EAXPROP,
                                                 DSPROPERTY_EAXBUFFER_ALL,
                                                 NULL,0,
                                                 allparameters,sizeof(EAXBUFFER_REVERBPROPERTIES));
#endif

   API_unlock();
}

//----------------------------------------------------------
// float GetSampleAll()
//
// DESCRIPTION: Gets all parameters of the current sample
//
// PARAMETERS: none
//
// RETURNS: pointer to structure
//
//----------------------------------------------------------

static void FAR * EAX_GetSampleAll(SAMPLE3D FAR *S)
{
#ifdef EAX2
   static EAXBUFFERPROPERTIES allparameters;
#else
   static EAXBUFFER_REVERBPROPERTIES allparameters;
#endif
   unsigned long r;

   API_lock();

   S->voice->lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                                                 DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
#else
                                                 DSPROPERTY_EAXBUFFER_ALL,
#endif
                                                 NULL,0,
                                                &allparameters,sizeof(float),&r);

   API_unlock();

   return &allparameters;
}

//############################################################################
//#                                                                          #
//# Destroy EAX-specific objects                                             #
//#                                                                          #
//############################################################################

static void EAX_destroy(void)
{
   API_lock();

   if (lpSecondaryBuffer != NULL)
      {
      lpSecondaryBuffer->Release();
      lpSecondaryBuffer = NULL;
      }

   if (lpDs3dBuffer != NULL)
      {
      lpDs3dBuffer->Release();
      lpDs3dBuffer = NULL;
      }

   if (lpPropertySet != NULL)
      {
      lpPropertySet->Release();
      lpPropertySet = NULL;
      }

   API_unlock();
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_room_type                                                       #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_room_type (S32 EAX_room_type)
{
   room_type=EAX_room_type;
   if (lpPropertySet)
     EAX_SetEnvironment(EAX_room_type);
}

//##############################################################################
//#                                                                            #
//# M3D_3D_room_type                                                           #
//#                                                                            #
//##############################################################################

static S32       AILEXPORT M3D_3D_room_type (void)
{
   return room_type;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_effects_level                                                #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_effects_level (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return EAX_GetReverbMix(S);
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_effects_level                                            #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_effects_level(H3DSAMPLE samp, //)
                                                     F32       effects_level)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   //
   // Assume the argument is zero or positive, and in decibels.
   // Convert it to a ratio suitable for passing
   //

   if (S->voice != NULL)
   {
     EAX_SetReverbMix(S,effects_level);
   }
}

#endif

#ifndef DSSPEAKER_5POINT1
#define DSSPEAKER_5POINT1 DSSPEAKER_SURROUND + 1
#endif

#ifndef DSSPEAKER_7POINT1
#define DSSPEAKER_7POINT1 DSSPEAKER_SURROUND + 2
#endif

static DWORD RAD_to_DS(S32 st)
{
  switch (st)
  {
    case AIL_3D_HEADPHONE:
      return(DSSPEAKER_HEADPHONE);
    case AIL_3D_SURROUND:
      return(DSSPEAKER_SURROUND);
    case AIL_3D_4_SPEAKER:
      return(DSSPEAKER_QUAD);
    case AIL_3D_51_SPEAKER:
      return(DSSPEAKER_5POINT1);
    case AIL_3D_71_SPEAKER:
      return(DSSPEAKER_7POINT1);
    case AIL_3D_2_SPEAKER:
      return(DSSPEAKER_STEREO);
    default:
      return(st - 128);
  }

}

static S32 DS_to_RAD(DWORD st)
{
  switch (DSSPEAKER_CONFIG(st))
  {
    case DSSPEAKER_HEADPHONE:
      return(AIL_3D_HEADPHONE);
    case DSSPEAKER_SURROUND:
      return(AIL_3D_SURROUND);
    case DSSPEAKER_QUAD:
      return(AIL_3D_4_SPEAKER);
    case DSSPEAKER_5POINT1:
      return(AIL_3D_51_SPEAKER);
    case DSSPEAKER_7POINT1:
      return(AIL_3D_71_SPEAKER);
    case DSSPEAKER_STEREO:
      return(AIL_3D_2_SPEAKER);
    default:
      return(st + 128);
  }

}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_speaker_type                                                    #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_speaker_type (S32 spk_type)
{
  API_lock();

  lpDS->SetSpeakerConfig( RAD_to_DS(spk_type) );

  API_unlock();
}

//##############################################################################
//#                                                                            #
//# M3D_3D_speaker_type                                                        #
//#                                                                            #
//##############################################################################

static S32       AILEXPORT M3D_3D_speaker_type (void)
{
   DWORD spk_type;

    API_lock();
   lpDS->GetSpeakerConfig( &spk_type );
    API_unlock();

   return( DS_to_RAD( spk_type ) );
}


//##############################################################################
//#                                                                            #
//# M3D_set_3D_rolloff_factor                                                  #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_rolloff_factor (F32 factor)
{
  API_lock();

  if (lp3DLISTENER)
  {
    lp3DLISTENER->SetRolloffFactor( factor, DS3D_IMMEDIATE);
  }

  API_unlock();
}

//##############################################################################
//#                                                                            #
//# M3D_3D_rolloff_factor                                                      #
//#                                                                            #
//##############################################################################

static F32       AILEXPORT M3D_3D_rolloff_factor (void)
{
   F32 result = 1.0F;

   API_lock();

   if (lp3DLISTENER)
   {
     lp3DLISTENER->GetRolloffFactor( &result );
   }

   API_unlock();

   return( result );
}


//##############################################################################
//#                                                                            #
//# M3D_set_3D_doppler_factor                                                  #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_doppler_factor (F32 factor)
{
  API_lock();

  if (lp3DLISTENER)
  {
    lp3DLISTENER->SetDopplerFactor( factor, DS3D_IMMEDIATE);
  }

  API_unlock();
}

//##############################################################################
//#                                                                            #
//# M3D_3D_doppler_factor                                                      #
//#                                                                            #
//##############################################################################

static F32       AILEXPORT M3D_3D_doppler_factor (void)
{
   F32 result = 1.0F;

   API_lock();

   if (lp3DLISTENER)
   {
     lp3DLISTENER->GetDopplerFactor( &result );
   }

   API_unlock();

   return( result );
}


//##############################################################################
//#                                                                            #
//# M3D_set_3D_distance_factor                                                 #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_distance_factor (F32 factor)
{
  API_lock();

  if (lp3DLISTENER)
  {
    lp3DLISTENER->SetDistanceFactor( factor, DS3D_IMMEDIATE);
  }

  API_unlock();
}

//##############################################################################
//#                                                                            #
//# M3D_3D_distance_factor                                                     #
//#                                                                            #
//##############################################################################

static F32       AILEXPORT M3D_3D_distance_factor (void)
{
   F32 result = 1.0F;

   API_lock();

   if (lp3DLISTENER)
   {
     lp3DLISTENER->GetDistanceFactor( &result );
   }

   API_unlock();

   return( result );
}


//##############################################################################
//#                                                                            #
//# M3D_3D_sample_obstruction                                                  #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_obstruction (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->obstruction;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_occlusion                                                    #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_occlusion (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->occlusion;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_exclusion                                                    #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_exclusion (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->exclusion;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_cone                                                         #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_sample_cone      (H3DSAMPLE samp,
                                              F32 FAR* inner_angle,
                                              F32 FAR* outer_angle,
                                              F32 FAR* outer_volume   )
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   if (inner_angle)
     *inner_angle=S->inner_angle;
   if (outer_angle)
     *outer_angle=S->outer_angle;
   if (outer_volume)
     *outer_volume=S->outer_volume;
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_cone                                                     #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_cone     (H3DSAMPLE samp, //)
                                                 F32       inner_angle,
                                                 F32       outer_angle,
                                                 F32       outer_volume)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->inner_angle = inner_angle;
   S->outer_angle = inner_angle;
   S->outer_volume = outer_volume;

   if (S->voice != NULL)
   {
      DS_set_cone(S);
   }
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_obstruction                                              #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_obstruction(H3DSAMPLE samp, //)
                                                   F32       obstruction)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->obstruction = obstruction;

   if (S->voice != NULL)
   {
#ifdef EAX2
     EAX2_Sample_Set_int(S,DSPROPERTY_EAXBUFFER_OBSTRUCTION,(S32)linear_to_dB(1.0F-obstruction));
#else
     DS_set_volume(S);
#endif
   }
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_occlusion                                                #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_occlusion(H3DSAMPLE samp, //)
                                                 F32       occlusion)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->occlusion = occlusion;

   if (S->voice != NULL)
   {
#ifdef EAX2
     EAX2_Sample_Set_int(S,DSPROPERTY_EAXBUFFER_OCCLUSION,(S32)linear_to_dB(1.0F-occlusion));
#else
     DS_set_volume(S);
#endif
   }
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_exclusion                                                #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_exclusion(H3DSAMPLE samp, //)
                                                 F32       exclusion)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->exclusion = exclusion;

   if (S->voice != NULL)
   {
#ifdef EAX3
     EAX2_Sample_Set_int(S,DSPROPERTY_EAXBUFFER_EXCLUSION,(S32)linear_to_dB(1.0F-exclusion));
#else
     DS_set_volume(S);
#endif
   }
}

//############################################################################
//#                                                                          #
//# Return floating-point type as unsigned long DWORD (without actually      #
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
//# Return signed long DWORD as single-precision float (without actually     #
//# converting the value)                                                    #
//#                                                                          #
//############################################################################

static F32 long_as_float(S32 integer)
{
   static F32 val;

   *(S32 FAR *) (&val) = integer;

   return val;
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_EOS                                                             #
//#                                                                            #
//##############################################################################

static AIL3DSAMPLECB AILEXPORT M3D_set_3D_EOS     (H3DSAMPLE client,
                                            H3DSAMPLE samp,
                                            AIL3DSAMPLECB eos)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  AIL3DSAMPLECB prev=S->eos;

  S->eos=eos;

  S->clientH3D=client;

  return prev;
}

//############################################################################
//#                                                                          #
//# Initialize sample                                                        #
//#                                                                          #
//############################################################################

static void init_sample(SAMPLE3D FAR *S)
{
   //
   // Initialize only the application-controlled fields, leaving DS/DS3D hardware buffer
   // information intact for possible reuse
   //

   S->type            =  IS_SAMPLE;

   S->start           =  NULL;
   S->len             =  0;
   S->pos             =  0;
   S->done            =  0;

   S->buffers_past_end = 0;

   S->loop_count      =  1;
   S->loop_start      =  0;
   S->loop_end        = -1;

   S->volume           =  1.0f;
   S->playback_rate    =  22050;
   S->chans            =  1;
   S->bytes_per_sample =  2;

   S->status          =  SMP_DONE;

   S->auto_update     =  0;

   S->previous_position = (U32) -1;
}

//############################################################################
//#                                                                          #
//# Cancel all pending events and return as soon as no events are being      #
//# processed                                                                #
//#                                                                          #
//############################################################################

static void sleep_sample(SAMPLE3D FAR *S)
{
#if 1
  while ( 1 )
  {   
    U32 v = incnoints( S );
    if ( v == 0 )
      break;
    decnoints( S );
    Sleep( 1 );
  } 
#else
   incnoints(S);

   AIL_unlock_mutex();

   while(S->noints!=1)
   {
     Sleep(1);
   }

   AIL_lock_mutex();
#endif
}

//############################################################################
//#                                                                          #
//# Terminate playing sample immediately, aborting all pending events        #
//#                                                                          #
//# Do NOT call from within background thread -- will cause deadlocks due    #
//# to sleep_sample() call!                                                  #
//#                                                                          #
//############################################################################

static void reset_sample_voice(SAMPLE3D FAR *S, //)
                               BOOL          flush_with_silence)
{
   //
   // Put sample to sleep
   //

   sleep_sample(S);

   //
   // Mark sample as DONE to inhibit position notifications
   //

   S->status = SMP_DONE;

   //
   // Reset sample buffer state
   //

   reset_voice_state(S, flush_with_silence);

   //
   // Resume normal sample thread execution
   //

   wake_sample(S);
}

static int GoodGetCurrentPosition( LPDIRECTSOUNDBUFFER b, DWORD * play, DWORD * write )
{

  while ( 1 )
  {
    DWORD p1, w1, p2, w2;
    
    // get first position
    if (!(SUCCEEDED ( b->GetCurrentPosition( &p1, &w1 ) ) ) )
    {
      return( 0 );
    }

    // acts a delay 
    b->GetCurrentPosition( &p2, &w2 );

    // get next position
    b->GetCurrentPosition( &p2, &w2 );

    // make sure it hasn't wrapped oddly (meaning that a backwards read position may have occurred)
    if ( ( ( p2 - p1 ) < 1024 ) && ( ( w2 - w1 ) < 1024 ) )
    {
      *play = p2;
      *write = w2;
      return( 1 );
    }
  }
}

//############################################################################
//#                                                                          #
//# Callback function to simulate IDirectSoundNotify interface (broken on    #
//# some drivers / DirectSound3D providers)                                  #
//#                                                                          #
//############################################################################

static void AILCALLBACK notify_timer(U32 user)
{
   S32 i;
   S32 docb;

   for (i=0; i < avail_samples; i++)
   {
      SAMPLE3D FAR *S = &samples[i];

      incnoints(S);

      if (S->noints!=1)
      {
        decnoints(S);
        continue;
      }

      if (((S->status&255) != SMP_PLAYING) || (S->voice->lpdsb == NULL))
      {
         S->previous_position = (U32) -1;
            decnoints(S);
         continue;
      }

      docb=0;

      API_lock();

      DWORD p,w;

      if (!GoodGetCurrentPosition(S->voice->lpdsb,&p,&w))
      {
         API_unlock();

         S->previous_position = (U32) -1;
         decnoints(S);
         continue;
      }

      API_unlock();


      U32 timer=AIL_ms_count();

      if ( ( p != S->previous_position ) || ( w != S->previous_write_position ) )
      {
        S->last_move_time = timer;
        S->previous_write_position = w;
      }
      else
      {         
        // if neither of the positions have moved in 128 ms - figure we
        //   hit the Creative bug and reset playback
        if ( ( timer - S->last_move_time ) > 128 )
        {
          API_lock();
          S->voice->lpdsb->Play(0,0,0);
          S->voice->lpdsb->Stop();
          S->voice->lpdsb->Play(0,0,DSBPLAY_LOOPING);
          API_unlock();

          {
            char buf[256]; wsprintf(buf,"Warning: DirectSound 3D playback reset due to non-moving playback cursor (buggy sound driver) time: %i play: %i write: %i.\n",timer,p,w);
            OutputDebugString( buf );
          }

          S->last_move_time = timer;
        }
      }

      if (S->previous_position != (U32) -1)
      {
         if (p < S->previous_position)
         {
            //
            // Cursor has wrapped past end of buffer
            //

            diag_printf("%X BUFFER 2\n",S);

            if ((S->status&255) == SMP_PLAYING)
            {
               docb|=DS3D_stream_to_buffer(S, 1, BUFF_SIZE);
            }
         }
         else if ((p > BUFF_SIZE) && (S->previous_position <= BUFF_SIZE))
         {
            //
            // Cursor has moved into second half of buffer since previous
            // call
            //

            diag_printf("%X BUFFER 1\n",S);

            if ((S->status&255) == SMP_PLAYING)
            {
               docb|=DS3D_stream_to_buffer(S, 0, BUFF_SIZE);
            }
         }
      }

      S->previous_position = p;

      decnoints(S);

      if (S->eos)
      {
        if (docb)
          S->eos(S->clientH3D);
        else
        {
          if ((S->status==SMP_PLAYING) && (S->pos) && (S->pos == S->len))
            if (timer>=S->lastblockdone)
            {
              S->status=SMP_PLAYING|256;
              S->eos(S->clientH3D);
            }
        }
      }

   }
}

//############################################################################
//#                                                                          #
//# Retrieve a standard RIB provider attribute by index                      #
//#                                                                          #
//############################################################################

static U32 AILEXPORT PROVIDER_query_attribute (HATTRIB index)
{
   switch ((ATTRIB) index)
      {
      case PROVIDER_NAME:             return (U32) PROVIDER_NAME_STR;
      case PROVIDER_VERSION:          return 0x102;

      case MAX_SUPPORTED_SAMPLES:     return avail_samples;
      case MUTE_AT_MAX:               return( mute_at_max );

#ifdef EAX3D
      case EAX_ENVIRONMENT:           return( (!lpPropertySet)? 0: (U32) EAX_GetEnvironment());
      case EAX_EFFECT_VOLUME:         return( (!lpPropertySet)? 0: (U32) float_as_long(EAX_GetVolume()));
      case EAX_DECAY_TIME:            return( (!lpPropertySet)? 0: (U32) float_as_long(EAX_GetDecayTime()));
      case EAX_DAMPING:               return( (!lpPropertySet)? 0: (U32) float_as_long(EAX_GetDamping()));
          case EAX_ALL_PARAMETERS:    return( (!lpPropertySet)? 0: (U32) EAX_GetAll());

#ifdef EAX2
      case EAX_ENVIRONMENT_SIZE:      return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE)));
      case EAX_ENVIRONMENT_DIFFUSION: return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION)));
      case EAX_ROOM:                  return( (!lpPropertySet)? 0: EAX2_Get_int(DSPROPERTY_EAXLISTENER_ROOM));
      case EAX_ROOM_HF:               return( (!lpPropertySet)? 0: EAX2_Get_int(DSPROPERTY_EAXLISTENER_ROOMHF));
      case EAX_DECAY_HF_RATIO:        return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_DECAYHFRATIO)));
      case EAX_REFLECTIONS:           return( (!lpPropertySet)? 0: EAX2_Get_int(DSPROPERTY_EAXLISTENER_REFLECTIONS));
      case EAX_REFLECTIONS_DELAY:     return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY)));
      case EAX_REVERB:                return( (!lpPropertySet)? 0: EAX2_Get_int(DSPROPERTY_EAXLISTENER_REVERB));
      case EAX_REVERB_DELAY:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_REVERBDELAY)));
      case EAX_ROOM_ROLLOFF:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR)));
      case EAX_AIR_ABSORPTION:        return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF)));
      case EAX_FLAGS:                 return( (!lpPropertySet)? 0: EAX2_Get_int(DSPROPERTY_EAXLISTENER_FLAGS));

#ifdef EAX3
      case EAX_ROOM_LF:               return( (!lpPropertySet)? 0: EAX2_Get_int(DSPROPERTY_EAXLISTENER_ROOMLF));
      case EAX_DECAY_LF_RATIO:        return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_DECAYLFRATIO)));
      case EAX_REFLECTIONS_PAN:       return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_REFLECTIONSPAN)));
      case EAX_REVERB_PAN:            return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_REVERBPAN)));
      case EAX_ECHO_TIME:             return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_ECHOTIME)));
      case EAX_ECHO_DEPTH:            return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_ECHODEPTH)));
      case EAX_MODULATION_TIME:       return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_MODULATIONTIME)));
      case EAX_MODULATION_DEPTH:      return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_MODULATIONDEPTH)));
      case EAX_HF_REFERENCE:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_HFREFERENCE)));
      case EAX_LF_REFERENCE:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_Get_float(DSPROPERTY_EAXLISTENER_LFREFERENCE)));
#endif

#endif

#endif
      }

   return 0;
}

//############################################################################
//#                                                                          #
//# Return M3D error text                                                    #
//#                                                                          #
//############################################################################

static C8 FAR *       AILEXPORT M3D_error       (void)
{
   if (!strlen(M3D_error_text))
      {
      return NULL;
      }

   return M3D_error_text;
}

//############################################################################
//#                                                                          #
//# Initialize M3D stream decoder                                            #
//#                                                                          #
//############################################################################

static M3DRESULT AILEXPORT M3D_startup     (void)
{
   if (M3D_started++)
      {
      strcpy(M3D_error_text,"Already started");
      return M3D_ALREADY_STARTED;
      }

   //
   // Init static prefs/attributes
   //

   M3D_error_text[0] = 0;

   return M3D_NOERR;
}

//############################################################################
//#                                                                          #
//# Shut down M3D stream decoder                                             #
//#                                                                          #
//############################################################################

static M3DRESULT      AILEXPORT M3D_shutdown    (void)
{
   if (!M3D_started)
      {
      strcpy(M3D_error_text,"Not initialized");
      return M3D_NOT_INIT;
      }

   --M3D_started;

   return M3D_NOERR;
}

#ifdef EAX4

GUID const * guids[ 4 ] = { &EAX_REVERB_EFFECT, &EAX_CHORUS_EFFECT, 0, 0 };
GUID const * fxslots[ 4 ] = { &EAXPROPERTYID_EAX40_FXSlot0, &EAXPROPERTYID_EAX40_FXSlot1, &EAXPROPERTYID_EAX40_FXSlot2, &EAXPROPERTYID_EAX40_FXSlot3 };
S32 slotvolumes[ 4 ] = { 0, 0, 0, 0 };

static S32 get_eax4( GUID const * guid, U32 prop, U32 isize, void * value )
{
  U32 slot;
  BOOL v;

  if ( lpPropertySet == 0 )
  {
    return( 0 );
  }

  slot = ( (U32*) value )[ 0 ];
 
  if ( slot >= 4 )
  {
    return( 0 );
  }

  if ( guids[ slot ] != guid )
  {
    return( 0 );
  }

  ( (U32*) value )[ 1 ] = slotvolumes[ 4 ];

  DWORD size=0;

  API_lock();
  v = SUCCEEDED( lpPropertySet->Get( *fxslots[ slot ],
                                     prop,
                                     NULL,0,
                                     ( (S32*) value ) + 2, isize, &size ) );

  API_unlock();

  return( v ? size : 0 );
}


static S32 set_eax4( GUID const * guid, U32 prop, U32 isize, void * value )
{
  U32 slot;
  BOOL v;

  if ( lpPropertySet == 0 )
  {
    return( 0 );
  }

  slot = ( (U32*) value )[ 0 ];

  if ( slot >= 4 )
  {
    return( 0 );
  }

  if ( guids[ slot ] != guid )
  {
    EAXFXSLOTPROPERTIES p;

    p.guidLoadEffect = *guid;
    p.lLock = EAXFXSLOT_UNLOCKED;
    p.lVolume = ( (U32*) value )[ 1 ];
    p.ulFlags = ( prop == EAXREVERB_ALLPARAMETERS ) ? EAXFXSLOTFLAGS_ENVIRONMENT : 0;

    API_lock();

    v = SUCCEEDED( lpPropertySet->Set( *fxslots[ slot ],
                                       EAXFXSLOT_ALLPARAMETERS,
                                       NULL,0,
                                       &p, sizeof( p ) ) );
    
    API_unlock();

    if ( !v )
    {
      return( 0 );
    }

    slotvolumes[ slot ] = ( (U32*) value )[ 1 ];
    guids[ slot ] = guid;
  }
  else
  {
    if ( slotvolumes[ slot ] != ( (U32*) value )[ 1 ] )
    {
      API_lock();

      v = SUCCEEDED( lpPropertySet->Set( *fxslots[ slot ],
                                         EAXFXSLOT_VOLUME,
                                         NULL,0,
                                         ( (U32*) value ) + 1, 4 ) );

      API_unlock();

      if ( !v )
      {
        return( 0 );
      }

      slotvolumes[ slot ] = ( (U32*) value )[ 1 ];
    }
  }

  API_lock();
  v = SUCCEEDED( lpPropertySet->Set( *fxslots[ slot ],
                                     prop,
                                     NULL,0,
                                     ( (S32*) value ) + 2, isize ) );

  API_unlock();

  return( v ? 1 : 0 );
}


static S32 get_eax4_sample_volumes( SAMPLE3D FAR *S, void * value )
{
  EAXSOURCESENDPROPERTIES all[ EAX_MAX_FXSLOTS ];
  EAXSOURCEOCCLUSIONSENDPROPERTIES occ[ EAX_MAX_FXSLOTS ];
  EAXACTIVEFXSLOTS active;
  U32 i;
  EAX_SAMPLE_SLOT_VOLUMES * vol = (EAX_SAMPLE_SLOT_VOLUMES*) value;

  if ( S->voice->lpPropertySet == 0 )
  {
    return( 0 );
  }

  BOOL v;
  DWORD size1 = 0;
  DWORD size2 = 0;
  DWORD size3 = 0;

  API_lock();
  v = SUCCEEDED( S->voice->lpPropertySet->Get( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_ACTIVEFXSLOTID,
                      NULL,0,
                      &active, sizeof( active ), &size1  ) );

  v &= SUCCEEDED( S->voice->lpPropertySet->Get( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_SENDPARAMETERS,
                      NULL,0,
                      &all, sizeof( all ), &size2  ) );

  v &= SUCCEEDED( S->voice->lpPropertySet->Get( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_OCCLUSIONSENDPARAMETERS,
                      NULL,0,
                      &occ, sizeof( occ ), &size3  ) );
  API_unlock();

  if ( ( !v ) || ( size1 != sizeof( active ) ) || ( size2 != sizeof( all ) ) || ( size3 != sizeof( occ ) ) )
  {
    return( 0 );
  }

  vol->NumVolumes = 0;
  for ( i = 0 ; i < EAX_MAX_ACTIVE_FXSLOTS ; i++ )
  {
    U32 slot;

    if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot0 ) slot = 0;
    else if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot1 ) slot = 1;
    else if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot2 ) slot = 2;
    else if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot3 ) slot = 3;
    else 
      continue;

    ++vol->NumVolumes;
    vol->volumes[ i ].Slot = slot;
    vol->volumes[ i ].Send = all[ slot ].lSend;
    vol->volumes[ i ].SendHF = all[ slot ].lSendHF;
    vol->volumes[ i ].Occlusion = occ[ slot ].lOcclusion;
    vol->volumes[ i ].OcclusionLFRatio = occ[ slot ].flOcclusionLFRatio;
    vol->volumes[ i ].OcclusionRoomRatio = occ[ slot ].flOcclusionRoomRatio;
    vol->volumes[ i ].OcclusionDirectRatio = occ[ slot ].flOcclusionDirectRatio;
  }

  return( 1 );
}


static S32 set_eax4_sample_volumes( SAMPLE3D FAR *S, void * value )
{
  EAXSOURCESENDPROPERTIES all[ EAX_MAX_FXSLOTS ];
  EAXSOURCEOCCLUSIONSENDPROPERTIES occ[ EAX_MAX_FXSLOTS ];
  EAXACTIVEFXSLOTS active;
  BOOL v;
  U32 i;
  EAX_SAMPLE_SLOT_VOLUMES * vol = (EAX_SAMPLE_SLOT_VOLUMES*) value;

  if ( S->voice->lpPropertySet == 0 )
  {
    return( 0 );
  }

  if ( vol->NumVolumes > 2 )
  {
    return( 0 );
  }

  for ( i = 0 ; i < 4 ; i++ )
  {
    all[ i ].guidReceivingFXSlotID = *fxslots[ i ];
    all[ i ].lSend = 0;
    all[ i ].lSendHF = 0;
    occ[ i ].guidReceivingFXSlotID = *fxslots[ i ];
    occ[ i ].lOcclusion = 0;
    occ[ i ].flOcclusionLFRatio = 0.25f;
    occ[ i ].flOcclusionRoomRatio = 1.5f;
    occ[ i ].flOcclusionDirectRatio = 1.0f;
  }

  for ( i = 0 ; i < vol->NumVolumes ; i++ )
  {
    if ( ( vol->volumes[ i ].Slot < 0 ) || ( vol->volumes[ i ].Slot > 3 ) )
    {
      return( 0 );
    }

    all[ vol->volumes[ i ].Slot ].lSend = vol->volumes[ i ].Send;
    all[ vol->volumes[ i ].Slot ].lSendHF = vol->volumes[ i ].SendHF;
    occ[ vol->volumes[ i ].Slot ].lOcclusion = vol->volumes[ i ].Occlusion;
    occ[ vol->volumes[ i ].Slot ].flOcclusionLFRatio = vol->volumes[ i ].OcclusionLFRatio;
    occ[ vol->volumes[ i ].Slot ].flOcclusionRoomRatio = vol->volumes[ i ].OcclusionRoomRatio;
    occ[ vol->volumes[ i ].Slot ].flOcclusionDirectRatio = vol->volumes[ i ].OcclusionDirectRatio;

    active.guidActiveFXSlots[ i ] = *fxslots[ vol->volumes[ i ].Slot ];
  }

  for ( i = vol->NumVolumes ; i < EAX_MAX_ACTIVE_FXSLOTS ; i++ )
  {
    active.guidActiveFXSlots[ i ] = EAX_NULL_GUID;
  }
  
  API_lock();
  v = SUCCEEDED( S->voice->lpPropertySet->Set( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_ACTIVEFXSLOTID,
                      NULL,0,
                      &active, sizeof( active ) ) );

  v &= SUCCEEDED( S->voice->lpPropertySet->Set( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_SENDPARAMETERS,
                      NULL,0,
                      &all, sizeof( all ) ) );

  v &= SUCCEEDED( S->voice->lpPropertySet->Set( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_OCCLUSIONSENDPARAMETERS,
                      NULL,0,
                      &occ, sizeof( occ ) ) );

  API_unlock();

  return( v ? 1 : 0 );
}

#endif

//############################################################################
//#                                                                          #
//# Set provider preference value, returning previous setting                #
//#                                                                          #
//############################################################################

static S32 AILEXPORT M3D_set_provider_preference (HATTRIB    preference, //)
                                           void FAR * value)
{
   S32 prev = -1;

   switch ((ATTRIB) preference)
      {

      case MUTE_AT_MAX:
        mute_at_max=*(U32*)value;
        break;

#ifdef EAX3D

      //
      // Preferences
      //

      case EAX_ENVIRONMENT:

         if (!lpPropertySet)
            {
            return 0;
            }
         else
            {
            prev = (S32) EAX_GetEnvironment();
            EAX_SetEnvironment(*(U32 FAR*)value);
            }
         break;

      case EAX_EFFECT_VOLUME:

         if (!lpPropertySet)
            {
            return 0;
            }
         else
            {
            prev = (S32) float_as_long(EAX_GetVolume());
            EAX_SetVolume(*(F32 FAR*)value);
            }
         break;

      case EAX_DECAY_TIME:

         if (!lpPropertySet)
            {
            return 0;
            }
         else
            {
            prev = (S32) float_as_long(EAX_GetDecayTime());
            EAX_SetDecayTime(*(F32 FAR*)value);
            }
         break;

      case EAX_DAMPING:

         if (!lpPropertySet)
            {
            return 0;
            }
         else
            {
            prev = (S32) float_as_long(EAX_GetDamping());
            EAX_SetDamping(*(F32 FAR*)value);
            }
         break;

          case EAX_ALL_PARAMETERS:

                  if (!lpPropertySet)
                        {
                        return 0;
                        }
                  else
                        {
                        prev = (S32) 1;
                        EAX_SetAll(value);
                        }

#ifdef EAX2
      case EAX_ENVIRONMENT_SIZE:      return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE,*(F32 FAR*)value)));
      case EAX_ENVIRONMENT_DIFFUSION: return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION,*(F32 FAR*)value)));
      case EAX_ROOM:                  return( (!lpPropertySet)? 0: EAX2_GetSet_int(DSPROPERTY_EAXLISTENER_ROOM,*(U32 FAR*)value));
      case EAX_ROOM_HF:               return( (!lpPropertySet)? 0: EAX2_GetSet_int(DSPROPERTY_EAXLISTENER_ROOMHF,*(U32 FAR*)value));
      case EAX_DECAY_HF_RATIO:        return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_DECAYHFRATIO,*(F32 FAR*)value)));
      case EAX_REFLECTIONS:           return( (!lpPropertySet)? 0: EAX2_GetSet_int(DSPROPERTY_EAXLISTENER_REFLECTIONS,*(U32 FAR*)value));
      case EAX_REFLECTIONS_DELAY:     return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY,*(F32 FAR*)value)));
      case EAX_REVERB:                return( (!lpPropertySet)? 0: EAX2_GetSet_int(DSPROPERTY_EAXLISTENER_REVERB,*(U32 FAR*)value));
      case EAX_REVERB_DELAY:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_REVERBDELAY,*(F32 FAR*)value)));
      case EAX_ROOM_ROLLOFF:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR,*(F32 FAR*)value)));
      case EAX_AIR_ABSORPTION:        return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF,*(F32 FAR*)value)));
      case EAX_FLAGS:                 return( (!lpPropertySet)? 0: EAX2_GetSet_int(DSPROPERTY_EAXLISTENER_FLAGS,*(U32 FAR*)value));

#ifdef EAX3
      case EAX_ROOM_LF:               return( (!lpPropertySet)? 0: EAX2_GetSet_int(DSPROPERTY_EAXLISTENER_ROOMLF,*(U32 FAR*)value));
      case EAX_DECAY_LF_RATIO:        return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_DECAYLFRATIO,*(F32 FAR*)value)));
      case EAX_REFLECTIONS_PAN:       return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_REFLECTIONSPAN,*(F32 FAR*)value)));
      case EAX_REVERB_PAN:            return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_REVERBPAN,*(F32 FAR*)value)));
      case EAX_ECHO_TIME:             return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_ECHOTIME,*(F32 FAR*)value)));
      case EAX_ECHO_DEPTH:            return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_ECHODEPTH,*(F32 FAR*)value)));
      case EAX_MODULATION_TIME:       return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_MODULATIONTIME,*(F32 FAR*)value)));
      case EAX_MODULATION_DEPTH:      return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_MODULATIONDEPTH,*(F32 FAR*)value)));
      case EAX_HF_REFERENCE:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_HFREFERENCE,*(F32 FAR*)value)));
      case EAX_LF_REFERENCE:          return( (!lpPropertySet)? 0: (U32) float_as_long(EAX2_GetSet_float(DSPROPERTY_EAXLISTENER_LFREFERENCE,*(F32 FAR*)value)));

#ifdef EAX4
      case EAX_AUTO_GAIN_P:  return( get_eax4( &EAX_AGCCOMPRESSOR_EFFECT, EAXAGCCOMPRESSOR_ALLPARAMETERS, sizeof(EAXAGCCOMPRESSORPROPERTIES), value ) );
      case EAX_AUTOWAH_P:    return( get_eax4( &EAX_AUTOWAH_EFFECT, EAXAUTOWAH_ALLPARAMETERS, sizeof(EAXAUTOWAHPROPERTIES), value ) );
      case EAX_CHORUS_P:     return( get_eax4( &EAX_CHORUS_EFFECT, EAXCHORUS_ALLPARAMETERS, sizeof(EAXCHORUSPROPERTIES), value ) );
      case EAX_DISTORTION_P: return( get_eax4( &EAX_DISTORTION_EFFECT, EAXDISTORTION_ALLPARAMETERS, sizeof(EAXDISTORTIONPROPERTIES), value ) );
      case EAX_ECHO_P:       return( get_eax4( &EAX_ECHO_EFFECT, EAXECHO_ALLPARAMETERS, sizeof(EAXECHOPROPERTIES), value ) );
      case EAX_EQUALIZER_P:  return( get_eax4( &EAX_EQUALIZER_EFFECT, EAXEQUALIZER_ALLPARAMETERS, sizeof(EAXEQUALIZERPROPERTIES), value ) );
      case EAX_FLANGER_P:    return( get_eax4( &EAX_FLANGER_EFFECT, EAXFLANGER_ALLPARAMETERS, sizeof(EAXFLANGERPROPERTIES), value ) );
      case EAX_FSHIFTER_P:   return( get_eax4( &EAX_FREQUENCYSHIFTER_EFFECT, EAXFREQUENCYSHIFTER_ALLPARAMETERS, sizeof(EAXFREQUENCYSHIFTERPROPERTIES), value ) );
      case EAX_VMORPHER_P:   return( get_eax4( &EAX_VOCALMORPHER_EFFECT, EAXVOCALMORPHER_ALLPARAMETERS, sizeof(EAXVOCALMORPHERPROPERTIES), value ) );
      case EAX_PSHIFTER_P:   return( get_eax4( &EAX_PITCHSHIFTER_EFFECT, EAXPITCHSHIFTER_ALLPARAMETERS, sizeof(EAXPITCHSHIFTERPROPERTIES), value ) );
      case EAX_RMODULATOR_P: return( get_eax4( &EAX_RINGMODULATOR_EFFECT, EAXRINGMODULATOR_ALLPARAMETERS, sizeof(EAXRINGMODULATORPROPERTIES), value ) );
      case EAX_REVERB_P:     return( get_eax4( &EAX_REVERB_EFFECT, EAXREVERB_ALLPARAMETERS, sizeof(EAXREVERBPROPERTIES), value ) );

      case EAX_AUTO_GAIN_S_P:  return( set_eax4( &EAX_AGCCOMPRESSOR_EFFECT, EAXAGCCOMPRESSOR_ALLPARAMETERS, sizeof(EAXAGCCOMPRESSORPROPERTIES), value ) );
      case EAX_AUTOWAH_S_P:    return( set_eax4( &EAX_AUTOWAH_EFFECT, EAXAUTOWAH_ALLPARAMETERS, sizeof(EAXAUTOWAHPROPERTIES), value ) );
      case EAX_CHORUS_S_P:     return( set_eax4( &EAX_CHORUS_EFFECT, EAXCHORUS_ALLPARAMETERS, sizeof(EAXCHORUSPROPERTIES), value ) );
      case EAX_DISTORTION_S_P: return( set_eax4( &EAX_DISTORTION_EFFECT, EAXDISTORTION_ALLPARAMETERS, sizeof(EAXDISTORTIONPROPERTIES), value ) );
      case EAX_ECHO_S_P:       return( set_eax4( &EAX_ECHO_EFFECT, EAXECHO_ALLPARAMETERS, sizeof(EAXECHOPROPERTIES), value ) );
      case EAX_EQUALIZER_S_P:  return( set_eax4( &EAX_EQUALIZER_EFFECT, EAXEQUALIZER_ALLPARAMETERS, sizeof(EAXEQUALIZERPROPERTIES), value ) );
      case EAX_FLANGER_S_P:    return( set_eax4( &EAX_FLANGER_EFFECT, EAXFLANGER_ALLPARAMETERS, sizeof(EAXFLANGERPROPERTIES), value ) );
      case EAX_FSHIFTER_S_P:   return( set_eax4( &EAX_FREQUENCYSHIFTER_EFFECT, EAXFREQUENCYSHIFTER_ALLPARAMETERS, sizeof(EAXFREQUENCYSHIFTERPROPERTIES), value ) );
      case EAX_VMORPHER_S_P:   return( set_eax4( &EAX_VOCALMORPHER_EFFECT, EAXVOCALMORPHER_ALLPARAMETERS, sizeof(EAXVOCALMORPHERPROPERTIES), value ) );
      case EAX_PSHIFTER_S_P:   return( set_eax4( &EAX_PITCHSHIFTER_EFFECT, EAXPITCHSHIFTER_ALLPARAMETERS, sizeof(EAXPITCHSHIFTERPROPERTIES), value ) );
      case EAX_RMODULATOR_S_P: return( set_eax4( &EAX_RINGMODULATOR_EFFECT, EAXRINGMODULATOR_ALLPARAMETERS, sizeof(EAXRINGMODULATORPROPERTIES), value ) );
      case EAX_REVERB_S_P:     return( set_eax4( &EAX_REVERB_EFFECT, EAXREVERB_ALLPARAMETERS, sizeof(EAXREVERBPROPERTIES), value ) );
#endif

#endif

#endif

#endif
      }

   return prev;
}

//##############################################################################
//#                                                                            #
//# M3D_allocate_3D_sample_handle                                              #
//#                                                                            #
//##############################################################################

static H3DSAMPLE  AILEXPORT M3D_allocate_3D_sample_handle (void)
{
   //
   // Look for an unallocated sample structure
   //

   S32 i;

   for (i=0; i < avail_samples; i++)
      {
      if (samples[i].status == SMP_FREE)
         {
         break;
         }
      }

   //
   // If all structures in use, return NULL
   //
   // (Unlike in the 2D case, this does not set the MSS error string, since
   // the application has no control or knowledge of the number of available
   // handles from various 3D providers)
   //

   if (i == avail_samples)
      {
      return NULL;
      }

   SAMPLE3D FAR *S = &samples[i];

   //
   // Initialize sample to default (SMP_DONE) status with nominal
   // sample attributes
   //

   init_sample(S);

   S->eos = 0;

   //
   // Assign system voice to sample
   // If this fails, then the sample can't be used!
   //
   // Voice will be assigned with init_sample()'s default attributes (16-bit mono).
   // Subsequent attempts to reuse the sample with different attributes will result in
   // release and reallocation of the voice
   //

   if (S->voice == NULL)
      {
      S32 result = DS_assign_voice_to_sample(S);

      if (!result)
         {
         S->status = SMP_FREE;
         return NULL;
         }

// always reset M3D defaults on allocation      if (result == 2)
         {
         //
         // Voice was just allocated -- set its default 3D properties,
         // along with those of the H3DSAMPLE
         //

         set_M3D_defaults((H3DSAMPLE) S);
         }
      }

   //
   // Return valid sample
   //

   return (H3DSAMPLE) S;
}

//##############################################################################
//#                                                                            #
//# M3D_release_3D_sample_handle                                               #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_release_3D_sample_handle (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   reset_sample_voice(S, FALSE);

   //
   // Mark sample available for immediate reallocation
   //
   // We don't free the voice -- it's reused whenever possible
   //

   S->status = SMP_FREE;
}

//##############################################################################
//#                                                                            #
//# M3D_start_3D_sample                                                        #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_start_3D_sample         (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   //
   // Make sure valid sample data exists
   //

   if ((S->len   == 0) ||
       (S->start == NULL))
      {
      return;
      }

   //
   // Initialize sample voice (without flushing with silence)
   //

   reset_sample_voice(S, FALSE);

   //
   // Rewind source buffer to beginning
   //

   S->pos = 0;

   //
   // Activate sample and wait for command to take effect
   //

   sleep_sample(S);

   DS3D_stream_to_buffer(S, 0, BUFF_SIZE);
   DS3D_stream_to_buffer(S, 1, BUFF_SIZE);

   DS_start_secondary(S);

   S->previous_position = -1;

   S->status = SMP_PLAYING;

   wake_sample(S);
}

//##############################################################################
//#                                                                            #
//# M3D_stop_3D_sample                                                         #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_stop_3D_sample          (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;
   if (S->status != SMP_PLAYING)
      {
      return;
      }

   sleep_sample(S);

   S->status = SMP_STOPPED;

   reset_voice_state(S, FALSE);

   wake_sample(S);
}

//##############################################################################
//#                                                                            #
//# M3D_resume_3D_sample                                                       #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_resume_3D_sample        (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if ((S->status == SMP_FREE) || (S->status == SMP_PLAYING))
      {
      return;
      }

   //
   // Make sure valid sample data exists
   //

   if ((S->len   == 0) ||
       (S->start == NULL))
      {
      return;
      }

   //
   // Initialize sample voice
   //

   reset_sample_voice(S, FALSE);

   //
   // Activate sample and wait for command to take effect
   //

   sleep_sample(S);

   DS3D_stream_to_buffer(S, 0, BUFF_SIZE);
   DS3D_stream_to_buffer(S, 1, BUFF_SIZE);

   DS_start_secondary(S);

   S->previous_position = -1;

   S->status = SMP_PLAYING;

   wake_sample(S);
}

//##############################################################################
//#                                                                            #
//# M3D_end_3D_sample                                                          #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_end_3D_sample        (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   reset_sample_voice(S, FALSE);

   //
   // Mark sample available for immediate reuse
   //

   S->status = SMP_DONE;
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_volume                                                   #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_volume(H3DSAMPLE samp, //)
                                              F32       volume)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->volume = volume; // convert to linear

   DS_set_volume(S);
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_playback_rate                                            #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_playback_rate    (H3DSAMPLE samp, //)
                                                         S32       playback_rate)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->playback_rate = playback_rate;
   DS_set_frequency(S);
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_offset                                                   #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_offset    (H3DSAMPLE samp, //)
                                                  U32       offset)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->pos = offset;
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_loop_count                                               #
//#                                                                            #
//#  1: Single iteration, no looping                                           #
//#  0: Loop indefinitely                                                      #
//#  n: Play sample n times                                                    #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_loop_count(H3DSAMPLE samp, //)
                                                  U32       loops)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->loop_count = loops;
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_loop_block                                               #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_loop_block(H3DSAMPLE samp, //)
                                                  S32       loop_start_offset,
                                                  S32       loop_end_offset)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S32 gran = S->bytes_per_sample;

   S->loop_start = ((loop_start_offset+gran/2) / gran)*gran;
   S->loop_end   = loop_end_offset;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_volume                                                       #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_volume(H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->volume;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_playback_rate                                                #
//#                                                                            #
//##############################################################################

static S32        AILEXPORT M3D_3D_sample_playback_rate        (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->playback_rate;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_status                                                       #
//#                                                                            #
//##############################################################################

static U32        AILEXPORT M3D_3D_sample_status        (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (((S->status&255)==SMP_PLAYING) && (S->pos) && (S->pos == S->len))
     if (AIL_ms_count()>=S->lastblockdone)
       return SMP_DONE;

   return S->status;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_offset                                                       #
//#                                                                            #
//##############################################################################

static U32        AILEXPORT M3D_3D_sample_offset        (H3DSAMPLE     samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->pos;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_length                                                       #
//#                                                                            #
//##############################################################################

static U32        AILEXPORT M3D_3D_sample_length        (H3DSAMPLE     samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->len;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_loop_count                                                   #
//#                                                                            #
//##############################################################################

static U32        AILEXPORT M3D_3D_sample_loop_count    (H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   return S->loop_count;
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_distances                                                #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_sample_distances (H3DSAMPLE samp, //)
                                                  F32       max_dist,
                                                  F32       min_dist)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   S->max_dist = max_dist;
   S->min_dist = min_dist;

   if (S->voice != NULL)
      {
      API_lock();

      S->voice->lpds3db->SetMaxDistance( max_dist, DS3D_DEFERRED);
      S->voice->lpds3db->SetMinDistance( min_dist, DS3D_DEFERRED);

      lp3DLISTENER->CommitDeferredSettings();

      API_unlock();
      }
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_distances                                                    #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_sample_distances     (H3DSAMPLE samp, //)
                                                  F32 FAR * max_dist,
                                                  F32 FAR * min_dist)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   if (max_dist) *max_dist = S->max_dist;
   if (min_dist) *min_dist = S->min_dist;
}

//############################################################################
//#                                                                          #
//# Retrieve a sample attribute by index                                     #
//#                                                                          #
//############################################################################

static U32 AILEXPORT M3D_3D_sample_query_attribute (H3DSAMPLE samp, //)
                                             HATTRIB   index)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   switch ((ATTRIB) index)
      {
      case DIRECTSOUND_SAMPLE_BUFFER:       return( (U32) S->voice->lpds3db );
        
#ifdef EAX3D

      case EAX_EFFECT_REVERBMIX:            return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX_GetReverbMix(S)));
      case EAX_EFFECT_ALL_PARAMETERS:       return( (!lpPropertySet) ? 0: (U32) EAX_GetSampleAll(S));
      case EAX_PROPERTY_SET:
        return (S32) ( lpPropertySet );

#ifdef EAX2
      case EAX_SAMPLE_DIRECT:               return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_DIRECT));
      case EAX_SAMPLE_DIRECT_HF:            return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_DIRECTHF));
      case EAX_SAMPLE_ROOM:                 return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_ROOM));
      case EAX_SAMPLE_ROOM_HF:              return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_ROOMHF));
      case EAX_SAMPLE_OBSTRUCTION:          return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_OBSTRUCTION));
      case EAX_SAMPLE_OBSTRUCTION_LF_RATIO: return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO)));
      case EAX_SAMPLE_OCCLUSION:            return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_OCCLUSION));
      case EAX_SAMPLE_OCCLUSION_LF_RATIO:   return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO)));
      case EAX_SAMPLE_OCCLUSION_ROOM_RATIO: return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO)));
      case EAX_SAMPLE_ROOM_ROLLOFF:         return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR)));
      case EAX_SAMPLE_AIR_ABSORPTION:       return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR)));
      case EAX_SAMPLE_OUTSIDE_VOLUME_HF:    return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF));
      case EAX_SAMPLE_FLAGS:                return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_FLAGS));

#ifdef EAX3
      case EAX_SAMPLE_OCCLUSION_DIRECT_RATIO:return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_OCCLUSIONDIRECTRATIO)));
      case EAX_SAMPLE_EXCLUSION:            return( (!lpPropertySet) ? 0: EAX2_Sample_Get_int(S,DSPROPERTY_EAXBUFFER_EXCLUSION));
      case EAX_SAMPLE_EXCLUSION_LF_RATIO:   return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_EXCLUSIONLFRATIO)));
      case EAX_SAMPLE_DOPPLER:              return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_Get_float(S,DSPROPERTY_EAXBUFFER_DOPPLERFACTOR)));
#endif
#endif
#endif

      }

   return 0;
}

//############################################################################
//#                                                                          #
//# Set provider preference value, returning previous setting                #
//#                                                                          #
//############################################################################

static S32 AILEXPORT M3D_3D_set_sample_preference (H3DSAMPLE  samp, //)
                                            HATTRIB    preference,
                                            void FAR*  value)
{
   S32 prev = -1;

   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return -1;
      }

#ifdef EAX3D

   switch ((ATTRIB) preference)
      {
      //
      // Preferences
      //

      case EAX_EFFECT_REVERBMIX:

         if (!S->voice->lpPropertySet)
            {
            return 0;
            }
         else
            {
            prev = (S32) float_as_long(EAX_GetReverbMix(S));
            EAX_SetReverbMix(S,*(F32 FAR*)value);
            }
         break;

      case EAX_EFFECT_ALL_PARAMETERS:

                 if (!S->voice->lpPropertySet)
                        {
                        return 0;
                        }
                 else
                        {
                        prev = (S32) 1;
                        EAX_SetSampleAll(S,value);
                        }
                 break;

      case EAX_SAMPLE_PROPERTY_SET:
        return (S32) ( S->voice->lpPropertySet );

#ifdef EAX2
      case EAX_SAMPLE_DIRECT:               return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_DIRECT,*(U32 FAR*)value));
      case EAX_SAMPLE_DIRECT_HF:            return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_DIRECTHF,*(U32 FAR*)value));
      case EAX_SAMPLE_ROOM:                 return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_ROOM,*(U32 FAR*)value));
      case EAX_SAMPLE_ROOM_HF:              return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_ROOMHF,*(U32 FAR*)value));
      case EAX_SAMPLE_OBSTRUCTION:          return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_OBSTRUCTION,*(U32 FAR*)value));
      case EAX_SAMPLE_OBSTRUCTION_LF_RATIO: return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO,*(F32 FAR*)value)));
      case EAX_SAMPLE_OCCLUSION:            return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_OCCLUSION,*(U32 FAR*)value));
      case EAX_SAMPLE_OCCLUSION_LF_RATIO:   return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO,*(F32 FAR*)value)));
      case EAX_SAMPLE_OCCLUSION_ROOM_RATIO: return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO,*(F32 FAR*)value)));
      case EAX_SAMPLE_ROOM_ROLLOFF:         return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR,*(F32 FAR*)value)));
      case EAX_SAMPLE_AIR_ABSORPTION:       return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR,*(F32 FAR*)value)));
      case EAX_SAMPLE_OUTSIDE_VOLUME_HF:    return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF,*(U32 FAR*)value));
      case EAX_SAMPLE_FLAGS:                return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_FLAGS,*(U32 FAR*)value));

#ifdef EAX3
      case EAX_SAMPLE_OCCLUSION_DIRECT_RATIO:return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_OCCLUSIONDIRECTRATIO,*(F32 FAR*)value)));
      case EAX_SAMPLE_EXCLUSION:             return( (!lpPropertySet) ? 0: EAX2_Sample_GetSet_int(S,DSPROPERTY_EAXBUFFER_EXCLUSION,*(U32 FAR*)value));
      case EAX_SAMPLE_EXCLUSION_LF_RATIO:    return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_EXCLUSIONLFRATIO,*(F32 FAR*)value)));
      case EAX_SAMPLE_DOPPLER:               return( (!lpPropertySet) ? 0: (U32) float_as_long(EAX2_Sample_GetSet_float(S,DSPROPERTY_EAXBUFFER_DOPPLERFACTOR,*(F32 FAR*)value)));

#ifdef EAX4
      case EAX_SAMPLE_SLOT_VOLUMES_P:   return( get_eax4_sample_volumes( S, value ) );
      case EAX_SAMPLE_SLOT_VOLUMES_S_P: return( set_eax4_sample_volumes( S, value ) );
#endif

#endif
#endif

      }

#endif

   return prev;
}


//##############################################################################
//#                                                                            #
//# M3D_active_3D_sample_count                                                 #
//#                                                                            #
//##############################################################################

static S32      AILEXPORT M3D_active_3D_sample_count   (void)
{
   S32 i,n;

   n = 0;

   for (i=0; i < avail_samples; i++)
      {
      if (samples[i].status == SMP_PLAYING)
         {
         ++n;
         }
      }

   return n;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_open_listener                                                       #
//#                                                                            #
//##############################################################################

static H3DPOBJECT AILEXPORT M3D_3D_open_listener        (void)
{
   static OBJTYPE listener = IS_LISTENER;

   return (H3DPOBJECT) &listener;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_close_listener                                                      #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_close_listener       (H3DPOBJECT listener)
{
   //
   // Not supported
   //
}

//##############################################################################
//#                                                                            #
//# M3D_3D_open_object                                                         #
//#                                                                            #
//##############################################################################

static H3DPOBJECT AILEXPORT M3D_3D_open_object          (void)
{
   //
   // Not supported
   //

   return NULL;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_close_object                                                        #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_close_object         (H3DPOBJECT obj)
{
   //
   // Not supported
   //
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_position                                                        #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_position         (H3DPOBJECT obj, //)
                                                  F32     X,
                                                  F32     Y,
                                                  F32     Z)
{
   OBJTYPE FAR *t = (OBJTYPE FAR *) obj;

   if (t == NULL)
      {
      return;
      }

   if (*t == IS_SAMPLE)
      {
      SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

      S->position.x = X;
      S->position.y = Y;
      S->position.z = Z;

      if (S->voice != NULL)
         {
         API_lock();
         S->voice->lpds3db->SetPosition(X,Y,Z,DS3D_IMMEDIATE);
         API_unlock();
         }
      }
   else if (*t == IS_LISTENER)
      {
      listen_position.x = X;
      listen_position.y = Y;
      listen_position.z = Z;

      API_lock();
      lp3DLISTENER->SetPosition(X,Y,Z,DS3D_IMMEDIATE);
      API_unlock();
      }
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_velocity_vector                                                 #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_velocity_vector  (H3DPOBJECT obj, //)
                                                  F32     dX_per_ms,
                                                  F32     dY_per_ms,
                                                  F32     dZ_per_ms)
{
   OBJTYPE FAR *t = (OBJTYPE FAR *) obj;

   if (t == NULL)
      {
      return;
      }

   if (*t == IS_SAMPLE)
      {
      SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

      S->velocity.x = dX_per_ms;
      S->velocity.y = dY_per_ms;
      S->velocity.z = dZ_per_ms;

      API_lock();
      if (S->voice != NULL)
         {
         S->voice->lpds3db->SetVelocity(dX_per_ms * 1000.0F,
                                 dY_per_ms * 1000.0F,
                                 dZ_per_ms * 1000.0F,
                                 DS3D_IMMEDIATE);
         }
      API_unlock();
      }
   else if (*t == IS_LISTENER)
      {
      listen_velocity.x = dX_per_ms;
      listen_velocity.y = dY_per_ms;
      listen_velocity.z = dZ_per_ms;

      API_lock();
      lp3DLISTENER->SetVelocity(dX_per_ms * 1000.0F,
                                dY_per_ms * 1000.0F,
                                dZ_per_ms * 1000.0F,
                                DS3D_IMMEDIATE);
      API_unlock();
      }
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_velocity                                                        #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_velocity         (H3DPOBJECT obj, //)
                                                  F32     dX_per_ms,
                                                  F32     dY_per_ms,
                                                  F32     dZ_per_ms,
                                                  F32     magnitude)
{
   M3D_set_3D_velocity_vector(obj,
                              dX_per_ms * magnitude,
                              dY_per_ms * magnitude,
                              dZ_per_ms * magnitude);
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_orientation                                                     #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_orientation      (H3DPOBJECT obj, //)
                                                  F32     X_face,
                                                  F32     Y_face,
                                                  F32     Z_face,
                                                  F32     X_up,
                                                  F32     Y_up,
                                                  F32     Z_up)
{
   OBJTYPE FAR *t = (OBJTYPE FAR *) obj;

   if (t == NULL)
      {
      return;
      }

   if (*t == IS_SAMPLE)
   {
      SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

      S->face.x = X_face;
      S->face.y = Y_face;
      S->face.z = Z_face;

      S->up.x = X_up;
      S->up.y = Y_up;
      S->up.z = Z_up;

      if (S->voice != NULL)
      {
        API_lock();
        S->voice->lpds3db->SetConeOrientation(X_face,
                                       Y_face,
                                       Z_face,
                                       DS3D_IMMEDIATE);
        API_unlock();
      }
   }
   else if (*t == IS_LISTENER)
   {
      //
      // "face" vector - points in facing direction
      //
      // "up" vector - perpendicular to orientation vector
      // This vector points to which direction is up
      // for the listener - it can not be parallel to the
      // listener orientation vector
      //

      listen_face.x = X_face;
      listen_face.y = Y_face;
      listen_face.z = Z_face;

      listen_up.x = X_up;
      listen_up.y = Y_up;
      listen_up.z = Z_up;

      API_lock();
      lp3DLISTENER->SetOrientation(X_face,
                                   Y_face,
                                   Z_face,
                                   X_up,
                                   Y_up,
                                   Z_up,
                                   DS3D_IMMEDIATE);
      API_unlock();
   }
}

//##############################################################################
//#                                                                            #
//# M3D_3D_position                                                            #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_position             (H3DPOBJECT  obj, //)
                                                  F32 FAR *X,
                                                  F32 FAR *Y,
                                                  F32 FAR *Z)
{
   OBJTYPE FAR *t = (OBJTYPE FAR *) obj;

   if (t == NULL)
      {
      return;
      }

   if (*t == IS_SAMPLE)
      {
      SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

      if (X) *X = S->position.x;
      if (Y) *Y = S->position.y;
      if (Z) *Z = S->position.z;
      }
   else if (*t == IS_LISTENER)
      {
      if (X) *X = listen_position.x;
      if (Y) *Y = listen_position.y;
      if (Z) *Z = listen_position.z;
      }
}

//##############################################################################
//#                                                                            #
//# M3D_3D_velocity                                                            #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_velocity             (H3DPOBJECT  obj, //)
                                                  F32 FAR *dX_per_ms,
                                                  F32 FAR *dY_per_ms,
                                                  F32 FAR *dZ_per_ms)
{
   OBJTYPE FAR *t = (OBJTYPE FAR *) obj;

   if (t == NULL)
      {
      return;
      }

   if (*t == IS_SAMPLE)
      {
      SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

      if (dX_per_ms) *dX_per_ms = S->velocity.x;
      if (dY_per_ms) *dY_per_ms = S->velocity.y;
      if (dZ_per_ms) *dZ_per_ms = S->velocity.z;
      }
   else if (*t == IS_LISTENER)
      {
      if (dX_per_ms) *dX_per_ms = listen_velocity.x;
      if (dY_per_ms) *dY_per_ms = listen_velocity.y;
      if (dZ_per_ms) *dZ_per_ms = listen_velocity.z;
      }
}

//##############################################################################
//#                                                                            #
//# M3D_3D_orientation                                                         #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_orientation          (H3DPOBJECT  obj, //)
                                                  F32 FAR *X_face,
                                                  F32 FAR *Y_face,
                                                  F32 FAR *Z_face,
                                                  F32 FAR *X_up,
                                                  F32 FAR *Y_up,
                                                  F32 FAR *Z_up)
{
   OBJTYPE FAR *t = (OBJTYPE FAR *) obj;

   if (t == NULL)
      {
      return;
      }

   if (*t == IS_SAMPLE)
      {
      SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

      if (X_face) *X_face = S->face.x;
      if (Y_face) *Y_face = S->face.y;
      if (Z_face) *Z_face = S->face.z;
      if (X_up)   *X_up   = S->up.x;
      if (Y_up)   *Y_up   = S->up.y;
      if (Z_up)   *Z_up   = S->up.z;
      }
   else if (*t == IS_LISTENER)
      {
      if (X_face) *X_face = listen_face.x;
      if (Y_face) *Y_face = listen_face.y;
      if (Z_face) *Z_face = listen_face.z;
      if (X_up)   *X_up   = listen_up.x;
      if (X_up)   *Y_up   = listen_up.y;
      if (X_up)   *Z_up   = listen_up.z;
      }
}

//##############################################################################
//#                                                                            #
//# M3D_3D_update_position                                                     #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_update_position      (H3DPOBJECT obj, //)
                                                  F32     dt_milliseconds)
{
   if (obj == NULL)
      {
      return;
      }

   F32 X,Y,Z;
   F32 dX_dt,dY_dt,dZ_dt;

   M3D_3D_velocity(obj,&dX_dt,&dY_dt,&dZ_dt);

   if ((fabs(dX_dt) < EPSILON) &&
       (fabs(dY_dt) < EPSILON) &&
       (fabs(dZ_dt) < EPSILON))
       {
       return;
       }

   M3D_3D_position(obj,&X,&Y,&Z);

   X += (dX_dt * dt_milliseconds);
   Y += (dY_dt * dt_milliseconds);
   Z += (dZ_dt * dt_milliseconds);

   M3D_set_3D_position(obj,X,Y,Z);
}

//##############################################################################
//#                                                                            #
//# M3D_3D_auto_update_position                                                #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_auto_update_position (H3DPOBJECT obj, //)
                                                  S32        enable)
{
   OBJTYPE FAR *t = (OBJTYPE FAR *) obj;

   if (t == NULL)
      {
      return;
      }

   if (*t == IS_SAMPLE)
      {
      SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

      S->auto_update = enable;
      }
   else if (*t == IS_LISTENER)
      {
      listen_auto_update = enable;
      }
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_data                                                     #
//#                                                                            #
//##############################################################################

static S32        AILEXPORT M3D_set_3D_sample_data      (H3DSAMPLE         samp, //)
                                                  AILSOUNDINFO FAR *info)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return 0;
      }

   //
   // Validate format
   //

   if (info->format != WAVE_FORMAT_PCM)
      {
      AIL_set_error("Not linear PCM data");
      return 0;
      }

   if (info->channels != 1)
      {
      AIL_set_error("Not monaural data");
      return 0;
      }

   S->eos=0;

   //
   // Ensure that the background thread does not try to access the
   // sample structure while we are manipulating it
   //

   sleep_sample(S);

   init_sample(S);      // Sets SMP_DONE, but does not free hardware voice

   S->start            = info->data_ptr;
   S->len              = info->data_len;
   S->playback_rate    = info->rate;
   S->bytes_per_sample = info->bits / 8;
   S->chans            = info->channels;

   //
   // Ensure that sample has an allocated voice that matches the  
   // requested bits/chans attributes
   //

   S32 result = DS_assign_voice_to_sample(S);

   if (!result)
      {
      //
      // No hardware voices are available to play sample
      //

      wake_sample(S);
      return 0;
      }

   if (result == 2)
      {
      //
      // Voice was just reallocated -- set its default 3D properties,
      // along with those of the H3DSAMPLE
      //
      // This function is documented to always reset these properties, but 
      // we only do it if the hardware voice was actually reallocated
      //

      set_M3D_defaults(samp);
      }

   //
   // Return success
   //

   wake_sample(S);
   return 1;
}

//############################################################################
//#                                                                          #
//# Set sample 3D defaults                                                   #
//#                                                                          #
//############################################################################

static void set_M3D_defaults(H3DSAMPLE samp)
{
   SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

   if (S->status == SMP_FREE)
      {
      return;
      }

   //
   // Set emitter distances
   //

   M3D_set_3D_sample_distances(samp,
                               200.0F,
                               1.0F);
   //
   // Set default emitter volume, orientation, and velocity
   //
   // We do not set a default position here, to avoid Doppler artifacts
   // when the application sets the actual emitter position
   //

   M3D_set_3D_sample_volume(samp,
                            S->volume);

   //
   // Clear all special attenuation effects
   //
   M3D_set_3D_sample_exclusion    (samp, 0.0F);
   M3D_set_3D_sample_obstruction  (samp, 0.0F);
   M3D_set_3D_sample_occlusion    (samp, 0.0F);

#ifdef EAX3D
   M3D_set_3D_sample_effects_level(samp, 0.0F);
#endif

   M3D_set_3D_orientation(samp,
                          0.0F, 0.0F, 1.0F,
                          0.0F, 1.0F, 0.0F);

   //
   // Set velocity
   //

   M3D_set_3D_velocity_vector(samp,
                              0.0F,
                              0.0F,
                              0.0F);

   //
   // Set cone
   //

   M3D_set_3D_sample_cone    (samp,
                              360.0F,
                              360.0F,
                              127);

}

//############################################################################
//#                                                                          #
//# AIL timer callback for automatic position updates                        #
//#                                                                          #
//############################################################################

static void AILCALLBACK M3D_serve(U32 user)
{
   S32 i;

   //
   // Update sample positions
   //

   for (i=0; i < avail_samples; i++)
      {
      SAMPLE3D FAR *S = &samples[i];

      if (!S->auto_update)
         {
         continue;
         }

      if ((S->status != SMP_PLAYING) && (S->status != SMP_STOPPED))
         {
         continue;
         }

      M3D_3D_update_position(H3DPOBJECT(S),
                             F32(SERVICE_MSECS));
      }

   //
   // Update listener positions
   //

   if (listen_auto_update)
      {
      OBJTYPE type = IS_LISTENER;

      M3D_3D_update_position(H3DPOBJECT(&type),
                             F32(SERVICE_MSECS));
      }
}

#ifdef AUREAL
  static HMODULE a3dlib=0;
  static LPVOID CREATEADDRESS=0;
  static S32 saved=0;
  static DWORD oldscreen=0;
  static DWORD oldaudio=0;
#else
  #define CREATEADDRESS 0
#endif

//##############################################################################
//#                                                                            #
//# M3D_activate                                                               #
//#                                                                            #
//##############################################################################

static M3DRESULT AILEXPORT M3D_activate                 (S32 enable)
{
   if (enable)
      {

      // -----------------------------------------
      // Activate
      // -----------------------------------------

      if (active)
         {
         AIL_set_error("M3D provider already activated");
         return M3D_ALREADY_STARTED;
         }

      //
      // Restart DirectSound with 3D audio enabled
      //

#ifdef AUREAL

      AIL_unlock_mutex();
      if (a3dlib==0)
        a3dlib=LoadLibrary("a3d");
      AIL_lock_mutex();
      if (((U32)a3dlib)>=32)
        CREATEADDRESS=GetProcAddress(a3dlib,"_A3dCreate@12");
      if (CREATEADDRESS==0) {
        AIL_set_error("The A3D library could not be loaded");
        return M3D_INTERNAL_ERR;
      }

      //
      // Are we running under DX5 or greater (use DS3D hardware if so)
      //

      {
        //
        // Get DirectSound object and primary buffer pointers from MSS
        //

        lpDS     = NULL;

        AIL_get_DirectSound_info(          NULL,
           (AILLPDIRECTSOUND FAR *)       &lpDS,
           0);

        if ((lpDS) && (AIL_get_preference(DIG_DS_CREATION_HANDLER)==0))
        {
          DSCaps.dwSize = sizeof(DSCaps);

          API_lock();

          if (SUCCEEDED(lpDS->GetCaps(&DSCaps)))
          {
            if (DSCaps.dwFreeHw3DStreamingBuffers)
            {
              // hardware buffers are available without A3D
              FreeLibrary(a3dlib);

              a3dlib=0;
              CREATEADDRESS=0;
            }
          }

          API_unlock();
        }
      }

      if (CREATEADDRESS)
      {
        // A3D registry key
        #define REG_SETTINGS_KEY           TEXT("Software\\Aureal\\A3D")
        // A3D splash registry values
        #define REG_SETTING_SPLASH_SCREEN  TEXT("SplashScreen")
        #define REG_SETTING_SPLASH_AUDIO   TEXT("SplashAudio")

        // turn off the stupid splash sound and graphic

        HKEY hReg;
        DWORD dwCreateDisposition;

        // Save current settings to our registry key
        if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, REG_SETTINGS_KEY,
                       0, NULL, 0, KEY_WRITE, NULL, &hReg,
                       &dwCreateDisposition)==ERROR_SUCCESS)
        {

          saved=1;
          DWORD typ,size=sizeof(DWORD);

          // read original value
          RegQueryValueEx(hReg,REG_SETTING_SPLASH_SCREEN, 0,
                          &typ,(LPBYTE)&oldscreen,&size);
          RegQueryValueEx(hReg,REG_SETTING_SPLASH_AUDIO, 0,
                          &typ,(LPBYTE)&oldaudio,&size);


          // Save the "SplashScreen" flag
          DWORD dwVal = (DWORD) false;        // "true" to enable
          RegSetValueEx(hReg, REG_SETTING_SPLASH_SCREEN, 0, REG_DWORD,
                      (LPBYTE) &dwVal, sizeof(DWORD));
          RegSetValueEx(hReg, REG_SETTING_SPLASH_AUDIO, 0, REG_DWORD,
                      (LPBYTE) &dwVal, sizeof(DWORD));

          RegCloseKey(hReg);
          hReg = NULL;
        }
      }

#endif

      if ((AIL_get_preference(DIG_DS_CREATION_HANDLER)!=(S32)CREATEADDRESS) ||
        (!AIL_get_preference(DIG_DS_DSBCAPS_CTRL3D))) {

        AIL_digital_handle_release(NULL);

        AIL_set_preference(DIG_DS_CREATION_HANDLER, (U32)CREATEADDRESS );

        AIL_set_preference(DIG_DS_DSBCAPS_CTRL3D, TRUE);

        AIL_digital_handle_reacquire(NULL);
      }

      //
      // Get DirectSound object and primary buffer pointers from MSS
      //
      // Fail if MSS is not using DirectSound
      //

      lpDS     = NULL;
      lpDSPRIM = NULL;

      AIL_get_DirectSound_info(          NULL,
         (AILLPDIRECTSOUND FAR *)       &lpDS,
         (AILLPDIRECTSOUNDBUFFER FAR *) &lpDSPRIM);

      if (lpDS == NULL)
         {
         AIL_set_error("M3D provider requires AIL_USE_WAVEOUT==NO");
         return M3D_INTERNAL_ERR;
         }

#ifdef HWARE
      //
      // Get # of buffers available in hardware, and set our sample-
      // allocation limit to this value
      //

      DSCaps.dwSize = sizeof(DSCaps);

      API_lock();

      if (!(SUCCEEDED(lpDS->GetCaps(&DSCaps))))
         {
         API_unlock();
         AIL_set_error("lpDS->GetCaps() failed");
         return M3D_INTERNAL_ERR;
         }

      API_unlock();

      avail_samples = max( DSCaps.dwMaxHw3DAllBuffers , DSCaps.dwFreeHw3DAllBuffers);

      avail_samples = min(N_SAMPLES, avail_samples);

#ifdef EAX3D
      //
      // EAX: Subtract one secondary buffer for our internal use
      //

      if (avail_samples)
         {
         --avail_samples;
         }
#endif

#else
      avail_samples = N_SAMPLES;
#endif

      //
      // Bomb out if no samples available
      //

      if (!avail_samples)
         {
         AIL_set_error("Provider could not be initialized -- no 3D voices available");
         return M3D_NOT_INIT;
         }

      //
      // Create listener object
      //

      API_lock();

      if (FAILED(lpDSPRIM->QueryInterface(IID_IDirectSound3DListener,
                                          (LPVOID *) &lp3DLISTENER)))
         {
         AIL_set_error("Failed to create DS3D listener object");

         API_unlock();
         return M3D_INTERNAL_ERR;
         }

      API_unlock();

#ifdef DX7
      API_lock();

      LPDIRECTSOUNDBUFFER lpSecondaryBuffer = NULL;

      WAVEFORMATEX   pcmOut;           // Format of the wave for secondary buffer if we need to make one.
      DSBUFFERDESC   dsbdSecondary;    // description for creating secondary buffer if we need to make one.

      // we don't have a secondary to work with so we will create one.
      ZeroMemory( &dsbdSecondary, sizeof(DSBUFFERDESC));
      ZeroMemory( &pcmOut, sizeof(WAVEFORMATEX));
      // any format should do I just say 11kHz 16 bit mono
      pcmOut.wFormatTag = WAVE_FORMAT_PCM;
      pcmOut.nChannels = 1;
      pcmOut.nSamplesPerSec = 11025;
      pcmOut.nAvgBytesPerSec = 22050;
      pcmOut.nBlockAlign = 2;
      pcmOut.wBitsPerSample = 16;
      pcmOut.cbSize = 0;
      // size is just arbitary but not too small as I have seen problems with single sample buffers...
      dsbdSecondary.dwSize = sizeof(DSBUFFERDESC);
      dsbdSecondary.dwBufferBytes = 1024;
      dsbdSecondary.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
      dsbdSecondary.lpwfxFormat = &pcmOut;
#ifdef DX7SN
      dsbdSecondary.guid3DAlgorithm =DS3DALG_NO_VIRTUALIZATION;
#else
#ifdef DX7SL
      dsbdSecondary.guid3DAlgorithm =DS3DALG_HRTF_LIGHT;
#else
#ifdef DX7SH
      dsbdSecondary.guid3DAlgorithm =DS3DALG_HRTF_FULL;
#endif
#endif
#endif

      if (lpDS->CreateSoundBuffer(&dsbdSecondary, &lpSecondaryBuffer, NULL) != DS_OK)
      {
         if (lp3DLISTENER)
            {
            lp3DLISTENER->Release();
            lp3DLISTENER = NULL;
            }

         API_unlock();

         AIL_set_error("Unable to initialize DirectSound 3D 7+ support");
         return M3D_NOT_INIT;
      }

      // now just release it
      lpSecondaryBuffer->Release();
      lpSecondaryBuffer = NULL;

      API_unlock();
#endif

#ifdef EAX3D

      //
      // EAX: Create dummy secondary buffer to support global reverb object,
      // and get property set
      //
      // (code from Creative's EAXTEST.CPP and .PDF file)
      //

      API_lock();

      lpSecondaryBuffer = NULL;
      lpDs3dBuffer      = NULL;
      lpPropertySet     = NULL;

      WAVEFORMATEX   pcmOut;           // Format of the wave for secondary buffer if we need to make one.
      DSBUFFERDESC   dsbdSecondary;    // description for creating secondary buffer if we need to make one.

      // we don't have a secondary to work with so we will create one.
      ZeroMemory( &dsbdSecondary, sizeof(DSBUFFERDESC));
      ZeroMemory( &pcmOut, sizeof(WAVEFORMATEX));
      // any format should do I just say 11kHz 16 bit mono
      pcmOut.wFormatTag = WAVE_FORMAT_PCM;
      pcmOut.nChannels = 1;
      pcmOut.nSamplesPerSec = 11025;
      pcmOut.nAvgBytesPerSec = 22050;
      pcmOut.nBlockAlign = 2;
      pcmOut.wBitsPerSample = 16;
      pcmOut.cbSize = 0;
      // size is just arbitary but not too small as I have seen problems with single sample buffers...
      dsbdSecondary.dwSize = sizeof(DSBUFFERDESC);
      dsbdSecondary.dwBufferBytes = 1024;
      dsbdSecondary.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
      dsbdSecondary.lpwfxFormat = &pcmOut;

      DWORD support = 0;

      if ((lpDS->CreateSoundBuffer(&dsbdSecondary, &lpSecondaryBuffer, NULL) != DS_OK)
          ||
          (lpSecondaryBuffer->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID *) &lpDs3dBuffer) != DS_OK)
          ||
          (lpDs3dBuffer->QueryInterface(IID_IKsPropertySet, (LPVOID *) &lpPropertySet) != DS_OK)
          ||
#ifdef EAX4
          (lpPropertySet->QuerySupport(EAXPROPERTYID_EAX40_Context,
            EAXCONTEXT_ALLPARAMETERS,
#else
          (lpPropertySet->QuerySupport(EAXPROP,
#ifdef EAX2
            DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
#else
            DSPROPERTY_EAX_ALL,
#endif
#endif
            &support) != DS_OK)
          ||
          ((support & PSET_SETGET) != PSET_SETGET))
         {
         EAX_destroy();

         if (lp3DLISTENER)
            {
            lp3DLISTENER->Release();
            lp3DLISTENER = NULL;
            }

         API_unlock();

         AIL_set_error("Unable to initialize EAX support");

         return M3D_NOT_INIT;
         }

      API_unlock();

#endif

      //
      // Set default listener position
      //

      OBJTYPE type = IS_LISTENER;

      M3D_set_3D_position(H3DPOBJECT(&type),
                          0.0F,
                          0.0F,
                          0.0F);

      //
      // Set default listener orientation and up-vector
      //

      M3D_set_3D_orientation(H3DPOBJECT(&type),
                             0.0F, 0.0F, 1.0F,
                             0.0F, 1.0F, 0.0F);

      //
      // Set default listener velocity
      //

      M3D_set_3D_velocity_vector(H3DPOBJECT(&type),
                                 0.0F,
                                 0.0F,
                                 0.0F);

      //
      // Initialize samples
      //

      S32 i;

      for (i=0; i < avail_samples; i++)
         {
         SAMPLE3D FAR *S = &samples[i];

         memset(S,
                0,
                sizeof(struct SAMPLE3D));

         S->index  = i;
         S->status = SMP_FREE;

         S->noints = 0;

         //
         // Clear sample emitter
         //

         S->voice = NULL;
         }

      //
      // Register and start service timer
      //

      service_timer = AIL_register_timer(M3D_serve);

      AIL_set_timer_period(service_timer, SERVICE_MSECS * 1000);

      AIL_start_timer(service_timer);

      //
      // Register and start buffer-notification timer at 20 Hz
      //

      buffer_timer = AIL_register_timer(notify_timer);

      AIL_set_timer_frequency(buffer_timer, 20);

      AIL_start_timer(buffer_timer);


#ifdef EAX3D
      M3D_set_3D_room_type( EAX_ENVIRONMENT_GENERIC );
#endif

      first_voice = NULL;

      active = 1;
      }
   else
      {
      // -----------------------------------------
      // Deactivate
      // -----------------------------------------

      if (!active)
         {
         AIL_set_error("M3D provider not activated");
         return M3D_NOT_INIT;
         }

      //
      // Stop service timer and buffer timer
      //

      AIL_stop_timer(service_timer);
      AIL_release_timer_handle(service_timer);

      AIL_stop_timer(buffer_timer);
      AIL_release_timer_handle(buffer_timer);

      //
      // Deallocate resources associated with samples
      //

      S32 i;

      for (i=0; i < avail_samples; i++)
         {
         SAMPLE3D FAR *S = &samples[i];

         reset_sample_voice(S, TRUE);

         //
         // Mark handle free
         //

         S->status = SMP_FREE;
         }

      //
      // Release hardware voices
      //

      while (first_voice != NULL)
         {
         DS_release_voice_resources(first_voice);
         first_voice = first_voice->next_voice;
         }

       //
       // Release EAX objects
       //

#ifdef EAX3D
      EAX_destroy();
#endif

      //
      // Release the listener
      //

      if (lp3DLISTENER)
         {
         API_lock();
         lp3DLISTENER->Release();
         lp3DLISTENER = NULL;
         API_unlock();
         }

      active = 0;

#ifdef AUREAL
      if (AIL_get_preference(DIG_DS_CREATION_HANDLER))
      {

        AIL_digital_handle_release(NULL);

        // turn off A3D, but leave 3D mode on
        AIL_set_preference(DIG_DS_CREATION_HANDLER, 0 );

        AIL_set_preference(DIG_DS_DSBCAPS_CTRL3D, TRUE);

        AIL_digital_handle_reacquire(NULL);
      }

      // restore the registry
      if (saved)
      {
        saved=0;

        HKEY hReg;
        DWORD dwCreateDisposition;

        // Save current settings to our registry key
        if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, REG_SETTINGS_KEY,
                       0, NULL, 0, KEY_WRITE, NULL, &hReg,
                       &dwCreateDisposition)==ERROR_SUCCESS)
        {

          // Save the "SplashScreen" flag
          RegSetValueEx(hReg, REG_SETTING_SPLASH_SCREEN, 0, REG_DWORD,
                      (LPBYTE) &oldscreen, sizeof(DWORD));
          RegSetValueEx(hReg, REG_SETTING_SPLASH_AUDIO, 0, REG_DWORD,
                      (LPBYTE) &oldaudio, sizeof(DWORD));

          RegCloseKey(hReg);
          hReg = NULL;
        }

      }

      if (a3dlib)
      {
        AIL_lock();
        AIL_unlock_mutex();
        FreeLibrary(a3dlib);
        AIL_lock_mutex();
        AIL_unlock();
        a3dlib=0;
      }

#endif

   }

   return M3D_NOERR;
}

//############################################################################
//#                                                                          #
//# DLLMain registers M3D API interface at load time                         #
//#                                                                          #
//############################################################################

#ifdef DX7SN
extern "C" S32 dx7snMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 dx7snMain( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef DX7SL
extern "C" S32 dx7slMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 dx7slMain( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef DX7SH
extern "C" S32 dx7shMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 dx7shMain( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef AUREAL
extern "C" S32 a3dMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 a3dMain( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef EAX4
extern "C" S32 EAX4Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX4Main( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef EAX3
extern "C" S32 EAX3Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX3Main( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef EAX2
extern "C" S32 EAX2Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX2Main( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef EAX3D
extern "C" S32 EAXMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAXMain( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef HWARE
extern "C" S32 hwMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 hwMain( HPROVIDER provider_handle, U32 up_down )
#else
extern "C" S32 swMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 swMain( HPROVIDER provider_handle, U32 up_down )
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
{
   const RIB_INTERFACE_ENTRY M3DSAMPLE[] =
      {
#ifdef EAX3D
      REG_AT("EAX sample reverb mix",            EAX_EFFECT_REVERBMIX,            RIB_FLOAT),
      REG_AT("EAX sample all parameters",        EAX_EFFECT_ALL_PARAMETERS,       RIB_DEC),

      REG_PR("EAX sample reverb mix",            EAX_EFFECT_REVERBMIX,            RIB_FLOAT),
      REG_PR("EAX sample all parameters",        EAX_EFFECT_ALL_PARAMETERS,       RIB_DEC),

      REG_AT("EAX sample property set",          EAX_PROPERTY_SET,                RIB_DEC),

#ifdef EAX2
      REG_AT("EAX2 sample direct",               EAX_SAMPLE_DIRECT,               RIB_DEC  ),
      REG_AT("EAX2 sample direct HF",            EAX_SAMPLE_DIRECT_HF,            RIB_DEC  ),
      REG_AT("EAX2 sample room",                 EAX_SAMPLE_ROOM,                 RIB_DEC  ),
      REG_AT("EAX2 sample room HF",              EAX_SAMPLE_ROOM_HF,              RIB_DEC  ),
      REG_AT("EAX2 sample obstruction",          EAX_SAMPLE_OBSTRUCTION,          RIB_DEC  ),
      REG_AT("EAX2 sample obstruction LF ratio", EAX_SAMPLE_OBSTRUCTION_LF_RATIO, RIB_FLOAT),
      REG_AT("EAX2 sample occlusion",            EAX_SAMPLE_OCCLUSION,            RIB_DEC  ),
      REG_AT("EAX2 sample occlusion LF ratio",   EAX_SAMPLE_OCCLUSION_LF_RATIO,   RIB_FLOAT),
      REG_AT("EAX2 sample occlusion room ratio", EAX_SAMPLE_OCCLUSION_ROOM_RATIO, RIB_FLOAT),
      REG_AT("EAX2 sample room rolloff",         EAX_SAMPLE_ROOM_ROLLOFF,         RIB_FLOAT),
      REG_AT("EAX2 sample air absorption",       EAX_SAMPLE_AIR_ABSORPTION,       RIB_FLOAT),
      REG_AT("EAX2 sample outside volume HF",    EAX_SAMPLE_OUTSIDE_VOLUME_HF,    RIB_DEC  ),
      REG_AT("EAX2 sample flags",                EAX_SAMPLE_FLAGS,                RIB_DEC  ),

      REG_PR("EAX2 sample direct",               EAX_SAMPLE_DIRECT,               RIB_DEC  ),
      REG_PR("EAX2 sample direct HF",            EAX_SAMPLE_DIRECT_HF,            RIB_DEC  ),
      REG_PR("EAX2 sample room",                 EAX_SAMPLE_ROOM,                 RIB_DEC  ),
      REG_PR("EAX2 sample room HF",              EAX_SAMPLE_ROOM_HF,              RIB_DEC  ),
      REG_PR("EAX2 sample obstruction",          EAX_SAMPLE_OBSTRUCTION,          RIB_DEC  ),
      REG_PR("EAX2 sample obstruction LF ratio", EAX_SAMPLE_OBSTRUCTION_LF_RATIO, RIB_FLOAT),
      REG_PR("EAX2 sample occlusion",            EAX_SAMPLE_OCCLUSION,            RIB_DEC  ),
      REG_PR("EAX2 sample occlusion LF ratio",   EAX_SAMPLE_OCCLUSION_LF_RATIO,   RIB_FLOAT),
      REG_PR("EAX2 sample occlusion room ratio", EAX_SAMPLE_OCCLUSION_ROOM_RATIO, RIB_FLOAT),
      REG_PR("EAX2 sample room rolloff",         EAX_SAMPLE_ROOM_ROLLOFF,         RIB_FLOAT),
      REG_PR("EAX2 sample air absorption",       EAX_SAMPLE_AIR_ABSORPTION,       RIB_FLOAT),
      REG_PR("EAX2 sample outside volume HF",    EAX_SAMPLE_OUTSIDE_VOLUME_HF,    RIB_DEC  ),
      REG_PR("EAX2 sample flags",                EAX_SAMPLE_FLAGS,                RIB_DEC  ),

#ifdef EAX3
      REG_AT("EAX3 sample occlusion direct ratio", EAX_SAMPLE_OCCLUSION_DIRECT_RATIO, RIB_FLOAT),
      REG_AT("EAX3 sample exclusion",            EAX_SAMPLE_EXCLUSION,              RIB_DEC  ),
      REG_AT("EAX3 sample exclusion LF ratio",   EAX_SAMPLE_EXCLUSION_LF_RATIO,     RIB_FLOAT),
      REG_AT("EAX3 sample doppler factor",       EAX_SAMPLE_DOPPLER,                RIB_FLOAT),

      REG_PR("EAX3 sample occlusion direct ratio", EAX_SAMPLE_OCCLUSION_DIRECT_RATIO, RIB_FLOAT),
      REG_PR("EAX3 sample exclusion",            EAX_SAMPLE_EXCLUSION,              RIB_DEC  ),
      REG_PR("EAX3 sample exclusion LF ratio",   EAX_SAMPLE_EXCLUSION_LF_RATIO,     RIB_FLOAT),
      REG_PR("EAX3 sample doppler factor",       EAX_SAMPLE_DOPPLER,                RIB_FLOAT),
#endif

#ifdef EAX4
      REG_PR("Get EAX4 slot volumes",            EAX_SAMPLE_SLOT_VOLUMES_P,         RIB_DEC),
      REG_PR("EAX4 slot volumes",                EAX_SAMPLE_SLOT_VOLUMES_S_P,       RIB_DEC),
#endif

#endif

      REG_FN(M3D_set_3D_sample_effects_level),
      REG_FN(M3D_3D_sample_effects_level),
      REG_FN(M3D_set_3D_room_type),
      REG_FN(M3D_3D_room_type),

#endif

      REG_AT("DirectSound sample buffer",        DIRECTSOUND_SAMPLE_BUFFER,         RIB_DEC),

      REG_FN(M3D_set_3D_EOS),
      REG_FN(M3D_3D_sample_query_attribute),
      REG_FN(M3D_3D_set_sample_preference),

      REG_FN(M3D_set_3D_speaker_type),
      REG_FN(M3D_3D_speaker_type),

      REG_FN(M3D_set_3D_sample_obstruction),
      REG_FN(M3D_3D_sample_obstruction),
      REG_FN(M3D_set_3D_sample_occlusion),
      REG_FN(M3D_3D_sample_occlusion),
      REG_FN(M3D_set_3D_sample_exclusion),
      REG_FN(M3D_3D_sample_exclusion),

      REG_FN(M3D_set_3D_sample_cone),
      REG_FN(M3D_3D_sample_cone),

      REG_FN(M3D_set_3D_rolloff_factor),
      REG_FN(M3D_3D_rolloff_factor),

      REG_FN(M3D_set_3D_doppler_factor),
      REG_FN(M3D_3D_doppler_factor),

      REG_FN(M3D_set_3D_distance_factor),
      REG_FN(M3D_3D_distance_factor),
   };

   const RIB_INTERFACE_ENTRY M3D[] =
      {
      REG_AT("Name",                        PROVIDER_NAME,                  RIB_STRING),
      REG_AT("Version",                     PROVIDER_VERSION,               RIB_HEX),
      REG_AT("Maximum supported samples",   MAX_SUPPORTED_SAMPLES,          RIB_DEC),
      REG_AT("Mute at maximum",             MUTE_AT_MAX, RIB_DEC),
      REG_PR("Mute at maximum",             MUTE_AT_MAX, RIB_DEC),

#ifdef EAX3D
      REG_AT("EAX environment selection",   EAX_ENVIRONMENT,                    RIB_DEC),
      REG_AT("EAX effect volume",           EAX_EFFECT_VOLUME,                  RIB_FLOAT),
      REG_AT("EAX decay time",              EAX_DECAY_TIME,                     RIB_FLOAT),
      REG_AT("EAX damping",                 EAX_DAMPING,                        RIB_FLOAT),
      REG_AT("EAX all parameters",          EAX_ALL_PARAMETERS,                 RIB_DEC),

      REG_AT("EAX property set",            EAX_PROPERTY_SET,                   RIB_DEC),

      REG_PR("EAX environment selection",   EAX_ENVIRONMENT,                    RIB_DEC),
      REG_PR("EAX effect volume",           EAX_EFFECT_VOLUME,                  RIB_FLOAT),
      REG_PR("EAX decay time",              EAX_DECAY_TIME,                     RIB_FLOAT),
      REG_PR("EAX damping",                 EAX_DAMPING,                        RIB_FLOAT),
      REG_PR("EAX all parameters",          EAX_ALL_PARAMETERS,                 RIB_DEC),

#ifdef EAX2
      REG_AT("EAX2 environment size",       EAX_ENVIRONMENT_SIZE,       RIB_FLOAT),
      REG_AT("EAX2 environment diffusion",  EAX_ENVIRONMENT_DIFFUSION,  RIB_FLOAT),
      REG_AT("EAX2 room",                   EAX_ROOM,                   RIB_DEC),
      REG_AT("EAX2 room HF",                EAX_ROOM_HF,                RIB_DEC),
      REG_AT("EAX2 decay HF ratio",         EAX_DECAY_HF_RATIO,         RIB_FLOAT),
      REG_AT("EAX2 reflections",            EAX_REFLECTIONS,            RIB_DEC),
      REG_AT("EAX2 reflections delay",      EAX_REFLECTIONS_DELAY,      RIB_FLOAT),
      REG_AT("EAX2 reverb",                 EAX_REVERB,                 RIB_DEC),
      REG_AT("EAX2 reverb delay",           EAX_REVERB_DELAY,           RIB_FLOAT),
      REG_AT("EAX2 room rolloff",           EAX_ROOM_ROLLOFF,           RIB_FLOAT),
      REG_AT("EAX2 air absorption",         EAX_AIR_ABSORPTION,         RIB_FLOAT),
      REG_AT("EAX2 flags",                  EAX_FLAGS,                  RIB_DEC),

      REG_PR("EAX2 environment size",       EAX_ENVIRONMENT_SIZE,       RIB_FLOAT),
      REG_PR("EAX2 environment diffusion",  EAX_ENVIRONMENT_DIFFUSION,  RIB_FLOAT),
      REG_PR("EAX2 room",                   EAX_ROOM,                   RIB_DEC),
      REG_PR("EAX2 room HF",                EAX_ROOM_HF,                RIB_DEC),
      REG_PR("EAX2 decay HF ratio",         EAX_DECAY_HF_RATIO,         RIB_FLOAT),
      REG_PR("EAX2 reflections",            EAX_REFLECTIONS,            RIB_DEC),
      REG_PR("EAX2 reflections delay",      EAX_REFLECTIONS_DELAY,      RIB_FLOAT),
      REG_PR("EAX2 reverb",                 EAX_REVERB,                 RIB_DEC),
      REG_PR("EAX2 reverb delay",           EAX_REVERB_DELAY,           RIB_FLOAT),
      REG_PR("EAX2 room rolloff",           EAX_ROOM_ROLLOFF,           RIB_FLOAT),
      REG_PR("EAX2 air absorption",         EAX_AIR_ABSORPTION,         RIB_FLOAT),
      REG_PR("EAX2 flags",                  EAX_FLAGS,                  RIB_DEC),

#ifdef EAX3
      REG_AT("EAX3 room LF",                EAX_ROOM_LF,                RIB_DEC),
      REG_AT("EAX3 decay LF ratio",         EAX_DECAY_LF_RATIO,         RIB_FLOAT),
      REG_AT("EAX3 reflections pan",        EAX_REFLECTIONS_PAN,        RIB_FLOAT),
      REG_AT("EAX3 reverb pan",             EAX_REVERB_PAN,             RIB_FLOAT),
      REG_AT("EAX3 echo time",              EAX_ECHO_TIME,              RIB_FLOAT),
      REG_AT("EAX3 echo depth",             EAX_ECHO_DEPTH,             RIB_FLOAT),
      REG_AT("EAX3 modulation time",        EAX_MODULATION_TIME,        RIB_FLOAT),
      REG_AT("EAX3 modulation depth",       EAX_MODULATION_DEPTH,       RIB_FLOAT),
      REG_AT("EAX3 HF reference",           EAX_HF_REFERENCE,           RIB_FLOAT),
      REG_AT("EAX3 LF reference",           EAX_LF_REFERENCE,           RIB_FLOAT),

      REG_PR("EAX3 room LF",                EAX_ROOM_LF,                RIB_DEC),
      REG_PR("EAX3 decay LF ratio",         EAX_DECAY_LF_RATIO,         RIB_FLOAT),
      REG_PR("EAX3 reflections pan",        EAX_REFLECTIONS_PAN,        RIB_FLOAT),
      REG_PR("EAX3 reverb pan",             EAX_REVERB_PAN,             RIB_FLOAT),
      REG_PR("EAX3 echo time",              EAX_ECHO_TIME,              RIB_FLOAT),
      REG_PR("EAX3 echo depth",             EAX_ECHO_DEPTH,             RIB_FLOAT),
      REG_PR("EAX3 modulation time",        EAX_MODULATION_TIME,        RIB_FLOAT),
      REG_PR("EAX3 modulation depth",       EAX_MODULATION_DEPTH,       RIB_FLOAT),
      REG_PR("EAX3 HF reference",           EAX_HF_REFERENCE,           RIB_FLOAT),
      REG_PR("EAX3 LF reference",           EAX_LF_REFERENCE,           RIB_FLOAT),
#endif

#ifdef EAX4
      REG_PR("Get EAX4 Auto Gain parameters",   EAX_AUTO_GAIN_P,            RIB_DEC),
      REG_PR("EAX4 Auto Gain parameters",       EAX_AUTO_GAIN_S_P,          RIB_DEC),

      REG_PR("Get EAX4 Autowah parameters",     EAX_AUTOWAH_P,              RIB_DEC),
      REG_PR("EAX4 Autowah parameters",         EAX_AUTOWAH_S_P,            RIB_DEC),

      REG_PR("Get EAX4 Chorus parameters",      EAX_CHORUS_P,               RIB_DEC),
      REG_PR("EAX4 Chorus parameters",          EAX_CHORUS_S_P,             RIB_DEC),

      REG_PR("Get EAX4 Distortion parameters",  EAX_DISTORTION_P,           RIB_DEC),
      REG_PR("EAX4 Distortion parameters",      EAX_DISTORTION_S_P,         RIB_DEC),

      REG_PR("Get EAX4 Echo parameters",        EAX_ECHO_P,                 RIB_DEC),
      REG_PR("EAX4 Echo parameters",            EAX_ECHO_S_P,               RIB_DEC),

      REG_PR("Get EAX4 Equalizer parameters",   EAX_EQUALIZER_P,            RIB_DEC),
      REG_PR("EAX4 Equalizer parameters",       EAX_EQUALIZER_S_P,          RIB_DEC),

      REG_PR("Get EAX4 Flanger parameters",     EAX_FLANGER_P,              RIB_DEC),
      REG_PR("EAX4 Flanger parameters",         EAX_FLANGER_S_P,            RIB_DEC),

      REG_PR("Get EAX4 FShifter parameters",    EAX_FSHIFTER_P,             RIB_DEC),
      REG_PR("EAX4 FShifter parameters",        EAX_FSHIFTER_S_P,           RIB_DEC),

      REG_PR("Get EAX4 VMorpher parameters",    EAX_VMORPHER_P,             RIB_DEC),
      REG_PR("EAX4 VMorpher parameters",        EAX_VMORPHER_S_P,           RIB_DEC),

      REG_PR("Get EAX4 PShifter parameters",    EAX_PSHIFTER_P,             RIB_DEC),
      REG_PR("EAX4 PShifter parameters",        EAX_PSHIFTER_S_P,           RIB_DEC),

      REG_PR("Get EAX4 RModulator parameters",  EAX_RMODULATOR_P,           RIB_DEC),
      REG_PR("EAX4 RModulator parameters",      EAX_RMODULATOR_S_P,         RIB_DEC),

      REG_PR("Get EAX4 Reverb parameters",      EAX_REVERB_P,               RIB_DEC),
      REG_PR("EAX4 Reverb parameters",          EAX_REVERB_S_P,             RIB_DEC),
#endif
#endif
#endif

      REG_FN(PROVIDER_query_attribute),
      REG_FN(M3D_startup),
      REG_FN(M3D_error),
      REG_FN(M3D_shutdown),
      REG_FN(M3D_set_provider_preference),
      REG_FN(M3D_activate),
      REG_FN(M3D_allocate_3D_sample_handle),
      REG_FN(M3D_release_3D_sample_handle),
      REG_FN(M3D_start_3D_sample),
      REG_FN(M3D_stop_3D_sample),
      REG_FN(M3D_resume_3D_sample),
      REG_FN(M3D_end_3D_sample),
      REG_FN(M3D_set_3D_sample_data),
      REG_FN(M3D_set_3D_sample_volume),
      REG_FN(M3D_set_3D_sample_playback_rate),
      REG_FN(M3D_set_3D_sample_offset),
      REG_FN(M3D_set_3D_sample_loop_count),
      REG_FN(M3D_set_3D_sample_loop_block),
      REG_FN(M3D_3D_sample_status),
      REG_FN(M3D_3D_sample_volume),
      REG_FN(M3D_3D_sample_playback_rate),
      REG_FN(M3D_3D_sample_offset),
      REG_FN(M3D_3D_sample_length),
      REG_FN(M3D_3D_sample_loop_count),
      REG_FN(M3D_set_3D_sample_distances),
      REG_FN(M3D_3D_sample_distances),
      REG_FN(M3D_active_3D_sample_count),
      REG_FN(M3D_3D_open_listener),
      REG_FN(M3D_3D_close_listener),
      REG_FN(M3D_3D_open_object),
      REG_FN(M3D_3D_close_object),
      REG_FN(M3D_set_3D_position),
      REG_FN(M3D_set_3D_velocity),
      REG_FN(M3D_set_3D_velocity_vector),
      REG_FN(M3D_set_3D_orientation),
      REG_FN(M3D_3D_position),
      REG_FN(M3D_3D_velocity),
      REG_FN(M3D_3D_orientation),
      REG_FN(M3D_3D_update_position),
      REG_FN(M3D_3D_auto_update_position)
      };

   if (up_down)
      {
         RIB_register(provider_handle,
                     "MSS 3D audio services",
                      M3D);

         RIB_register(provider_handle,
                     "MSS 3D sample services",
                      M3DSAMPLE);

      } else {
         RIB_unregister_all(provider_handle);
      }

   return TRUE;
}

#ifdef EAX4
extern "C" S32 EAX3Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX2Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAXMain( HPROVIDER provider_handle, U32 up_down );

DXDEF S32 AILEXPORT RIB_MAIN_NAME(EAX)( HPROVIDER provider_handle, U32 up_down )
{
  static HPROVIDER EAXPH, EAX2PH, EAX3PH;

  if ( up_down )
  {
    EAX3PH = RIB_alloc_provider_handle(0);
    EAX2PH = RIB_alloc_provider_handle(0);
    EAXPH = RIB_alloc_provider_handle(0);
  }

  EAX4Main( provider_handle, up_down );
  EAX3Main( EAX3PH, up_down );
  EAX2Main( EAX2PH, up_down );
  EAXMain( EAXPH, up_down );

  return( TRUE );
}

#else
#ifdef AUREAL

extern "C" S32 a3d2Main( HPROVIDER provider_handle, U32 up_down );

DXDEF S32 AILEXPORT RIB_MAIN_NAME(A3D2)( HPROVIDER provider_handle, U32 up_down )
{
  static HPROVIDER a3d2PH;

  if ( up_down )
  {
    a3d2PH = RIB_alloc_provider_handle(0);
  }

  a3dMain( provider_handle, up_down );
  a3d2Main( a3d2PH, up_down );
  return( TRUE );
}

#else
#ifdef DX7SN

extern "C" S32 dx7snMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 dx7slMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 dx7shMain( HPROVIDER provider_handle, U32 up_down );

DXDEF S32 AILEXPORT RIB_MAIN_NAME(DX7)( HPROVIDER provider_handle, U32 up_down )
{
  static HPROVIDER dx7slPH, dx7shPH;

  if ( up_down )
  {
    dx7slPH = RIB_alloc_provider_handle(0);
    dx7shPH = RIB_alloc_provider_handle(0);
  }

  dx7snMain( provider_handle, up_down );
  dx7slMain( dx7slPH, up_down );
  dx7shMain( dx7shPH, up_down );
  return( TRUE );
}

#else
#if defined(SWARE) && !defined(DX7)

extern "C" S32 hwMain( HPROVIDER provider_handle, U32 up_down );

DXDEF S32 AILEXPORT RIB_Main( HPROVIDER provider_handle, U32 up_down )
{
  static HPROVIDER hwarePH;

  if ( up_down )
  {
    hwarePH = RIB_alloc_provider_handle(0);
  }

  swMain( provider_handle, up_down );
  hwMain( hwarePH, up_down );
  return( TRUE );
}

#endif
#endif
#endif
#endif

#if defined(EAX4) || defined(DX7SN) || (defined(SWARE) && !defined(DX7)) || defined(AUREAL)

BOOL WINAPI DllMain( HINSTANCE hinstDll, U32 fdwReason, LPVOID plvReserved )
{
  if ( fdwReason == DLL_PROCESS_ATTACH )
  {
    DisableThreadLibraryCalls( hinstDll );
  }
  return TRUE;
}

#endif
