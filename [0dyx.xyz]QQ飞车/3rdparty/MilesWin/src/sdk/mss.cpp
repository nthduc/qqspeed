//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  MSS.C: C API module and support routines                              ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 9.0                  ##
//##                                                                        ##
//##  Version 1.00 of 15-Feb-95: Derived from AIL.C V1.00                   ##
//##          1.01 of  1-May-95: Moved WAILA functions here for portability ##
//##          1.02 of 15-Fed-96: Additions for multiple 16 bit loads (JKR)  ##
//##          1.03 of 11-Apr-96: Win32s updates (JKR)                       ##
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

#include "mss.h"
#include "imssapi.h"

//
// Handle to application thread (thread which called AIL_startup())
//

HANDLE hAppThread;

#ifdef IS_WIN32

extern "C" HWND MSShWnd = NULL;

#endif

HINSTANCE MSS_Instance;
char MSS_Directory[260];

//
// AIL "preferences" array
//

S32 AIL_preference[N_PREFS];

//
// ASCII error type string
//

C8 AIL_error[256];

//
// Background semaphore flag
//

volatile S32 AIL_bkgnd_flag=0;

//
// DIG_DRIVER list
//

HDIGDRIVER DIG_first = NULL;

//
// MDI_DRIVER list
//

HMDIDRIVER MDI_first = NULL;

//
// Timer array
//

static struct _AILTIMER FAR *timers = NULL;
static S32                   n_timers;

//
// Period of base multimedia timer in uS
//

static S32                   timer_period;

//
// Locking count
//

static S32                   lock_count=0;


#ifdef IS_WIN32

typedef struct THREADNAME_INFO
{
  DWORD dwType;
  LPCSTR szName;
  DWORD dwThreadID;
  DWORD dwFlags;
} THREADNAME_INFO;

