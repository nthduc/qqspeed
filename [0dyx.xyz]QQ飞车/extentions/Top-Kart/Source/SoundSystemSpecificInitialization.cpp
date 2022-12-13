
#include "Top-KartPCH.h"

#include <NiApplication.h>

#ifndef NO_SOUND
// This block of code is necessary to do the AUDIO system 
// specific initialization. Each different audio system
// will require a unique implementation

//---------------------------------------------------------------------------
//
//                  MILES AUDIO SYSTEM
//
//---------------------------------------------------------------------------
#include <NiMilesAudioSDM.h>
#include <NiMilesAudio.h>

bool SoundSystemSpecificInitialization()
{
    //
    // Start the sound system
    //
    NiApplication* pkApp = NiApplication::ms_pkApplication;
    NiMilesAudioSystem* pkSS = 
        (NiMilesAudioSystem*)NiAudioSystem::GetAudioSystem();


    NiWindowRef pkWnd;
    if (pkApp->GetFullscreen())
        pkWnd = pkApp->GetWindowReference();
    else
        pkWnd = pkApp->GetRenderWindowReference();

    pkSS->SetHWnd(pkWnd);

    // Put the path to the Miles redist files here.
    if (!pkSS->Startup("Miles"))
    {
        char acStr[256];
        NiSprintf(acStr, 256, "Error starting Audio system\n %s", 
            NiAudioSystem::GetAudioSystem()->GetLastError());
        if (pkApp->GetFullscreen())
        {
            NiOutputDebugString(acStr);
        }
        else
        {
            NiMessageBox(acStr, "Error");
        }
        return false;
    }

    // Get a provider by iterating through the provider list looking first for
    // "DirectSound3D Hardware Support" (hardware accelerated) and, if it's
    // unavailable, then for "Miles Fast 2D Positional Audio" (software
    // emulated), as "best choice" defaults.  If neither provider is
    // available, report an error and return false.

    NiTListIterator kIter;
    NiProviderInfo* pkProvider = pkSS->GetFirstProvider(kIter);
    while (pkProvider)
    {
        if (strstr(pkProvider->GetProviderName(),
            "DirectSound3D Hardware Support"))
        {
            break;
        }
        pkProvider = pkSS->GetNextProvider(kIter);
    }

    if ((!pkProvider) || (!pkProvider->OpenProvider()))
    {
        pkProvider = pkSS->GetFirstProvider(kIter);
        while (pkProvider)
        {
            if (strstr(pkProvider->GetProviderName(),
                "Miles Fast 2D Positional Audio"))
            {
                break;
            }
            pkProvider = pkSS->GetNextProvider(kIter);
        }

        if ((!pkProvider) || (!pkProvider->OpenProvider()))
        {
            char acStr[256];
            NiSprintf(acStr, 256, "%s%s%s",
                "Error:  These two providers are not available:\n",
                "  DirectSound3D Hardware Support\n",
                "  Miles Fast 2D Positional Audio\n");
            if (pkApp->GetFullscreen())
            {
                NiMessageBox(acStr, "Error");
            }
            else
            {
                NiOutputDebugString(acStr);
            }
            return false;
        }
    }

//    assert(pkProvider);
	if(!pkProvider)
		return false;

    // Set the Default Provider to be used
    pkSS->SetDefaultProvider(pkProvider);

    // set up the listener with the selected provider
    pkSS->GetListener()->Startup(pkProvider);
    return true;
}



#else
bool SoundSystemSpecificInitialization()
{
	return false;
}
#endif
