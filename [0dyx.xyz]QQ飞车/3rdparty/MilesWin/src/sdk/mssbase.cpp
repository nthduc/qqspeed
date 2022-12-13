//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: M3D module for Dolby Surround and 2D providers               ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.01 of 23-Nov-98: Initial                                    ##
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

#define diag_printf // AIL_debug_printf

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

#include "mss.h"
#include "imssapi.h"

#define DEFAULT_REAR_ADVANCE 32

static S32 M3D_started = 0;

static C8 M3D_error_text[256];

#define SPEED_OF_SOUND 0.355F   // Speed of sound in meters per millisecond
#define DOPPLER_THETA  1.000F   // cos(theta) assuming 0-degree angular displacement
#define PI (3.141592654f)

//
// Epsilon value used for FP comparisons with 0
//

#define EPSILON 0.0001F

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

#ifdef USE_REAR
  REAR_ADVANCE,
#endif
};

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
  F32 x;
  F32 y;
  F32 z;
};

//
// Sample descriptor
//

struct SAMPLE3D
{
  OBJTYPE  type;                // Runtime type of object

  U32      len;                 // Sample buffer size in bytes (W)
  F32      volume;              // Sample volume 0-1.0
  S32      playback_rate;

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

  S32     is_valid;                   // TRUE if OK to calculate Dolby parms
  S32     doppler_valid;              // TRUE if OK to calculate Doppler shift

  HSAMPLE samp;                       // HSAMPLE representing left-right axis

  F32 effects_level;
  F32 wet_level;
  F32 dry_level;

  F32 occlusion;
};

//
// Support up to 64 samples (arbitrary limit)
//

#define MAX_SAMPLES 64

static SAMPLE3D samples[MAX_SAMPLES];
static S32 avail_samples = -1;
static S32 n_samples = 64;

//
// Globals
//

static S32 active = 0;

static HDIGDRIVER  dig;

static S32 room_type = 0;

static S32 mute_at_max = 1;

static DS3VECTOR3D listen_position;
static DS3VECTOR3D listen_face;
static DS3VECTOR3D listen_up;
static DS3VECTOR3D listen_cross;
static DS3VECTOR3D listen_velocity;
static S32         listen_auto_update = 0;

static HTIMER      service_timer;

#ifdef USE_REAR
static S32         rear_advance = DEFAULT_REAR_ADVANCE;
static HPROVIDER   dolby;
#endif

static F32 rolloff_factor = 1.0f;
static F32 doppler_factor = 1.0f;
static F32 distance_factor = 1.0f;



static void __inline RAD_vector_cross_product(DS3VECTOR3D *c,
                                       const DS3VECTOR3D *v1,
                                       const DS3VECTOR3D *v2)
{
  c->x = v1->z * v2->y - v1->y * v2->z;
  c->y = v1->x * v2->z - v1->z * v2->x;
  c->z = v1->y * v2->x - v1->x * v2->y;
}


static void __inline RAD_vector_subtract(DS3VECTOR3D *d,
                                  const DS3VECTOR3D *v1,
                                  const DS3VECTOR3D *v2)
{
  d->x = v1->x - v2->x;
  d->y = v1->y - v2->y;
  d->z = v1->z - v2->z;
}


static void __inline RAD_vector_normalize(DS3VECTOR3D *v)
{
  F32 len = (F32) sqrt((v->x * v->x) +
                       (v->y * v->y) +
                       (v->z * v->z));
  if (len==0.0)
  {
    v->x=1.0;
    v->y=0.0;
    v->z=0.0;
  }
  else
  {
    v->x /= len;
    v->y /= len;
    v->z /= len;
  }
}


static F32 __inline RAD_vector_dot_product(const DS3VECTOR3D *v1,
                                    const DS3VECTOR3D *v2)
{
  F32 dot;

  dot  = v1->x * v2->x;
  dot += v1->y * v2->y;
  dot += v1->z * v2->z;

  return( dot );
}