void AILCALL Set_thread_name( DWORD thread_id, char const * thread_name )
{
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = thread_name;
  info.dwThreadID = thread_id;
  info.dwFlags = 0;

  __try
  {
      RaiseException( 0x406d1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
  }
  __except( EXCEPTION_CONTINUE_EXECUTION )
  {
  }
}

#else

#ifdef IS_WIN16

//
// Timer ID for multimedia service
//

extern "C"
{

static U32                   timer_ID;

void FAR MSSGetTaskInfo(S32 FAR* ct, S32 FAR* cds)
{
  HANDLE h;
  U16 t=(U16)GetCurrentTask();
  *ct=t;
  h=*((HANDLE FAR*)MAKELP(t,0x1c));
  *cds=((U32)GlobalLock(h))>>16;
  GlobalUnlock(h);
}


static S32                  backgroundCT=-2;  //will always be -2 except
                                              //in a timer callback

static U8 ok_CT(S32 val)
{
  S32 curtask=(U16)GetCurrentTask();
  return( ((val==curtask) || (backgroundCT==curtask) || (backgroundCT==-1)) ?1:0 );
}


}

#endif

#endif

//############################################################################
//##                                                                        ##
//## Set AIL operational preferences and policies                           ##
//##                                                                        ##
//## May be called by applications which need to alter the default          ##
//## behavior of the AIL system                                             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_set_preference(U32 number, S32 value)
{
   S32 old;

   old = AIL_preference[number];

#ifdef IS_WIN16
   if (number != DIG_USE_WAVEOUT)
#endif

   AIL_preference[number] = value;

   return old;
}

//############################################################################
//##                                                                        ##
//## Get AIL operational preferences and policies                           ##
//##                                                                        ##
//############################################################################

DXDEF
S32 AILEXPORT AIL_get_preference(U32 number)
{
   return AIL_preference[number];
}

static void init_preferences()
{
   AIL_preference[DIG_MIXER_CHANNELS]=        DEFAULT_DMC;
   AIL_preference[DIG_MAX_PREDELAY_MS]=       DEFAULT_MPDMS;
   AIL_preference[DIG_RESAMPLING_TOLERANCE]=  DEFAULT_DRT;
   AIL_preference[DIG_ENABLE_RESAMPLE_FILTER]=DEFAULT_DERF;
   AIL_preference[DIG_DS_FRAGMENT_SIZE]=      DEFAULT_DDFS;
   AIL_preference[DIG_DS_FRAGMENT_CNT]=       DEFAULT_DDFC;

#ifndef IS_XBOX

   AIL_preference[DIG_OUTPUT_BUFFER_SIZE]=    DEFAULT_DOBS;
   AIL_preference[DIG_MAX_CHAIN_ELEMENT_SIZE]=DEFAULT_MCES;
   AIL_preference[DIG_MIN_CHAIN_ELEMENT_TIME]=DEFAULT_MCET;
   AIL_preference[DIG_DS_DSBCAPS_CTRL3D]=     DEFAULT_DDDC;
   AIL_preference[DIG_DS_SECONDARY_SIZE]=     DEFAULT_DDSS;
#ifdef IS_WIN32
   AIL_preference[DIG_USE_WAVEOUT]=           DEFAULT_DUW;
#else                         // WAVEOUT must be used under Win16
   AIL_preference[DIG_USE_WAVEOUT]=           YES;
#endif
   AIL_preference[DIG_DS_MIX_FRAGMENT_CNT]=   DEFAULT_DDMFC;
   AIL_preference[DIG_DS_USE_PRIMARY]=        DEFAULT_DDUP;
   AIL_preference[DIG_DS_SAMPLE_CEILING]=     DEFAULT_DDSC;
   AIL_preference[DIG_DS_CREATION_HANDLER]=   DEFAULT_DDCH;
   AIL_preference[MDI_DOUBLE_NOTE_OFF]=       DEFAULT_MDNO;

   AIL_preference[DIG_INPUT_LATENCY]=DEFAULT_DIL;
   AIL_preference[DIG_USE_WAVEIN]=DEFAULT_DUWI;
#endif

   AIL_preference[DIG_3D_MUTE_AT_MAX]=        DEFAULT_D3MAM;
   AIL_preference[MDI_SYSEX_BUFFER_SIZE]=     DEFAULT_MSBS;
   AIL_preference[MDI_SERVICE_RATE]=          DEFAULT_MSR;
   AIL_preference[MDI_SEQUENCES]=             DEFAULT_MS;
   AIL_preference[MDI_DEFAULT_VOLUME]=        DEFAULT_MDV;
   AIL_preference[MDI_QUANT_ADVANCE]=         DEFAULT_MQA;
   AIL_preference[MDI_ALLOW_LOOP_BRANCHING]=  DEFAULT_ALB;
   AIL_preference[MDI_DEFAULT_BEND_RANGE]=    DEFAULT_MDBR;

   AIL_preference[AIL_MM_PERIOD]=             DEFAULT_AMP;
   AIL_preference[AIL_TIMERS]=                DEFAULT_AT;

   AIL_preference[AIL_LOCK_PROTECTION]=       DEFAULT_ALP;
   AIL_preference[AIL_MUTEX_PROTECTION]=      DEFAULT_AMPR;

   AIL_preference[DLS_TIMEBASE]=            DEFAULT_DTB;
   AIL_preference[DLS_VOICE_LIMIT]=         DEFAULT_DVL;
   AIL_preference[DLS_BANK_SELECT_ALIAS]=   DEFAULT_DBSA;
   AIL_preference[DLS_STREAM_BOOTSTRAP]=    DEFAULT_DSB;
   AIL_preference[DLS_VOLUME_BOOST]=        DEFAULT_DVB;
   AIL_preference[DLS_ENABLE_FILTERING]=    DEFAULT_DEF;
   AIL_preference[AIL_ENABLE_MMX_SUPPORT]=  DEFAULT_AEMS;
   AIL_preference[DLS_GM_PASSTHROUGH]=      DEFAULT_DGP;

#ifdef OLD_DLS_REVERB_PREFERENCES
   AIL_preference[DLS_ENABLE_GLOBAL_REVERB]=DEFAULT_DEGR;
   AIL_preference[DLS_GLOBAL_REVERB_LEVEL]= DEFAULT_GRL;
   AIL_preference[DLS_GLOBAL_REVERB_TIME]=  DEFAULT_GRT;
#endif

   AIL_preference[DLS_ADPCM_TO_ASI_THRESHOLD]=  DEFAULT_DATAT;


   AIL_error[0]   = 0;
}

//############################################################################
//##                                                                        ##
//## Initialize AIL API modules and resources                               ##
//##                                                                        ##
//## Must be called prior to any other AIL_...() calls!                     ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_startup(void)
{
   init_preferences();

   //
   // Load the registered static providers
   //

   RIB_load_static_provider_library(MIX_RIB_MAIN,"Miles Mixer");

   //
   // Load and initialize external MSS RIBs
   //

   HPROVIDER PROVIDER;
   HPROENUM  next;

   //
   // Load and start all available mixer providers
   //

   RIB_load_application_providers("*.mix");

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("MSS mixer services",
                                  &next,
                                  &PROVIDER))
      {
      MIXER_STARTUP MIXER_startup;

      if (RIB_request_interface_entry(PROVIDER,
                                     "MSS mixer services",
                                      RIB_FUNCTION,
                                     "MIXER_startup",
                         (U32 FAR *) &MIXER_startup) == RIB_NOERR)
         {
         MIXER_startup();
         }
      }

   //
   // Load and start all available ASI providers
   //

#ifdef IS_WIN32
   RIB_load_application_providers("*.asi");
#else
  #ifdef IS_WIN16
    RIB_load_application_providers("*.as6");
  #endif
#endif

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("ASI codec",
                                  &next,
                                  &PROVIDER))
      {
      ASI_STARTUP ASI_startup;

      if (RIB_request_interface_entry(PROVIDER,
                                     "ASI codec",
                                      RIB_FUNCTION,
                                     "ASI_startup",
                         (U32 FAR *) &ASI_startup) == RIB_NOERR)
         {
         ASI_startup();
         }
      }

   //
   // Load and start all available M3D providers
   //

