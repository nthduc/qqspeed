//############################################################################
//##                                                                        ##
//##  EAX4EXAM.CPP                                                          ##
//##                                                                        ##
//##  V1.00 of 10-May-04: Initial version                                   ##
//##                                                                        ##
//##  Author: Jeff Roberts                                                  ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  For technical support, contact RAD Game Tools at 425-893-4300.        ##
//##                                                                        ##
//############################################################################

#include <stdio.h>
#include <stdlib.h>

#include "mss.h"

#include "con_util.i"

// @cdep pre $DefaultsWinEXE
// @cdep pre $requiresbinary(mss32.lib)
// @cdep post $BuildWinEXE

#define HW_FORMAT  DIG_F_STEREO_16
#define HW_RATE    44100

#define FILE3D MSS_DIR_UP "media" MSS_DIR_SEP "Welcome.wav"

//
// AIL handles
//

HDIGDRIVER dig = 0;
HPROVIDER provider = 0;
H3DSAMPLE sample = 0;

// ---------------------------------------------------------------------------
// shutdown
// ---------------------------------------------------------------------------

static void MSS_MAIN_DEF shutdown(void)
{
  AIL_end_3D_sample( sample );
  
  if ( provider )
  {
    AIL_close_3D_provider( provider );
    provider = 0;
  }
  
  if ( dig != NULL )
  {
    AIL_close_digital_driver( dig );
    dig = 0;
  }
  
  AIL_shutdown();
  
  printf("\nEAX4 Example stopped.\n");
}

//############################################################################
//##                                                                        ##
//## Load a sample file into memory (decompress it if necessary)            ##
//##                                                                        ##
//############################################################################

static void * load_sample_file( char * name, U32 * size )
{
  U32 * s;
  void * d;
  S32 type;
  AILSOUNDINFO info;
  
  s = (U32*) AIL_file_read( name, FILE_READ_WITH_SIZE );
  
  if ( s == 0 )
  {
    return( 0 );
  }
  
  type = AIL_file_type( s + 1, s [ 0 ] );
  
  switch ( type ) 
  {
   case AILFILETYPE_PCM_WAV:
     *size = s[ 0 ];
     return( s + 1 );
  
   case AILFILETYPE_ADPCM_WAV:
     AIL_WAV_info( s + 1, &info );
     AIL_decompress_ADPCM( &info, &d, size );
     AIL_mem_free_lock( s );
     return( d );
  
   default:
     AIL_mem_free_lock( s );
     return( 0 );
  }
}

//
// utility functions to handle the 3d volumes
//

S32 slot_volumes[ 4 ] = { -10000, -10000, -10000, -10000 };
S32 slot_priority[ 2 ] = { -1, -1 }; // only 2 eax4 slots can be active at once

static void set_3D_volumes( H3DSAMPLE sample )
{
  EAX_SAMPLE_SLOT_VOLUMES vols;
  EAX_SAMPLE_SLOT_VOLUMES vols2;
  U32 i;
  S32 direct;
  
  vols.NumVolumes = 0;
  if ( slot_priority[ 0 ] != -1 )
    if ( slot_priority[ 1 ] != -1 )
      vols.NumVolumes = 2;
    else
      vols.NumVolumes = 1;
  else
    vols.NumVolumes = 0;
  
  for ( i = 0 ; i < vols.NumVolumes ; i ++ )
  {
    vols.volumes[ i ].Slot = slot_priority[ i ];
    vols.volumes[ i ].Send = slot_volumes[ slot_priority[ i ] ];
    vols.volumes[ i ].SendHF = 0;
    vols.volumes[ i ].Occlusion = 0;
    vols.volumes[ i ].OcclusionLFRatio = 0.25f;
    vols.volumes[ i ].OcclusionRoomRatio = 1.5f;
    vols.volumes[ i ].OcclusionDirectRatio = 1.0f;
  }

  AIL_set_3D_sample_preference( sample, "EAX4 slot volumes", &vols );


  AIL_set_3D_sample_preference( sample, "Get EAX4 slot volumes", &vols2 );

  direct = -1000;
  AIL_set_3D_sample_preference( sample, "EAX2 sample direct", &direct );
}  


static void make_highest_priority( H3DSAMPLE sample, S32 slot )
{
  if ( slot_priority[ 0 ] != slot )
  {
    slot_priority[ 1 ] = slot_priority[ 0 ];
    slot_priority[ 0 ] = slot;
  }

  set_3D_volumes( sample );
}