//############################################################################
//##                                                                        ##
//## Calculate pan, volume, and frequency for 2D HSAMPLE pair based on 3D   ##
//## space                                                                  ##
//##                                                                        ##
//############################################################################

static void DOLBY_update_sample(SAMPLE3D FAR *S)
{
  //
  // Return if sample uninitialized
  //

  if ((!S->is_valid) || (S->samp==0))
  {
    return;
  }

  //
  // Get absolute 3D distance between listener and sample
  //

  F32 dx = S->position.x - listen_position.x;
  F32 dy = S->position.y - listen_position.y;
  F32 dz = S->position.z - listen_position.z;

  F32 distance = (F32) sqrt((dx*dx) + (dy*dy) + (dz*dz));

  if ( mute_at_max )
  {

    //
    // Pause the sound if the distance is greater than the maximum distance
    //

    if (distance > S->max_dist)
    {
      if (AIL_sample_status(S->samp) != SMP_STOPPED)
      {
        AIL_set_sample_volume_pan(S->samp,0,0.5f);
      }

      return;
    }
  }
  else
  {
 
    //
    // Inhibit further attenuation at max dist
    //

    if (distance > S->max_dist)
    {
      distance = S->max_dist;
    }
  }

  //
  // Calculate base volume attenuation
  //

  F32 vol= (F32) pow( S->volume, 10.0f / 6.0f );

  //
  // Calculate distance attenuation by inverse-square law
  //

  F32 min            = S->min_dist;
  F32 distance_atten = 0.0F;

  if (min < EPSILON)
  {
    min = EPSILON;
  }

  if (distance > ( min + EPSILON ) )
  {
    //
    // The volume of a sound attenuated by 6dB at twice the minimum distance
    // from the listener, 6 dB again at 4X the minimum distance, and so on
    //

    vol = vol * ( min / ( min + rolloff_factor * ( distance - min ) ) );
  }

  DS3VECTOR3D diff;

  RAD_vector_subtract(&diff,&S->position,&listen_position);
  RAD_vector_normalize(&diff);

  F32 fb_dot=RAD_vector_dot_product(&listen_face,&diff);

  F32 lr_pan;

  //
  // Determine whether source is to the left or right of the listener
  //

  if (distance<=EPSILON)
  {
    lr_pan = 0.5f;
  }
  else
  {
    //
    // (face cross up) dot (the diff in position as a unit vector) gives
    //   an angle from 180 (all left) to 0 (all right).  we just scale
    //   the angle to our pan setting.
    //
    
    F32 dot=RAD_vector_dot_product(&listen_cross,&diff);
    lr_pan = (F32)acos(dot)/PI;
  }

#ifdef USE_REAR

  F32 fb_pan;

  if (distance<=EPSILON)
  {
    fb_pan = 0.5f;
  }
  else
  {
    //
    // (face cross up) dot (the diff in position as a unit vector) gives
    //   an angle from 180 (all left) to 0 (all right).  we just scale
    //   the angle to our pan setting.
    //

    fb_pan = (F32)acos(fb_dot)/PI;

  }

  AIL_set_sample_volume_levels(S->samp,vol * lr_pan, vol * ( 1.0f - lr_pan ) );

  AIL_set_filter_sample_preference(S->samp,"FB Pan",&fb_pan);

#else

  //
  // In 2D-only mode, back the amplitude off by 3 dB if the sound is to
  // the rear of the listener
  //

  if ( fb_dot < 0.0f )
  {
    vol = vol * 0.75f; // 3db down
  }

  AIL_set_sample_volume_levels(S->samp,vol * lr_pan, vol * ( 1.0f - lr_pan ) );

#endif

  //
  // Apply Doppler frequency-shifting if enabled for this sample
  //

  if ((fabs(listen_velocity.x) > EPSILON) ||
      (fabs(listen_velocity.y) > EPSILON) ||
      (fabs(listen_velocity.z) > EPSILON))
  {
    S->doppler_valid = 1;
  }

  if (S->doppler_valid)
  {
    F32 dx = S->velocity.x - listen_velocity.x;
    F32 dy = S->velocity.y - listen_velocity.y;
    F32 dz = S->velocity.z - listen_velocity.z;

    F32 velocity = (F32) sqrt((dx*dx) + (dy*dy) + (dz*dz));

    //
    // Sign of velocity determined by whether object is approaching
    // source or receding from it
    //

    dx = (S->position.x + S->velocity.x) - (listen_position.x + listen_velocity.x);
    dy = (S->position.y + S->velocity.y) - (listen_position.y + listen_velocity.y);
    dz = (S->position.z + S->velocity.z) - (listen_position.z + listen_velocity.z);

    F32 new_distance = (F32) sqrt((dx*dx) + (dy*dy) + (dz*dz));

    if (new_distance < distance)
    {
      //
      // Source and listener approaching each other -- flip sign of
      // velocity to create shift towards higher frequencies
      //

      velocity = -velocity;
    }

    //
    // Apply Doppler shift to playback rate
    //
   
    F32 f_obs = ((F32) S->playback_rate) *
       (SPEED_OF_SOUND / (SPEED_OF_SOUND + (velocity * distance_factor * doppler_factor * DOPPLER_THETA)));

    AIL_set_sample_playback_rate(S->samp, (S32) f_obs);
  }
  else
  {
    AIL_set_sample_playback_rate( S->samp, S->playback_rate );
  }

  AIL_set_sample_reverb_levels( S->samp, S->dry_level, S->wet_level*S->effects_level);
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_room_type                                                       #
//#                                                                            #
//##############################################################################

static void		  AILEXPORT M3D_set_3D_room_type (S32 EAX_room_type)
{
  room_type = EAX_room_type;
  AIL_set_digital_master_room_type( dig, EAX_room_type );
}

//##############################################################################
//#                                                                            #
//# M3D_3D_room_type                                                           #
//#                                                                            #
//##############################################################################

static S32		 AILEXPORT M3D_3D_room_type (void)
{
  return( room_type );
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_effects_level                                                #
//#                                                                            #
//##############################################################################

static F32		  AILEXPORT M3D_3D_sample_effects_level (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  return( S->effects_level );
}


//##############################################################################
//#                                                                            #
//# M3D_set_3D_rolloff_factor                                                  #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_rolloff_factor (F32 factor)
{
  rolloff_factor = factor;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_rolloff_factor                                                      #
//#                                                                            #
//##############################################################################

static F32       AILEXPORT M3D_3D_rolloff_factor (void)
{
  return( rolloff_factor );
}


//##############################################################################
//#                                                                            #
//# M3D_set_3D_doppler_factor                                                  #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_doppler_factor (F32 factor)
{
  doppler_factor = factor;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_doppler_factor                                                      #
//#                                                                            #
//##############################################################################

static F32       AILEXPORT M3D_3D_doppler_factor (void)
{
  return( doppler_factor );
}


//##############################################################################
//#                                                                            #
//# M3D_set_3D_distance_factor                                                 #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_set_3D_distance_factor (F32 factor)
{
  distance_factor = factor;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_distance_factor                                                     #
//#                                                                            #
//##############################################################################

static F32       AILEXPORT M3D_3D_distance_factor (void)
{
  return( distance_factor );
}


static void AILCALLBACK eos3d(HSAMPLE samp)
{
  AIL3DSAMPLECB cb = (AIL3DSAMPLECB) AIL_sample_user_data( samp, 0 );
  void* client = (void*) AIL_sample_user_data( samp, 1 );

  if ((cb) && (client))
  {
    cb((H3DSAMPLE)client);
  }
}


//##############################################################################
//#                                                                            #
//# M3D_set_3D_EOS                                                             #
//#                                                                            #
//##############################################################################

static AIL3DSAMPLECB AILEXPORT M3D_set_3D_EOS     (void * client,
                                                   H3DSAMPLE samp,
                                                   AIL3DSAMPLECB eos)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return(0);
  }

  AIL3DSAMPLECB prev=(AIL3DSAMPLECB)AIL_sample_user_data(S->samp,0);

  AIL_set_sample_user_data( S->samp, 0, (S32)eos);
  AIL_set_sample_user_data( S->samp, 1, (S32)client);
  AIL_register_EOS_callback( S->samp, eos3d );

  return( prev );
}

//##############################################################################
//#                                                                            #
//# M3D_set_3D_sample_effects_level															 #
//#                                                                            #
//##############################################################################

static void		  AILEXPORT M3D_set_3D_sample_effects_level(H3DSAMPLE samp, //)
                                                                    F32 effects_level)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return;
  }

  if ( effects_level < 0.0f )
    effects_level = -effects_level;

  S->effects_level = effects_level;

  DOLBY_update_sample(S);
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_obstruction                                                  #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_obstruction (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return -1.0f;
  }

  return 1.0f-S->dry_level;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_exclusion                                                    #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_exclusion (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return -1.0f;
  }

  return 1.0f-S->wet_level;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_occlusion                                                    #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_occlusion (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return 0;
  }

  return( S->occlusion );
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

  if (S->samp == 0)
  {
    return;
  }

  S->dry_level = 1.0f-obstruction;
  DOLBY_update_sample(S);
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

  if (S->samp == 0)
  {
    return;
  }

  S->wet_level = 1.0f-exclusion;
  DOLBY_update_sample(S);
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

  if (S->samp == 0)
  {
    return;
  }

  S->occlusion = occlusion;

  AIL_set_sample_low_pass_cut_off( S->samp, ( 1.00f - occlusion ) + 0.01f );
}