#ifdef IS_32
   RIB_load_application_providers("*.m3d");

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("MSS 3D audio services",
                                  &next,
                                  &PROVIDER))
      {
      M3D_STARTUP M3D_startup;

      if (RIB_request_interface_entry(PROVIDER,
                                     "MSS 3D audio services",
                                      RIB_FUNCTION,
                                     "M3D_startup",
                         (U32 FAR *) &M3D_startup) == RIB_NOERR)
         {
         M3D_startup();
         }
      }
#endif

   //
   // Load and start all available FLT providers
   //

#ifdef IS_32
   FLT_init_list();    // Initialize driver-association list so we can clean up at shutdown time

   RIB_load_application_providers("*.flt");

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("MSS pipeline filter",
                                  &next,
                                  &PROVIDER))
      {
      FLT_STARTUP FLT_startup;

      if (RIB_request_interface_entry(PROVIDER,
                                     "MSS pipeline filter",
                                      RIB_FUNCTION,
                                     "FLT_startup",
                         (U32 FAR *) &FLT_startup) == RIB_NOERR)
         {
         FLT_startup();
         }
      }
#endif
}

//############################################################################
//##                                                                        ##
//## Background timer server                                                ##
//##                                                                        ##
//############################################################################

U32 lastapitimerms=0;
static S32 timer_busy = 0;
static U32 highest_timer_delay=0;

