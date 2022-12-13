#ifndef LENGUINS_H
#define LENGUINS_H

#include <NiApplication.h>
#include "Task.h"

#ifdef WIN32
#include <NiD3DDefines.h>
class NiD3DShaderLibrary;
#endif

#include "customculler.h"
class ShaderHelper;
class IUIRenderer;
class CScene;

class GameMain : public NiApplication
{
public:
    GameMain();

    virtual bool Initialize();
    virtual void OnIdle();
    virtual void Terminate();
    virtual void ResetFrameTimings();
	virtual bool MeasureTime();

	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void	OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static unsigned int GetScreenWidth();
    static unsigned int GetScreenHeight();

    void ToggleShowAllTrackers();
    void UpdateFrameRate();

    // Adders for Visual Tracker info
    void AddUpdateTime(float fTime);
    void AddCullTime(float fTime);
    void AddRenderTime(float fTime);
    void AddAITime(float fTime);
    void AddAnimTime(float fTime);
    void AddVisObjects(float fCount);

	CCustomCullingProcess & GetCuller();
	NiCullingProcess & GetDefaultCuller();
	NiAccumulator*	GetSorter();
	CScene*	GetActiveScene();
	NiVisibleArray & GetVisibleArray(){return m_kVisible;}


	static bool IsDeviceLost();
	
	//shader factory
	ShaderHelper* m_pkShaderHelper;
	static bool m_bOnlostDevice;
public:
	static unsigned int ms_uiScreenWidth;
	static unsigned int ms_uiScreenHeight;
	NiNodePtr			m_spLogo; //如果不先调用Gamebryo绘制，那么在绘制UI的时候就会灰屏

protected:

	void LoadConfig();

    virtual bool CreateSceneMgr();
    virtual bool CreateRenderer();
    virtual bool CreateVisualTrackers();
    // bool CreateSound();

    void Reset();

    void DisplaySplash(const char* pcName);
    virtual void UpdateFrame();
	virtual void RenderFrame();

	bool ChangeDeviceSetting();

    NiScreenTexturePtr m_spDemoScreen;
    NiScreenTexturePtr m_spNDLLogo;
    NiScreenTexturePtr m_spGamebryoLogo;

	bool m_bDebug;

    float m_fAITime;
    float m_fAnimTime;
    float m_fVisableObjects;

	bool m_bKeyF7;
	bool m_bKeyF8;

	IUIRenderer * m_pkUIRender;

	CCustomCullingProcess	m_kCustomCuller;
	
#ifdef WIN32
    static NiD3DShaderLibrary* LibraryClassCreate(const char* pszLibFile, 
        NiD3DRenderer* pkRenderer, int iDirectoryCount, 
        char* pszDirectories[], bool bRecurseSubFolders);
    static unsigned int RunParser(const char* pszLibFile, 
        NiD3DRenderer* pkRenderer, const char* pszDirectory, 
        bool bRecurseSubFolders);
    static unsigned int ShaderErrorCallback(const char* pacError, 
        NiD3DError eError, bool bRecoverable);
    static bool CardMatchesShaderVersion();

    // Shader support
    static char* ms_apszShaderParsers[];
    static char* ms_apszShaderLibraries[];
	static bool ms_bShaderError;
#endif // #ifdef WIN32
};

#include "GameMain.inl"

#endif // LENGUINS_H