//############################################################################
//##                                                                        ##
//## Recalculate pan, volume, and frequency for all 2D HSAMPLEs based on    ##
//## listener attributes                                                    ##
//##                                                                        ##
//############################################################################

static void DOLBY_update_listener(void)
{
  S32 i;

  for (i=0; i < avail_samples; i++)
  {
    SAMPLE3D FAR *S = &samples[i];

    if ( S->samp )
    {
      DOLBY_update_sample(S);
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
#ifdef USE_REAR
    case PROVIDER_NAME:     return (U32) "Dolby Surround";
    case REAR_ADVANCE:      return rear_advance;
#else
    case PROVIDER_NAME:     return (U32) "Miles Fast 2D Positional Audio";
#endif
    case PROVIDER_VERSION:  return 0x101;

    case MAX_SUPPORTED_SAMPLES: return( (avail_samples!=-1)?avail_samples:n_samples );
    case MUTE_AT_MAX:       return( mute_at_max );
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
  if (M3D_started)
  {
    strcpy(M3D_error_text,"Already started");
    return M3D_ALREADY_STARTED;
  }

  ++M3D_started;

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

//############################################################################
//#                                                                          #
//# Set provider preference value, returning previous setting                #
//#                                                                          #
//############################################################################

static S32 AILEXPORT M3D_set_provider_preference (HATTRIB    preference, //)
                                           void FAR*  value)
{
  S32 prev = -1;

  switch ((ATTRIB) preference)
  {
    //
    // Preferences
    //

    case MAX_SUPPORTED_SAMPLES:
      n_samples=*(U32*)value;
      if (n_samples>MAX_SAMPLES)
        n_samples=MAX_SAMPLES;
      break;

    case MUTE_AT_MAX:
      mute_at_max=*(U32*)value;
      break;

#ifdef USE_REAR
    case REAR_ADVANCE:
      prev = rear_advance;
      rear_advance = *(S32 FAR*)value;
      break;
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
    if (samples[i].samp == 0)
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

  S->samp = AIL_allocate_sample_handle( dig );

  if ( S->samp == 0 )
  {
    return( 0 );
  }

  //
  // Initialize sample to default (SMP_DONE) status with nominal
  // sample attributes
  //

  S->is_valid        = 0;
  S->doppler_valid   = 0;

  S->len             =  0;

  S->volume          =  1.0f;

  S->auto_update     =  0;

  S->effects_level   = 0.0f;
  S->dry_level       = 1.0f;
  S->wet_level       = 1.0f;
  S->occlusion       = 0.0f;

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

  if (S->samp == 0)
  {
    return;
  }

  AIL_release_sample_handle(S->samp);

  //
  // Mark sample available for immediate reallocation
  //

  S->samp = 0;
}

//##############################################################################
//#                                                                            #
//# M3D_start_3D_sample                                                        #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_start_3D_sample         (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return;
  }

  DOLBY_update_sample(S);

  AIL_start_sample( S->samp );
}

//##############################################################################
//#                                                                            #
//# M3D_stop_3D_sample                                                         #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_stop_3D_sample          (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  AIL_stop_sample(S->samp);
}

//##############################################################################
//#                                                                            #
//# M3D_resume_3D_sample                                                       #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_resume_3D_sample        (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  AIL_resume_sample(S->samp);
}