void AILEXPORT API_timer (UINT  IDEvent,                                    //)
                          UINT  uReserved,
                          DWORD dwUser,
                          DWORD dwReserved1,
                          DWORD dwReserved2)
{
   static S32 i;
   static S32 lock_protect;
   U32 tme,diff;

   Only16Push32s();

   tme=AIL_ms_count();

   //
   // We want timer callback functions to behave like DOS interrupt handlers;
   // i.e., the application thread, once interrupted, should not execute
   // again until the callback function has returned.  So we'll suspend
   // the application thread (the one that called AIL_startup()) until we're
   // done.
   //

   lock_protect = AIL_preference[AIL_LOCK_PROTECTION];

#ifdef IS_WIN32API
   if (lock_protect)
      {
      if (SuspendThread(hAppThread) == 0xffffffff)
         {
         goto justexit;
         }
      }
#endif

   //
   // If timer services uninitialized or locked, or reentry attempted, exit
   //

   MSSLockedIncrement(timer_busy);


   if ((timers == NULL) || (lock_count > 0) || (timer_busy!=1))
      {
      goto resumethreadandexit;
      }

   //
   // Increment background count
   //

   MSSLockedIncrement(AIL_bkgnd_flag);

   //
   // Advance all running timers
   //

   if (lastapitimerms==0) {
     diff=timer_period;
     lastapitimerms=tme;
   } else {
     diff=tme-lastapitimerms;

     if (diff>highest_timer_delay)
       highest_timer_delay=diff;

     if (diff>100)
       diff=100;
     diff*=1000;
   }
   lastapitimerms=tme;

   for (i=0; i < n_timers; i++)
      {
      //
      // Skip timer if not active
      //

      if (timers[i].status != AILT_RUNNING)
         {
         continue;
         }

      //
      // Add base MME timer period to timer's accumulator
      //

      timers[i].elapsed += diff;

      //
      // If elapsed time >= timer's period, reset timer and
      // trigger callback function
      //

      while (timers[i].elapsed >= timers[i].value)
         {
         timers[i].elapsed -= timers[i].value;

         //
         // Invoke timer callback function with specified user value
         //

         #ifndef IS_WIN32API
         backgroundCT=timers[i].callingCT;
         #endif

         MSS_do_cb1( (AILTIMERCB), timers[i].callback, timers[i].callingDS, timers[i].user);

         // check again, in case they canceled the time in the background
         if (timers[i].status != AILT_RUNNING)
         {
           break;
         }

         }
      }

   #ifndef IS_WIN32API
   backgroundCT=-2;
   #endif

   //
   // Decrement background count
   //

   MSSLockedDecrement(AIL_bkgnd_flag);

   //
   // Resume application thread ("interrupt" handling finished)
   //

 resumethreadandexit:

   //
   // Enable future timer calls
   //

   MSSLockedDecrement(timer_busy);

#ifdef IS_WIN32API
   if (lock_protect)
      {
      ResumeThread(hAppThread);
      }
  justexit:
#endif

   Only16Pop32s();
}

#ifdef IS_WIN32API

//############################################################################
//##                                                                        ##
//## Thread to call timer services                                          ##
//##                                                                        ##
//############################################################################

static HANDLE thread_hand=0;
static HANDLE thread_wait;
DWORD WINAPI TMR_thread(LPVOID user)
{
  HANDLE both[2];
  both[0]=thread_wait;
  both[1]=MilesMutex;

  while (WaitForSingleObject(thread_wait,AIL_preference[AIL_MM_PERIOD])==WAIT_TIMEOUT) {
    if (AIL_preference[AIL_MUTEX_PROTECTION]==0)
    {
      API_timer(0,0,0,0,0);
    }
    else if (WaitForSingleObject(MilesMutex,1)==WAIT_OBJECT_0)
    {
     do_mutex_callback:
      #ifdef DEBUGMUTEX
        MilesCurThread=GetCurrentThreadId();
        MSSLockedIncrement(MilesCount);
        AIL_debug_printf("%X taken (%i)\n",MilesCurThread,MilesCount);
      #endif
      API_timer(0,0,0,0,0);
      #ifdef DEBUGMUTEX
        MSSLockedDecrement(MilesCount);
        AIL_debug_printf("%X relsd (%i)\n",MilesCurThread,MilesCount);
      #endif
      ReleaseMutex(MilesMutex);
    }
    else
    {
      // wait for either a close or the first moment the mutex is available
      DWORD wh=WaitForMultipleObjects(2,both,0,INFINITE);
      if (wh==WAIT_OBJECT_0)
        break;

      if (wh==(WAIT_OBJECT_0+1))
      {
        goto do_mutex_callback;
      }

    }

  }

  return(0);
}

#endif