static void change_volume( H3DSAMPLE sample, S32 slot, S32 change )
{
  slot_volumes[ slot ] += change;
  if ( slot_volumes[ slot ] < -10000 )
    slot_volumes[ slot ] = -10000;
  else if ( slot_volumes[ slot ] > 0 )
    slot_volumes[ slot ] = 0;
  
  make_highest_priority( sample, slot );
  
  if ( slot_priority[ 0 ] != -1 )
  {
    printf( "\rFirst active slot: %i at %i", slot_priority[ 0 ], slot_volumes[ slot_priority[ 0 ] ] );
    if ( slot_priority[ 1 ] != -1 )
    {
      printf( "  Second active slot: %i at %i     \r", slot_priority[ 1 ], slot_volumes[ slot_priority[ 1 ] ] );
    }
    else
    {
      printf( "     \r" );
    } 
  }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

void MSS_MAIN_DEF main( S32 argc, C8 ** argv )
{
  argc = argc;
  argv = argv;
  
  set_up_console( 1 );
  
  printf("_______________________________________________________________________________\n\n");
  printf("EAX 4 Example\n");
  printf("_______________________________________________________________________________\n\n");
  
  //
  // set the redist directory
  //
  
  AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );
  
  AIL_startup();
  atexit(shutdown);
  
  dig = NULL;
  
  dig = AIL_open_digital_driver( HW_RATE,
                                (HW_FORMAT & DIG_F_16BITS_MASK) ? 16 : 8,
                                (HW_FORMAT & DIG_F_16BITS_MASK) ? 2 : 1,
                                0 );
  
  
  //
  // Open EAX4 provider
  //
  
  HPROVIDER  current;
  C8   FAR * name;
  
  HPROENUM next = HPROENUM_FIRST;
  while ( AIL_enumerate_3D_providers( &next,
                                      &current,
                                      &name ) )
  
  {
    if ( strstr( name, "EAX 4" ) )
    {
      provider = current;
      break;
    }
  }
  
  //
  // Activate provider
  //
  
  S32 result = AIL_open_3D_provider( provider );
  
  if ( result != M3D_NOERR )
  {
    printf( "Error opening EAX4 provider, code %d\n", result );
    exit( 1 );
  }
  
  //
  // Allocate handle to play 3D sound
  //
  
  sample = AIL_allocate_3D_sample_handle( provider );
  

  //
  // the sample effects level only affects slot 0, but
  //   since this demo can use slot 0 when testing reverb, 
  //   we need to turn it on.
  //
  
  AIL_set_3D_sample_effects_level( sample, 1.0f );
  

  //
  // load sample data
  //
  
  U32        len_3d;
  void FAR * data_3d = load_sample_file( FILE3D, &len_3d );
  
  if ( data_3d == NULL )
  {
    printf( "Couldn't load " FILE3D "\n" );
    exit( 1 );
  }
  
  printf( "_______________________________________________________________________________\n\n");
  printf( "S         : Start a 3D sound\n\n" );
  
  printf( "R         : Turn on reverb in slot 0 (slot 0 is fixed to reverb)\n" );
  printf( "v/Shift-V : Turn on reverb in slot 2/3\n\n" );
  
  printf( "C         : Turn on chorus in slot 1 (slot 1 is fixed to chorus)\n" );
  printf( "h/Shift-H : Turn on chorus in slot 2/3\n\n" );

  printf( "d/Shift-D : Turn on distortion in slot 2/3\n" );
  printf( "e/Shift-E : Turn on echo in slot 2/3\n" );
  printf( "f/Shift-F : Turn on flange in slot 2/3\n" );
  printf( "m/Shift-M : Turn on ring modulation in slot 2/3\n" );
  printf( "a/Shift-A : Turn on autowah in slot 2/3\n" );
  printf( "p/Shift-P : Turn on pitch shifting in slot 2/3\n" );
  printf( "q/Shift-Q : Turn on frequency shifting in slot 2/3\n" );
  printf( "o/Shift-O : Turn on vocal morphing in slot 2/3\n\n" );
  
  printf( "0/Shift-0 : Decrease/increase volume of slot 0\n" );
  printf( "1/Shift-1 : Decrease/increase volume of slot 0\n" );
  printf( "2/Shift-2 : Decrease/increase volume of slot 0\n" );
  printf( "3/Shift-3 : Decrease/increase volume of slot 0\n\n" );
  
  printf("SPACE : Stop sound\n");
  printf("ESC   : Exit\n");
  printf("_______________________________________________________________________________\n\n");
  
  while ( 1 )
  {
    //
    // Give other threads a time slice
    //
    
    #ifdef IS_WIN32
      Sleep(10);
    #endif
    
    //
    // Poll keyboard
    //
    
    if ( kbhit() )
    {
      S32 ch = getch();
    
      switch ( ch )
      {
        //
        // ESC: Exit
        //
        case 27:
          exit( 0 );

        //
        // SPC: Stop all sounds
        //
        case ' ':
          AIL_end_3D_sample( sample );
          slot_volumes[ 0 ] = -10000;
          slot_volumes[ 1 ] = -10000;
          slot_volumes[ 2 ] = -10000;
          slot_volumes[ 3 ] = -10000;
          break;

        case 's': case 'S':
          AIL_end_3D_sample( sample );
          AIL_set_3D_sample_file( sample, data_3d );
          AIL_set_3D_sample_loop_count( sample, 0 );
          AIL_start_3D_sample( sample );
          set_3D_volumes( sample );
          break;
          
        case '0': change_volume( sample, 0, -1000 ); break;
        case ')': change_volume( sample, 0,  1000 ); break;
        case '1': change_volume( sample, 1, -1000 ); break;
        case '!': change_volume( sample, 1,  1000 ); break;
        case '2': change_volume( sample, 2, -1000 ); break;
        case '@': change_volume( sample, 2,  1000 ); break;
        case '3': change_volume( sample, 3, -1000 ); break;
        case '#': change_volume( sample, 3,  1000 ); break;
        
 
        //
        // example of using each effect
        //
 
        case 'd': case 'D':
        {
          EAX_DISTORTION dist_params;
          
          dist_params.Edge = 0.4f;
          dist_params.LowPassCutOff = 8000;
          dist_params.EQCenter = 3600;
          dist_params.EQBandwidth = 3600;
          dist_params.Gain = -2600;

          dist_params.Slot = ( ch == 'd' ) ? 2 : 3;
          dist_params.Volume = 0;
          
          AIL_set_3D_provider_preference( provider, "EAX4 Distortion parameters", &dist_params );
          change_volume( sample, dist_params.Slot, 10000 );
          break;
        }
          
        case 'a': case 'A':
        {
          EAX_AUTOWAH aw_params;
          
          aw_params.AttackTime = 0.06f;
          aw_params.ReleaseTime = 0.06f;
          aw_params.Resonance = 6000;
          aw_params.PeakLevel = 2100;

          aw_params.Slot = ( ch == 'a' ) ? 2 : 3;
          aw_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 Autowah parameters", &aw_params );
          change_volume( sample, aw_params.Slot, 10000 );
          break;
        }

        case 'e': case 'E':
        {
          EAX_ECHO echo_params;
          
          echo_params.Delay = 0.1f;
          echo_params.LRDelay = 0.1f;
          echo_params.Damping = 0.5f;
          echo_params.Feedback = 0.5f;
          echo_params.Spread = -1.0f;

          echo_params.Slot = ( ch == 'e' ) ? 2 : 3;
          echo_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 Echo parameters", &echo_params );
          change_volume( sample, echo_params.Slot, 10000 );
          break;
        }

        case 'f': case 'F':
        {
          EAX_FLANGER flanger_params;
          
          flanger_params.Waveform = 0;
          flanger_params.Phase = 86;
          flanger_params.Rate = 6;
          flanger_params.Depth = 1.0f;
          flanger_params.Feedback = -0.5f;
          flanger_params.Delay = 0.004f;

          flanger_params.Slot = ( ch == 'f' ) ? 2 : 3;
          flanger_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 Flanger parameters", &flanger_params );
          change_volume( sample, flanger_params.Slot, 10000 );
          break;
        }

        case 'm': case 'M':
        {
          EAX_RMODULATOR ringmod_params;
          
          ringmod_params.Frequency = 440.0f;
          ringmod_params.HighPassCutOff = 800.0f;
          ringmod_params.Waveform = 0;

          ringmod_params.Slot = ( ch == 'm' ) ? 2 : 3;
          ringmod_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 RModulator parameters", &ringmod_params );
          change_volume( sample, ringmod_params.Slot, 10000 );
          break;
        }
        
        case 'p': case 'P':
        {
          EAX_PSHIFTER pitch_params;
          
          pitch_params.CoarseTune = 12;
          pitch_params.FineTune = 0;

          pitch_params.Slot = ( ch == 'p' ) ? 2 : 3;
          pitch_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 PShifter parameters", &pitch_params );
          change_volume( sample, pitch_params.Slot, 10000 );
          break;
        }

        case 'q': case 'Q':
        {
          EAX_FSHIFTER freq_params;
          
          freq_params.Frequency = 270.0f;
          freq_params.LeftDirection = 0;
          freq_params.RightDirection = 1;

          freq_params.Slot = ( ch == 'q' ) ? 2 : 3;
          freq_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 FShifter parameters", &freq_params );
          change_volume( sample, freq_params.Slot, 10000 );
          break;
        }

        case 'o': case 'O':
        {
          EAX_VMORPHER morph_params;
          
          morph_params.PhonemeA = 0;
          morph_params.PhonemeACoarseTuning = 0;
          morph_params.PhonemeB = 10;
          morph_params.PhonemeBCoarseTuning = 0;
          morph_params.Waveform = 0;
          morph_params.Rate = 1.41f;

          morph_params.Slot = ( ch == 'o' ) ? 2 : 3;
          morph_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 VMorpher parameters", &morph_params );
          change_volume( sample, morph_params.Slot, 10000 );
          break;
        }

        case 'c': case 'C': case 'h': case 'H': 
        {
          EAX_CHORUS chorus_params;
          
          chorus_params.Waveform = 0;
          chorus_params.Phase = 30;
          chorus_params.Rate = 1.1f;
          chorus_params.Depth = 0.1f;
          chorus_params.Feedback = 0.7f;
          chorus_params.Delay = 0.016f;

          chorus_params.Slot = ( ( ch == 'c' ) || ( ch == 'C' ) ) ? 1 : ( ( ch == 'h' ) ? 2 : 3 );
          chorus_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 Chorus parameters", &chorus_params );
          change_volume( sample, chorus_params.Slot, 10000 );
          break;
        }

        case 'r': case 'R': case 'v': case 'V': 
        {
          EAX_REVERB reverb_params;
         
          reverb_params.Environment = 0;
          reverb_params.EnvironmentSize = 7.5f;
          reverb_params.EnvironmentDiffusion = 1.0f;
          reverb_params.Room = -1000;
          reverb_params.RoomHF = 0;
          reverb_params.RoomLF = -100;
          reverb_params.DecayTime = 1.43f;
          reverb_params.DecayHFRatio = 0.83f;
          reverb_params.DecayLFRatio = 1.0f;
          reverb_params.Reflections = -2602;
          reverb_params.ReflectionsDelay = 0.01f;
          reverb_params.ReflectionsPanX = 0.0f;
          reverb_params.ReflectionsPanY = 0.0f;
          reverb_params.ReflectionsPanZ = 0.0f;
          reverb_params.Reverb = 200;
          reverb_params.ReverbDelay = 0.01f;
          reverb_params.ReverbPanX = 0.0f;
          reverb_params.ReverbPanY = 0.0f;
          reverb_params.ReverbPanZ = 0.0f;
          reverb_params.EchoTime = 0.25f;
          reverb_params.EchoDepth = 0.0f;
          reverb_params.ModulationTime = 0.25f;
          reverb_params.ModulationDepth = 0.0f;
          reverb_params.AirAbsorptionHF = -5.0f;
          reverb_params.HFReference = 5000.0f;
          reverb_params.LFReference = 250.0f;
          reverb_params.RoomRolloffFactor = 0.0f;
          reverb_params.Flags = 0xff; // see EAX 4 docs

          reverb_params.Slot = ( ( ch == 'r' ) || ( ch == 'R' ) ) ? 0 : ( ( ch == 'v' ) ? 2 : 3 );
          reverb_params.Volume = 0;

          AIL_set_3D_provider_preference( provider, "EAX4 Reverb parameters", &reverb_params );
          change_volume( sample, reverb_params.Slot, 10000 );
          break;
        }

      }
    }
  }
}