//##############################################################################
//#                                                                            #
//# M3D_end_3D_sample                                                          #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_end_3D_sample        (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return;
  }

  AIL_end_sample(S->samp);
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

  if (S->samp == 0)
  {
    return;
  }

  S->volume = volume;

  DOLBY_update_sample(S);
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

  if (S->samp == 0)
  {
    return;
  }


  S->playback_rate = playback_rate;

  DOLBY_update_sample(S);
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

  if (S->samp == 0)
  {
    return;
  }

  AIL_set_sample_position( S->samp, offset );
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

  if (S->samp == 0)
  {
    return;
  }

  AIL_set_sample_loop_count( S->samp, loops );
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

  if (S->samp == 0)
  {
    return;
  }

  AIL_set_sample_loop_block( S->samp,loop_start_offset, loop_end_offset );
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_volume                                                       #
//#                                                                            #
//##############################################################################

static F32        AILEXPORT M3D_3D_sample_volume (H3DSAMPLE samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return(-1.0f);
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

  if (S->samp == 0)
  {
    return( 0 );
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

  if (S->samp == 0)
  {
    return( SMP_FREE );
  }

  return( AIL_sample_status( S->samp ) );
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_offset                                                       #
//#                                                                            #
//##############################################################################

static U32        AILEXPORT M3D_3D_sample_offset        (H3DSAMPLE     samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return(0);
  }

  return( AIL_sample_position( S->samp) );
}

//##############################################################################
//#                                                                            #
//# M3D_3D_sample_length                                                       #
//#                                                                            #
//##############################################################################

static U32        AILEXPORT M3D_3D_sample_length        (H3DSAMPLE     samp)
{
  SAMPLE3D FAR *S = (SAMPLE3D FAR *) samp;

  if (S->samp == 0)
  {
    return( 0 );
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

  if (S->samp == 0)
  {
    return( 0 );
  }

  return( AIL_sample_loop_count( S->samp ) );
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

  if (S->samp == 0)
  {
    return;
  }

  S->max_dist = max_dist;
  S->min_dist = min_dist;

  DOLBY_update_sample(S);
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

  if (S->samp == 0)
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
  return 0;
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
    if (samples[i].samp)
    {
      if (AIL_sample_status(samples[i].samp) == SMP_PLAYING)
      {
        ++n;
      }
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
}

//##############################################################################
//#                                                                            #
//# M3D_3D_open_object                                                         #
//#                                                                            #
//##############################################################################

static H3DPOBJECT AILEXPORT M3D_3D_open_object          (void)
{
  return NULL;
}

//##############################################################################
//#                                                                            #
//# M3D_3D_close_object                                                        #
//#                                                                            #
//##############################################################################

static void       AILEXPORT M3D_3D_close_object         (H3DPOBJECT obj)
{
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

    DOLBY_update_sample(S);
  } 
  else if (*t == IS_LISTENER)
  {
    listen_position.x = X;
    listen_position.y = Y;
    listen_position.z = Z;

    DOLBY_update_listener();
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

    //
    // Enable Doppler calculations if a non-zero velocity value is set
    //

    if ((fabs(S->velocity.x) > EPSILON) ||
        (fabs(S->velocity.y) > EPSILON) ||
        (fabs(S->velocity.z) > EPSILON))
    {
      S->doppler_valid = 1;
    }

    DOLBY_update_sample(S);
  }
  else if (*t == IS_LISTENER)
  {
    listen_velocity.x = dX_per_ms;
    listen_velocity.y = dY_per_ms;
    listen_velocity.z = dZ_per_ms;

    DOLBY_update_listener();
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

  //
  // Assign orientation to specified object
  //

  if (*t == IS_SAMPLE)
  {
    SAMPLE3D FAR *S = (SAMPLE3D FAR *) obj;

    S->face.x = X_face;
    S->face.y = Y_face;
    S->face.z = Z_face;

    S->up.x = X_up;
    S->up.y = Y_up;
    S->up.z = Z_up;
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

    RAD_vector_cross_product(&listen_cross, &listen_face, &listen_up);

    DOLBY_update_listener();
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
    if (Y_up)   *Y_up   = listen_up.y;
    if (Z_up)   *Z_up   = listen_up.z;
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

  if (S->samp == 0)
  {
    return 0;
  }

  S->is_valid = 0;

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
    AIL_set_error("Not mono data");
    return 0;
  }

  //
  // End and re-initialize samples assigned to this voice
  //

  AIL_end_sample(S->samp);

  AIL_init_sample(S->samp);

  AIL_set_sample_user_data( S->samp, 0, 0);
  AIL_set_sample_user_data( S->samp, 1, 0);

  AIL_set_sample_address(S->samp,info->data_ptr,info->data_len);

  S->len           =  info->data_len;

  S->volume        =  1.0f;
  S->playback_rate =  info->rate;

  S32 word_mask   = (info->bits == 16) ? DIG_F_16BITS_MASK : 0;
  S32 sample_type = (info->bits == 16) ? DIG_PCM_SIGN      : 0;

  AIL_set_sample_type(S->samp,
                      word_mask,
                      sample_type);

  AIL_set_sample_playback_rate(S->samp, S->playback_rate);

  //
  // Set default emitter distances
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

  M3D_set_3D_sample_effects_level(samp, 0.0F);

  M3D_set_3D_sample_obstruction  (samp, 0.0F);
  M3D_set_3D_sample_occlusion    (samp, 0.0F);
  M3D_set_3D_sample_exclusion    (samp, 0.0F);

  #ifdef USE_REAR
  AIL_set_sample_processor(S->samp, DP_FILTER,dolby);
  
  AIL_set_filter_sample_preference(S->samp,"Rear Delay",&rear_advance);
  #endif

  //
  // Mark sample valid and force calculation of initial conditions
  //

  S->is_valid = 1;

  DOLBY_update_sample(S);

  return 1;
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

    if ((S->samp==0) || ( AIL_sample_status( S->samp ) != SMP_PLAYING) )
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
    // Get handle to primary driver (by default, first driver allocated)
    //
    // If this is NULL, it means the digital subsystem hasn't been
    // initialized yet
    //

    dig = AIL_primary_digital_driver(NULL);

    if (dig == NULL)
    {
      AIL_set_error("2D subsystem invalid or uninitialized");
      return M3D_NOT_INIT;
    }

    //
    // Set default rear advance in samples
    //

#ifdef USE_REAR
    rear_advance = DEFAULT_REAR_ADVANCE;
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

    avail_samples = n_samples;

    memset(samples,
           0,
           sizeof(samples));

#ifdef USE_REAR
    HPROENUM next=HPROENUM_FIRST;
    HPROVIDER dest;
    char* str;

    dolby=0;

    while ((AIL_enumerate_filters(&next, &dest,&str)) && (dolby==0))
    {
      if (AIL_strcmp(str,"Dolby Surround Filter")==0)
        dolby=dest;
    }

    if ( dolby == 0 )
    {
      AIL_set_error("Could not find the Miles Dolby Surround Filter");
      return M3D_NOT_INIT;
    }
#endif

    //
    // Register and start service timer
    //

    service_timer = AIL_register_timer(M3D_serve);

    AIL_set_timer_period(service_timer, SERVICE_MSECS * 1000);

    AIL_start_timer(service_timer);

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

    //
    // Deallocate resources associated with samples
    //

    S32 i;

    for (i=0; i < avail_samples; i++)
    {
      SAMPLE3D FAR *S = &samples[i];

      HSAMPLE s = S->samp;
      S->samp = 0;
      AIL_release_sample_handle( s );
    }

    active = 0;

    room_type = 0;
  }

  return M3D_NOERR;
}


//
// Application must supply the provider handle explicitly
//

#ifdef USE_REAR
extern "C" S32 DolbyMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 DolbyMain( HPROVIDER provider_handle, U32 up_down )
#else
extern "C" S32 MilesFastMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 MilesFastMain( HPROVIDER provider_handle, U32 up_down )
#endif
{
  const RIB_INTERFACE_ENTRY M3DSAMPLE[] =
  {
    REG_FN(M3D_set_3D_sample_obstruction),
    REG_FN(M3D_3D_sample_obstruction),
    REG_FN(M3D_set_3D_sample_exclusion),
    REG_FN(M3D_3D_sample_exclusion),
    REG_FN(M3D_set_3D_sample_occlusion),
    REG_FN(M3D_3D_sample_occlusion),

    REG_FN(M3D_set_3D_EOS),
    REG_FN(M3D_set_3D_sample_effects_level),
    REG_FN(M3D_3D_sample_effects_level),
    REG_FN(M3D_set_3D_room_type),
    REG_FN(M3D_3D_room_type),

    REG_FN(M3D_3D_sample_query_attribute),
    REG_FN(M3D_3D_set_sample_preference),

    REG_FN(M3D_set_3D_rolloff_factor),
    REG_FN(M3D_3D_rolloff_factor),

    REG_FN(M3D_set_3D_doppler_factor),
    REG_FN(M3D_3D_doppler_factor),

    REG_FN(M3D_set_3D_distance_factor),
    REG_FN(M3D_3D_distance_factor),
  };

  const RIB_INTERFACE_ENTRY M3D[] =
  {
    REG_AT("Name",                       PROVIDER_NAME,         RIB_STRING),
    REG_AT("Version",                    PROVIDER_VERSION,      RIB_HEX),
    REG_AT("Maximum supported samples",  MAX_SUPPORTED_SAMPLES, RIB_DEC),
    REG_PR("Maximum supported samples",  MAX_SUPPORTED_SAMPLES, RIB_DEC),
    REG_AT("Mute at maximum",            MUTE_AT_MAX, RIB_DEC),
    REG_PR("Mute at maximum",            MUTE_AT_MAX, RIB_DEC),

#ifdef USE_REAR
    REG_AT("Rear advance samples",       REAR_ADVANCE,          RIB_DEC),
    REG_PR("Rear advance samples",       REAR_ADVANCE,          RIB_DEC),
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
  }
  else
  {
    RIB_unregister_all(provider_handle);
  }

  return TRUE;
}

#ifdef USE_REAR

extern "C" S32 MilesFastMain( HPROVIDER provider_handle, U32 up_down );


DXDEF S32 AILEXPORT RIB_MAIN_NAME(SOFT3D)( HPROVIDER provider_handle, U32 up_down )
{
  static HPROVIDER fastPH;

  if ( up_down )
  {
    fastPH = RIB_alloc_provider_handle(0);
  }

  DolbyMain( provider_handle, up_down );
  MilesFastMain( fastPH, up_down );
  return( TRUE );
}

#ifdef IS_WIN32

BOOL WINAPI DllMain( HINSTANCE hinstDll, U32 fdwReason, LPVOID plvReserved )
{
  if ( fdwReason == DLL_PROCESS_ATTACH )
  {
    DisableThreadLibraryCalls( hinstDll );
  }
  return TRUE;
}

#endif

#endif