//############################################################################
//##                                                                        ##
//## Shut down AIL API modules and resources                                ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_shutdown(void)
{
   HDIGDRIVER dig,d;
   HMDIDRIVER mdi,m;
   S32 i;

   //
   // Shut down any active MIDI drivers
   //

   mdi = MDI_first;

   while (mdi != NULL)
      {
      m = mdi->next;

#ifdef IS_WIN16
      if (ok_CT(mdi->callingCT))
#endif
        AIL_close_XMIDI_driver( mdi );

      mdi = m;
      }

   //
   // Shut down all M3D providers
   //

   HPROVIDER PROVIDER;
   HPROENUM next;

#ifdef IS_WIN32API

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("MSS 3D audio services",
                                  &next,
                                  &PROVIDER))
      {
      M3D_SHUTDOWN M3D_shutdown;

      if (RIB_request_interface_entry(PROVIDER,
                                     "MSS 3D audio services",
                                      RIB_FUNCTION,
                                     "M3D_shutdown",
                         (U32 FAR *) &M3D_shutdown) == RIB_NOERR)
         {
         M3D_shutdown();
         }
      }
#endif

   //
   // Shut down any active wave drivers
   //

   AIL_lock();

   dig = DIG_first;

   while (dig != NULL)
      {
      d = dig->next;

#ifdef IS_WIN16
      if (ok_CT(dig->callingCT))
#endif
        AIL_close_digital_driver( dig );

      dig = d;
      }

   //
   // Shut down all FLT providers
   //

#ifdef IS_WIN32API

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("MSS pipeline filter",
                                  &next,
                                  &PROVIDER))
      {
      FLT_SHUTDOWN FLT_shutdown;

      if (RIB_request_interface_entry(PROVIDER,
                                     "MSS pipeline filter",
                                      RIB_FUNCTION,
                                     "FLT_shutdown",
                         (U32 FAR *) &FLT_shutdown) == RIB_NOERR)
         {
         FLT_shutdown();
         }
      }
#endif

   //
   // Shut down all ASI providers
   //

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("ASI codec",
                                  &next,
                                  &PROVIDER))
      {
      ASI_SHUTDOWN ASI_shutdown;

      if (RIB_request_interface_entry(PROVIDER,
                                     "ASI codec",
                                      RIB_FUNCTION,
                                     "ASI_shutdown",
                         (U32 FAR *) &ASI_shutdown) == RIB_NOERR)
         {
         ASI_shutdown();
         }
      }

   //
   // Shut down all MIXER providers
   //

   next = HPROENUM_FIRST;

   while (RIB_enumerate_providers("MSS mixer services",
                                  &next,
                                  &PROVIDER))
      {
      MIXER_SHUTDOWN MIXER_shutdown;

      if (RIB_request_interface_entry(PROVIDER,
                                     "MSS mixer services",
                                      RIB_FUNCTION,
                                     "MIXER_shutdown",
                         (U32 FAR *) &MIXER_shutdown) == RIB_NOERR)
         {
         MIXER_shutdown();
         }
      }

   //
   // Release all loaded libraries
   //

   RIB_free_libraries();

   //
   // Shut down timer services
   //

   if (timers != NULL)
      {
      AIL_release_all_timers();

      for (i=0; i < n_timers; i++)           // don't free the timer system
        if (timers[i].status != AILT_FREE)   // if any timers remain
          goto done;

#ifdef IS_WIN32API

      //
      // Stop background thread
      //

      if (thread_hand) {
        SetEvent(thread_wait);
        WaitForSingleObject(thread_hand,INFINITE);
        CloseHandle(thread_hand);
        CloseHandle(thread_wait);
        thread_hand=0;
      }

#else

      //
      // Stop multimedia timer
      //

      timeKillEvent((UINT) timer_ID);

      timeEndPeriod((UINT) (timer_period / 1000));

#endif

      //
      // Free timer array
      //

      AIL_mem_free_lock(timers);

      timers = NULL;

      }

  done:
   AIL_unlock();
}

//###########################################################################
//##                                                                        ##
//## Lock/unlock AIL timer service (to enable atomic operations)            ##
//##                                                                        ##
//############################################################################

