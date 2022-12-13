
#include "Top-KartPCH.h"

#include "ShaderHelper.h"

// DLL shader libraries not supported
#define _LIB
    #include <NSBShaderLib.h>
    #include <NSFParserLib.h>
    #include <NiD3DXEffectShaderLib.h>
#if defined(_XENON)
    #include <NiFXLShaderLib.h>
#endif
#undef _LIB

#if defined(_XENON)
    #pragma comment(lib, "NiBinaryShaderLib.lib")
    #pragma comment(lib, "NSBShaderLib.lib")
    #pragma comment(lib, "NSFParserLib.lib")
    #pragma comment(lib, "NiD3DXEffectShaderLib.lib")
    #pragma comment(lib, "NiFXLShaderLib.lib")
#elif defined(_DX9)
    #pragma comment(lib, "NiBinaryShaderLibDX9.lib")
    #pragma comment(lib, "NSBShaderLibDX9.lib")
    #pragma comment(lib, "NSFParserLibDX9.lib")
  //  #pragma comment(lib, "NiD3DXEffectShaderLibDX9.lib")
#endif

//---------------------------------------------------------------------------
bool ShaderHelper::SetupShaderSystem(
    char* apcProgramDirectory[], unsigned int uiNumProgramDirectories, 
    char* apcShaderDirectories[], unsigned int uiNumShaderDirectories)
{
    assert (NiD3DShaderProgramFactory::GetInstance());

    for (unsigned int i = 0; i < uiNumProgramDirectories; i++)
    {
        NiD3DShaderProgramFactory::GetInstance()->AddProgramDirectory(
            apcProgramDirectory[i]);
    }

    m_uiShaderDirectoryCount = uiNumShaderDirectories;
    m_ppcShaderDirectories = apcShaderDirectories;

    // First, we will run the NSF parser. This is done to make sure that
    // any text-based shader files that have been modified are re-compiled
    // to binary before loading all the binary representations.
    if (!RunShaderParsers())
    {
        NiMessageBox("Failed to run shader parsers!", "ERROR");
        return false;
    }

    if (!RegisterShaderLibraries())
    {
        NiMessageBox("Failed to register shader libraries!", "ERROR");
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool ShaderHelper::RunShaderParsers()
{
    NiShaderFactory::RegisterRunParserCallback(NSFRunParser);

    for (unsigned int i = 0; i < m_uiShaderDirectoryCount; i++)
    {
        unsigned int uiCount = NiShaderFactory::LoadAndRunParserLibrary(
            NULL, m_ppcShaderDirectories[i], true);

        if (uiCount == 0)
        {
            NiOutputDebugString("NSF parser library failed to parse "
                "any shaders!");
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool ShaderHelper::RegisterShaderLibraries()
{
    NiShaderFactory::RegisterClassCreationCallback(NSBLibraryClassCreate);

    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(NULL, 
        m_uiShaderDirectoryCount, m_ppcShaderDirectories, true))
    {
        NiOutputDebugString("NSB shader library failed to load any shaders!");
    }

    NiShaderFactory::RegisterClassCreationCallback(FXLibraryClassCreate);

    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(NULL, 
        m_uiShaderDirectoryCount, m_ppcShaderDirectories, true))
    {
        NiOutputDebugString("FX shader library failed to load any shaders!");
    }


#if defined(_XENON)
    NiShaderFactory::RegisterClassCreationCallback(FXLiteLibraryClassCreate);

    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(NULL, 
        m_uiShaderDirectoryCount, m_ppcShaderDirectories, true))
    {
        NiOutputDebugString("FXL shader library failed to load any shaders!");
    }
#endif

    return true;
}
//---------------------------------------------------------------------------
bool ShaderHelper::NSBLibraryClassCreate(const char* pcLibFile, 
    NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], 
    bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary)
{
    *ppkLibrary = NULL;

    // Create the NSB Shader Library...
    return NSBShaderLib_LoadShaderLibrary(pkRenderer, iDirectoryCount, 
        apcDirectories, bRecurseSubFolders, ppkLibrary);
}
//---------------------------------------------------------------------------
bool ShaderHelper::FXLibraryClassCreate(const char* pcLibFile, 
    NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], 
    bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary)
{
    *ppkLibrary = NULL;

	return true;
    // Create the Shader Library...
  /*  return NiD3DXEffectShaderLib_LoadShaderLibrary(pkRenderer, 
        iDirectoryCount, apcDirectories, bRecurseSubFolders, ppkLibrary);*/
}
//---------------------------------------------------------------------------
bool ShaderHelper::FXLiteLibraryClassCreate(const char* pcLibFile, 
    NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], 
    bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary)
{
    *ppkLibrary = NULL;

#if defined(_XENON)
    // Create the FX Shader Library.
    return NiFXLShaderLib_LoadShaderLibrary(pkRenderer, 
        iDirectoryCount, apcDirectories, bRecurseSubFolders, ppkLibrary);
#else
    return false;
#endif
}
//---------------------------------------------------------------------------
unsigned int ShaderHelper::NSFRunParser(const char* pcLibFile, 
    NiRenderer* pkRenderer, const char* pcDirectory, 
    bool bRecurseSubFolders)
{
    return NSFParserLib_RunShaderParser(pcDirectory, bRecurseSubFolders);
}
//---------------------------------------------------------------------------
