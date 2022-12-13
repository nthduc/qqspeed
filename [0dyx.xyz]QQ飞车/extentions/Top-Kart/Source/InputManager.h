// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <NiApplication.h>
//#include "LevelManager.h"
#include "KeyEventManager.h"

class InputManager : public NiMemObject
{
public:
    static bool Create();
    static void Destroy();

    static InputManager* Get();

    void GameUpdate(const NiMatrix3& kRotation);
    void MenuUpdate();
    bool Initialize();
	void Enable(bool bEnable);

    const float& GetStrafeDir() const;
    const NiPoint2& GetRelativeDir() const;
    const NiPoint2& GetCorrectedDir() const;
    const NiPoint2& GetCameraDir() const;
    float GetZoomFactor() const;
    NiInputSystem* GetInputSystem() const;
	static bool keys[256];
	static bool m_bIsSpeedUp;
protected:
    InputManager();
    ~InputManager();

    void PollGameInput(float fSin, float fCos);
    void PollMenuInput();
    float AdjustToAxisRange(float fAxisValue);

    class InputData
    {
    public:
        InputData();

        float m_fStrafeDir;    //Currently only used to Strafe 
        NiPoint2 m_kRelativeDir;
        NiPoint2 m_kA1Dir;
        NiPoint2 m_kA2Dir;
        float m_fZoomVel;
    };

    InputData m_kInputData;
    NiInputSystemPtr m_spInputSystem;
    float m_fAxisLowRangeOffset;
    float m_fAxisTotalRange;
	bool      m_bEnable;
    static float ms_fLastInputTime;
    static const float ms_fInputDelay;
    static const float ms_fInputDelayThreshold;
    static InputManager* ms_pkTheInputManager;


	//-----------------------------------------------------------------------
	
	ZKeyEventManager * m_pEventMgr;

	//-----------------------------------------------------------------------
	

};



#include "InputManager.inl"

#endif // INPUTMANAGER_H