DXDEF
void    AILEXPORT AIL_lock                      (void)
{
   MSSLockedIncrement(lock_count);
}

DXDEF
void    AILEXPORT AIL_unlock                    (void)
{
   MSSLockedDecrement(lock_count);
}

DXDEF
void    AILEXPORT AIL_lock_mutex                (void)
{
  InMilesMutex();
}

DXDEF
void    AILEXPORT AIL_unlock_mutex              (void)
{
  OutMilesMutex();
}

//############################################################################
//##                                                                        ##
//## System-independent delay in 1/60 second intervals                      ##
//##                                                                        ##
//## Returns at once if called from background                              ##
//##                                                                        ##
//############################################################################

void    AILCALL AIL_API_delay                     (S32         intervals)
{
   //
   // Called from Win16 only -- Win32 version uses sleep()
   //

   if (AIL_bkgnd_flag)
      {
      return;
      }

#ifdef IS_WIN32API
   Sleep(16*intervals);
#else
   {
   
   U32 last_tick = AIL_ms_count()+(16*intervals);

   while (AIL_ms_count()<last_tick)
     ;
   }
#endif
}

//############################################################################
//##                                                                        ##
//## Returns TRUE if called from within timer handler or callback function  ##
//##                                                                        ##
//############################################################################

S32     AILCALL AIL_API_background                (void)
{
   return AIL_bkgnd_flag;
}


//############################################################################
//##                                                                        ##
//## Register a timer                                                       ##
//##                                                                        ##
//############################################################################

HTIMER  AILCALL AIL_API_register_timer            (AILTIMERCB    fn)
{
   S32 i;

   //
   // If timer array has not yet been allocated, allocate and initialize it
   //

   if (timers == NULL)
      {
      n_timers = AIL_preference[AIL_TIMERS];

      timers = (struct _AILTIMER FAR *) AIL_mem_alloc_lock(n_timers * sizeof(struct _AILTIMER));

      if (timers == NULL)
         {
         return -1;
         }

      //
      // Mark all timers free
      //

      for (i=0; i < n_timers; i++)
         {
         timers[i].status = AILT_FREE;

         }

#ifdef IS_WIN32API

     //
     // Start background thread to check for foreground service
     //

     thread_wait=CreateEvent(0,TRUE,0,0);

     thread_hand=CreateThread(0,65536,TMR_thread,0,0,(LPDWORD)&i);

#ifdef IS_WIN32
     Set_thread_name( i, "MSS Timer" );
#endif

     SetThreadPriority( thread_hand, THREAD_PRIORITY_TIME_CRITICAL);

#else

      //
      // Set up multimedia timer callback
      //

      timer_period = AIL_preference[AIL_MM_PERIOD] * 1000;

      if (timeBeginPeriod((UINT) (timer_period / 1000)) != 0)
         {
         return -1;
         }

      timer_ID = timeSetEvent((UINT) (timer_period / 1000),
                                      0,
                                      API_timer,
                                      0,
                                      TIME_PERIODIC);

      if (timer_ID == 0)
         {
         return -1;
         }

#endif


      }

   //
   // Find a free timer, if possible, and return its handle
   //

   for (i=0; i < n_timers; i++)
      {
      if (timers[i].status == AILT_FREE)
         {
         break;
         }
      }

   //
   // If no free timers, return -1
   //

   if (i == n_timers)
      {
      return -1;
      }

   //
   // Otherwise, mark timer "stopped" and record callback address
   //

   timers[i].status = AILT_STOPPED;

   timers[i].callback = fn;

   #ifdef IS_WIN16
     MSSGetTaskInfo(&timers[i].callingCT,&timers[i].callingDS);
   #endif

   //
   // Set default rate of 100 Hz
   //

   timers[i].value   = 10000;
   timers[i].elapsed = 0;

   return i;
}

//############################################################################
//##                                                                        ##
//## Set timer user word                                                    ##
//##                                                                        ##
//############################################################################

U32     AILCALL AIL_API_set_timer_user            (HTIMER      timer,      //)
                                                   U32         user)
{
   U32 temp;

   if (timer == -1)
      {
      return 0;
      }

   temp = timers[timer].user;

   timers[timer].user = user;

   return temp;
}

//############################################################################
//##                                                                        ##
//## Set timer period in microseconds, frequency in hertz, or equivalent    ##
//## interrupt divisor value                                                ##
//##                                                                        ##
//############################################################################

void    AILCALL AIL_API_set_timer_period          (HTIMER      timer,       //)
                                                   U32         microseconds)
{
   if (timer == -1)
      {
      return;
      }

   //
   // Begin atomic operation
   //

   AIL_lock();

   //
   // Reset timer and set new period in microseconds
   //

   timers[timer].elapsed = 0;
   timers[timer].value   = microseconds;

   //
   // End atomic operation
   //

   AIL_unlock();
}


void    AILCALL AIL_API_set_timer_frequency       (HTIMER      timer,      //)
                                                   U32         hertz)
{
   if (timer == -1)
      {
      return;
      }

   AIL_set_timer_period(timer,
                        1000000 / hertz);
}


void    AILCALL AIL_API_set_timer_divisor         (HTIMER      timer,      //)
                                                   U32         PIT_divisor)
{
   if (timer == -1)
      {
      return;
      }

   //
   // Ensure 100% precision with zero case
   //

   if (PIT_divisor == 0)
      {
      AIL_set_timer_period(timer,
                           54925);
      }
   else
      {
      AIL_set_timer_period(timer,
                           (PIT_divisor * 10000) / 11932);
      }
}

//############################################################################
//##                                                                        ##
//## Start timer(s)                                                         ##
//##                                                                        ##
//############################################################################

void    AILCALL AIL_API_start_timer               (HTIMER      timer)
{
   if (timer == -1)
      {
      return;
      }

   if (timers[timer].status == AILT_STOPPED)
      {
      timers[timer].status = AILT_RUNNING;
      }
}

void    AILCALL AIL_API_start_all_timers          (void)
{
   S32 i;

   for (i=0; i < n_timers; i++)
      {

#ifdef IS_WIN16
      if (ok_CT(timers[i].callingCT))
#endif

      AIL_start_timer(i);
      }
}


//############################################################################
//##                                                                        ##
//## Stop timer(s)                                                          ##
//##                                                                        ##
//############################################################################

void    AILCALL AIL_API_stop_timer                (HTIMER      timer)
{
   if (timer == -1)
      {
      return;
      }

   if (timers[timer].status == AILT_RUNNING)
      {
      timers[timer].status = AILT_STOPPED;
      }

}

void    AILCALL AIL_API_stop_all_timers           (void)
{
   S32 i;

   for (i=0; i < n_timers; i++)
      {

#ifdef IS_WIN16
      if (ok_CT(timers[i].callingCT))
#endif

      AIL_stop_timer(i);
      }
}


//############################################################################
//##                                                                        ##
//## Release timer handle(s)                                                ##
//##                                                                        ##
//############################################################################

void    AILCALL AIL_API_release_timer_handle      (HTIMER      timer)
{
   if (timer == -1)
      {
      return;
      }

   timers[timer].status = AILT_FREE;
}

void    AILCALL AIL_API_release_all_timers        (void)
{
   S32 i;

   for (i=0; i < n_timers; i++)
      {

#ifdef IS_WIN16
      if (ok_CT(timers[i].callingCT))
#endif

      AIL_release_timer_handle(i);
      }
}


DXDEF U32 AILEXPORT AIL_get_timer_highest_delay   (void)
{
  if (highest_timer_delay<(U32)AIL_preference[AIL_MM_PERIOD])
    return(0);
  else
  {
    U32 ret=highest_timer_delay-AIL_preference[AIL_MM_PERIOD];
    highest_timer_delay=0;
    return( ret );
  }
}


//############################################################################
//##                                                                        ##
//## Return HWND used by MSS                                                ##
//##                                                                        ##
//############################################################################

#ifdef IS_WIN32

HWND AILCALL AIL_API_HWND(void)
{
   DWORD curproc = GetCurrentProcessId();
   S32   curmod  = (S32) GetModuleHandle(NULL);
   DWORD proc;
   HWND  scan;

   if ((MSShWnd==NULL) || (!IsWindow(MSShWnd))) {

     MSShWnd=NULL;

     scan = GetTopWindow(0);

     while (scan)
        {
        GetWindowThreadProcessId(scan, &proc);

        if (proc == curproc) {

           // make sure the module matches and the window isn't a child

           if (GetWindowLong(scan, GWL_HINSTANCE) == curmod) {
             if ((GetWindowLong(scan,GWL_STYLE)&WS_CHILD)==0)
               MSShWnd=scan;
           }

           // however, if we haven't found anything, then save the window
           if ((MSShWnd==0) && (!(GetWindowLong(scan,GWL_STYLE) & WS_DISABLED)))
             MSShWnd=scan;

        }

        scan = GetWindow(scan, GW_HWNDNEXT);
        }

     //
     // If no window found, use active window or foreground window
     //

     if (MSShWnd==0)
       MSShWnd=GetActiveWindow();

     if (MSShWnd==0)
       MSShWnd=GetForegroundWindow();
   }

   return MSShWnd;
}

#endif

//############################################################################
//##                                                                        ##
//## Error text handling routines                                           ##
//##                                                                        ##
//############################################################################


void AILCALL AIL_API_set_error(C8 const FAR * error_msg)
{
  AIL_strcpy(AIL_error,error_msg);
}


C8 FAR * AILCALL AIL_API_last_error(void)
{
   return(AIL_error);
}

#ifdef IS_WINDOWS

S32 check_dirs(HINSTANCE hInstance)
{
  int i;
  char buf[272];

  // check to make sure the DLL isn't incorrectly installed

  GetModuleFileName(hInstance,MSS_Directory,271);
  for(i=AIL_strlen(MSS_Directory);i;i--) {
    if ((MSS_Directory[i]=='\\') || (MSS_Directory[i]==':'))
      break;
  }
  MSS_Directory[i]=0;

  GetWindowsDirectory(buf,271);
  if (AIL_strnicmp(MSS_Directory,buf,271)==0) {
   err:
    MessageBox(0,"The MSS DLL file is installed in the Windows or Windows system directory - it must be installed in the application directory.","Miles Error",MB_OK|MB_ICONSTOP);
    return(0);
  }
  GetSystemDirectory(buf,271);
  if (AIL_strnicmp(MSS_Directory,buf,271)==0) {
    goto err;
  }

  return(1);
}

#endif

//############################################################################
//##                                                                        ##
//## DLL routines                                                           ##
//##                                                                        ##
//############################################################################

#ifdef IS_WIN32

extern "C"
{
extern BOOL Win32Init(HINSTANCE hinstDll);
}


extern "C"
#ifdef MSSXTRA
DXDEC BOOL WINAPI MSSDllMain(
#else
DXDEC BOOL WINAPI DllMain(
#endif
                          HINSTANCE hinstDll,
                          DWORD fdwRreason,
                          LPVOID plvReserved)
{
  MSS_Instance=hinstDll;
  switch (fdwRreason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls( hinstDll );
      init_preferences();
      if (check_dirs(hinstDll)==0)
        return(0);
      return(Win32Init(hinstDll));
  }
  return TRUE;
}

#else

#ifdef IS_WIN16

extern "C"
{
void AILCALL Win16Init();
}


extern "C"
#ifdef MSSXTRA
BOOL __far __pascal MSSLibMain(
#else
BOOL __far __pascal LibMain(
#endif
     HANDLE hInstance, WORD wDataSegment, WORD wHeapSize, LPSTR lpszCmdLine )
{
  wDataSegment=wDataSegment;
  wHeapSize=wHeapSize;
  lpszCmdLine=lpszCmdLine;

  MSS_Instance=(HINSTANCE)hInstance;

  Win16Init();
  return((BOOL)check_dirs((const struct ::HINSTANCE__ __near *) hInstance));

}

int CALLBACK WEP(int exit)
{
  backgroundCT=-1;
  AIL_shutdown();
  return 1;
}

#endif

#endif


